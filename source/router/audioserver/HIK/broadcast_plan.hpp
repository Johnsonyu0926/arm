#ifndef __BROADCAST_PLAN_HPP__
#define __BROADCAST_PLAN_HPP__

#include <iostream>
#include <fstream>
#include <sstream>
#include <pthread.h>
#include <vector>
#include <string>
#include "json.hpp"
#include "doorsbase.h"
#include "add_custom_audio_file.hpp"
#include "talk.h"
#include "utils.h"

using json = nlohmann::json;

namespace asns {

    const std::string BROADCAST_PLAN_FILE_NAME = "/cfg/broadcast_plan.json";
    const int SCHEDULE_RESULT_DELETE = 200;
    const int MATCH = 1;
    const int SAME_DAY = 2;
    const int ORDER_DONE = 2;
    const int INVALID_DAYOFWEEK = 10;

    extern int g_updateJson;
    extern int g_playing_priority;
    extern CAddCustomAudioFileBusiness g_addAudioBusiness;

    pthread_mutex_t g_ThreadsPlanLock = PTHREAD_MUTEX_INITIALIZER;

    class CDateRange {
    public:
        std::string endDate;
        std::string startDate;

        int match() const {
            int y, m, d;
            sscanf(endDate.c_str(), "%d-%d-%d", &y, &m, &d);
            CSTime t1(y, m, d, 23, 59, 59);
            sscanf(startDate.c_str(), "%d-%d-%d", &y, &m, &d);
            CSTime t2(y, m, d, 0, 0, 0);
            CSTime tnow;
            tnow.GetCurTime();

            if (tnow.m_time >= t2.m_time && tnow.m_time <= t1.m_time) {
                LOG(INFO) << "endDate:" << endDate << ", startDate:" << startDate << ", match.";
                if (tnow.m_nDay == t1.m_nDay && tnow.m_nDay == t2.m_nDay) {
                    LOG(INFO) << "same day:" << tnow.m_nDay;
                    return SAME_DAY;
                }
                return MATCH;
            }
            LOG(INFO) << "endDate:" << endDate << ", startDate:" << startDate << ", not match.";
            return 0;
        }
    };

    void from_json(const json &j, CDateRange &p) {
        j.at("endDate").get_to(p.endDate);
        j.at("startDate").get_to(p.startDate);
    }

    void to_json(json &j, const CDateRange &p) {
        j["endDate"] = p.endDate;
        j["startDate"] = p.startDate;
    }

    class COperation {
    public:
        int audioLevel{0};
        std::string audioSource;
        std::vector<int> customAudioID;
        std::string speechSynthesisContent;
        std::string voiceType;

        COperation& operator=(const COperation &rhs) {
            if (this != &rhs) {
                audioLevel = rhs.audioLevel;
                audioSource = rhs.audioSource;
                customAudioID = rhs.customAudioID;
                speechSynthesisContent = rhs.speechSynthesisContent;
                voiceType = rhs.voiceType;
            }
            return *this;
        }
    };

    void from_json(const json &j, COperation &p) {
        j.at("audioLevel").get_to(p.audioLevel);
        j.at("audioSource").get_to(p.audioSource);
        j.at("customAudioID").get_to(p.customAudioID);
        j.at("speechSynthesisContent").get_to(p.speechSynthesisContent);
        j.at("voiceType").get_to(p.voiceType);
    }

    void to_json(json &j, const COperation &p) {
        j["audioLevel"] = p.audioLevel;
        j["audioSource"] = p.audioSource;
        j["customAudioID"] = p.customAudioID;
        j["speechSynthesisContent"] = p.speechSynthesisContent;
        j["voiceType"] = p.voiceType;
    }

    class CTimeRange {
    public:
        std::string endTime;
        std::string startTime;

