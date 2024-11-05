#ifndef AUDITCONFIGER_H
#define AUDITCONFIGER_H

#include <memory>
#include "CDsList.h"

class CAuditConfiger {
public:
    std::shared_ptr GetAttaList() const { return m_pAttaList; }
    std::shared_ptr GetContList() const { return m_pContList; }
    int GetAttaCheckMode() const { return m_nAttaCheckMode; }
    int GetContCheckMode() const { return m_nContCheckMode; }

    CAuditConfiger();
    ~CAuditConfiger() = default;

    bool Init(const std::string& szConf);

private:
    std::shared_ptr m_pAttaList;
    std::shared_ptr m_pContList;
    int m_nAttaCheckMode;
    int m_nContCheckMode;
};

#endif // AUDITCONFIGER_H

//BY GST ARMV8 GCC142.