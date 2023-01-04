#ifndef __BROADCAST_PLAN_HPP__
#define __BROADCAST_PLAN_HPP__
#include <iostream>
#include <fstream>
#include <sstream>
#include <pthread.h>
#include "json.hpp"
#include "doorsbase.h"
#include "add_custom_audio_file.hpp"
#include "talk.h"
using namespace std;
using json = nlohmann::json;
extern int g_bStop;

extern int g_playing_priority;
extern asns::CAddCustomAudioFileBusiness g_addAudioBusiness;

pthread_mutex_t g_ThreadsPlanLock = PTHREAD_MUTEX_INITIALIZER;

#define BROADCAST_PLAN_FILE_NAME "/cfg/broadcast_plan.json"
/*
string request = R"(
{
	"data": {
		"DailySchedule": [{
			"DateRange": {
				"endDate": "2022-10-15",
				"startDate": "2022-10-14"
			},
			"DayList": [{
				"Operation": {
					"audioLevel": 5,
					"audioMode": 5,
					"audioSource": "customAudio",
					"customAudioID": [1]
				},
				"TimeRange": {
					"endTime": "10:55:00+08:00",
					"startTime": "10:52:00+08:00"
				},
				"playMode": "order",
				"planExecID": 48600000000
			}]
		}],
		"TeminalInfoList": [{
			"audioOutID": [1],
			"terminalID": 1
		}]
	}
}
)";
*/

namespace asns
{

#define MATCH 1
#define SAME_DAY 2
#define ORDER_DONE 2

	class CDateRange
	{
	public:
		string endDate;
		string startDate;

	public:
		int match()
		{
			int y, m, d;
			sscanf(endDate.c_str(), "%d-%d-%d", &y, &m, &d);
			CSTime t1(y, m, d, 23, 59, 59);
			sscanf(startDate.c_str(), "%d-%d-%d", &y, &m, &d);
			CSTime t2(y, m, d, 0, 0, 0);
			CSTime tnow;
			tnow.GetCurTime();

			// check current day

			if (tnow.m_time >= t2.m_time && tnow.m_time <= t1.m_time)
			{
				cout << "endDate:" << endDate << ",startDate:" << startDate << ",match." << endl;
				if (tnow.m_nDay == t1.m_nDay && tnow.m_nDay == t2.m_nDay)
				{
					cout << "same day:" << tnow.m_nDay << endl;
					return SAME_DAY;
				}
				return MATCH;
			}
			cout << "endDate:" << endDate << ",startDate:" << startDate << ",not match." << endl;
			return 0;
		}
	};
	//顺序非常重要！
	void from_json(const json &j, CDateRange &p)
	{
		try
		{
			j.at("endDate").get_to(p.endDate);
		}
		catch (...)
		{
		}
		try
		{
			j.at("startDate").get_to(p.startDate);
		}
		catch (...)
		{
		}
	}
	void to_json(json &j, CDateRange &p)
	{

		try
		{
			j["endDate"] = p.endDate;
		}
		catch (...)
		{
		}
		try
		{
			j["startDate"] = p.startDate;
		}
		catch (...)
		{
		}

		//		j = json{{"endDate", p.endDate}, {"startDate", p.startDate}};
	}

	class COperation
	{
	public:
		int audioLevel;
		string audioSource;
		std::vector<int> customAudioID;
		string speechSynthesisContent;
		string voiceType;

	public:
		COperation &operator=(const COperation &rhs)
		{
			this->audioLevel = rhs.audioLevel;
			this->audioSource = rhs.audioSource;
			this->customAudioID = rhs.customAudioID;
			this->speechSynthesisContent = rhs.speechSynthesisContent;
			this->voiceType = rhs.voiceType;
			return *this;
		}
	};

	void from_json(const json &j, COperation &p)
	{
		try
		{
			j.at("audioLevel").get_to(p.audioLevel);
		}
		catch (...)
		{
		}
		try
		{
			j.at("audioSource").get_to(p.audioSource);
		}
		catch (...)
		{
		}

		try
		{
			j.at("customAudioID").get_to(p.customAudioID);
		}
		catch (...)
		{
		}

		try
		{
			j.at("speechSynthesisContent").get_to(p.speechSynthesisContent);
		}
		catch (...)
		{
		}

		try
		{
			j.at("voiceType").get_to(p.voiceType);
		}
		catch (...)
		{
		}
	}

	void to_json(json &j, COperation &p)
	{
		try
		{
			j["audioLevel"] = p.audioLevel;
		}
		catch (...)
		{
		}
		try
		{
			j["audioSource"] = p.audioSource;
		}
		catch (...)
		{
		}

		try
		{
			j["customAudioID"] = p.customAudioID;
		}
		catch (...)
		{
		}

		try
		{
			j["speechSynthesisContent"] = p.speechSynthesisContent;
		}
		catch (...)
		{
		}

		try
		{
			j["voiceType"] = p.voiceType;
		}
		catch (...)
		{
		}
	}