        int match() const {
            CSTime tnow;
            tnow.GetCurTime();

            int h, m, s;
            sscanf(endTime.c_str(), "%d:%d:%d", &h, &m, &s);
            CSTime t1(tnow.m_nYear, tnow.m_nMon, tnow.m_nDay, h, m, s);
            sscanf(startTime.c_str(), "%d:%d:%d", &h, &m, &s);
            CSTime t2(tnow.m_nYear, tnow.m_nMon, tnow.m_nDay, h, m, s);

            if (tnow.m_time <= t1.m_time && tnow.m_time >= t2.m_time) {
                LOG(INFO) << "endTime:" << endTime << ", startTime:" << startTime << ", match.";
                return 1;
            }
            LOG(INFO) << "endTime:" << endTime << ", startTime:" << startTime << ", not match.";
            return 0;
        }
    };

    void from_json(const json &j, CTimeRange &p) {
        j.at("endTime").get_to(p.endTime);
        j.at("startTime").get_to(p.startTime);
    }

    void to_json(json &j, const CTimeRange &p) {
        j["endTime"] = p.endTime;
        j["startTime"] = p.startTime;
    }

    class DayInfo {
    public:
        COperation Operation;
        CTimeRange TimeRange;
        int planExecID{0};
        std::string playMode;
        int dayOfWeek{INVALID_DAYOFWEEK};
        int orderExecDone{0};

        DayInfo() {
            LOG(INFO) << "DayInfo construct called.";
        }

        int is_order_done() const {
            return orderExecDone;
        }

        int checkDayOfWeek() const {
            if (dayOfWeek == INVALID_DAYOFWEEK) {
                return 1; // not check, bypass.
            }
            CSTime tnow;
            tnow.GetCurTime();
            return (tnow.m_nwDay == dayOfWeek);
        }

        int work(int level) {
            if (!TimeRange.match()) {
                LOG(INFO) << "time range not match.";
                return 0;
            }
            if (Operation.audioLevel != level) {
                LOG(INFO) << "level not match. level: " << level << ", audioLevel: " << Operation.audioLevel;
                return 0;
            }
            if (!checkDayOfWeek()) {
                LOG(INFO) << "day of week not match: " << dayOfWeek;
                return 0;
            }
            if (playMode == "order") {
                LOG(INFO) << "do order";
                return do_order();
            } else if (playMode == "loop") {
                LOG(INFO) << "do loop";
                do_loop();
            } else {
                LOG(INFO) << "error play mode: " << playMode;
            }
            return 1;
        }

    private:
        int do_loop() {
            while (!g_updateJson) {
                if (!TimeRange.match()) {
                    LOG(INFO) << "skip! time range not match. loop done for planExecID: " << planExecID;
                    break;
                }
                do_operation(0);
                sleep(1);
            }
            return 0;
        }

        int do_order() {
            LOG(INFO) << "orderExecDone: " << orderExecDone;
            if (orderExecDone) {
                LOG(INFO) << "skip! order exec done for planExecID: " << planExecID;
                return 0;
            }
            do_operation(1);
            orderExecDone = 1;
            LOG(INFO) << "orderExecDone: " << orderExecDone << ", remove it ...";
            return ORDER_DONE;
        }

        int do_operation(int model) {
            LOG(INFO) << "really exec plan exec id: " << planExecID;
            if (Operation.audioSource == "customAudio") {
                for (int id : Operation.customAudioID) {
                    LOG(INFO) << "playing audio id: " << id;
                    if (g_playing_priority <= Operation.audioLevel) {
                        LOG(INFO) << "skip playing since g_playing_priority = " << g_playing_priority << ", < operation audioLevel = " << Operation.audioLevel;
                        continue;
                    } else {
                        if (g_playing_priority != NON_PLAY_PRIORITY) {
                            LOG(INFO) << "stop madplay because the low level priority talking is in process, g_playing_priority = " << g_playing_priority;
                            AudioPlayUtil::audio_stop();
                        }
                    }
                    LOG(INFO) << "begin play audio priority: " << g_playing_priority;
                    g_playing_priority = Operation.audioLevel;
                    g_addAudioBusiness.play(id, TimeRange.endTime, g_playing_priority);
                    g_playing_priority = NON_PLAY_PRIORITY;
                }
            } else if (Operation.audioSource == "speechSynthesis") {
                if (g_playing_priority <= Operation.audioLevel) {
                    LOG(INFO) << "skip playing since g_playing_priority = " << g_playing_priority << ", < operation audioLevel = " << Operation.audioLevel;
                    return 0;
                } else {
                    if (g_playing_priority != NON_PLAY_PRIORITY) {
                        LOG(INFO) << "stop madplay because the low level priority talking is in process, g_playing_priority = " << g_playing_priority;
                        AudioPlayUtil::audio_stop();
                    }
                }
                LOG(INFO) << "begin play audio priority: " << g_playing_priority;
                g_playing_priority = Operation.audioLevel;
                CSpeechSynthesisBusiness bus;
                bus.play(TimeRange.endTime, g_playing_priority, Operation.speechSynthesisContent, Operation.voiceType, model);
                g_playing_priority = NON_PLAY_PRIORITY;
            }
            return 0;
        }
    };

