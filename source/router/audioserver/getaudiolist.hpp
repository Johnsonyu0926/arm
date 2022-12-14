#ifndef __CGETAUDIOLIST_H__
#define __CGETAUDIOLIST_H__
#include <iostream>
#include "json.hpp"
#include "add_custom_audio_file.hpp"
#include "add_column_custom_audio_file.hpp"
#include "audiocfg.hpp"
#include "utils.h"
using namespace std;

namespace asns
{

	class CGetAudioData
	{
	public:
		int storageType;
		int type;
		string fileName;
		float size;
		int audioId;

	public:
		NLOHMANN_DEFINE_TYPE_INTRUSIVE(CGetAudioData, storageType, type, fileName, size, audioId)
	};

	class CGetAudioListResult
	{
	private:
		string cmd;
		int resultId;
		string msg;
		vector<CGetAudioData> data;

	public:
		NLOHMANN_DEFINE_TYPE_INTRUSIVE(CGetAudioListResult, cmd, resultId, data, msg)

	public:
		int do_success()
		{
			cmd = "GetAudioList";
			resultId = 1;
			msg = "GetAudioList handle success";

			CAddCustomAudioFileBusiness audios;
			audios.load();
			for (int i = 0; i < audios.business.size(); i++)
			{
				CGetAudioData v;
				v.storageType = 1;
				v.type = 32;
				audios.business[i].parseFile();
				v.fileName = audios.business[i].customAudioName;
				CAudioCfgBusiness cfg;
				cfg.load();
				CUtils utils;
				v.size = utils.get_size(cfg.business[0].savePrefix.c_str(), audios.business[i].filename);
				v.audioId = audios.business[i].customAudioID;
				data.push_back(v);
			}

			CAddColumnCustomAudioFileBusiness business;
			business.Columnload();
			for (const auto &it : business.business)
			{
				CGetAudioData v;
				v.storageType = 0;
				v.type = 2;
				v.fileName = it.getName();
				CAudioCfgBusiness cfg;
				cfg.load();
				CUtils utils;
				v.size = utils.get_size(cfg.business[0].savePrefix.c_str(), it.getName().c_str());
				v.audioId = 0;
				data.push_back(v);
			}
		};
	};
	class CGetAudioList
	{
	private:
		string cmd;

	public:
		NLOHMANN_DEFINE_TYPE_INTRUSIVE(CGetAudioList, cmd)

	public:
		int do_req(CSocket *pClient)
		{
			CGetAudioListResult res;
			res.do_success();
			json j = res;
			std::string s = j.dump();
			pClient->Send(s.c_str(), s.length());
			return 1;
		}
	};

} // namespace tcpserver
#endif
