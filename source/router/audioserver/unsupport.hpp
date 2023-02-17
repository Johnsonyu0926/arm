#ifndef __CUNSUPPORT_H__
#define __CUNSUPPORT_H__
#include <iostream>
#include "json.hpp"
using namespace std;

namespace asns
{

	class CUnsupport
	{
	private:
		string cmd;
		int resultId;
		string msg;

	public:
		NLOHMANN_DEFINE_TYPE_INTRUSIVE(CUnsupport, cmd, resultId, msg)

	public:
		int do_success(string reqcmd)
		{
			cmd = reqcmd;
			resultId = 2;
			msg = "unsupport operation";
		}
	};
	class CUnsupportBusiness
	{
	private:
		string cmd;

	public:
		NLOHMANN_DEFINE_TYPE_INTRUSIVE(CUnsupportBusiness, cmd)

	public:
		void setCmd(string unsupport_cmd) { cmd = unsupport_cmd; }
		int do_req(CSocket *pClient)
		{
            DS_TRACE("unsuport cmd:" << cmd.c_str());
			CUnsupport res;
			res.do_success(cmd);
			json j = res;
			std::string s = j.dump();
			pClient->Send(s.c_str(), s.length());
			return 1;
		}
	};

} // namespace tcpserver
#endif