    void from_json(const json &j, DayInfo &p) {
        j.at("Operation").get_to(p.Operation);
        j.at("TimeRange").get_to(p.TimeRange);
        j.at("planExecID").get_to(p.planExecID);
        j.at("playMode").get_to(p.playMode);
        j.at("dayOfWeek").get_to(p.dayOfWeek);
    }

    void to_json(json &j, const DayInfo &p) {
        j["Operation"] = p.Operation;
        j["TimeRange"] = p.TimeRange;
        j["planExecID"] = p.planExecID;
        j["playMode"] = p.playMode;
        j["dayOfWeek"] = p.dayOfWeek;
    }

    struct TeminalInfo {
        std::vector<int> audioOutID;
        int terminalID{0};
    };

    void from_json(const json &j, TeminalInfo &p) {
        j.at("audioOutID").get_to(p.audioOutID);
        j.at("terminalID").get_to(p.terminalID);
    }

    void to_json(json &j, const TeminalInfo &p) {
        j["audioOutID"] = p.audioOutID;
        j["terminalID"] = p.terminalID;
    }

    class DailyScheduleInfo {
    public:
        CDateRange DateRange;
        std::vector<DayInfo> DayList;

        int work(int level) {
            int res = 0;
            int match = DateRange.match();
            if (match == 0) {
                LOG(INFO) << "date range not match.";
                return 0;
            }

            LOG(INFO) << "DayList.size() = " << DayList.size();

            for (size_t i = 0; i < DayList.size(); ++i) {
                int done = DayList[i].work(level);
                LOG(INFO) << "work ret: " << done << ", order_done: " << DayList[i].is_order_done();
                if (done == ORDER_DONE && match == SAME_DAY) {
                    LOG(INFO) << "erase dayinfo node i = " << i;
                    DayList.erase(DayList.begin() + i);
                    --i;
                    res = SCHEDULE_RESULT_DELETE;
                }
            }

            LOG(INFO) << "DayList.size() = " << DayList.size();
            return res;
        }
    };

    void from_json(const json &j, DailyScheduleInfo &p) {
        j.at("DateRange").get_to(p.DateRange);
        j.at("DayList").get_to(p.DayList);
    }

    void to_json(json &j, const DailyScheduleInfo &p) {
        j["DateRange"] = p.DateRange;
        j["DayList"] = p.DayList;
    }

    class CBroadcastPlanData {
    public:
        std::vector<DailyScheduleInfo> DailySchedule;
        std::vector<DailyScheduleInfo> WeeklySchedule;
        std::vector<TeminalInfo> TeminalInfoList;

        void clear() {
            DailySchedule.clear();
            WeeklySchedule.clear();
            TeminalInfoList.clear();
        }
    };

    void from_json(const json &j, CBroadcastPlanData &p) {
        j.at("DailySchedule").get_to(p.DailySchedule);
        j.at("WeeklySchedule").get_to(p.WeeklySchedule);
        j.at("TeminalInfoList").get_to(p.TeminalInfoList);
    }

