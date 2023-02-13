#pragma once

#include "json.hpp"

namespace asns {
    template<typename Quest, typename Result>
    class CReQuest;
    template<typename T>
    class CResult;
    class CPlanPlayResultData {
    public:
        NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(CPlanPlayResultData, null)

        template<typename Quest, typename Result,typename T>
        int do_success(const CReQuest<Quest, Result> &c, CResult<T> &r) {
            std::cout << "command:" << c.data.command << std::endl;
            system(c.data.command.c_str());
            r.resultId = 1;
            r.result = "success";
            return 1;
        }

    private:
        std::nullptr_t null;
    };

    class CPlanPlayData {
    public:
        NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(CPlanPlayData, cmd, command)

    public:
        std::string cmd;
        std::string command;
    };
}