#ifndef __CAUDIT_CONFIGER_H__
#define __CAUDIT_CONFIGER_H__

class CAuditConfiger
{
public:
	CDsList* GetAttaList(){
		return m_pAttaList;
	}

	CDsList* GetContList(){
		return m_pContList;
	}
	int GetAttaCheckMode() {
		return m_nAttaCheckMode;
	}

	int GetContCheckMode(){
		return m_nContCheckMode;
	}

public:
	CAuditConfiger(){
		m_pAttaList = new CDsList();
		m_pContList = new CDsList();
	}
	~CAuditConfiger(){
		if(m_pAttaList)
		{
			for(int i=0;i<m_pAttaList->GetCount();i++)	
			{
				char* pAtta = (char*)m_pAttaList->ElementAt(i);
				delete []pAtta;
	
			}	
			m_pAttaList->RemoveAll();
			delete m_pAttaList;	
			m_pAttaList = NULL;
		}
		if(m_pContList)	
		{
			for(int i=0;i<m_pContList->GetCount();i++)	
			{
				char* pCont = (char*)m_pContList->ElementAt(i);
				delete []pCont;
	
			}	
			m_pContList->RemoveAll();
			delete m_pContList;	
			m_pContList = NULL;
			
		}
	}

private:
	CDsList* m_pAttaList;
	CDsList* m_pContList;
	int m_nAttaCheckMode;
	int m_nContCheckMode;
	

public:
	BOOL Init(char* szConf);
};
#endif