	class CTimeRange
	{
	public:
		string endTime;
		string startTime;

	public:
		int match()
		{
			CSTime tnow;
			tnow.GetCurTime();

			int h, m, s;
			sscanf(endTime.c_str(), "%d:%d:%d", &h, &m, &s);
			CSTime t1(tnow.m_nYear, tnow.m_nMon, tnow.m_nDay, h, m, s);
			sscanf(startTime.c_str(), "%d:%d:%d", &h, &m, &s);
			CSTime t2(tnow.m_nYear, tnow.m_nMon, tnow.m_nDay, h, m, s);

			if (tnow.m_time <= t1.m_time && tnow.m_time >= t2.m_time)
			{
				cout << "endTime:" << endTime << ",startTime:" << startTime << ",match." << endl;
				return 1;
			}
			cout << "endTime:" << endTime << ",startTime:" << startTime << ",not match." << endl;
			return 0;
		}
	};

	void from_json(const json &j, CTimeRange &p)
	{
		j.at("endTime").get_to(p.endTime);
		j.at("startTime").get_to(p.startTime);
	}

	void to_json(json &j, CTimeRange &p)
	{
		j["endTime"] = p.endTime;
		j["startTime"] = p.startTime;
	}

#define INVALID_DAYOFWEEK 10
	class DayInfo
	{
	public:
		COperation Operation;
		CTimeRange TimeRange;
		int planExecID;
		string playMode;
		int dayOfWeek;
		int orderExecDone;

	public:
		DayInfo()
		{
			orderExecDone = 0;
			dayOfWeek = INVALID_DAYOFWEEK; // 0-6 is ok.
			cout << "DayInfo construct called." << endl;
		}
		int is_order_done()
		{
			return orderExecDone;
		}
		int checkDayOfWeek()
		{
			if (dayOfWeek == INVALID_DAYOFWEEK)
			{
				return 1; // not check , by pass.
			}
			CSTime tnow;
			tnow.GetCurTime();
			return (tnow.m_nwDay == dayOfWeek);
		}

	public:
		// 2-del
		int work(int level)
		{
			if (!TimeRange.match())
			{
				// cout<<"time range not match."<<endl;
				return 0;
			}
			if (Operation.audioLevel != level)
			{
				// cout<<"level not match.level:"<<level<<",audioLevel:"<<Operation.audioLevel<<endl;
				return 0;
			}
			// dayofweek check.

			if (!checkDayOfWeek())
			{
				cout << "day of week mot match:" << dayOfWeek << endl;
				return 0;
			}

			if (playMode.compare("order") == 0)
			{
				cout << "do order" << endl;
				return do_order();
			}
			else if (playMode.compare("loop") == 0)
			{
				cout << "do loop" << endl;
				do_loop();
			}
			else
			{
				cout << "error play mode:" << playMode << endl;
			}
			return 1;
		}

	private:
		int do_loop()
		{
			while (1)
			{
				if (!TimeRange.match())
				{
					cout << "skip ! time range not match. loop done for planExecID:" << planExecID << endl;
					break;
				}
				do_operation();
				sleep(1);
			}
			return 0;
		}
		int do_order()
		{
			cout << "orderExecDone:" << orderExecDone << endl;
			if (orderExecDone)
			{
				cout << "skip ! order exec done for planExecID:" << planExecID << endl;
				return 0;
			}
			do_operation();
			orderExecDone = 1;
			cout << "orderExecDone:" << orderExecDone << ", remove it ..." << endl;
			return 2;
		}
		int do_operation()
		{

			cout << "really exec plan exec id:" << planExecID << endl;
			if (Operation.audioSource.compare("customAudio") == 0)
			{
				for (unsigned int i = 0; i < Operation.customAudioID.size(); i++)
				{
					int id = Operation.customAudioID.at(i);
					cout << "playing audio id:" << id << endl;
					if(g_playing_priority < Operation.audioLevel) {
						cout << "skip playing since g_playing_priority = "<<g_playing_priority <<", < operataion audioLevel="<<Operation.audioLevel<<endl;
						continue;
					} else { 
						//playing it.
						if (g_playing_priority != NON_PLAY_PRIORITY) {
							cout << "stop madplay because the low level priority talking is inprocess , g_playing_priority = "<< g_playing_priority<<endl;
                            system("killall -9 madplay");
						}
					}
					g_playing_priority = Operation.audioLevel;
					g_addAudioBusiness.play(id, TimeRange.endTime);
					g_playing_priority = NON_PLAY_PRIORITY;
				}
			}
			return 0;
		}
	};
	void from_json(const json &j, DayInfo &p)
	{
		try
		{
			j.at("Operation").get_to(p.Operation);
		}
		catch (...)
		{
		}
		try
		{
			j.at("TimeRange").get_to(p.TimeRange);
		}
		catch (...)
		{
		}
		try
		{
			j.at("planExecID").get_to(p.planExecID);
		}
		catch (...)
		{
		}
		try
		{
			j.at("playMode").get_to(p.playMode);
		}
		catch (...)
		{
		}
		try
		{
			j.at("dayOfWeek").get_to(p.dayOfWeek);
		}
		catch (...)
		{
		}
	}

