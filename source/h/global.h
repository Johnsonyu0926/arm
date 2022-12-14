/*
 * $Id: doorsbases.h,v 1.31.2.1 2001/01/12 00:51:42 billy Exp $
 *
 * Author: Billy Shi (dxshi@yahoo.com)
 *
 * Copyright(C) 2001-2010 Doors Internet Software Co.,Ltd. 
 *
 */
#ifndef __GLOBAL_H__
#define __GLOBAL_H__
#include "database.h"

#define	INTERVAL_TIME		150
/*
#define DATABASE_DEFAULT	"szDB"
#define HOST_NAME		"192.168.2.197"
#define PORT			"0"
#define USER_NAME		"billy"
#define	PASSWORD		NULL

#define HTTP_PORT		"80"
#define TCP_PORT		"1270"
*/

/*ip*/
#define TABLE_IPPKT1		"ip_packet"
#define TABLE_IPPKT2		"ip_packet2"
#define TABLE_IPSTA_TCP		"ip_tcp_sta"
#define TABLE_IPSTA_ICMP	"ip_icmp_sta"
#define TABLE_IPSTA_UDP		"ip_udp_sta"
#define TABLE_IPSTA_OTHER	"ip_other_sta"
/* tcp */
#define TABLE_TCPINFO       	"tcp_conn_sta"
/* http */
#define TABLE_HTTPPAGE		"http_page_sta"
/*tp*/
#define TABLE_TPCONN		"tp_conn_sta"
#define TABLE_TPPDU		"tp_pdus"
#define TABLE_TPDISCONN		"tp_disconn_sta"

/*arm*/
		
#define TABLE_ARMCONN		"arm_conn_sta"
#define TABLE_ARMPDU		"arm_pdus"

/*conference mananger*/
#define TABLE_CMGPDU		"cmg_pdus"

/*meeting*/
#define TABLE_MTG_CONN		"mtg_conn_sta"

#define TABLE_APPSVR		"appsvr_trarte"

#define TABLE_SVRSTA     	"mtg_svrstatus"

#define TABLE_CPING_PDUS  	"mz_cping_pdus"
#define TABLE_MMCB_PDUS  	"mz_mmcb_pdus"
#define TABLE_APPS_PDUS		"mz_mmapp_pdus"       
#define TABLE_FCS_PDUS		"mz_mmfcs_pdus"
#define TABLE_PMS_PDUS		"mz_mmpms_pdus"
  


//
#define TABLE_WIN		"wbxWIN"
#define TABLE_DOMAIN		"wbxDomain"
#define TABLE_ZONE		"wbxZone"
#define TABLE_SITE		"wbxSite"

extern Database *pdb ;
/*ip*/ 
extern DBResult *pRes_Ippkt1 ;
extern DBResult *pRes_Ippkt2 ;
extern DBResult *pRes_IpSta_TCP ;
extern DBResult *pRes_IpSta_UDP;
extern DBResult *pRes_IpSta_ICMP;
extern DBResult *pRes_IpSta_OTHER;

/*http*/
extern DBResult * pRes_HttpPage;
/*tp*/
extern DBResult *pRes_TpConn ;
extern DBResult *pRes_TpPdu ;
extern DBResult *pRes_TpDisconn;

/*arm*/
extern DBResult *pRes_ArmConn ;
extern DBResult *pRes_ArmPdu;

/*conference mananger*/
extern DBResult *pRes_CmgPdu ;

/*meeting*/
extern DBResult *pRes_MtgConn   ;
extern DBResult *pRes_conf;

/*server performance*/
extern DBResult *pRes_appsvr;

extern DBResult* pRes_SvrSta ;

#endif