    void to_json(json &j, const CBroadcastPlanData &p) {
        j["DailySchedule"] = p.DailySchedule;
        j["WeeklySchedule"] = p.WeeklySchedule;
        j["TeminalInfoList"] = p.TeminalInfoList;
    }

    class CBroadcastPlanBusiness : public CSThread {
    private:
        CBroadcastPlanData plan;
        std::string filePath;

    public:
        CBroadcastPlanBusiness() {
            CAudioCfgBusiness business;
            business.load();
            filePath = business.business[0].savePrefix + BROADCAST_PLAN_FILE_NAME;
        }

        virtual int InitInstance() override {
            execPlanThreadStart();
            return 0;
        }

        virtual int ExitInstance() override {
            return 0;
        }

        int parseRequest(const std::string &data) {
            g_updateJson = 1;
            LOG(INFO) << data;
            std::lock_guard<std::mutex> lock(g_ThreadsPlanLock);
            json j = json::parse(data, nullptr, false);
            try {
                plan.clear();
                plan = j.at("data").get<CBroadcastPlanData>();
            } catch (json::parse_error &ex) {
                LOG(ERROR) << "parse error at byte " << ex.byte;
                return -1;
            }
            save_plan(data);
            return 0;
        }

        int save_plan(const std::string &data) {
            std::ofstream outFile(filePath);
            if (!outFile.is_open()) {
                LOG(ERROR) << "Failed to open file for writing: " << filePath;
                return -1;
            }
            outFile << data;
            outFile.close();
            return 0;
        }

        void saveToJson() {
            json j;
            j["data"] = plan;
            std::ofstream o(filePath);
            if (!o.is_open()) {
                LOG(ERROR) << "Failed to open file for writing: " << filePath;
                return;
            }
            o << std::setw(4) << j << std::endl;
            o.close();
        }

        int load() {
            std::ifstream i(filePath);
            if (!i.is_open()) {
                LOG(INFO) << "error read json file for broadcast plan: " << filePath;
                return -1;
            }
            json j;
            try {
                i >> j;
                LOG(INFO) << "broadcast plan, json content: " << j.dump();
                plan = j.at("data").get<CBroadcastPlanData>();
            } catch (json::parse_error &ex) {
                LOG(ERROR) << "parse error at byte " << ex.byte;
                return -1;
            }
            LOG(INFO) << "load " << filePath << " success! total plan count: " << plan.DailySchedule.size();
            return 0;
        }

        void dump() const {
            for (const auto &dailySchedule : plan.DailySchedule) {
                for (const auto &day : dailySchedule.DayList) {
                    LOG(INFO) << "planExecID: " << day.planExecID;
                }
            }

            for (const auto &weeklySchedule : plan.WeeklySchedule) {
                for (const auto &day : weeklySchedule.DayList) {
                    LOG(INFO) << "planExecID: " << day.planExecID;
                }
            }
        }

        int execPlanThreadStart() {
            LOG(INFO) << "plan thread starting. init total daily plan: " << plan.DailySchedule.size();
            while (true) {
                int flag = 0;
                std::lock_guard<std::mutex> lock(g_ThreadsPlanLock);
                g_updateJson = 0;
                for (int level = 0; level < 16; ++level) {
                    for (auto &dailySchedule : plan.DailySchedule) {
                        for (size_t j = 0; j < dailySchedule.DayList.size(); ++j) {
                            int temp = dailySchedule.work(level);
                            if (temp == SCHEDULE_RESULT_DELETE) {
                                flag = temp;
                            }
                        }
                    }
                    for (auto &weeklySchedule : plan.WeeklySchedule) {
                        for (size_t j = 0; j < weeklySchedule.DayList.size(); ++j) {
                            int temp = weeklySchedule.work(level);
                            if (temp == SCHEDULE_RESULT_DELETE) {
                                flag = temp;
                            }
                        }
                    }
                }
                if (flag == SCHEDULE_RESULT_DELETE) {
                    saveToJson();
                    LOG(INFO) << "updating json.... current total daily plan: " << plan.DailySchedule.size();
                }
                sleep(30);
            }
        }
    };

}

#endif