	void to_json(json &j, DayInfo &p)
	{
		json jsonop;
		to_json(jsonop, p.Operation);
		json jsontimerange;
		to_json(jsontimerange, p.TimeRange);

		try
		{
			j["Operation"] = jsonop; // p.Operation;
		}
		catch (...)
		{
		}
		try
		{
			j["TimeRange"] = jsontimerange; // p.TimeRange;
		}
		catch (...)
		{
		}
		try
		{
			j["planExecID"] = p.planExecID;
		}
		catch (...)
		{
		}
		try
		{
			j["playMode"] = p.playMode;
		}
		catch (...)
		{
		}
		try
		{
			j["dayOfWeek"] = p.dayOfWeek;
		}
		catch (...)
		{
		}
	}

	struct TeminalInfo
	{
		std::vector<int> audioOutID;
		int terminalID;
	};

	void from_json(const json &j, TeminalInfo &p)
	{
		try
		{
			j.at("audioOutID").get_to(p.audioOutID);
		}
		catch (...)
		{
		}

		try
		{
			j.at("terminalID").get_to(p.terminalID);
		}
		catch (...)
		{
		}
	}

	void to_json(json &j, TeminalInfo &p)
	{
		try
		{
			j["audioOutID"] = p.audioOutID;
			j["terminalID"] = p.terminalID;
		}
		catch (...)
		{
		}
	}
	class DailyScheduleInfo
	{
	public:
		CDateRange DateRange;
		vector<DayInfo> DayList;

	public:
		int work(int level)
		{
			int match = DateRange.match();
			if (match == 0)
			{
				cout << "date range not match." << endl;

				return 0;
			}

			cout << "DayList.size() = " << DayList.size() << endl;

			for (int i = 0; i != DayList.size(); i++)
			{
				int done = DayList[i].work(level);
				cout << "work ret:" << done << ",order_done:" << DayList[i].is_order_done() << endl;
				if (done == ORDER_DONE && match == SAME_DAY)
				{
					cout << "erase dayinfo node i=" << i << endl;
					DayList.erase(DayList.begin() + i);
					i--;
				}
			}

			cout << "DayList.size() = " << DayList.size() << endl;
		}
	};

	void from_json(const json &j, DailyScheduleInfo &p)
	{
		try
		{
			j.at("DateRange").get_to(p.DateRange);
		}
		catch (...)
		{
		}
		try
		{
			j.at("DayList").get_to(p.DayList);
		}
		catch (...)
		{
		}
	}

	void to_json(json &j, DailyScheduleInfo &p)
	{
		json json_daterange;
		to_json(json_daterange, p.DateRange);

		j["DateRange"] = json_daterange; // p.DateRange;
		for (auto &t : p.DayList)
		{
			json j_d;
			to_json(j_d, t);
			j["DayList"].push_back(j_d);
		}
	}
	class CBroadcastPlanData
	{
	public:
		vector<DailyScheduleInfo> DailySchedule;
		vector<DailyScheduleInfo> WeeklySchedule;
		vector<TeminalInfo> TeminalInfoList;

	public:
		void clear()
		{
			DailySchedule.clear();
			WeeklySchedule.clear();
			TeminalInfoList.clear();
		}
	};

	void from_json(const json &j, CBroadcastPlanData &p)
	{
		try
		{
			j.at("DailySchedule").get_to(p.DailySchedule);
		}
		catch (...)
		{
		}
		try
		{
			j.at("WeeklySchedule").get_to(p.WeeklySchedule);
		}
		catch (...)
		{
		}
		try
		{
			j.at("TeminalInfoList").get_to(p.TeminalInfoList);
		}
		catch (...)
		{
		}
	}

	void to_json(json &j, CBroadcastPlanData &p)
	{
		for (auto &t : p.DailySchedule)
		{
			json j_d;
			to_json(j_d, t);
			j["DailySchedule"].push_back(j_d);
		}

		for (auto &t : p.WeeklySchedule)
		{
			json j_d;
			to_json(j_d, t);
			j["WeeklyySchedule"].push_back(j_d);
		}

		for (auto &t : p.TeminalInfoList)
		{
			json j_d;
			to_json(j_d, t);
			j["TeminalInfoList"].push_back(j_d);
		}
	}

