// mqtt_topic_matcher.h
#ifndef __mqtt_topic_matcher_h
#define __mqtt_topic_matcher_h
#include <string>
#include "mqtt/types.h"
#include "mqtt/topic.h"
#include <forward_list>
#include <initializer_list>
#include <map>
#include <memory>
#include <vector>
#if __cplusplus < 201402L
namespace std {
template <class T>
struct _Unique_if {
    typedef unique_ptr<T> _Single_object;
};
template <class T>
struct _Unique_if<T[]> {
    typedef unique_ptr<T[]> _Unknown_bound;
};
template <class T, size_t N>
struct _Unique_if<T[N]> {
    typedef void _Known_bound;
};
template <class T, class... Args>
typename _Unique_if<T>::_Single_object make_unique(Args&&... args) {
    return unique_ptr<T>(new T(std::forward<Args>(args)...));
}
template <class T>
typename _Unique_if<T>::_Unknown_bound make_unique(size_t n) {
    typedef typename remove_extent<T>::type U;
    return unique_ptr<T>(new U[n]());
}
template <class T, class... Args>
typename _Unique_if<T>::_Known_bound make_unique(Args&&...) = delete;
}  // namespace std
#endif
namespace mqtt {
template <typename T>
class topic_matcher
{
public:
    using key_type = string;
    using mapped_type = T;
    using value_type = std::pair<key_type, mapped_type>;
    using reference = value_type;
    using const_reference = const value_type&;
    using value_ptr = std::unique_ptr<value_type>;
    using mapped_ptr = std::unique_ptr<mapped_type>;
private:
    struct node {
        using ptr_t = std::unique_ptr<node>;
        using map_t = std::map<string, ptr_t>;
        /** The value that matches the topic at this node, if any */
        value_ptr content;
        /** Child nodes mapped by the next field of the topic */
        map_t children;
        /** Creates a new, empty node */
        static ptr_t create() { return std::make_unique<node>(); }
        /** Determines if this node is empty (no content or children) */
        bool empty() const { return !content && children.empty(); }
        /** Removes the empty nodes under this one. */
        void prune() {
            for (auto& child : children) {
                child.second->prune();
            }
            for (auto child = children.cbegin(); child != children.cend();) {
                if (child->second->empty()) {
                    child = children.erase(child);
                }
                else {
                    ++child;
                }
            }
        }
    };
    using node_ptr = typename node::ptr_t;
    using node_map = typename node::map_t;
    /** The root node of the collection */
    node_ptr root_;
public:
    /** Generic iterator over all items in the collection. */
    class iterator
    {
        /** The last-found value */
        value_type* pval_;
        /** The nodes still to be checked, used as a stack */
        std::vector<node*> nodes_;
        void next() {
            // If there are no nodes left to search, we're done.
            if (nodes_.empty()) {
                pval_ = nullptr;
                return;
            }
            // Get the next node to search.
            auto snode = std::move(nodes_.back());
            nodes_.pop_back();
            // Push the children onto the stack for later
            for (auto const& child : snode->children) {
                nodes_.push_back(child.second.get());
            }
            // If there's a value in this node, use it;
            // otherwise keep looking.
            pval_ = snode->content.get();
            if (!pval_) this->next();
        }
        friend class topic_matcher;
        iterator(value_type* pval) : pval_{pval} {}
        iterator(node* root) : pval_{nullptr} {
            nodes_.push_back(root);
            next();
        }
    public:
        /**
         * Gets a reference to the current value.
         * @return A reference to the current value.
         */
        reference operator*() noexcept { return *pval_; }
        /**
         * Gets a const reference to the current value.
         * @return A const reference to the current value.
         */
        const_reference operator*() const noexcept { return *pval_; }
        /**
         * Get a pointer to the current value.
         * @return A pointer to the current value.
         */
        value_type* operator->() noexcept { return pval_; }
        /**
         * Get a const pointer to the current value.
         * @return A const pointer to the current value.
         */
        const value_type* operator->() const noexcept { return pval_; }
        /**
         * Postfix increment operator.
         * @return An iterator pointing to the previous matching item.
         */
        iterator operator++(int) noexcept {
            auto tmp = *this;
            this->next();
            return tmp;
        }
        /**
         * Prefix increment operator.
         * @return An iterator pointing to the next matching item.
         */
        iterator& operator++() noexcept {
            this->next();
            return *this;
        }
        /**
         * Compares two iterators to see if they don't refer to the same
         * node.
         *
         * @param other The other iterator to compare against this one.
         * @return @em true if they don't match, @em false if they do
         */
        bool operator!=(const iterator& other) const noexcept {
            return pval_ != other.pval_;
        }
    };
    /** A const iterator over all items in the collection. */
    class const_iterator : public iterator
    {
        using base = iterator;
        friend class topic_matcher;
        const_iterator(iterator it) : base(it) {}
    public:
        /**
         * Gets a const reference to the current value.
         * @return A const reference to the current value.
         */
        const_reference operator*() const noexcept { return base::operator*(); }
        /**
         * Get a const pointer to the current value.
         * @return A const pointer to the current value.
         */
        const value_type* operator->() const noexcept { return base::operator->(); }
    };
    /**
     * Iterator that efficiently searches the collection for topic
     * matches.
     */
    class match_iterator
    {
        /** Information about a node that needs to be searched. */
        struct search_node {
            /** The current node being searched. */
            node* node_;
            /** The fields of the topic still to be searched. */
            std::forward_list<string> fields_;
            /** Whether this is the first/root node */
            bool first_;
            search_node(node* nd, const std::forward_list<string>& sy, bool first = false)
                : node_{nd}, fields_{sy}, first_{first} {}
            search_node(node* nd, std::forward_list<string>&& sy, bool first = false)
                : node_{nd}, fields_{std::move(sy)}, first_{first} {}
        };
        /** The last-found value */
        value_type* pval_;
        /** The nodes still to be checked, used as a stack */
        std::vector<search_node> nodes_;
        /**
         * Move the next iterator to the next value, or to end(), if none
         * left.
         *
         * This will keep recursing until it finds a matching node that
         * contains a value or it reaches the end.
         */
        void next() {
            pval_ = nullptr;
            // If there are no nodes left to search, we're done.
            if (nodes_.empty()) return;
            // Get the next node to search.
            auto snode = std::move(nodes_.back());
            nodes_.pop_back();
            // If we're at the end of the topic fields, we either have a value,
            // or need to move on to the next node to search.
            if (snode.fields_.empty()) {
                pval_ = snode.node_->content.get();
                if (!pval_) this->next();
                return;
            }
            // Get the next field of the topic to search
            auto field = std::move(snode.fields_.front());
            snode.fields_.pop_front();
            typename node_map::iterator child;
            const auto map_end = snode.node_->children.end();
            // Look for an exact match
            if ((child = snode.node_->children.find(field)) != map_end) {
                nodes_.push_back({child->second.get(), snode.fields_});
            }
            // Topics starting with '$' don't match wildcards in the first field
            // https://docs.oasis-open.org/mqtt/mqtt/v5.0/os/mqtt-v5.0-os.html#_Toc3901246
            if (!snode.first_ || field.empty() || field[0] != '$') {
                // Look for a single-field wildcard match
                if ((child = snode.node_->children.find("+")) != map_end) {
                    nodes_.push_back({child->second.get(), snode.fields_});
                }
                // Look for a terminating match
                if ((child = snode.node_->children.find("#")) != map_end) {
                    // By definition, a '#' is a terminating leaf
                    pval_ = child->second->content.get();
                    return;
                }
            }
            this->next();
        }
        friend class topic_matcher;
        match_iterator() : pval_{nullptr} {}
        match_iterator(value_type* pval) : pval_{pval} {}
        match_iterator(node* root, const string& topic) : pval_{nullptr} {
            auto v = topic::split(topic);
            std::forward_list<string> fields{v.begin(), v.end()};
            nodes_.push_back(search_node{root, std::move(fields), true});
            next();
        }
    public:
        /**
         * Gets a reference to the current value.
         * @return A reference to the current value.
         */
        reference operator*() noexcept { return *pval_; }
        /**
         * Gets a const reference to the current value.
         * @return A const reference to the current value.
         */
        const_reference operator*() const noexcept { return *pval_; }
        /**
         * Get a pointer to the current value.
         * @return A pointer to the current value.
         */
        value_type* operator->() noexcept { return pval_; }
        /**
         * Get a const pointer to the current value.
         * @return A const pointer to the current value.
         */
        const value_type* operator->() const noexcept { return pval_; }
        /**
         * Postfix increment operator.
         * @return An iterator pointing to the previous matching item.
         */
        match_iterator operator++(int) noexcept {
            auto tmp = *this;
            this->next();
            return tmp;
        }
        /**
         * Prefix increment operator.
         * @return An iterator pointing to the next matching item.
         */
        match_iterator& operator++() noexcept {
            this->next();
            return *this;
        }
        /**
         * Compares two iterators to see if they don't refer to the same
         * node.
         *
         * @param other The other iterator to compare against this one.
         * @return @em true if they don't match, @em false if they do
         */
        bool operator!=(const match_iterator& other) const noexcept {
            return pval_ != other.pval_;
        }
    };
    /**
     * A const match iterator.
     */
    class const_match_iterator : public match_iterator
    {
        using base = match_iterator;
        friend class topic_matcher;
        const_match_iterator(match_iterator it) : base(it) {}
    public:
        /**
         * Gets a const reference to the current value.
         * @return A const reference to the current value.
         */
        const_reference operator*() const noexcept { return base::operator*(); }
        /**
         * Get a const pointer to the current value.
         * @return A const pointer to the current value.
         */
        const value_type* operator->() const noexcept { return base::operator->(); }
    };
    /**
     * Creates  new, empty collection.
     */
    topic_matcher() : root_(node::create()) {}
    /**
     * Creates a new collection with a list of key/value pairs.
     *
     * This can be used to create a connection from a table of entries, as
     * key/value pairs, like:
     *
     *     topic_matcher<int> matcher {
     *         { "#", -1 },
     *         { "some/random/topic", 42 },
     *         { "some/#", 99 }
     *     }
     *
     * @param lst The list of key/value pairs to populate the collection.
     */
    topic_matcher(std::initializer_list<value_type> lst) : root_(node::create()) {
        for (const auto& v : lst) {
            insert(v);
        }
    }
    /**
     * Determines if the collection is empty.
     * @return @em true if the collection is empty, @em false if it contains
     *         any filters.
     */
    bool empty() const { return root_.empty(); }
    /**
     * Inserts a new key/value pair into the collection.
     * @param val The value to place in the collection.
     */
    void insert(value_type&& val) {
        auto nd = root_.get();
        auto fields = topic::split(val.first);
        for (const auto& field : fields) {
            auto it = nd->children.find(field);
            if (it == nd->children.end()) {
                nd->children[field] = node::create();
                it = nd->children.find(field);
            }
            nd = it->second.get();
        }
        nd->content = std::make_unique<value_type>(std::move(val));
    }
    /**
     * Inserts a new value into the collection.
     * @param key The topic/filter entry
     * @param val The value to associate with that entry.
     */
    void insert(const value_type& val) {
        value_type v{val};
        this->insert(std::move(v));
    }
    /**
     * Removes an entry from the collection.
     *
     * This removes the value from the internal node, but leaves the node in
     * the collection, even if it is empty.
     * @param filter The topic filter to remove.
     * @return A unique pointer to the value, if any.
     */
    mapped_ptr remove(const key_type& filter) {
        auto nd = root_.get();
        auto fields = topic::split(filter);
        for (auto& field : fields) {
            auto it = nd->children.find(field);
            if (it == nd->children.end()) return mapped_ptr{};
            nd = it->second.get();
        }
        value_ptr valpair;
        nd->content.swap(valpair);
        return (valpair) ? std::make_unique<mapped_type>(valpair->second) : mapped_ptr{};
    }
    /**
     * Removes the empty nodes in the collection.
     */
    void prune() { root_->prune(); }
    /**
     * Gets an iterator to the full collection of filters.
     * @return An iterator to the full collection of filters.
     */
    iterator begin() { return iterator{root_.get()}; }
    /**
     * Gets an iterator to the end of the collection of filters.
     * @return An iterator to the end of collection of filters.
     */
    iterator end() { return iterator{static_cast<value_type*>(nullptr)}; }
    /**
     * Gets an iterator to the end of the collection of filters.
     * @return An iterator to the end of collection of filters.
     */
    const_iterator end() const noexcept {
        return const_iterator{static_cast<value_type*>(nullptr)};
    }
    /**
     * Gets a const iterator to the full collection of filters.
     * @return A const iterator to the full collection of filters.
     */
    const_iterator cbegin() const { return const_iterator{root_.get()}; }
    /**
     * Gets a const iterator to the end of the collection of filters.
     * @return A const iterator to the end of collection of filters.
     */
    const_iterator cend() const noexcept { return end(); }
    /**
     * Gets a pointer to the value at the requested key.
     * @param filter The topic filter entry to find.
     * @return An iterator to the value if found, @em end() if not found.
     */
    iterator find(const key_type& filter) {
        auto nd = root_.get();
        auto fields = topic::split(filter);
        for (auto& field : fields) {
            auto it = nd->children.find(field);
            if (it == nd->children.end()) return end();
            nd = it->second.get();
        }
        return iterator{nd->content.get()};
    }
    /**
     * Gets a const pointer to the value at the requested key.
     * @param filter The topic filter entry to find.
     * @return A const pointer to the value if found, @em nullptr if not
     *         found.
     */
    const_iterator find(const key_type& filter) const {
        return const_cast<topic_matcher*>(this)->find(filter);
    }
    /**
     * Gets an match_iterator that can find the matches to the topic.
     * @param topic The topic to search for matches.
     * @return An iterator that can find the matches to the topic.
     */
    match_iterator matches(const string& topic) {
        return match_iterator(root_.get(), topic);
    }
    /**
     * Gets a const iterator that can find the matches to the topic.
     * @param topic The topic to search for matches.
     * @return A const iterator that can find the matches to the topic.
     */
    const_match_iterator matches(const string& topic) const {
        return match_iterator(root_.get(), topic);
    }
    /**
     * Gets an iterator for the end of the collection.
     *
     * This simply returns an empty/null iterator which we can use to signal
     * the end of the collection.
     *
     * @return An empty/null iterator indicating the end of the collection.
     */
    const_match_iterator matches_end() const noexcept { return match_iterator{}; }
    /**
     * Gets an iterator for the end of the collection.
     *
     * This simply returns an empty/null iterator which we can use to signal
     * the end of the collection.
     *
     * @return An empty/null iterator indicating the end of the collection.
     */
    const_match_iterator matches_cend() const noexcept { return match_iterator{}; }
    /**
     * Determines if there are any matches for the specified topic.
     * @param topic The topic to search for matches.
     * @return Whether there are any matches for the topic in the
     *         collection.
     */
    bool has_match(const string& topic) { return matches(topic) != matches_cend(); }
};
/////////////////////////////////////////////////////////////////////////////
}  // namespace mqtt
#endif  // __mqtt_topic_matcher_h
// By GST @Date