// global.h

 /
#ifndef GLOBAL_H
#define GLOBAL_H

#include "database.h"
#include 
#include 

inline constexpr int INTERVAL_TIME = 150;

/ Table definitions /
inline constexpr std::string_view TABLE_IPPKT1      = "ip_packet";
inline constexpr std::string_view TABLE_IPPKT2      = "ip_packet2";
inline constexpr std::string_view TABLE_IPSTA_TCP   = "ip_tcp_sta";
inline constexpr std::string_view TABLE_IPSTA_ICMP  = "ip_icmp_sta";
inline constexpr std::string_view TABLE_IPSTA_UDP   = "ip_udp_sta";
inline constexpr std::string_view TABLE_IPSTA_OTHER = "ip_other_sta";
/ tcp /
inline constexpr std::string_view TABLE_TCPINFO     = "tcp_conn_sta";
/ http /
inline constexpr std::string_view TABLE_HTTPPAGE    = "http_page_sta";
/ tp /
inline constexpr std::string_view TABLE_TPCONN      = "tp_conn_sta";
inline constexpr std::string_view TABLE_TPPDU       = "tp_pdus";
inline constexpr std::string_view TABLE_TPDISCONN   = "tp_disconn_sta";
/ arm /
inline constexpr std::string_view TABLE_ARMCONN     = "arm_conn_sta";
inline constexpr std::string_view TABLE_ARMPDU      = "arm_pdus";
/ conference manager /
inline constexpr std::string_view TABLE_CMGPDU      = "cmg_pdus";
/ meeting /
inline constexpr std::string_view TABLE_MTG_CONN    = "mtg_conn_sta";
inline constexpr std::string_view TABLE_APPSVR      = "appsvr_trarte";
inline constexpr std::string_view TABLE_SVRSTA      = "mtg_svrstatus";
inline constexpr std::string_view TABLE_CPING_PDUS  = "mz_cping_pdus";
inline constexpr std::string_view TABLE_MMCB_PDUS   = "mz_mmcb_pdus";
inline constexpr std::string_view TABLE_APPS_PDUS   = "mz_mmapp_pdus";
inline constexpr std::string_view TABLE_FCS_PDUS    = "mz_mmfcs_pdus";
inline constexpr std::string_view TABLE_PMS_PDUS    = "mz_mmpms_pdus";
inline constexpr std::string_view TABLE_WIN         = "wbxWIN";
inline constexpr std::string_view TABLE_DOMAIN      = "wbxDomain";
inline constexpr std::string_view TABLE_ZONE        = "wbxZone";
inline constexpr std::string_view TABLE_SITE        = "wbxSite";

/ Database and DBResult pointers /
extern std::unique_ptr pdb;
/ ip /
extern std::unique_ptr pRes_Ippkt1;
extern std::unique_ptr pRes_Ippkt2;
extern std::unique_ptr pRes_IpSta_TCP;
extern std::unique_ptr pRes_IpSta_UDP;
extern std::unique_ptr pRes_IpSta_ICMP;
extern std::unique_ptr pRes_IpSta_OTHER;
/ http /
extern std::unique_ptr pRes_HttpPage;
/ tp /
extern std::unique_ptr pRes_TpConn;
extern std::unique_ptr pRes_TpPdu;
extern std::unique_ptr pRes_TpDisconn;
/ arm /
extern std::unique_ptr pRes_ArmConn;
extern std::unique_ptr pRes_ArmPdu;
/ conference manager /
extern std::unique_ptr pRes_CmgPdu;
/ meeting /
extern std::unique_ptr pRes_MtgConn;
extern std::unique_ptr pRes_conf;
/ server performance /
extern std::unique_ptr pRes_appsvr;
extern std::unique_ptr pRes_SvrSta;

#endif // GLOBAL_H

//BY GST ARMV8 GCC 13.2