	class CBroadcastPlanBusiness : public CSThread
	{
	private:
		CBroadcastPlanData plan;
        std::string filePath;

	public:
        CBroadcastPlanBusiness(){
            CAudioCfgBusiness business;
            business.load();
            filePath = business.business[0].savePrefix + BROADCAST_PLAN_FILE_NAME;
        }
		virtual int InitInstance() { execPlanThreadStart(); }
		virtual int ExitInstance() {}

		int parseRequest(string data)
		{
			//Begin modified by shidongxue . fix bug.2023.1.4
			/*if (g_playing_priority != NON_PLAY_PRIORITY)
			{ // the plan is playing...
				cout << "parseRequest for plan...the plan is plaing now, stop plan play first." << endl;
				system("killall -9 madplay");
			}
			*/ 
			
			//end modified by shidongxue . fix bug.2023.1.4

			cout << data << endl;
			pthread_mutex_lock(&g_ThreadsPlanLock);
			json j = json::parse(data, nullptr, false);
			try
			{
				// clear old plan?
				plan.clear();
				plan = j.at("data");
			}
			catch (json::parse_error &ex)
			{
				std::cerr << "parse error at byte " << ex.byte << std::endl;
				pthread_mutex_unlock(&g_ThreadsPlanLock);
				return -1;
			}
			save_plan(data);

			pthread_mutex_unlock(&g_ThreadsPlanLock);
			return 0;
		}

		int save_plan(string data)
		{
			ofstream outFile;
			outFile.open(filePath);
			outFile << data;
			outFile.close();
			return 0;
		}

		int saveToJson()
		{
			json j;
			to_json(j["data"], plan);
			// j = json{{"data":plan}};
			// j = json{{"name", p.name}, {"address", p.address}, {"age", p.age}};
			// j = json{{"data",plan}};
			std::ofstream o(filePath);
			o << std::setw(4) << j << std::endl;
		}
		int load()
		{
			std::ifstream i(filePath);
			json j;
			if (!i)
			{
				cout << "error read json file for broadcast plan." << filePath << endl;
				return -1;
			}

			i >> j;
			cout << "broadcast plan, json content:" << j.dump() << endl;
			try
			{
				plan = j.at("data");
			}
			catch (json::parse_error &ex)
			{
				std::cerr << "parse error at byte " << ex.byte << std::endl;
				return -1;
			}
			cout << "load " << filePath << " success! total plan count:" << plan.DailySchedule.size() << endl;
			return 0;
		}
		int dump()
		{
			for (int i = 0; i < plan.DailySchedule.size(); i++)
			{
				DailyScheduleInfo dailySchedule = plan.DailySchedule.at(i);
				for (int j = 0; j < dailySchedule.DayList.size(); j++)
				{
					DayInfo day = dailySchedule.DayList.at(j);
					cout << "planExecID:" << day.planExecID << endl;
				}
			}

			for (int i = 0; i < plan.WeeklySchedule.size(); i++)
			{
				DailyScheduleInfo dailySchedule = plan.WeeklySchedule.at(i);
				for (int j = 0; j < dailySchedule.DayList.size(); j++)
				{
					DayInfo day = dailySchedule.DayList.at(j);
					cout << "planExecID:" << day.planExecID << endl;
				}
			}
		}

		int execPlanThreadStart()
		{
			cout << "plan thread starting. init total daily plan: " << plan.DailySchedule.size() << endl;
			while (1)
			{
				if (g_bStop)
				{ // broadcast is stop.

					pthread_mutex_lock(&g_ThreadsPlanLock);
					for (int level = 0; level < 10; level++)
					{

						for (int i = 0; i < plan.DailySchedule.size(); i++)
						{
							for (int j = 0; j < plan.DailySchedule[i].DayList.size(); j++)
							{
								plan.DailySchedule[i].work(level); // DayList[j].work(level);
																   // plan.DailySchedule[i].DayList[j].work(level);
							}
						}
						for (int i = 0; i < plan.WeeklySchedule.size(); i++)
						{
							for (int j = 0; j < plan.WeeklySchedule[i].DayList.size(); j++)
							{
								plan.WeeklySchedule[i].work(level); // DayList[j].work(level);
							}
						}
					}

					cout << "updating json.... current total daily plan: " << plan.DailySchedule.size() << endl;
					saveToJson();

					pthread_mutex_unlock(&g_ThreadsPlanLock);
				}

				sleep(30);
			}
		}
	};

}

#endif
