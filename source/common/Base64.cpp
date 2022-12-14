// Base64.cpp: implementation of the CBase64 class.

#include "doorsbase.h"
#include "Share.h"
#include "memfile.h"
#include "cstring.h"


#include "Base64.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////windows function realization
int WideCharToMultiByte(UINT codepage, DWORD dwFlags, LPCWSTR wide, int widelen, LPSTR multi,
				int multilen, LPCSTR defchar, LPBOOL fuseddefault)
{
	short *cp;
	char *buffer = multi;
	int width = widelen;
	int size =0;
	
	if (fuseddefault)
		*fuseddefault=0;
		
	if (width==0)
//		width= lstrlen((short*)wide);

	for (cp=(short*)wide; width && *cp; width--)
	{
		if (*cp <256)
		{
			if (multi)
				*buffer=(*cp & 0xff);
		}
		else
		{
			if (multi)
				*buffer = defchar?*defchar:'?';
			if (fuseddefault)
				*fuseddefault=1;
		}

		cp++;
		buffer++;
		size++;
	}

	if (multi)
		*buffer =0;

	size+=1;

	return size;
}

int MultiByteToWideChar(UINT page, DWORD flags, LPCSTR src, int srcLen, LPWSTR dst, int dstlen)
{
	if (srcLen ==-1)
//		srcLen =lstrlen(src);

	if (!dst)
		return srcLen*2;

//	lstrcpynAtoW(dst, src ,srcLen);

	return srcLen*2;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CString CBase64::m_sBase64Alphabet( "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/" );

int CBase64::m_nMask[] = { 0, 1, 3, 7, 15, 31, 63, 127, 255 };


CBase64::CBase64()
{

}

CBase64::~CBase64()
{

}

int CBase64::DecodeString(char *szSrc, CString& strDecoded)
{
	strDecoded.Empty();//="";
	strDecoded="";

	char* p=szSrc;

	int nLength=strlen(szSrc);

	if(strstr(szSrc,"\r\n"))
		nLength-=2;

	if(nLength%4)
	{
		//AfxMessageBox("Error encoding!\n");
		return FALSE;
	}

	char sz6Bits[5];

	char szTmp[10];

	//CString strDecoded;
	for(int i=0;i<nLength;)
	{
		
		strncpy(sz6Bits,p,4);
		sz6Bits[4]='\0';
		if(!Decode(sz6Bits,szTmp))
		{
			//AfxMessageBox("Decoding failed! please check the sz6bits!");
			return FALSE;
		}

		strDecoded+=szTmp;
		p+=4;
		i+=4;
	}

	
	return TRUE;
}

int CBase64::AsciiTo6Bits(char cAsc)
{
	if(cAsc>='A' && cAsc<='Z')
		return cAsc-'A';

	if(cAsc>='a' && cAsc<='z')
		return (cAsc-'a')+('Z'-'A'+1);
	
	if(cAsc>='0' && cAsc<='9')
		return (cAsc-'0')+('Z'-'A'+1)+('z'-'a'+1);

	if(cAsc=='+')
		return 62; 
	if(cAsc=='/')
		return 63;

	return -1;
}

int CBase64::Decode(const char *p6Bits, char *szDecoded)
{

	char cFirst6Bits=AsciiTo6Bits(p6Bits[0]);//18;//'S';
	char cSecond6Bits=AsciiTo6Bits(p6Bits[1]);//52;//'0';

	char cTempHigh,cTempLow;
	char cFirstLetter,cSecondLetter,cThirdLetter;

	cTempHigh=(cFirst6Bits<<2);
	cTempLow=cSecond6Bits>>4;
	cFirstLetter=cTempHigh|cTempLow;
	//求出第一个OK


	if(p6Bits[2]=='=')
	{
		sprintf(szDecoded,"%c",cFirstLetter);
		szDecoded[1]='\0';
		return TRUE;		
	}

	char cThird6Bits=AsciiTo6Bits(p6Bits[2]);//21;//'V';

	if(cThird6Bits<0)
		return FALSE;

	cTempHigh=cSecond6Bits<<4;
	cTempLow=cThird6Bits>>2;
	cSecondLetter=cTempHigh|cTempLow;

	if(p6Bits[3]=='=')
	{
		sprintf(szDecoded,"%c%c",cFirstLetter,cSecondLetter);
		szDecoded[2]='\0';//
		return TRUE;
	}

	char cFourth6Bits=AsciiTo6Bits(p6Bits[3]);//'O';
	if(cFourth6Bits<0)
		return FALSE;

	cTempHigh=cThird6Bits<<6;
	cThirdLetter=cTempHigh|cFourth6Bits;
	
	sprintf(szDecoded,"%c%c%c",cFirstLetter,cSecondLetter,cThirdLetter);
	
	szDecoded[3]='\0';
	
	return TRUE;
		

}
/*
int CBase64::RecvBodyAndDecode(CString strFileName,
							   CString strBoundary)
{
	char szTmpBoundary[1024];
	strcpy(szTmpBoundary,LPCTSTR(strBoundary));

	//结束标记：nEndFlag
	//一般为\r\n\r\n结束
	//如果该行不足76个字符+\r\n
	//如果连续两个\r\n

	char szBuf[4096+1];
	int nRecv=0;
	CString strTmp;
	CString strBody;
	//BOOL bFoundBoundary=FALSE;
	BOOL bEndBody=FALSE;
	while(1)
	{

		nRecv=m_pClientSocket->GetLine(szBuf,4096);
		szBuf[nRecv]='\0';

		//strTmp=szBuf;

		//if(strTmp.Find(strBoundary)>=0)
		//	break;
		//if(strstr(szBuf,LPCTSTR(strBoundary)))
		//	break;

		
		if(strBoundary.GetLength()>0)
		{
			if(strstr(szBuf,LPCTSTR(strBoundary)))
			{
				
				break;
			}
		}
		//再取下一个的分界线
		
		
		if(strncmp(szBuf,"\r\n",2)==0)
		{
			bEndBody=TRUE;
			if(strBoundary.GetLength()==0)			
				break;
		}
		else
		{
			if(strlen(szBuf)<78)
			{
				strBody+=szBuf;
				bEndBody=TRUE;
				if(strBoundary.GetLength()==0)				
				break;
					//每行为78个字符
			}

		}

		if(!bEndBody)
			strBody+=szBuf;
	}

	
	Decode(strBody,strFileName);
	return TRUE;
}

CBase64::CBase64(CStreamSocket *pSocket)
{

	m_pClientSocket=pSocket;
}
*/
int CBase64::Decode(CString strBody, CString strFileName)
{
	FILE* fp1;
	fp1=fopen("body.txt","w");
	if(!fp1)
		return FALSE;
	fputs(LPCTSTR(strBody),fp1);
	fclose(fp1);
	
	/*

	int i=0;
	char szSrc[5];
	CString strDecoded;//解码后的串存放地

	FILE* fp;
	fp=fopen(LPCTSTR(strFileName),"w");
	if(!fp)
		return FALSE;

	char szDecoded[1024];

	int n=strBody.GetLength();

	if(strBody.Find("\r\n\r\n")>=0)
		//AfxMessageBox("Include new line!\n");

	while(i<n)
	{
		

		if(strBody[i]=='\r')
			i+=2;
		if(i>=strBody.GetLength())
			break;//正好在最后
		else
			if(strBody[i]=='\r')
				break;

		szSrc[0]=strBody[i];

		i++;
		if(i>=n)
			break;//正好在最后

		szSrc[1]=strBody[i];
		i++;
		if(i>=n)
			break;//正好在最后

		szSrc[2]=strBody[i];
		i++;
		if(i>=n)
			break;//正好在最后

		szSrc[3]=strBody[i];
		szSrc[4]='\0';
		
		Decode(szSrc,szDecoded);
		fputs(szDecoded,fp);

	}

	fclose(fp);
	*/

	FILE* fp;
	fp=fopen("body.txt","r");
	if(!fp)
		return FALSE;
	char szBuf[1024+1];

	CString strDecoded;

	CString strBodyDecoded;


	FILE* fp2;
	fp2=fopen(strFileName,"wb");

	if(!fp2)
		return FALSE;
	else
	{
		while(fgets(szBuf,1024,fp))
		{
			DecodingString(szBuf,fp2);			
		}
	}
	fclose(fp2);

	/*
	while(fgets(szBuf,1024,fp))
	{
		DecodeString(szBuf,strDecoded);
		strBodyDecoded+=strDecoded;
	}
*/


	fclose(fp);

/*
	if(strFileName.GetLength()>0)
	{
		//save in file
		FILE* fp2;
		fp2=fopen(LPCTSTR(strFileName),"w");
		if(!fp2)
			return FALSE;

		fputs(strBodyDecoded,fp2);
		fclose(fp2);

	}
	else
	{
		SetBodyText(strBodyDecoded);
	}

*/

	return TRUE;
}




int CBase64::SetBodyText(CString strBody)
{
	m_strBodyText=strBody;

	return TRUE;
}

int CBase64::GetBodyText(CString &strBodyText)
{

	strBodyText=m_strBodyText;
	return TRUE;
}

int CBase64::Decode(char *p6Bits, FILE *fp)
{

	
	char cFirst6Bits=AsciiTo6Bits(p6Bits[0]);//18;//'S';
	char cSecond6Bits=AsciiTo6Bits(p6Bits[1]);//52;//'0';

	char cTempHigh,cTempLow;
	char cFirstLetter,cSecondLetter,cThirdLetter;

	cTempHigh=(cFirst6Bits<<2);
	cTempLow=cSecond6Bits>>4;
	cFirstLetter=cTempHigh|cTempLow;
	//求出第一个OK


	fputc(cFirstLetter,fp);
	if(p6Bits[2]=='=')
	{
		//sprintf(szDecoded,"%c",cFirstLetter);
		//szDecoded[1]='\0';
		
		return TRUE;		
	}

	char cThird6Bits=AsciiTo6Bits(p6Bits[2]);//21;//'V';

	if(cThird6Bits<0)
		return FALSE;

	cTempHigh=cSecond6Bits<<4;
	cTempLow=cThird6Bits>>2;
	cSecondLetter=cTempHigh|cTempLow;

	fputc(cSecondLetter,fp);
	if(p6Bits[3]=='=')
	{
		//sprintf(szDecoded,"%c%c",cFirstLetter,cSecondLetter);
		//szDecoded[2]='\0';//
		
		return TRUE;
	}

	char cFourth6Bits=AsciiTo6Bits(p6Bits[3]);//'O';
	if(cFourth6Bits<0)
		return FALSE;

	cTempHigh=cThird6Bits<<6;
	cThirdLetter=cTempHigh|cFourth6Bits;
	
	//sprintf(szDecoded,"%c%c%c",cFirstLetter,cSecondLetter,cThirdLetter);
	
	//szDecoded[3]='\0';
	fputc(cThirdLetter,fp);
	
	return TRUE;

}

int CBase64::DecodingString(char *szSrc, FILE *fp)
{

	
	char* p=szSrc;

	int nLength=strlen(szSrc);

	nLength-=2;
	if(nLength%4)
	{
		//AfxMessageBox("Error encoding!\n");
		return FALSE;
	}

	char sz6Bits[5];

	
	//char szTmp[10];

	//CString strDecoded;
	int i=0;
	while(i<nLength)
	{
	
		strcpy(sz6Bits,"");
		strncpy(sz6Bits,p,4);
		sz6Bits[4]='\0';
		Decode(sz6Bits,fp);
		p+=4;
		i+=4;
	}

	
	return TRUE;
}

int CBase64::DecodeBody(CString strSrc, CString &strDecoded)
{

	strDecoded.Empty();
	strDecoded="";

	CMemFile mmfile;
	if(!DecodeBody(strSrc,mmfile))
	{
		DS_TRACE("<SYSERR/CBase64::DecodedBody>: DecodedBody return FALSE!\n");

		return FALSE;
	}

	int nRead;
	mmfile.SeekToBegin();
	char szBuf[BUF_MAX+1];
	while(1)
	{

		nRead=mmfile.Read(szBuf,BUF_MAX);

		szBuf[nRead]='\0';
		strDecoded+=szBuf;

		if(nRead<BUF_MAX)
		{
			DS_TRACE("<SYSOK/CBase64::DecodedBody>: Read from mmfile finished!\n");

			break;
		}

	}
	return TRUE;
}

int CBase64::DecodeStr(LPCTSTR szDecoding, LPTSTR szOutput)
{
	CString sInput;
    int c, lp =0;
	int nDigit;
    int nDecode[ 256 ];

	ASSERT( szDecoding != NULL );
	ASSERT( szOutput != NULL );
	if( szOutput == NULL )
		return 0;
	if( szDecoding == NULL )
		return 0;
	sInput = szDecoding;
	if( sInput.GetLength() == 0 )
		return 0;

	// Build Decode Table
	//
	int i=0;
	for( i = 0; i < 256; i++ ) 
		nDecode[i] = -2; // Illegal digit
	for( i=0; i < 64; i++ )
	{
		nDecode[ m_sBase64Alphabet[ i ] ] = i;
		nDecode[ m_sBase64Alphabet[ i ] | 0x80 ] = i; // Ignore 8th bit
		nDecode[ '=' ] = -1; 
		nDecode[ '=' | 0x80 ] = -1; // Ignore MIME padding char
    }

	// Clear the output buffer
	memset( szOutput, 0, sInput.GetLength() + 1 );

	// Decode the Input
	//
	for( lp = 0, i = 0; lp < sInput.GetLength(); lp++ )
	{
		c = sInput[ lp ];
		nDigit = nDecode[ c & 0x7F ];
		if( nDigit < -1 ) 
		{
			return 0;
		} 
		else if( nDigit >= 0 ) 
			// i (index into output) is incremented by write_bits()
			write_bits( nDigit & 0x3F, 6, szOutput, i );
    }	
	return i;

}

void CBase64::write_bits(UINT nBits,
						 int nNumBits,
						 LPTSTR szOutput,
						 int& i)
{
	UINT nScratch;

	m_lBitStorage = (m_lBitStorage << nNumBits) | nBits;
	m_nBitsRemaining += nNumBits;
	while( m_nBitsRemaining > 7 ) 
	{
		nScratch = m_lBitStorage >> (m_nBitsRemaining - 8);
		szOutput[ i++ ] = nScratch & 0xFF;
		m_nBitsRemaining -= 8;
	}
}

int CBase64::Decode(const char* p6Bits,char* szDecoded,int& nBytes/*返回字节数*/)
{
	
	char cFirst6Bits=AsciiTo6Bits(p6Bits[0]);//18;//'S';
	char cSecond6Bits=AsciiTo6Bits(p6Bits[1]);//52;//'0';

	char cTempHigh,cTempLow;
	char cFirstLetter,cSecondLetter,cThirdLetter;

	cTempHigh=(cFirst6Bits<<2);
	cTempLow=cSecond6Bits>>4;
	cFirstLetter=cTempHigh|cTempLow;
	//求出第一个OK

	szDecoded[0]=cFirstLetter;
	//保存


	if(p6Bits[2]=='=')
	{
		//sprintf(szDecoded,"%c",cFirstLetter);
		

		szDecoded[1]='\0';
		nBytes=1;
		return TRUE;		
	}

	char cThird6Bits=AsciiTo6Bits(p6Bits[2]);//21;//'V';

	if(cThird6Bits<0)
		return FALSE;

	cTempHigh=cSecond6Bits<<4;
	cTempLow=cThird6Bits>>2;
	cSecondLetter=cTempHigh|cTempLow;


	szDecoded[1]=cSecondLetter;
	//保存

	if(p6Bits[3]=='=')
	{
		//sprintf(szDecoded,"%c%c",cFirstLetter,cSecondLetter);
		
		szDecoded[2]='\0';//
		nBytes=2;
		return TRUE;
	}

	char cFourth6Bits=AsciiTo6Bits(p6Bits[3]);//'O';
	if(cFourth6Bits<0)
		return FALSE;

	cTempHigh=cThird6Bits<<6;
	cThirdLetter=cTempHigh|cFourth6Bits;
	
	//sprintf(szDecoded,"%c%c%c",cFirstLetter,cSecondLetter,cThirdLetter);
	szDecoded[2]=cThirdLetter;

	szDecoded[3]='\0';//end flag

	nBytes=3;
	return TRUE;		
}
int CBase64::DecodeString(char* szSrc,char* szDecoded,int& nLineBytes)
{
	
	if(szSrc==NULL || szDecoded==NULL)
	{
		return FALSE;
	}

	nLineBytes=0;

	char* p=szSrc;

	int nLength=strlen(szSrc);

	if(strstr(szSrc,"\r\n"))
		nLength-=2;

	if(nLength%4)
	{
		////AfxMessageBox("Error encoding!\n");
		return FALSE;
	}

	char sz6Bits[5];

	char szTmp[10];

	//CString strDecoded;

	
	for(int i=0;i<nLength;)
	{
		
		strncpy(sz6Bits,p,4);
		sz6Bits[4]='\0';

		int nBytes=0;
		if(!Decode(sz6Bits,szTmp,nBytes))
		{
			//AfxMessageBox("Decoding failed! please check the sz6bits!");
			return FALSE;
		}

		//strDecoded+=szTmp;
		memcpy(szDecoded+nLineBytes,szTmp,nBytes);
		nLineBytes+=nBytes;
		p+=4;
		i+=4;
	}

	
	return TRUE;
}

int CBase64::DecodeBody(CString strSrc,CMemFile& mmfile)
{
	//分解的结果存入CMemFile& mmfile中
	
	//中间暂存
	/*
	FILE* fp;
	fp=fopen("bodysrc.txt","w");
	fputs(strSrc,fp);
	fclose(fp);
*/

	//strDecoded.Empty();
	//strDecoded="";
	
	CString strBuf;
	int nFind=0;
	//char szDecoded[1024];



	//CMemFile mmfile;



	while((nFind=strSrc.Find("\r\n"))>0)
	{
		//开始处理一个串

		char szBuf[77];
		memset(szBuf,0,77);

		strcpy(szBuf,(LPCTSTR)strSrc.Mid(0,nFind));
		szBuf[nFind]='\0';

		//if(!DecodeStr(szBuf,szDecoded))
		//	return FALSE;
		
		//strDecoded+=szDecoded;
		int nLineBytes=0;
		char szDecoded[57];
		memset(szDecoded,0,57);


		if(!DecodeString(szBuf,szDecoded,nLineBytes))
		{
			DS_TRACE("<SYSERR/CBase64::DecodeBody>: Error calling DecodeString()\n");
			return FALSE;
		}

		mmfile.Write(szDecoded,nLineBytes);
		//存入CMemFile对象中


		strSrc=strSrc.Mid(nFind+2);//截去前串

	}

	return TRUE;

	//good!
}

//对一个字符串进行编码，编码结果存入CString对象中返回
CString CBase64::Encode(LPCTSTR szEncoding, int nSize)
{
	CString sOutput;
	int nNumBits = 6;
	UINT nDigit;
	int lp = 0;

	ASSERT( szEncoding != NULL );
	if( szEncoding == NULL )
		return sOutput;
	m_szInput = szEncoding;
	m_nInputSize = nSize;

	m_nBitsRemaining = 0;
	nDigit = read_bits( nNumBits, &nNumBits, lp );
	while( nNumBits > 0 )
	{
		sOutput += m_sBase64Alphabet[ (int)nDigit ];
		nDigit = read_bits( nNumBits, &nNumBits, lp );
	}
	while( sOutput.GetLength() % 4 != 0 )
	{
		sOutput += '=';
	}
	return sOutput;
}
UINT CBase64::read_bits(int nNumBits, int * pBitsRead, int& lp)
{
    ULONG lScratch;
    while( ( m_nBitsRemaining < nNumBits ) && 
		   ( lp < m_nInputSize ) ) 
	{
		int c = m_szInput[ lp++ ];
        m_lBitStorage <<= 8;
        m_lBitStorage |= (c & 0xff);
		m_nBitsRemaining += 8;
    }
    if( m_nBitsRemaining < nNumBits ) 
	{
		lScratch = m_lBitStorage << ( nNumBits - m_nBitsRemaining );
		*pBitsRead = m_nBitsRemaining;
		m_nBitsRemaining = 0;
    } 
	else 
	{
		lScratch = m_lBitStorage >> ( m_nBitsRemaining - nNumBits );
		*pBitsRead = nNumBits;
		m_nBitsRemaining -= nNumBits;
    }
    return (UINT)lScratch & m_nMask[nNumBits];
}

//解码：UTF7
int CBase64::DecodeEx(const char *szSrc, int nSize, BYTE *pBuf)
{

	int nTotal=0;

	/*
	CString strDecodeEx=szSrc;
	int nLack=nSize-(nSize/4)*4;
	for(int i=0;i<nLack;i++)
	{
		strDecodeEx.SetAt(i+nSize,'A');
	}
	*/
	char szTmp[1024];
	strcpy(szTmp,szSrc);

	int nLack=nSize-(nSize/4)*4;
	int i=0;
	for(i=0;i<nLack;i++)
	{
		strcat(szTmp,"A");
	}
	CString strDecodeEx((const char*)szTmp);

	const char* p=(LPCTSTR)strDecodeEx;
	
	
	int nLen=strDecodeEx.GetLength();
	for(i=0;i<nLen;i++)
	{
		char sz6Bits[4];
		strncpy(sz6Bits,p,4);
		char szDecoded[3];
		int nBytes=0;
		Decode(sz6Bits,szDecoded,nBytes);
		memcpy(pBuf+nTotal,szDecoded,nBytes);
		p+=4;
		nTotal+=nBytes;
	}
	return nTotal;
}

CQp::CQp()
{

}

CQp::~CQp()
{

}

int CQp::Decode(CString strSrc, CString& strDecoded)
{

	strDecoded.Empty();
	strDecoded="";

	CMemFile mmfile;
	Decode(strSrc,mmfile);
	char szBuf[1025];
	mmfile.SeekToBegin();
	while(1)
	{
		int nRead=mmfile.Read(szBuf,1024);
		szBuf[nRead]='\0';
		strDecoded+=szBuf;
		if(nRead<1024)
			break;
	}

	return TRUE;

	/*
	char szSrc[4096];
	strncpy(szSrc,(LPCTSTR)strSrc,4095);

	char szDecoded[4096];

	int i=0,j=0;
	for(;szSrc[i];i++,j++)
	{

		if( (szSrc[i]>=33 && szSrc[i]<=60) ||
			(szSrc[i]>=62 && szSrc[i]<=126) )
		{
			szDecoded[j]=szSrc[i];
			
			continue;
		}

		if(szSrc[i]=='=')
		{
			i++;
			if(szSrc[i]=='\r')
			{
				i++;//skip '\n'
				j--;
				continue;
			}
			else
			{		
			
				char szLeft[3];
				memset(szLeft,0,3);

				char szRight[3];
				memset(szRight,0,3);

				//i++;
				szLeft[0]=szSrc[i];//3
				i++;
				szLeft[1]=szSrc[i];//D
				
				//szDecoded[j]='=';
				GetCharByHex(szLeft,szDecoded[j]);

				if(szDecoded[j]=='=')
				{
					
					continue;
				}

				i++;//=
				i++;//
				szRight[0]=szSrc[i];//3
				i++;
				szRight[1]=szSrc[i];//D
				
				j++;
				GetCharByHex(szRight,szDecoded[j]);

				continue;

			}
				
		}

		if(szSrc[i]=='\r')
		{
			//continue;
			i++;
			if(szSrc[i]=='\n')
			{
				szDecoded[j]=szSrc[i];
			}
		}

		

		if(szSrc[i]==' '|| szSrc[i]=='	')
		{
			szDecoded[j]=szSrc[i];
		}

	}

	j++;
	szDecoded[j]='\0';
	strDecoded=szDecoded;

	return TRUE;
	*/
}

int CQp::GetCharByHex(char *szHex, char &szChar)
{
	int nHex0=GetHex(szHex[0]);
	int nHex1=GetHex(szHex[1]);
	if(nHex0==-1 || nHex1==-1)
		return FALSE;

	int nChar=nHex0*16+nHex1;

	szChar=nChar;

	return TRUE;
	
}

int CQp::GetHex(char szHex)
{
	if(szHex<='9' && szHex>='0')
	{
		return szHex-'0';
	}

	if(szHex<='F' && szHex>='A')
	{
		return 10+szHex-'A';
	}

	return -1;

}

int CQp::Decode(CString szSrc, CMemFile &mmfile)
{

	/*
	char szSrc[4096];
	strncpy(szSrc,(LPCTSTR)strSrc,4095);

	char szDecoded[4096];
*/

	int i=0;
	
	char szTmp;
	int nLen=szSrc.GetLength();
	for(i=0;i<nLen;i++)
	{

		if( (szSrc[i]>=33 && szSrc[i]<=60) ||
			(szSrc[i]>=62 && szSrc[i]<=126) )
		{
			//szDecoded[j]=szSrc[i];
			szTmp=szSrc[i];
			mmfile.Write(&szTmp,1);
			
			continue;
		}

		if(szSrc[i]=='=')
		{
			i++;

			//add by sdx,2000-4-25 for qp decode failed

			if(i>=nLen)
			{
				DS_TRACE("<SYSERR/QP::Decode>: i="<<i<<" nLen="<<nLen<<"\n");
				return TRUE;
			}
			
			//i<nLen
			//end add


			if(szSrc[i]=='\r')
			{
				i++;//skip '\n'
				
				continue;
			}
			else
			{		
			
				char szLeft[3];
				memset(szLeft,0,3);

				char szRight[3];
				memset(szRight,0,3);

				//i++;
				szLeft[0]=szSrc[i];//3
				i++;
				//add by sdx,2000-4-25 for qp decode failed
				
				if(i>=nLen)
				{
					DS_TRACE("<SYSERR/QP::Decode>: i="<<i<<" nLen="<<nLen<<"\n");
					return TRUE;
				}
				
				//i<nLen
				//end add

				szLeft[1]=szSrc[i];//D
				
				//szDecoded[j]='=';
				char szOutPut;
				GetCharByHex(szLeft,szOutPut);

				mmfile.Write(&szOutPut,1);

				if(szOutPut=='=')
				{
					
					continue;
				}

				i++;//=
				i++;//
				//add by sdx,2000-4-25 for qp decode failed
				
				if(i>=nLen)
				{	
					DS_TRACE("<SYSERR/QP::Decode>: i="<<i<<" nLen="<<nLen<<"\n");
					return TRUE;
				}
				
				//i<nLen
				//end add

				szRight[0]=szSrc[i];//3
				i++;
				//add by sdx,2000-4-25 for qp decode failed
				if(i>=nLen)
				{
					DS_TRACE("<SYSERR/QP::Decode>: i="<<i<<" nLen="<<nLen<<"\n");
					return TRUE;
				}				
				//i<nLen
				//end add
				szRight[1]=szSrc[i];//D
				
				
				GetCharByHex(szRight,szOutPut);
				mmfile.Write(&szOutPut,1);

				continue;

			}
				
		}

		if(szSrc[i]=='\r')
		{
			//continue;
			i++;
			//add by sdx,2000-4-25 for qp decode failed

			if(i>=nLen)
			{
				DS_TRACE("<SYSERR/QP::Decode>: i="<<i<<" nLen="<<nLen<<"\n");
				return TRUE;
			}
			
			//i<nLen
			//end add
			if(szSrc[i]=='\n')
			{
				//szDecoded[j]=szSrc[i];
				szTmp='\r';
				mmfile.Write(&szTmp,1);

				szTmp=szSrc[i];
				mmfile.Write(&szTmp,1);
			}
		}

		

		if(szSrc[i]==' '|| szSrc[i]=='	')
		{
			//szDecoded[j]=szSrc[i];
			szTmp=szSrc[i];
			mmfile.Write(&szTmp,1);
		}

	}

	
	return TRUE;

}
CUtf7::CUtf7()
{

}

CUtf7::~CUtf7()
{

}

unsigned short CUtf7::Exange(unsigned short dwByte)
{
	unsigned short nTemp;
	nTemp=dwByte<<8;

	return nTemp|(dwByte>>8);
	
}

int CUtf7::Encode(const char *szSrc, CString& strDecoded)
{
	unsigned short nUnicode[STR_MAX];

	int nRet=MultiByteToWideChar(CP_ACP,0,szSrc,
		-1,nUnicode,STR_MAX);


	unsigned short nUnicodeExchange[STR_MAX];

	for(int i=0;i<nRet;i++)
	{
		nUnicodeExchange[i]=Exange(nUnicode[i]);
	}

	BYTE* bEncode;
	bEncode=(BYTE*)nUnicodeExchange;

	return Encode(bEncode,nRet*2,strDecoded);
}

int CUtf7::Encode(BYTE *bToEncode, int nSize, CString& strEncoded)
{
	

	/*

	for(int i=0;i<nSize;i++)
	{
		DS_TRACE(<<*bToEncode++<<"\t");
	}
	*/
	
	CBase64 base64;
	strEncoded=base64.Encode((LPCTSTR)bToEncode,
		nSize);
	
	HandleTheEncodedString(strEncoded);
	//handle it to cut the zero bits!
	
	return 1;
}


int CUtf7::Decode(CString strSrc, CString &strDecoded)
{
	//返回值：TRUE/FALSE；成功/失败。

	HandleTheDecodeString(strSrc);

	CBase64 base64;
	BYTE pBuf[1024];
	int nRet=base64.DecodeEx((LPCTSTR)strSrc,strSrc.GetLength(),
		pBuf);
    
	if(nRet%2==1)
		return FALSE;

	unsigned short* pExUnicode=(unsigned short*)pBuf;


	unsigned short pUnicode[1024];

	for(int i=0;i<nRet/2;i++)
	{
  		pUnicode[i]=Exange(pExUnicode[i]);
	}


 	char szBuf[1025];
	int nRet1=WideCharToMultiByte(CP_ACP, 0, pUnicode, -1, szBuf, 1024, NULL, NULL);
	if(nRet1<=0 || nRet1>1024)
		return FALSE;

	szBuf[nRet1]='\0';

	strDecoded=szBuf;

	return TRUE;
}


////////////add by sdx in yao yue fei 's library!
///////////2000-4-1

int CUtf7::Encode(const char* p,char* pBuf)
{
	//for example
	//p= "abc草稿d"
	//
	int nTotal=0;
	while(*p)
	{
		if(IsPrintable(*p))
		{
			//if the char is printable
			// add  in  buf only!
			
			memcpy(pBuf+nTotal,p++,1);
			nTotal++;
			//*p='a','b','c'
			//ok! printable finished!
		}
		else
		{
			//*p="草稿"
			
			char szBuf[1024];
			int nLen=DealWithUtf7(p,szBuf);
			//begin to deal with until the *p is printable

			if(nLen>0)
				p+=nLen;

			CString strEncoded;
			Encode(szBuf,strEncoded);//Encode the unprintable string;
			
			
			memcpy(pBuf+nTotal,"&",1);//begin utf flag.
			nTotal++;

			int nEncLen=strlen((LPCTSTR)strEncoded);
			memcpy(pBuf+nTotal,(LPCTSTR)strEncoded,
				nEncLen);

			nTotal+=nEncLen;
			memcpy(pBuf+nTotal,"-",1);//point to end flag
			nTotal++;
		}
	}


	pBuf[nTotal]='\0';

	return nTotal;
	
	//ok! encode finished!
	//check ok.
}
int CUtf7::IsPrintable(char c)
{
	if( (c>=0x20 && c<=0x25) ||
		(c>=0x27 && c<=0x7e) )
	{
		return TRUE;
	}
	return FALSE;
}

int CUtf7::DealWithUtf7(const char* p,char* pBuf)
{
	//current p point to "草稿"
	int nTotal=0;
	while(*p)
	{
		if(!IsPrintable(*p))
		{
			//if not printable
			//add in pBuf
			memcpy(pBuf+nTotal,p++,1);
			nTotal++;
		}
		else
		{
			//p--;
			//return to point to the printable char.
			break;
		}
	}
	
	pBuf[nTotal]='\0';

	return nTotal;
	
	//When return , p point to the printable char.
	//And pBuf is store the string which is unprintable.
	
}

////////////decode
int CUtf7::Decode(const char* p,char* pBuf)
{
	//解码。
	//&作为UTF7编码的开始标志
	//但也不一定,有些即使有&和-标志，但解码不成功的
	//也只能原样显示。

	int nTotal=0;
	while(*p)
	{
		if(*p!='&')
		{
			memcpy(pBuf+nTotal,p++,1);
			nTotal++;
			//yes!
			
		}
		else
		{
			p++;//skip the char --'&'
			const char* qFind;
			qFind=strchr(p,'-');//end flag!
			if(!qFind)
			{
				//如果没有结束标志，直接保存。
				memcpy(pBuf,"&",1);
				nTotal+=1;
				continue;
			}
			

			char szBuf[1024];
			memcpy(szBuf,p,qFind-p);
			szBuf[qFind-p]='\0';
			//szBuf为&和-之间的字符

			CString strDecoded;
			int nRetCode=Decode(szBuf,(char*)(LPCTSTR)strDecoded);

			//如果解码成功
			if(nRetCode)
			{
				memcpy(pBuf+nTotal,(LPCTSTR)strDecoded,
					strDecoded.GetLength());
				nTotal+=strDecoded.GetLength();//good!
			}
			else
			{
				//if nRetCode==FALSE;
				//如果解码失败，则显示为未解码前串

				memcpy(pBuf+nTotal,"&",1);
				nTotal++;
				memcpy(pBuf+nTotal,szBuf,qFind-p);
				nTotal+=qFind-p;
				memcpy(pBuf+nTotal,"-",1);
				nTotal+=1;
				DS_TRACE("<SYSERR/CUft7::Decode>: Decode failed!\n");

			}
            
			
			p=qFind+1;//p point to the next char
		}
	}

	pBuf[nTotal]='\0';
	return nTotal;
}

int CUtf7::HandleTheEncodedString(CString& strEncoded)
{
	//对编码后的串进行再加工

	int nFind=strEncoded.Find("AAA==");
	if(nFind>=0)
	{
		strEncoded=strEncoded.Mid(0,nFind);
		strEncoded.Replace('/',',');
		return TRUE;
	}

	int nLen=strEncoded.GetLength();
	
	BOOL bEnd=FALSE;
	for(int i=nLen-1;i>=0;i--)
	{
		if(strEncoded.GetAt(i)=='/')
		{
			strEncoded.SetAt(i,',');
			//utf modify version
			//use ',' insert '/'
			//see rfc2060
		}
		
		if(strEncoded.GetAt(i)=='A' ||
			strEncoded.GetAt(i)=='=')
		{
			if(!bEnd)
				strEncoded.SetAt(i,'\0');

				
		}
		else
		{
			bEnd=TRUE;
		}
		
	}
	
	
	nLen=strlen((LPCTSTR)strEncoded);//
	
	
	if(nLen%2)
	{
		//if len is odd
		strEncoded.SetAt(nLen,'A');//+="A";
	}
	
	//ok encoded is finished!
	
	return TRUE;
}

int CUtf7::HandleTheDecodeString(CString& strSrc)
{

	//对要编码的串作预处理
	//比如在串后加上"AAA="或用A补全四位
	//单个汉字问题待定

	//传入参数STRSRC是指& - 这间的串
	int nLen=strSrc.GetLength();
	strSrc.Replace(',','/');
	//将所有的","替换成"/"

	
	int nOffset=nLen-(nLen/4)*4;
	if(nOffset==0)
	{
		
		if(nLen%8==0)
		{
			strSrc+="AAA=";//suite for three char's chinese 
		}
	}
	else
	{
		for(int i=0;i<nOffset;i++)
		{
			strSrc+="A";
		}
	}
	
	return TRUE;
}

int CUtf7::DecodeEx(const char *pBuf, char *szDecoded)
{
	//int nLen=strlen(pBuf)/2;
	unsigned short* pExUnicode=(unsigned short*)pBuf;
	unsigned short pUnicode[1024];
	for(int i=0;i<1024;i++)
	{
  		pUnicode[i]=Exange(pExUnicode[i]);
	}
	int nRet1=WideCharToMultiByte(CP_UTF7, 0,pUnicode, -1, szDecoded, 1024, NULL, NULL);
	return nRet1;
}
