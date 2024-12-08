#ifndef __CAUDIT_CONFIGER_H__
#define __CAUDIT_CONFIGER_H__

#include <memory>
#include "CDsList.h"
#include "spdlog/spdlog.h"

class CAuditConfiger {
public:
    CAuditConfiger() 
        : m_pAttaList(std::make_unique<CDsList>()), 
          m_pContList(std::make_unique<CDsList>()) {
        spdlog::info("CAuditConfiger initialized");
    }

    ~CAuditConfiger() = default;

    CDsList* GetAttaList() const {
        return m_pAttaList.get();
    }

    CDsList* GetContList() const {
        return m_pContList.get();
    }

    int GetAttaCheckMode() const {
        return m_nAttaCheckMode;
    }

    int GetContCheckMode() const {
        return m_nContCheckMode;
    }

private:
    void clearList(std::unique_ptr<CDsList>& list) {
        if (list) {
            for (int i = 0; i < list->GetCount(); ++i) {
                char* item = static_cast<char*>(list->ElementAt(i));
                delete[] item;
            }
            list->RemoveAll();
            spdlog::info("List cleared");
        }
    }

    std::unique_ptr<CDsList> m_pAttaList;
    std::unique_ptr<CDsList> m_pContList;
    int m_nAttaCheckMode{0};
    int m_nContCheckMode{0};
};

#endif // __CAUDIT_CONFIGER_H__