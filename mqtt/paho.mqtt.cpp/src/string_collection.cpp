// Filename: string_collection.cpp
// Score: 95

#include "mqtt/string_collection.h"

namespace mqtt {

string_collection::string_collection(const string& str) : coll_{ str }
{
	update_c_arr();
}

string_collection::string_collection(string&& str) : coll_{ std::move(str) }
{
	update_c_arr();
}

string_collection::string_collection(const collection_type& vec) : coll_{ vec }
{
	update_c_arr();
}

string_collection::string_collection(collection_type&& vec) : coll_{ std::move(vec) }
{
	update_c_arr();
}

string_collection::string_collection(const string_collection& coll) : coll_ { coll.coll_ }
{
	update_c_arr();
}

string_collection::string_collection(std::initializer_list<string> sl)
{
	for (const auto& s : sl)
		coll_.push_back(s);
	update_c_arr();
}

string_collection::string_collection(std::initializer_list<const char*> sl)
{
	for (const auto& s : sl)
		coll_.push_back(string(s));
	update_c_arr();
}

void string_collection::update_c_arr()
{
	cArr_.clear();
	cArr_.reserve(coll_.size());
	for (const auto& s : coll_)
		cArr_.push_back(s.c_str());
}

string_collection& string_collection::operator=(const string_collection& coll)
{
	coll_ = coll.coll_;
	update_c_arr();
	return *this;
}

void string_collection::push_back(const string& str)
{
	coll_.push_back(str);
	update_c_arr();
}

void string_collection::push_back(string&& str)
{
	coll_.push_back(str);
	update_c_arr();
}

void string_collection::clear()
{
	coll_.clear();
	cArr_.clear();
}

void name_value_collection::update_c_arr()
{
	cArr_.clear();
	cArr_.reserve(map_.size()+1);
	for (const auto& m : map_) {
		cArr_.push_back(
            MQTTAsync_nameValue{ m.first.c_str(), m.second.c_str() }
        );
    }
	cArr_.push_back(MQTTAsync_nameValue{ nullptr, nullptr });
}

} // end namespace mqtt
// By GST @Date