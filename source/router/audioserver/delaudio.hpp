#ifndef __CDelAUDIO_H__
#define __CDelAUDIO_H__
#include <iostream>
#include "json.hpp"
#include "add_custom_audio_file.hpp"
#include "audiocfg.hpp"
#include "utils.h"
#include "getaudiolist.hpp"
using namespace std;

extern asns::CAddCustomAudioFileBusiness g_addAudioBusiness;
namespace asns
{

	class CDelAudioResult
	{
	private:
		string cmd;
		int resultId;
		string msg;
		vector<CGetAudioData> data;

	public:
		NLOHMANN_DEFINE_TYPE_INTRUSIVE(CDelAudioResult, cmd, resultId, data, msg)

	public:
		int do_success()
		{
			cmd = "AudioDelete";
			resultId = 1;
			msg = "AudioDelete handle success";

			CAddCustomAudioFileBusiness audios;
			audios.load();
			for (int i = 0; i < audios.business.size(); i++)
			{
				CGetAudioData v;
				v.storageType = 0;
				v.type = 32;
				audios.business[i].parseFile();
				v.fileName = audios.business[i].customAudioName;
				CAudioCfgBusiness cfg;
				cfg.load();
				CUtils utils;
				v.size = utils.get_size(cfg.getAudioFilePath().c_str(), audios.business[i].filename);
				v.audioId = audios.business[i].customAudioID;
				data.push_back(v);
			}
		};
	};
	class CDelAudio
	{
	private:
		string cmd;
		int audioId;

	public:
		NLOHMANN_DEFINE_TYPE_INTRUSIVE(CDelAudio, cmd, audioId)

	private:
		int do_del(int id)
		{

			CAddCustomAudioFileBusiness audios;
			audios.load();
			// find id
			int do_reload = 0;
			for (int i = 0; i < audios.business.size(); i++)
			{
				if (audios.business[i].customAudioID != id)
				{
					continue;
				}
				do_reload = 1;
				audios.business[i].parseFile();
				char cmd[256];
				CAudioCfgBusiness cfg;
				cfg.load();

				sprintf(cmd, "rm %s/%s", cfg.getAudioFilePath().c_str(), audios.business[i].filename);
				system(cmd);
				cout << "del file cmd:" << cmd << endl;

				cout << "update json config and reload audio program." << endl;
				audios.business.erase(audios.business.begin() + i);
				audios.saveToJson();
				// system("killall audio");
				// system("audio&");

				return 1;
			}
			if (do_reload)
			{
				g_addAudioBusiness.load();
			}
			return 1;
		}

	public:
		int do_req(CSocket *pClient)
		{
			cout << "handle req for del audio id:" << audioId << endl;
			do_del(audioId);
			CDelAudioResult res;
			res.do_success();
			json j = res;
			std::string s = j.dump();
			pClient->Send(s.c_str(), s.length());
			return 1;
		}
	};

} // namespace tcpserver
#endif
