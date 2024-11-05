// CMail.h
#ifndef CMAIL_H
#define CMAIL_H

#include 
#include 
#include 

class CMail {
public:
    CMail();
    ~CMail();

    CMail(const CMail&) = delete;
    CMail& operator=(const CMail&) = delete;
    CMail(CMail&&) noexcept = default;
    CMail& operator=(CMail&&) noexcept = default;

    void SetSubject(const std::string& subject);
    void SetFrom(const std::string& from);
    void SetTo(const std::string& to);
    void SetDate(const std::string& date);
    void SetContent(const std::string& content);

    int BuildMail();

    const char* GetMessage() const;

private:
    std::unique_ptr m_szMessage;
    std::string m_szContent;
    std::string m_szSubject;
    std::string m_szFrom;
    std::string m_szTo;
    std::string m_szDate;
};

#endif // CMAIL_H

//BY GST ARMV8 GCC 13.2