#ifndef __CQSEARCH_H__
#define __CQSEARCH_H__
#include <qsearch.h>
#include "doorsbase.h"
class CQSearch
{
public:
	CQSearch(){}
	~CQSearch() {
		
		qsearch_destructor(m_obj);
		DS_TRACE("[CQSearch::~CQSearch] unload the plugin...");
		qsearch_unload_plugin(m_plugin);
	}
public:
	void AddContent(char* szContent) ;
	//after add content, Init to load the plugin... 
	BOOL Init(char* szPlugin,char* szAlgo,char* szFile);

	//Now , we can call this interface to search what you want.

	BOOL Search(char* pBuf,int nSize,char* szFoundKey,int nSize);

private:
	static int cb(void *cb_data, void *ptrn_data, size_t match_offset);
	qsearch_plugin_t* m_plugin;

	qsearch_algo_t *m_algo, **m_list;
	qsearch_obj_t *m_obj;

	static BOOL m_bFound;
	static char m_szFoundKey[128];
};
#endif
