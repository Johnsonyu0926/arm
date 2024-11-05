// CQSearch.h
#ifndef CQSEARCH_H
#define CQSEARCH_H

#include <string>
#include <vector>

class CCQSearch {
public:
    CCQSearch();
    virtual ~CCQSearch() = default;

    bool Init();
    bool Search(const std::string& query);

private:
    void ParseQuery(const std::string& query);
    void ExecuteSearch();

    std::string m_query;
    std::vector<std::string> m_parsedTokens;
    bool m_isInitialized{false};
};

#endif // CQSEARCH_H

//BY GST ARMV8 GCC 13.2