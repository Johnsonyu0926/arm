/*
 * Copyright (c) 1999,2000,2001. Zhe Jian University
 * All rights reserved
 *
 * Use of this software is governed by the terms of the license agreement
 * for the Zhe Jian University.
 */

/*
 * Filename	: cldap.h	class CLdap head file
 * Comment	: class CLdap head file
 * Author	: Shi dongxue
 * Date		: Sun Aug 22 19:17:41 CST 1999
 */

#ifndef __CLDAP_H__
#define __CLDAP_H__

#include <lber.h>
#include <ldap.h>
//#include "unixdef.h"
//#include "cemailuserinfo.h"

#include "doorsbase.h"

#define AUTH_SUCCESS		0
#define AUTH_USER_NOT_EXIST	1
#define AUTH_PASSWORD_ERROR	2
#define AUTH_LDAP_SERVER_ERROR	3



#define 	LDAP_PORT		389
#define		LDAP_HOST		"192.168.100.194"
#define 	BASE			"o=doors,c=cn"
#define 	ADMIN_BINDDN		"cn=admin,o=doors,c=cn"
#define		ADMIN_PASSWD		"admin123"
#define		HTTP_PROXY	0x01
#define 	HOME_PROXY	0X02
#define 	BASE_DOMAIN	"o=doors,c=cn"
#define		GROUP_BASE	"ou=Groups,o=zju,c=CN"
#define 	HTTP_GROUP_DN	"cn=httpgrp,ou=Groups,ou=zju,o=edu,c=cn"
#define 	HOME_GROUP_DN	"cn=domesticgrp,ou=Groups,ou=zju,o=edu,c=cn"
#define 	FTP_GROUP_DN	"cn=ftpgrp,ou=Groups,ou=zju,o=edu,c=cn"
#define 	SOCKS_GROUP_DN	"cn=socksgrp,ou=Groups,ou=zju,o=edu,c=cn"
class CLdap
{
	public:
		CLdap():m_pLdap(NULL),
			m_pMessage(NULL)
		{
			strcpy(m_szBase,BASE);
			strcpy(m_szLdapHost,LDAP_HOST);
			m_nLdapPort = LDAP_PORT;
			strcpy(m_szAdminBindDN,ADMIN_BINDDN);
			strcpy(m_szAdminPasswd,ADMIN_PASSWD);
		}
		~CLdap(){}
		/* comment:
		 * 	Contract and descontract.
		 */

	public:
		int Init(IN char* szHost=LDAP_HOST,
			IN int nPort=LDAP_PORT,
			IN char* szBindDN=ADMIN_BINDDN,
			IN char* szPswd=ADMIN_PASSWD,
			IN int nAuthMethod=LDAP_AUTH_SIMPLE);
		/* comment:
		 * 	One step to init the ldap.
		 * 	LDAP_AUTH_SIMPLE mean clear text-- not cript.
		 */

		int LdapSearchEx(IN char* szBaseDN,IN char* szFilter,
			OUT LDAPMessage** pRet,OUT char* szDN);

		int LdapSearch(IN char* szBaseDN,IN char* szFilter,
			OUT LDAPMessage** pRet);
		/* comment:
		 * 	Retrun value is the message id.
		 * 	The search respone is stored in LDAPMessage.
		 */


		int LdapSearch(IN char* szFilter);
		void ShowProperties(LDAP* ld,char* dn);

		void PrintMessage();
		/* comment:
		 * 	Print the informantion stored in LDAPMessage which is 
		 * respone to LdapSearch.
		 */ 

		int LdapModify(char* szModifyDN,LDAPMod** pMods);
		/* comment:
		 *	1).It is a sync function
		 *	2).return value ldapresult code:LDAP_SUCCESS is OK!
		 * Other return code refered to ldap.h or RFC1777
		 */


		int LdapAdd(char* szModifyDN,LDAPMod* pAttrs[]);

		void FreeMods(LDAPMod** pMods);

		int LdapDelete(char* szDN);
		int LdapModifyDN(char* szOldDN,char* szNewDN);

		/* ------------------------------------------------- */
		/* comment:
		 *	The follow is use to the user management.
		 */

		//int AddNewEmailUser(IN CEmailUserInfo* pUser);
		/* Comment:
		 * 	Add one new email user to the ldap database.
		 *	The function requires the CEmailUserInfo instance which
		 * defined in cemailuserinfo.h.
		 */

		int DeleteUser(IN char* szCommonName,//who
				IN char* szOu);	//origanize uint
				
		
		int ModifyUserAttribute(char* szDN,	//who
					char* szAttr,	//the attr to be modify
					char* szValue); //the value of the attr
		/* Comment:
		 * 	From szCommonName we can contruct the DN for the user
		 * default DN = cn=szCommonName,BASE("ou=People,o=zju,c=CN")
		 * 	szAttr must be the well know attribute define in the 
		 * file sims:/etc/opt/SUNWmail/ldap/current/slapd.at.conf
		 */ 	

		int ModifyUserAttribute(char* szCommonName,	//who
					char* szOuName,	//what ou
					char* szAttr,	//the attr to be modify
					char* szValue); //the value of the attr
		/* Comment:
		 * 	From szCommonName we can contruct the DN for the user
		 * default DN = cn=szCommonName,BASE("ou=People,o=zju,c=CN")
		 * 	szAttr must be the well know attribute define in the 
		 * file sims:/etc/opt/SUNWmail/ldap/current/slapd.at.conf
		 */ 	
		int AddAttr(char* szBaseDN,char* szAttrType,
			    char* szAttrValue);
		int AddMemberInProxyGroup(char* szMemberDN,long nStatus);

		int DisableUserHttp(char* szCN,char* szOu,int nProxyType=HTTP_PROXY);
		int EnableUserHttp(char* szCN,char* szOu,int nProxyType=HTTP_PROXY);

		int EnableUserHttp(char* szDN,int nProxyType=HTTP_PROXY);
		int DisableUserHttp(char* szDN,int nProxyType=HTTP_PROXY);

		/* ------------------------------------------------- */	

		int LdapUnBind();
		/* Comment:
		 *	Release the LDAP* var which has been bind.
		 */

		BOOL IsExistUserID(char* szUserID);		

		int LdapAddOu(char* szOuName,char* szBaseDN="ou=zju,o=edu,c=cn");
		LDAP* m_pLdap;


		int Auth(char* szUID,char* szPasswd);
		//用户验证

	private:
		
		LDAP* LdapOpen(char* szHost,int nPort=389);
		//Init the library and connect to the LDAP server.
			
		int LdapBind(char* szBindDN,char* szPasswd,
		int nAuthMethod=LDAP_AUTH_SIMPLE);

		LDAPMessage* m_pMessage;

		/* 	
		 * Comment:
		 * 	add in 99-9-1
		 */
		int GetEmail(IN char* szUid,IN char* szOuName,OUT char* szMail);
		int GetRFC822Proxy(IN char* szUid,OUT char* szRFC822Proxy);
		int GetRFC822Zjuem(IN char* szUid,OUT char* szRFC822Zjuem);
		
		int GetDNFromCN(IN char* szCN,IN char* szOuName,OUT char* szDN);
		int LdapDeleteAttr(char* szBaseDN,char* szType,char* szValue);


		//添加ＯＵ时用,1999-11-16

		int GetDCDN(char* szNewOu,char* szBaseDN,char* szDCDN);
		int GetURIAttr(char *szNewOU,char* szBaseDN,char* szURI);
		int GetAsName(char* szNewOU,char* szBaseDN,char* szAsName);
		int GetDesc(char* szNewOU,char* szBaseDN,char* szDesc);
		int GetDN(char* szNewOU,char* szBaseDN,char* szNewDN);
		int GetDC(char* szBase,char* szDC);
		int AddOUDN(char* szNewOU,char* szBaseDN);
		int AddDCDN(char* szNewOU,char* szBaseDN);	

	public:
		int StopEmail(char* szCN,char* szOU);
		int EnableEmail(char* szCN,char* szOU);
	private:
		char m_szLdapHost[32];
		int	m_nLdapPort;
		char	m_szAdminBindDN[256];
		char 	m_szAdminPasswd[32];
		char	m_szBase[256];

	public:
	
	int SetParam(char* szLdapHost,int nLdapPort,char* szBase,char* szAdminBindDN,char* szAdminPasswd);
};

#endif	//!__CLDAP_H__
