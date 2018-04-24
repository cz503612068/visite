// vistorApp.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include <vfw.h>
#include <time.h>
#include <stdio.h>
#include <io.h>
#include "vistorApp.h"
#include "ID2_USB.h"
#include "BarCode.h"
#include "WinAdo.h"
#include "SocketManage.h"
#include "WJProtocol.h"
#include "MacAdapter.h"
#include "SelectPrinttips.h"



#include "readmac.h"
#pragma comment(lib,"readmac.lib")


//#include <afx.h>
/*#include <locale.h>  
#include <windows.h>
#include <tchar.h>*/

void WINAPI XWnd_RedrawWnd(HWINDOW hWindow,BOOL bImmediately)
{
    XWnd_ReDrawWnd(hWindow,bImmediately);
}


#define L_V_I(str, line, idx)	{\
									strcpy(szT, str);\
									memset((char *)wBuf, 0, sizeof(wBuf));\
									MultiByteToWideChar(CP_ACP, 0, szT, -1, wBuf, 128);\
									XList_SetItemText(hList, line, idx, wBuf);\
								}

#define SET_EDIT(strid, resid)	{\
									memset((char *)wBuf, 0, sizeof(wBuf));\
									S_2_W(wBuf, (LPSTR)(LPCTSTR)outParas[strid], 128);\
									hE = XWnd_GetEle(g_hQueryDetail, resid);\
									XEdit_SetText(hE, wBuf);\
								}

#define CHK_INPUT(resid, keyname)	{\
										::GetPrivateProfileString("MustInput", keyname, "0", szT, sizeof(szT), CONFIG_FILE);\
										hE = GETELE(resid);\
										atoi(szT) == 0 ? XBtn_SetCheck(hE, false) : XBtn_SetCheck(hE, true);\
									}

#define SET_INPUT(resid, keyname)	{\
										hE = GETELE(resid);\
										bRet = XBtn_IsCheck(hE);\
										bRet ? strcpy(szT, "1") : strcpy(szT, "0");\
										::WritePrivateProfileString("MustInput", keyname, szT, CONFIG_FILE);\
									}

#define GETHWND			XWnd_GetHWnd(g_hWindow)
#define GETELE(res)		XWnd_GetEle(g_hWindow, res)

BOOL EnableButton(int nResID)
{
	/*
	nResID == 101 ? ENABLE_BTN(1) : DISABLE_BTN(1);
	nResID == 102 ? ENABLE_BTN(2) : DISABLE_BTN(2);
	nResID == 103 ? ENABLE_BTN(3) : DISABLE_BTN(3);
	nResID == 104 ? ENABLE_BTN(4) : DISABLE_BTN(4);
	nResID == 105 ? ENABLE_BTN(5) : DISABLE_BTN(5);
	nResID == 106 ? ENABLE_BTN(6) : DISABLE_BTN(6);
	nResID == 107 ? ENABLE_BTN(7) : DISABLE_BTN(7);
	nResID == 108 ? ENABLE_BTN(8) : DISABLE_BTN(8);			
	nResID == 109 ? ENABLE_BTN(9) : DISABLE_BTN(9);			
	nResID == 110 ? ENABLE_BTN(10) : DISABLE_BTN(10);
	nResID == 121 ? ENABLE_BTN(21) : DISABLE_BTN(21);
	nResID == 122 ? ENABLE_BTN(22) : DISABLE_BTN(22);
	nResID == 123 ? ENABLE_BTN(23) : DISABLE_BTN(23);
	nResID == 124 ? ENABLE_BTN(24) : DISABLE_BTN(24);
	nResID == 125 ? ENABLE_BTN(25) : DISABLE_BTN(25);
	nResID == 126 ? ENABLE_BTN(26) : DISABLE_BTN(26);
	nResID == 127 ? ENABLE_BTN(27) : DISABLE_BTN(27);
	nResID == 128 ? ENABLE_BTN(28) : DISABLE_BTN(28);
	nResID == 132 ? ENABLE_BTN(32) : DISABLE_BTN(32);
	nResID == 133 ? ENABLE_BTN(33) : DISABLE_BTN(33);
	nResID == 134 ? ENABLE_BTN(34) : DISABLE_BTN(34);
	nResID == 137 ? ENABLE_BTN(37) : DISABLE_BTN(37);
	nResID == 138 ? ENABLE_BTN(38) : DISABLE_BTN(38);
	nResID == 139 ? ENABLE_BTN(39) : DISABLE_BTN(39);
	*/
	return true;
}

bool getVisitor2Xml(char *xml,VISITORINFO &visitor,char *ip)
{
	strcat(xml,"<visitor>");
	strcat(xml,"<name>");
	strcat(xml,visitor.szName);
	strcat(xml,"</name>");

	strcat(xml,"<id>");
	strcat(xml,visitor.szID);
	strcat(xml,"</id>");

	strcat(xml,"<reason>");
	strcat(xml,visitor.szReason);
	strcat(xml,"</reason>");

	strcat(xml,"<Pic>");
	
	CString sPic;
	sPic=CString(visitor.szUserIDPic);
	sPic.Replace("#", "\\");
	if(_access(sPic,0)==0) // ֤��ͷ��
	{
		//strcat(xml,sPic);
	}
	else
	{
		sPic=CString(visitor.szUserIDFullPic);
		sPic.Replace("#", "\\");
		if(_access(sPic,0)==0) // ֤��ɨ��ͼ
		{
			//strcat(xml,sPic);

		}
		else
		{
			sPic=CString(visitor.szUserVideoPic);
			sPic.Replace("#", "\\");
			if(_access(sPic,0)==0) // �ֳ�����ͼ
			{
				//strcat(xml,sPic);
			}
			else
			{
				sPic=CString(visitor.szUserHeadPic);
				sPic.Replace("#", "\\");
				if(_access(sPic,0)==0) // �ֳ�����ͼ��ͼ
				{
					//strcat(xml,sPic);
				}
				else
				{
					return false;
				}
			}
		}
	}
	::DeleteFile(".\\send\\send.bmp");
	::CopyFile(sPic, ".\\send\\send.bmp", false);
	//strcat(xml,sPic);
	strcat(xml,".\\send\\send.bmp");
	strcat(xml,"</Pic>");

	strcat(xml,"<ip>");
	strcat(xml,ip);
	strcat(xml,"</ip>");

	strcat(xml,"</visitor>");
	return true;
}

bool getNodeXmlInfo(char *xmlbuf,char *pic,char *node)
{
	::CoInitialize(NULL);

    MSXML2::IXMLDOMDocumentPtr pDoc;
    HRESULT hr;
    hr=pDoc.CreateInstance(__uuidof(MSXML2::DOMDocument60));
    if(FAILED(hr))
    {  
        XMessageBox(g_hWindow, L"�޷�����DOMDocument���������Ƿ�װ��MS XML Parser ���п�!", L"����");  
		::CoUninitialize();
        return false;
    } 
    /* ����xml�ַ��� */
    pDoc->loadXML(xmlbuf); 

	try{


		//char sret[30] = {'/0'};
		strcpy(pic,pDoc->selectSingleNode(node)->Gettext());
		if(strlen(pic)<1)
		{
			pDoc.Release();
			::CoUninitialize();
			return false;
		}
	}
	catch(...){
			pDoc.Release();
			::CoUninitialize();
			return false;
	}

    //ʹ�ù��˵�ָ��Ҫ�ͷš�δʹ�õ�ָ�벻Ҫ�ͷţ�����ᴥ���жϴ���
    pDoc.Release();

	::CoUninitialize();
	return true;
}

bool getYueJSon(char *js,VISITORINFO &visitor)
{
	//char text[] = "{\"timestamp\":\"2013-11-19T08:50:11\",\"value\":1}";
	cJSON *json , *json_value , *json_timestamp;
    // �������ݰ�
    json = cJSON_Parse(js);
    if (!json)
    {
        //printf("Error before: [%s]\n",cJSON_GetErrorPtr());
		return false;
    }
    else
    {
        // ��������ֵ //�ÿ�ԤԼ�������ַ�ֵ
		//�ÿ�����
        json_value = cJSON_GetObjectItem( json , "vn");		
		strcpy(visitor.szName,json_value->valuestring);
		//����������
     		//����������
        json_value = cJSON_GetObjectItem( json , "in");
		try
		{
			strcmp(json_value->valuestring , "");
			strcpy(visitor.szToOfficialName,json_value->valuestring);

		}
		catch (...)
		{
			//XMessageBox(g_hWindow, L"����������Ϊ��", L"��ʾ");
		}
	
        //�������ֻ�
		json_value = cJSON_GetObjectItem( json , "im");
		try
		{
			strcmp(json_value->valuestring , "");
			strcpy(visitor.szToOfficialPhone,json_value->valuestring);

		}
		catch (...)
		{
			//XMessageBox(g_hWindow, L"�������ֻ�����Ϊ��", L"��ʾ");

		}
		
        //�ÿ��ֻ�
		json_value = cJSON_GetObjectItem( json , "vm");
		try
		{
			strcmp(json_value->valuestring , "");
			strcpy(visitor.szPhone,json_value->valuestring);

		}
		catch (...)
		{
			//XMessageBox(g_hWindow, L"�������ֻ�����Ϊ��", L"��ʾ");
		}

		//����ʱ��
		json_value = cJSON_GetObjectItem( json , "tm");
		try
		{
			strcmp(json_value->valuestring , "");
			strcpy(visitor.szYueTime,json_value->valuestring);
		}
		catch (...)
		{
			//XMessageBox(g_hWindow, L"������ʱ��", L"��ʾ");
		}		
		
        //��������
		json_value = cJSON_GetObjectItem( json , "pn");
		try
		{
			strcmp(json_value->valuestring , "");
			strcpy(visitor.szTotalPeople,json_value->valuestring);
		}
		catch (...)
		{
			//XMessageBox(g_hWindow, L"��������Ϊ��", L"��ʾ");
		}

        //��������
		json_value = cJSON_GetObjectItem( json , "rs");
		try
		{
			strcmp(json_value->valuestring , "");
			strcpy(visitor.szReason,json_value->valuestring);

		}
		catch (...)
		{
			//XMessageBox(g_hWindow, L"��������Ϊ��", L"��ʾ");
		}
		
		//���ƺ���
		json_value = cJSON_GetObjectItem( json , "lp");
		try
		{
			strcmp(json_value->valuestring , "");
			strcpy(visitor.szCarNum,json_value->valuestring);
		}
		catch (...)
		{
			//XMessageBox(g_hWindow, L"���ƺ���Ϊ��", L"��ʾ");
		}
        // �ͷ��ڴ�ռ�
        cJSON_Delete(json);
	}

	return true;
}

bool getCardXmlInfo(char *xmlbuf,IDCARD_ALL &visitorID2Card)
{
	::CoInitialize(NULL);

    MSXML2::IXMLDOMDocumentPtr pDoc;
    HRESULT hr;
    hr=pDoc.CreateInstance(__uuidof(MSXML2::DOMDocument60));
    if(FAILED(hr))
    {  
        XMessageBox(g_hWindow, L"�޷�����DOMDocument���������Ƿ�װ��MS XML Parser ���п�!", L"����"); 
		::CoUninitialize();
        return false;
    } 
    /* ����xml�ַ��� */
    pDoc->loadXML(xmlbuf); 


	try{


		char sret[30] = {'/0'};
		strcpy(sret,pDoc->selectSingleNode("//visitor//response")->Gettext());
		if(strcmp(sret,"0"))
		{
			pDoc.Release();
			::CoUninitialize();
			return false;
		}
		strcpy(visitorID2Card.name,pDoc->selectSingleNode("//visitor//name")->Gettext());
		strcpy(visitorID2Card.id,pDoc->selectSingleNode("//visitor//id")->Gettext());
		strcpy(visitorID2Card.number,pDoc->selectSingleNode("//visitor//id")->Gettext());
		strcpy(visitorID2Card.sex,pDoc->selectSingleNode("//visitor//sex")->Gettext());
		strcpy(visitorID2Card.address,pDoc->selectSingleNode("//visitor//address")->Gettext());
		strcpy(visitorID2Card.birthday,pDoc->selectSingleNode("//visitor//birthday")->Gettext());
		strcpy(visitorID2Card.people,pDoc->selectSingleNode("//visitor//folk")->Gettext());
	}
	catch(...){
			pDoc.Release();
			::CoUninitialize();
			return true;
	}

    //ʹ�ù��˵�ָ��Ҫ�ͷš�δʹ�õ�ָ�벻Ҫ�ͷţ�����ᴥ���жϴ���
    pDoc.Release();

	::CoUninitialize();
	return true;
}

int GetTcpBarAndUpVisitorInfo(char *bar)
{
	CNovelSocket novelSocket;
	if(novelSocket.Connect("127.0.0.1",Ask_Bar_Port)!=0)
		return -3;
/*
	switch(type)
	{
	case ID_TYPE_ID2:
		novelSocket.SendData("2",1); //���֤
		break;
	case ID_TYPE_PASSPORT:
		novelSocket.SendData("13",2);  //����
		break;
	case ID_TYPE_DriverLicense:
		novelSocket.SendData("5",1); //����
		break;
	default:
		return -1;
		break;
	}
*/	
	novelSocket.SendData("5",1);
	char buf[2000]= {'\0'};
	//int reti = 0;
	if(novelSocket.RecvData(buf,4000)<1)
		return -2;
	else
	{
		strcpy(bar,buf);
		return 0;
	}
}

int  GetTcpCardScan(int type,IDCARD_ALL &visitorID2Card)
{
	CNovelSocket novelSocket;
	if(novelSocket.Connect("127.0.0.1",ZT_Scan_Port)!=0)
		return -3;

	switch(type)
	{
	case ID_TYPE_ID2:
		novelSocket.SendData("2",1); //���֤
		break;
	case ID_TYPE_PASSPORT:
		novelSocket.SendData("13",2);  //����
		break;
	case ID_TYPE_DriverLicense:
		novelSocket.SendData("5",1); //����
		break;
	default:
		return -1;
		break;
	}
	
	char buf[2000]= {'\0'};
	//int reti = 0;
	if(novelSocket.RecvData(buf,4000)<1)
		return -2;
	//memset((char *)&stIDCard, 0x0, sizeof(IDCARD_ALL));
	//MessageBox(XWnd_GetHWnd(g_hWindow),buf,"��ʾ",MB_OK);
	if(getCardXmlInfo(buf,visitorID2Card))
	{
		return 0;
	}
	return -1;
}

BOOL CALLBACK OnMyEventTabClick(HELE hEle, HELE hEventEle) 
{
	if (hEle != hEventEle)
	{
		return false;
	}

	XWnd_RedrawWnd(XEle_GetHWindow(hEle));

	return false;
}

//���������������Ϣ�б��е���
BOOL CALLBACK OnHistoryVisitorList(HELE hEle, HELE hEventEle, int nIndex) 
{
	if (hEle != hEventEle)
	{
		return false;
	}
	if (nIndex < 0)
	{
		return false;
	}

//	XList_IsItemCheck(hEle, nIndex) ? XList_SetItemCheck(hEle, nIndex, false) : XList_SetItemCheck(hEle, nIndex, true);


	wchar_t wBuf[256] = {0};
	char szT[512] = {'\0'};
	long FldCount = 0, ret = 0;
	CString outParas[FLD_MAX];
	CWinAdo tAdo(sCONNSTR_MYSQL);
	
	ret = tAdo.OpenAdo();
	if (ret != ADOOK)
	{
		return false;
	}

	for (int i = 0; i < FLD_MAX; i++)
	{
		outParas[i].Empty();
	}

	char szName[64] = {'\0'}, szID[64] = {'\0'}, szVisitTime[32] = {'\0'}, szOfficialName[64] = {'\0'}, szOfficeName[64] = {'\0'};
	char szDate[64] = {'\0'}, szofficialduty[32] ={'\0'};
	int n = XList_GetSelectItem(hEle);
	memset((char *)wBuf, 0, sizeof(wBuf));
	wcscpy(wBuf, XList_GetItemText(hEle, n, 1));
	W_2_S(wBuf, szName, 128);

/*	memset((char *)wBuf, 0, sizeof(wBuf));
	wcscpy(wBuf, XList_GetItemText(hEle, n, 4));
	W_2_S(wBuf, szID, 128);

	memset((char *)wBuf, 0, sizeof(wBuf));
	wcscpy(wBuf, XList_GetItemText(hEle, n, 5));
	W_2_S(wBuf, szVisitTime, 128);*/
	
	memset((char *)wBuf, 0, sizeof(wBuf));
	wcscpy(wBuf, XList_GetItemText(hEle, n, 2));
	W_2_S(wBuf, szOfficialName, 128);

	memset((char *)wBuf, 0, sizeof(wBuf));
	wcscpy(wBuf, XList_GetItemText(hEle, n, 3));
	W_2_S(wBuf, szofficialduty, 128);

    memset((char *)wBuf, 0, sizeof(wBuf));
	wcscpy(wBuf, XList_GetItemText(hEle, n, 4));
	W_2_S(wBuf, szVisitTime, 128);

/*	memset((char *)wBuf, 0, sizeof(wBuf));
	wcscpy(wBuf, XList_GetItemText(hEle, n, 10));
	W_2_S(wBuf, szOfficeName, 128);*/

	//sprintf(szT, "select UNIX_TIMESTAMP(\'%s\')", szVisitTime);
	if (g_nSqlConnectType == SQL_CONNECT_SERVER2005)
	{	
		sprintf(szT, "select DATEDIFF(s, '1970-01-01 08:00:00', \'%s\')", szVisitTime);
	}
	else
        sprintf(szT, "select UNIX_TIMESTAMP(\'%s\')", szVisitTime);
	ret = tAdo.execSelect(szT, &FldCount, outParas);
	if (ret == ADOERR /*|| (FldCount == 0 && outParas[0].IsEmpty())*/)
	{
		return false;
	}
	strcpy(szVisitTime, outParas[0]);

	for (int j = 0; j < FLD_MAX; j++)
	{
		outParas[j].Empty();
	}

	sprintf(szT, "select * from t_visitor where visitor_name = \'%s\' and visitor_to_official_name = \'%s\' and  visitor_visitetime = %ld", szName, szOfficialName, atol(szVisitTime));
	ret = tAdo.execSelect(szT, &FldCount, outParas);
	if (ret == ADOERR || (FldCount == 0 && outParas[0].IsEmpty()))
	{
		return false;
	}

	VISITORINFO visitor;
	memset((char *)&visitor, 0, sizeof(VISITORINFO));
	strcpy(visitor.szName, outParas[0]);
	strcpy(visitor.szSex, outParas[1]);
	strcpy(visitor.szFolk, outParas[2]);
	strcpy(visitor.szPhone, outParas[3]);
	strcpy(visitor.szIDType, outParas[4]);
	strcpy(visitor.szID, outParas[5]);
	strcpy(visitor.szSignDepartment, outParas[6]);
	strcpy(visitor.szWithGoods, outParas[7]);
	strcpy(visitor.szReason, outParas[8]);
	strcpy(visitor.szAddr, outParas[9]);
	strcpy(visitor.szTotalPeople, outParas[10]);
	strcpy(visitor.szUnit, outParas[11]);
	strcpy(visitor.szCarNum, outParas[12]);

	outParas[19].Replace("#", "\\");
	strcpy(visitor.szUserVideoPic, outParas[19]);
	outParas[20].Replace("#", "\\");
	strcpy(visitor.szUserIDPic, outParas[20]);
	outParas[21].Replace("#", "\\");
	strcpy(visitor.szUserHeadPic, outParas[21]);
	outParas[26].Replace("#", "\\");
	strcpy(visitor.szUserIDFullPic, outParas[26]);

	strcpy(visitor.szValidDate, outParas[22]);
    strcpy(visitor.szID2Flag, outParas[23]);
	strcpy(visitor.szBirth, outParas[24]);
	strcpy(visitor.szJob, outParas[27]);
	strcpy(visitor.szType, outParas[28]);

	//g_Printvisitor
	strcpy(g_Printvisitor.szName, outParas[0]);
	strcpy(g_Printvisitor.szSex, outParas[1]);
	strcpy(g_Printvisitor.szFolk, outParas[2]);
	strcpy(g_Printvisitor.szPhone, outParas[3]);
	strcpy(g_Printvisitor.szIDType, outParas[4]);
	strcpy(g_Printvisitor.szID, outParas[5]);
	strcpy(g_Printvisitor.szSignDepartment, outParas[6]);
	strcpy(g_Printvisitor.szWithGoods, outParas[7]);
	strcpy(g_Printvisitor.szReason, outParas[8]);
	strcpy(g_Printvisitor.szAddr, outParas[9]);
	strcpy(g_Printvisitor.szTotalPeople, outParas[10]);
	strcpy(g_Printvisitor.szUnit, outParas[11]);
	strcpy(g_Printvisitor.szCarNum, outParas[12]);

	outParas[19].Replace("#", "\\");
	strcpy(g_Printvisitor.szUserVideoPic, outParas[19]);
	outParas[20].Replace("#", "\\");
	strcpy(g_Printvisitor.szUserIDPic, outParas[20]);
	outParas[21].Replace("#", "\\");
	strcpy(g_Printvisitor.szUserHeadPic, outParas[21]);
	outParas[26].Replace("#", "\\");
	strcpy(g_Printvisitor.szUserIDFullPic, outParas[26]);



	strcpy(g_Printvisitor.szValidDate, outParas[22]);
	strcpy(g_Printvisitor.szBirth, outParas[24]);
	strcpy(g_Printvisitor.szJob, outParas[27]);
	strcpy(g_Printvisitor.szType, outParas[28]);
	
	strcpy(g_szVisitorName,outParas[0]);
	strcpy(g_szRecord,outParas[16]);
	SetVisitorInfo(visitor);

	HELE hE = XWnd_GetEle(g_hWindow, IDP_ID2_HEADPIC);
	XPic_SetImage(hE, NULL);
	hE = XWnd_GetEle(g_hWindow, IDP_CAMERA);
	XPic_SetImage(hE, NULL);
	hE = XWnd_GetEle(g_hWindow, IDP_PASSPORTPIC);
	XPic_SetImage(hE, NULL);
	hE = XWnd_GetEle(g_hWindow, IDP_HEADPIC);
	XPic_SetImage(hE, NULL);
	hE = XWnd_GetEle(g_hWindow, IDP_HEADPIC2);
	XPic_SetImage(hE, NULL);
	


	::DeleteFile(USER_BARCODE_PIC);
	::DeleteFile(USER_ID2_PIC);
	::DeleteFile(USER_ID_PIC);
	::DeleteFile(USER_HEAD_PIC); 
	::DeleteFile(USER_VIDEO_PIC);
	if (_access(visitor.szUserIDPic,0)!=0&&g_bFtpOpened)
	{
		m_pFtpConnection->GetFile(visitor.szUserIDPic,USER_ID_PIC,true,FILE_ATTRIBUTE_ARCHIVE,INTERNET_FLAG_RELOAD,0);  	  
	}
	else
	{
        ::CopyFile(visitor.szUserIDPic, USER_ID_PIC,false);
	}
	
	if (_access(visitor.szUserIDFullPic,0)!=0&&g_bFtpOpened)
	{			  
		m_pFtpConnection->GetFile(visitor.szUserIDFullPic,USER_ID2_PIC,true,FILE_ATTRIBUTE_ARCHIVE,INTERNET_FLAG_RELOAD,0);
	}
	else
	{		
		::CopyFile(visitor.szUserIDFullPic, USER_ID2_PIC,false);
	}	

	if (_access(visitor.szUserHeadPic,0)!=0&&g_bFtpOpened)
	{			  
		m_pFtpConnection->GetFile(visitor.szUserHeadPic,USER_HEAD_PIC,true,FILE_ATTRIBUTE_ARCHIVE,INTERNET_FLAG_RELOAD,0);
	}
	else
	{		
		//::CopyFile(visitor.szUserHeadPic, USER_HEAD_PIC,false);
		::CopyFile(visitor.szUserHeadPic, ".\\pic\\user_head_pic.bmp",false);
	}	
	
	if (_access(visitor.szUserVideoPic,0)!=0&&g_bFtpOpened)
	{
		m_pFtpConnection->GetFile(visitor.szUserVideoPic,USER_VIDEO_PIC,true,FILE_ATTRIBUTE_ARCHIVE,INTERNET_FLAG_RELOAD,0);  
	}	
	else
	{
		::CopyFile(visitor.szUserVideoPic, USER_VIDEO_PIC,false);
	}

	strcpy(visitor.szUserIDPic,USER_ID_PIC);
	strcpy(visitor.szUserIDFullPic,USER_ID2_PIC);
	strcpy(visitor.szUserHeadPic,USER_HEAD_PIC);
	strcpy(visitor.szUserVideoPic,USER_VIDEO_PIC);

	if (atoi(visitor.szID2Flag) == 1)		//����֤
	{
		g_nID2ReadType=ID2_READ;
		ShowID2(true, visitor);
		strcpy(g_stIDCard.szPicPath,visitor.szUserIDPic);
	    strcpy(g_stIDCard.number,visitor.szID);
	    strcpy(g_stIDCard.name,visitor.szName);
	    strcpy(g_stIDCard.sex,visitor.szSex);
	    strcpy(g_stIDCard.people,visitor.szFolk);
	    strcpy(g_stIDCard.address,visitor.szAddr);
	    strcpy(g_stIDCard.szSignDep,visitor.szSignDepartment);
	    strcpy(g_stIDCard.birthday,visitor.szBirth);
	    strcpy(g_stIDCard.validterm,visitor.szValidDate);
	}
	else
	{
		ShowID2(false, visitor);		
	}

	//if(g_bSSO)
	//	sprintf(szT, "select * from t_official where official_name = \'%s\' and official_duty = \'%s\'", szOfficialName, szofficialduty);
	//else
		sprintf(szT, "select * from t_official where official_name = \'%s\' and official_duty = \'%s\'", szOfficialName, szofficialduty);

	ret = tAdo.execSelect(szT, &FldCount, outParas);
	if (ret == ADOERR || (FldCount == 0 && outParas[0].IsEmpty()))
	{
		return false;
	}
	OFFICIALINFO official;
	memset((char *)&official, 0, sizeof(OFFICIALINFO));
	strcpy(official.szName, outParas[0]);
	strcpy(official.szSex, outParas[1]);
	strcpy(official.szOfficePhone, outParas[2]);
	strcpy(official.szMobile, outParas[3]);
	strcpy(official.szOfficeName, outParas[4]);
	strcpy(official.szOfficeAddr, outParas[5]);
	strcpy(official.szOfficialDuty, outParas[6]);
	strcpy(official.szRoom, outParas[7]);
	strcpy(official.szIP, outParas[8]);
	strcpy(official.szNO, outParas[10]);
	strcpy(official.szSSO, outParas[13]);

	strcpy(g_Printvisitor.szToOfficeName, official.szName);
    strcpy(g_Printvisitor.szToOfficialName, official.szOfficeName);
	strcpy(g_Printvisitor.szToRoom, official.szRoom);
	strcpy(g_Printvisitor.szToSSO, official.szSSO);
	strcpy(g_szOfficialIP, official.szIP);
	

	::WritePrivateProfileString("OfficialInfo", "Name", official.szName, CONFIG_FILE);
	::WritePrivateProfileString("OfficialInfo", "Sex", official.szSex, CONFIG_FILE);
	::WritePrivateProfileString("OfficialInfo", "OfficePhone", official.szOfficePhone, CONFIG_FILE);
	::WritePrivateProfileString("OfficialInfo", "Mobile", official.szMobile, CONFIG_FILE);
	::WritePrivateProfileString("OfficialInfo", "OfficeName", official.szOfficeName, CONFIG_FILE);
	::WritePrivateProfileString("OfficialInfo", "OfficeAddr", official.szOfficeAddr, CONFIG_FILE);
	::WritePrivateProfileString("OfficialInfo", "Level", official.szOfficialDuty, CONFIG_FILE);
	::WritePrivateProfileString("OfficialInfo", "RoomNum", official.szRoom, CONFIG_FILE);
	::WritePrivateProfileString("OfficialInfo", "SSO", official.szSSO, CONFIG_FILE);
	
	SetOfficialInfo(official);
	g_bShowCap=false;
    ShowWindow(g_hCapWnd, false);
	Pause();
//	BackScreenCheckInfo();
	BackScreenSelectOfficial();  //��ʾ���������ѯ������
	XWnd_CloseWindow(g_hOfficialWin);
    //g_bShowCap=false;
    //ShowWindow(g_hCapWnd, false);
	XWnd_RedrawWnd(g_hWindow);
	return true;
}

//���������������Ϣ�б��е���
BOOL CALLBACK OnVisitorList(HELE hEle, HELE hEventEle, int nIndex) 
{
	if (hEle != hEventEle)
	{
		return false;
	}
	if (nIndex < 0)
	{
		return false;
	}
	XEle_SetFocus(hEle,true);
	if (g_bCalled)
	{
		//XMessageBox();
		long ret = XMessageBox(g_hWindow, L"�ÿ���Ϣû�б��棿��[ȷ��]����,��[ȡ��]����", L"��ʾ��Ϣ", XMB_OK | XMB_CANCEL); //fy
		if (ret == XMB_CANCEL)
		{
			g_bCalled=false;
		}
	    else
		{
			SaveAndPrint();
		}
	}

	XList_IsItemCheck(hEle, nIndex) ? XList_SetItemCheck(hEle, nIndex, false) : XList_SetItemCheck(hEle, nIndex, true);


	wchar_t wBuf[256] = {0};
	char szT[512] = {'\0'};
	long FldCount = 0, ret = 0;
	CString outParas[FLD_MAX];
	CWinAdo tAdo(sCONNSTR_MYSQL);
	
	ret = tAdo.OpenAdo();
	if (ret != ADOOK)
	{
		return false;
	}

	for (int i = 0; i < FLD_MAX; i++)
	{
		outParas[i].Empty();
	}

	char szName[64] = {'\0'}, szID[64] = {'\0'}, szVisitTime[32] = {'\0'}, szOfficialName[64] = {'\0'}, szOfficeName[64] = {'\0'};
	char szDate[64] = {'\0'};
	int n = nIndex;
	memset((char *)wBuf, 0, sizeof(wBuf));
	wcscpy(wBuf, XList_GetItemText(hEle, n, 2));
	W_2_S(wBuf, szName, 128);

	memset((char *)wBuf, 0, sizeof(wBuf));
	wcscpy(wBuf, XList_GetItemText(hEle, n, 4));
	W_2_S(wBuf, szID, 128);

	memset((char *)wBuf, 0, sizeof(wBuf));
	wcscpy(wBuf, XList_GetItemText(hEle, n, 5));
	W_2_S(wBuf, szVisitTime, 128);
	
	memset((char *)wBuf, 0, sizeof(wBuf));
	wcscpy(wBuf, XList_GetItemText(hEle, n, 8));
	W_2_S(wBuf, szOfficialName, 128);

	memset((char *)wBuf, 0, sizeof(wBuf));
	wcscpy(wBuf, XList_GetItemText(hEle, n, 9));
	W_2_S(wBuf, szOfficeName, 128);

	//sprintf(szT, "select UNIX_TIMESTAMP(\'%s\')", szVisitTime);
	if (g_nSqlConnectType == SQL_CONNECT_SERVER2005)
	{	
		sprintf(szT, "select DATEDIFF(s, '1970-01-01 08:00:00', \'%s\')", szVisitTime);
	}
	else
        sprintf(szT, "select UNIX_TIMESTAMP(\'%s\')", szVisitTime);
	ret = tAdo.execSelect(szT, &FldCount, outParas);
	if (ret == ADOERR || (FldCount == 0 && outParas[0].IsEmpty()))
	{
		return false;
	}
	strcpy(szVisitTime, outParas[0]);

	for (int j = 0; j < FLD_MAX; j++)
	{
		outParas[j].Empty();
	}

	sprintf(szT, "select * from t_visitor where visitor_name = \'%s\' and visitor_id = \'%s\' and visitor_station_id = \'%s\' and visitor_visitetime = %ld", szName, szID, g_szStation,atol(szVisitTime));
	
	ret = tAdo.execSelect(szT, &FldCount, outParas);
	if (ret == ADOERR || (FldCount == 0 && outParas[0].IsEmpty()))
	{
		return false;
	}
	if (outParas[0].IsEmpty())
	{
		XMessageBox(g_hWindow,L"������Ч��¼������ϵ����Ա",L"��ʾ");
		return false;
	}

	if (strlen(outParas[17])>1)  //�Ѿ�ǩ�룬����������ķÿͻ�ǩ��
	{
        ListTodayVistorsInfo();
	    VisitorTotalInfo(szT);	
		if (::GetPrivateProfileInt("TerminalSet", "bLeaveOfficial",0, CONFIG_FILE)==1)
		{
			//�������������
			InitVisitorData();
		}
		else
		{
			//�����������
			InitData();
		}
		return false;
	}
	
	VISITORINFO visitor;
	memset((char *)&visitor, 0, sizeof(VISITORINFO));
	strcpy(visitor.szName, outParas[0]);
	strcpy(visitor.szSex, outParas[1]);
	strcpy(visitor.szFolk, outParas[2]);
	strcpy(visitor.szPhone, outParas[3]);
	strcpy(visitor.szIDType, outParas[4]);
	strcpy(visitor.szID, outParas[5]);
	strcpy(visitor.szSignDepartment, outParas[6]);
	strcpy(visitor.szWithGoods, outParas[7]);
	strcpy(visitor.szReason, outParas[8]);
	strcpy(visitor.szAddr, outParas[9]);
	strcpy(visitor.szTotalPeople, outParas[10]);
	strcpy(visitor.szUnit, outParas[11]);
	strcpy(visitor.szCarNum, outParas[12]);

	outParas[19].Replace("#", "\\");
	strcpy(visitor.szUserVideoPic, outParas[19]);
	outParas[20].Replace("#", "\\");
	strcpy(visitor.szUserIDPic, outParas[20]);
	outParas[21].Replace("#", "\\");
	strcpy(visitor.szUserHeadPic, outParas[21]);
	outParas[26].Replace("#", "\\");
	strcpy(visitor.szUserIDFullPic, outParas[26]);

	strcpy(visitor.szValidDate, outParas[22]);
	strcpy(visitor.szID2Flag,outParas[23]);
	strcpy(visitor.szBirth, outParas[24]);
	strcpy(visitor.szJob, outParas[27]);
	strcpy(visitor.szType, outParas[28]);

	//g_Printvisitor
	strcpy(g_Printvisitor.szName, outParas[0]);
	strcpy(g_Printvisitor.szSex, outParas[1]);
	strcpy(g_Printvisitor.szFolk, outParas[2]);
	strcpy(g_Printvisitor.szPhone, outParas[3]);
	strcpy(g_Printvisitor.szIDType, outParas[4]);
	strcpy(g_Printvisitor.szID, outParas[5]);
	strcpy(g_Printvisitor.szSignDepartment, outParas[6]);
	strcpy(g_Printvisitor.szWithGoods, outParas[7]);
	strcpy(g_Printvisitor.szReason, outParas[8]);
	strcpy(g_Printvisitor.szAddr, outParas[9]);
	strcpy(g_Printvisitor.szTotalPeople, outParas[10]);
	strcpy(g_Printvisitor.szUnit, outParas[11]);
	strcpy(g_Printvisitor.szCarNum, outParas[12]);
    strcpy(g_Printvisitor.szBarCode, outParas[13]);
	g_Printvisitor.lVisiteTime = atol(outParas[16]);


	outParas[19].Replace("#", "\\");
	strcpy(g_Printvisitor.szUserVideoPic, outParas[19]);
	outParas[20].Replace("#", "\\");
	strcpy(g_Printvisitor.szUserIDPic, outParas[20]);
	outParas[21].Replace("#", "\\");
	strcpy(g_Printvisitor.szUserHeadPic, outParas[21]);
	outParas[26].Replace("#", "\\");
	strcpy(g_Printvisitor.szUserIDFullPic, outParas[26]);

	strcpy(g_Printvisitor.szValidDate, outParas[22]);
	strcpy(g_Printvisitor.szBirth, outParas[24]);
	strcpy(g_Printvisitor.szJob, outParas[27]);
	strcpy(g_Printvisitor.szType, outParas[28]);

    strcpy(g_szVisitorName,outParas[0]);
	strcpy(g_szRecord,outParas[16]);
	SetVisitorInfo(visitor);


	if (atoi(visitor.szID2Flag) == 1)		//��Ӧ�����Ķ���֤
	{
		ShowID2(true, visitor);
	}
	else
	{
		ShowID2(false, visitor);
		
	}

	for (int k = 0; k < FLD_MAX; k++)
	{
		outParas[k].Empty();
	}
	sprintf(szT, "select * from t_official where official_name = \'%s\' and official_office_name = \'%s\'", szOfficialName, szOfficeName);
	ret = tAdo.execSelect(szT, &FldCount, outParas);
	if (ret == ADOERR || (FldCount == 0 && outParas[0].IsEmpty()))
	{
		return false;
	}
	OFFICIALINFO official;
	memset((char *)&official, 0, sizeof(OFFICIALINFO));
	strcpy(official.szName, outParas[0]);
	strcpy(official.szSex, outParas[1]);
	strcpy(official.szOfficePhone, outParas[2]);
	strcpy(official.szMobile, outParas[3]);
	strcpy(official.szOfficeName, outParas[4]);
	strcpy(official.szOfficeAddr, outParas[5]);
	strcpy(official.szOfficialDuty, outParas[6]);
	strcpy(official.szRoom, outParas[7]);
	strcpy(official.szIP, outParas[8]);
	strcpy(official.szNO, outParas[10]);
	strcpy(official.szSSO, outParas[13]);

	strcpy(g_Printvisitor.szToOfficeName, official.szName);
    strcpy(g_Printvisitor.szToOfficialName, official.szOfficeName);
	strcpy(g_Printvisitor.szToRoom, official.szRoom);
	strcpy(g_Printvisitor.szToSSO, official.szSSO);
	strcpy(g_szOfficialIP, official.szIP);
	
	SetOfficialInfo(official);
    g_bShowCap=false;
    ShowWindow(g_hCapWnd, false);
	Pause();
	HideSoftKeybd();
	XWnd_RedrawWnd(g_hWindow);
	HWINDOW hWindow;
	if(g_bAutoCallOfficials)
	{
		 hWindow=XModalWnd_CreateWindow(410, 120, L"ǩ��ÿ���Ϣ��", XWnd_GetHWnd(g_hWindow));
		if(hWindow)
		{
			HELE hButton=XBtn_Create(10,10,65,65,L"��",hWindow);//������ť
			HELE hButton2=XBtn_Create(90,10,65,65,L"ȫ��",hWindow);//������ť
			HELE hButton3=XBtn_Create(170,10,65,65,L"��",hWindow);//������ť
			XEle_RegisterEvent(hButton,XE_BNCLICK,OnLogOutBtnClick);//ע�ᰴť����¼�
			XEle_RegisterEvent(hButton2,XE_BNCLICK,OnAllLogOutBtnClick);
			XEle_RegisterEvent(hButton3,XE_BNCLICK,OnCloseLogOutBtnClick);

			//���·��ʵ���
			HELE hButton4=XBtn_Create(250,10,65,65,L"ָ���ط�",hWindow);//������ť
			HELE hButton5=XBtn_Create(330,10,65,65,L"ȫ���ط�",hWindow);//������ť
			XEle_RegisterEvent(hButton4,XE_BNCLICK,OnReCallBtnClick);
			XEle_RegisterEvent(hButton5,XE_BNCLICK,OnAllReCallBtnClick);
			//XWnd_ShowWindow(hWindow,SW_SHOW);//��ʾ����
			//XRunXCGUI();
		}
	}
	else
	{
		 hWindow=XModalWnd_CreateWindow(250, 120, L"ǩ��ÿ���Ϣ��", XWnd_GetHWnd(g_hWindow));
		if(hWindow)
		{

			HELE hButton=XBtn_Create(10,10,65,65,L"��",hWindow);//������ť
			HELE hButton2=XBtn_Create(90,10,65,65,L"ȫ��",hWindow);//������ť
			HELE hButton3=XBtn_Create(170,10,65,65,L"��",hWindow);//������ť
			XEle_RegisterEvent(hButton,XE_BNCLICK,OnLogOutBtnClick);//ע�ᰴť����¼�
			XEle_RegisterEvent(hButton2,XE_BNCLICK,OnAllLogOutBtnClick);
			XEle_RegisterEvent(hButton3,XE_BNCLICK,OnCloseLogOutBtnClick);
			//XWnd_ShowWindow(hWindow,SW_SHOW);//��ʾ����
			//XRunXCGUI();
		}
	}

	XWnd_SetBorderSize(hWindow, 3, 3, 3, 3);
	//HIMAGE hImg = XImage_LoadFile(L".\\skin\\image\\frame.jpg", true);
	//XWnd_SetImageNC(hWindow, hImg);
	//g_nIDType = ID_TYPE_OTHER;
    XModalWnd_DoModal(hWindow);
	return true;
}

//�������ϲ�ѯͳ�ư�ť����Ӧ
BOOL CALLBACK OnChaxunTongji(HELE hEle, HELE hEventEle) 
{
	if (hEle != hEventEle)
	{
		return false;
	}
	
	HideSoftKeybd();

	if (::GetPrivateProfileInt("TerminalSet", "bMainChaxun",0, CONFIG_FILE)==1)
	{
		//SuspendThread(g_HWDThreadID2ID);  //��ͣ����֤��̨�߳�
		SuspendThread(g_ThreadReadBarcode);
		//XWnd_CloseWindow(XEle_GetHWindow(hEle));
		XBtn_SetCheck(GETELE(IDB_CHAXUN), true);
		XBtn_SetCheck(GETELE(IDB_TONGJI), false);
		XBtn_SetCheck(GETELE(IDB_TONGJICHAXUNRETURN), false);


		ShowWindow(g_hCapWnd, false);
		Pause();
		XEle_ShowEle(g_hPage1, false);
		XEle_ShowEle(g_hPage6, true);

		ChaXunToDayInfo();

 		HideSoftKeybd();
		ShowStandSoftKeybd();
		XWnd_RedrawWnd(g_hWindow);
		return false;
	}

	HWINDOW hWindow=XModalWnd_CreateWindow(210, 160, L"���������", XWnd_GetHWnd(g_hWindow));
	if(hWindow)
	{

		//HELE hButton=XStatic_Create(10,10,65,65,L"��",hWindow);//������ť
        htxtChaxunPwd=XEdit_Create(20,10,165,25,hWindow);//�ı�����
		XEdit_EnablePassBox(htxtChaxunPwd,true);
		HELE hBtnOK=XBtn_Create(20,45,65,65,L"ȷ��",hWindow);
		HELE hBtnCancel=XBtn_Create(120,45,65,65,L"����",hWindow);//������ť


		//XEle_RegisterEvent(hButton,XE_BNCLICK,OnLogOutBtnClick);//ע�ᰴť����¼�
		XEle_RegisterEvent(htxtChaxunPwd,XE_SETFOCUS,OnOtherSetFocus);
        XEle_RegisterEvent(hBtnOK,XE_BNCLICK,OnOKBtnClick);
		XEle_RegisterEvent(hBtnCancel,XE_BNCLICK,OnCloseLogOutBtnClick);
		//XWnd_ShowWindow(hWindow,SW_SHOW);//��ʾ����
		//XRunXCGUI();
	}
	SetMainDlgEditBorder();
	XWnd_SetBorderSize(hWindow, 3, 3, 3, 3);
	HIMAGE hImg = XImage_LoadFile(L".\\skin\\image\\frame.jpg", true);
	//XWnd_SetImageNC(hWindow, hImg);
    XModalWnd_DoModal(hWindow);
	return false;
}

//���ͳ�Ʋ�ѯҳ���ϵ�ͳ�ư�ť
BOOL CALLBACK OnTongji(HELE hEle, HELE hEventEle) 
{
	g_nChaXunTongJiType = BY_TONGJI;
	
	XBtn_SetCheck(GETELE(IDB_CHAXUN), false);
	XBtn_SetCheck(GETELE(IDB_TONGJI), true);
	XBtn_SetCheck(GETELE(IDB_TONGJICHAXUNRETURN), false);

	TongJiInfo();

	XWnd_RedrawWnd(XEle_GetHWindow(hEle));
	return false;
}

//��ѯ����������Ϣ
void ChaXunToDayInfo()
{
	char szT[512] = {'\0'};
	long FldCount = 0, ret = 0, n = 0;
	CString outParas[FLD_MAX], outP1[FLD_MAX];
	CWinAdo tAdo(sCONNSTR_MYSQL), tAdo1(sCONNSTR_MYSQL);

	g_nChaXunTongJiType = BY_CHAXUN;

	XBtn_SetCheck(GETELE(IDB_CHAXUN), true);
	XBtn_SetCheck(GETELE(IDB_TONGJI), false);
	XBtn_SetCheck(GETELE(IDB_TONGJICHAXUNRETURN), false);
	
	ret = tAdo.OpenAdo();
	if (ret != ADOOK)
	{
		return;
	}
	ret = tAdo1.OpenAdo();
	if (ret != ADOOK)
	{
		return;
	}
	
	for (int i = 0; i < FLD_MAX; i++)
	{
		outParas[i].Empty();
		outP1[i].Empty();
	}

	wchar_t wBuf[128] = {'\0'};
	char szOffice[64] = {'\0'}, szVisitor[64] = {'\0'}, szOfficial[64] = {'\0'}, szID[64] = {'\0'};

	HELE hE = GETELE(IDE_BEIFANGBUMEN);
	XEdit_GetText(hE, wBuf, 128);
	W_2_S(wBuf, szOffice, 128);

	hE = GETELE(IDE_BEIFANGRENYUAN);
	XEdit_GetText(hE, wBuf, 128);
	W_2_S(wBuf, szOfficial, 128);

	hE = GETELE(IDE_LAIFANGRENYUAN);
	XEdit_GetText(hE, wBuf, 128);
	W_2_S(wBuf, szVisitor, 128);

	hE = GETELE(IDE_ZHENGJIANHAOMA);
	XEdit_GetText(hE, wBuf, 128);
	W_2_S(wBuf, szID, 128);

	HELE hList = GETELE(IDL_LISTCHAXUNTONGJI);
	XList_DeleteAllColumns(hList);
	XList_DeleteAllItems(hList);
	
	XList_SetHeaderHeight(hList, 30);
	XList_EnableGrid(hList, false);
	XList_EnableMultilineSelect(hList, false);

	if (g_nSqlConnectType == SQL_CONNECT_SERVER2005)
	{
		sprintf(szT, "select * from t_visitor where visitor_station_id = \'%s\' and DateDiff(d, getdate(),DATEADD(s,visitor_visitetime,'1970-01-01 08:00:00'))=0 or visitor_lefttime <= 0 and visitor_status >1 order by visitor_visitetime desc", g_szStation);
	}
	else
    {
	    sprintf(szT, "select * from t_visitor where visitor_station_id = \'%s\' and (TO_DAYS(NOW()) - TO_DAYS(FROM_UNIXTIME(visitor_visitetime))=0 or visitor_lefttime <= 0) and visitor_status >1 order by visitor_visitetime desc", g_szStation);
	}
	ret = tAdo.execSelect(szT, &FldCount, outParas);
	if (ret == ADOERR /*|| (FldCount == 0 && outParas[0].IsEmpty())*/)
	{
		return;
	}

    if (FldCount > 0)
    {
		XList_AddColumn(hList, 50, L"���");
		XList_AddColumn(hList, 40, L"�Ÿ�");
		XList_AddColumn(hList, 60, L"�ÿ�����");
		XList_AddColumn(hList, 35, L"�Ա�");
		XList_AddColumn(hList, 0, L"����");
		XList_AddColumn(hList, 0, L"֤������");  //5
		XList_AddColumn(hList, 120, L"֤������");
		XList_AddColumn(hList, 0, L"��ͥסַ");
        XList_AddColumn(hList, 0, L"��֤����");
		XList_AddColumn(hList, 0, L"�ÿ͵�λ");
		XList_AddColumn(hList, 0, L"�绰����");  //10
		XList_AddColumn(hList, 0, L"���ƺ���");  
		XList_AddColumn(hList, 125, L"����ʱ��");
		XList_AddColumn(hList, 90, L"��������");
		XList_AddColumn(hList, 90, L"Я����Ʒ");
		XList_AddColumn(hList, 125, L"�뿪ʱ��");//15
		XList_AddColumn(hList, 0, L"��������");  
		XList_AddColumn(hList, 60, L"������");   
		XList_AddColumn(hList, 100, L"��ȥ����");
		XList_AddColumn(hList, 0, L"���õ绰");
		n = 0;
		do 
		{
			memset(szT, 0, sizeof(szT));
			sprintf(szT, "%d", n + 1);		//���
			S_2_W(wBuf, szT, 128);
			XList_AddItem(hList, wBuf, n);
			
			//list visitor information
			L_V_I(outParas[18], n, 1)		//�Ÿ�
				L_V_I(outParas[0], n, 2)    //����
				L_V_I(outParas[1], n, 3)    //�Ա�
				L_V_I(outParas[2], n, 4)    //����
				L_V_I(outParas[3], n, 10) //�绰
				L_V_I(outParas[4], n, 5) //֤������
				L_V_I(outParas[5], n, 6)    //֤������
				L_V_I(outParas[6], n, 8) //��֤����
				L_V_I(outParas[7], n, 14) //Я����Ʒ
				L_V_I(outParas[8], n, 13) //��������
				L_V_I(outParas[9], n, 7) //��ַ
				L_V_I(outParas[10], n, 16) //��������
				L_V_I(outParas[11], n, 9) //�ÿ͵�λ
				L_V_I(outParas[12], n, 11) //���ƺ�
				
				if (atol(outParas[16]) <= 0)
				{
					L_V_I("", n, 12)
				}
				else
				{
					//sprintf(szT, "select FROM_UNIXTIME(%s)", outParas[16]);	//����ʱ��
					if (g_nSqlConnectType == SQL_CONNECT_SERVER2005)
					{
						 sprintf(szT, "SELECT DATEADD(s,%d,'1970-01-01 08:00:00')", atol(outParas[16]));	//����ʱ��
					}
					else
					{
                         sprintf(szT, "select FROM_UNIXTIME(%s)", outParas[16]);	//����ʱ��
					}
					ret = tAdo1.execSelect(szT, &FldCount, outP1);
					if (ret == ADOERR || (FldCount == 0 && outP1[0].IsEmpty()))
					{
						L_V_I("", n, 12)
					}
					else
					{
						L_V_I(outP1[0], n, 12)			//����ʱ��
					}
				}
				
					if (atol(outParas[17]) <= 0)
					{
						L_V_I("", n, 15)
					}
					else
					{
						//sprintf(szT, "select FROM_UNIXTIME(%s)", outParas[17]);	//�뿪ʱ��
						if (g_nSqlConnectType == SQL_CONNECT_SERVER2005)
						{
							sprintf(szT, "SELECT DATEADD(s,%d,'1970-01-01 08:00:00')", atol(outParas[17]));	//�뿪ʱ��
						}
						else
						{
							sprintf(szT, "select FROM_UNIXTIME(%s)", outParas[17]);	//�뿪ʱ��
						}
						ret = tAdo1.execSelect(szT, &FldCount, outP1);
						if (ret == ADOERR || (FldCount == 0 && outP1[0].IsEmpty()))
						{
							L_V_I("", n, 15)
						}
						else
						{
							L_V_I(outP1[0], n, 15)			//�뿪ʱ��
						}
					}
					
					L_V_I(outParas[14], n, 17)			//����������
						L_V_I(outParas[15], n, 18)			//�����˰칫��
						L_V_I(outParas[16], n, 19)
						n++;
		} while(tAdo.getNext(outParas) == ADOOK);
    }
	else
	{
         	XList_AddColumn(hList, 900, L"��ʾ");
			XList_AddItem(hList, L"û�в�ѯ������ʾ����Ϣ��", 0);
			//XEle_SetTextColor(hList,RGB(255,0,0));
	}



	return;
}

//ͳ����Ϣ
BOOL TongJiInfo()
{
	char szT[512] = {'\0'};
	long FldCount = 0, ret = 0;
	CString outParas[FLD_MAX];
	CWinAdo tAdo(sCONNSTR_MYSQL);
	char szMaxOffice[64] = {'\0'}, szMaxOfficial[64] = {'\0'}, szMaxVisitor[64] = {'\0'};
	char szMaxOfficeTimes[64] = {'\0'}, szMaxOfficialTimes[64] = {'\0'}, szMaxVisitorTimes[64] = {'\0'};
	char szTodayTotalVisitor[64] = {'\0'}, szTotalVisitor[64] = {'\0'};
	
	ret = tAdo.OpenAdo();
	if (ret != ADOOK)
	{
		return false;
	}

	for (int i = 0; i < FLD_MAX; i++)
	{
		outParas[i].Empty();
	}

	//��������������
	//strcpy(szT, "select count(*) from t_visitor where TO_DAYS(NOW()) - TO_DAYS(FROM_UNIXTIME(visitor_visitetime)) = 0");
	//sprintf(szT, "select count(*) from t_visitor where TO_DAYS(NOW()) - TO_DAYS(FROM_UNIXTIME(visitor_visitetime)) = 0 and visitor_status >1 and visitor_station_id = \'%s\'",g_szStation);
	if (g_nSqlConnectType == SQL_CONNECT_SERVER2005)
	{
	    sprintf(szT, "select count(*) from t_visitor where DateDiff(d, getdate(),DATEADD(s,visitor_visitetime,'1970-01-01 08:00:00'))=0 and visitor_status >1 and visitor_station_id = \'%s\'",g_szStation);
	}
	else
    {
	    sprintf(szT, "select count(*) from t_visitor where TO_DAYS(NOW()) - TO_DAYS(FROM_UNIXTIME(visitor_visitetime)) = 0 and visitor_status >1 and visitor_station_id = \'%s\'",g_szStation);
	}
	ret = tAdo.execSelect(szT, &FldCount, outParas);
	if (ret == ADOERR /*|| (FldCount == 0 && outParas[0].IsEmpty())*/)
	{
		return false;
	}
	strcpy(szTodayTotalVisitor, outParas[0]);

	//����������
	//strcpy(szT, "select count(*) from t_visitor");
	sprintf(szT, "select count(*) from t_visitor where visitor_station_id = \'%s\' and visitor_status >1 ",g_szStation);
	ret = tAdo.execSelect(szT, &FldCount, outParas);
	if (ret == ADOERR /*|| (FldCount == 0 && outParas[0].IsEmpty())*/)
	{
		return false;
	}
	strcpy(szTotalVisitor, outParas[0]);

	//�ܷ����Ŀ���
	//strcpy(szT, "select t_office.office_name, count(*) from t_visitor, t_office where visitor_station_id = \'%s\' and t_visitor.visitor_to_office_name = t_office.office_name group by t_office.office_name asc",g_szStation);
    sprintf(szT, "select visitor_to_office_name, count(*) as c from t_visitor where visitor_station_id = \'%s\' and visitor_to_office_name<>'' group by visitor_to_office_name order by c desc",g_szStation);
	ret = tAdo.execSelect(szT, &FldCount, outParas);
	if (ret == ADOERR /*|| (FldCount == 0 && outParas[0].IsEmpty())*/)
	{
		return false;
	}
	strcpy(szMaxOffice, outParas[0]);
	sprintf(szMaxOfficeTimes, "���ܷ� %s ��", outParas[1]);

	//�ܷ�������
	//strcpy(szT, "select t_official.official_name, count(*) from t_visitor, t_official where visitor_station_id = \'%s\' and t_visitor.visitor_to_official_name = t_official.official_name group by t_official.official_name desc",g_szStation);
	sprintf(szT, "select visitor_to_official_name, count(*) as c from t_visitor where visitor_station_id = \'%s\' group by visitor_to_official_name order by c desc",g_szStation);
	ret = tAdo.execSelect(szT, &FldCount, outParas);
	if (ret == ADOERR /*|| (FldCount == 0 && outParas[0].IsEmpty())*/)
	{
		return false;
	}
	strcpy(szMaxOfficial, outParas[0]);
	sprintf(szMaxOfficialTimes, "���ܷ� %s ��", outParas[1]);

	//���÷�������
	//strcpy(szT, "select distinct visitor_name, sum(case visitor_name when visitor_name then 1 else 0 end) from t_visitor group by visitor_name asc");
	sprintf(szT, "select visitor_name, count(*) as c from t_visitor where  visitor_station_id = \'%s\' and visitor_status >1 group by visitor_name order by c desc",g_szStation);
	ret = tAdo.execSelect(szT, &FldCount, outParas);
	if (ret == ADOERR /*|| (FldCount == 0 && outParas[0].IsEmpty())*/)
	{
		return false;
	}
	strcpy(szMaxVisitor, outParas[0]);
	sprintf(szMaxVisitorTimes, "������ %s ��", outParas[1]);

	HELE hList = XWnd_GetEle(g_hWindow, IDL_LISTCHAXUNTONGJI);
	XList_DeleteAllColumns(hList);
	XList_DeleteAllItems(hList);
	
	XList_SetHeaderHeight(hList, 30);
	XList_EnableGrid(hList, false);
	XList_EnableMultilineSelect(hList, false);

	XList_AddColumn(hList, 40, L"���");
	XList_AddColumn(hList, 200, L"ͳ������");
	XList_AddColumn(hList, 100, L"���");
	XList_AddColumn(hList, 100, L"��ע");
	
	wchar_t wBuf[256] = {0};

	XList_AddItem(hList, L"1", 0);
	L_V_I("��������������", 0, 1)
	L_V_I(szTodayTotalVisitor, 0, 2)
	L_V_I("", 0, 3)

	XList_AddItem(hList, L"2", 1);
	L_V_I("����������", 1, 1)
	L_V_I(szTotalVisitor, 1, 2)
	L_V_I("", 1, 3)

	XList_AddItem(hList, L"4", 2);
	L_V_I("�ܷ����Ŀ���", 2, 1)
	L_V_I(szMaxOffice, 2, 2)
	L_V_I(szMaxOfficeTimes, 2, 3)

	XList_AddItem(hList, L"5", 3);
	L_V_I("�ܷ�������Ա", 3, 1)
	L_V_I(szMaxOfficial, 3, 2)
	L_V_I(szMaxOfficialTimes, 3, 3)

	XList_AddItem(hList, L"6", 4);
	L_V_I("����������Ա", 4, 1)
	L_V_I(szMaxVisitor, 4, 2)
	L_V_I(szMaxVisitorTimes, 4, 3)

	return true;
}

//���ͳ�Ʋ�ѯҳ���ϵĲ�ѯ��ť
BOOL CALLBACK OnChaxun(HELE hEle, HELE hEventEle) 
{
	if (hEle != hEventEle)
	{
		return false;
	}

	char szT[512] = {'\0'},szSelectHead[512] = {'\0'},szTmp[512] = {'\0'};
	long FldCount = 0, ret = 0, n = 0;
	CString outParas[FLD_MAX], outP1[FLD_MAX];
	CWinAdo tAdo(sCONNSTR_MYSQL), tAdo1(sCONNSTR_MYSQL);


	XBtn_SetCheck(GETELE(IDB_CHAXUN), true);
	XBtn_SetCheck(GETELE(IDB_TONGJI), false);
	XBtn_SetCheck(GETELE(IDB_TONGJICHAXUNRETURN), false);
	
	ret = tAdo.OpenAdo();
	if (ret != ADOOK)
	{
		return false;
	}
	ret = tAdo1.OpenAdo();
	if (ret != ADOOK)
	{
		return false;
	}
	
	for (int i = 0; i < FLD_MAX; i++)
	{
		outParas[i].Empty();
		outP1[i].Empty();
	}

	wchar_t wBuf[128] = {'\0'};
	char szOffice[64] = {'\0'}, szVisitor[64] = {'\0'}, szOfficial[64] = {'\0'}, szID[64] = {'\0'},szIDtype[32] = {'\0'};
    char szVisitorType[64] = {'\0'};

	memset((char *)wBuf, 0, sizeof(wBuf));
	HELE hE = GETELE(IDE_BEIFANGBUMEN);
	XEdit_GetText(hE, wBuf, 128);
	W_2_S(wBuf, szOffice, 128);

	memset((char *)wBuf, 0, sizeof(wBuf));
	hE = GETELE(IDE_BEIFANGRENYUAN);
	XEdit_GetText(hE, wBuf, 128);
	W_2_S(wBuf, szOfficial, 128);

	memset((char *)wBuf, 0, sizeof(wBuf));
	hE = GETELE(IDE_LAIFANGRENYUAN);
	XEdit_GetText(hE, wBuf, 128);
	W_2_S(wBuf, szVisitor, 128);

	memset((char *)wBuf, 0, sizeof(wBuf));
	hE = GETELE(IDE_ZHENGJIANHAOMA);
	XEdit_GetText(hE, wBuf, 128);
	W_2_S(wBuf, szID, 128);

	if(g_bSSO)
	{
		memset((char *)wBuf, 0, sizeof(wBuf));
		hE = GETELE(ID_VisitorType_Comb); //ID_VisitorType_Comb
		//XEdit_GetText(hE, wBuf, 128);
		n = XComboBox_GetSelectItem(hE);
		//XComboBox_GetItemText(hE,n)��
		if(n>0)
		{
			wcscpy(wBuf, XComboBox_GetItemText(hE, n));
			W_2_S(wBuf, szVisitorType, 128);
		}

	}
	else
	{
		hE = GETELE(ID_IDType_Comb);
		n = XComboBox_GetSelectItem(hE);
		if (n>6||wcslen(XComboBox_GetItemText(hE,n))<1)
		{
		   sprintf(szIDtype, "%ld", 99);
		}
		else if (n==0)
		{
			;
		}
		else if (n==1)
		{
			sprintf(szIDtype, "%ld", n);
		}
		else
		   sprintf(szIDtype, "%ld", n+1);
	}



	HELE hList = GETELE(IDL_LISTCHAXUNTONGJI);
	XList_DeleteAllItems(hList);
	XList_DeleteAllColumns(hList);
	
	
	XList_SetHeaderHeight(hList, 30);
	XList_EnableGrid(hList, false);
	XList_EnableMultilineSelect(hList, false);

	HELE hE1 = GETELE(IDC_VISITEDATE);    //��ѡ��������
	HELE hE2 = GETELE(IDC_LEFTDATE);    //��ѡǩ������
 
	bool bNeedAnd=false;
	if (strlen(szOffice) > 0)
	{
        if (!bNeedAnd)
        {
			bNeedAnd=true;
        }
		sprintf(szTmp, "visitor_to_office_name = \'%s\'", szOffice);
		strcat(szT,szTmp);
	}

    if (strlen(szOfficial) > 0)
	{
		if (!bNeedAnd)
        {
			bNeedAnd=true;
        }
		else 
		{
            strcat(szT," and ");
		}
		sprintf(szTmp, "visitor_to_official_name = \'%s\'", szOfficial);
		strcat(szT,szTmp);
	}

    if (strlen(szVisitor) > 0)
	{
		
		if (!bNeedAnd)
        {
			bNeedAnd=true;
        }
		else 
		{
            strcat(szT," and ");
		}
		sprintf(szTmp, "visitor_name = \'%s\'", szVisitor);
		strcat(szT,szTmp);
	}

    if (strlen(szID) > 0)
	{
		if (!bNeedAnd)
        {
			bNeedAnd=true;
        }
		else 
		{
            strcat(szT," and ");
		}
		sprintf(szTmp, "visitor_id = \'%s\'", szID);
		strcat(szT,szTmp);
	}

	if(g_bSSO)
	{
		if (strlen(szVisitorType) > 0)
		{
			if (!bNeedAnd)
			{
				bNeedAnd=true;
			}
			else 
			{
				strcat(szT," and ");
			}
			sprintf(szTmp, "visitor_type = \'%s\'", szVisitorType);
			strcat(szT,szTmp);
		}
	}
	else
	{
		if (strlen(szIDtype) > 0)
		{
			if (!bNeedAnd)
			{
				bNeedAnd=true;
			}
			else 
			{
				strcat(szT," and ");
			}
			sprintf(szTmp, "visitor_idtype = \'%s\'", szIDtype);
			strcat(szT,szTmp);
		}
	}

	
	if (XBtn_IsCheck(hE2))
	{
		hE2 = GETELE(ID_LEFTDATE1);
		//XEdit_GetText(hE, wBuf, 128);
		int y=XDateTime_GetYear(hE2);
		int m=XDateTime_GetMonth(hE2);
		int d=XDateTime_GetDay(hE2);

		hE2 = GETELE(ID_LEFTDATE2);
		//XEdit_GetText(hE, wBuf, 128);
		int y2=XDateTime_GetYear(hE2);
		int m2=XDateTime_GetMonth(hE2);
		int d2=XDateTime_GetDay(hE2);
		//W_2_S(wBuf, szID, 128);
		if (!bNeedAnd)
        {
			bNeedAnd=true;
        }
		else 
		{
            strcat(szT," and ");
		}
		if (g_nSqlConnectType == SQL_CONNECT_SERVER2005)
		{
            sprintf(szTmp, "visitor_lefttime between DATEDIFF(s, '1970-01-01 08:00:00', '%d-%d-%d 00:00:00') and DATEDIFF(s, '1970-01-01 08:00:00', '%d-%d-%d 23:59:59')",y,m,d,y2,m2,d2);
		}
		else
		    sprintf(szTmp, "visitor_lefttime between UNIX_TIMESTAMP('%d-%d-%d 00:00:00') and UNIX_TIMESTAMP('%d-%d-%d 23:59:59')",y,m,d,y2,m2,d2);
		strcat(szT,szTmp);
	}
	
	if (XBtn_IsCheck(hE1))
	{
		hE1 = GETELE(ID_VISITEDATE1);
		//XEdit_GetText(hE, wBuf, 128);
		int y=XDateTime_GetYear(hE1);
		int m=XDateTime_GetMonth(hE1);
		int d=XDateTime_GetDay(hE1);

		hE1 = GETELE(ID_VISITEDATE2);
		//XEdit_GetText(hE, wBuf, 128);
		int y2=XDateTime_GetYear(hE1);
		int m2=XDateTime_GetMonth(hE1);
		int d2=XDateTime_GetDay(hE1);
		//W_2_S(wBuf, szID, 128);
		if (!bNeedAnd)
        {
			bNeedAnd=true;
        }
		else 
		{
            strcat(szT," and ");
		}

		if (g_nSqlConnectType == SQL_CONNECT_SERVER2005)
		{
            sprintf(szTmp, "visitor_visitetime between DATEDIFF(s, '1970-01-01 08:00:00', '%d-%d-%d 00:00:00') and DATEDIFF(s, '1970-01-01 08:00:00', '%d-%d-%d 23:59:59')",y,m,d,y2,m2,d2);
		}
		else
		    sprintf(szTmp, "visitor_visitetime between UNIX_TIMESTAMP('%d-%d-%d 00:00:00') and UNIX_TIMESTAMP('%d-%d-%d 23:59:59')",y,m,d,y2,m2,d2);
		strcat(szT,szTmp);
	
	}
	
	hE1 = GETELE(IDC_VISITEDATE);    //��ѡ��������
	hE2 = GETELE(IDC_LEFTDATE);    //��ѡǩ������

	if (XBtn_IsCheck(hE1)==false && XBtn_IsCheck(hE2)==false)
	{
		if (!bNeedAnd)
        {
			bNeedAnd=true;
        }
		else 
		{
            strcat(szT," and");
		}

		if (g_nSqlConnectType == SQL_CONNECT_SERVER2005)
		{
			sprintf(szTmp, "(DateDiff(d, getdate(),DATEADD(s,visitor_visitetime,'1970-01-01 08:00:00'))=0 or visitor_lefttime <= 0)");
		}
		else
			sprintf(szTmp, "(TO_DAYS(NOW()) - TO_DAYS(FROM_UNIXTIME(visitor_visitetime))=0 or visitor_lefttime <= 0)");
		strcat(szT,szTmp);

	}

    strcat(szT,"order by visitor_visitetime desc");		
    sprintf(szSelectHead,"select t_visitor.*,t_official.official_office_phone from t_visitor inner join t_official on t_visitor.visitor_to_official_name=t_official.official_name and t_visitor.visitor_to_office_name=t_official.official_office_name where visitor_station_id = \'%s\' and visitor_status >1 and ",g_szStation);
	strcat(szSelectHead,szT);
	try{
	//MessageBox(NULL,szSelectHead,"����",MB_OK);
	ret = tAdo.execSelect(szSelectHead, &FldCount, outParas);
	if (ret == ADOERR)
	{
		
		XList_AddColumn(hList, 900, L"��ʾ");
		 XList_AddItem(hList, L"û�в�ѯ������ʾ����Ϣ��", 0);
         XWnd_RedrawWnd(XEle_GetHWindow(hEle));
		return false;
	}
	//MessageBox(NULL,"ok","����",MB_OK);
	g_nChaXunTongJiType = BY_CHAXUN;

	

	if (FldCount > 0)
	{
		XList_AddColumn(hList, 50, L"���");
		XList_AddColumn(hList, 40, L"�Ÿ�");
		XList_AddColumn(hList, 60, L"�ÿ�����");
		XList_AddColumn(hList, 35, L"�Ա�");
		XList_AddColumn(hList, 0, L"����");
		XList_AddColumn(hList, 0, L"֤������");  //5
		XList_AddColumn(hList, 120, L"֤������");
		XList_AddColumn(hList, 0, L"��ͥסַ");
        XList_AddColumn(hList, 0, L"��֤����");
		XList_AddColumn(hList, 0, L"�ÿ͵�λ");
		XList_AddColumn(hList, 0, L"�绰����");  //10
		XList_AddColumn(hList, 0, L"���ƺ���");  
		XList_AddColumn(hList, 125, L"����ʱ��");
		XList_AddColumn(hList, 90, L"��������");
		XList_AddColumn(hList, 90, L"Я����Ʒ");
		XList_AddColumn(hList, 125, L"�뿪ʱ��");//15
		XList_AddColumn(hList, 0, L"��������");  
		XList_AddColumn(hList, 60, L"������");   
		XList_AddColumn(hList, 100, L"��ȥ����");
		XList_AddColumn(hList, 0, L"���õ绰");
		n = 0;
		do 
		{
			memset(szT, 0, sizeof(szT));
			sprintf(szT, "%d", n + 1);		//���
			memset((char *)wBuf, 0, sizeof(wBuf));
			S_2_W(wBuf, szT, 128);
			XList_AddItem(hList, wBuf, n);
			
			//list visitor information
			L_V_I(outParas[18], n, 1)		//�Ÿ�
				L_V_I(outParas[0], n, 2)    //����
				L_V_I(outParas[1], n, 3)    //�Ա�
				L_V_I(outParas[2], n, 4)    //����
				L_V_I(outParas[3], n, 10) //�绰
				L_V_I(outParas[4], n, 5) //֤������
				L_V_I(outParas[5], n, 6)    //֤������
				L_V_I(outParas[6], n, 8) //��֤����
				L_V_I(outParas[7], n, 14) //Я����Ʒ
				L_V_I(outParas[8], n, 13) //��������
				L_V_I(outParas[9], n, 7) //��ַ
				L_V_I(outParas[10], n, 16) //��������
				L_V_I(outParas[11], n, 9) //�ÿ͵�λ
				L_V_I(outParas[12], n, 11) //���ƺ�
				
				if (atol(outParas[16]) <= 0)
				{
					L_V_I("", n, 12)
				}
				else
				{
					//sprintf(szT, "select FROM_UNIXTIME(%s)", outParas[16]);	//����ʱ��
					if (g_nSqlConnectType == SQL_CONNECT_SERVER2005)
					{
						 sprintf(szT, "SELECT DATEADD(s,%d,'1970-01-01 08:00:00')", atol(outParas[16]));	//����ʱ��
					}
					else
					{
                         sprintf(szT, "select FROM_UNIXTIME(%s)", outParas[16]);	//����ʱ��
					}
					ret = tAdo1.execSelect(szT, &FldCount, outP1);
					if (ret == ADOERR || (FldCount == 0 && outP1[0].IsEmpty()))
					{
						L_V_I("", n, 12)
					}
					else
					{
						L_V_I(outP1[0], n, 12)			//����ʱ��
					}
				}
				
					if (atol(outParas[17]) <= 0)
					{
						L_V_I("", n, 15)
					}
					else
					{
						//sprintf(szT, "select FROM_UNIXTIME(%s)", outParas[17]);	//�뿪ʱ��
						if (g_nSqlConnectType == SQL_CONNECT_SERVER2005)
						{
							sprintf(szT, "SELECT DATEADD(s,%d,'1970-01-01 08:00:00')", atol(outParas[17]));	//�뿪ʱ��
						}
						else
						{
							sprintf(szT, "select FROM_UNIXTIME(%s)", outParas[17]);	//�뿪ʱ��
						}
						ret = tAdo1.execSelect(szT, &FldCount, outP1);
						if (ret == ADOERR || (FldCount == 0 && outP1[0].IsEmpty()))
						{
							L_V_I("", n, 15)
						}
						else
						{
							L_V_I(outP1[0], n, 15)			//�뿪ʱ��
						}
					}
					
					L_V_I(outParas[14], n, 17)			//����������
						L_V_I(outParas[15], n, 18)			//�����˰칫��
						L_V_I(outParas[16], n, 19)
						n++;
		} while(tAdo.getNext(outParas) == ADOOK);
	}
	else
	{
		 //MessageBox(NULL,"ok2","����",MB_OK);
		 XList_DeleteAllColumns(hList);
		 XList_AddColumn(hList, 900, L"��ʾ");
		 XList_AddItem(hList, L"û�в�ѯ������ʾ����Ϣ��", 0);
		 //MessageBox(NULL,"ok3","����",MB_OK);
         //XWnd_RedrawWnd(XEle_GetHWindow(hEle));
		//XEle_SetTextColor(hList,RGB(255,0,0));
	}

	}catch(...)
	{
	}
    //XEle_EnableEle(hList,FALSE);
	XWnd_RedrawWnd(XEle_GetHWindow(hEle));
	return false;
}

//���ͳ�Ʋ�ѯҳ���ϵķ��ذ�ť
BOOL CALLBACK OnTongjiChaxunReturn(HELE hEle, HELE hEventEle) 
{
	if (hEle != hEventEle)
	{
		return false;
	}

    if (::GetPrivateProfileInt("TerminalSet", "bMainChaxun",0, CONFIG_FILE)==1)
	{
		return OnReturnSet(hEle,hEventEle);
	}

	XEle_ShowEle(g_hPage1, false);
	XEle_ShowEle(g_hPage6, false);
	XEle_ShowEle(g_hPage3, true);
	g_bICReg = true;
    ShowWindow(g_hCapWnd, false);
	Pause();
	//OnQueryEmployerForList(XWnd_GetEle(g_hWindow, IDB_QUERYOFFICIAL), XWnd_GetEle(g_hWindow, IDB_QUERYOFFICIAL));	
	XEle_ShowEle(XWnd_GetEle(g_hWindow, IDP_PAGE3_1), true);
	
	EnableButton(IDB_TAB32);

	SetSexInfo(IDC_ADD_OFFICIALSEX);
	HELE hE = XWnd_GetEle(g_hWindow, IDC_ADD_OFFICIALSEX);
	XComboBox_AddString(hE, L" ");

	HELE hList = XWnd_GetEle(g_hWindow, IDL_LISTOFFICIAL);		//����б�Ԫ��
	XList_DeleteAllItems(hList);
	XList_DeleteAllColumns(hList);
	
	XList_SetHeaderHeight(hList, 30);
	XList_EnableGrid(hList, false);

	XList_AddColumn(hList, 40, L"���");
	XList_AddColumn(hList, 100, L"����");
	XList_AddColumn(hList, 40, L"�Ա�");
	XList_AddColumn(hList, 100, L"�칫�绰");
	XList_AddColumn(hList, 100, L"�ƶ��绰");
	XList_AddColumn(hList, 140, L"���ſ���");
	XList_AddColumn(hList, 160, L"�칫�ص�");
	if(g_bSSO)
	{		
		XList_AddColumn(hList, 0, L"����");
		XList_AddColumn(hList, 0, L"����");
		XList_AddColumn(hList, 70, L"SSO");
		XList_AddColumn(hList, 70, L"IC��");
	}
	else
	{
		XList_AddColumn(hList, 70, L"����");
		XList_AddColumn(hList, 70, L"����");
		XList_AddColumn(hList, 0, L"SSO");
		XList_AddColumn(hList, 0, L"IC��");
	}

	XList_AddColumn(hList, 90, L"IP��ַ");
    OnQueryEmployerForList(XWnd_GetEle(g_hWindow, IDB_QUERYOFFICIAL), XWnd_GetEle(g_hWindow, IDB_QUERYOFFICIAL));
	XWnd_RedrawWnd(XEle_GetHWindow(hEle));
	return false;
}

//��������������ն˵İ�ť��Ӧ
BOOL CALLBACK OnSendOfficialRequest(HELE hEle, HELE hEventEle) 
{
	if (hEle != hEventEle)
	{
		return false;
	}

	//strcpy(g_szOfficialIP,"cc");
	//SendRequestToOfficial("<visitor><name>ˮ �� ��</name><id>1234567890</id><reason>�;�</reason><Pic>c:\\head.bmp</Pic><ip>127.0.0.1</ip></visitor>");
    //return false;

	HideSoftKeybd();
	EnableButton(IDB_TAB3);
	
	
	VISITORINFO visitor;
	wchar_t wBuf[256 + 1] = {0};
	long n = 0;
	char sz[64] = {'\0'};
	char szFolkName[64] = {'\0'};
	char szOfficialName[64] = {'\0'};
	char szOfficeName[64] = {'\0'};
	long FldCount = 0;
	CWinAdo tAdo(sCONNSTR_MYSQL);
	
	int ret = tAdo.OpenAdo();
	if (ret != ADOOK)
	{
		return false;
	}

	memset((char *)&visitor, 0, sizeof(VISITORINFO));
	::GetPrivateProfileString("MustInput", "VisitorName", "1", sz, sizeof(sz), CONFIG_FILE);
	HELE hE = XWnd_GetEle(g_hWindow, IDE_VISITORNAME);	//�ÿ�����
	if (XEdit_GetTextLength(hE) <= 0 && atoi(sz) == 1)
	{
		XMessageBox(g_hWindow, L"�ÿ���������Ϊ��", L"����");
		return false;
	}
	if (XEdit_GetTextLength(hE) > 16)
	{
		XMessageBox(g_hWindow, L"�ÿ�����̫��", L"����");
		return false;
	}
	XEdit_GetText(hE, wBuf, 128);
	W_2_S(wBuf, visitor.szName, 128);
	W_2_S(wBuf, g_szVisitorName, 128);

	memset((char *)wBuf, 0, sizeof(wBuf));
	hE = XWnd_GetEle(g_hWindow, IDC_VISITORSEX);		//�ÿ��Ա�
	//n = XComboBox_GetSelectItem(hE);
	XEdit_GetText(hE, wBuf, 128);
    W_2_S(wBuf, visitor.szSex, 128);
	//n == 0 ? strcpy(visitor.szSex, "��") : strcpy(visitor.szSex, "Ů");

	memset((char *)wBuf, 0, sizeof(wBuf));
	::GetPrivateProfileString("MustInput", "VisitorMobile", "0", sz, sizeof(sz), CONFIG_FILE);
	hE = XWnd_GetEle(g_hWindow, IDE_VISITORMOBILE);		//�ÿ͵绰
	if (XEdit_GetTextLength(hE) <= 0 && atoi(sz) == 1)
	{
		XMessageBox(g_hWindow, L"�ÿ͵绰����Ϊ��", L"����");
		return false;
	}
	if (XEdit_GetTextLength(hE) > 16)
	{
		XMessageBox(g_hWindow, L"�ÿ͵绰����̫��", L"����");
		return false;
	}
	XEdit_GetText(hE, wBuf, 128);
	W_2_S(wBuf, visitor.szPhone, 128);

	memset((char *)wBuf, 0, sizeof(wBuf));
	::GetPrivateProfileString("MustInput", "VisitorID", "0", sz, sizeof(sz), CONFIG_FILE);
	hE = XWnd_GetEle(g_hWindow, IDE_VISITORIDNUM);		//�ÿ�֤������
	if (XEdit_GetTextLength(hE) <= 0 && atoi(sz) == 1)
	{
		XMessageBox(g_hWindow, L"�ÿ�֤�����벻��Ϊ��", L"����");
		return false;
	}
	if (XEdit_GetTextLength(hE) <= 0 && atoi(sz) == 1)
	{
		XMessageBox(g_hWindow, L"�ÿ�֤�����벻��Ϊ��", L"����");
		return false;
	}
	XEdit_GetText(hE, wBuf, 128);
	W_2_S(wBuf, visitor.szID, 128);

	memset((char *)wBuf, 0, sizeof(wBuf));
	::GetPrivateProfileString("MustInput", "VisitorGoods", "0", sz, sizeof(sz), CONFIG_FILE);
	hE = XWnd_GetEle(g_hWindow, IDE_VISITORTHING);		//�ÿ���Ʒ
	if (XEdit_GetTextLength(hE) <= 0 && atoi(sz) == 1)
	{
		XMessageBox(g_hWindow, L"�ÿ�Я����Ʒ����Ϊ��", L"����");
		return false;
	}
	if (XEdit_GetTextLength(hE) > 30)
	{
		XMessageBox(g_hWindow, L"�ÿ�Я����Ʒ̫��", L"����");
		return false;
	}
	XEdit_GetText(hE, wBuf, 128);
	W_2_S(wBuf, visitor.szWithGoods, 128);

	memset((char *)wBuf, 0, sizeof(wBuf));
	hE = XWnd_GetEle(g_hWindow, IDC_VISITORIDTYPE);		//�ÿ�֤������
	//n = XComboBox_GetSelectItem(hE);
    XEdit_GetText(hE, wBuf, 128);
    W_2_S(wBuf, visitor.szIDType, 128);

	if (strcmp(visitor.szIDType,"�������֤")==0)
	{
        n=0;
	}
	else if (strcmp(visitor.szIDType,"һ�����֤")==0)
	{
        n=1;
	}
	else if (strcmp(visitor.szIDType,"����")==0)
	{
        n=2;
	}
	else if (strcmp(visitor.szIDType,"��½����۰�ͨ��֤")==0)
	{
        n=3;
	}
	else if (strcmp(visitor.szIDType,"�������֤")==0)
	{
        n=4;
	}
	else if (strcmp(visitor.szIDType,"̨��֤")==0)
	{
        n=5;
	}
	else if (strcmp(visitor.szIDType,"��ʻ֤")==0)
	{
        n=6;		
	}
	else if (strcmp(visitor.szIDType,"��ʦ֤")==0)
	{
        n=7;		
	}
	else if (strcmp(visitor.szIDType,"��Ƭ")==0)
	{
        n=8;		
	}
	else
        n=98;
	sprintf(visitor.szIDType, "%ld", n + 1);

	memset((char *)wBuf, 0, sizeof(wBuf));
	hE = XWnd_GetEle(g_hWindow, IDC_VISITORFOLK);		//�ÿ�����
	//n = XComboBox_GetSelectItem(hE);
	//sprintf(visitor.szFolk, "%ld", n + 1);
	//wcscpy(wBuf, XComboBox_GetItemText(hE, n));
	XEdit_GetText(hE, wBuf, 128);
	W_2_S(wBuf, visitor.szFolk, 32);

	memset((char *)wBuf, 0, sizeof(wBuf));
	::GetPrivateProfileString("MustInput", "VisitorReason", "0", sz, sizeof(sz), CONFIG_FILE);
	hE = XWnd_GetEle(g_hWindow, IDE_VISITORREASON);		//�ÿ�ԭ��
	if (XEdit_GetTextLength(hE) <= 0 && atoi(sz) == 1)
	{
		XMessageBox(g_hWindow, L"�ÿ��������ɲ���Ϊ��", L"����");
		return false;
	}
	if (XEdit_GetTextLength(hE) > 20)
	{
		XMessageBox(g_hWindow, L"�ÿ���������̫��", L"����");
		return false;
	}
	XEdit_GetText(hE, wBuf, 128);
	W_2_S(wBuf, visitor.szReason, 128);

	memset((char *)wBuf, 0, sizeof(wBuf));
	::GetPrivateProfileString("MustInput", "VisitorAddr", "0", sz, sizeof(sz), CONFIG_FILE);
	hE = XWnd_GetEle(g_hWindow, IDE_VISITORADDR);		//�ÿ͵�ַ
	if (XEdit_GetTextLength(hE) <= 0 && atoi(sz) == 1)
	{
		XMessageBox(g_hWindow, L"�ÿ͵�ַ����Ϊ��", L"����");
		return false;
	}
	if (XEdit_GetTextLength(hE) > 50)
	{
		XMessageBox(g_hWindow, L"�ÿ͵�ַ̫��", L"����");
		return false;
	}
	XEdit_GetText(hE, wBuf, 128);
	W_2_S(wBuf, visitor.szAddr, 128);

	if(g_bVisitorType)
	{
		strcpy(visitor.szTotalPeople, "1");  //Ĭ��1��
		
		memset((char *)wBuf, 0, sizeof(wBuf));
		::GetPrivateProfileString("MustInput", "VisitorTotal", "0", sz, sizeof(sz), CONFIG_FILE);
		hE = XWnd_GetEle(g_hWindow, IDE_VISITORTYPE);		//�ÿ�����
		if (XEdit_GetTextLength(hE) <= 0 && atoi(sz) == 1)
		{
			XMessageBox(g_hWindow, L"�ÿ����Ͳ���Ϊ��", L"����");
			return false;
		}
		if (XEdit_GetTextLength(hE) > 50)
		{
			XMessageBox(g_hWindow, L"�ÿ�����̫��", L"����");
			return false;
		}
		XEdit_GetText(hE, wBuf, 128);
		W_2_S(wBuf, visitor.szType, 128);
	}
	else
	{
		memset((char *)wBuf, 0, sizeof(wBuf));
		::GetPrivateProfileString("MustInput", "VisitorTotal", "0", sz, sizeof(sz), CONFIG_FILE);
		hE = XWnd_GetEle(g_hWindow, IDE_VISITORTOTAL);		//�ÿ�����
		if (XEdit_GetTextLength(hE) <= 0 && atoi(sz) == 1)
		{
			XMessageBox(g_hWindow, L"�ÿ�������������Ϊ��", L"����");
			return false;
		}
		if (XEdit_GetTextLength(hE) > 3)
		{
			XMessageBox(g_hWindow, L"�ÿ���������̫��", L"����");
			return false;
		}
		XEdit_GetText(hE, wBuf, 128);
		W_2_S(wBuf, visitor.szTotalPeople, 128);
		strcpy(visitor.szType, "");
	}
	/*

	*/


	memset((char *)wBuf, 0, sizeof(wBuf));
	::GetPrivateProfileString("MustInput", "VisitorUnit", "0", sz, sizeof(sz), CONFIG_FILE);
	hE = XWnd_GetEle(g_hWindow, IDE_VISITORUNIT);		//�ÿ͵�λ
	if (XEdit_GetTextLength(hE) <= 0 && atoi(sz) == 1)
	{
		XMessageBox(g_hWindow, L"�ÿ͵�λ����Ϊ��", L"����");
		return false;
	}
	if (XEdit_GetTextLength(hE) > 50)
	{
		XMessageBox(g_hWindow, L"�ÿ͵�λ̫��", L"����");
		return false;
	}
	XEdit_GetText(hE, wBuf, 128);
	W_2_S(wBuf, visitor.szUnit, 128);

	memset((char *)wBuf, 0, sizeof(wBuf));
	::GetPrivateProfileString("MustInput", "VisitorCarNum", "0", sz, sizeof(sz), CONFIG_FILE);
	hE = XWnd_GetEle(g_hWindow, IDE_VISITORCARNUM);		//���ƺ�
	if (XEdit_GetTextLength(hE) <= 0 && atoi(sz) == 1)
	{
		XMessageBox(g_hWindow, L"�ÿͳ��ƺ��벻��Ϊ��", L"����");
		return false;
	}
	if (XEdit_GetTextLength(hE) > 10)
	{
		XMessageBox(g_hWindow, L"�ÿͳ��ƺ���̫��", L"����");
		return false;
	}
	XEdit_GetText(hE, wBuf, 128);
	W_2_S(wBuf, visitor.szCarNum, 128);

	memset((char *)wBuf, 0, sizeof(wBuf));
	::GetPrivateProfileString("MustInput", "VisitorSignDepartment", "0", sz, sizeof(sz), CONFIG_FILE);
	hE = XWnd_GetEle(g_hWindow, IDE_FZJG);				//��֤����
	if(g_bJob)
	{
		if (XEdit_GetTextLength(hE) <= 0 && atoi(sz) == 1)
		{
			XMessageBox(g_hWindow, L"�ÿ�ְ�����ֲ���Ϊ��", L"����");
			return false;
		}
		if (XEdit_GetTextLength(hE) > 50)
		{
			XMessageBox(g_hWindow, L"�ÿ�ְ������̫��", L"����");
			return false;
		}
		XEdit_GetText(hE, wBuf, 128);
		W_2_S(wBuf, visitor.szJob, 128);
		
		if(n==0)  // �������֤
		{
			//memset((char *)wBuf, 0, sizeof(wBuf));
			hE = XWnd_GetEle(g_hWindow, IDS_ID2_SIGNDEPARTMENT);
			//XStatic_GetText(hE, wBuf, 128);
		    W_2_S(XStatic_GetText(hE), visitor.szSignDepartment, 128);
		}
	}
	else
	{
		if (XEdit_GetTextLength(hE) <= 0 && atoi(sz) == 1)
		{
			XMessageBox(g_hWindow, L"�ÿ�֤���ķ�֤���ز���Ϊ��", L"����");
			return false;
		}
		if (XEdit_GetTextLength(hE) > 30)
		{
			XMessageBox(g_hWindow, L"�ÿ�֤���ķ�֤����̫��", L"����");
			return false;
		}
		XEdit_GetText(hE, wBuf, 128);
		W_2_S(wBuf, visitor.szSignDepartment, 128);
	}




	memset((char *)wBuf, 0, sizeof(wBuf));
	::GetPrivateProfileString("MustInput", "OfficialName", "1", sz, sizeof(sz), CONFIG_FILE);
	hE = XWnd_GetEle(g_hWindow, IDE_MASTERNAME);		//������������
	if (XEdit_GetTextLength(hE) <= 0 && atoi(sz) == 1)
	{
		XMessageBox(g_hWindow, L"����������������Ϊ��", L"����");
		return false;
	}
	if (XEdit_GetTextLength(hE) > 16)
	{
		XMessageBox(g_hWindow, L"������������̫��", L"����");
		return false;
	}
	XEdit_GetText(hE, wBuf, 128);
	W_2_S(wBuf, visitor.szToOfficialName, 128);

	memset((char *)wBuf, 0, sizeof(wBuf));
	::GetPrivateProfileString("MustInput", "OfficeName", "0", sz, sizeof(sz), CONFIG_FILE);
	hE = XWnd_GetEle(g_hWindow, IDE_OFFICENAME);			//�����˰칫��
	if (XEdit_GetTextLength(hE) <= 0 && atoi(sz) == 1)
	{
		XMessageBox(g_hWindow, L"�������˰칫����������Ϊ��", L"����");
		return false;
	}
	if (XEdit_GetTextLength(hE) > 20)
	{
		XMessageBox(g_hWindow, L"�������˰칫������̫��", L"����");
		return false;
	}
	XEdit_GetText(hE, wBuf, 128);
	W_2_S(wBuf, visitor.szToOfficeName, 128);


	memset((char *)wBuf, 0, sizeof(wBuf));
	::GetPrivateProfileString("MustInput", "OfficialPhone", "0", sz, sizeof(sz), CONFIG_FILE);
	hE = XWnd_GetEle(g_hWindow, IDE_OFFICEPHONE);				//�����˰칫�绰
	if (XEdit_GetTextLength(hE) <= 0 && atoi(sz) == 1)
	{
		XMessageBox(g_hWindow, L"�������˰칫�绰����Ϊ��", L"����");
		return false;
	}
	if (XEdit_GetTextLength(hE) > 10)
	{
		XMessageBox(g_hWindow, L"�������˰칫�绰̫��", L"����");
		return false;
	}

	memset((char *)wBuf, 0, sizeof(wBuf));
	::GetPrivateProfileString("MustInput", "OfficeAddr", "0", sz, sizeof(sz), CONFIG_FILE);
	hE = XWnd_GetEle(g_hWindow, IDE_MASTERMOBILE);				//�����˰칫��ַ
	if (XEdit_GetTextLength(hE) <= 0 && atoi(sz) == 1)
	{
		XMessageBox(g_hWindow, L"�������˰칫��ַ����Ϊ��", L"����");
		return false;
	}
	if (XEdit_GetTextLength(hE) > 15)
	{
		XMessageBox(g_hWindow, L"�������˰칫��ַ̫��", L"����");
		return false;
	}


	if(g_bSSO)
	{
		memset((char *)wBuf, 0, sizeof(wBuf));
		::GetPrivateProfileString("MustInput", "OfficialSSO", "0", sz, sizeof(sz), CONFIG_FILE);
		hE = XWnd_GetEle(g_hWindow, IDC_MASTERSSO);				//�����˷����
		if (XEdit_GetTextLength(hE) <= 0 && atoi(sz) == 1)
		{
			XMessageBox(g_hWindow, L"��������SSO����Ϊ��", L"����");
			return false;
		}
		if (XEdit_GetTextLength(hE) > 10)
		{
			XMessageBox(g_hWindow, L"��������SSO̫��", L"����");
			return false;
		}
		XEdit_GetText(hE, wBuf, 128);
		W_2_S(wBuf, visitor.szToSSO, 128);
		//W_2_S(wBuf, official.szSSO,128);
	}
	else
	{
		memset((char *)wBuf, 0, sizeof(wBuf));
		::GetPrivateProfileString("MustInput", "OfficialRoom", "0", sz, sizeof(sz), CONFIG_FILE);
		hE = XWnd_GetEle(g_hWindow, IDE_ROOMNUM);				//�����˷����
		if (XEdit_GetTextLength(hE) <= 0 && atoi(sz) == 1)
		{
			XMessageBox(g_hWindow, L"�������˷���Ų���Ϊ��", L"����");
			return false;
		}
		if (XEdit_GetTextLength(hE) > 10)
		{
			XMessageBox(g_hWindow, L"�������˷����̫��", L"����");
			return false;
		}
		XEdit_GetText(hE, wBuf, 128);
		W_2_S(wBuf, visitor.szToRoom, 128);
		//W_2_S(wBuf, official.szRoom,128);

		memset((char *)wBuf, 0, sizeof(wBuf));
		::GetPrivateProfileString("MustInput", "OfficialMobile", "0", sz, sizeof(sz), CONFIG_FILE);
		hE = XWnd_GetEle(g_hWindow, IDE_MASTERMOBILE);				//�������ƶ��绰
		if (XEdit_GetTextLength(hE) <= 0 && atoi(sz) == 1)
		{
			XMessageBox(g_hWindow, L"���������ֻ��Ų���Ϊ��", L"����");
			return false;
		}
		if (XEdit_GetTextLength(hE) > 20)
		{
			XMessageBox(g_hWindow, L"���������ֻ���̫��", L"����");
			return false;
		}
		//XEdit_GetText(hE, wBuf, 128);
		//W_2_S(wBuf, official.szMobile,128);

		memset((char *)wBuf, 0, sizeof(wBuf));
		::GetPrivateProfileString("MustInput", "OfficialLevel", "0", sz, sizeof(sz), CONFIG_FILE);
		hE = XWnd_GetEle(g_hWindow, IDC_MASTERZHIWU);				//�����˼���
		if (XEdit_GetTextLength(hE) <= 0 && atoi(sz) == 1)
		{
			XMessageBox(g_hWindow, L"�������˼�����Ϊ��", L"����");
			return false;
		}
		if (XEdit_GetTextLength(hE) > 10)
		{
			XMessageBox(g_hWindow, L"�������˼���̫��", L"����");
			return false;
		}
		//XEdit_GetText(hE, wBuf, 128);
		//W_2_S(wBuf, official.szOfficialDuty,128);
	}
	//�ÿʹ��ĸ��Ÿڽ���
	::GetPrivateProfileString("TerminalSet", "StationName", "", visitor.szVisitStation, sizeof(visitor.szVisitStation), CONFIG_FILE);
	time((time_t *)&n);
	visitor.lVisiteTime = n;			//����ʱ��
	visitor.lLeftTime = 0;				//�뿪ʱ��
	CString str;
	str.Format("%ld",n);
	strcpy(g_szRecord,str);
    char szT[256] = {'\0'};
    int b = CheckVisitor(visitor);
	if (b==1)
	{
		wchar_t wBuf[256] = {0};
		char szT[512] = {'\0'};
		long FldCount = 0, ret = 0;;
		CString outParas[FLD_MAX];
		CWinAdo tAdo(sCONNSTR_MYSQL);
		
		ret = tAdo.OpenAdo();
		if (ret != ADOOK)
		{
			return true;
		}
		
		for (int i = 0; i < FLD_MAX; i++)
		{
			outParas[i].Empty();
		}
		
		sprintf(szT, "select * from t_visitor where visitor_name = \'%s\' and visitor_id = \'%s\' and visitor_status>1 and visitor_station_id = \'%s\' and visitor_lefttime <= 0",\
			visitor.szName, visitor.szID,g_szStation);
		ret = tAdo.execSelect(szT, &FldCount, outParas);
		if (ret == ADOERR || (FldCount == 0 && outParas[0].IsEmpty()))
		{
			return false;
		}

        outParas[19].Replace("#", "\\");
		strcpy(visitor.szUserVideoPic, outParas[19]);
		//strcpy(g_VisitorPic.szUserVideoPic, outParas[19]);
		outParas[20].Replace("#", "\\");
		strcpy(visitor.szUserIDPic, outParas[20]);
		//strcpy(g_VisitorPic.szUserIDPic, outParas[20]);
		outParas[21].Replace("#", "\\");
		strcpy(visitor.szUserHeadPic, outParas[21]);
		outParas[26].Replace("#", "\\");
		strcpy(visitor.szUserIDFullPic, outParas[26]);
		//strcpy(g_VisitorPic.szUserHeadPic, outParas[21]);
        memset((char *)&g_VisitorPic, 0, sizeof(PICPATH));
		strcpy(g_szVisitorName,outParas[0]);
	    strcpy(g_szRecord,outParas[16]);
	}
	else if (b==-1)
	{
		//XMessageBox(g_hWindow, L"�������߸ı䱻���ˣ�����ǩ������µǼ�!", L"��ʾ");
        return false;
	}
	else  //������
	{
		
		char szF[256] = {'\0'}, szTime[32] = {'\0'};
		GetDayFolder(szF);
		sprintf(szT, "%s\\%s_%s_%ld_video.jpg", szF, visitor.szName, visitor.szID, n);
		::CopyFile(USER_VIDEO_PIC, szT, false);	//����Ƶ�������Ƭ�������
		if (g_bFtpOpened)
		{
             m_pFtpConnection->PutFile(szT,szT);     //ftp�ϴ��ļ�
		}
		
		CString str = CString(szT);
		strcpy(g_VisitorPic.szUserVideoPic, szT);	//��������Ƶ��Ƭ���·��
		str.Replace("\\", "#");					//����������ݿ��е�·�����Ż���#
		strcpy(visitor.szUserVideoPic, str);

		sprintf(szT, "%s\\%s_%s_%ld_idfull.jpg", szF, visitor.szName, visitor.szID, n);
		::CopyFile(USER_ID2_PIC, szT, false);	//��֤����Ƭ�������
		if (g_bFtpOpened)
		{
             m_pFtpConnection->PutFile(szT,szT);     //ftp�ϴ��ļ�
		}
		str = CString(szT);
		strcpy(g_VisitorPic.szUserIDFullPic, szT);	//��������Ƶ��Ƭ���·��
		str.Replace("\\", "#");					//����������ݿ��е�·�����Ż���#
		strcpy(visitor.szUserIDFullPic, str);
		
		sprintf(szT, "%s\\%s_%s_%ld_id.bmp", szF, visitor.szName, visitor.szID, n);
		if (g_nID2ReadType==ID2_READ)
		{
            ::CopyFile(g_stIDCard.szPicPath, szT, false);
			sprintf(visitor.szID2Flag, "%d", g_nID2ReadType);	//����֤���Ķ���ʽ
		}
		else
		    ::CopyFile(USER_ID_PIC, szT, false);	//���û�ɨ���֤����Ƭ�������
		if (g_bFtpOpened)
		{
             m_pFtpConnection->PutFile(szT,szT);     //ftp�ϴ��ļ�
		}
		
		if (g_nID2ReadType == ID2_READ)
		{
		    strcpy(visitor.szValidDate, g_stIDCard.validterm);		//����֤����Ч����
		    strcpy(visitor.szBirth, g_stIDCard.birthday);			//����
		}


		//::CopyFile(USER_ID_PIC, szT, false);	//���û�ɨ���֤����Ƭ�������
		str = CString(szT);
		//strcpy(visitor.szUserIDPic, szT);
		strcpy(g_VisitorPic.szUserIDPic, szT);
		str.Replace("\\", "#");					//����������ݿ��е�·�����Ż���#
		strcpy(visitor.szUserIDPic, str);			//������֤����Ƭ���·��
		
		
		sprintf(szT, "%s\\%s_%s_%ld_head.bmp", szF, visitor.szName, visitor.szID, n);
		::CopyFile(USER_HEAD_PIC, szT, false);	//���û�ɨ���ͷ����Ƭ�������
		if (g_bFtpOpened)
		{
             m_pFtpConnection->PutFile(szT,szT);     //ftp�ϴ��ļ�
		}
		str = CString(szT);
		strcpy(g_VisitorPic.szUserHeadPic, szT);
		str.Replace("\\", "#");					//����������ݿ��е�·�����Ż���#
		strcpy(visitor.szUserHeadPic, str);		//������ͷ����Ƭ���·��
		visitor.nStatus=1;                          //�Ѻ���״̬
	    BOOL bRet = SaveVisitorInfo(visitor);
		if (bRet==false)
		{
			XMessageBox(g_hWindow,L"���������˵Ǽ���Ϣʧ��,����������Ϣ�����Ƿ�Խ��",L"��ʾ");
			return false;
		}
		g_bCalled = true;		//�ÿͱ����й��������ӡʱ��ֻ�����visitor_status״̬����
	}

	CString outParas[FLD_MAX];
	for (int i = 0; i < FLD_MAX; i++)
	{
		outParas[i].Empty();
	}
    sprintf(szT, "select * from t_official where official_name = \'%s\' and official_office_name = \'%s\'", visitor.szToOfficialName, visitor.szToOfficeName);
	ret = tAdo.execSelect(szT, &FldCount, outParas);
	strcpy(g_szOfficialIP, outParas[8]); //������ip

    //��ʾ���ս�ͼ
	if (::GetPrivateProfileInt("TerminalSet", "bVisitorPicMust",0, CONFIG_FILE)==1)
	{
		CString sVideoPic,sHeadPic,sIDPic;
		sVideoPic=CString(visitor.szUserVideoPic);
		sVideoPic.Replace("#", "\\");
		if(_access(sVideoPic,0)!=0) // �������ļ�
		{
			XMessageBox(g_hWindow,L"������û������ͼƬ",L"��ʾ");
			return false;
		}
		
		sHeadPic=CString(visitor.szUserHeadPic);
		sHeadPic.Replace("#", "\\");
		sIDPic=CString(visitor.szUserIDPic);
		sIDPic.Replace("#", "\\");
		if (atoi(visitor.szIDType) == ID2_READ)		//�Ķ��Ķ���֤
		{
			if(_access(sHeadPic,0)!=0&&_access(sIDPic,0)!=0)  //Сͼ��������
			{
				XMessageBox(g_hWindow,L"������������Ҫͷ��Сͼ���ͼ",L"��ʾ");
				return false;
			}
		}
		else
		{
			if(_access(sHeadPic,0)!=0)  //ɨ��õ���֤����ֻ�жϽ�ͼ
			{
				XMessageBox(g_hWindow,L"��������Ҫ��ͼ",L"��ʾ");
				return false;
			}
		}
	}

	//char szXml[1024] = {'\0'};

	memset(g_szXmlInfo, 0, sizeof(g_szXmlInfo));
	if(!getVisitor2Xml(g_szXmlInfo,visitor,g_szOfficialIP))
	{
		XMessageBox(g_hWindow,L"��������Ҫ����һ��ͼƬ��",L"��ʾ");
		return false;
	}

	//strcpy(g_szOfficialIP,"cc");
	//strcpy(g_szXmlInfo,"<visitor><name>ˮ �� ��</name><id></id><reason></reason><Pic>c:\\ˮ �� ��_head.bmp</Pic><ip>127.0.0.1</ip></visitor>");
	//MessageBox(XEle_GetHWnd(hEle),g_szXmlInfo,"��ʾ",MB_OK);
	
	SendRequestToOfficial(g_szXmlInfo);
	//SendRequestToOfficial("<visitor><name>ˮ �� ��</name><id></id><reason></reason><Pic>c:\\ˮ �� ��_head.bmp</Pic><ip>127.0.0.1</ip></visitor>");
	g_bRequestThreadIsExist = true;
	//return false;

	//ret = SendRequestToOfficial(g_szXmlInfo);
    //g_bPrintAfterAllow=true;
	if (::GetPrivateProfileInt("TerminalSet", "bPrintAfterAllow",0, CONFIG_FILE)==1)
	{
		while(g_bRequestThreadIsExist)
			Sleep(200);

		if (g_bPrintAfterAllow)
		{
			g_bPrintAfterAllow=false;
			if (!b)
			{
				BackScreenPlayADV();//��ӡ��͵�ʱ���ָ��������
				//if (g_bAcceptVisite)
				{
					SaveAndPrint();
				}
			}	
			else if (b==-1)  //�����˸ı���δǩ��
			{
				return false;
			}
			else             //�����͵�
			{
				if (strlen(g_Printvisitor.szName)==0)
				{
					XMessageBox(g_hWindow, L"�޴�ӡ��Ϣ!", L"����");
					return false;
				}
				PrintVisitorNoteAgain();

			}
			
		}
	}

	XWnd_RedrawWnd(XEle_GetHWindow(hEle));
	return false;
}

BOOL CALLBACK OnSearchPage(HELE hEle, HELE hEventEle)
{
	EnableButton(IDB_TAB4);

	//::WritePrivateProfileString("TerminalSet", "DataInputPage", "1", CONFIG_FILE);
	//ExecPro(".\\softKeybdApp.exe", true, NULL);
	//ExecPro("%SystemRoot%\system32\mstsc.exe", true, NULL);
	StartKeybd();
    MoveBottomSoftKeybd();
	//SendKeyBdAppData("299");
	XWnd_RedrawWnd(g_hWindow);
	return false;
}

BOOL CALLBACK OnCallHandInput(HELE hEle, HELE hEventEle)
{
	EnableButton(IDB_TAB4);
	
	//::WritePrivateProfileString("TerminalSet", "DataInputPage", "1", CONFIG_FILE);
	//ExecPro(".\\softKeybdApp.exe", true, NULL);
    StartKeybd();
	
	XWnd_RedrawWnd(g_hWindow);
	return false;
}

BOOL CALLBACK OnVisitorLogOut(HWINDOW hWindow, WPARAM wParam, LPARAM lParam)
{
		SuspendThread(g_ThreadReadBarcode);
	char s[10000] = {'\0'};
	//strcpy(s, (char *)lParam);
	//::GetPrivateProfileString("Bar", "BarNumber", "",s,sizeof(s), CONFIG_FILE);
	//char unBarNumber[10000] = {'\0'};
	//strcpy(unBarNumber,(char *)lParam);
	char json[1024] = {'\0'};
	strcpy(s,g_szComBarcode);
	//AfxMessageBox(s);
	//return false;
	
	//::GetPrivateProfileString("Bar", "BarNumber", "",unBarNumber,sizeof(unBarNumber), CONFIG_FILE);
	if (GetAESJson(s,json) == 0)
	{
		CString cstr;
		VISITORINFO visitor;
		memset((char *)&visitor, 0, sizeof(VISITORINFO));
		OFFICIALINFO official;
		memset((char *)&official, 0, sizeof(OFFICIALINFO));
		getYueJSon(json,visitor);

        cstr.Format(visitor.szYueTime);
		//cstr.Replace("T"," ");
		cstr = cstr.Left(cstr.GetLength() - 14);
        CTime time = CTime::GetCurrentTime();
		CString m_strTime = time.Format("%Y-%m-%d");
		//CString m_strTime = time.Format("%Y-%m-%d %H:%M:%S");
		//m_strTime = m_strTime.Left(m_strTime.GetLength() - 9)
		//CTimeSpan span=m_strTime-g_szQRvisitime;
		if (cstr != m_strTime) //�˴������޸ģ���Ҫ�Ļ���Ӧ��Ϊ�� cstr != m_strTime
		{
			XMessageBox(g_hWindow, L"�������ں�ԤԼ���ڲ���ͬһ��!", L"����");
			ResumeThread(g_ThreadReadBarcode);
			return false;
		}
		else
		{
			InitVisitorData();

			
			SetVisitorInfo(visitor);
			strcpy(official.szName,visitor.szToOfficialName);
			SetOfficialInfo(official);
			strcpy(official.szMobile,visitor.szToOfficialPhone);
			SetOfficialInfo(official);
			XWnd_RedrawWnd(g_hWindow);
			ResumeThread(g_ThreadReadBarcode);
		
		 //���ö�ά������Ϊtrue
			g_bQRcode = true;
			return true;
		}
		
	}
	else
	{
		VisitorLogOut(s);

		if (::GetPrivateProfileInt("TerminalSet", "bLeaveOfficial",0, CONFIG_FILE)==1)
		{
			//�������������
			InitVisitorData();
		}
		else
		{
			//�����������
			InitData();
		}

		ShowWindow(g_hCapWnd, true);
		RePlayCapture();//RePlay();

	    XWnd_RedrawWnd(g_hWindow);
	    ResumeThread(g_ThreadReadBarcode);
	    return false;
	}
	/*
	SuspendThread(g_ThreadReadBarcode);
	char s[128] = {'\0'};
	//strcpy(s, (char *)lParam);
	//::GetPrivateProfileString("Bar", "BarNumber", "",s,sizeof(s), CONFIG_FILE);
	VisitorLogOut(s);

	if (::GetPrivateProfileInt("TerminalSet", "bLeaveOfficial",0, CONFIG_FILE)==1)
	{
		//�������������
		InitVisitorData();
	}
	else
	{
		//�����������
		InitData();
	}

	ShowWindow(g_hCapWnd, true);
	RePlayCapture();//RePlay();

	XWnd_RedrawWnd(g_hWindow);
	ResumeThread(g_ThreadReadBarcode);
	*/
	return false;
}

//ѡ���˱����������������Ϣ��Ӧ�ӿ�
BOOL CALLBACK OnOfficialNameSelected(HWINDOW hWindow, WPARAM wParam, LPARAM lParam)
{
	wchar_t *wBuf = (wchar_t *)lParam;		//ѡ��İ�ť����
	XEdit_SetText(XWnd_GetEle(g_hWindow, IDE_MASTERNAME), wBuf);
	HELE hE = XWnd_GetEle(g_hWindow, IDB_QUERYOFFICIAL);
	QueryEmployer(hE);
	XWnd_RedrawWnd(g_hWindow);
	return false;
}

//�һ���������ͷ
BOOL CALLBACK OnCapture(HWINDOW hWindow, WPARAM wParam, LPARAM lParam)
{
	return false;
	if (XEle_IsShowEle(g_hPage1))
	{
		if (g_nCaptureTick < 1 )
		{
			
			VideoPreview2();
			XEle_EnableEle(g_hTab1, true);
		}
		g_nCaptureTick = 30;
	}
	else
	{
		/*
		CloseCapture();
		RECT rc;
		rc.bottom = 40 + 40;
		rc.right = 590 + 320;
		rc.left = 590;
		rc.top = 40;
		//OpenCapture((long)XWnd_GetHWnd(g_hWindow),rc);



		OpenCapture((long)XWnd_GetHWnd(g_hWindow), rc);
		//g_nCaptureTick = 0;
		//Pause();
		//g_bVideoOpened = false;*/
	}

	return false;
}

BOOL CALLBACK OnReadICAOver(HWINDOW hWindow, WPARAM wParam, LPARAM lParam)
{
	//XMessageBox(g_hWindow, L"��������!", L"����");
	//return false;
	::GetPrivateProfileString("IC", "IDSerial", "",g_stIDCard.szICSerial, sizeof(g_stIDCard.szICSerial), CONFIG_FILE);
	if(wParam == IC_REG)
	{
		//ƥ�䱻����ע����Ϣ
		char szOfficialName[32] = {'\0'};
		wchar_t wBuf[256 + 1] = {0};
		
		char szT[512] = {'\0'};
		long FldCount = 0, ret = 0;
		CString outParas[FLD_MAX];
		CWinAdo tAdo(sCONNSTR_MYSQL);
		
		ret = tAdo.OpenAdo();
		if (ret != ADOOK)
		{
			return false;
		}
		
		///char szData[32] = {'\0'};
		//int n = 0,m = 0,*nSel;

		//memset((char *)wBuf, 0, sizeof(wBuf));
	//	

		//m = XList_GetSelectCount(hList);
		//if (m < 0)
		{
			//return false;
		}    
		//nSel = new int[m];
		//XList_GetSelectAllItem(hList,nSel,m);

		//int n=XList_GetSelectItem(hList);
	/*	if (strlen(szData) <= 0)
		{
			XMessageBox(g_hWindow, L"ɾ������Ա��������Ϊ��!", L"����");
			return false;
		}
		else*/


		//for (int i=0;i<m;i++)
		//{
		//wcscpy(wBuf, XList_GetItemText(hList, n, 0));		//���
		//W_2_S(wBuf, szData, 256);
		sprintf(szT, "select * from t_official where official_id = \'%s\'", g_szOfficialID);
		ret = tAdo.execSelect(szT, &FldCount, outParas);
		if (ret == ADOERR || (FldCount == 0 && outParas[0].IsEmpty()))
		{
			return false;
		}

		ret = XMessageBox(g_hWindow, L"����ԱIC����Ϣ,��[ȷ��]����,��[ȡ��]����", L"��ʾ��Ϣ", XMB_OK | XMB_CANCEL);
		if (ret == XMB_CANCEL)
		{
			//delete []nSel;
			return false;
		}

		//��ʼ��
		{
			
			CWinAdo tAdo1(sCONNSTR_MYSQL);
			ret = tAdo1.OpenAdo();
			if (ret != ADOOK)
			{
				return false;
			}
			sprintf(szT, "update t_official set official_NO = \'%s\' where official_id = \'%s\'", \
			g_stIDCard.szICSerial,g_szOfficialID);
			ret = tAdo1.execUpdateInsert(szT);
			if (ret != ADOOK)
			{
				return false;
			}
		}

		//}
		//delete []nSel;
		//if (ret == ADOOK)
		HELE hList = XWnd_GetEle(g_hWindow, IDL_LISTOFFICIAL);
		{
			HELE hE_txt = XWnd_GetEle(g_hWindow, IDE_ADD_OFFICIALNAME);
			XEdit_SetText(hE_txt,L"");
			hE_txt = XWnd_GetEle(g_hWindow, IDC_ADD_OFFICIALSEX);
			XEdit_SetText(hE_txt,L"");
			hE_txt = XWnd_GetEle(g_hWindow, IDE_ADD_OFFICEPHONE);
			XEdit_SetText(hE_txt,L"");
			hE_txt = XWnd_GetEle(g_hWindow, IDE_ADD_OFFICENAME);
			XEdit_SetText(hE_txt,L"");
			hE_txt = XWnd_GetEle(g_hWindow, IDE_ADD_OFFICIALMOBILE);
			XEdit_SetText(hE_txt,L"");
			hE_txt = XWnd_GetEle(g_hWindow, IDE_ADD_OFFICEADDR);
			XEdit_SetText(hE_txt,L"");
			hE_txt = XWnd_GetEle(g_hWindow, IDC_ADD_OFFICIALZHIWU);
			XEdit_SetText(hE_txt,L"");
			hE_txt = XWnd_GetEle(g_hWindow, IDE_ADD_ROOMNUM);
			XEdit_SetText(hE_txt,L"");
			hE_txt = XWnd_GetEle(g_hWindow, IDE_ADD_IP);
			XEdit_SetText(hE_txt,L"");

			OnQueryEmployerForList(XWnd_GetEle(g_hWindow, IDB_QUERYOFFICIAL), XWnd_GetEle(g_hWindow, IDB_QUERYOFFICIAL));
			XList_SetSelectItem(hList, 0);
		}

		XWnd_RedrawWnd(g_hWindow);
		return false;
	}
	else
	{
		wchar_t wBuf[256] = {0};
		char szT[512] = {'\0'};
		long FldCount = 0, ret = 0;
		CString outParas[FLD_MAX];
		
		CWinAdo tAdo(sCONNSTR_MYSQL);
		
		ret = tAdo.OpenAdo();
		if (ret != ADOOK)
		{
			return false;
		}
		for (int k = 0; k < FLD_MAX; k++)
		{
			outParas[k].Empty();
		}
		
		sprintf(szT, "select * from t_official where official_NO = \'%s\'", g_stIDCard.szICSerial);
		ret = tAdo.execSelect(szT, &FldCount, outParas);
		if (ret == ADOERR || (FldCount == 0 && outParas[0].IsEmpty()))
		{
			return false;
		}
		OFFICIALINFO official;
		memset((char *)&official, 0, sizeof(OFFICIALINFO));
		strcpy(official.szName, outParas[0]);
		strcpy(official.szSex, outParas[1]);
		strcpy(official.szOfficePhone, outParas[2]);
		strcpy(official.szMobile, outParas[3]);
		strcpy(official.szOfficeName, outParas[4]);
		strcpy(official.szOfficeAddr, outParas[5]);
		strcpy(official.szOfficialDuty, outParas[6]);
		strcpy(official.szRoom, outParas[7]);
		strcpy(official.szIP, outParas[8]);
		strcpy(official.szNO, outParas[10]);
		strcpy(official.szSSO, outParas[13]);

		strcpy(g_Printvisitor.szToOfficeName, official.szName);
		strcpy(g_Printvisitor.szToOfficialName, official.szOfficeName);
		strcpy(g_Printvisitor.szToRoom, official.szRoom);
		strcpy(g_Printvisitor.szToSSO, official.szSSO);
		
		strcpy(g_szOfficialIP, official.szIP);
		SetOfficialInfo(official);

		if (::GetPrivateProfileInt("TerminalSet", "bSaveAfterScan",0, CONFIG_FILE)==1)
			SaveAndPrint();
	}


	return false;
}

BOOL CALLBACK OnCompareFace(HWINDOW hWindow, WPARAM wParam, LPARAM lParam)
{
	char szT[30] = {'\0'};
	CSendTcpSocket socket;
	
	if(socket.Connect(Compare_Face_Port)!=0)
	{
		//ƥ�����û������
		return false;
	}

	if(socket.SendText("1")<0)
	{
		//����ʧ��
		return false;
	}

	if(socket.Recv(szT,30)<0)
	{
		//����ʧ��
		return false;
	}

	return false;
}

//����֤�Ķ���ɵ���Ϣ��Ӧ
BOOL CALLBACK OnReadID2Over(HWINDOW hWindow, WPARAM wParam, LPARAM lParam)
{
	//char id2file[20]={'\0'};
	if((int)wParam==6)
	{
		//Messagebox2(L"Read card failed!");
		//XWnd_RedrawWnd(g_hWindow);
		//MessageBox(XWnd_GetHWnd(hWindow),"Read card failed!","Salah",MB_ICONEXCLAMATION);
		char szTime[40] = {'\0'};
		int v =::GetPrivateProfileInt("Info", "Value",80, ".\\FaceCompare\\faceCompare.ini");
		//time((time_t *)&lefttime);
		g_bStopCompare = true;
		if((int)lParam > v)
		{
			g_bFaceCompare = true;
			wchar_t wBuf[256] = {0};
			S_2_W(wBuf, USER_HEAD_PIC, 256);
			HIMAGE hImg = XImage_LoadFile(wBuf, true);
			HELE hE = XWnd_GetEle(g_hWindow, IDP_HEADPIC2);
			XPic_SetImage(hE, hImg);

			sprintf(szTime, "�ȶ�ͨ�������ƶȣ�%d��", (int)lParam);
			MessageBox(XWnd_GetHWnd(hWindow),szTime,"��ʾ",MB_ICONEXCLAMATION);
			
		}
		else
		{
			g_bFaceCompare = false;
			sprintf(szTime, "�ȶ�ʧ�ܣ����ƶȣ�%d�֡��Ƿ����±ȶԣ�", (int)lParam);
			int result = MessageBox(XWnd_GetHWnd(hWindow),szTime,"��ʾ",MB_OKCANCEL|MB_ICONEXCLAMATION);
			if(result == IDOK)
			{
				HELE hE = XWnd_GetEle(g_hWindow, IDP_CAMERA);
				XPic_SetImage(hE, NULL);
				::DeleteFile(USER_VIDEO_PIC);

				hE = XWnd_GetEle(g_hWindow, IDP_HEADPIC2);		
				XPic_SetImage(hE, NULL);
				::DeleteFile(USER_HEAD_PIC);
				XBtn_SetText(g_hTab1, L"���ձȶ�");
				ShowWindow(g_hCapWnd, true);
				RePlayCapture();//RePlay();
				::DeleteFile(USER_VIDEO_PIC);
				::DeleteFile(USER_HEAD_PIC); 
				
				//::WritePrivateProfileString("Info", "bStopRead", "1", ".\\FaceCompare\\faceCompare.ini");
				g_bStopCompare = false;
			}
		}

		
		//if((int)lParam < 80)
			
		return false;
	}

	VISITORINFO visitor;
	memset((char *)&visitor, 0, sizeof(VISITORINFO));
    HELE hEle = NULL;
	HIMAGE hImg = NULL;
	wchar_t wBuf[256] = {0};
	hEle = XWnd_GetEle(g_hWindow, IDP_PASSPORTPIC);	
	XPic_SetImage(hEle, NULL);
	hEle = XWnd_GetEle(g_hWindow, IDP_HEADPIC);		
	XPic_SetImage(hEle, NULL);
	hEle = XWnd_GetEle(g_hWindow, IDP_ID2PIC);		
	XPic_SetImage(hEle, NULL);


	/*strcpy(id2file,".\\id2.ini");
	::CopyFile("photo.bmp", USER_ID2_TMP, false);
    ::GetPrivateProfileString("VisitorInfo","Name","",g_stIDCard.name,sizeof(g_stIDCard.name),id2file);
	::GetPrivateProfileString("VisitorInfo","Sex","",g_stIDCard.sex,sizeof(g_stIDCard.sex),id2file);
	::GetPrivateProfileString("VisitorInfo","People","",g_stIDCard.people,sizeof(g_stIDCard.people),id2file);
	::GetPrivateProfileString("VisitorInfo","Birthday","",g_stIDCard.birthday,sizeof(g_stIDCard.birthday),id2file);
	::GetPrivateProfileString("VisitorInfo","Address","",g_stIDCard.address,sizeof(g_stIDCard.address),id2file);
	::GetPrivateProfileString("VisitorInfo","Number","",g_stIDCard.number,sizeof(g_stIDCard.number),id2file);
	::GetPrivateProfileString("VisitorInfo","SignDep","",g_stIDCard.szSignDep,sizeof(g_stIDCard.szSignDep),id2file);
	::GetPrivateProfileString("VisitorInfo","Validterm","",g_stIDCard.validterm,sizeof(g_stIDCard.validterm),id2file);*/

	::DeleteFile(USER_ID2_PIC);            
	::DeleteFile(USER_ID_PIC); 
	::DeleteFile(USER_BARCODE_PIC);

	strcpy(visitor.szName, g_stIDCard.name);
	strcpy(visitor.szID, g_stIDCard.number);
	BOOL b = CheckVisitor(visitor);
	if (b)
	{
		wchar_t wBuf[256] = {0};
		char szT[512] = {'\0'},szOfficialName[100] = {'\0'};
		long FldCount = 0, ret = 0,lefttime;
		CString outParas[FLD_MAX];
		CWinAdo tAdo(sCONNSTR_MYSQL);

		ret = tAdo.OpenAdo();
		if (ret != ADOOK)
		{
			return false;
		}
		
		for (int i = 0; i < FLD_MAX; i++)
		{
			outParas[i].Empty();
		}

		char szTime[40] = {'\0'};
		time((time_t *)&lefttime);
		sprintf(szTime, "%ld", lefttime);
		CallLeft2Server(visitor.szName,visitor.szID,"",szTime,""); //Զ��ǩ�����
	
		sprintf(szT, "select * from t_visitor where visitor_name = \'%s\' and visitor_id = \'%s\' and visitor_status>1 and visitor_lefttime <= 0",\
			visitor.szName, visitor.szID);
		ret = tAdo.execSelect(szT, &FldCount, outParas);
		if (ret == ADOERR || (FldCount == 0 && outParas[0].IsEmpty()))
		{
			return false;               //�÷ÿ�δ�Ǽ�
		}
		
		sprintf(szOfficialName, "[%s]  \r\nͨ������֤ǩ��",  visitor.szName);	
		S_2_W(wBuf, szOfficialName, 256);
		/*ret = XMessageBox(g_hWindow, wBuf, L"��ʾ", XMB_OK | XMB_CANCEL);
		if (ret == XMB_CANCEL)
		{
			return false;
		}*/
		//g_hLogoutWindow = XModalWnd_CreateWindow(200, 150, L"��ʾ", XWnd_GetHWnd(g_hWindow));
				   // g_hMsgWindow = XModalWnd_CreateWindow(200, 150, L"��ʾ", XWnd_GetHWnd(g_hWindow));
		g_hLogoutWindow = XWnd_CreateWindow(200, 150,200, 150,L"��ʾ",XWnd_GetHWnd(g_hWindow),XC_SY_CAPTION | XC_SY_BORDER | 	XC_SY_ROUND | 	XC_SY_CENTER | XC_SY_DRAG_BORDER | XC_SY_DRAW_CAPTION_ICON | XC_SY_DRAW_CAPTION_TITLE);
				
		::SetWindowPos(XWnd_GetHWnd(g_hLogoutWindow), XWnd_GetHWnd(g_hWindow), 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);

		XStatic_Create(15,40,180,40,wBuf,g_hLogoutWindow);
		XWnd_SetBorderSize(g_hLogoutWindow, 3, 3, 3, 3);
		SetTimer(XWnd_GetHWnd(g_hWindow),1,2000,TimerMsgBox);
		//XModalWnd_DoModal(g_hLogoutWindow);
		XWnd_ShowWindow(g_hLogoutWindow,SW_SHOW);

		//time((time_t *)&ret);
		sprintf(szT, "update t_visitor set visitor_lefttime = %ld where visitor_name = \'%s\' and visitor_id = \'%s\' and visitor_visitetime = %ld", lefttime, visitor.szName, visitor.szID, atol(outParas[16]));
		ret = tAdo.execUpdateInsert(szT);
		
		//ǩ��ɹ���ˢ�½���
		if (::GetPrivateProfileInt("TerminalSet", "bLeaveOfficial",0, CONFIG_FILE)==1)
		{
			//�������������
			InitVisitorData();
		}
		else
		{
			//�����������
			InitData();
		}
	    ShowWindow(g_hCapWnd, true);
		RePlayCapture();//RePlay();
		ListTodayVistorsInfo();		//ˢ����������Ϣ���
		VisitorTotalInfo(szT);
		//ResumeThread(g_HWDThreadID2ID);
		XWnd_RedrawWnd(g_hWindow);
		return true;
	}
    g_bShowCap = true;
	HELE hE = NULL;
	//hE = XWnd_GetEle(g_hWindow, IDC_VISITORIDTYPE);
	//int i = XComboBox_GetSelectItem(hE);
	//if (i==0)
	{
		hE = XWnd_GetEle(g_hWindow, IDP_ID2PIC);
		hImg = XImage_LoadFile(L".\\skin\\image\\id2.bmp", true);
		XPic_SetImage(hE, hImg);

		hE = XWnd_GetEle(g_hWindow, IDP_ID2PIC);
		XEle_ShowEle(hE, true);

		hE = XWnd_GetEle(g_hWindow, IDG_ID2);	//����֤ͼ���
		XEle_ShowEle(hE, true);

		hE = XWnd_GetEle(g_hWindow, IDG_PASSPORT);	//�������
		XEle_ShowEle(hE, false);
	}
	
    ShowWindow(g_hCapWnd, true);
	RePlayCapture();//RePlay();
	
	char szT[128] = {'\0'},szF[128] = {'\0'};
	g_nID2ReadType = ID2_READ;
	hE = XWnd_GetEle(g_hWindow, IDP_HEADPIC);		//����֤��ͼƬ�ŵ�ͷ����Ƭ��
	XPic_SetImage(hE, NULL);

	
	strcpy(visitor.szID, g_stIDCard.number);
	strcpy(visitor.szName, g_stIDCard.name);
	//strcat(visitor.szName,"����");
	strcpy(visitor.szSex, g_stIDCard.sex);
	strcpy(visitor.szFolk, g_stIDCard.people);
	strcpy(visitor.szAddr, g_stIDCard.address);
	strcpy(visitor.szSignDepartment, g_stIDCard.szSignDep);
	strcpy(visitor.szBirth, g_stIDCard.birthday);
	strcpy(visitor.szValidDate, g_stIDCard.validterm);
    
	long n;
	time((time_t *)&n);
	visitor.lVisiteTime = n;			//����ʱ��
	GetDayFolder(szF);
	sprintf(szT, "%s\\%s_%s_%ld_id.bmp", szF, visitor.szName, visitor.szID, n);
	::CopyFile(USER_ID2_TMP, szT, false);	//���û�ɨ���֤����Ƭ�������
	strcpy(g_stIDCard.szPicPath, szT);			//������֤����Ƭ���·��
	strcpy(visitor.szUserIDPic, g_stIDCard.szPicPath);
	strcpy(visitor.szIDType,"1");  //����֤
    //::DeleteFile(USER_ID2_TMP);
	if(g_bQRcode)
	{
		wchar_t wBuf2[256] = {0};
		char szPhone[512] = {'\0'};
		HELE hE2 = GETELE(IDE_VISITORMOBILE);
		XEdit_GetText(hE2, wBuf2, 32);
		if (wcslen(wBuf2) > 0)
        {
			W_2_S(wBuf2,szPhone,32);
			strcpy(visitor.szPhone, szPhone);
			memset((char *)wBuf2, 0, sizeof(wBuf2));
		}		

		char szReason[512] = {'\0'};
		hE2 = GETELE(IDE_VISITORREASON);
		XEdit_GetText(hE2, wBuf2, 32);
		if (wcslen(wBuf2) > 0)
		{
			W_2_S(wBuf2,szReason,32);
			strcpy(visitor.szReason, szReason);
			memset((char *)wBuf2, 0, sizeof(wBuf2));
		}
			

		char szCarNumber[10] = {'\0'};
		hE2 = GETELE(IDE_VISITORCARNUM);
		XEdit_GetText(hE2, wBuf2, 32);
		if (wcslen(wBuf2) > 0)
		{	
			W_2_S(wBuf2,szCarNumber,32);
			strcpy(visitor.szCarNum, szCarNumber);
			memset((char *)wBuf2, 0, sizeof(wBuf2));
		}
		
	}
	
	ShowID2(true, visitor);
	ShowMainMenu(g_hPage2, true);
	//memset(szT, 0, sizeof(szT));
	//VisitorTotalTimes(g_stIDCard.name, szT);
	//::WritePrivateProfileString("TerminalSet", "DataInputPage", "2", CONFIG_FILE);
	StartKeybd();
	XWnd_RedrawWnd(g_hWindow);
	SetVisitorInfo(visitor);
	if (::GetPrivateProfileInt("TerminalSet", "bSaveOfficial",0, CONFIG_FILE)==1)  //�����±�����
	   SetOfficialReadOnly(false);
	else
	   SetOfficialReadOnly(true);

	//��֤�ȶ�
	::DeleteFile(USER_VIDEO_PIC);
	XBtn_SetText(g_hTab1, L"���ձȶ�");
	//::WritePrivateProfileString("Info", "bStopRead", "1", ".\\FaceCompare\\faceCompare.ini");
	g_bStopCompare = false;

    if (::GetPrivateProfileInt("TerminalSet", "bMJConnetion",0, CONFIG_FILE)==1) 
	{
		if (g_ThreadID2IC)
		{
			SuspendThread(g_ThreadID2IC);
			CloseHandle(g_ThreadID2IC);
			g_ThreadID2IC = NULL;
		}
		if (g_hCom)
		{
			CloseHandle(g_hCom);
			g_hCom = NULL;
		}
		g_ThreadID2IC = CreateThread(NULL, 0, ReadICThread, NULL, 0, NULL);
	}

	if (::GetPrivateProfileInt("TerminalSet", "bSaveAfterScan",0, CONFIG_FILE)==1)
	    SaveAndPrint();

	return true;
}

////////////////////////////////////////////////////////////////
void TrackRect()
{
	if (!g_bRectStart)
	{
		return;
	}
	HDC hDC = ::GetDC(GETHWND);
	if (hDC == NULL)
	{
		g_bRectStart = false;
		return;
	}

	POINT point;
	::GetCursorPos(&point);
	::ScreenToClient(GETHWND, &point);
	g_ptEnd = point;

	TrackDraw(hDC, g_ptEnd);

	::ReleaseDC(GETHWND, hDC);
	return;
}

BOOL CALLBACK OnCameraMouseMove(HELE hEle, UINT flags, POINT *pPt)
{
	if (!g_bStartClipScreen)
	{
		return false;
	}
	TrackRect();
	return false;
}

BOOL CALLBACK OnCameraLButtonDown(HELE hEle, UINT flags, POINT *pPt)
{
	if (!g_bStartClipScreen)
	{
		return false;
	}

	if (g_bRectStart)  //�Ѿ��ڻ�ͼ��
	{
		return false;
	}
	else
	{
		g_bRectStart = true;  //��ʼ��ͼ
	}

	HELE hE = XWnd_GetEle(g_hWindow, IDP_HEADPIC2);
	XPic_SetImage(hE, NULL);
	//::DeleteFile(USER_HEAD_PIC);
	::GetCursorPos(&g_ptStart);
	::ScreenToClient(GETHWND, &g_ptStart);

	HDC hDC = ::GetDC(GETHWND);
	if (hDC == NULL)
	{
		g_bRectStart = false;
		return false;
	}

	StartClip(hDC, g_ptStart);
	::ReleaseDC(GETHWND, hDC);

	return false;
}

BOOL CALLBACK OnCameraLButtonUp(HELE hEle, UINT flags, POINT *pPt)
{
	if (!g_bStartClipScreen)
	{
		return false;
	}
	if (!g_bRectStart)
	{
		return false;
	}


	//RECT rect;
	::GetCursorPos(&g_ptEnd);
	::ScreenToClient(GETHWND, &g_ptEnd);

	g_bRectStart = false;

	HDC hDC = ::GetDC(GETHWND);
	if (hDC == NULL)
	{
		return false;
	}

	HBITMAP hBitmap = StopClip(hDC, g_ptEnd);
	if (hBitmap == NULL)
	{
		return false;
	}
	HELE hP = XWnd_GetEle(g_hWindow, IDP_HEADPIC2);
	XPic_SetImage(hP, NULL);
	::DeleteFile(USER_HEAD_PIC);

	BOOL bRet = SaveBitmapToFile(hBitmap, USER_HEAD_PIC);
	EndClip();
	::ReleaseDC(GETHWND, hDC);

	wchar_t wBuf[256] = {0};
	S_2_W(wBuf, USER_HEAD_PIC, 256);
	HIMAGE hImg = XImage_LoadFile(wBuf, true);
	HELE hE = XWnd_GetEle(g_hWindow, IDP_HEADPIC2);
	XPic_SetImage(hE, hImg);

	XWnd_RedrawWnd(XEle_GetHWindow(hEle));
	return false;
}
////////////////////////////////////////////////////////////////

//���ɨ��֤��ҳ���ϵġ����ء���ť���¼���Ӧ
BOOL CALLBACK OnReturnMain(HELE hEle, HELE hEventEle)
{
	if (hEle != hEventEle) 
	{
		return false;
	}

	XEle_ShowEle(g_hTab21, false);
	XEle_ShowEle(g_hTab22, false);
	XEle_ShowEle(g_hTab23, false);
	XEle_ShowEle(g_hTab24, false);
	XEle_ShowEle(g_hTab25, false);
	XEle_ShowEle(g_hTab26, false);
	XEle_ShowEle(g_hTab27, false);
	XEle_ShowEle(g_hTab28, false);
	XEle_ShowEle(g_hPage2, false);
	XEle_ShowEle(g_hPage1, true);			//��ʾ������

	if(g_bShowCap)
	{
		ShowWindow(g_hCapWnd, true);
		RePlayCapture();//RePlay();
	}




	XEle_ShowEle(g_hTab1, true);
	XEle_ShowEle(g_hTab2, true);
	XEle_ShowEle(g_hTab3, true);
//	XEle_ShowEle(g_hTab4, true);
	XEle_ShowEle(g_hTab5, true);
	//XEle_ShowEle(g_hTab6, bShow);
	XEle_ShowEle(g_hTab8, true);
	XEle_ShowEle(g_hTab9, true);
	XEle_ShowEle(g_hTab10, true);

	//if (bShow)
	{
		//SetInfo(IDS_DEVINFO, L"");

		//XEle_ShowEle(g_hPage1, true);
	//XBtn_SetCheck(g_hTab1, true);
	}
	//EnableButton(IDB_TAB1);					//����������󼤻���ʾ��һ����ť
	XWnd_RedrawWnd(XEle_GetHWindow(hEle));
	return false;
}

//�����ҳ���ϡ�֤��ɨ�衱��ť���¼���Ӧ
BOOL CALLBACK OnScanVisitorCertification(HELE hEle, HELE hEventEle)
{
	if (hEle != hEventEle) 
	{
		return false;
	}
	g_bFaceCompare = true;
	HideSoftKeybd();
	XEle_ShowEle(g_hPage1, true);
	XEle_ShowEle(g_hTab21, true);
	XEle_ShowEle(g_hTab22, true);
	XEle_ShowEle(g_hTab23, true);
	XEle_ShowEle(g_hTab24, true);
	XEle_ShowEle(g_hTab25, true);
	XEle_ShowEle(g_hTab26, true);
	XEle_ShowEle(g_hTab27, true);
	XEle_ShowEle(g_hTab28, true);
	XEle_ShowEle(g_hPage2, true);			//��ʾ֤���Ǽǽ���
	//XBtn_SetCheck(g_hTab21, true);
	ShowMainMenu(NULL, false);
	if(1 ==::GetPrivateProfileInt("TerminalSet", "ZT",0, CONFIG_FILE))
	{
		if(::FindWindow(NULL, "ChinaSafe-cc") == NULL)
			g_bScanerISOK = false;
	}
	else if(2 ==::GetPrivateProfileInt("TerminalSet", "ZT",0, CONFIG_FILE))		
	{
		if(::FindWindow(NULL, "OCR-cc") == NULL)
			g_bScanerISOK = false;
	}

	if (!g_bScanerISOK)						//ɨ����δ���ӣ���ɨ���������ذ�ť��Ч
	{

		XEle_EnableEle(g_hTab21, false);
		XEle_EnableEle(g_hTab22, false);
		XEle_EnableEle(g_hTab23, false);
		XEle_EnableEle(g_hTab24, false);
		XEle_EnableEle(g_hTab25, false);
		XEle_EnableEle(g_hTab26, false);
		XEle_EnableEle(g_hTab27, false);
        XBtn_SetCheck(g_hTab28, true);
	}

	RECT rect;
	HELE hE = XWnd_GetEle(g_hWindow, IDB_TAB1);
	XEle_GetRect(hE, &rect);
	rect.left = 942;
	rect.bottom = 740;
	XEle_SetRect(g_hPage2, &rect, true);

	if (::GetPrivateProfileInt("TerminalSet", "bLeaveOfficial",0, CONFIG_FILE)==1)
	{
		//�������������
		//if (::GetPrivateProfileInt("TerminalSet", "bSaveAfterScan",0, CONFIG_FILE)!=1)
			InitVisitorData();
	}
	else
	{
		//�����������
		InitData();
	}
	g_bStopID2 = false;

	/*if (g_bScanerISOK)
	{
		SetInfo(IDS_DEVINFO, L"���ȡ����֤��ɨ��֤��");
	}
	else
	{
		SetInfo(IDS_DEVINFO, L"�뽫����֤�������Ķ�����");
	}*/

	::DeleteFile(USER_VIDEO_PIC);
	::DeleteFile(USER_ID_PIC);
	::DeleteFile(USER_ID2_PIC);
	::DeleteFile(USER_HEAD_PIC);
	::DeleteFile(USER_ID2_TMP);

	XWnd_RedrawWnd(XEle_GetHWindow(hEle));

	return false;
}

//����������ϵġ�����/��ӡ����ť���¼���Ӧ
BOOL CALLBACK OnSavePrint2(HELE hEle, HELE hEventEle)
{
	if (hEle != hEventEle)
	{
		return false;
	}
	

	HWINDOW hWindow=XModalWnd_CreateWindow(410, 160, L"��ѡ���ӡ����", XWnd_GetHWnd(g_hWindow));
	if(hWindow)
	{
		HELE hButton=XBtn_Create(20,45,74,82,L"��ʱ",hWindow);
		HELE hBtnCancel=XBtn_Create(120,45,74,82,L"Ѳ��",hWindow);//������ť
		HELE hBtnCancel2=XBtn_Create(220,45,74,82,L"��ͨ",hWindow);//������ť
		HELE hButtonOK=XBtn_Create(320,45,74,82,L"����",hWindow);//������ť
		
		//XEle_RegisterEvent(hButton,XE_BNCLICK,OnLogOutBtnClick);//ע�ᰴť����¼�
		XEle_RegisterEvent(hButton,XE_BNCLICK,OnLSBtnClick);
		XEle_RegisterEvent(hBtnCancel,XE_BNCLICK,OnXCBtnClick);
		XEle_RegisterEvent(hBtnCancel2,XE_BNCLICK,OnPTBtnClick);
		XEle_RegisterEvent(hButtonOK,XE_BNCLICK,OnQTBtnClick);
		//XWnd_ShowWindow(hWindow,SW_SHOW);//��ʾ����
		HIMAGE hImageStay=XImage_LoadFileRect(L".\\skin\\image\\toolbar_hover.png",0,0,74,82);
		HIMAGE hImageStay1=XImage_LoadFileRect(L".\\skin\\image\\toolbar_pushed.png",0,0,74,82);
		//XEle_RegisterEvent(XWnd_GetEle(hWindow, ID_Reg_Btn), XE_BNCLICK,OnExitApp);
		XBtn_SetImageLeave(hButtonOK,hImageStay);
		XBtn_SetImageStay(hButtonOK,hImageStay1);
		XBtn_SetImageDown(hButtonOK,hImageStay1);
		XEle_SetBkTransparent(hButtonOK,true);
		
		XBtn_SetImageLeave(hBtnCancel,hImageStay);
		XBtn_SetImageStay(hBtnCancel,hImageStay1);
		XBtn_SetImageDown(hBtnCancel,hImageStay1);
		XEle_SetBkTransparent(hBtnCancel,true);
		
		XBtn_SetImageLeave(hBtnCancel2,hImageStay);
		XBtn_SetImageStay(hBtnCancel2,hImageStay1);
		XBtn_SetImageDown(hBtnCancel2,hImageStay1);
		XEle_SetBkTransparent(hBtnCancel2,true);
		
		XBtn_SetImageLeave(hButton,hImageStay);
		XBtn_SetImageStay(hButton,hImageStay1);
		XBtn_SetImageDown(hButton,hImageStay1);
		XEle_SetBkTransparent(hButton,true);
	}
	SetMainDlgEditBorder();
	XWnd_SetBorderSize(hWindow, 0, 0, 0, 0);
	HIMAGE hImg = XImage_LoadFile(L".\\skin\\image\\frame.jpg", true);
	XWnd_SetImageNC(hWindow, hImg);
	XModalWnd_DoModal(hWindow);

	return false;
}

//����������ϵġ�����/��ӡ����ť���¼���Ӧ
BOOL CALLBACK OnSavePrint(HELE hEle, HELE hEventEle)
{
	if (hEle != hEventEle) 
	{
		return false;
	}

	
	wchar_t wBuf[256] = {0};
	HELE hE;
    VISITORINFO visitor;
	memset((char *)&visitor, 0, sizeof(VISITORINFO));

	memset((char *)wBuf, 0, sizeof(wBuf));
	hE = XWnd_GetEle(g_hWindow, IDE_VISITORNAME);
	XEdit_GetText(hE, wBuf, 128);
	W_2_S(wBuf, visitor.szName, 128);

	memset((char *)wBuf, 0, sizeof(wBuf));
	hE = XWnd_GetEle(g_hWindow, IDE_VISITORIDNUM);
	XEdit_GetText(hE, wBuf, 128);
	W_2_S(wBuf, visitor.szID, 128);

	memset((char *)wBuf, 0, sizeof(wBuf));
	hE = XWnd_GetEle(g_hWindow, IDE_MASTERNAME);
	XEdit_GetText(hE, wBuf, 128);
	W_2_S(wBuf, visitor.szToOfficialName, 128);

	int b = CheckVisitor(visitor);
	if (!b)  //������
	{
		BackScreenPlayADV();//��ӡ��͵�ʱ���ָ��������
		//if (g_bAcceptVisite)
		{
			SaveAndPrint();
		}
		/*else
		{
			XMessageBox(g_hWindow, L"�������˵ķ�������û�б�����!", L"����");
			return false;
		}*/

	}
	else if (b==-1)  //�����˸ı���δǩ��
	{
		return false;
	}
	else             //�����͵�
	{
			if (strlen(g_Printvisitor.szName)==0)
			{
                 XMessageBox(g_hWindow, L"�޴�ӡ��Ϣ!", L"����");
		         return false;
			}
		    PrintVisitorNoteAgain();

	}

	XWnd_RedrawWnd(XEle_GetHWindow(hEle));
	HideSoftKeybd();
	return true;
}

BOOL CALLBACK OnSavePrintBySoftKeyBD2(HWINDOW hWindow, WPARAM wParam, LPARAM lParam)
{
	HWINDOW hWindow1=XModalWnd_CreateWindow(410, 160, L"��ѡ���ӡ����", XWnd_GetHWnd(g_hWindow));
	if(hWindow1)
	{
		HELE hButton=XBtn_Create(20,45,74,82,L"��ʱ",hWindow);
		HELE hBtnCancel=XBtn_Create(120,45,74,82,L"Ѳ��",hWindow);//������ť
		HELE hBtnCancel2=XBtn_Create(220,45,74,82,L"��ͨ",hWindow);//������ť
		HELE hButtonOK=XBtn_Create(320,45,74,82,L"����",hWindow);//������ť
		
		//XEle_RegisterEvent(hButton,XE_BNCLICK,OnLogOutBtnClick);//ע�ᰴť����¼�
		XEle_RegisterEvent(hButton,XE_BNCLICK,OnLSBtnClick);
		XEle_RegisterEvent(hBtnCancel,XE_BNCLICK,OnXCBtnClick);
		XEle_RegisterEvent(hBtnCancel2,XE_BNCLICK,OnPTBtnClick);
		XEle_RegisterEvent(hButtonOK,XE_BNCLICK,OnQTBtnClick);
		//XWnd_ShowWindow(hWindow,SW_SHOW);//��ʾ����
		HIMAGE hImageStay=XImage_LoadFileRect(L".\\skin\\image\\toolbar_hover.png",0,0,74,82);
		HIMAGE hImageStay1=XImage_LoadFileRect(L".\\skin\\image\\toolbar_pushed.png",0,0,74,82);
		//XEle_RegisterEvent(XWnd_GetEle(hWindow, ID_Reg_Btn), XE_BNCLICK,OnExitApp);
		XBtn_SetImageLeave(hButtonOK,hImageStay);
		XBtn_SetImageStay(hButtonOK,hImageStay1);
		XBtn_SetImageDown(hButtonOK,hImageStay1);
		XEle_SetBkTransparent(hButtonOK,true);
		
		XBtn_SetImageLeave(hBtnCancel,hImageStay);
		XBtn_SetImageStay(hBtnCancel,hImageStay1);
		XBtn_SetImageDown(hBtnCancel,hImageStay1);
		XEle_SetBkTransparent(hBtnCancel,true);
		
		XBtn_SetImageLeave(hBtnCancel2,hImageStay);
		XBtn_SetImageStay(hBtnCancel2,hImageStay1);
		XBtn_SetImageDown(hBtnCancel2,hImageStay1);
		XEle_SetBkTransparent(hBtnCancel2,true);
		
		XBtn_SetImageLeave(hButton,hImageStay);
		XBtn_SetImageStay(hButton,hImageStay1);
		XBtn_SetImageDown(hButton,hImageStay1);
		XEle_SetBkTransparent(hButton,true);
	}
	SetMainDlgEditBorder();
	XWnd_SetBorderSize(hWindow1, 0, 0, 0, 0);
	HIMAGE hImg = XImage_LoadFile(L".\\skin\\image\\frame.jpg", true);
	XWnd_SetImageNC(hWindow1, hImg);
	XModalWnd_DoModal(hWindow1);
	return false;
}
//�Զ���Ӽ��̴�ӡ����Ϣ��Ӧ
BOOL CALLBACK OnSavePrintBySoftKeyBD(HWINDOW hWindow, WPARAM wParam, LPARAM lParam)
{
	wchar_t wBuf[256] = {0};
	HELE hE;
    VISITORINFO visitor;
	memset((char *)&visitor, 0, sizeof(VISITORINFO));

	memset((char *)wBuf, 0, sizeof(wBuf));
	hE = XWnd_GetEle(g_hWindow, IDE_VISITORNAME);
	XEdit_GetText(hE, wBuf, 128);
	W_2_S(wBuf, visitor.szName, 128);

	memset((char *)wBuf, 0, sizeof(wBuf));
	hE = XWnd_GetEle(g_hWindow, IDE_VISITORIDNUM);
	XEdit_GetText(hE, wBuf, 128);
	W_2_S(wBuf, visitor.szID, 128);

	memset((char *)wBuf, 0, sizeof(wBuf));
	hE = XWnd_GetEle(g_hWindow, IDE_MASTERNAME);
	XEdit_GetText(hE, wBuf, 128);
	W_2_S(wBuf, visitor.szToOfficialName, 128);

	BOOL b = CheckVisitor(visitor);
	if (!b)  //������
	{
		//if (g_bAcceptVisite)
		{
			SaveAndPrint();
		}
		/*else
		{
			XMessageBox(g_hWindow, L"�������˵ķ�������û�б�����!", L"����");
			return false;
		}*/

	}
	else if (b==-1)  //�����˸ı���δǩ��
	{
		return false;
	}
	else             //�����͵�
	{
			if (strlen(g_Printvisitor.szName)==0)
			{
                 XMessageBox(g_hWindow, L"�޴�ӡ��Ϣ!", L"����");
		         return false;
			}
		    PrintVisitorNoteAgain();

	}	
			
	XWnd_RedrawWnd(g_hWindow);
	return false;
}

BOOL SaveAndPrint()
{
	if(!g_bFaceCompare)
	{
		XMessageBox(g_hWindow, L"��֤�ȶԲ�ͨ���������µǼ�", L"����");
		return false;
	}

	VISITORINFO visitor;
	OFFICIALINFO official;
	wchar_t wBuf[256 + 1] = {0};
	long n = 0;
	char sz[32] = {'\0'};
	char szFolkName[40] = {'\0'};
	char szOfficialName[32] = {'\0'};
	char szOfficeName[32] = {'\0'};
	long FldCount = 0;
	CWinAdo tAdo(sCONNSTR_MYSQL);
	
	int ret = tAdo.OpenAdo();
	if (ret != ADOOK)
	{
		return false;
	}

	memset((char *)&visitor, 0, sizeof(VISITORINFO));
	memset((char *)&official, 0, sizeof(VISITORINFO));

	::GetPrivateProfileString("MustInput", "VisitorName", "1", sz, sizeof(sz), CONFIG_FILE);
	HELE hE = XWnd_GetEle(g_hWindow, IDE_VISITORNAME);	//�ÿ�����
	if (XEdit_GetTextLength(hE) <= 0 && atoi(sz) == 1)
	{
		XMessageBox(g_hWindow, L"�ÿ���������Ϊ��", L"����");
		return false;
	}
	if (XEdit_GetTextLength(hE) > 16)
	{
		XMessageBox(g_hWindow, L"�ÿ�����̫��", L"����");
		return false;
	}
	XEdit_GetText(hE, wBuf, 128);
	W_2_S(wBuf, visitor.szName, 128);

	memset((char *)wBuf, 0, sizeof(wBuf));
	hE = XWnd_GetEle(g_hWindow, IDC_VISITORSEX);		//�ÿ��Ա�
	//n = XComboBox_GetSelectItem(hE);
	XEdit_GetText(hE, wBuf, 128);
    W_2_S(wBuf, visitor.szSex, 128);
//	n == 0 ? strcpy(visitor.szSex, "��") : strcpy(visitor.szSex, "Ů");

	memset((char *)wBuf, 0, sizeof(wBuf));
	::GetPrivateProfileString("MustInput", "VisitorMobile", "0", sz, sizeof(sz), CONFIG_FILE);
	hE = XWnd_GetEle(g_hWindow, IDE_VISITORMOBILE);		//�ÿ͵绰
	if (XEdit_GetTextLength(hE) <= 0 && atoi(sz) == 1)
	{
		XMessageBox(g_hWindow, L"�ÿ͵绰���벻��Ϊ��", L"����");
		return false;
	}
	if (XEdit_GetTextLength(hE) > 16)
	{
		XMessageBox(g_hWindow, L"�ÿ͵绰����̫��", L"����");
		return false;
	}
	XEdit_GetText(hE, wBuf, 128);
	W_2_S(wBuf, visitor.szPhone, 128);

	memset((char *)wBuf, 0, sizeof(wBuf));
	::GetPrivateProfileString("MustInput", "VisitorID", "0", sz, sizeof(sz), CONFIG_FILE);
	hE = XWnd_GetEle(g_hWindow, IDE_VISITORIDNUM);		//�ÿ�֤������
	if (XEdit_GetTextLength(hE) <= 0 && atoi(sz) == 1)
	{
		XMessageBox(g_hWindow, L"�ÿ�֤�����벻��Ϊ��", L"����");
		return false;
	}
	if (XEdit_GetTextLength(hE) > 30)
	{
		XMessageBox(g_hWindow, L"�ÿ�֤������̫��", L"����");
		return false;
	}
	XEdit_GetText(hE, wBuf, 128);
	W_2_S(wBuf, visitor.szID, 128);

	memset((char *)wBuf, 0, sizeof(wBuf));
	::GetPrivateProfileString("MustInput", "VisitorGoods", "0", sz, sizeof(sz), CONFIG_FILE);
	hE = XWnd_GetEle(g_hWindow, IDE_VISITORTHING);		//�ÿ���Ʒ
	if (XEdit_GetTextLength(hE) <= 0 && atoi(sz) == 1)
	{
		XMessageBox(g_hWindow, L"�ÿ�Я����Ʒ����Ϊ��", L"����");
		return false;
	}
	if (XEdit_GetTextLength(hE) > 30)
	{
		XMessageBox(g_hWindow, L"�ÿ�Я����Ʒ̫��", L"����");
		return false;
	}
	XEdit_GetText(hE, wBuf, 128);
	W_2_S(wBuf, visitor.szWithGoods, 128);

	memset((char *)wBuf, 0, sizeof(wBuf));
	hE = XWnd_GetEle(g_hWindow, IDC_VISITORIDTYPE);		//�ÿ�֤������
	//n = XComboBox_GetSelectItem(hE);
    XEdit_GetText(hE, wBuf, 128);
    W_2_S(wBuf, visitor.szIDType, 128);

	if (strcmp(visitor.szIDType,"�������֤")==0)
	{
        n=0;
	}
	else if (strcmp(visitor.szIDType,"һ�����֤")==0)
	{
        n=1;
	}
	else if (strcmp(visitor.szIDType,"����")==0)
	{
        n=2;
	}
	else if (strcmp(visitor.szIDType,"��½����۰�ͨ��֤")==0)
	{
        n=3;
	}
	else if (strcmp(visitor.szIDType,"�������֤")==0)
	{
        n=4;
	}
	else if (strcmp(visitor.szIDType,"̨��֤")==0)
	{
        n=5;
	}
	else if (strcmp(visitor.szIDType,"��ʻ֤")==0)
	{
        n=6;		
	}
	else if (strcmp(visitor.szIDType,"��ʦ֤")==0)
	{
        n=7;		
	}
	else if (strcmp(visitor.szIDType,"��Ƭ")==0)
	{
        n=8;		
	}
	else
        n=98;
	sprintf(visitor.szIDType, "%ld", n + 1);

	memset((char *)wBuf, 0, sizeof(wBuf));
	hE = XWnd_GetEle(g_hWindow, IDC_VISITORFOLK);		//�ÿ�����
	//n = XComboBox_GetSelectItem(hE);
	//sprintf(visitor.szFolk, "%ld", n + 1);
	//wcscpy(wBuf, XComboBox_GetItemText(hE, n));
	XEdit_GetText(hE, wBuf, 128);
	W_2_S(wBuf, visitor.szFolk, 32);

	memset((char *)wBuf, 0, sizeof(wBuf));
	::GetPrivateProfileString("MustInput", "VisitorReason", "0", sz, sizeof(sz), CONFIG_FILE);
	hE = XWnd_GetEle(g_hWindow, IDE_VISITORREASON);		//�ÿ�ԭ��
	if (XEdit_GetTextLength(hE) <= 0 && atoi(sz) == 1)
	{
		XMessageBox(g_hWindow, L"�ÿ��������ɲ���Ϊ��", L"����");
		return false;
	}
	if (XEdit_GetTextLength(hE) > 20)
	{
		XMessageBox(g_hWindow, L"�ÿ���������̫��", L"����");
		return false;
	}
	XEdit_GetText(hE, wBuf, 128);
	W_2_S(wBuf, visitor.szReason, 128);

	memset((char *)wBuf, 0, sizeof(wBuf));
	::GetPrivateProfileString("MustInput", "VisitorAddr", "0", sz, sizeof(sz), CONFIG_FILE);
	hE = XWnd_GetEle(g_hWindow, IDE_VISITORADDR);		//�ÿ͵�ַ
	if (XEdit_GetTextLength(hE) <= 0 && atoi(sz) == 1)
	{
		XMessageBox(g_hWindow, L"�ÿ͵�ַ����Ϊ��", L"����");
		return false;
	}
	if (XEdit_GetTextLength(hE) > 50)
	{
		XMessageBox(g_hWindow, L"�ÿ͵�ַ̫��", L"����");
		return false;
	}
	XEdit_GetText(hE, wBuf, 128);
	W_2_S(wBuf, visitor.szAddr, 128);

	if(g_bVisitorType)
	{
		strcpy(visitor.szTotalPeople, "1");  //Ĭ��1��
		
		memset((char *)wBuf, 0, sizeof(wBuf));
		::GetPrivateProfileString("MustInput", "VisitorTotal", "0", sz, sizeof(sz), CONFIG_FILE);
		hE = XWnd_GetEle(g_hWindow, IDE_VISITORTYPE);		//�ÿ�����
		if (XEdit_GetTextLength(hE) <= 0 && atoi(sz) == 1)
		{
			XMessageBox(g_hWindow, L"�ÿ����Ͳ���Ϊ��", L"����");
			return false;
		}
		if (XEdit_GetTextLength(hE) > 50)
		{
			XMessageBox(g_hWindow, L"�ÿ�����̫��", L"����");
			return false;
		}
		XEdit_GetText(hE, wBuf, 128);
		W_2_S(wBuf, visitor.szType, 128);
	}
	else
	{
		memset((char *)wBuf, 0, sizeof(wBuf));
		::GetPrivateProfileString("MustInput", "VisitorTotal", "0", sz, sizeof(sz), CONFIG_FILE);
		hE = XWnd_GetEle(g_hWindow, IDE_VISITORTOTAL);		//�ÿ�����
		if (XEdit_GetTextLength(hE) <= 0 && atoi(sz) == 1)
		{
			XMessageBox(g_hWindow, L"�ÿ�������������Ϊ��", L"����");
			return false;
		}
		if (XEdit_GetTextLength(hE) > 3)
		{
			XMessageBox(g_hWindow, L"�ÿ���������̫��", L"����");
			return false;
		}
		XEdit_GetText(hE, wBuf, 128);
		W_2_S(wBuf, visitor.szTotalPeople, 128);

		strcpy(visitor.szType, "");
	}

	memset((char *)wBuf, 0, sizeof(wBuf));
	::GetPrivateProfileString("MustInput", "VisitorUnit", "0", sz, sizeof(sz), CONFIG_FILE);
    if(g_nIDType == ID_TYPE_Lawyer)
	{
		hE = XWnd_GetEle(g_hWindow, IDC_Notice_Comb);		//��֪ͥͨ��
		if (XEdit_GetTextLength(hE) <= 0 && atoi(sz) == 1)
		{
			XMessageBox(g_hWindow, L"��֪ͥͨ�鲻��Ϊ��", L"����");
			return false;
		}
		if (XEdit_GetTextLength(hE) > 50)
		{
			XMessageBox(g_hWindow, L"��֪ͥͨ��̫��", L"����");
			return false;
		}
	}
	else
	{
		hE = XWnd_GetEle(g_hWindow, IDE_VISITORUNIT);		//�ÿ͵�λ
		if (XEdit_GetTextLength(hE) <= 0 && atoi(sz) == 1)
		{
			XMessageBox(g_hWindow, L"�ÿ͵�λ����Ϊ��", L"����");
			return false;
		}
		if (XEdit_GetTextLength(hE) > 50)
		{
			XMessageBox(g_hWindow, L"�ÿ͵�λ̫��", L"����");
			return false;
		}
	}
	XEdit_GetText(hE, wBuf, 128);
	W_2_S(wBuf, visitor.szUnit, 128);

	memset((char *)wBuf, 0, sizeof(wBuf));
	::GetPrivateProfileString("MustInput", "VisitorCarNum", "0", sz, sizeof(sz), CONFIG_FILE);
	if(g_nIDType == ID_TYPE_Lawyer)
	{
		hE = XWnd_GetEle(g_hWindow, IDC_LawyerLetter_Comb);		//��ʦ����������
		if (XEdit_GetTextLength(hE) <= 0 && atoi(sz) == 1)
		{
			XMessageBox(g_hWindow, L"��ʦ��������������Ϊ��", L"����");
			return false;
		}
		if (XEdit_GetTextLength(hE) > 10)
		{
			XMessageBox(g_hWindow, L"��ʦ����������̫��", L"����");
			return false;
		}
	}
	else
	{
		hE = XWnd_GetEle(g_hWindow, IDE_VISITORCARNUM);		//���ƺ�
		if (XEdit_GetTextLength(hE) <= 0 && atoi(sz) == 1)
		{
			XMessageBox(g_hWindow, L"�ÿͳ��ƺ��벻��Ϊ��", L"����");
			return false;
		}
		if (XEdit_GetTextLength(hE) > 10)
		{
			XMessageBox(g_hWindow, L"�ÿͳ��ƺ���̫��", L"����");
			return false;
		}
	}
	XEdit_GetText(hE, wBuf, 128);
	W_2_S(wBuf, visitor.szCarNum, 128);

	memset((char *)wBuf, 0, sizeof(wBuf));
	::GetPrivateProfileString("MustInput", "VisitorSignDepartment", "0", sz, sizeof(sz), CONFIG_FILE);
	hE = XWnd_GetEle(g_hWindow, IDE_FZJG);				//��֤����
	if(g_bJob)
	{
		if (XEdit_GetTextLength(hE) <= 0 && atoi(sz) == 1)
		{
			XMessageBox(g_hWindow, L"�ÿ�ְ�����ֲ���Ϊ��", L"����");
			return false;
		}
		if (XEdit_GetTextLength(hE) > 50)
		{
			XMessageBox(g_hWindow, L"�ÿ�ְ������̫��", L"����");
			return false;
		}
		XEdit_GetText(hE, wBuf, 128);
		W_2_S(wBuf, visitor.szJob, 128);

		if(n==0)  // �������֤
		{
			//memset((char *)wBuf, 0, sizeof(wBuf));
			hE = XWnd_GetEle(g_hWindow, IDS_ID2_SIGNDEPARTMENT);
			//XStatic_GetText(hE, wBuf, 128);
		    W_2_S(XStatic_GetText(hE), visitor.szSignDepartment, 128);
		}
	}
	else
	{
		if (XEdit_GetTextLength(hE) <= 0 && atoi(sz) == 1)
		{
			XMessageBox(g_hWindow, L"�ÿ�֤���ķ�֤���ز���Ϊ��", L"����");
			return false;
		}
		if (XEdit_GetTextLength(hE) > 30)
		{
			XMessageBox(g_hWindow, L"�ÿ�֤���ķ�֤����̫��", L"����");
			return false;
		}
		XEdit_GetText(hE, wBuf, 128);
		W_2_S(wBuf, visitor.szSignDepartment, 128);
	}




	memset((char *)wBuf, 0, sizeof(wBuf));
	::GetPrivateProfileString("MustInput", "OfficialName", "1", sz, sizeof(sz), CONFIG_FILE);
	hE = XWnd_GetEle(g_hWindow, IDE_MASTERNAME);		//������������
	if (XEdit_GetTextLength(hE) <= 0 && atoi(sz) == 1)
	{
		XMessageBox(g_hWindow, L"����������������Ϊ��", L"����");
		return false;
	}
	if (XEdit_GetTextLength(hE) > 16)
	{
		XMessageBox(g_hWindow, L"������������̫��", L"����");
		return false;
	}
	XEdit_GetText(hE, wBuf, 128);
	W_2_S(wBuf, visitor.szToOfficialName, 128);
	W_2_S(wBuf, official.szName,128);

	memset((char *)wBuf, 0, sizeof(wBuf));
	hE = XWnd_GetEle(g_hWindow, IDC_MASTERSEX);		//���������Ա�
	//n = XComboBox_GetSelectItem(hE);
	//n == 0 ? strcpy(official.szSex, "��") : strcpy(official.szSex, "Ů");
	XEdit_GetText(hE, wBuf, 128);
    W_2_S(wBuf, official.szSex, 128);

	memset((char *)wBuf, 0, sizeof(wBuf));
	::GetPrivateProfileString("MustInput", "OfficeName", "0", sz, sizeof(sz), CONFIG_FILE);
	hE = XWnd_GetEle(g_hWindow, IDE_OFFICENAME);			//�����˰칫��
	if (XEdit_GetTextLength(hE) <= 0 && atoi(sz) == 1)
	{
		XMessageBox(g_hWindow, L"�������˰칫����������Ϊ��", L"����");
		return false;
	}
	if (XEdit_GetTextLength(hE) > 20)
	{
		XMessageBox(g_hWindow, L"�������˰칫������̫��", L"����");
		return false;
	}
	XEdit_GetText(hE, wBuf, 128);
	W_2_S(wBuf, visitor.szToOfficeName, 128);
	W_2_S(wBuf, official.szOfficeName,128);


	memset((char *)wBuf, 0, sizeof(wBuf));
	::GetPrivateProfileString("MustInput", "OfficialPhone", "0", sz, sizeof(sz), CONFIG_FILE);
	hE = XWnd_GetEle(g_hWindow, IDE_OFFICEPHONE);				//�����˰칫�绰
	if (XEdit_GetTextLength(hE) <= 0 && atoi(sz) == 1)
	{
		XMessageBox(g_hWindow, L"�������˰칫�绰����Ϊ��", L"����");
		return false;
	}
	if (XEdit_GetTextLength(hE) > 10)
	{
		XMessageBox(g_hWindow, L"�������˰칫�绰̫��", L"����");
		return false;
	}
	XEdit_GetText(hE, wBuf, 128);
	W_2_S(wBuf, official.szOfficePhone,128);

	memset((char *)wBuf, 0, sizeof(wBuf));
	::GetPrivateProfileString("MustInput", "OfficeAddr", "0", sz, sizeof(sz), CONFIG_FILE);
	hE = XWnd_GetEle(g_hWindow, IDE_OFFICENUM);				//�����˰칫��ַ
	if (XEdit_GetTextLength(hE) <= 0 && atoi(sz) == 1)
	{
		XMessageBox(g_hWindow, L"�������˰칫��ַ����Ϊ��", L"����");
		return false;
	}
	if (XEdit_GetTextLength(hE) > 15)
	{
		XMessageBox(g_hWindow, L"�������˰칫��ַ̫��", L"����");
		return false;
	}
	XEdit_GetText(hE, wBuf, 128);
	W_2_S(wBuf, official.szOfficeAddr,128);

	if(g_bSSO)
	{
		memset((char *)wBuf, 0, sizeof(wBuf));
		::GetPrivateProfileString("MustInput", "OfficialSSO", "0", sz, sizeof(sz), CONFIG_FILE);
		hE = XWnd_GetEle(g_hWindow, IDC_MASTERSSO);				//�����˷����
		if (XEdit_GetTextLength(hE) <= 0 && atoi(sz) == 1)
		{
			XMessageBox(g_hWindow, L"��������SSO����Ϊ��", L"����");
			return false;
		}
		if (XEdit_GetTextLength(hE) > 10)
		{
			XMessageBox(g_hWindow, L"��������SSO̫��", L"����");
			return false;
		}
		XEdit_GetText(hE, wBuf, 128);
		W_2_S(wBuf, visitor.szToSSO, 128);
		W_2_S(wBuf, official.szSSO,128);
	}
	else
	{
		memset((char *)wBuf, 0, sizeof(wBuf));
		::GetPrivateProfileString("MustInput", "OfficialRoom", "0", sz, sizeof(sz), CONFIG_FILE);
		hE = XWnd_GetEle(g_hWindow, IDE_ROOMNUM);				//�����˷����
		if (XEdit_GetTextLength(hE) <= 0 && atoi(sz) == 1)
		{
			XMessageBox(g_hWindow, L"�������˷���Ų���Ϊ��", L"����");
			return false;
		}
		if (XEdit_GetTextLength(hE) > 10)
		{
			XMessageBox(g_hWindow, L"�������˷����̫��", L"����");
			return false;
		}
		XEdit_GetText(hE, wBuf, 128);
		W_2_S(wBuf, visitor.szToRoom, 128);
		W_2_S(wBuf, official.szRoom,128);

		memset((char *)wBuf, 0, sizeof(wBuf));
		::GetPrivateProfileString("MustInput", "OfficialMobile", "0", sz, sizeof(sz), CONFIG_FILE);
		hE = XWnd_GetEle(g_hWindow, IDE_MASTERMOBILE);				//�������ƶ��绰
		if (XEdit_GetTextLength(hE) <= 0 && atoi(sz) == 1)
		{
			XMessageBox(g_hWindow, L"���������ֻ��Ų���Ϊ��", L"����");
			return false;
		}
		if (XEdit_GetTextLength(hE) > 20)
		{
			XMessageBox(g_hWindow, L"���������ֻ���̫��", L"����");
			return false;
		}
		XEdit_GetText(hE, wBuf, 128);
		W_2_S(wBuf, official.szMobile,128);

		memset((char *)wBuf, 0, sizeof(wBuf));
		::GetPrivateProfileString("MustInput", "OfficialLevel", "0", sz, sizeof(sz), CONFIG_FILE);
		hE = XWnd_GetEle(g_hWindow, IDC_MASTERZHIWU);				//�����˼���
		if (XEdit_GetTextLength(hE) <= 0 && atoi(sz) == 1)
		{
			XMessageBox(g_hWindow, L"�������˼�����Ϊ��", L"����");
			return false;
		}
		if (XEdit_GetTextLength(hE) > 10)
		{
			XMessageBox(g_hWindow, L"�������˼���̫��", L"����");
			return false;
		}
		XEdit_GetText(hE, wBuf, 128);
		W_2_S(wBuf, official.szOfficialDuty,128);
	}



	char szT[512] = {'\0'}, szF[256] = {'\0'}, szTime[32] = {'\0'};
	GetDayFolder(szF);
	if (g_bCalled)    //�����й��󣬻�ȡ����ʱ��ʱ��
	{
		visitor.lVisiteTime = atol(g_szRecord);
       	
        //HDC hDC = ::GetDC(GETHWND);
		
	    //DrawBarCode(hDC, visitor.szBarCode, USER_BARCODE_PIC);
	    //::ReleaseDC(GETHWND, hDC);
		//GetBarCode(visitor.szBarCode); //���ɷ��ϸ�ʽ���������ݣ�����
		//GetBarCodePic(visitor.szBarCode);
	    CString str = CString(g_VisitorPic.szUserIDPic);
		strcpy(g_Printvisitor.szUserIDPic, g_VisitorPic.szUserIDPic);
	    str.Replace("\\", "#");	
		strcpy(visitor.szUserIDPic,str);
		
		str = CString(g_VisitorPic.szUserIDFullPic);
		strcpy(g_Printvisitor.szUserIDFullPic, g_VisitorPic.szUserIDFullPic);
	    str.Replace("\\", "#");	
		strcpy(visitor.szUserIDFullPic,str);

		str = CString(g_VisitorPic.szUserHeadPic);
		strcpy(g_Printvisitor.szUserHeadPic, g_VisitorPic.szUserHeadPic);
	    str.Replace("\\", "#");	
		strcpy(visitor.szUserHeadPic,str);

		str = CString(g_VisitorPic.szUserVideoPic);
		strcpy(g_Printvisitor.szUserVideoPic, g_VisitorPic.szUserVideoPic);
	    str.Replace("\\", "#");	
		strcpy(visitor.szUserVideoPic,str);
	}
	else
	{
		time((time_t *)&n);
	    visitor.lVisiteTime = n;			//����ʱ��	
		
		//sprintf(visitor.szBarCode, "%ld879", n);	//���ʵ�������,�ԵǼ�ʱ����Ϊ������
        
	    //HDC hDC = ::GetDC(GETHWND);
	    //DrawBarCode(hDC, visitor.szBarCode, USER_BARCODE_PIC);
	    //::ReleaseDC(GETHWND, hDC);
		//GetBarCode(visitor.szBarCode); //���ɷ��ϸ�ʽ���������ݣ�����
		//GetBarCodePic(visitor.szBarCode);
		sprintf(szT, "%s\\%s_%s_%ld_id.bmp", szF, visitor.szName, visitor.szID, n);
		if (g_nID2ReadType==ID2_READ)
		{
            ::CopyFile(g_stIDCard.szPicPath, szT, false);
			strcpy(visitor.szID2Flag,"1");
		}
		else
		    ::CopyFile(USER_ID_PIC, szT, false);	//���û�ɨ���֤��ͷ����Ƭ�������
		if (g_bFtpOpened)
		{
             m_pFtpConnection->PutFile(szT,szT);     //ftp�ϴ��ļ�
		}
		CString str = CString(szT);
		strcpy(g_Printvisitor.szUserIDPic, szT);
		str.Replace("\\", "#");					//����������ݿ��е�·�����Ż���#
		memset(szT, 0, sizeof(szT));
		strcpy(szT, str);
		strcpy(visitor.szUserIDPic, szT);			//������֤��ͷ����Ƭ���·��

		sprintf(szT, "%s\\%s_%s_%ld_idfull.bmp", szF, visitor.szName, visitor.szID, n);
		::CopyFile(USER_ID2_PIC, szT, false);	//���û�ɨ���֤����Ƭ�������
		if (g_bFtpOpened)
		{
             m_pFtpConnection->PutFile(szT,szT);     //ftp�ϴ��ļ�
		}

		str = CString(szT);
		strcpy(g_Printvisitor.szUserIDFullPic, szT);
		str.Replace("\\", "#");					//����������ݿ��е�·�����Ż���#
		memset(szT, 0, sizeof(szT));
		strcpy(szT, str);
		strcpy(visitor.szUserIDFullPic, szT);		//������ͷ����Ƭ���·��

		sprintf(szT, "%s\\%s_%s_%ld_head.bmp", szF, visitor.szName, visitor.szID, n);
		::CopyFile(USER_HEAD_PIC, szT, false);	//���û�ɨ���ͷ����Ƭ�������
		if (g_bFtpOpened)
		{
             m_pFtpConnection->PutFile(szT,szT);     //ftp�ϴ��ļ�
		}

		str = CString(szT);
		strcpy(g_Printvisitor.szUserHeadPic, szT);
		str.Replace("\\", "#");					//����������ݿ��е�·�����Ż���#
		memset(szT, 0, sizeof(szT));
		strcpy(szT, str);
		strcpy(visitor.szUserHeadPic, szT);		//������ͷ����Ƭ���·��

		sprintf(szT, "%s\\%s_%s_%ld_video.jpg", szF, visitor.szName, visitor.szID, n);
		::CopyFile(USER_VIDEO_PIC, szT, false);	//����Ƶ�������Ƭ�������
		if (g_bFtpOpened)
		{
             m_pFtpConnection->PutFile(szT,szT);     //ftp�ϴ��ļ�
		}

		str = CString(szT);
		str.Replace("\\", "#");					//����������ݿ��е�·�����Ż���#
		memset(szT, 0, sizeof(szT));
		strcpy(szT, str);
		strcpy(visitor.szUserVideoPic, szT);	//��������Ƶ��Ƭ���·��		

		    //��ʾ���ս�ͼ
		if (::GetPrivateProfileInt("TerminalSet", "bVisitorPicMust",0, CONFIG_FILE)==1)
		{
			CString sVideoPic,sHeadPic,sIDPic;
			sVideoPic=CString(visitor.szUserVideoPic);
			sVideoPic.Replace("#", "\\");
			if(_access(sVideoPic,0)!=0) // �������ļ�
			{
				XMessageBox(g_hWindow,L"������û������ͼƬ",L"��ʾ");
				return false;
			}
			
			sHeadPic=CString(visitor.szUserHeadPic);
			sHeadPic.Replace("#", "\\");
			sIDPic=CString(visitor.szUserIDPic);
			sIDPic.Replace("#", "\\");
			if (atoi(visitor.szID2Flag) == ID2_READ)		//�Ķ��Ķ���֤
			{
				if(_access(sHeadPic,0)!=0&&_access(sIDPic,0)!=0)  //Сͼ��������
				{
					XMessageBox(g_hWindow,L"������������Ҫһ��ͷ��Сͼ���ͼ",L"��ʾ");
					return false;
				}
			}
			else
			{
				if(_access(sHeadPic,0)!=0)  //ɨ��õ���֤����ֻ�жϽ�ͼ
				{
					XMessageBox(g_hWindow,L"��������Ҫ��ͼ",L"��ʾ");
					return false;
				}
			}
		}
	}
    visitor.lLeftTime = 0;				//�뿪ʱ��
	memset((char *)&g_VisitorPic, 0, sizeof(PICPATH));//����ͼƬ�õ����棬���
	

	//�ÿʹ��ĸ��Ÿڽ���
	::GetPrivateProfileString("TerminalSet", "StationName", "", visitor.szVisitStation, sizeof(visitor.szVisitStation), CONFIG_FILE);

	if (g_nID2ReadType == ID2_READ)
	{
		sprintf(visitor.szID2Flag, "%d", g_nID2ReadType);	//����֤���Ķ���ʽ
		strcpy(visitor.szValidDate, g_stIDCard.validterm);		//����֤����Ч����
		strcpy(visitor.szBirth, g_stIDCard.birthday);			//����
		if (!CheckID2(visitor))
		{
		   XMessageBox(g_hWindow, L"�������˵�֤����Ϣ���ֶ��޸�!������ˢȡ֤��", L"����");
		   return false;
		}
	}

	EnableButton(IDB_TAB9);

	//if (CheckBlackName(visitor))
	char szNotes[200] = {'\0'},szMsg[500] = {'\0'};
	if (CheckBlackNameNotes(visitor,szNotes))
	{
		if (strlen(szNotes) > 0)
		{
			sprintf(szMsg, "���������ں�������,����ϸȷ��������!\r\n\r\n�澯��%s",szNotes);
			S_2_W(wBuf, szMsg, 256);
			CSelectPrinttips s(GETHWND);
			s.CreateWindow_XC(wBuf);
			s.DoModal_XC();
		}
		else
		{
			sprintf(szMsg, "���������ں�������,����ϸȷ��������!");
			S_2_W(wBuf, szMsg, 256);
			XMessageBox(g_hWindow, wBuf, L"����");
			//CSelectPrinttips s(GETHWND);
			//s.CreateWindow_XC(wBuf);
			//s.DoModal_XC();
		}
		return false;
	}

	CString outParas[FLD_MAX];
	for (int i = 0; i < FLD_MAX; i++)
	{
		outParas[i].Empty();
	}
    sprintf(szT, "select official_IP from t_official where official_name = \'%s\' and official_office_name = \'%s\' and official_sex = \'%s\' and official_office_phone = \'%s\' and official_mobile = \'%s\' and official_office_addr = \'%s\' and official_duty = \'%s\' and official_room = \'%s\'",
	official.szName, official.szOfficeName,official.szSex,official.szOfficePhone,official.szMobile,official.szOfficeAddr,official.szOfficialDuty,official.szRoom);
	ret = tAdo.execSelect(szT, &FldCount, outParas);
	strcpy(g_szOfficialIP, outParas[0]); //������ip

	//������Ϊ������д���Ҳ�����ʱ��Ĭ�ϱ��浽�����˱���
	if (FldCount == 0)
	{
	    if (::GetPrivateProfileInt("TerminalSet", "bSaveOfficial",0, CONFIG_FILE)==1)	
		{
			sprintf(szT, "insert into t_official set official_name =  \'%s\', official_office_name = \'%s\',official_sex = \'%s\', official_office_phone = \'%s\' , official_mobile = \'%s\' , official_office_addr = \'%s\' , official_duty = \'%s\' , official_room = \'%s\',official_pwd = \'123\',official_state = 0,official_NO=\'\',official_IP=\'\'",
				official.szName, official.szOfficeName,official.szSex,official.szOfficePhone,official.szMobile,official.szOfficeAddr,official.szOfficialDuty,official.szRoom);
			ret = tAdo.execUpdateInsert(szT);
			if (ret != ADOOK)
			{
				XMessageBox(g_hWindow,L"��ӱ�������Ϣʧ��",L"��ʾ");
				return false;
			}
		}
	}

	if(g_bAutoCallOfficials == false)
	{
		strcpy(g_szOfficialIP, outParas[0]); //������ip
		if (g_bAcceptVisite||strlen(g_szOfficialIP)==0)
		{
			;//SaveAndPrint();
		}
		else
		{
			XMessageBox(g_hWindow, L"�������˵ķ�������û�б�����!", L"����");
			return false;
		}
	}

	//ֻ֧�ֶ���֤IC�����
	if (::GetPrivateProfileInt("TerminalSet", "bMJConnetion",0, CONFIG_FILE)==1) 
	{
		if (atoi(visitor.szIDType)==ID2_READ||atoi(visitor.szIDType)==ID2_SCAN)
		{
			if (strlen(g_szICCode)<=0)  //����IC����
			{
				XMessageBox(g_hWindow,L"�������֤IC��",L"��ʾ");
				return false;
			}
			sprintf(szT, "insert into t_card set card_Factory = \'%s\',card_id = \'%s\',card_status = \'%d\'", g_szICCode,visitor.szID,2);
			ret = tAdo.execUpdateInsert(szT);
			if (ret != ADOOK)
			{
				XMessageBox(g_hWindow,L"IC������ʧ��",L"��ʾ");
				return false;
			}
			memset(g_szICCode,0,sizeof(g_szICCode));
		}
	}


	visitor.nStatus=2;  //�ÿ��Ѵ�ӡ��͵�
	if (::GetPrivateProfileInt("TerminalSet", "bUptoServer",0, CONFIG_FILE)==1)
	{
		CreateVisitorInfTipsUptoServer(visitor);
	}
	GetBarCode(visitor.szBarCode); //���ɷ��ϸ�ʽ���������ݣ�����
	GetBarCodePic(visitor.szBarCode);

	BOOL bRet;
	if (g_bCalled)  //��ʾ�����ն�����ϵͳ���ÿ���Ϣ�ѵõ�����
	{
        sprintf(szT, "update t_visitor set visitor_status = 2, visitor_barcode = \'%s\' where visitor_visitetime = %ld",visitor.szBarCode, visitor.lVisiteTime);
	    bRet = tAdo.execUpdateInsert(szT);
		if (ret != ADOOK)
		{
			XMessageBox(g_hWindow,L"���������˵Ǽ���Ϣ״̬Ϊ2ʱʧ��",L"��ʾ");
			return false;
		}
	}
	else
	{
		visitor.nStatus=2;  //�ÿ��Ѵ�ӡ��͵�
		bRet=SaveVisitorInfo(visitor);
		if (bRet==false)
		{
			XMessageBox(g_hWindow,L"���������˵Ǽ���Ϣʧ��,����������Ϣ�����Ƿ�Խ��",L"��ʾ");
			return false;
		}

		//�Զ�����ÿ͵�λ�����Ҽ���Ƶ��
		bRet=AutoCheckCompanyName(visitor.szUnit);
		if (bRet==false)
		{
			XMessageBox(g_hWindow,L"�Զ�����ÿ͵�λʧ��,������ѯ������Ա",L"��ʾ");
			return false;
		}
	}




	if(g_bAutoCallOfficials == false)
	{
		if (::GetPrivateProfileInt("TerminalSet", "bSaveNoPrint",0, CONFIG_FILE)==0)
		{

			char szT[512] = {'\0'}, szPath[256] = {'\0'};
			::GetCurrentDirectory(256, szPath);
			//sprintf(szT, "%s\\%s", szPath, RECEIPT_HTML);
			sprintf(szT, "%s\\%s", szPath, INDEX_HTML);
			//strcpy(visitor.szFolk, szFolkName);
			
			if (g_PrintTipsCfg.ePrintType==PRINT_FULL_TYPE&&g_PrintTipsCfg.bTotalPeoplePrint)
			{
				if(!CreateReceiptHtml(visitor, szT))
				{
					XMessageBox(g_hWindow,L"�޷�������͵�",L"����");
				}
			}
			else if (g_PrintTipsCfg.ePrintType==PRINT_NOFRAME_TYPE)
			{
				if(!CreateNoFrameReceiptHtml(visitor, szT))
				{
					XMessageBox(g_hWindow,L"�޷�������͵�",L"����");
				}
			}
			else
			{
				if(!CreateReceiptHtml2(visitor, szT))
				//if(!CreateNoFrameReceiptHtml(visitor, szT))
				{
					XMessageBox(g_hWindow,L"�޷�������͵�",L"����");
				}
			}

			PrintHtmlPage(szT);					//��ӡ��͵�
		}
	}

    ListTodayVistorsInfo();				//ˢ����ʾ������������Ϣ
	VisitorTotalInfo(szT);				//ˢ���ܵ�����Ϣ
	

	if (::GetPrivateProfileInt("TerminalSet", "bLeaveOfficial",0, CONFIG_FILE)==1)
	{
		//�������������
		if (::GetPrivateProfileInt("TerminalSet", "bSaveAfterScan",0, CONFIG_FILE)!=1)
			InitVisitorData();
	}
	else
	{
		//�����������
		InitData();
	}


	::DeleteFile(USER_HEAD_PIC);            //ɾ���ϴ�ͷ��
	::DeleteFile(USER_ID_PIC); 
	::DeleteFile(USER_ID2_PIC);
	::DeleteFile(USER_ID2_TMP);
	::DeleteFile(USER_VIDEO_PIC);
	return true;
}
void YUVToRGB(int y,int u,int v,int &r,int &g,int &b)
{
	r=y+1.402*(v-128);
	g=y-0.34414*(u-128)-0.71414*(v-128);
	b=y+1.772*(u-128);
	if(r>255)r=255;
	if(g>255)g=255;
	if(b>255)b=255;

	if(r<0)r=0;
	if(g<0)g=0;
	if(b<0)b=0;
}



BOOL VideoPreview2()
{
	if (g_bVideoOpened)
	{
		return true;
	}

	//HWND hWnd = XWnd_GetHWnd(g_hWindow);
	RECT rc;
	rc.bottom=40+240;
	rc.right=590+320;
	rc.left=590;
	rc.top=40;
	//OpenCapture((long)XWnd_GetHWnd(g_hWindow),rc);

	
	/*g_hCapWnd = capCreateCaptureWindow((LPTSTR)TEXT("video"),
										WS_CHILD | WS_VISIBLE | WS_EX_CLIENTEDGE | WS_EX_DLGMODALFRAME,
										590, 40, 320, 240, hWnd, 0);*/


	if (OpenCapture((long)XWnd_GetHWnd(g_hWindow),rc))
	{

		g_bVideoOpened = true;
		g_bStartClipScreen = false;

	}
	else
	{
		return false;
	}

	return true;
}
//��������ͷԤ��
BOOL VideoPreview()
{
	if (g_bVideoOpened)
	{
		return true;
	}

//	CAPDRIVERCAPS CapDrvCap;
//	CAPSTATUS CapStatus;

	HWND hWnd = XWnd_GetHWnd(g_hWindow);
	g_hCapWnd = capCreateCaptureWindow((LPTSTR)TEXT("video"),
										WS_CHILD | WS_VISIBLE | WS_EX_CLIENTEDGE | WS_EX_DLGMODALFRAME,
										590, 40, 320, 240, hWnd, 0);

/*	BITMAPINFOHEADER bi;
    capGetVideoFormat(g_hCapWnd,&bi,sizeof(bi));
      bi.biWidth = 174;
      bi.biHeight = 144;
      bi.biSizeImage = 0;    
    if(!capSetVideoFormat(g_hCapWnd, &bi, sizeof(bi)))
		return false;*/
          // MessageBox("��֧�ֵķֱ���");

    //SendMessage(g_hCapWnd, WM_CAP_DLG_VIDEOSOURCE, 1, 0);
	int i=0;
	while((capDriverConnect(g_hCapWnd, 0)!=TRUE)&&i<1000)
	{
		i++;
		continue;
	}
	if (i>=1000)
	{
		return false;
	}

	BOOL b = false;
	if ((b = capDriverConnect(g_hCapWnd, 0)))
	{
		//memset((char *)&CapDrvCap, 0, sizeof(CAPDRIVERCAPS));
		//b = capDriverGetCaps(g_hCapWnd, sizeof(CAPDRIVERCAPS), &CapDrvCap);

		//capGetStatus(g_hCapWnd, &CapStatus, sizeof(CapStatus));
		capPreviewRate(g_hCapWnd, 80);
		capPreview(g_hCapWnd, TRUE);
		capPreviewScale(g_hCapWnd, TRUE);

		g_bVideoOpened = true;
		g_bStartClipScreen = false;

	}
	else
	{
		return false;
	}

	/*while (1)
	{
       capDriverConnect(g_hCapWnd, 0);
	}*/
/*	CAPTUREPARMS CapParms={0};	
	capCaptureGetSetup(g_hCapWnd,&CapParms,sizeof(CapParms));

	CapParms.fAbortLeftMouse = FALSE;
	CapParms.fAbortRightMouse = FALSE;
	CapParms.fYield = TRUE;
	CapParms.fCaptureAudio = FALSE;
	CapParms.wPercentDropForError = 50;

	if(capCaptureSetSetup(g_hCapWnd,&CapParms,sizeof(CapParms))==FALSE)
	{
	//log.WriteString("\n Failed to set the capture parameters ");
	return FALSE;
	}
*/
	BITMAPINFOHEADER bi;
    capGetVideoFormat(g_hCapWnd,&bi,sizeof(bi));
     bi.biWidth = 320;
     bi.biHeight = 240;
     //bi.biSizeImage = 9024; 
	 //bi.biSize=9024;
    if(!capSetVideoFormat(g_hCapWnd, &bi, sizeof(bi)))
		return false;
	ShowWindow(g_hCapWnd, true);
	capSetCallbackOnFrame(g_hCapWnd,FrameCallBack);
	return true;
}

LRESULT CALLBACK FrameCallBack(HWND hWnd, LPVIDEOHDR lpVHdr)
{
	if (!g_bSendVidoe)
	{
		return 0;
	}

	if (strlen(g_szOfficialIP)<7)
	{
		return 0;
	}
	
    ShowWindow(g_hCapWnd, true);
	BOOL bKeyFrame;
	BITMAPINFO bi;
    capGetVideoFormat(g_hCapWnd,&bi,sizeof(BITMAPINFO));

	BYTE *pData=(BYTE *)lpVHdr->lpData;



	CSize size;
	size.cx=bi.bmiHeader.biWidth;
	size.cy=bi.bmiHeader.biHeight;

	BYTE *pDataNew;
	int length=size.cx*size.cy*3;
	pDataNew=new BYTE[length];
	int oneLengthNew=size.cx*3;
	int oneLengthOld=size.cx*2;
	int  r,g,b,y0,u0,v0,y1,u1,v1,y,u,v;
	int i,j;
	if (844715353==bi.bmiHeader.biCompression)
	{
		for ( i=0;i<size.cy;i++)
			for (j=0;j<size.cx;j+=2)
			{
				
				BYTE temp1=pData[i*oneLengthOld+2*j];//��
				BYTE temp2=pData[i*oneLengthOld+2*j+1];//��
				
				BYTE temp3=pData[i*oneLengthOld+2*(j+1)];//��
				BYTE temp4=pData[i*oneLengthOld+2*(j+1)+1];//��
				
				/////YUV2 Y0 U0 Y1 V0
				y0=temp1;
				u0=temp2;
				v0=temp4;//��
				
				y1=temp3;
				u1=u0;
				v1=temp4;//��
				
				
				
				YUVToRGB(y0,u0,v0,r,g,b);
				
				pDataNew[(size.cy-i-1)*oneLengthNew+3*j]=b;//������ȷ
				pDataNew[(size.cy-i-1)*oneLengthNew+3*j+1]=g;//
				pDataNew[(size.cy-i-1)*oneLengthNew+3*j+2]=r;//
				
				YUVToRGB(y1,u1,v1,r,g,b);
				
				pDataNew[(size.cy-i-1)*oneLengthNew+3*(j+1)]=b;//������ȷ
				pDataNew[(size.cy-i-1)*oneLengthNew+3*(j+1)+1]=g;//
				pDataNew[(size.cy-i-1)*oneLengthNew+3*(j+1)+2]=r;//				
			}//end for			
	}
	else
	{
		memcpy(pDataNew,pData,length);
	}

 /*	FILE * fp = fopen( "c:\\cc.bmp", "w");
	Sleep(0);
    if( !fp ) return 0;
    fwrite( &bfh, 1, sizeof(BITMAPFILEHEADER), fp );
    fwrite( &bih, 1, sizeof(BITMAPINFOHEADER), fp );
    fwrite( pDataNew, 1, bih.biSizeImage, fp );
	Sleep(0);
    fclose( fp );*/

/*	BITMAPINFOHEADER bih;
	bih=bi.bmiHeader;	
	bih.biBitCount=24;
	bih.biCompression=0;
	bih.biSize=sizeof(BITMAPINFOHEADER);
    bih.biSizeImage=length;

    BITMAPFILEHEADER bfh;
     bfh.bfType = 0x4d42; //bm
     bfh.bfSize = bih.biSizeImage // data size
              + sizeof( BITMAPFILEHEADER ) // first section size
              + sizeof( BITMAPINFOHEADER ) // second section size
             ;

       bfh.bfReserved1 = 0; // reserved
       bfh.bfReserved2 = 0; // reserved
       //bfh.bfOffBits = bfh.bfSize - bih.biSizeImage;
	   bfh.bfOffBits = sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER);




	CFile mFileCombin;
	if(!mFileCombin.Open("c:\\cc.bmp",CFile::modeCreate|CFile::modeWrite))
	{
		AfxMessageBox("�����ļ�ʧ��");
		return (LRESULT) FALSE ;
	}
	mFileCombin.Write(&bfh,sizeof(BITMAPFILEHEADER));
	mFileCombin.Write(&bih,sizeof(BITMAPINFOHEADER));
    mFileCombin.Write(pDataNew,bih.biSizeImage);

	return 0;*/
	// fwrite( lpVHdr->lpData, 1, bi.biSizeImage, fp );

	long ret = 0;
	char szT[1024] = {'\0'},sData[1024] = {'\0'},sSendHead[4]={'\0'},sStart[5]={'\0'},sEnd[5]={'\0'};
	char sIP[20] = {'\0'};
	CString str;
	
	CNovelSocket tSock(SOCK_DGRAM);
	UINT dwRead=0;
    //str=lpVHdr->lpData;
	int n=3; 
	sStart[0]=0xa2;
	sStart[1]=0x2;
	sStart[2]=0x0;
	sStart[3]=0x1;
	sStart[4]=0x0;

	sSendHead[0]=0xa2;
	sSendHead[1]=0x3;

	sEnd[0]=0xa2;
	sEnd[1]=0x4;
	sEnd[2]=0x0;
	sEnd[3]=0x1;
    sEnd[4]=0x0;
	strcpy(sIP,g_szOfficialIP);
	//strcpy(sIP,"127.0.0.1");
    

    ret = tSock.SendDataTo(sStart, sizeof(sStart), sIP, 25007);
	//int k=(int)(bi.biSizeImage)/512;
	int k=length/512;
	for (i=0;i<k;i++)
	{
		 memset(szT, 0, sizeof(szT));
         memset(sData, 0, sizeof(sData));
		 //CString stem=str.Mid(i,1024);
		 //strcpy(szT,stem);
         memcpy( szT, sSendHead, sizeof(sSendHead));
		 szT[2]=2;
		 szT[3]=0;
		 n=i*512;
		 //memcpy(sData, lpVHdr->lpData+n, 512);
		 memcpy(sData, pDataNew+n, 512);
		 memcpy(&szT[4], sData, 512);
		 ret = tSock.SendDataTo(szT, 516, sIP, 25007);
	}
    //dwRead=bi.biSizeImage%512;
    dwRead=length%512;
	if(dwRead>0)
	{	 //str.Right(dwRead);
		 memset(szT, 0, sizeof(szT));
		 memcpy( szT, sSendHead, sizeof(sSendHead));
		 n=i*512;
		 if (dwRead>255)
		 {
             szT[2]=1;
			 szT[3]=dwRead-256;
		 }
		 else
		 {
			 szT[2]=0;
			 szT[3]=dwRead;
		 }
	     //memcpy(sData, lpVHdr->lpData+n, dwRead);
		 //memcpy(&szT[4], lpVHdr->lpData+n, dwRead+4);
         memcpy(sData, pDataNew+n, dwRead);
		 memcpy(&szT[4], pDataNew+n, dwRead+4);
	     ret = tSock.SendDataTo(szT, dwRead+4, sIP, 25007);
	     //ret = tSock.SendData(str, dwRead);
	}
	delete [] pDataNew;
	ret = tSock.SendDataTo(sEnd, sizeof(sEnd), sIP, 25007);
	tSock.CloseSock();
	return 0;
}
//����ͷ���ղ��洢Ϊָ���ļ��ٹر�����ͷ
BOOL TakePhoto(wchar_t *pszFileName)
{
	BOOL b = FALSE;
	/*if (!g_bVideoOpened)
	{
		return false;
	}*/

	b = capCaptureSingleFrame(g_hCapWnd);

	char *pcstr = (char *)new char[2 * wcslen(pszFileName) + 1];
	WideCharToMultiByte(CP_ACP, 0, pszFileName, -1, pcstr, 2 * wcslen(pszFileName) + 1, NULL, NULL);
	b = capFileSaveDIB(g_hCapWnd, pcstr);
	delete []pcstr;

	//StopVideoPreview();
	ShowWindow(g_hCapWnd, false);
	Pause();
	g_bVideoOpened = false;

	HIMAGE hImg = XImage_LoadFile(pszFileName, true);
	HELE hEle = XWnd_GetEle(g_hWindow, IDP_CAMERA);
	XPic_SetImage(hEle, hImg);

	g_bStartClipScreen = true;


	return true;		
}

BOOL TakePhoto2(wchar_t *pszFileName)
{
	BOOL b = FALSE;
	/*if (!g_bVideoOpened)
	{
		return false;
	}*/

	char *pcstr = (char *)new char[2 * wcslen(pszFileName) + 1];
	WideCharToMultiByte(CP_ACP, 0, pszFileName, -1, pcstr, 2 * wcslen(pszFileName) + 1, NULL, NULL);
	//b = capFileSaveDIB(g_hCapWnd, pcstr);
	TakePhoto(pcstr);
	delete []pcstr;

	//StopVideoPreview();
	ShowWindow(g_hCapWnd, false);
	Pause();
	//g_bVideoOpened = false;

	HIMAGE hImg = XImage_LoadFile(pszFileName, true);
	HELE hEle = XWnd_GetEle(g_hWindow, IDP_CAMERA);
	XPic_SetImage(hEle, hImg);

	g_bStartClipScreen = true;


	return true;		
}

//ֹͣ��ƵԤ��
BOOL StopVideoPreview()
{
	BOOL b = FALSE;
	if (!g_bVideoOpened)
	{
		return false;
	}

	b = capDriverDisconnect(g_hCapWnd);
	::PostMessage(g_hCapWnd, WM_CLOSE, NULL, NULL);
	g_hCapWnd = NULL;
	g_bVideoOpened = false;
	return true;
}

//�����õİ�ť��Ӧ���������
BOOL CALLBACK OnNewVisitor(HELE hEle, HELE hEventEle)
{
	if (hEle != hEventEle) 
	{
		return false;
	}


	//g_bAcceptVisite = true;					//ȱʡΪ���շ���
    g_bShowCap = true;
	g_bQRcode = false;
    
	::DeleteFile(USER_HEAD_PIC);            //ɾ���ϴ�ͷ��
	::DeleteFile(USER_ID_PIC); 
	::DeleteFile(USER_ID2_PIC);
	::DeleteFile(USER_ID2_TMP);
	::DeleteFile(USER_VIDEO_PIC);
	::DeleteFile(USER_BARCODE_PIC);
    ::DeleteFile(INDEX_HTML);
    //CloseHandle(g_hCom);
	//CloseHandle(g_ThreadID2IC);
    g_nIDType = ID_TYPE_ID2;
	InitData();
	HELE hE = NULL;
	HIMAGE hImg = NULL;
	//hE = XWnd_GetEle(g_hWindow, IDC_VISITORIDTYPE);
	//int n = XComboBox_GetSelectItem(hE);
	//if (n==0)
	{
		hE = XWnd_GetEle(g_hWindow, IDP_ID2PIC);
		hImg = XImage_LoadFile(L".\\skin\\image\\id2.bmp", true);
		XPic_SetImage(hE, hImg);

		hE = XWnd_GetEle(g_hWindow, IDP_ID2PIC);
		XEle_ShowEle(hE, true);

		hE = XWnd_GetEle(g_hWindow, IDG_ID2);	//����֤ͼ���
		XEle_ShowEle(hE, true);

		hE = XWnd_GetEle(g_hWindow, IDG_PASSPORT);	//�������
		XEle_ShowEle(hE, false);
	}
    ShowWindow(g_hCapWnd, true);
	RePlayCapture();//RePlay();
	
	XWnd_RedrawWnd(XEle_GetHWindow(hEle));

	return false;
}

//���յİ�ť��Ӧ
BOOL CALLBACK OnTakeVisitorPic(HELE hEle, HELE hEventEle)
{
	if (hEle != hEventEle) 
	{
		return false;
	}

	char szPic[128] = {'\0'};
	strcpy(szPic, USER_VIDEO_PIC);
	//XBtn_GetText(hEle);
    if(wcscmp(XBtn_GetText(hEle), L"����") == 0 || wcscmp(XBtn_GetText(hEle), L"���ձȶ�") == 0)
	{
		int nLen = MultiByteToWideChar(CP_ACP, 0, szPic, -1, NULL, 0);
		wchar_t *wBuf = new wchar_t[nLen + 1];
		MultiByteToWideChar(CP_ACP, 0, szPic, -1, wBuf, nLen);	
		
		CreateThread(NULL, 0, FlashFrameThread, wBuf, 0, NULL);
        Sleep(500);
		TakePhoto2(wBuf);	

		delete []wBuf;
		HWND hWnd = ::FindWindow(NULL, "Face");
		if(hWnd)
		{
			PostMessage(hWnd, 10000, 1, 0);
		}
		XBtn_SetText(hEle, L"����");
	}
	else
	{
		HELE hE = XWnd_GetEle(g_hWindow, IDP_CAMERA);
		XPic_SetImage(hE, NULL);
		::DeleteFile(szPic);

		hE = XWnd_GetEle(g_hWindow, IDP_HEADPIC2);		
	    XPic_SetImage(hE, NULL);
		::DeleteFile(USER_HEAD_PIC);

		ShowWindow(g_hCapWnd, true);
		RePlayCapture();//RePlay();
		/*BOOL b = VideoPreview();				//������ƵԤ��
		if (!b)
		{
			XMessageBox(g_hWindow, L"��������ͷʧ��", L"����");
		}*/
		XBtn_SetText(hEle, L"����");
		XWnd_RedrawWnd(XEle_GetHWindow(hEle));
	}

    /**/
	return false;
}

//����ÿ͵Ǽ�ҳ���ϡ���Ƶ���ա���ť���¼���Ӧ
BOOL CALLBACK OnTakePhoto(HELE hEle, HELE hEventEle)
{
	if (hEle != hEventEle) 
	{
		return false;
	}

	EnableButton(XEle_GetId(hEle));

	char szPic[128] = {'\0'};
	strcpy(szPic, USER_VIDEO_PIC);

	if (wcscmp(XBtn_GetText(hEle), L"��Ƶ����") == 0)
	{
		int nLen = MultiByteToWideChar(CP_ACP, 0, szPic, -1, NULL, 0);
		wchar_t *wBuf = new wchar_t[nLen + 1];
		MultiByteToWideChar(CP_ACP, 0, szPic, -1, wBuf, nLen);

		TakePhoto(wBuf);

		delete []wBuf;

		XBtn_SetText(hEle, L"ͼ��ɼ�");
	}
	else
	{
		HELE hE = XWnd_GetEle(g_hWindow, IDP_CAMERA);
		XPic_SetImage(hE, NULL);
		::DeleteFile(szPic);

		BOOL b = VideoPreview();				//������ƵԤ��
		if (!b)
		{
			XMessageBox(g_hWindow, L"��������ͷʧ��", L"����");
		}
		XBtn_SetText(hEle, L"��Ƶ����");
	}

	XWnd_RedrawWnd(XEle_GetHWindow(hEle));

	return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//ɨ���ʻ֤
BOOL CALLBACK OnScanDriverLicense(HELE hEle, HELE hEventEle)
{

	int ll = CheckIsReg();
	if(ll != 0)
	{
		//XMessageBox(L"�������������к�",L"��ʾ"); 
		//MessageBox(NULL,"���ǵ����û��ܺ��ߣ���������Ա��ȡ�������к�","����",MB_ICONEXCLAMATION);
		//HideTaskBar(false);
		//ExecPro(".\\Reg.exe", true, NULL);
		XMessageBox(g_hWindow,L"���ð治֧�ּ�ʻ֤ɨ�裬����ע�ᣬ����֧�־������ǵĶ�����лл֧�֣�",L"��ʾ");
		return false;
	}

	if (hEle != hEventEle)
	{
		return false;
	}
    SuspendThread(g_HWDThreadID2ID);
	EnableScanBtns(false);
	SetVisitorReadOnly(false);
	XEdit_SetReadOnly(XWnd_GetEle(g_hWindow, 5023),false);
	InitVisitorData();
	
	
	HELE hPic = XWnd_GetEle(g_hWindow, IDP_PASSPORTPIC);	//��ʼ�����û���ͼƬ��ʾΪ�հ�
	XPic_SetImage(hPic, NULL);
	hPic = XWnd_GetEle(g_hWindow, IDP_HEADPIC);	//��ʼ������ͷ��ͼƬ��ʾΪ�հ�
	XPic_SetImage(hPic, NULL);

	long ret = 0;
	char szUserPic[256] = {'\0'}, szIDHeadPic[256] = {'\0'};
	Scan_Para	sP, tP;
	IDCARD_ALL stIDCard;
	memset((char *)&stIDCard, 0x0, sizeof(IDCARD_ALL));
	sP.left = 0;
	sP.top = 0;
	sP.right = 300;
	sP.bottom = 200;
	sP.scan_mode = SCAN_IR_MODE; 
	sP.brightness = 0;
	sP.contrast = 0;
	sP.resolution = SCAN_300_DPI;
	sP.gamma = 10;
	sP.highlight = 0;
	sP.qualityscan = 1;
	sP.inversion   = 0;
	
	g_nIDType = ID_TYPE_DriverLicense;

	EmptyVisitorPara();
	XWnd_RedrawWnd(g_hWindow,TRUE);
	g_nID2ReadType = ID2_NO;

	strcpy(szUserPic, USER_ID2_PIC);
	strcpy(szIDHeadPic,   USER_ID_PIC);
	//ret = RecogNewIdcardALL(szUserPic, stIDCard, false, INFO_ALL_2, NULL);
	if(2 ==::GetPrivateProfileInt("TerminalSet", "ZT",0, CONFIG_FILE))
		sP.scan_mode = SCAN_COLOR_MODE; 
	else
		sP.scan_mode = 4;//SCAN_COLOR_MODE; 
	memcpy(&tP, &sP, sizeof(Scan_Para));
	tP.scan_mode = SCAN_COLOR_MODE; 


	if(1 ==::GetPrivateProfileInt("TerminalSet", "ZT",0, CONFIG_FILE))
	{
		ret = GetTcpCardScan(g_nIDType,stIDCard);
		if(ret == 0)
		{
			::CopyFile(".\\ZT\\test.bmp", szUserPic, false);
			::DeleteFile(".\\ZT\\test.bmp");
			::CopyFile(".\\ZT\\test_Head.bmp", szIDHeadPic, false);
			::DeleteFile(".\\ZT\\test_Head.bmp");
		}
	}
	else 
	{
		if(2 ==::GetPrivateProfileInt("TerminalSet", "ZT",0, CONFIG_FILE))
		{
			ret = ScanAndRcgCard(szUserPic, "", szIDHeadPic, stIDCard, sP, tP, SCAN_DriverLicense, 0, 0, 0, 15);
			ret = GetTcpCardScan(g_nIDType,stIDCard);
			//
		}
		else
			ret = ScanAndRcgCard(szUserPic, "", szIDHeadPic, stIDCard, sP, tP, SCAN_DriverLicense, 0, 1, 1, 15);
	}
	//ret = ScanImageByUser(szUserPic, szIDHeadPic, stIDCard, sP, tP, SCAN_BusinessCard, 0, 1, 1, 15);

	
    //SetTimer(XWnd_GetHWnd(g_hWindow),2,300,TimerMsgBox);
	if (ret == 0 || ret == 3)
	{		
		if (ret == 0)
		{	
			if(strlen(stIDCard.name) < 1 && strlen(stIDCard.SurnameCH) > 0)
				strcpy(stIDCard.name,stIDCard.SurnameCH);

			SetVisitorInfo(stIDCard);
		}

	//	XComboBox_SetSelectItem(XWnd_GetEle(g_hWindow, IDC_VISITORIDTYPE), 0); //����֤��Ͽ���ʾ
        XEdit_SetReadOnly(XWnd_GetEle(g_hWindow, IDC_VISITORIDTYPE),false);
		XEdit_SetText(XWnd_GetEle(g_hWindow, IDC_VISITORIDTYPE), L"��ʻ֤");

		//SetInfo(IDS_DEVINFO, L"�������֤ɨ�����");
		//int nLen = MultiByteToWideChar(CP_ACP, 0, szUserPic, -1, NULL, 0);
		wchar_t wBuf[256] = {0};
		//MultiByteToWideChar(CP_ACP, 0, szUserPic, -1, wBuf, nLen);
        S_2_W(wBuf, szUserPic, 256);
		HIMAGE hImg = XImage_LoadFile(wBuf, true);
		HELE hPic = XWnd_GetEle(g_hWindow, IDP_PASSPORTPIC);
		XPic_SetImage(hPic, hImg);

		//wchar_t wBuf[256] = {0};
	    S_2_W(wBuf, szIDHeadPic, 256);
	    hImg = XImage_LoadFile(wBuf, true);
	    hPic = XWnd_GetEle(g_hWindow, IDP_HEADPIC);
	    XPic_SetImage(hPic, hImg);

		//delete []wBuf;
		SetVisitorReadOnly(false);
		::WritePrivateProfileString("TerminalSet", "DataInputPage", "2", CONFIG_FILE);
		//StartKeybd();
	}
	else
	{
		EnableScanBtns(true);
		XMessageBox(g_hWindow,L"ɨ��ʧ��",L"��ʾ");
		ResumeThread(g_HWDThreadID2ID);
		XWnd_RedrawWnd(g_hWindow);
		return false;
	}	
	::DeleteFile(USER_VIDEO_PIC);
	XBtn_SetText(g_hTab1, L"���ձȶ�");
	//::WritePrivateProfileString("Info", "bStopRead", "1", ".\\FaceCompare\\faceCompare.ini");
	g_bStopCompare = false;
	EnableScanBtns(true);
    XWnd_RedrawWnd(g_hWindow);
	ShowMainMenu(g_hPage2, true);
	ResumeThread(g_HWDThreadID2ID);
	return false;
}
//ɨ����Ƭ
BOOL CALLBACK OnScanBusinessCard(HELE hEle, HELE hEventEle)
{
	if (hEle != hEventEle)
	{
		return false;
	}
    SuspendThread(g_HWDThreadID2ID);
	EnableScanBtns(false);
	SetVisitorReadOnly(false);
	XEdit_SetReadOnly(XWnd_GetEle(g_hWindow, 5023),false);
	InitVisitorData();
	
	
	HELE hPic = XWnd_GetEle(g_hWindow, IDP_PASSPORTPIC);	//��ʼ�����û���ͼƬ��ʾΪ�հ�
	XPic_SetImage(hPic, NULL);
	hPic = XWnd_GetEle(g_hWindow, IDP_HEADPIC);	//��ʼ������ͷ��ͼƬ��ʾΪ�հ�
	XPic_SetImage(hPic, NULL);

	long ret = 0;
	char szUserPic[256] = {'\0'}, szIDHeadPic[256] = {'\0'};
	Scan_Para	sP, tP;
	IDCARD_ALL stIDCard;
	memset((char *)&stIDCard, 0x0, sizeof(IDCARD_ALL));
	sP.left = 0;
	sP.top = 0;
	sP.right = 300;
	sP.bottom = 200;
	sP.scan_mode = SCAN_IR_MODE; 
	sP.brightness = 0;
	sP.contrast = 0;
	sP.resolution = SCAN_300_DPI;
	sP.gamma = 10;
	sP.highlight = 0;
	sP.qualityscan = 1;
	sP.inversion   = 0;
	
	g_nIDType = ID_TYPE_BusinessCard;

	EmptyVisitorPara();
	XWnd_RedrawWnd(g_hWindow,TRUE);
	g_nID2ReadType = ID2_NO;

	strcpy(szUserPic, USER_ID2_PIC);
	strcpy(szIDHeadPic,   USER_ID_PIC);
	//ret = RecogNewIdcardALL(szUserPic, stIDCard, false, INFO_ALL_2, NULL);
	sP.scan_mode = 4;//SCAN_COLOR_MODE; 
	memcpy(&tP, &sP, sizeof(Scan_Para));
	tP.scan_mode = SCAN_COLOR_MODE; 


	ret = ScanAndRcgCard(szUserPic, "", szIDHeadPic, stIDCard, sP, tP, SCAN_BusinessCard, 0, 1, 1, 15);
	//ret = ScanImageByUser(szUserPic, szIDHeadPic, stIDCard, sP, tP, SCAN_BusinessCard, 0, 1, 1, 15);

	
    //SetTimer(XWnd_GetHWnd(g_hWindow),2,300,TimerMsgBox);
	if (ret == 0 || ret == 3)
	{		
		if (ret == 0)
		{	
			if(strlen(stIDCard.name) < 1 && strlen(stIDCard.SurnameCH) > 0)
				strcpy(stIDCard.name,stIDCard.SurnameCH);

			SetVisitorInfo(stIDCard);
		}

	//	XComboBox_SetSelectItem(XWnd_GetEle(g_hWindow, IDC_VISITORIDTYPE), 0); //����֤��Ͽ���ʾ
        XEdit_SetReadOnly(XWnd_GetEle(g_hWindow, IDC_VISITORIDTYPE),false);
		XEdit_SetText(XWnd_GetEle(g_hWindow, IDC_VISITORIDTYPE), L"��Ƭ");

		//SetInfo(IDS_DEVINFO, L"�������֤ɨ�����");
		//int nLen = MultiByteToWideChar(CP_ACP, 0, szUserPic, -1, NULL, 0);
		wchar_t wBuf[256] = {0};
		//MultiByteToWideChar(CP_ACP, 0, szUserPic, -1, wBuf, nLen);
        S_2_W(wBuf, szUserPic, 256);
		HIMAGE hImg = XImage_LoadFile(wBuf, true);
		HELE hPic = XWnd_GetEle(g_hWindow, IDP_PASSPORTPIC);
		XPic_SetImage(hPic, hImg);

		//wchar_t wBuf[256] = {0};
	    S_2_W(wBuf, szIDHeadPic, 256);
	    hImg = XImage_LoadFile(wBuf, true);
	    hPic = XWnd_GetEle(g_hWindow, IDP_HEADPIC);
	    XPic_SetImage(hPic, hImg);

		//delete []wBuf;
		SetVisitorReadOnly(false);
		::WritePrivateProfileString("TerminalSet", "DataInputPage", "2", CONFIG_FILE);
		//StartKeybd();
	}
	else
	{
		EnableScanBtns(true);
		XMessageBox(g_hWindow,L"ɨ��ʧ��",L"��ʾ");
		ResumeThread(g_HWDThreadID2ID);
		XWnd_RedrawWnd(g_hWindow);
		return false;
	}	
	EnableScanBtns(true);
    XWnd_RedrawWnd(g_hWindow);
	ShowMainMenu(g_hPage2, true);
	ResumeThread(g_HWDThreadID2ID);
	return false;
}
//ɨ�����֤
BOOL CALLBACK OnScanID2(HELE hEle, HELE hEventEle)
{
	if (hEle != hEventEle)
	{
		return false;
	}
    SuspendThread(g_HWDThreadID2ID);
	EnableScanBtns(false);
	SetVisitorReadOnly(false);
	XEdit_SetReadOnly(XWnd_GetEle(g_hWindow, 5023),false);
	InitVisitorData();
	
	
	HELE hPic = XWnd_GetEle(g_hWindow, IDP_PASSPORTPIC);	//��ʼ�����û���ͼƬ��ʾΪ�հ�
	XPic_SetImage(hPic, NULL);
	hPic = XWnd_GetEle(g_hWindow, IDP_HEADPIC);	//��ʼ������ͷ��ͼƬ��ʾΪ�հ�
	XPic_SetImage(hPic, NULL);

	long ret = 0;
	char szUserPic[256] = {'\0'}, szIDHeadPic[256] = {'\0'};
	Scan_Para	sP, tP;
	IDCARD_ALL stIDCard;
	memset((char *)&stIDCard, 0x0, sizeof(IDCARD_ALL));
	sP.left = 0;
	sP.top = 0;
	sP.right = 300;
	sP.bottom = 200;
	sP.scan_mode = SCAN_IR_MODE; 
	sP.brightness = 0;
	sP.contrast = 0;
	sP.resolution = SCAN_300_DPI;
	sP.gamma = 10;
	sP.highlight = 0;
	sP.qualityscan = 1;
	sP.inversion   = 0;
	
	g_nIDType = ID_TYPE_ID2;

	EmptyVisitorPara();
	XWnd_RedrawWnd(g_hWindow,TRUE);
	g_nID2ReadType = ID2_SCAN;

	strcpy(szUserPic, USER_ID2_PIC);
	strcpy(szIDHeadPic,   USER_ID_PIC);
	//ret = RecogNewIdcardALL(szUserPic, stIDCard, false, INFO_ALL_2, NULL);
	sP.scan_mode = SCAN_COLOR_MODE; 
	memcpy(&tP, &sP, sizeof(Scan_Para));
	tP.scan_mode = SCAN_COLOR_MODE; 

	if(1 ==::GetPrivateProfileInt("TerminalSet", "ZT",0, CONFIG_FILE))
	{
		ret = GetTcpCardScan(g_nIDType,stIDCard);
		if(ret == 0)
		{
			::CopyFile(".\\ZT\\test.bmp", szUserPic, false);
			::DeleteFile(".\\ZT\\test.bmp");
			::CopyFile(".\\ZT\\test_Head.bmp", szIDHeadPic, false);
			::DeleteFile(".\\ZT\\test_Head.bmp");
		}
	}
	else if (::GetPrivateProfileInt("TerminalSet", "FS533",0, CONFIG_FILE)==1) 
	{
		ret = CallFS533Bin("id2");
		if(ret == 1)
		{
			::GetPrivateProfileString("Scan", "Name", "", stIDCard.name, sizeof(stIDCard.name), FS533Scan_FILE);
			::GetPrivateProfileString("Scan", "Number", "", stIDCard.number, sizeof(stIDCard.number), FS533Scan_FILE);
			::GetPrivateProfileString("Scan", "Sex", "", stIDCard.sex, sizeof(stIDCard.sex), FS533Scan_FILE);
			::GetPrivateProfileString("Scan", "Address", "", stIDCard.address, sizeof(stIDCard.address), FS533Scan_FILE);
			::GetPrivateProfileString("Scan", "People", "", stIDCard.people, sizeof(stIDCard.people), FS533Scan_FILE);
			ret = 0;
		}
		else
		{
			ret = -1;
		}
	}
	else
	{
		ret = ScanAndRcgCard(szUserPic, "", szIDHeadPic, stIDCard, sP, tP, SCAN_ID2, 0, 1, 1, 15);
	}
	
    //SetTimer(XWnd_GetHWnd(g_hWindow),2,300,TimerMsgBox);
	if (ret == 0 || ret == 3)
	{		
		if (ret == 0)
		{
			SetVisitorInfo(stIDCard);
		}

	//	XComboBox_SetSelectItem(XWnd_GetEle(g_hWindow, IDC_VISITORIDTYPE), 0); //����֤��Ͽ���ʾ
        XEdit_SetReadOnly(XWnd_GetEle(g_hWindow, IDC_VISITORIDTYPE),false);
		XEdit_SetText(XWnd_GetEle(g_hWindow, IDC_VISITORIDTYPE), L"�������֤");

		//SetInfo(IDS_DEVINFO, L"�������֤ɨ�����");
		//int nLen = MultiByteToWideChar(CP_ACP, 0, szUserPic, -1, NULL, 0);
		wchar_t wBuf[256] = {0};
		//MultiByteToWideChar(CP_ACP, 0, szUserPic, -1, wBuf, nLen);
        S_2_W(wBuf, szUserPic, 256);
		HIMAGE hImg = XImage_LoadFile(wBuf, true);
		HELE hPic = XWnd_GetEle(g_hWindow, IDP_PASSPORTPIC);
		XPic_SetImage(hPic, hImg);

		//wchar_t wBuf[256] = {0};
	    S_2_W(wBuf, szIDHeadPic, 256);
	    hImg = XImage_LoadFile(wBuf, true);
	    hPic = XWnd_GetEle(g_hWindow, IDP_HEADPIC);
	    XPic_SetImage(hPic, hImg);

		//delete []wBuf;

		::WritePrivateProfileString("TerminalSet", "DataInputPage", "2", CONFIG_FILE);
		//StartKeybd();
	}
	else
	{
		EnableScanBtns(true);
		XMessageBox(g_hWindow,L"ɨ��ʧ��",L"��ʾ");
		ResumeThread(g_HWDThreadID2ID);
		XWnd_RedrawWnd(g_hWindow);
		return false;
	}	
	::DeleteFile(USER_VIDEO_PIC);
	XBtn_SetText(g_hTab1, L"���ձȶ�");
	//::WritePrivateProfileString("Info", "bStopRead", "1", ".\\FaceCompare\\faceCompare.ini");
	g_bStopCompare = false;
	EnableScanBtns(true);
    XWnd_RedrawWnd(g_hWindow);
	ShowMainMenu(g_hPage2, true);
	ResumeThread(g_HWDThreadID2ID);

//�Ž�IC����Ż�ȡ
	if (::GetPrivateProfileInt("TerminalSet", "bMJConnetion",0, CONFIG_FILE)==1) 
	{
		if (g_ThreadID2IC)
		{
			SuspendThread(g_ThreadID2IC);
			CloseHandle(g_ThreadID2IC);
			g_ThreadID2IC = NULL;
		}
		if (g_hCom)
		{
			CloseHandle(g_hCom);
			g_hCom = NULL;
		}
		g_ThreadID2IC = CreateThread(NULL, 0, ReadICThread, NULL, 0, NULL);
	}


	return false;
}

//ɨ��һ�����֤
BOOL CALLBACK OnScanID(HELE hEle, HELE hEventEle)
{
	if (hEle != hEventEle) 
	{
		return false;
	}
    SuspendThread(g_HWDThreadID2ID);
	EnableScanBtns(false);
	SetVisitorReadOnly(false);
	XEdit_SetReadOnly(XWnd_GetEle(g_hWindow, 5023),false);
	InitVisitorData();
    //XWnd_RedrawWnd(g_hWindow,TRUE);
	HELE hPic = XWnd_GetEle(g_hWindow, IDP_PASSPORTPIC);	//��ʼ�����û���ͼƬ��ʾΪ�հ�
	XPic_SetImage(hPic, NULL);
	hPic = XWnd_GetEle(g_hWindow, IDP_HEADPIC);	//��ʼ������ͷ��ͼƬ��ʾΪ�հ�
	XPic_SetImage(hPic, NULL);

	long ret = 0;
	char szUserPic[256] = {'\0'}, szIDHeadPic[256] = {'\0'};
	Scan_Para	sP, tP;
	IDCARD_ALL stIDCard;
	memset((char *)&stIDCard, 0x0, sizeof(IDCARD_ALL));
	sP.left = 0;
	sP.top = 0;
	sP.right = 300;
	sP.bottom = 200;
	sP.scan_mode = SCAN_IR_MODE; 
	sP.brightness = 0;
	sP.contrast = 0;
	sP.resolution = SCAN_300_DPI;
	sP.gamma = 10;
	sP.highlight = 0;
	sP.qualityscan = 1;
	sP.inversion   = 0;

	g_nID2ReadType = ID2_NO;
	g_nIDType = ID_TYPE_ID;
	EmptyVisitorPara();
	XWnd_RedrawWnd(g_hWindow,TRUE);

	strcpy(szUserPic, USER_ID2_PIC);
	strcpy(szIDHeadPic,   USER_ID_PIC);
	sP.scan_mode = SCAN_GRAY_MODE; 
	memcpy(&tP, &sP, sizeof(Scan_Para));
	tP.scan_mode = SCAN_COLOR_MODE; 

	if (::GetPrivateProfileInt("TerminalSet", "FS533",0, CONFIG_FILE)==1) 
	{
		ret = CallFS533Bin("id");
		if(ret == 1)
		{
			::GetPrivateProfileString("Scan", "Name", "", stIDCard.name, sizeof(stIDCard.name), FS533Scan_FILE);
			::GetPrivateProfileString("Scan", "Number", "", stIDCard.number, sizeof(stIDCard.number), FS533Scan_FILE);
			::GetPrivateProfileString("Scan", "Sex", "", stIDCard.sex, sizeof(stIDCard.sex), FS533Scan_FILE);
			::GetPrivateProfileString("Scan", "Address", "", stIDCard.address, sizeof(stIDCard.address), FS533Scan_FILE);
			::GetPrivateProfileString("Scan", "People", "", stIDCard.people, sizeof(stIDCard.people), FS533Scan_FILE);
			ret = 0;
		}
		else
		{
			ret = -1;
		}
	}
	else
		ret = ScanAndRcgCard(szUserPic, "", szIDHeadPic, stIDCard, sP, tP, SCAN_ID1, 1, 1, 1, 15);
	//ret = ScanAndRcgCard(szUserPic, "", szIDHeadPic, stIDCard, sP, tP, 9, 1, 1, 1, 15);
	if (ret == 0||ret == 50)
	{
		EmptyVisitorPara();
		//XComboBox_SetSelectItem(XWnd_GetEle(g_hWindow, IDC_VISITORIDTYPE), 1);
		XEdit_SetReadOnly(XWnd_GetEle(g_hWindow, IDC_VISITORIDTYPE),false);
		XEdit_SetText(XWnd_GetEle(g_hWindow, IDC_VISITORIDTYPE), L"һ�����֤"); //һ�����֤
		SetVisitorInfo(stIDCard);
		
		
		wchar_t wBuf[256] = {0};
		//MultiByteToWideChar(CP_ACP, 0, szUserPic, -1, wBuf, nLen);
        S_2_W(wBuf, szUserPic, 256);
		HIMAGE hImg = XImage_LoadFile(wBuf, true);
		HELE hPic = XWnd_GetEle(g_hWindow, IDP_PASSPORTPIC);
		XPic_SetImage(hPic, hImg);

		//wchar_t wBuf[256] = {0};
	    S_2_W(wBuf, szIDHeadPic, 256);
	    hImg = XImage_LoadFile(wBuf, true);
	    hPic = XWnd_GetEle(g_hWindow, IDP_HEADPIC);
	    XPic_SetImage(hPic, hImg);
		
		::WritePrivateProfileString("TerminalSet", "DataInputPage", "2", CONFIG_FILE);
		//StartKeybd();
	}
	else
	{
		EnableScanBtns(true);
		XMessageBox(g_hWindow,L"ɨ��ʧ��",L"��ʾ");
		ResumeThread(g_HWDThreadID2ID);
		XWnd_RedrawWnd(g_hWindow);
		return false;
	}


	EnableScanBtns(true);
	ShowMainMenu(g_hPage2, true);
	XWnd_RedrawWnd(g_hWindow);
	ResumeThread(g_HWDThreadID2ID);
	return false;
}

//ɨ�軤��
BOOL CALLBACK OnScanPassPort(HELE hEle, HELE hEventEle)
{
	if (hEle != hEventEle) 
	{
		return false;
	}

    SuspendThread(g_HWDThreadID2ID);
	EnableScanBtns(false);
	SetVisitorReadOnly(false);
	XEdit_SetReadOnly(XWnd_GetEle(g_hWindow, 5023),false);
	//InitVisitorData();
	EmptyVisitorPara();
    
	HELE hPic = XWnd_GetEle(g_hWindow, IDP_PASSPORTPIC);	//��ʼ�����û���ͼƬ��ʾΪ�հ�
	XPic_SetImage(hPic, NULL);
	hPic = XWnd_GetEle(g_hWindow, IDP_HEADPIC);	//��ʼ������ͷ��ͼƬ��ʾΪ�հ�
	XPic_SetImage(hPic, NULL);

	long ret = 0;
	char szUserPic[256] = {'\0'}, szIDHeadPic[256] = {'\0'};
	Scan_Para	sP, tP;
	IDCARD_ALL stIDCard;
	memset((char *)&stIDCard, 0x0, sizeof(IDCARD_ALL));
	sP.left = 0;
	sP.top = 0;
	sP.right = 300;
	sP.bottom = 200;
	sP.scan_mode = SCAN_IR_MODE; 
	sP.brightness = 0;
	sP.contrast = 0;
	sP.resolution = SCAN_300_DPI;
	sP.gamma = 10;
	sP.highlight = 0;
	sP.qualityscan = 1;
	sP.inversion   = 0;

	g_nID2ReadType = ID2_NO;
	g_nIDType = ID_TYPE_PASSPORT;
	EmptyVisitorPara();
	XWnd_RedrawWnd(g_hWindow,TRUE);

	strcpy(szUserPic, USER_ID2_PIC);
//	ret = RecogPassPortALL(szUserPic, stIDCard, false, INFO_ALL_1, NULL);
	strcpy(szIDHeadPic,   USER_ID_PIC);
	//sP.scan_mode = SCAN_GRAY_MODE; 
	if(2 ==::GetPrivateProfileInt("TerminalSet", "ZT",0, CONFIG_FILE))
		sP.scan_mode = SCAN_COLOR_MODE; 
	else
		sP.scan_mode = SCAN_GRAY_MODE;//SCAN_COLOR_MODE; 
	memcpy(&tP, &sP, sizeof(Scan_Para));
	tP.scan_mode = SCAN_COLOR_MODE; 
	if(1 ==::GetPrivateProfileInt("TerminalSet", "ZT",0, CONFIG_FILE))
	{
		ret = GetTcpCardScan(g_nIDType,stIDCard);
		if(ret == 0)
		{
			::CopyFile(".\\ZT\\test.bmp", szUserPic, false);
			::DeleteFile(".\\ZT\\test.bmp");
			::CopyFile(".\\ZT\\test_Head.bmp", szIDHeadPic, false);
			::DeleteFile(".\\ZT\\test_Head.bmp");
		}
	}
	else if (::GetPrivateProfileInt("TerminalSet", "FS533",0, CONFIG_FILE)==1) 
	{
		ret = CallFS533Bin("RecogPassPortALL");
		if(ret == 1)
		{
			::GetPrivateProfileString("Scan", "Name", "", stIDCard.name, sizeof(stIDCard.name), FS533Scan_FILE);
			::GetPrivateProfileString("Scan", "Number", "", stIDCard.number, sizeof(stIDCard.number), FS533Scan_FILE);
			::GetPrivateProfileString("Scan", "Sex", "", stIDCard.sex, sizeof(stIDCard.sex), FS533Scan_FILE);
			::GetPrivateProfileString("Scan", "Address", "", stIDCard.address, sizeof(stIDCard.address), FS533Scan_FILE);
			::GetPrivateProfileString("Scan", "People", "", stIDCard.people, sizeof(stIDCard.people), FS533Scan_FILE);
			ret = 0;
		}
		else
		{
			ret = -1;
		}
	}
	else
	{
		if(2 ==::GetPrivateProfileInt("TerminalSet", "ZT",0, CONFIG_FILE))
		{
			ret = ScanAndRcgCard(szUserPic, "", szIDHeadPic, stIDCard, sP, tP, SCAN_PASSPORT, 0, 0, 0, 15);
			ret = GetTcpCardScan(g_nIDType,stIDCard);
			//
		}
		else
			ret = ScanAndRcgCard(szUserPic, "", szIDHeadPic, stIDCard, sP, tP, SCAN_PASSPORT, 0, 1, 1, 15);
	}
		//ret = ScanAndRcgCard(szUserPic, "", szIDHeadPic, stIDCard, sP, tP, SCAN_PASSPORT, 1, 1, 1, 15);
	if (ret == 0 || ret == 50)
	{
		//EmptyVisitorPara();
		//XComboBox_SetSelectItem(XWnd_GetEle(g_hWindow, IDC_VISITORIDTYPE), 2);
		XEdit_SetReadOnly(XWnd_GetEle(g_hWindow, IDC_VISITORIDTYPE),false);
		XEdit_SetText(XWnd_GetEle(g_hWindow, IDC_VISITORIDTYPE), L"����");

		SetVisitorInfo(stIDCard);	
		wchar_t wBuf[256] = {0};
		//MultiByteToWideChar(CP_ACP, 0, szUserPic, -1, wBuf, nLen);
        S_2_W(wBuf, szUserPic, 256);
		HIMAGE hImg = XImage_LoadFile(wBuf, true);
		HELE hPic = XWnd_GetEle(g_hWindow, IDP_PASSPORTPIC);
		XPic_SetImage(hPic, hImg);

		//wchar_t wBuf[256] = {0};
	    S_2_W(wBuf, szIDHeadPic, 256);
	    hImg = XImage_LoadFile(wBuf, true);
	    hPic = XWnd_GetEle(g_hWindow, IDP_HEADPIC);
	    XPic_SetImage(hPic, hImg);
		
		::WritePrivateProfileString("TerminalSet", "DataInputPage", "2", CONFIG_FILE);
		//StartKeybd();
	}
	else
	{
		EnableScanBtns(true);
		XMessageBox(g_hWindow,L"ɨ��ʧ��",L"��ʾ");
		ResumeThread(g_HWDThreadID2ID);
		XWnd_RedrawWnd(g_hWindow);
		return false;
	}
	::DeleteFile(USER_VIDEO_PIC);
	XBtn_SetText(g_hTab1, L"���ձȶ�");
	//::WritePrivateProfileString("Info", "bStopRead", "1", ".\\FaceCompare\\faceCompare.ini");
	g_bStopCompare = false;
	EnableScanBtns(true);
	XWnd_RedrawWnd(g_hWindow);
	ShowMainMenu(g_hPage2, true);
	ResumeThread(g_HWDThreadID2ID);
	return false;
}

//ɨ��۰�ͨ��֤
BOOL CALLBACK OnScanToHKPassPort(HELE hEle, HELE hEventEle)
{
	if (hEle != hEventEle) 
	{
		return false;
	}

    SuspendThread(g_HWDThreadID2ID);
	EnableScanBtns(false);
	SetVisitorReadOnly(false);
	XEdit_SetReadOnly(XWnd_GetEle(g_hWindow, 5023),false);
	InitVisitorData();
	//XWnd_RedrawWnd(g_hWindow,TRUE);
	HELE hPic = XWnd_GetEle(g_hWindow, IDP_PASSPORTPIC);	//��ʼ�����û���ͼƬ��ʾΪ�հ�
	XPic_SetImage(hPic, NULL);
	hPic = XWnd_GetEle(g_hWindow, IDP_HEADPIC);	//��ʼ������ͷ��ͼƬ��ʾΪ�հ�
	XPic_SetImage(hPic, NULL);

	long ret = 0;
	char szUserPic[256] = {'\0'}, szIDHeadPic[256] = {'\0'};
	Scan_Para	sP, tP;
	IDCARD_ALL stIDCard;
	memset((char *)&stIDCard, 0x0, sizeof(IDCARD_ALL));
	sP.left = 0;
	sP.top = 0;
	sP.right = 300;
	sP.bottom = 200;
	sP.scan_mode = SCAN_IR_MODE; 
	sP.brightness = 0;
	sP.contrast = 0;
	sP.resolution = SCAN_300_DPI;
	sP.gamma = 10;
	sP.highlight = 0;
	sP.qualityscan = 1;
	sP.inversion   = 0;
	g_nID2ReadType = ID2_NO;
	g_nIDType = ID_TYPE_TOHKPASSPORT;
	EmptyVisitorPara();
	XWnd_RedrawWnd(g_hWindow,TRUE);	
	strcpy(szUserPic, USER_ID2_PIC);
//	ret = RecogPassPortALL(szUserPic, stIDCard, false, INFO_ALL_1, NULL);
	strcpy(szIDHeadPic,   USER_ID_PIC);
	sP.scan_mode = SCAN_GRAY_MODE; 
	memcpy(&tP, &sP, sizeof(Scan_Para));
	tP.scan_mode = SCAN_COLOR_MODE; 
	if (::GetPrivateProfileInt("TerminalSet", "FS533",0, CONFIG_FILE)==1) 
	{
		ret = CallFS533Bin("RecogPassPortALL");
		if(ret == 1)
		{
			::GetPrivateProfileString("Scan", "Name", "", stIDCard.name, sizeof(stIDCard.name), FS533Scan_FILE);
			::GetPrivateProfileString("Scan", "Number", "", stIDCard.number, sizeof(stIDCard.number), FS533Scan_FILE);
			::GetPrivateProfileString("Scan", "Sex", "", stIDCard.sex, sizeof(stIDCard.sex), FS533Scan_FILE);
			::GetPrivateProfileString("Scan", "Address", "", stIDCard.address, sizeof(stIDCard.address), FS533Scan_FILE);
			::GetPrivateProfileString("Scan", "People", "", stIDCard.people, sizeof(stIDCard.people), FS533Scan_FILE);
			ret = 0;
		}
		else
		{
			ret = -1;
		}
	}
	else
		ret = ScanAndRcgCard(szUserPic, "", szIDHeadPic, stIDCard, sP, tP, SCAN_ToHKTaiWanPasPot, 1, 1, 1, 15);
	if (ret == 0 || ret == 50)
	{
		EmptyVisitorPara();
		//XComboBox_SetSelectItem(XWnd_GetEle(g_hWindow, IDC_VISITORIDTYPE), 2);
		XEdit_SetReadOnly(XWnd_GetEle(g_hWindow, IDC_VISITORIDTYPE),false);
		XEdit_SetText(XWnd_GetEle(g_hWindow, IDC_VISITORIDTYPE), L"��½����۰�ͨ��֤");
		SetVisitorInfo(stIDCard);	
		wchar_t wBuf[256] = {0};
		//MultiByteToWideChar(CP_ACP, 0, szUserPic, -1, wBuf, nLen);
        S_2_W(wBuf, szUserPic, 256);
		HIMAGE hImg = XImage_LoadFile(wBuf, true);
		HELE hPic = XWnd_GetEle(g_hWindow, IDP_PASSPORTPIC);
		XPic_SetImage(hPic, hImg);

		//wchar_t wBuf[256] = {0};
	    S_2_W(wBuf, szIDHeadPic, 256);
	    hImg = XImage_LoadFile(wBuf, true);
	    hPic = XWnd_GetEle(g_hWindow, IDP_HEADPIC);
	    XPic_SetImage(hPic, hImg);
		::WritePrivateProfileString("TerminalSet", "DataInputPage", "2", CONFIG_FILE);
		//StartKeybd();
	}
	else
	{
		EnableScanBtns(true);
		XMessageBox(g_hWindow,L"ɨ��ʧ��",L"��ʾ");
		ResumeThread(g_HWDThreadID2ID);
		XWnd_RedrawWnd(g_hWindow);
		return false;
	}
	
	EnableScanBtns(true);
	XWnd_RedrawWnd(g_hWindow);
	ShowMainMenu(g_hPage2, true);
	ResumeThread(g_HWDThreadID2ID);
	return false;
}

//��ʾ֤��ɨ�谴ť�Ƿ���Ч
void EnableScanBtns(BOOL isEnable)
{
     XEle_EnableEle(g_hTab21, isEnable);
	 XEle_EnableEle(g_hTab22, isEnable);
	 XEle_EnableEle(g_hTab23, isEnable);
	 XEle_EnableEle(g_hTab24, isEnable);
	 XEle_EnableEle(g_hTab25, isEnable);
	 XEle_EnableEle(g_hTab26, isEnable);
	 XEle_EnableEle(g_hTab27, isEnable);
	 XEle_EnableEle(g_hTab28, isEnable);
}
//ɨ�����֤
//�������֤
BOOL CALLBACK OnScanHKID(HELE hEle, HELE hEventEle)
{
	if (hEle != hEventEle) 
	{
		return false;
	}
    SuspendThread(g_HWDThreadID2ID);
	EnableScanBtns(false);
	SetVisitorReadOnly(false);
	XEdit_SetReadOnly(XWnd_GetEle(g_hWindow, 5023),false);
	InitVisitorData();
	//XWnd_RedrawWnd(g_hWindow,TRUE);
	HELE hPic = XWnd_GetEle(g_hWindow, IDP_PASSPORTPIC);	//��ʼ�����û���ͼƬ��ʾΪ�հ�
	XPic_SetImage(hPic, NULL);
	hPic = XWnd_GetEle(g_hWindow, IDP_HEADPIC);	//��ʼ������ͷ��ͼƬ��ʾΪ�հ�
	XPic_SetImage(hPic, NULL);

	long ret = 0;
	char szUserPic[256] = {'\0'}, szIDHeadPic[256] = {'\0'};
	Scan_Para	sP, tP;
	IDCARD_ALL stIDCard;
	memset((char *)&stIDCard, 0x0, sizeof(IDCARD_ALL));
	sP.left = 0;
	sP.top = 0;
	sP.right = 300;
	sP.bottom = 200;
	sP.scan_mode = SCAN_IR_MODE; 
	sP.brightness = 0;
	sP.contrast = 0;
	sP.resolution = SCAN_300_DPI;
	sP.gamma = 10;
	sP.highlight = 0;
	sP.qualityscan = 1;
	sP.inversion   = 0;
	g_nID2ReadType = ID2_NO;
	g_nIDType = ID_TYPE_HKID;
	EmptyVisitorPara();
	XWnd_RedrawWnd(g_hWindow,TRUE);	
	strcpy(szUserPic, USER_ID2_PIC);
//	ret = RecogFXZALL(szUserPic, stIDCard, false, INFO_ALL_1, NULL);	
	strcpy(szIDHeadPic,   USER_ID_PIC);
	sP.scan_mode = SCAN_GRAY_MODE; 
	memcpy(&tP, &sP, sizeof(Scan_Para));
	tP.scan_mode = SCAN_COLOR_MODE; 
	if (::GetPrivateProfileInt("TerminalSet", "FS533",0, CONFIG_FILE)==1) 
	{
		ret = CallFS533Bin("RecogFXZALLL");
		if(ret == 1)
		{
			::GetPrivateProfileString("Scan", "Name", "", stIDCard.name, sizeof(stIDCard.name), FS533Scan_FILE);
			::GetPrivateProfileString("Scan", "Number", "", stIDCard.number, sizeof(stIDCard.number), FS533Scan_FILE);
			::GetPrivateProfileString("Scan", "Sex", "", stIDCard.sex, sizeof(stIDCard.sex), FS533Scan_FILE);
			::GetPrivateProfileString("Scan", "Address", "", stIDCard.address, sizeof(stIDCard.address), FS533Scan_FILE);
			::GetPrivateProfileString("Scan", "People", "", stIDCard.people, sizeof(stIDCard.people), FS533Scan_FILE);
			ret = 0;
		}
		else
		{
			ret = -1;
		}
	}
	else
		ret = ScanAndRcgCard(szUserPic, "", szIDHeadPic, stIDCard, sP, tP, SCAN_ForeResidencePermit, 1, 1, 1, 15);
	if (ret == 0 || ret == 50)
	{
		EmptyVisitorPara();
		//XComboBox_SetSelectItem(XWnd_GetEle(g_hWindow, IDC_VISITORIDTYPE), 2);
		XEdit_SetReadOnly(XWnd_GetEle(g_hWindow, IDC_VISITORIDTYPE),false);
		XEdit_SetText(XWnd_GetEle(g_hWindow, IDC_VISITORIDTYPE), L"�������֤");
		SetVisitorInfo(stIDCard);	
		wchar_t wBuf[256] = {0};
		//MultiByteToWideChar(CP_ACP, 0, szUserPic, -1, wBuf, nLen);
        S_2_W(wBuf, szUserPic, 256);
		HIMAGE hImg = XImage_LoadFile(wBuf, true);
		HELE hPic = XWnd_GetEle(g_hWindow, IDP_PASSPORTPIC);
		XPic_SetImage(hPic, hImg);

		//wchar_t wBuf[256] = {0};
	    S_2_W(wBuf, szIDHeadPic, 256);
	    hImg = XImage_LoadFile(wBuf, true);
	    hPic = XWnd_GetEle(g_hWindow, IDP_HEADPIC);
	    XPic_SetImage(hPic, hImg);
		
		::WritePrivateProfileString("TerminalSet", "DataInputPage", "2", CONFIG_FILE);
		//StartKeybd();
	}
	else
	{
		EnableScanBtns(true);
		XMessageBox(g_hWindow,L"ɨ��ʧ��",L"��ʾ");
		ResumeThread(g_HWDThreadID2ID);
		XWnd_RedrawWnd(g_hWindow);
		return false;
	}
	
	ShowMainMenu(g_hPage2, true);
	EnableScanBtns(true);
	XWnd_RedrawWnd(g_hWindow);
	ResumeThread(g_HWDThreadID2ID);
	return false;
}

//ɨ��̨��֤
BOOL CALLBACK OnScanTaiWanPassPort(HELE hEle, HELE hEventEle)
{
	if (hEle != hEventEle) 
	{
		return false;
	}
    SuspendThread(g_HWDThreadID2ID);
	EnableScanBtns(false);
	SetVisitorReadOnly(false);
	XEdit_SetReadOnly(XWnd_GetEle(g_hWindow, 5023),false);
	InitVisitorData();
	//XWnd_RedrawWnd(g_hWindow,TRUE);
	HELE hPic = XWnd_GetEle(g_hWindow, IDP_PASSPORTPIC);	//��ʼ�����û���ͼƬ��ʾΪ�հ�
	XPic_SetImage(hPic, NULL);
	hPic = XWnd_GetEle(g_hWindow, IDP_HEADPIC);	//��ʼ������ͷ��ͼƬ��ʾΪ�հ�
	XPic_SetImage(hPic, NULL);

	long ret = 0;
	char szUserPic[256] = {'\0'}, szIDHeadPic[256] = {'\0'};
	Scan_Para	sP, tP;
	IDCARD_ALL stIDCard;
	memset((char *)&stIDCard, 0x0, sizeof(IDCARD_ALL));
	sP.left = 0;
	sP.top = 0;
	sP.right = 300;
	sP.bottom = 200;
	sP.scan_mode = SCAN_IR_MODE; 
	sP.brightness = 0;
	sP.contrast = 0;
	sP.resolution = SCAN_300_DPI;
	sP.gamma = 10;
	sP.highlight = 0;
	sP.qualityscan = 1;
	sP.inversion   = 0;
	g_nID2ReadType = ID2_NO;
	g_nIDType = ID_TYPE_TAIWAN;
	EmptyVisitorPara();
	XWnd_RedrawWnd(g_hWindow,TRUE);
	
	strcpy(szUserPic, USER_ID2_PIC);
//	ret = RecogTBZALL(szUserPic, stIDCard, false, INFO_ALL_1, NULL);
	strcpy(szIDHeadPic,   USER_ID_PIC);
	sP.scan_mode = SCAN_GRAY_MODE; 
	memcpy(&tP, &sP, sizeof(Scan_Para));
	tP.scan_mode = SCAN_COLOR_MODE; 
	if (::GetPrivateProfileInt("TerminalSet", "FS533",0, CONFIG_FILE)==1) 
	{
		ret = CallFS533Bin("RecogTBZALL");
		if(ret == 1)
		{
			::GetPrivateProfileString("Scan", "Name", "", stIDCard.name, sizeof(stIDCard.name), FS533Scan_FILE);
			::GetPrivateProfileString("Scan", "Number", "", stIDCard.number, sizeof(stIDCard.number), FS533Scan_FILE);
			::GetPrivateProfileString("Scan", "Sex", "", stIDCard.sex, sizeof(stIDCard.sex), FS533Scan_FILE);
			::GetPrivateProfileString("Scan", "Address", "", stIDCard.address, sizeof(stIDCard.address), FS533Scan_FILE);
			::GetPrivateProfileString("Scan", "People", "", stIDCard.people, sizeof(stIDCard.people), FS533Scan_FILE);
			ret = 0;
		}
		else
		{
			ret = -1;
		}
	}
	else
		ret = ScanAndRcgCard(szUserPic, "", szIDHeadPic, stIDCard, sP, tP, SCAN_TaiWanPassPort, 1, 1, 1, 15);
	if (ret == 0 || ret == 50)
	{
		XEdit_SetText(XWnd_GetEle(g_hWindow, IDC_VISITORIDTYPE), L"̨��֤");
		SetVisitorInfo(stIDCard);	
		wchar_t wBuf[256] = {0};
		//MultiByteToWideChar(CP_ACP, 0, szUserPic, -1, wBuf, nLen);
        S_2_W(wBuf, szUserPic, 256);
		HIMAGE hImg = XImage_LoadFile(wBuf, true);
		HELE hPic = XWnd_GetEle(g_hWindow, IDP_PASSPORTPIC);
		XPic_SetImage(hPic, hImg);

	    S_2_W(wBuf, szIDHeadPic, 256);
	    hImg = XImage_LoadFile(wBuf, true);
	    hPic = XWnd_GetEle(g_hWindow, IDP_HEADPIC);
	    XPic_SetImage(hPic, hImg);
		
		::WritePrivateProfileString("TerminalSet", "DataInputPage", "2", CONFIG_FILE);
		//StartKeybd();
	}
	else
	{
		EnableScanBtns(true);
		XMessageBox(g_hWindow,L"ɨ��ʧ��",L"��ʾ");
		ResumeThread(g_HWDThreadID2ID);
		XWnd_RedrawWnd(g_hWindow);
		return false;
	}
	
	EnableScanBtns(true);
	XWnd_RedrawWnd(g_hWindow);
	ShowMainMenu(g_hPage2, true);
	ResumeThread(g_HWDThreadID2ID);
	return false;
}


//ɨ����ʦ֤
BOOL CALLBACK OnScanLawyer(HELE hEle, HELE hEventEle)
{
	if (hEle != hEventEle) 
	{
		return false;
	}

    SuspendThread(g_HWDThreadID2ID);
	EnableScanBtns(false);
	
	HELE hPic = XWnd_GetEle(g_hWindow, IDP_PASSPORTPIC);	//��ʼ�����û���ͼƬ��ʾΪ�հ�
	XPic_SetImage(hPic, NULL);
	hPic = XWnd_GetEle(g_hWindow, IDP_HEADPIC);	//��ʼ������ͷ��ͼƬ��ʾΪ�հ�
	XPic_SetImage(hPic, NULL);

    
	
	long ret = 0;
	char szUserPic[256] = {'\0'}, szIDHeadPic[256] = {'\0'};
	Scan_Para	sP, tP;
	IDCARD_ALL stIDCard;
	memset((char *)&stIDCard, 0x0, sizeof(IDCARD_ALL));
	sP.left = 0;
	sP.top = 0;
	sP.right = 300;
	sP.bottom = 200;
	sP.scan_mode = SCAN_IR_MODE; 
	sP.brightness = 0;
	sP.contrast = 0;
	sP.resolution = SCAN_200_DPI;
	sP.gamma = 10;
	sP.highlight = 0;
	sP.qualityscan = 1;
	sP.inversion   = 0;
	g_nID2ReadType = ID2_NO;
	g_nIDType = ID_TYPE_Lawyer;
	EmptyVisitorPara();
	XWnd_RedrawWnd(g_hWindow,TRUE);
	strcpy(szUserPic, USER_ID2_PIC);
//	ret = ScanImageEx(szUserPic, 10);
	strcpy(szIDHeadPic,   USER_ID_PIC);
	sP.scan_mode = SCAN_GRAY_MODE; 
	memcpy(&tP, &sP, sizeof(Scan_Para));
	tP.scan_mode = SCAN_COLOR_MODE; 
	if (::GetPrivateProfileInt("TerminalSet", "FS533",0, CONFIG_FILE)==1) 
	{
		ret = CallFS533Bin("ScanImageEx");
		if(ret == 1)
		{
			//::GetPrivateProfileString("Scan", "Name", "", stIDCard.name, sizeof(stIDCard.name), FS533Scan_FILE);
			//::GetPrivateProfileString("Scan", "Number", "", stIDCard.number, sizeof(stIDCard.number), FS533Scan_FILE);
			//::GetPrivateProfileString("Scan", "Sex", "", stIDCard.sex, sizeof(stIDCard.sex), FS533Scan_FILE);
			//::GetPrivateProfileString("Scan", "Address", "", stIDCard.address, sizeof(stIDCard.address), FS533Scan_FILE);
			//::GetPrivateProfileString("Scan", "People", "", stIDCard.people, sizeof(stIDCard.people), FS533Scan_FILE);
			ret = 0;
		}
		else
		{
			ret = -1;
		}
	}
	else
		ret = ScanAndRcgCard(szUserPic, "", szIDHeadPic, stIDCard, sP, tP, SCAN_UNKNOW, 1, 1, 1, 15);
	if (ret == 0 || ret==48)
	{
	    SetVisitorReadOnly(false);
	    XEdit_SetReadOnly(XWnd_GetEle(g_hWindow, 5023),false);
	    
		XEdit_SetText(XWnd_GetEle(g_hWindow, IDC_VISITORIDTYPE), L"��ʦ֤");
		//SetVisitorInfo(stIDCard);	
		wchar_t wBuf[256] = {0};
		//MultiByteToWideChar(CP_ACP, 0, szUserPic, -1, wBuf, nLen);
        S_2_W(wBuf, szUserPic, 256);
		HIMAGE hImg = XImage_LoadFile(wBuf, true);
		HELE hPic = XWnd_GetEle(g_hWindow, IDP_PASSPORTPIC);
		XPic_SetImage(hPic, hImg);

		//wchar_t wBuf[256] = {0};
	    S_2_W(wBuf, szIDHeadPic, 256);
	    hImg = XImage_LoadFile(wBuf, true);
	    hPic = XWnd_GetEle(g_hWindow, IDP_HEADPIC);
	    XPic_SetImage(hPic, hImg);
		SetVisitorReadOnly(false);
		XEdit_SetText(XWnd_GetEle(g_hWindow, IDE_VISITORNAME), L"��ʦ"); 



		::WritePrivateProfileString("TerminalSet", "DataInputPage", "2", CONFIG_FILE);
		//StartKeybd();
	}
	else
	{
		EnableScanBtns(true);
		XMessageBox(g_hWindow,L"ɨ��ʧ��",L"��ʾ");
		ResumeThread(g_HWDThreadID2ID);
		XWnd_RedrawWnd(g_hWindow);
		return false;
	}
	EnableScanBtns(true);
	XWnd_RedrawWnd(g_hWindow);
	ShowMainMenu(g_hPage2, true);
	ResumeThread(g_HWDThreadID2ID);
	return true;
}

//ɨ������֤��
BOOL CALLBACK OnScanOther(HELE hEle, HELE hEventEle)
{
	if (hEle != hEventEle) 
	{
		return false;
	}

    SuspendThread(g_HWDThreadID2ID);
	EnableScanBtns(false);
	//XWnd_RedrawWnd(g_hWindow,TRUE);
	HELE hPic = XWnd_GetEle(g_hWindow, IDP_PASSPORTPIC);	//��ʼ�����û���ͼƬ��ʾΪ�հ�
	XPic_SetImage(hPic, NULL);
	hPic = XWnd_GetEle(g_hWindow, IDP_HEADPIC);	//��ʼ������ͷ��ͼƬ��ʾΪ�հ�
	XPic_SetImage(hPic, NULL);


	long ret = 0;
	char szUserPic[256] = {'\0'}, szIDHeadPic[256] = {'\0'};
	Scan_Para	sP, tP;
	IDCARD_ALL stIDCard;
	memset((char *)&stIDCard, 0x0, sizeof(IDCARD_ALL));
	sP.left = 0;
	sP.top = 0;
	sP.right = 300;
	sP.bottom = 200;
	sP.scan_mode = SCAN_IR_MODE; 
	sP.brightness = 0;
	sP.contrast = 0;
	sP.resolution = SCAN_200_DPI;
	sP.gamma = 10;
	sP.highlight = 0;
	sP.qualityscan = 1;
	sP.inversion   = 0;
	g_nID2ReadType = ID2_NO;
	g_nIDType = ID_TYPE_OTHER;
	EmptyVisitorPara();
	XWnd_RedrawWnd(g_hWindow,TRUE);

	strcpy(szUserPic, USER_ID2_PIC);
//	ret = ScanImageEx(szUserPic, 10);
	strcpy(szIDHeadPic,   USER_ID_PIC);
	sP.scan_mode = SCAN_GRAY_MODE; 
	memcpy(&tP, &sP, sizeof(Scan_Para));
	tP.scan_mode = SCAN_COLOR_MODE; 
	if (::GetPrivateProfileInt("TerminalSet", "FS533",0, CONFIG_FILE)==1) 
	{
		ret = CallFS533Bin("ScanImageEx");
		if(ret == 1)
		{
			//::GetPrivateProfileString("Scan", "Name", "", stIDCard.name, sizeof(stIDCard.name), FS533Scan_FILE);
			//::GetPrivateProfileString("Scan", "Number", "", stIDCard.number, sizeof(stIDCard.number), FS533Scan_FILE);
			//::GetPrivateProfileString("Scan", "Sex", "", stIDCard.sex, sizeof(stIDCard.sex), FS533Scan_FILE);
			//::GetPrivateProfileString("Scan", "Address", "", stIDCard.address, sizeof(stIDCard.address), FS533Scan_FILE);
			//::GetPrivateProfileString("Scan", "People", "", stIDCard.people, sizeof(stIDCard.people), FS533Scan_FILE);
			ret = 0;
		}
		else
		{
			ret = -1;
		}
	}
	else
		ret = ScanAndRcgCard(szUserPic, "", szIDHeadPic, stIDCard, sP, tP, SCAN_UNKNOW, 1, 1, 1, 15);
	if (ret == 0 || ret==48)
	{
	    SetVisitorReadOnly(false);
	    XEdit_SetReadOnly(XWnd_GetEle(g_hWindow, 5023),false);
	    InitVisitorData();
		XEdit_SetText(XWnd_GetEle(g_hWindow, IDC_VISITORIDTYPE), L"����֤��");
		//SetVisitorInfo(stIDCard);	
		wchar_t wBuf[256] = {0};
		//MultiByteToWideChar(CP_ACP, 0, szUserPic, -1, wBuf, nLen);
        S_2_W(wBuf, szUserPic, 256);
		HIMAGE hImg = XImage_LoadFile(wBuf, true);
		HELE hPic = XWnd_GetEle(g_hWindow, IDP_PASSPORTPIC);
		XPic_SetImage(hPic, hImg);

		//wchar_t wBuf[256] = {0};
	    S_2_W(wBuf, szIDHeadPic, 256);
	    hImg = XImage_LoadFile(wBuf, true);
	    hPic = XWnd_GetEle(g_hWindow, IDP_HEADPIC);
	    XPic_SetImage(hPic, hImg);
		SetVisitorReadOnly(false);
		::WritePrivateProfileString("TerminalSet", "DataInputPage", "2", CONFIG_FILE);
		//StartKeybd();
	}
	else
	{
		EnableScanBtns(true);
		XMessageBox(g_hWindow,L"ɨ��ʧ��",L"��ʾ");
		ResumeThread(g_HWDThreadID2ID);
		XWnd_RedrawWnd(g_hWindow);
		return false;
	}
	EnableScanBtns(true);
	XWnd_RedrawWnd(g_hWindow);
	ShowMainMenu(g_hPage2, true);
	ResumeThread(g_HWDThreadID2ID);
	return true;
}

bool bQuit= false;
////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CALLBACK OnCloseDevice(HELE hEle, HELE hEventEle)
{   
	HANDLE hToken;
	TOKEN_PRIVILEGES tkp;
	DWORD dwVersion;
	
	dwVersion = GetVersion();         // �õ�WINDOWS NT��Win32�İ汾��	
	if(dwVersion < 0x80000000)        // �����ж�WINϵ�У��Ӷ�������Ӧ��Ȩ��
	{
		OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken);
		LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &tkp.Privileges[0].Luid);
		tkp.PrivilegeCount = 1;
		tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
		AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, (PTOKEN_PRIVILEGES)NULL, 0);
		ExitWindowsEx(EWX_SHUTDOWN | EWX_FORCE, 0);
	}
	else
	{
		ExitWindowsEx(EWX_FORCE | EWX_SHUTDOWN, 0);
	}  
	bQuit = true;
	XWnd_CloseWindow(g_hExitWindow);
    //exit(0);
	return false;
}


BOOL CALLBACK OnCloseApp(HELE hEle, HELE hEventEle)
{

	//exit(0);*/
    bQuit =true;
	XWnd_CloseWindow(g_hExitWindow);
	return true;
}
//�����ҳ���ϡ��˳�����ť���¼���Ӧ
BOOL CALLBACK OnExitApp(HELE hEle, HELE hEventEle)
{
	if (hEle != hEventEle)
	{
		return false;
	}
	EnableButton(IDB_TAB8);
	//= XMessageBox(XEle_GetHWindow(hEle), L"�����˳�ϵͳ,��[ȷ��]�˳�,��[ȡ��]����ϵͳ!", L"��ʾ��Ϣ", XMB_OK | XMB_CANCEL);

	HideSoftKeybd();
    g_hExitWindow = XModalWnd_CreateWindow(340, 160, L"��ʾ��Ϣ", XWnd_GetHWnd(g_hWindow));
	if (g_hExitWindow)
	{
		HELE hButton1=XBtn_Create(20,30,80,60,L"�˳�����",g_hExitWindow); 
		XEle_SetBkTransparent(hButton1,false);
		XEle_RegisterEvent(hButton1,XE_BNCLICK,OnCloseApp);
		
		HELE hButton2=XBtn_Create(120,30,80,60,L"�ر��豸",g_hExitWindow); 
		XEle_SetBkTransparent(hButton2,true);
		XEle_RegisterEvent(hButton2,XE_BNCLICK,OnCloseDevice);

		HELE hButton3=XBtn_Create(220,30,80,60,L"����",g_hExitWindow); 
		XEle_SetBkTransparent(hButton3,true);
		XEle_RegisterEvent(hButton3,XE_BNCLICK,OnCloseLogOutBtnClick);  //���á�ǩ�롱�����¼�
	}
	XModalWnd_DoModal(g_hExitWindow);

	if (bQuit)
	{
		if (g_hWeb != NULL)
		{
			XWeb_Destroy(g_hWeb);
		}
		OleUninitialize();
		g_bStopID2 = true;			//ֹͣ����֤��д�߳�
		g_bStopBarCode = true;		//ֹͣ�����Ķ��߳�	
		FreeIdcardLibrary();		//ж�ض���֤��д��
		EndWJUsb();
		CloseCardReader();
		CloseSoftKeybd();
		CloseXYB();
		CloseBackScreen();
		HideTaskBar(false);
		//WaitForSingleObject(g_ThreadSocketMutex,INFINITE);
		//g_tSock->CloseSock();
		
		ReleaseMutex(g_ThreadSocketMutex);
		//SuspendThread(g_HWDThreadID2ID);
		//ExitThread(g_HWDThreadID2ID);
		//WaitForSingleObject(g_HWDThreadID2ID,INFINITE);//�ȴ��߳̽���
		//WaitForSingleObject(g_ThreadReadBarcode,INFINITE);
		//WaitForSingleObject(g_HWDThreadTime,INFINITE);
		TerminateThread(g_HWDThreadID2ID,0);
		TerminateThread(g_ThreadReadBarcode,0);
		TerminateThread(g_HWDThreadTime,0);
		//g_tSock->CloseSock();
		delete  g_tSock;
		//g_tSock=NULL;
		//delete pThread;//ɾ���߳�
		XWnd_CloseWindow(XEle_GetHWindow(hEle));
		XWnd_CloseWindow(g_hWindow); 
	}

	return false;
}

//�����������Ϣ�Ǽ��еġ����顱��ť����ʾ�������˵���ʷ������Ϣ
BOOL CALLBACK OnVisitInformationList(HELE hEle, HELE hEventEle)
{
	HELE hE = XWnd_GetEle(g_hWindow, IDE_VISITORNAME);
	HideSoftKeybd();
	if (XEdit_GetTextLength(hE) <= 0)
	{
		XMessageBox(g_hWindow, L"��������������Ϊ��", L"��ʾ��Ϣ");
		return false;
	}

	char szT[512] = {'\0'};
	long FldCount = 0, ret = 0;
	CString outParas[FLD_MAX], outP1[FLD_MAX];
	CWinAdo tAdo(sCONNSTR_MYSQL);
	CWinAdo tAdo1(sCONNSTR_MYSQL);

	wchar_t wBuf[256 + 1] = {0};
	XEdit_GetText(hE, wBuf, 256);
	memset(g_szHistVisitorName, 0, sizeof(g_szVisitorName));
	W_2_S(wBuf, g_szHistVisitorName, 256);

 	g_hOfficialWin = XModalWnd_CreateWindow(800, 600, L"�������õ���ʷ���ʼ�¼", XWnd_GetHWnd(g_hWindow));
	XWnd_SetBorderSize(g_hOfficialWin, 3, 3, 3, 3);

	HIMAGE hImg = XImage_LoadFile(L".\\skin\\image\\frame.jpg", true);
	XWnd_SetImageNC(g_hOfficialWin, hImg);
	HELE hList = XList_Create(0, 0, 795, 570, g_hOfficialWin);

	XEle_RegisterEvent(hList, XE_LIST_SELECT, OnHistoryVisitorList);
	XList_SetHeaderHeight(hList, 30);	
	XList_EnableGrid(hList, false);

	ret = tAdo.OpenAdo();
	if (ret != ADOOK)
	{
		XList_AddColumn(hList, 800, L"");
		XModalWnd_DoModal(g_hOfficialWin);
		return false;
	}

	ret = tAdo1.OpenAdo();
	if (ret != ADOOK)
	{
		XList_AddColumn(hList, 800, L"");
		XModalWnd_DoModal(g_hOfficialWin);
		return false;
	}

	for (int i = 0; i < FLD_MAX; i++)
	{
		outParas[i].Empty();
		outP1[i].Empty();
	}

	sprintf(szT, "select * from t_visitor where visitor_name = \'%s\' and visitor_status>1 order by visitor_visitetime desc", g_szHistVisitorName);
	ret = tAdo.execSelect(szT, &FldCount, outParas);
	if (ret == ADOERR || (FldCount == 0 && outParas[0].IsEmpty()))
	{
		XList_AddColumn(hList, 800, L"");
		//XList_AddItem(hList, L"û��ָ����������ʷ����", 0);

		XModalWnd_DoModal(g_hOfficialWin);
		return false;
	}

	XList_AddColumn(hList, 40, L"���");
	XList_AddColumn(hList, 100, L"����������");
	XList_AddColumn(hList, 100, L"����������");
	XList_AddColumn(hList, 200, L"�����˼���");
	XList_AddColumn(hList, 200, L"�ݷ�ʱ��");

	int n = 0;

	do 
	{
		memset(szT, 0, sizeof(szT));
		memset((char *)wBuf, 0, sizeof(wBuf));

		sprintf(szT, "%d", n + 1);		//���
		S_2_W(wBuf, szT, 256);
		XList_AddItem(hList, wBuf, n);

		for (int j = 0; j < FLD_MAX; j++)
		{
			outP1[j].Empty();
		}

		sprintf(szT, "select * from t_official where official_name = \'%s\'", outParas[14]);
		ret = tAdo1.execSelect(szT, &FldCount, outP1);
		if (ret == ADOERR || (FldCount == 0 && outP1[0].IsEmpty()))
		{
			//XList_AddItem(hList, L"û������", 0);
		}

		L_V_I(g_szHistVisitorName, n, 1)
		L_V_I(outP1[0], n, 2)
		L_V_I(outP1[6], n, 3)

		for (int k = 0; k < FLD_MAX; k++)
		{
			outP1[k].Empty();
		}
		//sprintf(szT, "select FROM_UNIXTIME(%s)", outParas[16]);
		if (g_nSqlConnectType == SQL_CONNECT_SERVER2005)
		{
			sprintf(szT, "SELECT DATEADD(s,%d,'1970-01-01 08:00:00')", atol(outParas[16]));	//����ʱ��
		}
		else
		{
			sprintf(szT, "select FROM_UNIXTIME(%s)", outParas[16]);	//����ʱ��
		}
		ret = tAdo1.execSelect(szT, &FldCount, outP1);
		if (ret == ADOERR || (FldCount == 0 && outP1[0].IsEmpty()))
		{
			L_V_I("", n, 4)
		}
		else
		{
			L_V_I(outP1[0], n, 4)
		}
		n++;
	} while (tAdo.getNext(outParas) == ADOOK);

    if (GetPrivateProfileInt("TerminalSet", "bFtpOpen",0, CONFIG_FILE)==1)
	{
		try 
		{ 
			m_pInetSession->Close();
			m_pInetSession=new CInternetSession("cc",1,PRE_CONFIG_INTERNET_ACCESS);
			char szSvrName[40] = {'\0'},szUsr[40] = {'\0'},szPwd[40] = {'\0'};
			::GetPrivateProfileString("TerminalSet", "FtpSeverName", "", szSvrName, sizeof(szSvrName), CONFIG_FILE);
			::GetPrivateProfileString("TerminalSet", "FtpUser", "", szUsr, sizeof(szUsr), CONFIG_FILE);
			::GetPrivateProfileString("TerminalSet", "FtpPwd", "", szPwd, sizeof(szPwd), CONFIG_FILE);
			//�½����Ӷ���
			m_pFtpConnection=m_pInetSession->GetFtpConnection(szSvrName,szUsr,szPwd); 
		} 
		catch(CInternetException *pEx) 
		{
			//��ȡ����
			TCHAR szError[1024];
			if(pEx->GetErrorMessage(szError,1024))
				AfxMessageBox(szError);
			else  
				AfxMessageBox("There was an exception");
			pEx->Delete();
			m_pFtpConnection=NULL;
			g_bFtpOpened = false;
			return false;
		}
		g_bFtpOpened = true;
	}
	XModalWnd_DoModal(g_hOfficialWin);

	return false;
}

//����������ϡ���ѯ��������Ϣ����ť����ʼ��ѯ�������˵���Ϣ
BOOL CALLBACK OnQueryEmployer(HELE hEle, HELE hEventEle)
{
	HideSoftKeybd();
	BOOL bRet = PiPeiEmployer();
	if (!bRet)
	{
		XMessageBox(g_hWindow, L"û��ƥ�䵽��������Ϣ", L"����");
		XWnd_RedrawWnd(XEle_GetHWindow(hEle));
		return false;
	}
	/*XEle_ShowEle(g_hPage12, true);
    XEle_SetZOrder(g_hPage12,g_hPage12,XC_ZORDER_TOP,NULL);
	XWnd_RedrawWnd(XEle_GetHWindow(hEle));*/
	return false;
}

BOOL QueryEmployer(HELE hEle)
{
	char szOfficialName[32] = {'\0'};
	wchar_t wBuf[256 + 1] = {0};

	HELE hE = XWnd_GetEle(g_hWindow, IDE_MASTERNAME);
	if (XEdit_GetTextLength(hE) <= 0)
	{
		XMessageBox(g_hWindow, L"��������������Ϊ��!", L"��ʾ��Ϣ", XMB_OK);
		return false;
	}

	XEdit_GetText(hE, wBuf, 256);
	W_2_S(wBuf, szOfficialName, 256);

	char szT[512] = {'\0'};
	long FldCount = 0, ret = 0;;
	CString outParas[FLD_MAX];
	CWinAdo tAdo(sCONNSTR_MYSQL);

	ret = tAdo.OpenAdo();
	if (ret != ADOOK)
	{
		return false;
	}
	
	for (int i = 0; i < FLD_MAX; i++)
	{
		outParas[i].Empty();
	}
	
	sprintf(szT, "select * from t_official where official_name = \'%s\'", szOfficialName);	//��ѯ�����˵���Ϣ
	ret = tAdo.execSelect(szT, &FldCount, outParas);
	if (ret != ADOERR)
	{
		if (FldCount == 0)		//û�иñ����˵���Ϣ����ʾ�Ƿ���еǼ�
		{
			ret = XMessageBox(g_hWindow, L"û�иñ����˵���Ϣ,�Ƿ�ǼǸñ������˵���Ϣ?", L"��ʾ��Ϣ", XMB_OK | XMB_CANCEL);
			if (ret == XMB_OK)
			{
				OFFICIALINFO official;
				int n = 0;
				memset((char *)&official, 0, sizeof(OFFICIALINFO));

				memset((char *)wBuf, 0, sizeof(wBuf));
				hE = XWnd_GetEle(g_hWindow, IDE_MASTERNAME);
				XEdit_GetText(hE, wBuf, 128);
				W_2_S(wBuf, official.szName, 128);

				hE = XWnd_GetEle(g_hWindow, IDC_MASTERSEX);
				n = XComboBox_GetSelectItem(hE);
				n == 0 ? strcpy(official.szSex, "��") : strcpy(official.szSex, "Ů");

				memset((char *)wBuf, 0, sizeof(wBuf));
				hE = XWnd_GetEle(g_hWindow, IDE_OFFICEPHONE);
				XEdit_GetText(hE, wBuf, 128);
				W_2_S(wBuf, official.szOfficePhone, 128);

				memset((char *)wBuf, 0, sizeof(wBuf));
				hE = XWnd_GetEle(g_hWindow, IDE_MASTERMOBILE);
				XEdit_GetText(hE, wBuf, 128);
				W_2_S(wBuf, official.szMobile, 128);

				memset((char *)wBuf, 0, sizeof(wBuf));
				hE = XWnd_GetEle(g_hWindow, IDE_OFFICENAME);
				XEdit_GetText(hE, wBuf, 128);
				W_2_S(wBuf, official.szOfficeName, 128);

				memset((char *)wBuf, 0, sizeof(wBuf));
				hE = XWnd_GetEle(g_hWindow, IDE_OFFICENUM);
				XEdit_GetText(hE, wBuf, 128);
				W_2_S(wBuf, official.szOfficeAddr, 128);

				memset((char *)wBuf, 0, sizeof(wBuf));
				hE = XWnd_GetEle(g_hWindow, IDC_MASTERZHIWU);
				XEdit_GetText(hE, wBuf, 128);
				W_2_S(wBuf, official.szOfficialDuty, 128);

				SaveOfficialInfo(official);
				return true;
			}
		}
		return false;
	}
	do
	{
		memset((char *)wBuf, 0, sizeof(wBuf));
		MultiByteToWideChar(CP_ACP, 0, outParas[2], -1, wBuf, 256);		//�������˰칫�绰
		hE = XWnd_GetEle(g_hWindow, IDE_OFFICEPHONE);
		XEdit_SetText(hE, wBuf);

		memset((char *)wBuf, 0, sizeof(wBuf));
		MultiByteToWideChar(CP_ACP, 0, outParas[3], -1, wBuf, 256);		//���������ֻ�
		hE = XWnd_GetEle(g_hWindow, IDE_MASTERMOBILE);
		XEdit_SetText(hE, wBuf);

		memset((char *)wBuf, 0, sizeof(wBuf));
		MultiByteToWideChar(CP_ACP, 0, outParas[4], -1, wBuf, 256);		//�������˰칫������
		hE = XWnd_GetEle(g_hWindow, IDE_OFFICENAME);
		XEdit_SetText(hE, wBuf);

		memset((char *)wBuf, 0, sizeof(wBuf));
		MultiByteToWideChar(CP_ACP, 0, outParas[5], -1, wBuf, 256);		//�������˰칫�ҵ�ַ
		hE = XWnd_GetEle(g_hWindow, IDE_OFFICENUM);
		XEdit_SetText(hE, wBuf);

		memset((char *)wBuf, 0, sizeof(wBuf));
		MultiByteToWideChar(CP_ACP, 0, outParas[6], -1, wBuf, 256);		//�������˼���
		hE = XWnd_GetEle(g_hWindow, IDC_MASTERZHIWU);
		XEdit_SetText(hE, wBuf);

		memset((char *)wBuf, 0, sizeof(wBuf));
		MultiByteToWideChar(CP_ACP, 0, outParas[7], -1, wBuf, 256);		//�������˷���
		hE = XWnd_GetEle(g_hWindow, IDE_ROOMNUM);
		XEdit_SetText(hE, wBuf);
	} while (tAdo.getNext(outParas) == ADOOK) ;

	return false;
}

BOOL CALLBACK OnSelectFolk(HELE hEle, HELE hEventEle, int nIndex)
{
	wchar_t wBuf[256] = {0};
	char szT[256] = {'\0'};

	if (nIndex < 0)
	{
		return false;
	}
	int n = nIndex;

	memset((char *)wBuf, 0, sizeof(wBuf));
	//memset(szT, 0, sizeof(szT));
	wcscpy(wBuf, XList_GetItemText(hEle, n, 0));
	XEdit_SetReadOnly(XWnd_GetEle(g_hWindow, IDC_VISITORFOLK),false);

	XEdit_SetText(XWnd_GetEle(g_hWindow, IDC_VISITORFOLK),L"");
	XEdit_SetText(XWnd_GetEle(g_hWindow, IDC_VISITORFOLK),wBuf);
	XEdit_SetReadOnly(XWnd_GetEle(g_hWindow, IDC_VISITORFOLK),true);
	//W_2_S(wBuf, szT, 128);
	//::WritePrivateProfileString("OfficialInfo", "Name", szT, CONFIG_FILE);

	//XWnd_CloseWindow(g_hOfficialWin);
	XWnd_CloseWindow(XEle_GetHWindow(hEle));
	XWnd_RedrawWnd(g_hWindow);
	return true;
}

BOOL CALLBACK OnSelectVisitorType(HELE hEle, HELE hEventEle, int nIndex)
{
	wchar_t wBuf[256] = {0};
	char szT[256] = {'\0'};

	if (nIndex < 0)
	{
		return false;
	}
	int n = nIndex;

	memset((char *)wBuf, 0, sizeof(wBuf));
	//memset(szT, 0, sizeof(szT));
	wcscpy(wBuf, XList_GetItemText(hEle, n, 0));
	XEdit_SetReadOnly(XWnd_GetEle(g_hWindow, IDE_VISITORTYPE),false);
	XEdit_SetText(XWnd_GetEle(g_hWindow, IDE_VISITORTYPE),L"");
	XEdit_SetText(XWnd_GetEle(g_hWindow, IDE_VISITORTYPE),wBuf);
    XEdit_SetReadOnly(XWnd_GetEle(g_hWindow, IDE_VISITORTYPE),true);
	//W_2_S(wBuf, szT, 128);
	//::WritePrivateProfileString("OfficialInfo", "Name", szT, CONFIG_FILE);
	//XWnd_CloseWindow(g_hOfficialWin);
	XWnd_CloseWindow(XEle_GetHWindow(hEle));
	XWnd_RedrawWnd(g_hWindow);
	return true;
}

BOOL CALLBACK OnSelectIDType(HELE hEle, HELE hEventEle, int nIndex)
{
	wchar_t wBuf[256] = {0};
	char szT[256] = {'\0'};

	if (nIndex < 0)
	{
		return false;
	}
	int n = nIndex;

	memset((char *)wBuf, 0, sizeof(wBuf));
	//memset(szT, 0, sizeof(szT));
	wcscpy(wBuf, XList_GetItemText(hEle, n, 0));
	XEdit_SetReadOnly(XWnd_GetEle(g_hWindow, IDC_VISITORIDTYPE),false);
	XEdit_SetText(XWnd_GetEle(g_hWindow, IDC_VISITORIDTYPE),L"");
	XEdit_SetText(XWnd_GetEle(g_hWindow, IDC_VISITORIDTYPE),wBuf);
    XEdit_SetReadOnly(XWnd_GetEle(g_hWindow, IDC_VISITORIDTYPE),true);
	//W_2_S(wBuf, szT, 128);
	//::WritePrivateProfileString("OfficialInfo", "Name", szT, CONFIG_FILE);
	if(wcscmp(wBuf,L"��ʦ֤") == 0 )
		g_nIDType = ID_TYPE_Lawyer;
	else
        g_nIDType = ID_TYPE_ID2;

	if(g_nIDType == ID_TYPE_Lawyer) //ɨ����ʦ֤ʱ��ʾ��ʦ�����Ŀ
	{
        XEle_ShowEle(XWnd_GetEle(g_hWindow, IDS_Notice), true);
		XEle_ShowEle(XWnd_GetEle(g_hWindow, IDC_Notice_Comb), true);
		XEle_ShowEle(XWnd_GetEle(g_hWindow, IDS_LawyerLetter), true);
		XEle_ShowEle(XWnd_GetEle(g_hWindow, IDC_LawyerLetter_Comb), true);
		XComboBox_DeleteItemAll(GETELE(IDC_Notice_Comb));
	    XComboBox_AddString(GETELE(IDC_Notice_Comb), L"��");
	    XComboBox_AddString(GETELE(IDC_Notice_Comb), L"��");
		XComboBox_SetSelectItem(GETELE(IDC_Notice_Comb), 0);

		XComboBox_DeleteItemAll(GETELE(IDC_LawyerLetter_Comb));
	    XComboBox_AddString(GETELE(IDC_LawyerLetter_Comb), L"��");
	    XComboBox_AddString(GETELE(IDC_LawyerLetter_Comb), L"��");
		XComboBox_SetSelectItem(GETELE(IDC_LawyerLetter_Comb), 0);

		XEle_ShowEle(XWnd_GetEle(g_hWindow, IDS_VISITORUNIT), false);
		XEle_ShowEle(XWnd_GetEle(g_hWindow, IDE_VISITORUNIT), false);
		XEle_ShowEle(XWnd_GetEle(g_hWindow, IDS_CARNUM), false);
		XEle_ShowEle(XWnd_GetEle(g_hWindow, IDE_VISITORCARNUM), false);
	}
	else
	{
        XEle_ShowEle(XWnd_GetEle(g_hWindow, IDS_Notice), false);
		XEle_ShowEle(XWnd_GetEle(g_hWindow, IDC_Notice_Comb), false);
		XEle_ShowEle(XWnd_GetEle(g_hWindow, IDS_LawyerLetter), false);
		XEle_ShowEle(XWnd_GetEle(g_hWindow, IDC_LawyerLetter_Comb), false);

        XEle_ShowEle(XWnd_GetEle(g_hWindow, IDS_VISITORUNIT), true);
		XEle_ShowEle(XWnd_GetEle(g_hWindow, IDE_VISITORUNIT), true);
		XEle_ShowEle(XWnd_GetEle(g_hWindow, IDS_CARNUM), true);
		XEle_ShowEle(XWnd_GetEle(g_hWindow, IDE_VISITORCARNUM), true);

	}
	//XWnd_CloseWindow(g_hOfficialWin);
	XWnd_CloseWindow(XEle_GetHWindow(hEle));
	XWnd_RedrawWnd(g_hWindow);
	return true;
}

BOOL CALLBACK OnSelectOfficialListClick(HELE hEle, HELE hEventEle, int nIndex)
{
	wchar_t wBuf[256] = {0};
	char szT[256] = {'\0'};

	if (nIndex < 0)
	{
		return false;
	}
	int n = nIndex;

	memset((char *)wBuf, 0, sizeof(wBuf));
	memset(szT, 0, sizeof(szT));
	wcscpy(wBuf, XList_GetItemText(hEle, n, 1));
	W_2_S(wBuf, szT, 128);
	::WritePrivateProfileString("OfficialInfo", "Name", szT, CONFIG_FILE);

	memset((char *)wBuf, 0, sizeof(wBuf));
	memset(szT, 0, sizeof(szT));
	wcscpy(wBuf, XList_GetItemText(hEle, n, 2));
	W_2_S(wBuf, szT, 128);
	::WritePrivateProfileString("OfficialInfo", "Sex", szT, CONFIG_FILE);

	memset((char *)wBuf, 0, sizeof(wBuf));
	memset(szT, 0, sizeof(szT));
	wcscpy(wBuf, XList_GetItemText(hEle, n, 3));
	W_2_S(wBuf, szT, 128);
	::WritePrivateProfileString("OfficialInfo", "OfficePhone", szT, CONFIG_FILE);

	memset((char *)wBuf, 0, sizeof(wBuf));
	memset(szT, 0, sizeof(szT));
	wcscpy(wBuf, XList_GetItemText(hEle, n, 4));
	W_2_S(wBuf, szT, 128);
	::WritePrivateProfileString("OfficialInfo", "Mobile", szT, CONFIG_FILE);

	memset((char *)wBuf, 0, sizeof(wBuf));
	memset(szT, 0, sizeof(szT));
	wcscpy(wBuf, XList_GetItemText(hEle, n, 5));
	W_2_S(wBuf, szT, 128);
	::WritePrivateProfileString("OfficialInfo", "OfficeName", szT, CONFIG_FILE);

	memset((char *)wBuf, 0, sizeof(wBuf));
	memset(szT, 0, sizeof(szT));
	wcscpy(wBuf, XList_GetItemText(hEle, n, 6));
	W_2_S(wBuf, szT, 128);
	::WritePrivateProfileString("OfficialInfo", "OfficeAddr", szT, CONFIG_FILE);

	memset((char *)wBuf, 0, sizeof(wBuf));
	memset(szT, 0, sizeof(szT));
	wcscpy(wBuf, XList_GetItemText(hEle, n, 7));
	W_2_S(wBuf, szT, 128);
	::WritePrivateProfileString("OfficialInfo", "Level", szT, CONFIG_FILE);

	memset((char *)wBuf, 0, sizeof(wBuf));
	memset(szT, 0, sizeof(szT));
	wcscpy(wBuf, XList_GetItemText(hEle, n, 8));
	W_2_S(wBuf, szT, 128);
	::WritePrivateProfileString("OfficialInfo", "RoomNum", szT, CONFIG_FILE);

	SetOfficialInfo();
//	XEle_ShowEle(hEventEle, false);
	XWnd_CloseWindow(g_hOfficialWin);
	XWnd_RedrawWnd(g_hWindow);
	return false;
}

//ϵͳ����ҳ���еĲ�ѯ��Ա��Ϣ
BOOL CALLBACK OnQueryEmployerForList(HELE hEle, HELE hEventEle)
{
	char szOfficialName[32] = {'\0'},szTmp[40] = {'\0'},szSex[10]={'\0'};
	wchar_t wBuf[256 + 1] = {0};

	char szT[512] = {'\0'};
	long FldCount = 0, ret = 0;
	CString outParas[FLD_MAX];
	CWinAdo tAdo(sCONNSTR_MYSQL);

	ret = tAdo.OpenAdo();
	if (ret != ADOOK)
	{
		return false;
	}
	
	for (int i = 0; i < FLD_MAX; i++)
	{
		outParas[i].Empty();
	}

	HELE hE = XWnd_GetEle(g_hWindow, IDE_ADD_OFFICIALNAME);
		
  /*      XEdit_SetText(hE_txt,L"");
        hE_txt = XWnd_GetEle(g_hWindow, IDE_ADD_OFFICEPHONE);
		XEdit_SetText(hE_txt,L"");
        hE_txt = XWnd_GetEle(g_hWindow, IDE_ADD_OFFICENAME);
		XEdit_SetText(hE_txt,L"");
        hE_txt = XWnd_GetEle(g_hWindow, IDE_ADD_OFFICIALMOBILE);
		XEdit_SetText(hE_txt,L"");
        hE_txt = XWnd_GetEle(g_hWindow, IDE_ADD_OFFICEADDR);
		XEdit_SetText(hE_txt,L"");
        hE_txt = XWnd_GetEle(g_hWindow, IDC_ADD_OFFICIALZHIWU);
		XEdit_SetText(hE_txt,L"");
        hE_txt = XWnd_GetEle(g_hWindow, IDE_ADD_ROOMNUM);
		XEdit_SetText(hE_txt,L"");
        hE_txt = XWnd_GetEle(g_hWindow, IDE_ADD_IP);
		XEdit_SetText(hE_txt,L"");*/
    //int l=XEdit_GetTextLength(XWnd_GetEle(g_hWindow, IDC_ADD_OFFICIALSEX));
	XEdit_GetText(XWnd_GetEle(g_hWindow, IDC_ADD_OFFICIALSEX), wBuf, 256);
	W_2_S(wBuf, szSex, 256);

	if (XEdit_GetTextLength(hE) > 0)
	{
		XEdit_GetText(hE, wBuf, 256);
		W_2_S(wBuf, szOfficialName, 256);	
		sprintf(szT, "select official_name,official_sex,official_office_phone,official_mobile,official_office_name,official_office_addr,official_duty,\
			official_SSO,official_NO,official_room,official_IP,official_id from t_official where official_name = \'%s\'", 
			szOfficialName);	//��ѯ�����˵���Ϣ

	}
	else if (XEdit_GetTextLength(XWnd_GetEle(g_hWindow, IDC_ADD_OFFICIALSEX)) > 0 &&strcmp(szSex," ")!=0)
	{
		XEdit_GetText(XWnd_GetEle(g_hWindow, IDC_ADD_OFFICIALSEX), wBuf, 256);
		W_2_S(wBuf, szTmp, 256);	
		sprintf(szT, "select official_name,official_sex,official_office_phone,official_mobile,official_office_name,official_office_addr,official_duty,\
			official_SSO,official_NO,official_room,official_IP,official_id from t_official where official_sex = \'%s\'", 
			szTmp);
	}
	else if (XEdit_GetTextLength(XWnd_GetEle(g_hWindow, IDE_ADD_OFFICEPHONE)) > 0)
	{
		XEdit_GetText(XWnd_GetEle(g_hWindow, IDE_ADD_OFFICEPHONE), wBuf, 256);
		W_2_S(wBuf, szTmp, 256);	
		sprintf(szT, "select official_name,official_sex,official_office_phone,official_mobile,official_office_name,official_office_addr,official_duty,\
			official_SSO,official_NO,official_room,official_IP,official_id from t_official where official_office_phone = \'%s\'", 
			szTmp);
	}
	else if (XEdit_GetTextLength(XWnd_GetEle(g_hWindow, IDE_ADD_OFFICENAME)) > 0)
	{
		XEdit_GetText(XWnd_GetEle(g_hWindow, IDE_ADD_OFFICENAME), wBuf, 256);
		W_2_S(wBuf, szTmp, 256);	
		sprintf(szT, "select official_name,official_sex,official_office_phone,official_mobile,official_office_name,official_office_addr,official_duty,\
			official_SSO,official_NO,official_room,official_IP,official_id from t_official where official_office_name = \'%s\'", 
			szTmp);
	}
	else if (XEdit_GetTextLength(XWnd_GetEle(g_hWindow, IDE_ADD_OFFICIALMOBILE)) > 0)
	{
		XEdit_GetText(XWnd_GetEle(g_hWindow, IDE_ADD_OFFICIALMOBILE), wBuf, 256);
		W_2_S(wBuf, szTmp, 256);	
		sprintf(szT, "select official_name,official_sex,official_office_phone,official_mobile,official_office_name,official_office_addr,official_duty,\
			official_SSO,official_NO,official_room,official_IP,official_id from t_official where official_mobile = \'%s\'", 
			szTmp);
	}
	else if (XEdit_GetTextLength(XWnd_GetEle(g_hWindow, IDE_ADD_OFFICEADDR)) > 0)
	{
		XEdit_GetText(XWnd_GetEle(g_hWindow, IDE_ADD_OFFICEADDR), wBuf, 256);
		W_2_S(wBuf, szTmp, 256);	
		sprintf(szT, "select official_name,official_sex,official_office_phone,official_mobile,official_office_name,official_office_addr,official_duty,\
			official_SSO,official_NO,official_room,official_IP,official_id from t_official where official_office_addr = \'%s\'", 
			szTmp);
	}
	else if (XEdit_GetTextLength(XWnd_GetEle(g_hWindow, IDC_ADD_OFFICIALZHIWU)) > 0)
	{
		XEdit_GetText(XWnd_GetEle(g_hWindow, IDC_ADD_OFFICIALZHIWU), wBuf, 256);
		W_2_S(wBuf, szTmp, 256);	
		sprintf(szT, "select official_name,official_sex,official_office_phone,official_mobile,official_office_name,official_office_addr,official_duty,\
			official_SSO,official_NO,official_room,official_IP,official_id from t_official where official_duty = \'%s\'", 
			szTmp);
	}
	else if (XEdit_GetTextLength(XWnd_GetEle(g_hWindow, IDE_ADD_ROOMNUM)) > 0)
	{
		XEdit_GetText(XWnd_GetEle(g_hWindow, IDE_ADD_ROOMNUM), wBuf, 256);
		W_2_S(wBuf, szTmp, 256);	
		sprintf(szT, "select official_name,official_sex,official_office_phone,official_mobile,official_office_name,official_office_addr,official_duty,\
			official_SSO,official_NO,official_room,official_IP,official_id from t_official where official_room = \'%s\'", 
			szTmp);
	}
	else if (XEdit_GetTextLength(XWnd_GetEle(g_hWindow, IDE_ADD_IP)) > 0)
	{
		XEdit_GetText(XWnd_GetEle(g_hWindow, IDE_ADD_IP), wBuf, 256);
		W_2_S(wBuf, szTmp, 256);	
		sprintf(szT, "select official_name,official_sex,official_office_phone,official_mobile,official_office_name,official_office_addr,official_duty,\
			official_SSO,official_NO,official_room,official_IP,official_id from t_official where official_IP = \'%s\'", 
			szTmp);
	}
	else if (XEdit_GetTextLength(XWnd_GetEle(g_hWindow, IDE_ADD_SSO)) > 0)
	{
		XEdit_GetText(XWnd_GetEle(g_hWindow, IDE_ADD_SSO), wBuf, 256);
		W_2_S(wBuf, szTmp, 256);	
		sprintf(szT, "select official_name,official_sex,official_office_phone,official_mobile,official_office_name,official_office_addr,official_duty,\
			official_SSO,official_NO,official_room,official_IP,official_id from t_official where official_SSO = \'%s\'", 
			szTmp);
	}
	else
	{
		strcpy(szT, "select official_name,official_sex,official_office_phone,official_mobile,official_office_name,official_office_addr,official_duty,\
			official_SSO,official_NO,official_room,official_IP,official_id from t_official");	//��ѯ���б����˵���Ϣ		
//		XEdit_GetText(hE, wBuf, 256);
//		W_2_S(wBuf, szOfficialName, 256);	
//		sprintf(szT, "select official_name,official_sex,official_office_phone,official_mobile,official_office_name,official_office_addr,official_duty,official_room,official_IP,official_id from t_official where official_name = \'%s\'", szOfficialName);	//��ѯ�����˵���Ϣ
	}
    ret = tAdo.execSelect(szT, &FldCount, outParas);

	HELE hList = XWnd_GetEle(g_hWindow, IDL_LISTOFFICIAL);		//����б�Ԫ��
	
	//XList_DeleteAllColumns(hList);
	XList_DeleteAllItems(hList);
	
	/*XList_SetHeaderHeight(hList, 30);
	XList_EnableGrid(hList, false);

	XList_AddColumn(hList, 40, L"���");
	XList_AddColumn(hList, 100, L"����");
	XList_AddColumn(hList, 40, L"�Ա�");
	XList_AddColumn(hList, 100, L"�칫�绰");
	XList_AddColumn(hList, 100, L"�ƶ��绰");
	XList_AddColumn(hList, 140, L"���ſ���");
	XList_AddColumn(hList, 160, L"�칫�ص�");
	XList_AddColumn(hList, 70, L"����");
	XList_AddColumn(hList, 70, L"����");
	XList_AddColumn(hList, 90, L"IP��ַ");*/
	if (ret == ADOERR || (FldCount == 0 && outParas[0].IsEmpty()))
	{
//		XMessageBox(g_hWindow,L"����Ϣ",L"��ʾ");
		XWnd_RedrawWnd(XEle_GetHWindow(hList));
		return false;
	}	
	int n = 0;
	do
	{
		memset((char *)wBuf, 0, sizeof(wBuf));
		//memset(szT, 0, sizeof(szT));
		//sprintf(szT, "%d", n + 1);		//���
		S_2_W(wBuf, (LPSTR)(LPCTSTR)outParas[11], 256);
		XList_AddItem(hList, wBuf, n);

		L_V_I(outParas[0], n, 1)	//����
		L_V_I(outParas[1], n, 2)	//�Ա�
		L_V_I(outParas[2], n, 3)	//�������˰칫�绰
		L_V_I(outParas[3], n, 4)	//���������ֻ�
		L_V_I(outParas[4], n, 5)	//�������˰칫������
		L_V_I(outParas[5], n, 6)	//�������˰칫�ҵ�ַ
		L_V_I(outParas[6], n, 7)	//�������˼���
		L_V_I(outParas[9], n, 8)	//����

		L_V_I(outParas[7], n, 9)	//SSO
		L_V_I(outParas[8], n, 10)	//NO

		L_V_I(outParas[10], n, 11)	//IP��ַ

		n++;
	} while (tAdo.getNext(outParas) == ADOOK) ;

	//XList_SetSelectItem(hList, 0);
	g_nTableType = TABLE_OFFICIAL;

	XWnd_RedrawWnd(XEle_GetHWindow(hEle));

	return false;
}

//ϵͳ����ҳ���е�������Ա��Ϣ
BOOL CALLBACK OnAddOfficialInfo(HELE hEle, HELE hEventEle)
{
	char szOfficialName[32] = {'\0'};
	wchar_t wBuf[256 + 1] = {0};
	
	char szT[512] = {'\0'};
	long ret = 0;;
	CWinAdo tAdo(sCONNSTR_MYSQL);
	
	ret = tAdo.OpenAdo();
	if (ret != ADOOK)
	{
		return false;
	}
	
	char szName[32] = {'\0'}, szSex[8] = {'\0'}, szPhone[32] = {'\0'}, szMobile[32]= {'\0'};
	char szOfficeName[128] = {'\0'}, szOfficeAddr[128] = {'\0'}, szLevel[32] = {'\0'}, szRoom[32] = {'\0'},szIP[20]={'\0'};
	
	memset((char *)wBuf, 0, sizeof(wBuf));
	HELE hE = XWnd_GetEle(g_hWindow, IDE_ADD_OFFICIALNAME);
	if (XEdit_GetTextLength(XWnd_GetEle(g_hWindow, IDE_ADD_OFFICIALNAME)) > 10)
	{
		XMessageBox(g_hWindow, L"����̫��", L"����");
		return false;
	}
	XEdit_GetText(hE, wBuf, 256);
	W_2_S(wBuf, szName, 256);
	
	memset((char *)wBuf, 0, sizeof(wBuf));
	hE = XWnd_GetEle(g_hWindow, IDC_ADD_OFFICIALSEX);
	XEdit_GetText(hE, wBuf, 256);
	W_2_S(wBuf, szSex, 256);
	
	memset((char *)wBuf, 0, sizeof(wBuf));
	hE = XWnd_GetEle(g_hWindow, IDE_ADD_OFFICEPHONE);
	if (XEdit_GetTextLength(XWnd_GetEle(g_hWindow, IDE_ADD_OFFICEPHONE)) > 10)
	{
		XMessageBox(g_hWindow, L"�칫�绰��̫��", L"����");
		return false;
	}
	XEdit_GetText(hE, wBuf, 256);
	W_2_S(wBuf, szPhone, 256);
	
	memset((char *)wBuf, 0, sizeof(wBuf));
	hE = XWnd_GetEle(g_hWindow, IDE_ADD_OFFICIALMOBILE);
	if (XEdit_GetTextLength(XWnd_GetEle(g_hWindow, IDE_ADD_OFFICIALMOBILE)) > 20)
	{
		XMessageBox(g_hWindow, L"�ֻ���̫��", L"����");
		return false;
	}
	XEdit_GetText(hE, wBuf, 256);
	W_2_S(wBuf, szMobile, 256);
	
	memset((char *)wBuf, 0, sizeof(wBuf));
	hE = XWnd_GetEle(g_hWindow, IDE_ADD_OFFICENAME);
	if (XEdit_GetTextLength(XWnd_GetEle(g_hWindow, IDE_ADD_OFFICENAME)) > 20)
	{
		XMessageBox(g_hWindow, L"�칫������̫��", L"����");
		return false;
	}
	XEdit_GetText(hE, wBuf, 256);
	W_2_S(wBuf, szOfficeName, 256);
	
	memset((char *)wBuf, 0, sizeof(wBuf));
	hE = XWnd_GetEle(g_hWindow, IDE_ADD_OFFICEADDR);
	if (XEdit_GetTextLength(XWnd_GetEle(g_hWindow, IDE_ADD_OFFICEADDR)) > 15)
	{
		XMessageBox(g_hWindow, L"�칫�ҵ�ַ̫��", L"����");
		return false;
	}
	XEdit_GetText(hE, wBuf, 256);
	W_2_S(wBuf, szOfficeAddr, 256);
	
	memset((char *)wBuf, 0, sizeof(wBuf));
	hE = XWnd_GetEle(g_hWindow, IDC_ADD_OFFICIALZHIWU);
	if (XEdit_GetTextLength(XWnd_GetEle(g_hWindow, IDC_ADD_OFFICIALZHIWU)) > 10)
	{
		XMessageBox(g_hWindow, L"��������̫��", L"����");
		return false;
	}
	XEdit_GetText(hE, wBuf, 256);
	W_2_S(wBuf, szLevel, 256);
	
	memset((char *)wBuf, 0, sizeof(wBuf));
	hE = XWnd_GetEle(g_hWindow, IDE_ADD_ROOMNUM);
	if (XEdit_GetTextLength(XWnd_GetEle(g_hWindow, IDE_ADD_ROOMNUM)) > 10)
	{
		XMessageBox(g_hWindow, L"�����̫��", L"����");
		return false;
	}
	XEdit_GetText(hE, wBuf, 256);
	W_2_S(wBuf, szRoom, 256);

	memset((char *)wBuf, 0, sizeof(wBuf));
	hE = XWnd_GetEle(g_hWindow, IDE_ADD_IP);
	XEdit_GetText(hE, wBuf, 256);
	if (XEdit_GetTextLength(XWnd_GetEle(g_hWindow, IDE_ADD_IP)) > 20)
	{
		XMessageBox(g_hWindow, L"IP��ַ̫��", L"����");
		return false;
	}
	W_2_S(wBuf, szIP, 256);
	
	if (strlen(szName) <= 0)
	{
		XMessageBox(g_hWindow, L"���ӵ���Ա��������Ϊ��!", L"����");
		return false;
	}
	
	sprintf(szT, "insert into t_official (official_name,official_sex,official_office_phone,official_mobile,official_office_name,official_office_addr,official_duty,official_room,official_IP,official_state,official_pwd,official_NO) \
		values('\%s\', '\%s\', '\%s\', '\%s\', '\%s\', '\%s\', '\%s\', '\%s\', '\%s\',0,'123','')",szName, szSex, szPhone, szMobile, szOfficeName, szOfficeAddr, szLevel, szRoom,szIP);
	ret = tAdo.execUpdateInsert(szT);
  	if (ret == ADOOK)
	{
		HELE hE_txt = XWnd_GetEle(g_hWindow, IDE_ADD_OFFICIALNAME);
		XEdit_SetText(hE_txt,L"");
		OnQueryEmployerForList(XWnd_GetEle(g_hWindow, IDB_QUERYOFFICIAL), XWnd_GetEle(g_hWindow, IDB_QUERYOFFICIAL));
	}

	XEdit_SetText(XWnd_GetEle(g_hWindow, IDE_ADD_OFFICIALNAME),L"");
	XEdit_SetText(XWnd_GetEle(g_hWindow, IDC_ADD_OFFICIALSEX),L"");
	XEdit_SetText(XWnd_GetEle(g_hWindow, IDE_ADD_OFFICEPHONE),L"");
	XEdit_SetText(XWnd_GetEle(g_hWindow, IDE_ADD_OFFICENAME),L"");
	XEdit_SetText(XWnd_GetEle(g_hWindow, IDE_ADD_OFFICIALMOBILE),L"");
	XEdit_SetText(XWnd_GetEle(g_hWindow, IDE_ADD_OFFICEADDR),L"");
	XEdit_SetText(XWnd_GetEle(g_hWindow, IDC_ADD_OFFICIALZHIWU),L"");
	XEdit_SetText(XWnd_GetEle(g_hWindow, IDE_ADD_ROOMNUM),L"");
	XEdit_SetText(XWnd_GetEle(g_hWindow, IDE_ADD_IP),L"");
	
	XWnd_RedrawWnd(XEle_GetHWindow(hEle));
	return false;
}
//�������б�
BOOL CALLBACK OnOfficialList(HELE hEle, HELE hEventEle, int nIndex)
{
	if (hEle != hEventEle)
	{
		return false;
	}
	if (nIndex < 0)
	{
		return false;
	}

	wchar_t wBuf[256] = {0};
	int n = nIndex;
	memset((char *)wBuf, 0, sizeof(wBuf));
	wcscpy(wBuf, XList_GetItemText(hEle, n, 1));
	XEdit_SetText(XWnd_GetEle(g_hWindow, IDE_ADD_OFFICIALNAME), wBuf);

		
	memset((char *)wBuf, 0, sizeof(wBuf));
	wcscpy(wBuf, XList_GetItemText(hEle, n, 2));
	XEdit_SetText(XWnd_GetEle(g_hWindow, IDC_ADD_OFFICIALSEX), wBuf);

	memset((char *)wBuf, 0, sizeof(wBuf));
	wcscpy(wBuf, XList_GetItemText(hEle, n, 3));
	XEdit_SetText(XWnd_GetEle(g_hWindow, IDE_ADD_OFFICEPHONE), wBuf);

	memset((char *)wBuf, 0, sizeof(wBuf));
	wcscpy(wBuf, XList_GetItemText(hEle, n, 4));
	XEdit_SetText(XWnd_GetEle(g_hWindow, IDE_ADD_OFFICIALMOBILE), wBuf);

	memset((char *)wBuf, 0, sizeof(wBuf));
	wcscpy(wBuf, XList_GetItemText(hEle, n, 5));
	XEdit_SetText(XWnd_GetEle(g_hWindow, IDE_ADD_OFFICENAME), wBuf);

	memset((char *)wBuf, 0, sizeof(wBuf));
	wcscpy(wBuf, XList_GetItemText(hEle, n, 6));
	XEdit_SetText(XWnd_GetEle(g_hWindow, IDE_ADD_OFFICEADDR), wBuf);

	memset((char *)wBuf, 0, sizeof(wBuf));
	wcscpy(wBuf, XList_GetItemText(hEle, n, 7));
	XEdit_SetText(XWnd_GetEle(g_hWindow, IDC_ADD_OFFICIALZHIWU), wBuf);

	memset((char *)wBuf, 0, sizeof(wBuf));
	wcscpy(wBuf, XList_GetItemText(hEle, n, 8));
	XEdit_SetText(XWnd_GetEle(g_hWindow, IDE_ADD_ROOMNUM), wBuf);
		
	memset((char *)wBuf, 0, sizeof(wBuf));
	wcscpy(wBuf, XList_GetItemText(hEle, n, 11));
	XEdit_SetText(XWnd_GetEle(g_hWindow, IDE_ADD_IP), wBuf);

	memset((char *)wBuf, 0, sizeof(wBuf));
	wcscpy(wBuf, XList_GetItemText(hEle, n, 9));
	XEdit_SetText(XWnd_GetEle(g_hWindow, IDE_ADD_SSO), wBuf);


	memset(g_szOfficialID, 0, sizeof(g_szOfficialID));
	memset((char *)wBuf, 0, sizeof(wBuf));
	wcscpy(wBuf, XList_GetItemText(hEle, n, 10));
	if(wcslen(wBuf) > 0)
	{
		XEle_ShowEle(XWnd_GetEle(g_hWindow,IDB_ADDBINDERIC),true);
	}
	else
	{
		XEle_ShowEle(XWnd_GetEle(g_hWindow,IDB_ADDBINDERIC),false);
		//��ȡ������id�ţ��Ա��ic����
		memset((char *)wBuf, 0, sizeof(wBuf));
		wcscpy(wBuf, XList_GetItemText(hEle, n, 0));
		W_2_S(wBuf,g_szOfficialID,128);
	}
	XWnd_RedrawWnd(XEle_GetHWindow(hEle));
	return  false;
}
//ϵͳ����ҳ���е�ɾ����Ա��Ϣ
BOOL CALLBACK OnDeleteOfficial(HELE hEle, HELE hEventEle)
{
	char szOfficialName[32] = {'\0'};
	wchar_t wBuf[256 + 1] = {0};
	
	char szT[512] = {'\0'};
	long ret = 0;;
	CWinAdo tAdo(sCONNSTR_MYSQL);
	
	ret = tAdo.OpenAdo();
	if (ret != ADOOK)
	{
		return false;
	}
	
	char szData[32] = {'\0'};
	int n = 0,m = 0,*nSel;

	memset((char *)wBuf, 0, sizeof(wBuf));
	HELE hList = XWnd_GetEle(g_hWindow, IDL_LISTOFFICIAL);

	m = XList_GetSelectCount(hList);
	if (m < 0)
	{
		return false;
	}    
	nSel = new int[m];
	XList_GetSelectAllItem(hList,nSel,m);

	
/*	if (strlen(szData) <= 0)
	{
		XMessageBox(g_hWindow, L"ɾ������Ա��������Ϊ��!", L"����");
		return false;
	}
	else*/
	ret = XMessageBox(g_hWindow, L"���潫ɾ��ѡ�е���Ա��Ϣ,��[ȷ��]����,��[ȡ��]����", L"��ʾ��Ϣ", XMB_OK | XMB_CANCEL);
	if (ret == XMB_CANCEL)
	{
		delete []nSel;
		return false;
	}

	for (int i=0;i<m;i++)
	{
		wcscpy(wBuf, XList_GetItemText(hList, nSel[i], 0));		//���
	    W_2_S(wBuf, szData, 256);
        sprintf(szT, "delete from t_official where official_id = \'%s\'", szData);
	    ret = tAdo.execUpdateInsert(szT);
	}
	delete []nSel;
	if (ret == ADOOK)
	{
		HELE hE_txt = XWnd_GetEle(g_hWindow, IDE_ADD_OFFICIALNAME);
		XEdit_SetText(hE_txt,L"");
		hE_txt = XWnd_GetEle(g_hWindow, IDC_ADD_OFFICIALSEX);
        XEdit_SetText(hE_txt,L"");
        hE_txt = XWnd_GetEle(g_hWindow, IDE_ADD_OFFICEPHONE);
		XEdit_SetText(hE_txt,L"");
        hE_txt = XWnd_GetEle(g_hWindow, IDE_ADD_OFFICENAME);
		XEdit_SetText(hE_txt,L"");
        hE_txt = XWnd_GetEle(g_hWindow, IDE_ADD_OFFICIALMOBILE);
		XEdit_SetText(hE_txt,L"");
        hE_txt = XWnd_GetEle(g_hWindow, IDE_ADD_OFFICEADDR);
		XEdit_SetText(hE_txt,L"");
        hE_txt = XWnd_GetEle(g_hWindow, IDC_ADD_OFFICIALZHIWU);
		XEdit_SetText(hE_txt,L"");
        hE_txt = XWnd_GetEle(g_hWindow, IDE_ADD_ROOMNUM);
		XEdit_SetText(hE_txt,L"");
        hE_txt = XWnd_GetEle(g_hWindow, IDE_ADD_IP);
		XEdit_SetText(hE_txt,L"");

		OnQueryEmployerForList(XWnd_GetEle(g_hWindow, IDB_QUERYOFFICIAL), XWnd_GetEle(g_hWindow, IDB_QUERYOFFICIAL));
		XList_SetSelectItem(hList, 0);
	}

	XWnd_RedrawWnd(XEle_GetHWindow(hEle));
	return false;
}

//ϵͳ����ҳ���еĲ�ѯ��������Ϣ
BOOL CALLBACK OnQueryBlack(HELE hEle, HELE hEventEle)
{
	QueryBlack();
	
	XWnd_RedrawWnd(XEle_GetHWindow(hEle));
	return false;
}

BOOL CALLBACK OnOfficeLevel(HELE hEle, HELE hEventEle, int nIndex)
{
	if (hEle != hEventEle)
	{
		return false;
	}
	if (nIndex < 0)
	{
		return false;
	}

	wchar_t wBuf[256] = {0},*wHead=NULL;
	int n = nIndex;

	wHead = XList_GetHeaderItemText(hEle,1);
	if(wcscmp(wHead,L"��������")==0)
	{
		memset((char *)wBuf, 0, sizeof(wBuf));
		wcscpy(wBuf, XList_GetItemText(hEle, n, 1));
		XEdit_SetText(XWnd_GetEle(g_hWindow, IDE_ADD_OFFICE_NAME), wBuf);
		
		memset((char *)wBuf, 0, sizeof(wBuf));
		wcscpy(wBuf, XList_GetItemText(hEle, n, 2));
		XEdit_SetText(XWnd_GetEle(g_hWindow, IDE_ADD_OFFICE_PHONE), wBuf);

		memset((char *)wBuf, 0, sizeof(wBuf));
		wcscpy(wBuf, XList_GetItemText(hEle, n, 3));
		XEdit_SetText(XWnd_GetEle(g_hWindow, IDE_ADD_OFFICE_ADDR), wBuf);

		memset((char *)wBuf, 0, sizeof(wBuf));
		wcscpy(wBuf, XList_GetItemText(hEle, n, 4));
		XEdit_SetText(XWnd_GetEle(g_hWindow, IDE_ADD_OFFICE_ROOMNUM), wBuf);
	}	
	else if (wcscmp(wHead,L"��������")==0)
	{
	    memset((char *)wBuf, 0, sizeof(wBuf));
		wcscpy(wBuf, XList_GetItemText(hEle, n, 1));
		XEdit_SetText(XWnd_GetEle(g_hWindow, IDE_ADD_LEVELNAME), wBuf);
	}
	/*else if (wcscmp(wHead,L"��������")==0)
	{
		memset((char *)wBuf, 0, sizeof(wBuf));
		wcscpy(wBuf, XList_GetItemText(hEle, n, 1));
		XEdit_SetText(XWnd_GetEle(g_hWindow, IDE_ADD_REASONNAME), wBuf);
	}
	else if (wcscmp(wHead,L"����Ա��")==0)
	{
		memset((char *)wBuf, 0, sizeof(wBuf));
		wcscpy(wBuf, XList_GetItemText(hEle, n, 1));
		XEdit_SetText(XWnd_GetEle(g_hWindow, IDE_ADD_CZYID), wBuf);
			
		memset((char *)wBuf, 0, sizeof(wBuf));
		//wcscpy(wBuf, XList_GetItemText(hEle, n, 3));
		XEdit_SetText(XWnd_GetEle(g_hWindow, IDE_ADD_CZYMM), L"*******");
			
		memset((char *)wBuf, 0, sizeof(wBuf));
		wcscpy(wBuf, XList_GetItemText(hEle, n, 2));
		XEdit_SetText(XWnd_GetEle(g_hWindow, IDE_ADD_CZYRIGHT), wBuf);		
	}*/
	XWnd_RedrawWnd(XEle_GetHWindow(hEle));
	return false;
}
BOOL CALLBACK OnGoodsReasonStationUser(HELE hEle, HELE hEventEle, int nIndex)
{
	if (hEle != hEventEle)
	{
		return false;
	}
	if (nIndex < 0)
	{
		return false;
	}

	wchar_t wBuf[256] = {0},*wHead=NULL;
	int n = nIndex;

	wHead = XList_GetHeaderItemText(hEle,1);
	if(wcscmp(wHead,L"��Ʒ����")==0)
	{
		memset((char *)wBuf, 0, sizeof(wBuf));
		wcscpy(wBuf, XList_GetItemText(hEle, n, 1));
		XEdit_SetText(XWnd_GetEle(g_hWindow, IDE_ADD_GOODSNAME), wBuf);
	}	
	else if (wcscmp(wHead,L"�Ÿ�")==0)
	{
	    memset((char *)wBuf, 0, sizeof(wBuf));
		wcscpy(wBuf, XList_GetItemText(hEle, n, 1));
		XEdit_SetText(XWnd_GetEle(g_hWindow, IDE_ADD_STATIONNAME), wBuf);
	}
	else if (wcscmp(wHead,L"��������")==0)
	{
		memset((char *)wBuf, 0, sizeof(wBuf));
		wcscpy(wBuf, XList_GetItemText(hEle, n, 1));
		XEdit_SetText(XWnd_GetEle(g_hWindow, IDE_ADD_REASONNAME), wBuf);
	}
	else if (wcscmp(wHead,L"����Ա��")==0)
	{
		memset((char *)wBuf, 0, sizeof(wBuf));
		wcscpy(wBuf, XList_GetItemText(hEle, n, 1));
		XEdit_SetText(XWnd_GetEle(g_hWindow, IDE_ADD_CZYID), wBuf);
			
		memset((char *)wBuf, 0, sizeof(wBuf));
		//wcscpy(wBuf, XList_GetItemText(hEle, n, 3));
		XEdit_SetText(XWnd_GetEle(g_hWindow, IDE_ADD_CZYMM), L"*******");
			
		memset((char *)wBuf, 0, sizeof(wBuf));
		wcscpy(wBuf, XList_GetItemText(hEle, n, 2));
		XEdit_SetText(XWnd_GetEle(g_hWindow, IDE_ADD_CZYRIGHT), wBuf);		
	}
/*	switch(g_nTableType)
	{
	case TABLE_GOODS:
		{
			memset((char *)wBuf, 0, sizeof(wBuf));
			wcscpy(wBuf, XList_GetItemText(hEle, n, 1));
			XEdit_SetText(XWnd_GetEle(g_hWindow, IDE_ADD_GOODSNAME), wBuf);
		}
		break;
	case TABLE_REASON:
		{
			memset((char *)wBuf, 0, sizeof(wBuf));
			wcscpy(wBuf, XList_GetItemText(hEle, n, 1));
			XEdit_SetText(XWnd_GetEle(g_hWindow, IDE_ADD_REASONNAME), wBuf);
		}
		break;
	case TABLE_STATION:
		{
			memset((char *)wBuf, 0, sizeof(wBuf));
			wcscpy(wBuf, XList_GetItemText(hEle, n, 1));
			XEdit_SetText(XWnd_GetEle(g_hWindow, IDE_ADD_STATIONNAME), wBuf);
		}	
		break;
	case TABLE_CZY:
		{
			memset((char *)wBuf, 0, sizeof(wBuf));
			wcscpy(wBuf, XList_GetItemText(hEle, n, 1));
			XEdit_SetText(XWnd_GetEle(g_hWindow, IDE_ADD_CZYID), wBuf);
			
			memset((char *)wBuf, 0, sizeof(wBuf));
			//wcscpy(wBuf, XList_GetItemText(hEle, n, 3));
			XEdit_SetText(XWnd_GetEle(g_hWindow, IDE_ADD_CZYMM), L"*******");
			
			memset((char *)wBuf, 0, sizeof(wBuf));
			wcscpy(wBuf, XList_GetItemText(hEle, n, 2));
			XEdit_SetText(XWnd_GetEle(g_hWindow, IDE_ADD_CZYRIGHT), wBuf);
		}
	    break;
	default:
		break;
	}*/
	XWnd_RedrawWnd(XEle_GetHWindow(hEle));
	return true;
}

BOOL CALLBACK OnBlackNameList(HELE hEle, HELE hEventEle, int nIndex)
{
	if (hEle != hEventEle)
	{
		return false;
	}
	if (nIndex < 0)
	{
		return false;
	}

	wchar_t wBuf[256] = {0};
	int n = nIndex;
	memset((char *)wBuf, 0, sizeof(wBuf));
	wcscpy(wBuf, XList_GetItemText(hEle, n, 1));
	XEdit_SetText(XWnd_GetEle(g_hWindow, IDE_ADD_BLACKNAME), wBuf);

		
	memset((char *)wBuf, 0, sizeof(wBuf));
	wcscpy(wBuf, XList_GetItemText(hEle, n, 2));
	XEdit_SetText(XWnd_GetEle(g_hWindow, IDE_ADD_BLACKALIAS), wBuf);

	memset((char *)wBuf, 0, sizeof(wBuf));
	wcscpy(wBuf, XList_GetItemText(hEle, n, 3));
	XEdit_SetText(XWnd_GetEle(g_hWindow, IDC_ADD_BLACKSEX), wBuf);

/*	memset((char *)wBuf, 0, sizeof(wBuf));
	wcscpy(wBuf, XList_GetItemText(hEle, n, 4));
	XEdit_SetText(XWnd_GetEle(g_hWindow, IDE_ADD_BLACKMOBILE), wBuf);*/

	memset((char *)wBuf, 0, sizeof(wBuf));
	wcscpy(wBuf, XList_GetItemText(hEle, n, 5));
	XEdit_SetText(XWnd_GetEle(g_hWindow, IDE_ADD_BLACKADDR), wBuf);

	memset((char *)wBuf, 0, sizeof(wBuf));
	wcscpy(wBuf, XList_GetItemText(hEle, n, 6));
	XEdit_SetText(XWnd_GetEle(g_hWindow, IDE_ADD_BLACKMOBILE), wBuf);

	CString outP1[1];
	long FldCount = 0,ret=0;
	CWinAdo tAdo(sCONNSTR_MYSQL), tAdo1(sCONNSTR_MYSQL);
	
	ret = tAdo.OpenAdo();
	if (ret != ADOOK)
	{
		return false;
	}
	char szIDtype[20],szT[100]={'\0'};
	memset((char *)wBuf, 0, sizeof(wBuf));
	wcscpy(wBuf, XList_GetItemText(hEle, n, 7));
    W_2_S(wBuf,szIDtype,256);
	sprintf(szT, "select type_id from t_id_type where type_name = \'%s\'", szIDtype);
	ret = tAdo.execSelect(szT, &FldCount, outP1);
	/*if (ret == ADOERR || (FldCount == 0 && outP1[0].IsEmpty()))
	{
		continue;
	}*/
	FldCount=atoi(outP1[0]);
	if (FldCount>0&&FldCount<8)
	{
        XComboBox_SetSelectItem(XWnd_GetEle(g_hWindow, IDC_ADD_BLACKIDTYPE), FldCount-1);
	}
	else
	{
		XComboBox_SetSelectItem(XWnd_GetEle(g_hWindow, IDC_ADD_BLACKIDTYPE), 7);
	}


	//XEdit_SetText(XWnd_GetEle(g_hWindow, IDC_ADD_BLACKIDTYPE), wBuf); 

	memset((char *)wBuf, 0, sizeof(wBuf));
	wcscpy(wBuf, XList_GetItemText(hEle, n, 8));
	XEdit_SetText(XWnd_GetEle(g_hWindow, IDE_ADD_BLACKID), wBuf);

	memset((char *)wBuf, 0, sizeof(wBuf));
	wcscpy(wBuf, XList_GetItemText(hEle, n, 9));
	XEdit_SetText(XWnd_GetEle(g_hWindow, IDE_ADD_BLACKUNIT), wBuf);

	memset((char *)wBuf, 0, sizeof(wBuf));
	wcscpy(wBuf, XList_GetItemText(hEle, n, 10));
	XEdit_SetText(XWnd_GetEle(g_hWindow, IDE_ADD_BLACKCARNUM), wBuf);

    XWnd_RedrawWnd(XEle_GetHWindow(hEle));
	return true;
}
BOOL QueryBlack()
{
	char szBlackName[32] = {'\0'};
	wchar_t wBuf[256 + 1] = {0};
	
	char szT[512] = {'\0'};
	long FldCount = 0, ret = 0;
	CString outParas[FLD_MAX], outP1[FLD_MAX];
	CWinAdo tAdo(sCONNSTR_MYSQL), tAdo1(sCONNSTR_MYSQL);
	
	ret = tAdo.OpenAdo();
	if (ret != ADOOK)
	{
		return false;
	}

	ret = tAdo1.OpenAdo();
	if (ret != ADOOK)
	{
		return false;
	}
	
	for (int i = 0; i < FLD_MAX; i++)
	{
		outParas[i].Empty();
		outP1[i].Empty();
	}

	HELE hList = XWnd_GetEle(g_hWindow, IDL_LISTBLACK);		//����б�Ԫ��
	
	XList_DeleteAllColumns(hList);
	XList_DeleteAllItems(hList);
	
	XList_SetHeaderHeight(hList, 30);
	XList_EnableGrid(hList, false);

	HELE hE = XWnd_GetEle(g_hWindow, IDE_ADD_BLACKNAME);
	if (XEdit_GetTextLength(hE) <= 0)
	{
		strcpy(szT, "select * from t_blackname");	//��ѯ���к���������Ϣ
	}
	else
	{
		
		XEdit_GetText(hE, wBuf, 256);
		W_2_S(wBuf, szBlackName, 256);	
		sprintf(szT, "select * from t_blackname where black_name = \'%s\' or black_alias = \'%s\'", szBlackName, szBlackName);	//��ѯ�����˵���Ϣ
	}
	
	ret = tAdo.execSelect(szT, &FldCount, outParas);
	
	XList_AddColumn(hList, 40, L"���");
	XList_AddColumn(hList, 80, L"����");
	XList_AddColumn(hList, 80, L"������");
	XList_AddColumn(hList, 40, L"�Ա�");
	XList_AddColumn(hList, 40, L"����");
	XList_AddColumn(hList, 100, L"��ַ");
	XList_AddColumn(hList, 60, L"�绰");
	XList_AddColumn(hList, 80, L"֤������");
	XList_AddColumn(hList, 100, L"֤������");
	XList_AddColumn(hList, 160, L"��λ����");
	XList_AddColumn(hList, 60, L"���ƺ�");
	if (ret == ADOERR || (FldCount == 0 && outParas[0].IsEmpty()))
	{
//		XMessageBox(g_hWindow,L"����Ϣ",L"��ʾ");
		XWnd_RedrawWnd(XEle_GetHWindow(hList));
		return false;
	}
	int n = 0;
	do
	{
		memset((char *)wBuf, 0, sizeof(wBuf));
		memset(szT, 0, sizeof(szT));
		sprintf(szT, "%d", n + 1);		//���
		S_2_W(wBuf, szT, 256);
		XList_AddItem(hList, wBuf, n);
		
		L_V_I(outParas[0], n, 1)	//����
		L_V_I(outParas[1], n, 2)	//������
		L_V_I(outParas[2], n, 3)	//�Ա�
		L_V_I(outParas[3], n, 4)	//����
		L_V_I(outParas[4], n, 5)	//��ַ
		L_V_I(outParas[5], n, 6)	//�绰

		sprintf(szT, "select type_name from t_id_type where type_id = \'%s\'", outParas[6]);
		ret = tAdo1.execSelect(szT, &FldCount, outP1);
		if (ret == ADOERR || (FldCount == 0 && outP1[0].IsEmpty()))
		{
			continue;
		}
		L_V_I(outP1[0], n, 7)		//֤������


		L_V_I(outParas[7], n, 8)	//֤������
		L_V_I(outParas[8], n, 9)	//��λ����
		L_V_I(outParas[9], n, 10)	//���ƺ�

		n++;
	} while (tAdo.getNext(outParas) == ADOOK) ;

	//XList_SetSelectItem(hList, 0);
	g_nTableType = TABLE_BLACKNAME;

	return false;
}

//ϵͳ����ҳ���е����Ӻ�������Ϣ
BOOL CALLBACK OnAddBlack(HELE hEle, HELE hEventEle)
{
	char szOfficialName[32] = {'\0'};
	wchar_t wBuf[256 + 1] = {0};
	
	char szT[512] = {'\0'};
	long FldCount = 0, ret = 0;
	CString outParas[FLD_MAX];
	CWinAdo tAdo(sCONNSTR_MYSQL);
	
	ret = tAdo.OpenAdo();
	if (ret != ADOOK)
	{
		return false;
	}
	for (int i = 0; i < FLD_MAX; i++)
	{
		outParas[i].Empty();
	}
	
	char szName[32] = {'\0'}, szAlias[32] = {'\0'}, szSex[8] = {'\0'}, szMobile[32]= {'\0'};
	char szAddr[128] = {'\0'}, szIDType[32] = {'\0'}, szID[32] = {'\0'}, szUnit[FLD_MAX] = {'\0'};
	char szCarNum[32] = {'\0'}, szPic[256] = {'\0'};
	int n = 99;
	
	memset((char *)wBuf, 0, sizeof(wBuf));
	HELE hE = XWnd_GetEle(g_hWindow, IDE_ADD_BLACKNAME);
	if (XEdit_GetTextLength(hE) > 10)
	{
		XMessageBox(g_hWindow, L"����̫��", L"����");
		return false;
	}
	XEdit_GetText(hE, wBuf, 256);
	W_2_S(wBuf, szName, 256);

	memset((char *)wBuf, 0, sizeof(wBuf));
	hE = XWnd_GetEle(g_hWindow, IDE_ADD_BLACKALIAS);
	if (XEdit_GetTextLength(hE) > 10)
	{
		XMessageBox(g_hWindow, L"����̫��", L"����");
		return false;
	}
	XEdit_GetText(hE, wBuf, 256);
	W_2_S(wBuf, szAlias, 256);
	
	memset((char *)wBuf, 0, sizeof(wBuf));
	hE = XWnd_GetEle(g_hWindow, IDC_ADD_BLACKSEX);
	XComboBox_GetText(hE, wBuf, 256);
	W_2_S(wBuf, szSex, 256);
	
	memset((char *)wBuf, 0, sizeof(wBuf));
	hE = XWnd_GetEle(g_hWindow, IDE_ADD_BLACKMOBILE);
	if (XEdit_GetTextLength(hE) > 20)
	{
		XMessageBox(g_hWindow, L"�绰����̫��", L"����");
		return false;
	}
	XEdit_GetText(hE, wBuf, 256);
	W_2_S(wBuf, szMobile, 256);

	memset((char *)wBuf, 0, sizeof(wBuf));
	hE = XWnd_GetEle(g_hWindow, IDE_ADD_BLACKADDR);
	if (XEdit_GetTextLength(hE) > 50)
	{
		XMessageBox(g_hWindow, L"סַ̫��", L"����");
		return false;
	}
	XEdit_GetText(hE, wBuf, 256);
	W_2_S(wBuf, szAddr, 256);
	
	memset((char *)wBuf, 0, sizeof(wBuf));
	hE = XWnd_GetEle(g_hWindow, IDC_ADD_BLACKIDTYPE);
	n = XComboBox_GetSelectItem(hE);
	if (n>7||wcslen(XComboBox_GetItemText(hE,n))<1)
	{
       sprintf(szIDType, "%ld", 99);
	}
	else
	   sprintf(szIDType, "%ld", n + 1);

	memset((char *)wBuf, 0, sizeof(wBuf));
	hE = XWnd_GetEle(g_hWindow, IDE_ADD_BLACKID);
	if (XEdit_GetTextLength(hE) > 40)
	{
		XMessageBox(g_hWindow, L"����̫��", L"����");
		return false;
	}
	XEdit_GetText(hE, wBuf, 256);
	W_2_S(wBuf, szID, 256);
	
	memset((char *)wBuf, 0, sizeof(wBuf));
	hE = XWnd_GetEle(g_hWindow, IDE_ADD_BLACKUNIT);
	if (XEdit_GetTextLength(hE) > 30)
	{
		XMessageBox(g_hWindow, L"��λ��̫��", L"����");
		return false;
	}
	XEdit_GetText(hE, wBuf, 256);
	W_2_S(wBuf, szUnit, 256);

	memset((char *)wBuf, 0, sizeof(wBuf));
	hE = XWnd_GetEle(g_hWindow, IDE_ADD_BLACKCARNUM);
	if (XEdit_GetTextLength(hE) > 20)
	{
		XMessageBox(g_hWindow, L"���ƺ�̫��", L"����");
		return false;
	}
	XEdit_GetText(hE, wBuf, 256);
	W_2_S(wBuf, szCarNum, 256);

	strcpy(szPic, "");

	if (strlen(szName) <= 0)
	{
		XMessageBox(g_hWindow, L"���ӵĺ�������������Ϊ��!", L"����");
		return false;
	}
	
    if (strlen(szID)>0)
    {
		 sprintf(szT, "select black_name,black_id from t_blackname where black_name = \'%s\' or black_id = \'%s\'", szName, szID);
    }
	else
	{
		 sprintf(szT, "select black_name,black_id from t_blackname where black_name = \'%s\'", szName);
	}
   
	ret = tAdo.execSelect(szT, &FldCount, outParas);
	if (ret == ADOOK&&!outParas[0].IsEmpty())
	{
		 //XMessageBox(g_hWindow, L"���ӵĺ�����������֤��������ͬ�����Ƿ��滻ԭ���ļ�¼���ǵ��[ȷ��]�����[ȡ��]", L"��ʾ",XMB_OK|XMB_CANCEL);
         if (XMB_CANCEL==XMessageBox(g_hWindow, L"���ӵĺ�����������֤��������ͬ�����Ƿ��滻ԭ���ļ�¼���ǵ��[ȷ��]�����[ȡ��]", L"��ʾ",XMB_OK|XMB_CANCEL))
         {	     
		      return false;
         }
		 if (!outParas[1].IsEmpty()) //֤���Ų�Ϊ��
		 {
            sprintf(szT, "update t_blackname set black_alias = \'%s\',black_sex = \'%s\',black_folk = \'%s\',black_addr = \'%s\',black_mobile = \'%s\',black_id_type = \'%s\',black_id = \'%s\',black_unit = \'%s\',black_carnum = \'%s\',black_user_video_pic = \'%s\',black_user_id_pic = \'%s\',black_user_head_pic = \'%s\' where black_name = \'%s\' and black_id = \'%s\'",\
		         szAlias, szSex, "", szAddr, szMobile, szIDType, szID, szUnit, szCarNum, szPic, szPic, szPic,outParas[0], outParas[1]);
		 }
		 else
		 {
             sprintf(szT, "update t_blackname set black_alias = \'%s\',black_sex = \'%s\',black_folk = \'%s\',black_addr = \'%s\',black_mobile = \'%s\',black_id_type = \'%s\',black_id = \'%s\',black_unit = \'%s\',black_carnum = \'%s\',black_user_video_pic = \'%s\',black_user_id_pic = \'%s\',black_user_head_pic = \'%s\' where black_name = \'%s\'",\
		         szAlias, szSex, "", szAddr, szMobile, szIDType, szID, szUnit, szCarNum, szPic, szPic, szPic,outParas[0]);
		 }
	
	}
	else
	{
	    sprintf(szT, "insert into t_blackname values('\%s\', '\%s\', '\%s\', '\%s\', '\%s\', '\%s\', '\%s\', '\%s\', \'%s\', \'%s\', \'%s\', \'%s\', \'%s\', \'%s\')",\
		  szName, szAlias, szSex, "", szAddr, szMobile, szIDType, szID, szUnit, szCarNum, szPic, szPic, szPic,"");
	}




/*	visitor.szName, visitor.szName, visitor.szSex, visitor.szFolk, visitor.szAddr, visitor.szPhone,\
		visitor.szIDType, visitor.szID, visitor.szUnit, visitor.szCarNum, "", "", "");*/
	ret = tAdo.execUpdateInsert(szT);
	if (ret == ADOOK)
	{
		//HELE hE_txt = XWnd_GetEle(g_hWindow, IDE_ADD_BLACKNAME);
		//XEdit_SetText(hE_txt,L"");
		//OnQueryBlack(XWnd_GetEle(g_hWindow, IDB_QUERYBLACK), XWnd_GetEle(g_hWindow, IDB_QUERYBLACK));
		QueryBlack();
	}
	XEdit_SetText(XWnd_GetEle(g_hWindow, IDE_ADD_BLACKNAME),L"");
	XEdit_SetText(XWnd_GetEle(g_hWindow, IDE_ADD_BLACKALIAS),L"");
	XEdit_SetText(XWnd_GetEle(g_hWindow, IDC_ADD_BLACKSEX),L"");
	XEdit_SetText(XWnd_GetEle(g_hWindow, IDE_ADD_BLACKMOBILE),L"");
	XEdit_SetText(XWnd_GetEle(g_hWindow, IDE_ADD_BLACKADDR),L"");
	XEdit_SetText(XWnd_GetEle(g_hWindow, IDC_ADD_BLACKIDTYPE),L"");
	XEdit_SetText(XWnd_GetEle(g_hWindow, IDE_ADD_BLACKID),L"");
	XEdit_SetText(XWnd_GetEle(g_hWindow, IDE_ADD_BLACKUNIT),L"");
	XEdit_SetText(XWnd_GetEle(g_hWindow, IDE_ADD_BLACKCARNUM),L"");
	XWnd_RedrawWnd(XEle_GetHWindow(hEle));
	return false;
}

//ϵͳ����ҳ���е�ɾ����������Ϣ
BOOL CALLBACK OnDeleteBlack(HELE hEle, HELE hEventEle)
{
	char szBlackName[32] = {'\0'},szSex[8] = {'\0'},szFolk[32] = {'\0'},szAddr[50] = {'\0'},szMobile[32] = {'\0'},szID[32] = {'\0'};
	char szIDtype[32] = {'\0'},szUnit[50] = {'\0'},szCarNum[32] = {'\0'},szAlias[32] = {'\0'};
	wchar_t wBuf[256 + 1] = {0};
	
	char szT[512] = {'\0'};
	long ret = 0;
	CWinAdo tAdo(sCONNSTR_MYSQL);
	
	ret = tAdo.OpenAdo();
	if (ret != ADOOK)
	{
		return false;
	}
	
	char szData[32] = {'\0'};
	int n = 0;
	
	memset((char *)wBuf, 0, sizeof(wBuf));
	HELE hList = XWnd_GetEle(g_hWindow, IDL_LISTBLACK);

	n = XList_GetSelectItem(hList);		//��ȡѡ����
	if (n < 0)
	{
		return false;
	}

	wcscpy(wBuf, XList_GetItemText(hList, n, 1));		//����
	W_2_S(wBuf, szBlackName, 256);
	wcscpy(wBuf, XList_GetItemText(hList, n, 2));		//������
	W_2_S(wBuf, szAlias, 256);
	wcscpy(wBuf, XList_GetItemText(hList, n, 3));		//�Ա�
	W_2_S(wBuf, szSex, 256);
	wcscpy(wBuf, XList_GetItemText(hList, n, 4));		//����
	W_2_S(wBuf, szFolk, 256);
	wcscpy(wBuf, XList_GetItemText(hList, n, 5));		//��ַ
	W_2_S(wBuf, szAddr, 256);
	wcscpy(wBuf, XList_GetItemText(hList, n, 6));		//�绰
	W_2_S(wBuf, szMobile, 256);
	wcscpy(wBuf, XList_GetItemText(hList, n, 7));		//֤������
	W_2_S(wBuf, szIDtype, 256);
	wcscpy(wBuf, XList_GetItemText(hList, n, 8));		//֤����
	W_2_S(wBuf, szID, 256);
	wcscpy(wBuf, XList_GetItemText(hList, n, 9));		//��λ����
	W_2_S(wBuf, szUnit, 256);
	wcscpy(wBuf, XList_GetItemText(hList, n, 10));		//���ƺ�
	W_2_S(wBuf, szCarNum, 256);

    //sprintf(szT, "delete from t_blackname where black_name = \'%s\' and black_alias = \'%s\' and black_sex  = \'%s\' and black_folk  = \'%s\' and black_addr = \'%s\' and black_mobile = \'%s\' and black_id_type = \'%s\' and black_id = \'%s\' and black_unit = \'%s\' and black_carnum = \'%s\'", szBlackName,szAlias,szSex,szFolk,szAddr,szMobile,szIDtype,szID,szUnit,szCarNum);
    sprintf(szT, "delete from t_blackname where black_name = \'%s\' and black_alias = \'%s\' and black_sex  = \'%s\' and black_folk  = \'%s\' and black_addr = \'%s\' and black_mobile = \'%s\' and black_id = \'%s\' and black_unit = \'%s\' and black_carnum = \'%s\'", szBlackName,szAlias,szSex,szFolk,szAddr,szMobile,szID,szUnit,szCarNum);
	
	ret = tAdo.execUpdateInsert(szT);
	if (ret == ADOOK)
	{
		//HELE hE_txt = XWnd_GetEle(g_hWindow, IDE_ADD_BLACKNAME);
		//XEdit_SetText(hE_txt,L"");
		//OnQueryBlack(XWnd_GetEle(g_hWindow, IDB_QUERYBLACK), XWnd_GetEle(g_hWindow, IDB_QUERYBLACK));
		QueryBlack();
		XList_SetSelectItem(hList, 0);
	}
	XEdit_SetText(XWnd_GetEle(g_hWindow, IDE_ADD_BLACKNAME),L"");
	XEdit_SetText(XWnd_GetEle(g_hWindow, IDE_ADD_BLACKALIAS),L"");
	XEdit_SetText(XWnd_GetEle(g_hWindow, IDC_ADD_BLACKSEX),L"");
	XEdit_SetText(XWnd_GetEle(g_hWindow, IDE_ADD_BLACKMOBILE),L"");
	XEdit_SetText(XWnd_GetEle(g_hWindow, IDE_ADD_BLACKADDR),L"");
	XEdit_SetText(XWnd_GetEle(g_hWindow, IDC_ADD_BLACKIDTYPE),L"");
	XEdit_SetText(XWnd_GetEle(g_hWindow, IDE_ADD_BLACKID),L"");
	XEdit_SetText(XWnd_GetEle(g_hWindow, IDE_ADD_BLACKUNIT),L"");
	XEdit_SetText(XWnd_GetEle(g_hWindow, IDE_ADD_BLACKCARNUM),L"");		
	XWnd_RedrawWnd(XEle_GetHWindow(hEle));
	return false;
}

//ϵͳ����ҳ���еĲ�ѯ��Ʒ��Ϣ
BOOL CALLBACK OnQueryGoods(HELE hEle, HELE hEventEle)
{
	char szName[32] = {'\0'};
	wchar_t wBuf[256 + 1] = {0};
	
	char szT[512] = {'\0'};
	long FldCount = 0, ret = 0;;
	CString outParas[FLD_MAX];
	CWinAdo tAdo(sCONNSTR_MYSQL);
	
	ret = tAdo.OpenAdo();
	if (ret != ADOOK)
	{
		return false;
	}
	
	for (int i = 0; i < FLD_MAX; i++)
	{
		outParas[i].Empty();
	}
	
	HELE hE = XWnd_GetEle(g_hWindow, IDE_ADD_GOODSNAME);
	if (XEdit_GetTextLength(hE) <= 0)
	{
		strcpy(szT, "select * from t_goods");	//��ѯ������Ʒ����Ϣ
	}
	else
	{
		
		XEdit_GetText(hE, wBuf, 256);
		W_2_S(wBuf, szName, 256);
		sprintf(szT, "select * from t_goods where goods_name = \'%s\'", szName);
	}

	ret = tAdo.execSelect(szT, &FldCount, outParas);
	
	HELE hList = XWnd_GetEle(g_hWindow, IDL_LISTGOODSORREASONORSTATION);		//����б�Ԫ��
	
	XList_DeleteAllColumns(hList);
	XList_DeleteAllItems(hList);
	
	XList_SetHeaderHeight(hList, 30);
	XList_EnableGrid(hList, false);
	
	XList_AddColumn(hList, 40, L"���");
	XList_AddColumn(hList, 100, L"��Ʒ����");
	if (ret == ADOERR || (FldCount == 0 && outParas[0].IsEmpty()))
	{
//		XMessageBox(g_hWindow,L"����Ϣ",L"��ʾ");
		XWnd_RedrawWnd(XEle_GetHWindow(hList));
		return false;
	}	
	int n = 0;
	do
	{
		memset((char *)wBuf, 0, sizeof(wBuf));
		memset(szT, 0, sizeof(szT));
		sprintf(szT, "%d", n + 1);		//���
		S_2_W(wBuf, szT, 256);
		XList_AddItem(hList, wBuf, n);
		
		L_V_I(outParas[0], n, 1)	//��Ʒ����
		n++;
	} while (tAdo.getNext(outParas) == ADOOK) ;
	
	//XList_SetSelectItem(hList, 0);
	g_nTableType = TABLE_GOODS;

	XWnd_RedrawWnd(XEle_GetHWindow(hEle));
	return false;
}

//ϵͳ����ҳ���е�������Ʒ��Ϣ
BOOL CALLBACK OnAddGoods(HELE hEle, HELE hEventEle)
{
	char szName[32] = {'\0'};
	wchar_t wBuf[256 + 1] = {0};
	
	char szT[512] = {'\0'};
	long ret = 0;;
	CWinAdo tAdo(sCONNSTR_MYSQL);
	
	ret = tAdo.OpenAdo();
	if (ret != ADOOK)
	{
		return false;
	}
	
	int n = 0;
	
	memset((char *)wBuf, 0, sizeof(wBuf));
	HELE hE = XWnd_GetEle(g_hWindow, IDE_ADD_GOODSNAME);
	XEdit_GetText(hE, wBuf, 256);
	W_2_S(wBuf, szName, 256);
	
	if (strlen(szName) <= 0)
	{
		XMessageBox(g_hWindow, L"���ӵ���Ʒ���Ʋ���Ϊ��!", L"����");
		return false;
	}
    
	if (XEdit_GetTextLength(hE) > 60)
	{
		XMessageBox(g_hWindow, L"��Ʒ��̫��", L"����");
		return false;
	}
	sprintf(szT, "insert into t_goods values ('\%s\')", szName);
	ret = tAdo.execUpdateInsert(szT);
	if (ret == ADOOK)
	{
		HELE hE_txt = XWnd_GetEle(g_hWindow, IDE_ADD_GOODSNAME);
		XEdit_SetText(hE_txt,L"");
		OnQueryGoods(XWnd_GetEle(g_hWindow, IDB_ADDGOODS), XWnd_GetEle(g_hWindow, IDB_ADDGOODS));
	}
	XWnd_RedrawWnd(XEle_GetHWindow(hEle));
	return false;
}

//ϵͳ����ҳ���е�ɾ����Ʒ��Ϣ
BOOL CALLBACK OnDeleteGoods(HELE hEle, HELE hEventEle)
{
	if (g_nTableType != TABLE_GOODS)
	{
		return false;
	}

	char szName[32] = {'\0'};
	wchar_t wBuf[256 + 1] = {0};
	
	char szT[512] = {'\0'};
	long ret = 0;;
	CWinAdo tAdo(sCONNSTR_MYSQL);

	ret = tAdo.OpenAdo();
	if (ret != ADOOK)
	{
		return false;
	}
	
	char szData[32] = {'\0'};
	int n = 0;
	
	memset((char *)wBuf, 0, sizeof(wBuf));
	HELE hList = XWnd_GetEle(g_hWindow, IDL_LISTGOODSORREASONORSTATION);
	
	n = XList_GetSelectItem(hList);		//��ȡѡ����
	if (n < 0)
	{
		return false;
	}

	wcscpy(wBuf, XList_GetItemText(hList, n, 1));		//��Ʒ����
	W_2_S(wBuf, szData, 256);
	
	if (strlen(szData) <= 0)
	{
		XMessageBox(g_hWindow, L"ɾ������Ʒ���Ʋ���Ϊ��!", L"����");
		return false;
	}
	else
	{
		ret = XMessageBox(g_hWindow, L"���潫ɾ��ѡ�е���Ʒ��Ϣ,��[ȷ��]����,��[ȡ��]����", L"��ʾ��Ϣ", XMB_OK | XMB_CANCEL);
		if (ret == XMB_CANCEL)
		{
			return false;
		}
	}
	
	sprintf(szT, "delete from t_goods where goods_name = \'%s\'", szData);
	ret = tAdo.execUpdateInsert(szT);
	if (ret == ADOOK)
	{
		HELE hE_txt = XWnd_GetEle(g_hWindow, IDE_ADD_GOODSNAME);
		XEdit_SetText(hE_txt,L"");
		OnQueryGoods(XWnd_GetEle(g_hWindow, IDB_ADDGOODS), XWnd_GetEle(g_hWindow, IDB_ADDGOODS));
		XList_SetSelectItem(hList, 0);
	}
	g_nTableType=TABLE_GOODS;
	XWnd_RedrawWnd(XEle_GetHWindow(hEle));
	return false;
}

//ϵͳ����ҳ���еĲ�ѯ������Ϣ
BOOL CALLBACK OnQueryReason(HELE hEle, HELE hEventEle)
{
	char szName[32] = {'\0'};
	wchar_t wBuf[256 + 1] = {0};
	
	char szT[512] = {'\0'};
	long FldCount = 0, ret = 0;;
	CString outParas[FLD_MAX];
	CWinAdo tAdo(sCONNSTR_MYSQL);
	
	ret = tAdo.OpenAdo();
	if (ret != ADOOK)
	{
		return false;
	}
	
	for (int i = 0; i < FLD_MAX; i++)
	{
		outParas[i].Empty();
	}
	
	HELE hE = XWnd_GetEle(g_hWindow, IDE_ADD_REASONNAME);
	if (XEdit_GetTextLength(hE) <= 0)
	{
		strcpy(szT, "select * from t_reason");	//��ѯ�������ɵ���Ϣ
	}
	else
	{
		
		XEdit_GetText(hE, wBuf, 256);
		W_2_S(wBuf, szName, 256);
		sprintf(szT, "select * from t_reason where reason_name = \'%s\'", szName);
	}
	
	HELE hList = XWnd_GetEle(g_hWindow, IDL_LISTGOODSORREASONORSTATION);		//����б�Ԫ��
	
	XList_DeleteAllColumns(hList);
	XList_DeleteAllItems(hList);
	
	XList_SetHeaderHeight(hList, 30);
	XList_EnableGrid(hList, false);
	
	XList_AddColumn(hList, 40, L"���");
	XList_AddColumn(hList, 100, L"��������");

	ret = tAdo.execSelect(szT, &FldCount, outParas);
	if (ret == ADOERR || (FldCount == 0 && outParas[0].IsEmpty()))
	{
//		XMessageBox(g_hWindow,L"����Ϣ",L"��ʾ");
		XWnd_RedrawWnd(XEle_GetHWindow(hList));
		return false;
	}
	
	int n = 0;
	do
	{
		memset((char *)wBuf, 0, sizeof(wBuf));
		memset(szT, 0, sizeof(szT));
		sprintf(szT, "%d", n + 1);		//���
		S_2_W(wBuf, szT, 256);
		XList_AddItem(hList, wBuf, n);
		
		L_V_I(outParas[0], n, 1)	//��������
		n++;
	} while (tAdo.getNext(outParas) == ADOOK) ;
	
	//XList_SetSelectItem(hList, 0);
	g_nTableType = TABLE_REASON;

	XWnd_RedrawWnd(XEle_GetHWindow(hEle));
	
	return false;
}

//ϵͳ����ҳ���е�����������Ϣ
BOOL CALLBACK OnAddReason(HELE hEle, HELE hEventEle)
{
	char szName[32] = {'\0'};
	wchar_t wBuf[256 + 1] = {0};
	
	char szT[512] = {'\0'};
	long ret = 0;;
	CWinAdo tAdo(sCONNSTR_MYSQL);
	
	ret = tAdo.OpenAdo();
	if (ret != ADOOK)
	{
		return false;
	}

	int n = 0;

	memset((char *)wBuf, 0, sizeof(wBuf));
	HELE hE = XWnd_GetEle(g_hWindow, IDE_ADD_REASONNAME);
	XEdit_GetText(hE, wBuf, 256);
	W_2_S(wBuf, szName, 256);
	
	if (strlen(szName) <= 0)
	{
		XMessageBox(g_hWindow, L"���ӵ��������ɲ���Ϊ��!", L"����");
		return false;
	}
	if (XEdit_GetTextLength(hE) > 40)
	{
		XMessageBox(g_hWindow, L"��������̫��", L"����");
		return false;
	}	
	sprintf(szT, "insert into t_reason values ('\%s\')", szName);
	ret = tAdo.execUpdateInsert(szT);
	if (ret == ADOOK)
	{
		HELE hE_txt = XWnd_GetEle(g_hWindow, IDE_ADD_REASONNAME);
		XEdit_SetText(hE_txt,L"");
		OnQueryReason(XWnd_GetEle(g_hWindow, IDB_QUERYREASON), XWnd_GetEle(g_hWindow, IDB_QUERYREASON));
	}
	
	XWnd_RedrawWnd(XEle_GetHWindow(hEle));
	return false;
}

//ϵͳ����ҳ���е�ɾ��������Ϣ
BOOL CALLBACK OnDeleteReason(HELE hEle, HELE hEventEle)
{
	if (g_nTableType != TABLE_REASON)
	{
		return false;
	}

	char szName[32] = {'\0'};
	wchar_t wBuf[256 + 1] = {0};
	
	char szT[512] = {'\0'};
	long ret = 0;;
	CWinAdo tAdo(sCONNSTR_MYSQL);
	
	ret = tAdo.OpenAdo();
	if (ret != ADOOK)
	{
		return false;
	}
	
	char szData[32] = {'\0'};
	int n = 0;
	
	memset((char *)wBuf, 0, sizeof(wBuf));
	HELE hList = XWnd_GetEle(g_hWindow, IDL_LISTGOODSORREASONORSTATION);
	
	n = XList_GetSelectItem(hList);		//��ȡѡ����
	if (n < 0)
	{
		return false;
	}
	wcscpy(wBuf, XList_GetItemText(hList, n, 1));		//��������
	W_2_S(wBuf, szData, 256);
	
	if (strlen(szData) <= 0)
	{
		XMessageBox(g_hWindow, L"ɾ�����������ɲ���Ϊ��!", L"����");
		return false;
	}
	else
	{
		ret = XMessageBox(g_hWindow, L"���潫ɾ��ѡ�е�����������Ϣ,��[ȷ��]����,��[ȡ��]����", L"��ʾ��Ϣ", XMB_OK | XMB_CANCEL);
		if (ret == XMB_CANCEL)
		{
			return false;
		}
	}
	
	sprintf(szT, "delete from t_reason where reason_name = \'%s\'", szData);
	ret = tAdo.execUpdateInsert(szT);
	if (ret == ADOOK)
	{
		HELE hE_txt = XWnd_GetEle(g_hWindow, IDE_ADD_REASONNAME);
		XEdit_SetText(hE_txt,L"");
		OnQueryReason(XWnd_GetEle(g_hWindow, IDB_QUERYREASON), XWnd_GetEle(g_hWindow, IDB_QUERYREASON));
		XList_SetSelectItem(hList, 0);
	}
	g_nTableType = TABLE_REASON;
	XWnd_RedrawWnd(XEle_GetHWindow(hEle));
	return false;
}

//ϵͳ����ҳ���еĲ�ѯ�Ÿ�
BOOL CALLBACK OnQueryStation(HELE hEle, HELE hEventEle)
{
	char szName[32] = {'\0'};
	wchar_t wBuf[256 + 1] = {0};
	
	char szT[512] = {'\0'};
	long FldCount = 0, ret = 0;;
	CString outParas[FLD_MAX];
	CWinAdo tAdo(sCONNSTR_MYSQL);
	
	ret = tAdo.OpenAdo();
	if (ret != ADOOK)
	{
		return false;
	}
	
	for (int i = 0; i < FLD_MAX; i++)
	{
		outParas[i].Empty();
	}
	
	HELE hE = XWnd_GetEle(g_hWindow, IDE_ADD_STATIONNAME);
	if (XEdit_GetTextLength(hE) <= 0)
	{
		strcpy(szT, "select * from t_station");	//��ѯ���е���Ϣ
	}
	else
	{
		
		XEdit_GetText(hE, wBuf, 256);
		W_2_S(wBuf, szName, 256);
		sprintf(szT, "select * from t_station where station_name = \'%s\'", szName);
	}
	
	ret = tAdo.execSelect(szT, &FldCount, outParas);
	
	HELE hList = XWnd_GetEle(g_hWindow, IDL_LISTGOODSORREASONORSTATION);		//����б�Ԫ��
	
	XList_DeleteAllColumns(hList);
	XList_DeleteAllItems(hList);
	
	XList_SetHeaderHeight(hList, 30);
	XList_EnableGrid(hList, false);
	
	XList_AddColumn(hList, 40, L"���");
	XList_AddColumn(hList, 100, L"�Ÿ�");
	if (ret == ADOERR || (FldCount == 0 && outParas[0].IsEmpty()))
	{
//		XMessageBox(g_hWindow,L"����Ϣ",L"��ʾ");
		XWnd_RedrawWnd(XEle_GetHWindow(hList));
		return false;
	}	
	int n = 0;
	do
	{
		memset((char *)wBuf, 0, sizeof(wBuf));
		memset(szT, 0, sizeof(szT));
		sprintf(szT, "%d", n + 1);		//���
		S_2_W(wBuf, szT, 256);
		XList_AddItem(hList, wBuf, n);
		
		L_V_I(outParas[0], n, 1)	//�Ÿ�����
		n++;
	} while (tAdo.getNext(outParas) == ADOOK) ;
	
	//XList_SetSelectItem(hList, 0);
	g_nTableType = TABLE_STATION;

	XWnd_RedrawWnd(XEle_GetHWindow(hEle));
	
	return false;
}

//ϵͳ����ҳ���е������Ÿ�
BOOL CALLBACK OnAddStation(HELE hEle, HELE hEventEle)
{
	char szName[32] = {'\0'};
	wchar_t wBuf[256 + 1] = {0};
	
	char szT[512] = {'\0'};
	long ret = 0;;
	CWinAdo tAdo(sCONNSTR_MYSQL);
	
	ret = tAdo.OpenAdo();
	if (ret != ADOOK)
	{
		return false;
	}
	
	int n = 0;

	memset((char *)wBuf, 0, sizeof(wBuf));
	HELE hE = XWnd_GetEle(g_hWindow, IDE_ADD_STATIONNAME);
	XEdit_GetText(hE, wBuf, 256);
	W_2_S(wBuf, szName, 256);
	
	if (strlen(szName) <= 0)
	{
		XMessageBox(g_hWindow, L"���ӵ��Ÿڲ���Ϊ��!", L"����");
		return false;
	}

   	if (XEdit_GetTextLength(XWnd_GetEle(g_hWindow, IDE_ADD_STATIONNAME)) > 4)
	{
		XMessageBox(g_hWindow, L"�Ÿ���̫��", L"����");
		return false;
	}
	sprintf(szT, "insert into t_station values ('\%s\')", szName);
	ret = tAdo.execUpdateInsert(szT);
	if (ret == ADOOK)
	{
		HELE hE_txt = XWnd_GetEle(g_hWindow, IDE_ADD_STATIONNAME);
		XEdit_SetText(hE_txt,L"");
		OnQueryStation(XWnd_GetEle(g_hWindow, IDB_QUERYSTATION), XWnd_GetEle(g_hWindow, IDB_QUERYSTATION));
	}
	XWnd_RedrawWnd(XEle_GetHWindow(hEle));
	return false;
}

//ϵͳ����ҳ���е�ɾ���Ÿ�
BOOL CALLBACK OnDeleteStation(HELE hEle, HELE hEventEle)
{
	if (g_nTableType != TABLE_STATION)
	{
		return false;
	}

	char szName[32] = {'\0'};
	wchar_t wBuf[256 + 1] = {0};
	
	char szT[512] = {'\0'};
	long ret = 0;;
	CWinAdo tAdo(sCONNSTR_MYSQL);
	
	ret = tAdo.OpenAdo();
	if (ret != ADOOK)
	{
		return false;
	}
	
	char szData[32] = {'\0'};
	int n = 0;
	
	memset((char *)wBuf, 0, sizeof(wBuf));
	HELE hList = XWnd_GetEle(g_hWindow, IDL_LISTGOODSORREASONORSTATION);
	
	n = XList_GetSelectItem(hList);		//��ȡѡ����
	if (n < 0)
	{
		return false;
	}
	wcscpy(wBuf, XList_GetItemText(hList, n, 1));		//�Ÿ�����
	W_2_S(wBuf, szData, 256);
	
	if (strlen(szData) <= 0)
	{
		XMessageBox(g_hWindow, L"ɾ�����Ÿ����Ʋ���Ϊ��!", L"����");
		return false;
	}
	else
	{
		ret = XMessageBox(g_hWindow, L"���潫ɾ��ѡ�е��Ÿ���Ϣ,��[ȷ��]����,��[ȡ��]����", L"��ʾ��Ϣ", XMB_OK | XMB_CANCEL);
		if (ret == XMB_CANCEL)
		{
			return false;
		}
	}
	
	sprintf(szT, "delete from t_station where station_name = \'%s\'", szData);
	ret = tAdo.execUpdateInsert(szT);
	if (ret == ADOOK)
	{
		HELE hE_txt = XWnd_GetEle(g_hWindow, IDE_ADD_STATIONNAME);
		XEdit_SetText(hE_txt,L"");
		OnQueryStation(XWnd_GetEle(g_hWindow, IDB_QUERYSTATION), XWnd_GetEle(g_hWindow, IDB_QUERYSTATION));
		XList_SetSelectItem(hList, 0);
	}
    g_nTableType = TABLE_STATION;
	XWnd_RedrawWnd(XEle_GetHWindow(hEle));
	return false;
}
//ϵͳ����ҳ���еĲ�ѯ����
BOOL CALLBACK OnQueryOffice(HELE hEle, HELE hEventEle)
{
	char szName[32] = {'\0'};
	wchar_t wBuf[256 + 1] = {0};
	
	char szT[512] = {'\0'};
	long FldCount = 0, ret = 0;;
	CString outParas[FLD_MAX];
	CWinAdo tAdo(sCONNSTR_MYSQL);

	ret = tAdo.OpenAdo();
	if (ret != ADOOK)
	{
		return false;
	}
	
	for (int i = 0; i < FLD_MAX; i++)
	{
		outParas[i].Empty();
	}
	
	HELE hE = XWnd_GetEle(g_hWindow, IDE_ADD_OFFICE_NAME);
	if (XEdit_GetTextLength(hE) <= 0)
	{
		strcpy(szT, "select * from t_office");	//��ѯ���е���Ϣ
	}
	else
	{
		
		XEdit_GetText(hE, wBuf, 256);
		W_2_S(wBuf, szName, 256);
		sprintf(szT, "select * from t_office where office_name = \'%s\'", szName);
	}
	
	ret = tAdo.execSelect(szT, &FldCount, outParas);
	
	HELE hList = XWnd_GetEle(g_hWindow, IDL_LISTOFFICEORLEVEL);		//����б�Ԫ��
	
	XList_DeleteAllColumns(hList);
	XList_DeleteAllItems(hList);
	
	XList_SetHeaderHeight(hList, 30);
	XList_EnableGrid(hList, false);
	
	XList_AddColumn(hList, 40, L"���");
	XList_AddColumn(hList, 100, L"��������");
	XList_AddColumn(hList, 100, L"�绰");
	XList_AddColumn(hList, 100, L"�칫��ַ");
	XList_AddColumn(hList, 100, L"�����");
	if (ret == ADOERR || (FldCount == 0 && outParas[0].IsEmpty()))
	{
//		XMessageBox(g_hWindow,L"����Ϣ",L"��ʾ");
		XWnd_RedrawWnd(XEle_GetHWindow(hList));
		return false;
	}	
	int n = 0;
	do
	{
		memset((char *)wBuf, 0, sizeof(wBuf));
		memset(szT, 0, sizeof(szT));
		sprintf(szT, "%d", n + 1);		//���
		S_2_W(wBuf, szT, 256);
		XList_AddItem(hList, wBuf, n);
		
		L_V_I(outParas[0], n, 1)	//����
		L_V_I(outParas[1], n, 2)	//�绰
		L_V_I(outParas[2], n, 3)	//��ַ
		L_V_I(outParas[3], n, 4)	//�����
		n++;
	} while (tAdo.getNext(outParas) == ADOOK) ;

	//XList_SetSelectItem(hList, 0);
	g_nTableType = TABLE_OFFICE;

	XWnd_RedrawWnd(XEle_GetHWindow(hEle));

	return false;
}
//ϵͳ����ҳ���е����ӿ���
BOOL CALLBACK OnAddOffice(HELE hEle, HELE hEventEle)
{
	char szName[32] = {'\0'}, szPhone[32] = {'\0'}, szAddr[128] = {'\0'}, szRoom[16] = {'\0'};
	wchar_t wBuf[256 + 1] = {0};
	
	char szT[512] = {'\0'};
	long ret = 0;;
	CWinAdo tAdo(sCONNSTR_MYSQL);
	
	ret = tAdo.OpenAdo();
	if (ret != ADOOK)
	{
		return false;
	}
	
	int n = 0;
	
	memset((char *)wBuf, 0, sizeof(wBuf));
	HELE hE = XWnd_GetEle(g_hWindow, IDE_ADD_OFFICE_NAME);
	XEdit_GetText(hE, wBuf, 256);
	W_2_S(wBuf, szName, 256);
	if (strlen(szName) <= 0)
	{
		XMessageBox(g_hWindow, L"���ӵĿ������Ʋ���Ϊ��!", L"����");
		return false;
	}

	memset((char *)wBuf, 0, sizeof(wBuf));
	hE = XWnd_GetEle(g_hWindow, IDE_ADD_OFFICE_PHONE);
	XEdit_GetText(hE, wBuf, 256);
	W_2_S(wBuf, szPhone, 256);

	memset((char *)wBuf, 0, sizeof(wBuf));
	hE = XWnd_GetEle(g_hWindow, IDE_ADD_OFFICE_ADDR);
	XEdit_GetText(hE, wBuf, 256);
	W_2_S(wBuf, szAddr, 256);

	memset((char *)wBuf, 0, sizeof(wBuf));
	hE = XWnd_GetEle(g_hWindow, IDE_ADD_OFFICE_ROOMNUM);
	XEdit_GetText(hE, wBuf, 256);
	W_2_S(wBuf, szRoom, 256);

	sprintf(szT, "insert into t_office values (\'%s\', \'%s\', \'%s\', \'%s\')", szName, szPhone, szAddr, szRoom);
	ret = tAdo.execUpdateInsert(szT);
	if (ret == ADOOK)
	{
		HELE hE_txt = XWnd_GetEle(g_hWindow, IDE_ADD_OFFICE_NAME);
		XEdit_SetText(hE_txt,L"");
		OnQueryOffice(XWnd_GetEle(g_hWindow, IDB_QUERYOFFICIAL), XWnd_GetEle(g_hWindow, IDB_QUERYOFFICIAL));
	}


	XWnd_RedrawWnd(XEle_GetHWindow(hEle));
	return false;
}


//ϵͳ����ҳ���е�ɾ������
BOOL CALLBACK OnDeleteOffice(HELE hEle, HELE hEventEle)
{
	if (g_nTableType != TABLE_OFFICE)
	{
		return false;
	}
	char szName[32] = {'\0'};
	wchar_t wBuf[256 + 1] = {0};
	
	char szT[512] = {'\0'};
	long ret = 0;;
	CWinAdo tAdo(sCONNSTR_MYSQL);
	
	ret = tAdo.OpenAdo();
	if (ret != ADOOK)
	{
		return false;
	}
	
	char szData[32] = {'\0'};
	int n = 0;
	
	memset((char *)wBuf, 0, sizeof(wBuf));
	HELE hList = XWnd_GetEle(g_hWindow, IDL_LISTOFFICEORLEVEL);
	
	n = XList_GetSelectItem(hList);		//��ȡѡ����
	if (n < 0)
	{
		return false;
	}
	wcscpy(wBuf, XList_GetItemText(hList, n, 1));		//����
	W_2_S(wBuf, szData, 256);
	
	if (strlen(szData) <= 0)
	{
		XMessageBox(g_hWindow, L"ɾ���Ŀ������Ʋ���Ϊ��!", L"����");
		return false;
	}
	else
	{
		ret = XMessageBox(g_hWindow, L"���潫ɾ��ѡ�еĿ�����Ϣ,��[ȷ��]����,��[ȡ��]����", L"��ʾ��Ϣ", XMB_OK | XMB_CANCEL);
		if (ret == XMB_CANCEL)
		{
			return false;
		}
	}
	
	sprintf(szT, "delete from t_office where office_name = \'%s\'", szData);
	ret = tAdo.execUpdateInsert(szT);
	if (ret == ADOOK)
	{
		HELE hE_txt = XWnd_GetEle(g_hWindow, IDE_ADD_OFFICE_NAME);
		XEdit_SetText(hE_txt,L"");
		OnQueryOffice(XWnd_GetEle(g_hWindow, IDB_QUERYOFFICIAL), XWnd_GetEle(g_hWindow, IDB_QUERYOFFICIAL));
		XList_SetSelectItem(hList, 0);
	}
	XWnd_RedrawWnd(XEle_GetHWindow(hEle));
	return false;
}
//ϵͳ����ҳ���еĲ�ѯ����
BOOL CALLBACK OnQueryLevel(HELE hEle, HELE hEventEle)
{
	char szName[32] = {'\0'};
	wchar_t wBuf[256 + 1] = {0};
	
	char szT[512] = {'\0'};
	long FldCount = 0, ret = 0;;
	CString outParas[FLD_MAX];
	CWinAdo tAdo(sCONNSTR_MYSQL);

	ret = tAdo.OpenAdo();
	if (ret != ADOOK)
	{
		return false;
	}
	
	for (int i = 0; i < FLD_MAX; i++)
	{
		outParas[i].Empty();
	}
	
	HELE hE = XWnd_GetEle(g_hWindow, IDE_ADD_LEVELNAME);
	if (XEdit_GetTextLength(hE) <= 0)
	{
		strcpy(szT, "select * from t_duty");	//��ѯ���е���Ϣ
	}
	else
	{
		
		XEdit_GetText(hE, wBuf, 256);
		W_2_S(wBuf, szName, 256);
		sprintf(szT, "select * from t_duty where duty_name = \'%s\'", szName);
	}
	
	ret = tAdo.execSelect(szT, &FldCount, outParas);
	
	HELE hList = XWnd_GetEle(g_hWindow, IDL_LISTOFFICEORLEVEL);		//����б�Ԫ��
	
	XList_DeleteAllColumns(hList);
	XList_DeleteAllItems(hList);
	
	XList_SetHeaderHeight(hList, 30);
	XList_EnableGrid(hList, false);
	
	XList_AddColumn(hList, 40, L"���");
	XList_AddColumn(hList, 100, L"��������");
	if (ret == ADOERR || (FldCount == 0 && outParas[0].IsEmpty()))
	{
//		XMessageBox(g_hWindow,L"����Ϣ",L"��ʾ");
		XWnd_RedrawWnd(XEle_GetHWindow(hList));
		return false;
	}	
	int n = 0;
	do
	{
		memset((char *)wBuf, 0, sizeof(wBuf));
		memset(szT, 0, sizeof(szT));
		sprintf(szT, "%d", n + 1);		//���
		S_2_W(wBuf, szT, 256);
		XList_AddItem(hList, wBuf, n);
		
		L_V_I(outParas[0], n, 1)	//����
		n++;
	} while (tAdo.getNext(outParas) == ADOOK) ;

	//XList_SetSelectItem(hList, 0);

	g_nTableType = TABLE_DUTY;
	
	XWnd_RedrawWnd(XEle_GetHWindow(hEle));
	
	return false;
}
//ϵͳ����ҳ���е����Ӽ���
BOOL CALLBACK OnAddLevel(HELE hEle, HELE hEventEle)
{
	char szName[32] = {'\0'};
	wchar_t wBuf[256 + 1] = {0};
	
	char szT[512] = {'\0'};
	long ret = 0;;
	CWinAdo tAdo(sCONNSTR_MYSQL);
	
	ret = tAdo.OpenAdo();
	if (ret != ADOOK)
	{
		return false;
	}
	
	int n = 0;
	
	memset((char *)wBuf, 0, sizeof(wBuf));
	HELE hE = XWnd_GetEle(g_hWindow, IDE_ADD_LEVELNAME);
	XEdit_GetText(hE, wBuf, 256);
	W_2_S(wBuf, szName, 256);
	if (strlen(szName) <= 0)
	{
		XMessageBox(g_hWindow, L"���ӵļ������Ʋ���Ϊ��!", L"����");
		return false;
	}
	
	sprintf(szT, "insert into t_duty values (\'%s\')", szName);
	ret = tAdo.execUpdateInsert(szT);
	if (ret == ADOOK)
	{
		HELE hE_txt = XWnd_GetEle(g_hWindow, IDE_ADD_LEVELNAME);
		XEdit_SetText(hE_txt,L"");
		OnQueryLevel(XWnd_GetEle(g_hWindow, IDB_QUERYLEVEL), XWnd_GetEle(g_hWindow, IDB_QUERYLEVEL));
	}
	XWnd_RedrawWnd(XEle_GetHWindow(hEle));
	return false;
}



//ϵͳ����ҳ���е�ɾ������
BOOL CALLBACK OnDeleteLevel(HELE hEle, HELE hEventEle)
{
	if (g_nTableType != TABLE_DUTY)
	{
		return false;
	}

	char szName[32] = {'\0'};
	wchar_t wBuf[256 + 1] = {0};
	
	char szT[512] = {'\0'};
	long ret = 0;;
	CWinAdo tAdo(sCONNSTR_MYSQL);
	
	ret = tAdo.OpenAdo();
	if (ret != ADOOK)
	{
		return false;
	}

	char szData[32] = {'\0'};
	int n = 0;
	
	memset((char *)wBuf, 0, sizeof(wBuf));
	HELE hList = XWnd_GetEle(g_hWindow, IDL_LISTOFFICEORLEVEL);
	
	n = XList_GetSelectItem(hList);		//��ȡѡ����
	if (n < 0)
	{
		return false;
	}
	wcscpy(wBuf, XList_GetItemText(hList, n, 1));		//����
	W_2_S(wBuf, szData, 256);
	
	if (strlen(szData) <= 0)
	{
		XMessageBox(g_hWindow, L"ɾ���ļ������Ʋ���Ϊ��!", L"����");
		return false;
	}
	else
	{
		ret = XMessageBox(g_hWindow, L"���潫ɾ��ѡ�еļ�����Ϣ,��[ȷ��]����,��[ȡ��]����", L"��ʾ��Ϣ", XMB_OK | XMB_CANCEL);
		if (ret == XMB_CANCEL)
		{
			return false;
		}
	}
	
	sprintf(szT, "delete from t_duty where duty_name = \'%s\'", szData);
	ret = tAdo.execUpdateInsert(szT);
	if (ret == ADOOK)
	{
		XEdit_SetText(XWnd_GetEle(g_hWindow, IDE_ADD_CZYID),L"");
		XEdit_SetText(XWnd_GetEle(g_hWindow, IDE_ADD_CZYMM),L"");
		XComboBox_SetSelectItem(XWnd_GetEle(g_hWindow, IDE_ADD_CZYRIGHT),0);
		OnQueryLevel(XWnd_GetEle(g_hWindow, IDB_QUERYLEVEL), XWnd_GetEle(g_hWindow, IDB_QUERYLEVEL));
		XList_SetSelectItem(hList, 0);
	}
	XWnd_RedrawWnd(XEle_GetHWindow(hEle));
	return false;
}

//ϵͳ����ҳ���е����Ӳ���Ա
BOOL CALLBACK OnAddCzy(HELE hEle, HELE hEventEle)
{
	char szID[32] = {'\0'}, szMM[32] = {'\0'}, szRight[32] = {'\0'};
	wchar_t wBuf[256 + 1] = {0};
	
	char szT[512] = {'\0'};
	long ret = 0;;
	CWinAdo tAdo(sCONNSTR_MYSQL);
	
	ret = tAdo.OpenAdo();
	if (ret != ADOOK)
	{
		return false;
	}
	
	int n = 0;
	
	memset((char *)wBuf, 0, sizeof(wBuf));
	HELE hE = XWnd_GetEle(g_hWindow, IDE_ADD_CZYID);
	if (XEdit_GetTextLength(XWnd_GetEle(g_hWindow, IDE_ADD_CZYID)) > 10)
	{
		XMessageBox(g_hWindow, L"����Ա��̫��", L"����");
		return false;
	}
	XEdit_GetText(hE, wBuf, 256);
	W_2_S(wBuf, szID, 256);
	if (strlen(szID) <= 0)
	{
		XMessageBox(g_hWindow, L"���ӵĲ���Ա�����Ʋ���Ϊ��!", L"����");
		XWnd_RedrawWnd(XEle_GetHWindow(hEle));
		return false;
	}

	memset((char *)wBuf, 0, sizeof(wBuf));
	hE = XWnd_GetEle(g_hWindow, IDE_ADD_CZYMM);
	if (XEdit_GetTextLength(XWnd_GetEle(g_hWindow, IDE_ADD_CZYMM)) > 20)
	{
		XMessageBox(g_hWindow, L"����̫��", L"����");
		return false;
	}
	XEdit_GetText(hE, wBuf, 256);
	W_2_S(wBuf, szMM, 256);
	if (strlen(szMM) <= 0||strcmp(szMM,"*******")==0)
	{
		XMessageBox(g_hWindow, L"���ӵĲ������벻��Ϊ��!", L"����");
		XWnd_RedrawWnd(XEle_GetHWindow(hEle));
		return false;
	}

	memset((char *)wBuf, 0, sizeof(wBuf));
	hE = XWnd_GetEle(g_hWindow, IDE_ADD_CZYRIGHT);
	//n = XComboBox_GetSelectItem(hE);
    XEdit_GetText(hE, wBuf, 256);
	W_2_S(wBuf, szRight, 256);
	if (strcmp(szRight,"����Ա")==0)
	{
        sprintf(szRight, "%d", 0);
	}
	else if (strcmp(szRight,"��������Ա")==0)
	{
        sprintf(szRight, "%d", 2);
	}
	else 
	{
        sprintf(szRight, "%d", 1);
	}
	
	sprintf(szT, "insert into t_user values (\'%s\', \'%s\', \'%s\')", szID, szMM, szRight);
	ret = tAdo.execUpdateInsert(szT);
	if (ret == ADOOK)
	{
		XEdit_SetText(XWnd_GetEle(g_hWindow, IDE_ADD_CZYID),L"");
		XEdit_SetText(XWnd_GetEle(g_hWindow, IDE_ADD_CZYMM),L"");
		XComboBox_SetSelectItem(XWnd_GetEle(g_hWindow, IDE_ADD_CZYRIGHT),0);
		QueryCzy();
	}

	XWnd_RedrawWnd(XEle_GetHWindow(hEle));
	return false;
}

//ϵͳ����ҳ���еĲ�ѯ����Ա
BOOL CALLBACK OnQueryCzy(HELE hEle, HELE hEventEle)
{
	QueryCzy();
	g_nTableType = TABLE_CZY;
	XWnd_RedrawWnd(XEle_GetHWindow(hEle));
	return false;
}

BOOL QueryCzy()
{
	char szName[32] = {'\0'};
	wchar_t wBuf[256 + 1] = {0};
	
	char szT[512] = {'\0'},szRight[4]={'\0'};
	long FldCount = 0, ret = 0;
	CString outParas[FLD_MAX],sRight;
	CWinAdo tAdo(sCONNSTR_MYSQL);
	
	ret = tAdo.OpenAdo();
	if (ret != ADOOK)
	{
		return false;
	}
	
	for (int i = 0; i < FLD_MAX; i++)
	{
		outParas[i].Empty();
	}
	
	HELE hE = XWnd_GetEle(g_hWindow, IDE_ADD_CZYID);
	if (XEdit_GetTextLength(hE) <= 0)
	{
		::GetPrivateProfileString("TerminalSet", "UserRight","", szRight, sizeof(szRight),CONFIG_FILE);
		if (atoi(szRight) == 0)
		{
			strcpy(szT, "select * from t_user where user_right_id=1");	//����Ա����һ�����Ա
		}
		else if (atoi(szRight) == 2)
		{
			strcpy(szT, "select * from t_user where user_right_id=1 or user_right_id=0");	//��������Ա����������Ϣ
		}
		
	}
	else
	{
		
		XEdit_GetText(hE, wBuf, 256);
		W_2_S(wBuf, szName, 256);
		sprintf(szT, "select * from t_user where user_id = \'%s\'", szName);
	}

	HELE hList = XWnd_GetEle(g_hWindow, IDL_LISTGOODSORREASONORSTATION);		//����б�Ԫ��
	
	XList_DeleteAllColumns(hList);
	XList_DeleteAllItems(hList);
	
	XList_SetHeaderHeight(hList, 30);
	XList_EnableGrid(hList, false);
	
	XList_AddColumn(hList, 60, L"���");
	XList_AddColumn(hList, 100, L"����Ա��");
	XList_AddColumn(hList, 100, L"����ԱȨ��");

	ret = tAdo.execSelect(szT, &FldCount, outParas);
	if (ret == ADOERR || (FldCount == 0 && outParas[0].IsEmpty()))
	{
//		XMessageBox(g_hWindow,L"����Ϣ",L"��ʾ");
		XWnd_RedrawWnd(XEle_GetHWindow(hList));
		return false;
	}	
	int n = 0;
	do
	{
		memset((char *)wBuf, 0, sizeof(wBuf));
		memset(szT, 0, sizeof(szT));
		sprintf(szT, "%d", n + 1);		//���
		S_2_W(wBuf, szT, 256);
		XList_AddItem(hList, wBuf, n);
		
		L_V_I(outParas[0], n, 1)	//����ԱID
		if (atoi(outParas[2]) == 0)
		{
			L_V_I("����Ա", n ,2);
		}
		else if(atoi(outParas[2]) == 1)
		{
			L_V_I("һ�����Ա", n ,2);
		}
		else if(atoi(outParas[2]) == 2)
		{
			L_V_I("��������Ա", n ,2);
		}
		else
		{
            L_V_I(" ", n ,2);
		}
		n++;
	} while (tAdo.getNext(outParas) == ADOOK) ;
	
	//XList_SetSelectItem(hList, 0);
    g_nTableType = TABLE_CZY;
	return false;
}

//ϵͳ����ҳ���е�ɾ������Ա
BOOL CALLBACK OnDeleteCzy(HELE hEle, HELE hEventEle)
{
	if (g_nTableType != TABLE_CZY)
	{
		return false;
	}
	
	wchar_t wBuf[256 + 1] = {0};
	
	char szT[512] = {'\0'};
	long ret = 0;;
	CWinAdo tAdo(sCONNSTR_MYSQL);
	
	ret = tAdo.OpenAdo();
	if (ret != ADOOK)
	{
		return false;
	}
	
	char szData[32] = {'\0'};
	int n = 0;
	
	memset((char *)wBuf, 0, sizeof(wBuf));
	HELE hList = XWnd_GetEle(g_hWindow, IDL_LISTGOODSORREASONORSTATION);
	
	n = XList_GetSelectItem(hList);		//��ȡѡ����
	if (n < 0)
	{
		return false;
	}
	wcscpy(wBuf, XList_GetItemText(hList, n, 1));		//id
	W_2_S(wBuf, szData, 32);
	
	if (strlen(szData) <= 0)
	{
		XMessageBox(g_hWindow, L"ɾ���Ĳ���Ա�Ų���Ϊ��!", L"����");
		XWnd_RedrawWnd(XEle_GetHWindow(hEle));
		return false;
	}
	else if (strcmp(szData,"admin")==0)
	{
        XMessageBox(g_hWindow, L"����ɾ������Ա�˺�!", L"����");
		XWnd_RedrawWnd(XEle_GetHWindow(hEle));
		return false;
	}
	else
	{
		ret = XMessageBox(g_hWindow, L"���潫ɾ��ѡ�еĲ���Ա��Ϣ,��[ȷ��]����,��[ȡ��]����", L"��ʾ��Ϣ", XMB_OK | XMB_CANCEL);
		if (ret == XMB_CANCEL)
		{
			XWnd_RedrawWnd(XEle_GetHWindow(hEle));
			return false;
		}
	}
	
	sprintf(szT, "delete from t_user where user_id = \'%s\'", szData);
	ret = tAdo.execUpdateInsert(szT);
	if (ret == ADOOK)
	{
		HELE hE_txt = XWnd_GetEle(g_hWindow, IDE_ADD_CZYID);
		XEdit_SetText(hE_txt,L"");
		QueryCzy();
		XList_SetSelectItem(hList, 0);
	}
	

	XWnd_RedrawWnd(XEle_GetHWindow(hEle));
	return false;
}

//����������ϵġ�ϵͳ���á�
BOOL CALLBACK OnSysSet(HELE hEle, HELE hEventEle)
{
	if (hEle != hEventEle) 
	{
		return false;
	}

    HideSoftKeybd();
	HWINDOW hWindow=XModalWnd_CreateWindow(210, 160, L"���������", XWnd_GetHWnd(g_hWindow));
	if(hWindow)
	{
        
		//HELE hButton=XStatic_Create(10,10,65,65,L"��",hWindow);//������ť
        htxtChaxunPwd=XEdit_Create(20,10,165,25,hWindow);//�ı�����
		XEdit_EnablePassBox(htxtChaxunPwd,true);
		XEle_SetFocus(htxtChaxunPwd,true);
		HELE hBtnOK=XBtn_Create(20,45,65,65,L"ȷ��",hWindow);
		HELE hBtnCancel=XBtn_Create(120,45,65,65,L"����",hWindow);//������ť

        
		//XEle_RegisterEvent(hButton,XE_BNCLICK,OnLogOutBtnClick);//ע�ᰴť����¼�
		XEle_RegisterEvent(htxtChaxunPwd,XE_SETFOCUS,OnOtherSetFocus);
        XEle_RegisterEvent(hBtnOK,XE_BNCLICK,OnOKSysBtnClick);
		XEle_RegisterEvent(hBtnCancel,XE_BNCLICK,OnCloseLogOutBtnClick);

		SetFocus(XWnd_GetHWnd(hWindow));
		//XWnd_ShowWindow(hWindow,SW_SHOW);//��ʾ����
		//XRunXCGUI();
		SetMainDlgEditBorder();
		XWnd_SetBorderSize(hWindow, 3, 3, 3, 3);
		HIMAGE hImg = XImage_LoadFile(L".\\skin\\image\\frame.jpg", true);
		//XWnd_SetImageNC(hWindow, hImg);
		XModalWnd_DoModal(hWindow);
	}

	XWnd_RedrawWnd(XEle_GetHWindow(hEle));
	return false;
}
//////////////ϵͳ����ҳ���ϵ���ش�����/////////////////////////
BOOL CALLBACK OnExportOfficial(HELE hEle, HELE hEventEle)
{
    if (hEle != hEventEle) 
	{
		return false;
	}
	CString szpathDir="�����˱�.xls";
	if(!SaveXlsDlg(szpathDir))  //û��ѡ���ļ�·��
	{
        XMessageBox(g_hWindow,L"�ļ��д���ʧ�ܣ�",L"����");
		return false;
	}

	//szpathDir=szpathDir+"\\�����˱�.xls";
     //����xls��szpathDir
	CString tmp=szpathDir.Right(4);
	if(tmp.CompareNoCase(".xls")!=0) //û�к�׺�������ļ���׺
        szpathDir+=".xls";
	bool b = false;
	try{
		b=ExportOfficial((LPSTR)(LPCTSTR)szpathDir);
	}catch(...){}
	
    if(!b)
	{
		XMessageBox(g_hWindow,L"����ʧ�ܿ��ܴ���ͬ���ļ���",L"����");
		return false;
	}
	XMessageBox(g_hWindow,L"��������Ϣ�����ɹ�",L"��ʾ");
	return true;
}

BOOL CALLBACK OnImportOfficial(HELE hEle, HELE hEventEle)
{
    if (hEle != hEventEle) 
	{
		return false;
	}
	char szFileName[MAX_PATH] = "�����˱�.xls";
	//char *szFileName=new char[MAX_PATH];
	if(!OpenXlsDlg(szFileName))
		return false;
	if (!ImportOfficial(szFileName))
	{
		XMessageBox(g_hWindow,L"����ʧ�ܣ�",L"����");
		return false;
	}
	//delete []szFileName;
	XMessageBox(g_hWindow,L"��������Ϣ����ɹ�",L"��ʾ");
	return true;
}

BOOL CALLBACK OnICUnReg(HELE hEle, HELE hEventEle)
{
    if (hEle != hEventEle) 
	{
		return false;
	}
	wchar_t wBuf[256] = {0};//,wTxt[256]={0};
	char  szID[32];//,szID[32],szSex[8],szPhone[32],szMoblie[32],szRoomNum[8],szOffice[32],szAddr[32],szZhiWu[32],szIP[32];
	char szT[512] = {'\0'};
	HELE hList=GETELE(IDL_LISTOFFICIAL);
	CWinAdo tAdo(sCONNSTR_MYSQL);
	long ret;
    int n=XList_GetSelectItem(hList);

	if (n<0)
	{
		XMessageBox(g_hWindow,L"��ѡ���б���",L"��ʾ");
		return false;
	}
	ret = tAdo.OpenAdo();
	if (ret != ADOOK)
	{
		return false;
	}

	memset((char *)wBuf, 0, sizeof(wBuf));
	//memset((char *)wTxt, 0, sizeof(wTxt));
	wcscpy(wBuf, XList_GetItemText(hList, n, 0));
    W_2_S(wBuf,szID,32);

    //memset((char *)wBuf, 0, sizeof(wBuf));
	//memset((char *)wTxt, 0, sizeof(wTxt));
	//wcscpy(wBuf, XList_GetItemText(hList, n, 1));
    //XEdit_GetText(XWnd_GetEle(g_hWindow, IDE_ADD_OFFICIALNAME), wTxt,256);
	//if (XEdit_GetTextLength(XWnd_GetEle(g_hWindow, IDE_ADD_OFFICIALNAME)) > 10)
	{
	//	XMessageBox(g_hWindow, L"����̫��", L"����");
	//	return false;
	}
	//W_2_S(wTxt,szName,32);
	//if (wcscmp(wBuf,wTxt))
	{	
		//if (wcslen(wTxt)>0)
		{
			sprintf(szT, "update t_official set official_NO = \'\' where official_id = \'%s\'", szID);
		    ret = tAdo.execUpdateInsert(szT);
		}
		//else
		{
		//	XMessageBox(g_hWindow,L"������Ա����Ϊ��",L"����");			
		//	return false;
		}
		
	}
	OnQueryEmployerForList(XWnd_GetEle(g_hWindow, IDB_QUERYOFFICIAL), XWnd_GetEle(g_hWindow, IDB_QUERYOFFICIAL));
	XList_SetSelectItem(hList, 0);

	XEdit_SetText(XWnd_GetEle(g_hWindow, IDE_ADD_OFFICIALNAME),L"");
	XEdit_SetText(XWnd_GetEle(g_hWindow, IDC_ADD_OFFICIALSEX),L"");
	XEdit_SetText(XWnd_GetEle(g_hWindow, IDE_ADD_OFFICEPHONE),L"");
	XEdit_SetText(XWnd_GetEle(g_hWindow, IDE_ADD_OFFICENAME),L"");
	XEdit_SetText(XWnd_GetEle(g_hWindow, IDE_ADD_OFFICIALMOBILE),L"");
	XEdit_SetText(XWnd_GetEle(g_hWindow, IDE_ADD_OFFICEADDR),L"");
	XEdit_SetText(XWnd_GetEle(g_hWindow, IDC_ADD_OFFICIALZHIWU),L"");
	XEdit_SetText(XWnd_GetEle(g_hWindow, IDE_ADD_ROOMNUM),L"");
	XEdit_SetText(XWnd_GetEle(g_hWindow, IDE_ADD_IP),L"");
	XEdit_SetText(XWnd_GetEle(g_hWindow, IDE_ADD_SSO),L"");

	XEle_ShowEle(hEle,false);
	XWnd_RedrawWnd(XEle_GetHWindow(hEle));
	return false;
}

BOOL CALLBACK OnSaveOfficial(HELE hEle, HELE hEventEle)
{
    if (hEle != hEventEle) 
	{
		return false;
	}
	wchar_t wBuf[256] = {0},wTxt[256]={0};
	char  szName[32],szID[32],szSex[8],szPhone[32],szMoblie[32],szRoomNum[8],szOffice[32],szAddr[32],szZhiWu[32],szIP[32];
	char szSSO[32],szT[512] = {'\0'};
	HELE hList=GETELE(IDL_LISTOFFICIAL);
	CWinAdo tAdo(sCONNSTR_MYSQL);
	long ret;
    int n=XList_GetSelectItem(hList);

	if (n<0)
	{
		XMessageBox(g_hWindow,L"��ѡ���б���",L"��ʾ");
		return false;
	}
	ret = tAdo.OpenAdo();
	if (ret != ADOOK)
	{
		return false;
	}

	memset((char *)wBuf, 0, sizeof(wBuf));
	memset((char *)wTxt, 0, sizeof(wTxt));
	wcscpy(wBuf, XList_GetItemText(hList, n, 0));
    W_2_S(wBuf,szID,32);

    memset((char *)wBuf, 0, sizeof(wBuf));
	memset((char *)wTxt, 0, sizeof(wTxt));
	wcscpy(wBuf, XList_GetItemText(hList, n, 1));
    XEdit_GetText(XWnd_GetEle(g_hWindow, IDE_ADD_OFFICIALNAME), wTxt,256);
	if (XEdit_GetTextLength(XWnd_GetEle(g_hWindow, IDE_ADD_OFFICIALNAME)) > 10)
	{
		XMessageBox(g_hWindow, L"����̫��", L"����");
		return false;
	}
	W_2_S(wTxt,szName,32);
	if (wcscmp(wBuf,wTxt))
	{	
		if (wcslen(wTxt)>0)
		{
			sprintf(szT, "update t_official set official_name = \'%s\' where official_id = \'%s\'",szName, szID);
		    ret = tAdo.execUpdateInsert(szT);
		}
		else
		{
			XMessageBox(g_hWindow,L"������Ա����Ϊ��",L"����");			
			return false;
		}
		
	}

    memset((char *)wBuf, 0, sizeof(wBuf));
	memset((char *)wTxt, 0, sizeof(wTxt));
	wcscpy(wBuf, XList_GetItemText(hList, n, 2));
	XEdit_GetText(XWnd_GetEle(g_hWindow, IDC_ADD_OFFICIALSEX), wTxt,256);
	if (wcscmp(wBuf,wTxt))
	{
		//if (wcslen(wTxt)>0)
		{
			W_2_S(wTxt,szSex,8);
		    sprintf(szT, "update t_official set official_sex = \'%s\' where official_id = \'%s\'",szSex, szID);
		    ret = tAdo.execUpdateInsert(szT);
		}

	}

    memset((char *)wBuf, 0, sizeof(wBuf));
	memset((char *)wTxt, 0, sizeof(wTxt));
	wcscpy(wBuf, XList_GetItemText(hList, n, 3));
	XEdit_GetText(XWnd_GetEle(g_hWindow, IDE_ADD_OFFICEPHONE), wTxt,256);
	if (XEdit_GetTextLength(XWnd_GetEle(g_hWindow, IDE_ADD_OFFICEPHONE)) > 10)
	{
		XMessageBox(g_hWindow, L"�칫�绰��̫��", L"����");
		return false;
	}
	if (wcscmp(wBuf,wTxt))
	{
		W_2_S(wTxt,szPhone,32);
		sprintf(szT, "update t_official set official_office_phone = \'%s\' where official_id = \'%s\'",szPhone, szID);
		ret = tAdo.execUpdateInsert(szT);
	}

    memset((char *)wBuf, 0, sizeof(wBuf));
	memset((char *)wTxt, 0, sizeof(wTxt));
	wcscpy(wBuf, XList_GetItemText(hList, n, 4));
	XEdit_GetText(XWnd_GetEle(g_hWindow, IDE_ADD_OFFICIALMOBILE), wTxt,256);
	if (XEdit_GetTextLength(XWnd_GetEle(g_hWindow, IDE_ADD_OFFICIALMOBILE)) > 20)
	{
		XMessageBox(g_hWindow, L"�ֻ���̫��", L"����");
		return false;
	}
	if (wcscmp(wBuf,wTxt))
	{
		W_2_S(wTxt,szMoblie,32);
		sprintf(szT, "update t_official set official_mobile = \'%s\' where official_id = \'%s\'",szMoblie, szID);
		ret = tAdo.execUpdateInsert(szT);
	}

    memset((char *)wBuf, 0, sizeof(wBuf));
	memset((char *)wTxt, 0, sizeof(wTxt));
	wcscpy(wBuf, XList_GetItemText(hList, n, 8));
	XEdit_GetText(XWnd_GetEle(g_hWindow, IDE_ADD_ROOMNUM), wTxt,256);
	if (XEdit_GetTextLength(XWnd_GetEle(g_hWindow, IDE_ADD_ROOMNUM)) > 10)
	{
		XMessageBox(g_hWindow, L"�����̫��", L"����");
		return false;
	}
	if (wcscmp(wBuf,wTxt))
	{
		W_2_S(wTxt,szRoomNum,8);
		sprintf(szT, "update t_official set official_room = \'%s\' where official_id = \'%s\'",szRoomNum, szID);
		ret = tAdo.execUpdateInsert(szT);
	}

    memset((char *)wBuf, 0, sizeof(wBuf));
	memset((char *)wTxt, 0, sizeof(wTxt));
	wcscpy(wBuf, XList_GetItemText(hList, n, 5));
	XEdit_GetText(XWnd_GetEle(g_hWindow, IDE_ADD_OFFICENAME), wTxt,256);
	if (XEdit_GetTextLength(XWnd_GetEle(g_hWindow, IDE_ADD_OFFICENAME)) > 20)
	{
		XMessageBox(g_hWindow, L"�칫������̫��", L"����");
		return false;
	}
	if (wcscmp(wBuf,wTxt))
	{
		W_2_S(wTxt,szOffice,32);
		sprintf(szT, "update t_official set official_office_name = \'%s\' where official_id = \'%s\'",szOffice, szID);
		ret = tAdo.execUpdateInsert(szT);
	}

    memset((char *)wBuf, 0, sizeof(wBuf));
	memset((char *)wTxt, 0, sizeof(wTxt));
	wcscpy(wBuf, XList_GetItemText(hList, n, 6));
	XEdit_GetText(XWnd_GetEle(g_hWindow, IDE_ADD_OFFICEADDR), wTxt,256);
	if (XEdit_GetTextLength(XWnd_GetEle(g_hWindow, IDE_ADD_OFFICEADDR)) > 15)
	{
		XMessageBox(g_hWindow, L"�칫�ҵ�ַ̫��", L"����");
		return false;
	}
	if (wcscmp(wBuf,wTxt))
	{
		W_2_S(wTxt,szAddr,32);
		sprintf(szT, "update t_official set official_office_addr = \'%s\' where official_id = \'%s\'",szAddr, szID);
		ret = tAdo.execUpdateInsert(szT);
	}

    memset((char *)wBuf, 0, sizeof(wBuf));
	memset((char *)wTxt, 0, sizeof(wTxt));
	wcscpy(wBuf, XList_GetItemText(hList, n, 7));
	XEdit_GetText(XWnd_GetEle(g_hWindow, IDC_ADD_OFFICIALZHIWU), wTxt,256);
	if (XEdit_GetTextLength(XWnd_GetEle(g_hWindow, IDC_ADD_OFFICIALZHIWU)) > 10)
	{
		XMessageBox(g_hWindow, L"��������̫��", L"����");
		return false;
	}
	if (wcscmp(wBuf,wTxt))
	{
		W_2_S(wTxt,szZhiWu,32);
		sprintf(szT, "update t_official set official_duty = \'%s\' where official_id = \'%s\'",szZhiWu, szID);
		ret = tAdo.execUpdateInsert(szT);
	}

    memset((char *)wBuf, 0, sizeof(wBuf));
	memset((char *)wTxt, 0, sizeof(wTxt));
	wcscpy(wBuf, XList_GetItemText(hList, n, 11)); //9
	XEdit_GetText(XWnd_GetEle(g_hWindow, IDE_ADD_IP), wTxt,256);
	if (XEdit_GetTextLength(XWnd_GetEle(g_hWindow, IDE_ADD_IP)) > 20)
	{
		XMessageBox(g_hWindow, L"IP��ַ̫��", L"����");
		return false;
	}
	if (wcscmp(wBuf,wTxt))
	{
		W_2_S(wTxt,szIP,32);
		sprintf(szT, "update t_official set official_IP = \'%s\' where official_id = \'%s\'",szIP, szID);
		ret = tAdo.execUpdateInsert(szT);
	}
	
    memset((char *)wBuf, 0, sizeof(wBuf));
	memset((char *)wTxt, 0, sizeof(wTxt));
	wcscpy(wBuf, XList_GetItemText(hList, n, 9));
	XEdit_GetText(XWnd_GetEle(g_hWindow, IDE_ADD_SSO), wTxt,256);
	if (XEdit_GetTextLength(XWnd_GetEle(g_hWindow, IDE_ADD_SSO)) > 32)
	{
		XMessageBox(g_hWindow, L"SSO̫��", L"����");
		return false;
	}
	if (wcscmp(wBuf,wTxt))
	{
		W_2_S(wTxt,szIP,32);
		sprintf(szT, "update t_official set official_SSO = \'%s\' where official_id = \'%s\'",szSSO, szID);
		ret = tAdo.execUpdateInsert(szT);
	}

	OnQueryEmployerForList(XWnd_GetEle(g_hWindow, IDB_QUERYOFFICIAL), XWnd_GetEle(g_hWindow, IDB_QUERYOFFICIAL));
	XList_SetSelectItem(hList, 0);

	XEdit_SetText(XWnd_GetEle(g_hWindow, IDE_ADD_OFFICIALNAME),L"");
	XEdit_SetText(XWnd_GetEle(g_hWindow, IDC_ADD_OFFICIALSEX),L"");
	XEdit_SetText(XWnd_GetEle(g_hWindow, IDE_ADD_OFFICEPHONE),L"");
	XEdit_SetText(XWnd_GetEle(g_hWindow, IDE_ADD_OFFICENAME),L"");
	XEdit_SetText(XWnd_GetEle(g_hWindow, IDE_ADD_OFFICIALMOBILE),L"");
	XEdit_SetText(XWnd_GetEle(g_hWindow, IDE_ADD_OFFICEADDR),L"");
	XEdit_SetText(XWnd_GetEle(g_hWindow, IDC_ADD_OFFICIALZHIWU),L"");
	XEdit_SetText(XWnd_GetEle(g_hWindow, IDE_ADD_ROOMNUM),L"");
	XEdit_SetText(XWnd_GetEle(g_hWindow, IDE_ADD_IP),L"");
	XEdit_SetText(XWnd_GetEle(g_hWindow, IDE_ADD_SSO),L"");

	XWnd_RedrawWnd(XEle_GetHWindow(hEle));
	return true;
}

BOOL SaveXlsDlg(CString &m_pathDir)
{
     //_tsetlocale(LC_CTYPE, TEXT(""));//��wprintf ֧������   
    TCHAR szPathName[MAX_PATH];  
    OPENFILENAME ofn = {sizeof (OPENFILENAME)};//or  {sizeof (OPENFILENAME)}   
// lStructSize   
// ָ������ṹ�Ĵ�С�����ֽ�Ϊ��λ��   
// Windows 95/98��Windows NT 4.0������ΪWindows 95/98��Windows NT 4.0��������WINVER��_WIN32_WINNT >= 0x0500����ʱ��   
//  Ϊ�����Աʹ��OPENFILENAME_SIZE_VERSION_400��   
// Windows 2000�����߰汾���������ʹ��sizeof (OPENFILENAME) ��   
     ofn.hwndOwner =GetForegroundWindow();// ��OR�����ļ��Ի���ĸ�����   
     ofn.lpstrFilter = TEXT("EXCEL Files(*.xls)\0*.XLS\0All Files\0*.*\0\0");   
            //������ ���Ϊ NULL ��ʹ�ù�����   
            //�����÷�������  ע�� /0   
     lstrcpy(szPathName, m_pathDir);  
     ofn.lpstrFile = szPathName;  
     ofn.nMaxFile = sizeof(szPathName);//����û�ѡ���ļ��� ·�����ļ��� ������   
     ofn.lpstrTitle = TEXT("ѡ���ļ�");//ѡ���ļ��Ի������   
     TCHAR szCurDir[MAX_PATH];  
     GetCurrentDirectory(sizeof(szCurDir),szCurDir);  
     ofn.lpstrInitialDir=szCurDir;//���öԻ�����ʾ�ĳ�ʼĿ¼   
     ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST;//�����Ҫѡ�����ļ� ��������  OFN_ALLOWMULTISELECT��־   
     _tprintf(TEXT("select file/n"));  
     /*BOOL bOk = GetOpenFileName(&ofn);//���öԻ�����ļ�   
     if (bOk)  
     {  
         //showFile(szPathName);  
     } */ 
     ofn.lpstrTitle = TEXT("ѡ�񱣴��ļ�λ��");//ѡ���ļ��Ի������   
     BOOL bOk=GetSaveFileName(&ofn);//���öԻ��򱣴��ļ�   
     if (!bOk)  
     {  
		 return false;
        // showFile(szPathName);  
     }  
	 m_pathDir=szPathName;
/*	BROWSEINFO bi;
    ITEMIDLIST* pidl;
    bi.hwndOwner = GETHWND;
    bi.iImage = 0;
    bi.lParam = 0;
    bi.lpfn = 0;
    bi.lpszTitle = _T("Please choose the directory:");
    bi.pidlRoot = 0;
    bi.pszDisplayName = 0;
    bi.ulFlags = BIF_USENEWUI | BIF_EDITBOX;
	//bi.ulFlags = 0;
    pidl = SHBrowseForFolder(&bi);
        //m_pathDir���洢·����CString�ַ�����ȫ�ֱ���
    BOOL bret=SHGetPathFromIDList(pidl,m_pathDir.GetBuffer(MAX_PATH));	
    m_pathDir.ReleaseBuffer();*/
	return true;
}

BOOL OpenXlsDlg(char *szFileName)
{
	OPENFILENAME file = { 0 };
	file.lStructSize = sizeof(file);
	
	file.hwndOwner = XWnd_GetHWnd(g_hWindow);
	file.lpstrFile = szFileName;
	file.nMaxFile = MAX_PATH;
	file.nMaxFileTitle = MAX_PATH;
	file.lpstrFilter = "EXCEL Files(*.xls)\0*.XLS\0All Files\0*.*\0\0";
	//file.lpstrFilter = "EXCEL Files(*.xls)";
	file.nFilterIndex = 1;//Ĭ��ѡ���һ��
	file.Flags=OFN_FILEMUSTEXIST | OFN_EXPLORER;
	// �������ļ��ĶԻ���
	if(GetOpenFileName(&file))
	{
		return TRUE;
	}
	return FALSE;
}

BOOL CALLBACK OnOfficialSet(HELE hEle, HELE hEventEle)
{
	if (hEle != hEventEle) 
	{
		return false;
	}
	
	XEle_ShowEle(g_hPage4, false);			//���غ�����ҳ��
	XEle_ShowEle(g_hPage5, false);			//������Ʒҳ��
	XEle_ShowEle(g_hPage8, false);
	XEle_ShowEle(g_hPage9, false);

	EnableButton(IDB_TAB32);
	XEle_ShowEle(g_hPage3, true);
	g_bICReg = true;
	XEle_ShowEle(XWnd_GetEle(g_hWindow, IDP_PAGE3_1), true);
    //OnQueryOffice(XWnd_GetEle(g_hWindow, IDB_QUERYOFFICIAL), XWnd_GetEle(g_hWindow, IDB_QUERYOFFICIAL)); //Ĭ�ϲ�ѯ���п�����Ա
	SetOfficialEditBorder();
	//SetBlackNameEditBorder();
	//SetOtherDlgEditBorder();	
	XWnd_RedrawWnd(XEle_GetHWindow(hEle));
	return false;
}

BOOL CALLBACK OnBlackSet(HELE hEle, HELE hEventEle)
{
	if (hEle != hEventEle) 
	{
		return false;
	}

	XEle_ShowEle(XWnd_GetEle(g_hWindow, IDP_PAGE3_1), false);
	XEle_ShowEle(g_hPage5, false);			//������Ʒҳ��
	XEle_ShowEle(g_hPage8, false);
	XEle_ShowEle(g_hPage9, false);

	XEle_ShowEle(g_hPage4, true);
	EnableButton(IDB_TAB33);

	SetSexInfo(IDC_ADD_BLACKSEX);
	XComboBox_AddString(XWnd_GetEle(g_hWindow, IDC_ADD_BLACKSEX), L" ");
	XComboBox_EnableEdit(XWnd_GetEle(g_hWindow, IDC_ADD_BLACKSEX),false);
	SetIDType(IDC_ADD_BLACKIDTYPE);
    //QueryBlack();
	//SetOfficialEditBorder();
	SetBlackNameEditBorder();
	//SetOtherDlgEditBorder();
	OnQueryBlack(XWnd_GetEle(g_hWindow, IDB_QUERYBLACK), XWnd_GetEle(g_hWindow, IDB_QUERYBLACK));
	XWnd_RedrawWnd(XEle_GetHWindow(hEle));
	return false;
}

BOOL CALLBACK OnImportBlackName(HELE hEle, HELE hEventEle)
{
	char szFileName[MAX_PATH] = "������.xls";
	if(!OpenXlsDlg(szFileName))
		return false;
	if (!ImportBlackName(szFileName))
	{
		XMessageBox(g_hWindow,L"����ʧ�ܣ�",L"����");
		return false;
	}
	XMessageBox(g_hWindow,L"��������Ϣ����ɹ�",L"��ʾ");
	return true;
}

BOOL CALLBACK OnExportBlackName(HELE hEle, HELE hEventEle)
{
	CString szpathDir="��������.xls";
	if(!SaveXlsDlg(szpathDir))  //û��ѡ���ļ�·��
		return false;

	//szpathDir=szpathDir+"\\������.xls";
     //����xls��szpathDir
	CString tmp=szpathDir.Right(4);
	if(tmp.CompareNoCase(".xls")!=0) //û�к�׺�������ļ���׺
        szpathDir+=".xls";
    if(!ExportBlackName((LPSTR)(LPCTSTR)szpathDir))
	{
		XMessageBox(g_hWindow,L"����ʧ�ܿ��ܴ���ͬ���ļ���",L"����");
		return false;
	}
	XMessageBox(g_hWindow,L"��������Ϣ�����ɹ�",L"��ʾ");
	return true;
}

BOOL CALLBACK OnGoodsSet(HELE hEle, HELE hEventEle)
{
	if (hEle != hEventEle) 
	{
		return false;
	}
	
	XEle_ShowEle(XWnd_GetEle(g_hWindow, IDP_PAGE3_1), false);
	XEle_ShowEle(g_hPage4, false);
	XEle_ShowEle(g_hPage8, false);
	XEle_ShowEle(g_hPage9, false);

	XEle_ShowEle(g_hPage5, true);
    OnQueryGoods(XWnd_GetEle(g_hWindow, IDB_ADDGOODS), XWnd_GetEle(g_hWindow, IDB_ADDGOODS));
	EnableButton(IDB_TAB34);

	SetUserRight();		//�����û�Ȩ����Ϣ
	//SetOfficialEditBorder();
	//SetBlackNameEditBorder();
	SetOtherDlgEditBorder();

	XWnd_RedrawWnd(XEle_GetHWindow(hEle));
	return false;
}

BOOL CALLBACK OnExportVisitor(HELE hEle, HELE hEventEle)
{
	CString szpathDir="�����˱�.xls";
	char szT[128] = {'\0'};
	if(!SaveXlsDlg(szpathDir))  //û��ѡ���ļ�·��
		return false;

    time_t t;
	time(&t);
	struct tm *tim = NULL;
	time(&t);
	tim = localtime(&t);

    //sprintf(szT, "\\���ü�¼.%04ld-%02ld-%02ld.xls", tim->tm_year + 1900, tim->tm_mon + 1, tim->tm_mday,\
	//	tim->tm_hour, tim->tm_min, tim->tm_sec);	
	//szpathDir="c:\\���ü�¼.xls";
	//szpathDir=szpathDir+szT;
     //����xls��szpathDir
	CString tmp=szpathDir.Right(4);
	if(tmp.CompareNoCase(".xls")!=0) //û�к�׺�������ļ���׺
        szpathDir+=".xls";

    if(!ExportVisitor((LPSTR)(LPCTSTR)szpathDir))
	{
		XMessageBox(g_hWindow,L"����ʧ�ܿ��ܴ���ͬ���ļ���",L"����");
		return false;
	}
	XMessageBox(g_hWindow,L"�����ɹ�",L"��ʾ");
	return true;
}

BOOL CALLBACK OnImportGoods(HELE hEle, HELE hEventEle)
{
	char szFileName[MAX_PATH] = "Я����Ʒ.xls";
	if(!OpenXlsDlg(szFileName))
		return false;
	if (!ImportWithGoods(szFileName))
	{
		XMessageBox(g_hWindow,L"����ʧ�ܣ�",L"����");
		return false;
	}
	XMessageBox(g_hWindow,L"Я����Ʒ��Ϣ����ɹ�",L"��ʾ");
	return true;
}
BOOL CALLBACK OnExportGoods(HELE hEle, HELE hEventEle)
{
	CString szpathDir="Я����Ʒ��.xls";
	if(!SaveXlsDlg(szpathDir))  //û��ѡ���ļ�·��
		return false;

	//szpathDir=szpathDir+"\\Я����Ʒ.xls";
     //����xls��szpathDir
	CString tmp=szpathDir.Right(4);
	if(tmp.CompareNoCase(".xls")!=0) //û�к�׺�������ļ���׺
        szpathDir+=".xls";
    if(!ExportWithGoods((LPSTR)(LPCTSTR)szpathDir))
	{
		XMessageBox(g_hWindow,L"����ʧ�ܿ��ܴ���ͬ���ļ���",L"����");
		return false;
	}
	XMessageBox(g_hWindow,L"Я����Ʒ��Ϣ�����ɹ�",L"��ʾ");
	return true;
}
BOOL CALLBACK OnSaveGoods(HELE hEle, HELE hEventEle)
{
    if (hEle != hEventEle) 
	{
		return false;
	}
	wchar_t wBuf[256] = {0},wTxt[256]={0},*wHead=NULL;
	char  szGoods[64];
	char szT[512] = {'\0'};
	HELE hList=GETELE(IDL_LISTGOODSORREASONORSTATION);
	CWinAdo tAdo(sCONNSTR_MYSQL);
	long ret;
    int n=XList_GetSelectItem(hList);

	if (n<0)
	{
		XMessageBox(g_hWindow,L"��ѡ���б���",L"��ʾ");
		return false;
	}
	ret = tAdo.OpenAdo();
	if (ret != ADOOK)
	{
		return false;
	}

	wHead = XList_GetHeaderItemText(hList,1);
	if(wcscmp(wHead,L"��Ʒ����")==0)
	{
		memset((char *)wBuf, 0, sizeof(wBuf));
		memset((char *)wTxt, 0, sizeof(wTxt));
		wcscpy(wBuf, XList_GetItemText(hList, n, 1));
		XEdit_GetText(XWnd_GetEle(g_hWindow, IDE_ADD_GOODSNAME), wTxt,256);

    	if (XEdit_GetTextLength(XWnd_GetEle(g_hWindow, IDE_ADD_GOODSNAME)) > 60)
		{
			 XMessageBox(g_hWindow, L"��Ʒ��̫��", L"����");
		     return false;
		}
		char szOldGoods[64];
		W_2_S(wBuf,szOldGoods,64);
		W_2_S(wTxt,szGoods,64);
		if (wcscmp(wBuf,wTxt)&&wcslen(wTxt)>0)
		{
			sprintf(szT, "update t_goods set goods_name = \'%s\' where goods_name = \'%s\'",szGoods, szOldGoods);
			ret = tAdo.execUpdateInsert(szT);
		}
		else
			return false;
	}
	else
	{
		XMessageBox(g_hWindow,L"��ѡ����Ʒ�б�",L"����");
		return false;
	}
	OnQueryGoods(XWnd_GetEle(g_hWindow, IDB_ADDGOODS), XWnd_GetEle(g_hWindow, IDB_ADDGOODS));
	XList_SetSelectItem(hList, 0);
	XEdit_SetText(XWnd_GetEle(g_hWindow, IDE_ADD_GOODSNAME),L"");
	XWnd_RedrawWnd(XEle_GetHWindow(hEle));
    return true;
}
BOOL CALLBACK OnImportReason(HELE hEle, HELE hEventEle)
{
	char szFileName[MAX_PATH] = "��������.xls";
	if(!OpenXlsDlg(szFileName))
		return false;
	if (!ImportReason(szFileName))
	{
		XMessageBox(g_hWindow,L"����ʧ�ܣ�",L"����");
		return false;
	}
	XMessageBox(g_hWindow,L"����������Ϣ����ɹ�",L"��ʾ");
	return true;
}
BOOL CALLBACK OnExportReason(HELE hEle, HELE hEventEle)
{
	CString szpathDir="�������ɱ�.xls";
	if(!SaveXlsDlg(szpathDir))  //û��ѡ���ļ�·��
		return false;

	//szpathDir=szpathDir+"\\��������.xls";
     //����xls��szpathDir
	CString tmp=szpathDir.Right(4);
	if(tmp.CompareNoCase(".xls")!=0) //û�к�׺�������ļ���׺
        szpathDir+=".xls";
    if(!ExportReason((LPSTR)(LPCTSTR)szpathDir))
	{
		XMessageBox(g_hWindow,L"����ʧ�ܿ��ܴ���ͬ���ļ���",L"����");
		return false;
	}
	XMessageBox(g_hWindow,L"����������Ϣ�����ɹ�",L"��ʾ");
	return true;
}
BOOL CALLBACK OnSaveReason(HELE hEle, HELE hEventEle)
{
	if (hEle != hEventEle) 
	{
		return false;
	}
	wchar_t wBuf[256] = {0},wTxt[256]={0},*wHead=NULL;
	char  szReason[64];
	char szT[512] = {'\0'};
	HELE hList=GETELE(IDL_LISTGOODSORREASONORSTATION);
	CWinAdo tAdo(sCONNSTR_MYSQL);
	long ret;
    int n=XList_GetSelectItem(hList);

	if (n<0)
	{
		XMessageBox(g_hWindow,L"��ѡ���б���",L"��ʾ");
		return false;
	}
	ret = tAdo.OpenAdo();
	if (ret != ADOOK)
	{
		return false;
	}

	wHead = XList_GetHeaderItemText(hList,1);
	if(wcscmp(wHead,L"��������")==0)
	{
		memset((char *)wBuf, 0, sizeof(wBuf));
		memset((char *)wTxt, 0, sizeof(wTxt));
		wcscpy(wBuf, XList_GetItemText(hList, n, 1));
    	if (XEdit_GetTextLength(XWnd_GetEle(g_hWindow, IDE_ADD_REASONNAME)) > 40)
		{
			 XMessageBox(g_hWindow, L"��������̫��", L"����");
		     return false;
		}
		XEdit_GetText(XWnd_GetEle(g_hWindow, IDE_ADD_REASONNAME), wTxt,256);
		char szOldReason[64];
		W_2_S(wBuf,szOldReason,64);
		W_2_S(wTxt,szReason,64);
		if (wcscmp(wBuf,wTxt)&&wcslen(wTxt)>0)
		{
			sprintf(szT, "update t_reason set reason_name = \'%s\' where reason_name = \'%s\'",szReason, szOldReason);
			ret = tAdo.execUpdateInsert(szT);
		}
		else
		{
			return false;
		}
	}
	else
	{
		XMessageBox(g_hWindow,L"��ѡ�����������б�",L"����");
		return false;
	}
	OnQueryReason(XWnd_GetEle(g_hWindow, IDB_QUERYREASON), XWnd_GetEle(g_hWindow, IDB_QUERYREASON));
	XList_SetSelectItem(hList, 0);
	XEdit_SetText(XWnd_GetEle(g_hWindow, IDE_ADD_REASONNAME),L"");
	XWnd_RedrawWnd(XEle_GetHWindow(hEle));
    return true;
}
BOOL CALLBACK OnImportStation(HELE hEle, HELE hEventEle)
{
	char szFileName[MAX_PATH] = "�Ÿ�.xls";
	if(!OpenXlsDlg(szFileName))
		return false;
	if (!ImportStation(szFileName))
	{
		XMessageBox(g_hWindow,L"����ʧ�ܣ�",L"����");
		return false;
	}
	XMessageBox(g_hWindow,L"�Ÿ���Ϣ����ɹ�",L"��ʾ");
	return true;
}
BOOL CALLBACK OnExportStation(HELE hEle, HELE hEventEle)
{
	CString szpathDir="�Ÿ���Ϣ��.xls";
	if(!SaveXlsDlg(szpathDir))  //û��ѡ���ļ�·��
		return false;

	//szpathDir=szpathDir+"\\�Ÿ�.xls";
     //����xls��szpathDir
	CString tmp=szpathDir.Right(4);
	if(tmp.CompareNoCase(".xls")!=0) //û�к�׺�������ļ���׺
        szpathDir+=".xls";
    if(!ExportStation((LPSTR)(LPCTSTR)szpathDir))
	{
		XMessageBox(g_hWindow,L"����ʧ�ܿ��ܴ���ͬ���ļ���",L"����");
		return false;
	}
	XMessageBox(g_hWindow,L"�Ÿ���Ϣ�����ɹ�",L"��ʾ");
	return true;
}
BOOL CALLBACK OnSaveStation(HELE hEle, HELE hEventEle)
{
	if (hEle != hEventEle) 
	{
		return false;
	}
	wchar_t wBuf[256] = {0},wTxt[256]={0},*wHead=NULL;
	char  szStation[64];
	char szT[512] = {'\0'};
	HELE hList=GETELE(IDL_LISTGOODSORREASONORSTATION);
	CWinAdo tAdo(sCONNSTR_MYSQL);
	long ret;
    int n=XList_GetSelectItem(hList);

	if (n<0)
	{
		XMessageBox(g_hWindow,L"��ѡ���б���",L"��ʾ");
		return false;
	}
	ret = tAdo.OpenAdo();
	if (ret != ADOOK)
	{
		return false;
	}

	wHead = XList_GetHeaderItemText(hList,1);
	if(wcscmp(wHead,L"�Ÿ�")==0)
	{
		memset((char *)wBuf, 0, sizeof(wBuf));
		memset((char *)wTxt, 0, sizeof(wTxt));
		wcscpy(wBuf, XList_GetItemText(hList, n, 1));
		XEdit_GetText(XWnd_GetEle(g_hWindow, IDE_ADD_STATIONNAME), wTxt,256);
   	    if (XEdit_GetTextLength(XWnd_GetEle(g_hWindow, IDE_ADD_STATIONNAME)) > 4)
		{
			 XMessageBox(g_hWindow, L"�Ÿ���̫��", L"����");
		     return false;
		}
		char szOldStation[64];
		W_2_S(wBuf,szOldStation,64);
		W_2_S(wTxt,szStation,64);
		if (wcscmp(wBuf,wTxt)&&wcslen(wTxt)>0)
		{
			sprintf(szT, "update t_station set station_name = \'%s\' where station_name = \'%s\'",szStation, szOldStation);
			ret = tAdo.execUpdateInsert(szT);
		}
		else
		{
			return false;
		}
	}
	else
	{
		XMessageBox(g_hWindow,L"��ѡ���Ÿ��б�",L"����");
		return false;
	}
	OnQueryStation(XWnd_GetEle(g_hWindow, IDB_QUERYSTATION), XWnd_GetEle(g_hWindow, IDB_QUERYSTATION));
	XList_SetSelectItem(hList, 0);
	XEdit_SetText(XWnd_GetEle(g_hWindow, IDE_ADD_STATIONNAME),L"");
	XWnd_RedrawWnd(XEle_GetHWindow(hEle));
    return true;
}
BOOL CALLBACK OnImportCZY(HELE hEle, HELE hEventEle)
{
	char szFileName[MAX_PATH] = "����Ա.xls";
	if(!OpenXlsDlg(szFileName))
		return false;
	if (!ImportUser(szFileName))
	{
		XMessageBox(g_hWindow,L"����ʧ�ܣ�",L"����");
		return false;
	}
	XMessageBox(g_hWindow,L"����Ա��Ϣ����ɹ�",L"��ʾ");
	return true;
}
BOOL CALLBACK OnExportCZY(HELE hEle, HELE hEventEle)
{
	CString szpathDir="����Ա��.xls";
	if(!SaveXlsDlg(szpathDir))  //û��ѡ���ļ�·��
		return false;

	//szpathDir=szpathDir+"\\����Ա.xls";
     //����xls��szpathDir
	CString tmp=szpathDir.Right(4);
	if(tmp.CompareNoCase(".xls")!=0) //û�к�׺�������ļ���׺
        szpathDir+=".xls";
    if(!ExportUser((LPSTR)(LPCTSTR)szpathDir))
	{
		XMessageBox(g_hWindow,L"����ʧ�ܿ��ܴ���ͬ���ļ���",L"����");
		return false;
	}
	XMessageBox(g_hWindow,L"����Ա��Ϣ�����ɹ�",L"��ʾ");
	return true;
}
BOOL CALLBACK OnSaveCZY(HELE hEle, HELE hEventEle)
{
	if (hEle != hEventEle) 
	{
		return false;
	}
	wchar_t wBuf[256] = {0},wTxt[256]={0},*wHead=NULL;
	char  szUser[32],szRight[32],szPwd[32];
	char szT[512] = {'\0'};
	HELE hList=GETELE(IDL_LISTGOODSORREASONORSTATION);
	CWinAdo tAdo(sCONNSTR_MYSQL);
	long FldCount = 0, ret = 0;
	CString outParas[FLD_MAX],sRight;
    int n=XList_GetSelectItem(hList);

	if (n<0)
	{
		XMessageBox(g_hWindow,L"��ѡ���б���",L"��ʾ");
		return false;
	}
	ret = tAdo.OpenAdo();
	if (ret != ADOOK)
	{
		return false;
	}

	wHead = XList_GetHeaderItemText(hList,1);
	if(wcscmp(wHead,L"����Ա��")==0)
	{
		memset((char *)wBuf, 0, sizeof(wBuf));
		memset((char *)wTxt, 0, sizeof(wTxt));
		wcscpy(wBuf, XList_GetItemText(hList, n, 1));
		XEdit_GetText(XWnd_GetEle(g_hWindow, IDE_ADD_CZYID), wTxt,256);
  	    if (XEdit_GetTextLength(XWnd_GetEle(g_hWindow, IDE_ADD_CZYID)) > 10)
		{
			 XMessageBox(g_hWindow, L"����Ա��̫��", L"����");
		     return false;
		}
		char szOldUser[32];
		W_2_S(wBuf,szOldUser,32);
		W_2_S(wTxt,szUser,32);

		::GetPrivateProfileString("TerminalSet", "UserRight","", szRight, sizeof(szRight),CONFIG_FILE);
		if (atoi(szRight) == 0)
		{
            sprintf(szT, "select * from t_user where user_right_id=1 and user_id=\'%s\'",szUser);	//����Ա����һ�����Ա
			ret = tAdo.execSelect(szT, &FldCount, outParas);
			if (ret == ADOERR || (FldCount == 0 && outParas[0].IsEmpty()))
			{
				XMessageBox(g_hWindow,L"�˻������ڻ���Ȩ�޸�",L"��ʾ");
				return false;
			}	
		}
		else if (atoi(szRight) == 1)
		{
				XMessageBox(g_hWindow,L"һ�����Ա�����޸��˺���Ϣ������ϵ����Ա",L"��ʾ");
				return false;
		}

  
		memset((char *)wBuf, 0, sizeof(wBuf));
		memset((char *)wTxt, 0, sizeof(wTxt));
		wcscpy(wBuf, XList_GetItemText(hList, n, 2));
		XEdit_GetText(XWnd_GetEle(g_hWindow, IDE_ADD_CZYRIGHT), wTxt,256);
		if (wcscmp(wBuf,wTxt))
		{
			//W_2_S(wTxt,szRight,32);
			if (wcslen(wTxt)>0)
			{

				if (wcscmp(wTxt,L"����Ա")==0)
				{
					sprintf(szT, "update t_user set user_right_id = \'0\' where user_id = \'%s\'", szOldUser);
					ret = tAdo.execUpdateInsert(szT);
				}
				else if (wcscmp(wTxt,L"һ�����Ա")==0)
				{
					sprintf(szT, "update t_user set user_right_id = \'1\' where user_id = \'%s\'", szOldUser);
					ret = tAdo.execUpdateInsert(szT);
				}
			}
			else
			{
				XMessageBox(g_hWindow,L"Ȩ�޲���",L"����");
				return false;
			}
		}
		memset((char *)wBuf, 0, sizeof(wBuf));
		memset((char *)wTxt, 0, sizeof(wTxt));
		//wcscpy(wBuf, XList_GetItemText(hList, n, 9));
		XEdit_GetText(XWnd_GetEle(g_hWindow, IDE_ADD_CZYMM), wTxt,256);
  	    if (XEdit_GetTextLength(XWnd_GetEle(g_hWindow, IDE_ADD_CZYMM)) > 20)
		{
			 XMessageBox(g_hWindow, L"����̫��", L"����");
		     return false;
		}
		if (wcscmp(L"*******",wTxt))
		{
			if (wcslen(wTxt)>0)
			{
				W_2_S(wTxt,szPwd,32);
			    sprintf(szT, "update t_user set user_pwd = \'%s\' where user_id = \'%s\'",szPwd, szOldUser);
			    ret = tAdo.execUpdateInsert(szT);
			}
			else
			{
				XMessageBox(g_hWindow,L"���벻��Ϊ��",L"����");
				return false;
			}
			
		}
        //�����µ�¼��
		if (strcmp(szUser,szOldUser))
		{
			if (strlen(szUser)>0)
			{
				sprintf(szT, "update t_user set user_id = \'%s\' where user_id = \'%s\'",szUser, szOldUser);
			    ret = tAdo.execUpdateInsert(szT);
			}
			else
				return false;
		}
	}
	else
	{
		XMessageBox(g_hWindow,L"��ѡ�����Ա�б�",L"����");
		return false;
	}
	QueryCzy();
	XList_SetSelectItem(hList, 0);
	XWnd_RedrawWnd(XEle_GetHWindow(hEle));
	XEdit_SetText(XWnd_GetEle(g_hWindow, IDE_ADD_CZYID),L"");
	XEdit_SetText(XWnd_GetEle(g_hWindow, IDE_ADD_CZYMM),L"");
	XComboBox_SetSelectItem(XWnd_GetEle(g_hWindow, IDE_ADD_CZYRIGHT),0);
//	XMessageBox(g_hWindow,L"�ѱ���",L"��ʾ");
    return true;
}


BOOL CALLBACK OnSysConfig(HELE hEle, HELE hEventEle)
{
	if (hEle != hEventEle) 
	{
		return false;
	}
	
	XEle_ShowEle(XWnd_GetEle(g_hWindow, IDP_PAGE3_1), false);
	XEle_ShowEle(g_hPage4, false);	
	XEle_ShowEle(g_hPage5, false);
	XEle_ShowEle(g_hPage9, false);
	
	XEle_ShowEle(g_hPage8, true);

	EnableButton(IDB_TAB37);
	CheckAllMustInput();

    //SetOfficialEditBorder();
	//SetBlackNameEditBorder();
	SetOtherDlgEditBorder();

	char szT[256] = {'\0'};
	wchar_t wBuf[256] = {0};
	HELE hE = NULL;

	::GetPrivateProfileString("TerminalSet", "UnitName", "", szT, sizeof(szT), CONFIG_FILE);
	S_2_W(wBuf, szT, 256);
	hE = XWnd_GetEle(g_hWindow, IDE_CFG_UNITNAME);
	XEdit_SetText(hE, wBuf);

	::GetPrivateProfileString("TerminalSet", "UnitAddr", "", szT, sizeof(szT), CONFIG_FILE);
	S_2_W(wBuf, szT, 256);
	hE = XWnd_GetEle(g_hWindow, IDE_CFG_UNITADDR);
	XEdit_SetText(hE, wBuf);

	::GetPrivateProfileString("TerminalSet", "UnitPhone", "", szT, sizeof(szT), CONFIG_FILE);
	S_2_W(wBuf, szT, 256);
	hE = XWnd_GetEle(g_hWindow, IDE_CFG_UNITPHONE);
	XEdit_SetText(hE, wBuf);

	::GetPrivateProfileString("TerminalSet", "UnitFax", "", szT, sizeof(szT), CONFIG_FILE);
	S_2_W(wBuf, szT, 256);
	hE = XWnd_GetEle(g_hWindow, IDE_CFG_UNITFAX);
	XEdit_SetText(hE, wBuf);

	SetStationInfo(IDC_CFG_STATIONNAME);
	::GetPrivateProfileString("TerminalSet", "StationName", "", szT, sizeof(szT), CONFIG_FILE);
	S_2_W(wBuf, szT, 256);
	hE = XWnd_GetEle(g_hWindow, IDC_CFG_STATIONNAME);
	if (strlen(szT) <= 0)
	{
		XComboBox_SetSelectItem(hE, 0);
	}
	else
	{
		char szR[128] = {'\0'};
		for (int n = 0; n < g_nComboBoxCount; n++)
		{
			memset((char *)wBuf, 0, sizeof(wBuf));
			wcscpy(wBuf, XComboBox_GetItemText(hE, n));
			W_2_S(wBuf, szR, 256);
			if (memcmp(szR, szT, strlen(szT)) == 0)
			{
				XComboBox_SetSelectItem(hE, n);
				break;
			}
		}
	}

	::GetPrivateProfileString("TerminalSet", "FtpSeverName", "", szT, sizeof(szT), CONFIG_FILE);
	S_2_W(wBuf, szT, 256);
	hE = XWnd_GetEle(g_hWindow, IDE_FTPADDR);
	XEdit_SetText(hE, wBuf);

	::GetPrivateProfileString("TerminalSet", "FtpUser", "", szT, sizeof(szT), CONFIG_FILE);
	S_2_W(wBuf, szT, 256);
	hE = XWnd_GetEle(g_hWindow, IDE_FTPUSER);
	XEdit_SetText(hE, wBuf);

	::GetPrivateProfileString("TerminalSet", "FtpPwd", "", szT, sizeof(szT), CONFIG_FILE);
	S_2_W(wBuf, szT, 256);
	hE = XWnd_GetEle(g_hWindow, IDE_FTPPWD);
	XEdit_SetText(hE, wBuf);

	::GetPrivateProfileString("TerminalSet", "PrefixPhone", "", szT, sizeof(szT), CONFIG_FILE);
	S_2_W(wBuf, szT, 256);
	hE = XWnd_GetEle(g_hWindow, IDE_CFG_PrefixPhone);
	XEdit_SetText(hE, wBuf);

	::GetPrivateProfileString("ReceiptPrinter", "Port", "", szT, sizeof(szT), CONFIG_FILE);
	S_2_W(wBuf, szT, 256);
	hE = XWnd_GetEle(g_hWindow, IDE_CFG_RECEIPTPORT);
	XEdit_SetText(hE, wBuf);

	::GetPrivateProfileString("BarCode", "Port", "", szT, sizeof(szT), CONFIG_FILE);
	S_2_W(wBuf, szT, 256);
	hE = XWnd_GetEle(g_hWindow, IDE_CFG_BARCODEPORT);
	XEdit_SetText(hE, wBuf);

	::GetPrivateProfileString("BarCode", "InfrarePort", "", szT, sizeof(szT), CONFIG_FILE);
	S_2_W(wBuf, szT, 256);
	hE = XWnd_GetEle(g_hWindow, IDE_INFRARED);
	XEdit_SetText(hE, wBuf);

	BOOL bSend=::GetPrivateProfileInt("TerminalSet", "bSendRequest",0, CONFIG_FILE);
    if (bSend==1)
    {
		XBtn_SetCheck(GETELE(IDB_OPEN_SENDREQUEST), true);
    }
	else
	{
        XBtn_SetCheck(GETELE(IDB_OPEN_SENDREQUEST), false);
	}

    if (GetPrivateProfileInt("TerminalSet", "bVisitorReadOnly",0, CONFIG_FILE)==1)
    {
		XBtn_SetCheck(GETELE(IDK_IMPORT_CZY), true);
    }
	else
	{
        XBtn_SetCheck(GETELE(IDK_IMPORT_CZY), false);
	}
	
    if (GetPrivateProfileInt("TerminalSet", "bVisitorPicMust",0, CONFIG_FILE)==1)
    {
		XBtn_SetCheck(GETELE(IDC_VISITORPIC), true);
    }
	else
	{
        XBtn_SetCheck(GETELE(IDC_VISITORPIC), false);
	}

	if (GetPrivateProfileInt("TerminalSet", "bHideLast4ID",0, CONFIG_FILE)==1)
	{
        XBtn_SetCheck(GETELE(IDC_HIDELAST4ID), true);
	}
	else
	{
        XBtn_SetCheck(GETELE(IDC_HIDELAST4ID), false);
	}

	if (GetPrivateProfileInt("TerminalSet", "bHideGoodsPrint",0, CONFIG_FILE)==1)
	{
        XBtn_SetCheck(GETELE(IDC_GOODS_Print), true);
	}
	else
	{
        XBtn_SetCheck(GETELE(IDC_GOODS_Print), false);
	}

	if (GetPrivateProfileInt("TerminalSet", "bHideToRoomPrint",0, CONFIG_FILE)==1)
	{
        XBtn_SetCheck(GETELE(IDC_ROOM_Print), true);
	}
	else
	{
        XBtn_SetCheck(GETELE(IDC_ROOM_Print), false);
	}

	if (GetPrivateProfileInt("TerminalSet", "bHideTotalPeoplePrint",0, CONFIG_FILE)==1)
	{
        XBtn_SetCheck(GETELE(IDC_People_Print), true);
	}
	else
	{
        XBtn_SetCheck(GETELE(IDC_People_Print), false);
	}

	if (GetPrivateProfileInt("TerminalSet", "bPrintTips2",0, CONFIG_FILE)==1)
	{
        XBtn_SetCheck(GETELE(IDC_Tips2_Print), true);
	}
	else
	{
        XBtn_SetCheck(GETELE(IDC_Tips2_Print), false);
	}

	
	if (GetPrivateProfileInt("TerminalSet", "bPrintAfterAllow",0, CONFIG_FILE)==1)
	{
        XBtn_SetCheck(GETELE(IDC_PRINTAFTERALLOW), true);
	}
	else
	{
        XBtn_SetCheck(GETELE(IDC_PRINTAFTERALLOW), false);
	}

	if (GetPrivateProfileInt("TerminalSet", "bSaveNoPrint",0, CONFIG_FILE)==1)
	{
        XBtn_SetCheck(GETELE(IDC_SAVENOPRINT), true);
	}
	else
	{
        XBtn_SetCheck(GETELE(IDC_SAVENOPRINT), false);
	}	

	if (GetPrivateProfileInt("TerminalSet", "bSaveOfficial",0, CONFIG_FILE)==1)
	{
        XBtn_SetCheck(GETELE(IDC_SAVEOFFICIAL), true);
	}
	else
	{
        XBtn_SetCheck(GETELE(IDC_SAVEOFFICIAL), false);
	}	
	

	if (GetPrivateProfileInt("TerminalSet", "bFtpOpen",0, CONFIG_FILE)==1)
	{
        XBtn_SetCheck(GETELE(IDC_FTPOPEN), true);
	}
	else
	{
        XBtn_SetCheck(GETELE(IDC_FTPOPEN), false);
	}	

	if (GetPrivateProfileInt("TerminalSet", "bStationExp",0, CONFIG_FILE)==1)
	{
        XBtn_SetCheck(GETELE(IDC_Station_EXPT), true);
	}
	else
	{
        XBtn_SetCheck(GETELE(IDC_Station_EXPT), false);
	}

	if (GetPrivateProfileInt("TerminalSet", "bVisitorExp",0, CONFIG_FILE)==1)
	{
        XBtn_SetCheck(GETELE(IDC_Visitor_EXPT), true);
	}
	else
	{
        XBtn_SetCheck(GETELE(IDC_Visitor_EXPT), false);
	}

	if (GetPrivateProfileInt("TerminalSet", "bVSexExp",0, CONFIG_FILE)==1)
	{
        XBtn_SetCheck(GETELE(IDC_VSex_EXPT), true);
	}
	else
	{
        XBtn_SetCheck(GETELE(IDC_VSex_EXPT), false);
	}

	if (GetPrivateProfileInt("TerminalSet", "bVFolkExp",0, CONFIG_FILE)==1)
	{
        XBtn_SetCheck(GETELE(IDC_VFolk_EXPT), true);
	}
	else
	{
        XBtn_SetCheck(GETELE(IDC_VFolk_EXPT), false);
	}

	if (GetPrivateProfileInt("TerminalSet", "bIDTypeExp",0, CONFIG_FILE)==1)
	{
        XBtn_SetCheck(GETELE(IDC_IDType_EXPT), true);
	}
	else
	{
        XBtn_SetCheck(GETELE(IDC_IDType_EXPT), false);
	}

	if (GetPrivateProfileInt("TerminalSet", "bIDExp",0, CONFIG_FILE)==1)
	{
        XBtn_SetCheck(GETELE(IDC_ID_EXPT), true);
	}
	else
	{
        XBtn_SetCheck(GETELE(IDC_ID_EXPT), false);
	}

	if (GetPrivateProfileInt("TerminalSet", "bAddrExp",0, CONFIG_FILE)==1)
	{
        XBtn_SetCheck(GETELE(IDC_Addr_EXPT), true);
	}
	else
	{
        XBtn_SetCheck(GETELE(IDC_Addr_EXPT), false);
	}

	if (GetPrivateProfileInt("TerminalSet", "bSigndepartmentExp",0, CONFIG_FILE)==1)
	{
        XBtn_SetCheck(GETELE(IDC_Signdepartment_EXPT), true);
	}
	else
	{
        XBtn_SetCheck(GETELE(IDC_Signdepartment_EXPT), false);
	}

	if (GetPrivateProfileInt("TerminalSet", "bUnitExp",0, CONFIG_FILE)==1)
	{
        XBtn_SetCheck(GETELE(IDC_Unit_EXPT), true);
	}
	else
	{
        XBtn_SetCheck(GETELE(IDC_Unit_EXPT), false);
	}

	if (GetPrivateProfileInt("TerminalSet", "bTelExp",0, CONFIG_FILE)==1)
	{
        XBtn_SetCheck(GETELE(IDC_Tel_EXPT), true);
	}
	else
	{
        XBtn_SetCheck(GETELE(IDC_Tel_EXPT), false);
	}

	if (GetPrivateProfileInt("TerminalSet", "bCarNumExp",0, CONFIG_FILE)==1)
	{
        XBtn_SetCheck(GETELE(IDC_CarNum_EXPT), true);
	}
	else
	{
        XBtn_SetCheck(GETELE(IDC_CarNum_EXPT), false);
	}

	if (GetPrivateProfileInt("TerminalSet", "bVisittimeExp",0, CONFIG_FILE)==1)
	{
        XBtn_SetCheck(GETELE(IDC_Visittime_EXPT), true);
	}
	else
	{
        XBtn_SetCheck(GETELE(IDC_Visittime_EXPT), false);
	}

	if (GetPrivateProfileInt("TerminalSet", "bReasonExp",0, CONFIG_FILE)==1)
	{
        XBtn_SetCheck(GETELE(IDC_Reason_EXPT), true);
	}
	else
	{
        XBtn_SetCheck(GETELE(IDC_Reason_EXPT), false);
	}

	if (GetPrivateProfileInt("TerminalSet", "bGoodsExp",0, CONFIG_FILE)==1)
	{
        XBtn_SetCheck(GETELE(IDC_Goods_EXPT), true);
	}
	else
	{
        XBtn_SetCheck(GETELE(IDC_Goods_EXPT), false);
	}

	if (GetPrivateProfileInt("TerminalSet", "bLefttimeExp",0, CONFIG_FILE)==1)
	{
        XBtn_SetCheck(GETELE(IDC_Lefttime_EXPT), true);
	}
	else
	{
        XBtn_SetCheck(GETELE(IDC_Lefttime_EXPT), false);
	}

	if (GetPrivateProfileInt("TerminalSet", "bTotalPeopleExp",0, CONFIG_FILE)==1)
	{
        XBtn_SetCheck(GETELE(IDC_TotalPeople_EXPT), true);
	}
	else
	{
        XBtn_SetCheck(GETELE(IDC_TotalPeople_EXPT), false);
	}

	if (GetPrivateProfileInt("TerminalSet", "bOfficerExp",0, CONFIG_FILE)==1)
	{
        XBtn_SetCheck(GETELE(IDC_Officer_EXPT), true);
	}
	else
	{
        XBtn_SetCheck(GETELE(IDC_Officer_EXPT), false);
	}

	if (GetPrivateProfileInt("TerminalSet", "bOfficeNameExp",0, CONFIG_FILE)==1)
	{
        XBtn_SetCheck(GETELE(IDC_OfficeName_EXPT), true);
	}
	else
	{
        XBtn_SetCheck(GETELE(IDC_OfficeName_EXPT), false);
	}

	if (GetPrivateProfileInt("TerminalSet", "bOfficePhoneExp",0, CONFIG_FILE)==1)
	{
        XBtn_SetCheck(GETELE(IDC_OfficePhone_EXPT), true);
	}
	else
	{
        XBtn_SetCheck(GETELE(IDC_OfficePhone_EXPT), false);
	}

	XWnd_RedrawWnd(XEle_GetHWindow(hEle));
	return false;
}

BOOL CALLBACK OnImportOffice(HELE hEle, HELE hEventEle)
{
    char szFileName[MAX_PATH] = "���ұ�.xls";
	if(!OpenXlsDlg(szFileName))
		return false;
	if (!ImportOffice(szFileName))
	{
		XMessageBox(g_hWindow,L"����ʧ�ܣ�",L"����");
		return false;
	}
	XMessageBox(g_hWindow,L"���ұ���Ϣ����ɹ�",L"��ʾ");
	return true;
}
BOOL CALLBACK OnExportOffice(HELE hEle, HELE hEventEle)
{
	CString szpathDir="���ұ�.xls";
	if(!SaveXlsDlg(szpathDir))  //û��ѡ���ļ�·��
		return false;

	//szpathDir=szpathDir+"\\���ұ�.xls";
     //����xls��szpathDir
	CString tmp=szpathDir.Right(4);
	if(tmp.CompareNoCase(".xls")!=0) //û�к�׺�������ļ���׺
        szpathDir+=".xls";
    if(!ExportOffice((LPSTR)(LPCTSTR)szpathDir))
	{
		XMessageBox(g_hWindow,L"����ʧ�ܿ��ܴ���ͬ���ļ���",L"����");
		return false;
	}
	XMessageBox(g_hWindow,L"���ұ���Ϣ�����ɹ�",L"��ʾ");
	return true;
}
BOOL CALLBACK OnSaveOffice(HELE hEle, HELE hEventEle)
{
    if (hEle != hEventEle) 
	{
		return false;
	}
	wchar_t wBuf[256] = {0},wTxt[256] = {0},*wHead=NULL;
	char  szOffice[32],szPhone[32],szAddr[32],szRoom[32];
	char szT[512] = {'\0'};
	HELE hList=GETELE(IDL_LISTOFFICEORLEVEL);
	CWinAdo tAdo(sCONNSTR_MYSQL);
	long ret;
    int n=XList_GetSelectItem(hList);

	if (n<0)
	{
		XMessageBox(g_hWindow,L"��ѡ���б���",L"��ʾ");
		return false;
	}
	ret = tAdo.OpenAdo();
	if (ret != ADOOK)
	{
		return false;
	}

	wHead = XList_GetHeaderItemText(hList,1);
	if(wcscmp(wHead,L"��������")==0)
	{
		memset((char *)wBuf, 0, sizeof(wBuf));
		memset((char *)wTxt, 0, sizeof(wTxt));
		wcscpy(wBuf, XList_GetItemText(hList, n, 1));
		XEdit_GetText(XWnd_GetEle(g_hWindow, IDE_ADD_OFFICE_NAME), wTxt,256);
		char szOldOffice[32];
		W_2_S(wBuf,szOldOffice,32);
		W_2_S(wTxt,szOffice,32);
		
		
		memset((char *)wBuf, 0, sizeof(wBuf));
		memset((char *)wTxt, 0, sizeof(wTxt));
		wcscpy(wBuf, XList_GetItemText(hList, n, 2));
		XEdit_GetText(XWnd_GetEle(g_hWindow, IDE_ADD_OFFICE_PHONE), wTxt,256);
		if (wcscmp(wBuf,wTxt))
		{
			//if (wcslen(wTxt)>0)
			{
				W_2_S(wTxt,szPhone,32);
				sprintf(szT, "update t_office set office_phone = \'%s\' where office_name = \'%s\'",szPhone, szOldOffice);
				ret = tAdo.execUpdateInsert(szT);
			}
			
		}
		
		memset((char *)wBuf, 0, sizeof(wBuf));
		memset((char *)wTxt, 0, sizeof(wTxt));
		wcscpy(wBuf, XList_GetItemText(hList, n, 3));
		XEdit_GetText(XWnd_GetEle(g_hWindow, IDE_ADD_OFFICE_ADDR), wTxt,256);
		if (wcscmp(wBuf,wTxt))
		{
			W_2_S(wTxt,szAddr,32);
			sprintf(szT, "update t_office set office_addr = \'%s\' where office_name = \'%s\'",szAddr, szOldOffice);
			ret = tAdo.execUpdateInsert(szT);
		}
		
		memset((char *)wBuf, 0, sizeof(wBuf));
		memset((char *)wTxt, 0, sizeof(wTxt));
		wcscpy(wBuf, XList_GetItemText(hList, n, 4));
		XEdit_GetText(XWnd_GetEle(g_hWindow, IDE_ADD_OFFICE_ROOMNUM), wTxt,256);
		if (wcscmp(wBuf,wTxt))
		{
			W_2_S(wTxt,szRoom,32);
			sprintf(szT, "update t_office set office_roomnum = \'%s\' where official_name = \'%s\'",szRoom, szOldOffice);
			ret = tAdo.execUpdateInsert(szT);
		}	
		
		if (strcmp(szOffice,szOldOffice))
		{	
			sprintf(szT, "update t_office set office_name = \'%s\' where office_name = \'%s\'",szOffice, szOldOffice);
			ret = tAdo.execUpdateInsert(szT);
		}
	}
	else
	{
		XMessageBox(g_hWindow,L"��ѡ������б�",L"����");
		return false;
	}		

	//OnQueryEmployerForList(XWnd_GetEle(g_hWindow, IDB_QUERYOFFICIAL), XWnd_GetEle(g_hWindow, IDB_QUERYOFFICIAL));
	OnQueryOffice(XWnd_GetEle(g_hWindow, IDB_QUERYOFFICIAL), XWnd_GetEle(g_hWindow, IDB_QUERYOFFICIAL));
	XList_SetSelectItem(hList, 0);
	XWnd_RedrawWnd(XEle_GetHWindow(hEle));
    return true;
}
BOOL CALLBACK OnSaveLevel(HELE hEle, HELE hEventEle)
{
	if (hEle != hEventEle) 
	{
		return false;
	}
	wchar_t wBuf[256] = {0},wTxt[256]={0},*wHead=NULL;
	char  szLevel[32];
	char szT[512] = {'\0'};
	HELE hList=GETELE(IDL_LISTOFFICEORLEVEL);
	CWinAdo tAdo(sCONNSTR_MYSQL);
	long ret;
    int n=XList_GetSelectItem(hList);

	if (n<0)
	{
		XMessageBox(g_hWindow,L"��ѡ���б���",L"��ʾ");
		return false;
	}
	ret = tAdo.OpenAdo();
	if (ret != ADOOK)
	{
		return false;
	}

	wHead = XList_GetHeaderItemText(hList,1);
	if(wcscmp(wHead,L"��������")==0)
	{
		memset((char *)wBuf, 0, sizeof(wBuf));
		memset((char *)wTxt, 0, sizeof(wTxt));
		wcscpy(wBuf, XList_GetItemText(hList, n, 1));
		XEdit_GetText(XWnd_GetEle(g_hWindow, IDE_ADD_LEVELNAME), wTxt,256);
		char szOldLevel[32];
		W_2_S(wBuf,szOldLevel,32);
		W_2_S(wTxt,szLevel,32);
		if (wcscmp(wBuf,wTxt)&&wcslen(wTxt)>0)
		{
			sprintf(szT, "update t_duty set duty_name = \'%s\' where duty_name = \'%s\'",szLevel, szOldLevel);
			ret = tAdo.execUpdateInsert(szT);
		}
		else
		{
			return false;
		}
	}
	else
	{
		XMessageBox(g_hWindow,L"��ѡ�񼶱��б�",L"����");
		return false;
	}
	OnQueryLevel(XWnd_GetEle(g_hWindow, IDB_QUERYLEVEL), XWnd_GetEle(g_hWindow, IDB_QUERYLEVEL));
	XList_SetSelectItem(hList, 0);
	XWnd_RedrawWnd(XEle_GetHWindow(hEle));
    return true;
}

BOOL CALLBACK OnBaseInfo(HELE hEle, HELE hEventEle)
{
	if (hEle != hEventEle) 
	{
		return false;
	}
	
	XEle_ShowEle(g_hPage3, false);			//����ϵͳ����ҳ��
	XEle_ShowEle(g_hPage4, false);			//����ϵͳ����ҳ��
	XEle_ShowEle(g_hPage5, false);			//����ϵͳ����ҳ��
	XEle_ShowEle(g_hPage8, false);
	XEle_ShowEle(g_hPage9, false);
	g_bICReg = false;
	/*XEle_ShowEle(g_hPage9, true);
	EnableButton(IDB_TAB38);*/

	//SetOfficialEditBorder();
	//SetBlackNameEditBorder();
	//SetOtherDlgEditBorder();
	XBtn_SetCheck(GETELE(IDB_CHAXUN), true);
	XBtn_SetCheck(GETELE(IDB_TONGJI), false);
	XBtn_SetCheck(GETELE(IDB_TONGJICHAXUNRETURN), false);


    ShowWindow(g_hCapWnd, false);
	Pause();
	XEle_ShowEle(g_hPage1, false);
	XEle_ShowEle(g_hPage6, true);

	ChaXunToDayInfo();

	XWnd_RedrawWnd(XEle_GetHWindow(hEle));
	return false;
}

//ϵͳ�������ñ���
BOOL CALLBACK OnConfigSave(HELE hEle, HELE hEventEle)
{
	char szT[256] = {'\0'};
	wchar_t wBuf[256] = {0};
	HELE hE = NULL;
	
	memset(szT, 0, sizeof(szT));
	memset((char *)wBuf, 0, sizeof(wBuf));
	hE = XWnd_GetEle(g_hWindow, IDE_CFG_UNITNAME);
	XEdit_GetText(hE, wBuf, 256);
	W_2_S(wBuf, szT, 256);
	::WritePrivateProfileString("TerminalSet", "UnitName", szT, CONFIG_FILE);
	
	memset(szT, 0, sizeof(szT));
	memset((char *)wBuf, 0, sizeof(wBuf));
	hE = XWnd_GetEle(g_hWindow, IDE_CFG_UNITADDR);
	XEdit_GetText(hE, wBuf, 256);
	W_2_S(wBuf, szT, 256);
	::WritePrivateProfileString("TerminalSet", "UnitAddr", szT, CONFIG_FILE);
	
	memset(szT, 0, sizeof(szT));
	memset((char *)wBuf, 0, sizeof(wBuf));
	hE = XWnd_GetEle(g_hWindow, IDE_CFG_UNITPHONE);
	XEdit_GetText(hE, wBuf, 256);
	W_2_S(wBuf, szT, 256);
	::WritePrivateProfileString("TerminalSet", "UnitPHONE", szT, CONFIG_FILE);
	
	memset(szT, 0, sizeof(szT));
	memset((char *)wBuf, 0, sizeof(wBuf));
	hE = XWnd_GetEle(g_hWindow, IDE_CFG_UNITFAX);
	XEdit_GetText(hE, wBuf, 256);
	W_2_S(wBuf, szT, 256);
	::WritePrivateProfileString("TerminalSet", "UnitFax", szT, CONFIG_FILE);

	memset(szT, 0, sizeof(szT));
	memset((char *)wBuf, 0, sizeof(wBuf));
	hE = XWnd_GetEle(g_hWindow, IDE_CFG_PrefixPhone);
	XEdit_GetText(hE, wBuf, 256);
	W_2_S(wBuf, szT, 256);
	::WritePrivateProfileString("TerminalSet", "PrefixPhone", szT, CONFIG_FILE);

	memset(szT, 0, sizeof(szT));
	memset((char *)wBuf, 0, sizeof(wBuf));
	hE = XWnd_GetEle(g_hWindow, IDE_FTPADDR);
	XEdit_GetText(hE, wBuf, 256);
	W_2_S(wBuf, szT, 256);
	::WritePrivateProfileString("TerminalSet", "FtpSeverName", szT, CONFIG_FILE);

	memset(szT, 0, sizeof(szT));
	memset((char *)wBuf, 0, sizeof(wBuf));
	hE = XWnd_GetEle(g_hWindow, IDE_FTPUSER);
	XEdit_GetText(hE, wBuf, 256);
	W_2_S(wBuf, szT, 256);
	::WritePrivateProfileString("TerminalSet", "FtpUser", szT, CONFIG_FILE);

	memset(szT, 0, sizeof(szT));
	memset((char *)wBuf, 0, sizeof(wBuf));
	hE = XWnd_GetEle(g_hWindow, IDE_FTPPWD);
	XEdit_GetText(hE, wBuf, 256);
	W_2_S(wBuf, szT, 256);
	::WritePrivateProfileString("TerminalSet", "FtpPwd", szT, CONFIG_FILE);

	memset(szT, 0, sizeof(szT));
	memset((char *)wBuf, 0, sizeof(wBuf));
	hE = XWnd_GetEle(g_hWindow, IDE_CFG_RECEIPTPORT);
	XEdit_GetText(hE, wBuf, 256);
	W_2_S(wBuf, szT, 256);
	::WritePrivateProfileString("ReceiptPrinter", "Port", szT, CONFIG_FILE);
	
	memset(szT, 0, sizeof(szT));
	memset((char *)wBuf, 0, sizeof(wBuf));
	hE = XWnd_GetEle(g_hWindow, IDE_CFG_BARCODEPORT);
	XEdit_GetText(hE, wBuf, 256);
	W_2_S(wBuf, szT, 256);
	::WritePrivateProfileString("BarCode", "Port", szT, CONFIG_FILE);

	memset(szT, 0, sizeof(szT));
	memset((char *)wBuf, 0, sizeof(wBuf));
	hE = XWnd_GetEle(g_hWindow, IDE_INFRARED);
	XEdit_GetText(hE, wBuf, 256);
	W_2_S(wBuf, szT, 256);
	::WritePrivateProfileString("BarCode", "InfrarePort", szT, CONFIG_FILE);

	hE = GETELE(IDC_Station_EXPT);
    if (XBtn_IsCheck(hE))
	{
        ::WritePrivateProfileString("TerminalSet", "bStationExp", "1", CONFIG_FILE);
	}
	else
        ::WritePrivateProfileString("TerminalSet", "bStationExp", "0", CONFIG_FILE);

	hE = GETELE(IDC_Visitor_EXPT);
    if (XBtn_IsCheck(hE))
	{
        ::WritePrivateProfileString("TerminalSet", "bVisitorExp", "1", CONFIG_FILE);
	}
	else
        ::WritePrivateProfileString("TerminalSet", "bVisitorExp", "0", CONFIG_FILE);

	hE = GETELE(IDC_VSex_EXPT);
    if (XBtn_IsCheck(hE))
	{
        ::WritePrivateProfileString("TerminalSet", "bVSexExp", "1", CONFIG_FILE);
	}
	else
        ::WritePrivateProfileString("TerminalSet", "bVSexExp", "0", CONFIG_FILE);

	hE = GETELE(IDC_VFolk_EXPT);
    if (XBtn_IsCheck(hE))
	{
        ::WritePrivateProfileString("TerminalSet", "bVFolkExp", "1", CONFIG_FILE);
	}
	else
        ::WritePrivateProfileString("TerminalSet", "bVFolkExp", "0", CONFIG_FILE);

	hE = GETELE(IDC_IDType_EXPT);
    if (XBtn_IsCheck(hE))
	{
        ::WritePrivateProfileString("TerminalSet", "bIDTypeExp", "1", CONFIG_FILE);
	}
	else
        ::WritePrivateProfileString("TerminalSet", "bIDTypeExp", "0", CONFIG_FILE);

	hE = GETELE(IDC_ID_EXPT);
    if (XBtn_IsCheck(hE))
	{
        ::WritePrivateProfileString("TerminalSet", "bIDExp", "1", CONFIG_FILE);
	}
	else
        ::WritePrivateProfileString("TerminalSet", "bIDExp", "0", CONFIG_FILE);

	hE = GETELE(IDC_Addr_EXPT);
    if (XBtn_IsCheck(hE))
	{
        ::WritePrivateProfileString("TerminalSet", "bAddrExp", "1", CONFIG_FILE);
	}
	else
        ::WritePrivateProfileString("TerminalSet", "bAddrExp", "0", CONFIG_FILE);

	hE = GETELE(IDC_Signdepartment_EXPT);
    if (XBtn_IsCheck(hE))
	{
        ::WritePrivateProfileString("TerminalSet", "bSigndepartmentExp", "1", CONFIG_FILE);
	}
	else
        ::WritePrivateProfileString("TerminalSet", "bSigndepartmentExp", "0", CONFIG_FILE);

	hE = GETELE(IDC_Unit_EXPT);
    if (XBtn_IsCheck(hE))
	{
        ::WritePrivateProfileString("TerminalSet", "bUnitExp", "1", CONFIG_FILE);
	}
	else
        ::WritePrivateProfileString("TerminalSet", "bUnitExp", "0", CONFIG_FILE);

	hE = GETELE(IDC_Tel_EXPT);
    if (XBtn_IsCheck(hE))
	{
        ::WritePrivateProfileString("TerminalSet", "bTelExp", "1", CONFIG_FILE);
	}
	else
        ::WritePrivateProfileString("TerminalSet", "bTelExp", "0", CONFIG_FILE);

	hE = GETELE(IDC_CarNum_EXPT);
    if (XBtn_IsCheck(hE))
	{
        ::WritePrivateProfileString("TerminalSet", "bCarNumExp", "1", CONFIG_FILE);
	}
	else
        ::WritePrivateProfileString("TerminalSet", "bCarNumExp", "0", CONFIG_FILE);

	hE = GETELE(IDC_Visittime_EXPT);
    if (XBtn_IsCheck(hE))
	{
        ::WritePrivateProfileString("TerminalSet", "bVisittimeExp", "1", CONFIG_FILE);
	}
	else
        ::WritePrivateProfileString("TerminalSet", "bVisittimeExp", "0", CONFIG_FILE);

	hE = GETELE(IDC_Reason_EXPT);
    if (XBtn_IsCheck(hE))
	{
        ::WritePrivateProfileString("TerminalSet", "bReasonExp", "1", CONFIG_FILE);
	}
	else
        ::WritePrivateProfileString("TerminalSet", "bReasonExp", "0", CONFIG_FILE);

	hE = GETELE(IDC_Reason_EXPT);
    if (XBtn_IsCheck(hE))
	{
        ::WritePrivateProfileString("TerminalSet", "bGoodsExp", "1", CONFIG_FILE);
	}
	else
        ::WritePrivateProfileString("TerminalSet", "bGoodsExp", "0", CONFIG_FILE);

	hE = GETELE(IDC_Lefttime_EXPT);
    if (XBtn_IsCheck(hE))
	{
        ::WritePrivateProfileString("TerminalSet", "bLefttimeExp", "1", CONFIG_FILE);
	}
	else
        ::WritePrivateProfileString("TerminalSet", "bLefttimeExp", "0", CONFIG_FILE);

	hE = GETELE(IDC_TotalPeople_EXPT);
    if (XBtn_IsCheck(hE))
	{
        ::WritePrivateProfileString("TerminalSet", "bTotalPeopleExp", "1", CONFIG_FILE);
	}
	else
        ::WritePrivateProfileString("TerminalSet", "bTotalPeopleExp", "0", CONFIG_FILE);

	hE = GETELE(IDC_Officer_EXPT);
    if (XBtn_IsCheck(hE))
	{
        ::WritePrivateProfileString("TerminalSet", "bOfficerExp", "1", CONFIG_FILE);
	}
	else
        ::WritePrivateProfileString("TerminalSet", "bOfficerExp", "0", CONFIG_FILE);

	hE = GETELE(IDC_OfficeName_EXPT);
    if (XBtn_IsCheck(hE))
	{
        ::WritePrivateProfileString("TerminalSet", "bOfficeNameExp", "1", CONFIG_FILE);
	}
	else
        ::WritePrivateProfileString("TerminalSet", "bOfficeNameExp", "0", CONFIG_FILE);

	hE = GETELE(IDC_OfficePhone_EXPT);
    if (XBtn_IsCheck(hE))
	{
        ::WritePrivateProfileString("TerminalSet", "bOfficePhoneExp", "1", CONFIG_FILE);
	}
	else
        ::WritePrivateProfileString("TerminalSet", "bOfficePhoneExp", "0", CONFIG_FILE);

	int result=XMessageBox(g_hWindow,L"���ѱ���ɹ�����[ȷ��]���������Ч��",L"��ʾ",XMB_OK|XMB_CANCEL);
    if(XMB_OK==result)
    {
        //ȷ����ť
        if (g_hWeb != NULL)
		{
			XWeb_Destroy(g_hWeb);
		}
		OleUninitialize();
		g_bStopID2 = true;			//ֹͣ����֤��д�߳�
		g_bStopBarCode = true;		//ֹͣ�����Ķ��߳�	
		FreeIdcardLibrary();		//ж�ض���֤��д��
		EndWJUsb();
		CloseCardReader();
		CloseSoftKeybd();
		CloseXYB();
		CloseBackScreen();
		HideTaskBar(false);
		WaitForSingleObject(g_ThreadSocketMutex,INFINITE);
        g_tSock->CloseSock();
		ReleaseMutex(g_ThreadSocketMutex);
		XWnd_CloseWindow(XEle_GetHWindow(hEle));
    }
	return false;
}

//��ϵͳ����ҳ�淵��
BOOL CALLBACK OnReturnSet(HELE hEle, HELE hEventEle)
{
	if (hEle != hEventEle) 
	{
		return false;
	}

	XEle_ShowEle(g_hPage3, false);			//����ϵͳ����ҳ��
	XEle_ShowEle(g_hPage4, false);			//����ϵͳ����ҳ��
	XEle_ShowEle(g_hPage5, false);			//����ϵͳ����ҳ��
	XEle_ShowEle(g_hPage6, false);
	XEle_ShowEle(g_hPage8, false);
	XEle_ShowEle(g_hPage9, false);
	g_bICReg = false;
	XEle_ShowEle(g_hPage1, true);			//��ʾ������
	if(g_bShowCap)
	{
		ShowWindow(g_hCapWnd, true);
        RePlayCapture();//RePlay();
	}

	EnableButton(IDB_TAB1);					//����������󼤻���ʾ��һ����ť

    SetMainDlgEditBorder();
	SetOfficialEditBorder();
	SetBlackNameEditBorder();
	SetOtherDlgEditBorder();
	HideSoftKeybd();
	ResumeThread(g_HWDThreadID2ID);
	ResumeThread(g_ThreadReadBarcode);
	XWnd_RedrawWnd(XEle_GetHWindow(hEle));
	ShowWindow(g_hCapWnd,true);
	RePlayCapture();//RePlay();
	return true;
}

void   CALLBACK   TimerPro(HWND   hwnd,UINT   message,UINT   iTIMERID,DWORD   dwTIMER)
{ 
	//SendCmd(CTL_USB_ON);
	//��ʱ�ر�����ͷ
    switch(iTIMERID) {
	case TimerCapture:
	   {
			if (g_nCaptureTick-- < 1 && g_bVideoOpened)
			{
				Pause();
				CloseCapture();
				//Pause();
				g_bVideoOpened = false;
				XEle_EnableEle(g_hTab1, false);

			}
	    }
	   XWnd_RedrawWnd(g_hWindow);
	 break;
	case TimerReFresh:
		{
			CString str,sTime;
			char szT[100];
			wchar_t wBuf[100];
            CTime CurrentTime=CTime::GetCurrentTime();
			sTime="ʱ��: ";
			if (CurrentTime.GetHour()<10)
			{
				str.Format("0%d:",CurrentTime.GetHour());
			}
			else
                str.Format("%d:",CurrentTime.GetHour());
			sTime+=str;

			if (CurrentTime.GetMinute()<10)
			{
				str.Format("0%d:",CurrentTime.GetMinute());
			}
			else
                str.Format("%d:",CurrentTime.GetMinute());
			sTime+=str;

			if (CurrentTime.GetSecond()<10)
			{
				str.Format("0%d",CurrentTime.GetSecond());
			}
			else
                str.Format("%d",CurrentTime.GetSecond());
			sTime+=str;

            S_2_W(wBuf,(LPSTR)(LPCTSTR)sTime,100);
            XStatic_SetText(GETELE(IDS_SYSTIME),wBuf);
			XWnd_RedrawWnd(g_hWindow);
		}
		break;
    case TimerHandOn:
		{
			SendCmd(CTL_USB_ON);
		}	
    	break;
	case TimerRegCard:
		{
			char str[100];
			memset(str,0,sizeof(str));
			DWORD wCount=100;//��ȡ���ֽ���
			BOOL bReadStat;
			
			bReadStat=ReadFile(g_hCom,str,wCount,&wCount,NULL);
			if(!bReadStat)
				break;
			PurgeComm(g_hCom, PURGE_TXABORT|PURGE_RXABORT|PURGE_TXCLEAR|PURGE_RXCLEAR);//
			if (strlen(str)>0)
			{
				unsigned int buff[3],iRet;
				char szRet[20];
				CString stemp;
				buff[0]=(unsigned char)str[4];
				buff[1]=(unsigned char)str[5];
				buff[2]=(unsigned char)str[6];
				//iRet=buff[1]<<16+buff[2];
				
				sprintf(szRet, "%d", buff[0]);
				int n=strlen(szRet);
		
				for (int i=0;i<3-n;i++)
				{
					stemp+="0";
				}
				stemp+=szRet;

				memset(szRet,0,sizeof(szRet));
				iRet=buff[1]*256;
				iRet=iRet+buff[2];
				sprintf(szRet,"%d",iRet);
                n=strlen(szRet);
				for (int j=0;j<5-n;j++)
				{
                    stemp+="0";
				}
				stemp+=szRet;
				//wchar_t wBuf[256] = {0};
				char szT[512] = {'\0'};
				long FldCount = 0, ret = 0;;
				CString outParas[2];
				CWinAdo tAdo(sCONNSTR_MYSQL);			
				ret = tAdo.OpenAdo();
				if (ret != ADOOK)
				{
					return ;
				}			
				for (int k = 0; k < 2; k++)
				{
					outParas[k].Empty();
				}
				sprintf(szT, "select card_cardFactory,card_status from t_card where card_cardFactory = \'%s\'", stemp);
				ret = tAdo.execSelect(szT, &FldCount, outParas);
				if (ret == ADOERR)
				{
					//XMessageBox(g_hWindow, L"�û������������,��¼ʧ��!", L"����");
					//XWnd_RedrawWnd(XEle_GetHWindow(hEle));
					return ;
				}
				if (FldCount == 0 && outParas[0].IsEmpty()) //û�鵽���ż�¼
				{
					if(OperateRegCard("192.168.1.116",(LPSTR)(LPCTSTR)stemp,1))    //ע�Ῠ�Ų������¼�����ݿ�
					{
						sprintf(szT, "insert into t_card set card_cardFactory = \'%s\',card_status = \'%d\'", stemp,2);
				        ret = tAdo.execSelect(szT, &FldCount, outParas);
						XMessageBox(g_hWindow,L"�����ɹ���",L"��ʾ");
					}
				}
				else if (outParas[1]=="1")
				{
			        if(OperateRegCard("192.168.1.116",(LPSTR)(LPCTSTR)stemp,1))    //ע�Ῠ�Ų����¼�¼�����ݿ�
					{
                         sprintf(szT, "update t_card set card_status = \'%d\' where card_cardFactory = \'%s\'", 2,stemp);
				         ret = tAdo.execSelect(szT, &FldCount, outParas);
						 XMessageBox(g_hWindow,L"�����ɹ���",L"��ʾ");
					}
					
				}
				else if (outParas[1]=="2")
				{
			        if(OperateRegCard("192.168.1.116",(LPSTR)(LPCTSTR)stemp,2))     //ע�����Ų����¼�¼�����ݿ�
					{
						sprintf(szT, "update t_card set card_status = \'%d\' where card_cardFactory = \'%s\'", 1,stemp);
				        ret = tAdo.execSelect(szT, &FldCount, outParas);
						XMessageBox(g_hWindow,L"�����ɹ���",L"��ʾ");
					}
				}

			}
		}
		break;
    default:
		break;
    }
} 

void  CALLBACK  TimerMsgBox(HWND   hwnd,UINT   message,UINT   iTIMERID,DWORD   dwTIMER)
{ 
	switch(iTIMERID) {
	case 1:
		{
              KillTimer(XWnd_GetHWnd(g_hWindow),1);
			  XWnd_CloseWindow(g_hLogoutWindow);
		}	
		break;
	case 2:
		{
			KillTimer(XWnd_GetHWnd(g_hWindow),2);
			OnReturnMain(g_hTab28, g_hTab28);
		}
	}
	return ;
}

void     CALLBACK   TimerProHandOff(HWND   hwnd,UINT   message,UINT   iTIMERID,DWORD   dwTIMER)
{ 
 
	KillTimer(NULL,TimerDialEndChecker);
	SendCmd(CTL_HANDFREE_OFF);
	g_bPhoneIsBusy = false;
	//g_bPhoneIsIni = false;
	//SendCmd(ASK_PHONE_STATE);
	//	break;
          //XMessageBox(g_hWindow, L"������6000�������У�", L"��ʾ");
} 

///////////////////////////////////////////////////////////////////////

//����XML�����ļ���������
int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{  	
	/*HWND hWnd = ::FindWindow(NULL, "����");
	if (hWnd != NULL)
	{
		//CString str="sadfsadfsadfasdfsadf";
		CString* str  = new CString("dir");
		::PostMessage(hWnd, WM_SHOW_HEADPIC,NULL , (LPARAM)str);
	}*/
	//IDCARD_ALL stIDCard;
	//memset((char *)&stIDCard, 0x0, sizeof(IDCARD_ALL));
	//GetTcpCardScan(ID_TYPE_DriverLicense,stIDCard);
	//char bar[200];
	//GetTcpBarAndUpVisitorInfo(bar);

    //if(::GetPrivateProfileInt("TerminalSet", "bNoReg",0, CONFIG_FILE)!=1)
	{
		int ll = CheckIsReg();
		if(ll!=0) //�˴�ԭ�汾Ϊ ll!=0
		{
			//XMessageBox(L"�������������к�",L"��ʾ"); 
			MessageBox(NULL,"���ǵ����û��ܺ��ߣ���������Ա��ȡ�������к�","����",MB_ICONEXCLAMATION);
			HideTaskBar(false);
			ExecPro(".\\Reg.exe", true, NULL);
			return false;
		}
	}

	XInitXCGUI("K38H-Y0QY-7670-G567-B542-76C5-1BA0-8E9F"); //��ʼ�������
	//XInitXCGUI(); 
	//XInitXCGUI("K823-HF23-GH4N-L34U-C559-6DCE-0FB4-6D97");
	CoInitialize(NULL);
    ::CreateMutex(NULL,TRUE,"HLST_VISITOR");  
	if(GetLastError()==ERROR_ALREADY_EXISTS)
	{
		MessageBox(NULL,"���Ѿ����˸ó���","����",MB_ICONQUESTION);
		exit(0);
		return false;
	}

	g_bFaceCompare = true;
	//ȫ�ֱ�����ʼ��
	g_bVideoOpened = false;
	g_hCapWnd = NULL;
	g_bStopID2 = false;
	g_bStopBarCode = false;
	g_bStartClipScreen = false;
	g_nTableType = 0;
	g_nID2ReadType = ID2_NO;
	g_bPhoneIsBusy = false;
	g_bPhoneIsIni = false;
	g_bAcceptVisite = true;  //����Ҫ������Ƶ��֤
	g_bSendVidoe = false;    
	g_bPrintAfterAllow = false;
	g_bShowCap = true;
	g_bCalled = false;
	g_bFtpOpened = false;
	g_bJob = false;
	g_bVisitorType = true;

	g_bAutoCallOfficials = true;

	g_ThreadID2IC = NULL;
	g_nIDType = ID_TYPE_ID2;

	g_bSSO = false;

	char sz[256] = {'\0'};
	GetPrivateProfileString("TerminalSet", "DBLinkString", "", sz, sizeof(sz), CONFIG_FILE);
	sCONNSTR_MYSQL=sz;
	if(sCONNSTR_MYSQL.Find("MySQL ODBC")>0)  //My Sqlϵ��
    {
        g_nSqlConnectType = SQL_CONNECT_MYSQL;
	}
	else if (sCONNSTR_MYSQL.Find("SQL Native Client")>0) //SQL Server 2005
	{
		g_nSqlConnectType = SQL_CONNECT_SERVER2005;
	}
	else if (sCONNSTR_MYSQL.Find("SQL Server Native Client")>0)//SQL Server 2008
	{
        g_nSqlConnectType = SQL_CONNECT_SERVER2005;
	}
	else if (sCONNSTR_MYSQL.Find("SQL Server")>0)  //SQL Server 2000
	{
        g_nSqlConnectType = SQL_CONNECT_SERVER2005;
	}
	else if (sCONNSTR_MYSQL.Find("Microsoft Access Driver")>0)  //Office Access 2003
	{
        g_nSqlConnectType = SQL_CONNECT_SERVER2005;
	}
	else
	{
        MessageBox(NULL,"���ݿ�����������֧��","����",MB_ICONQUESTION);
        return false;
	}
	
	CWinAdo tAdo(sCONNSTR_MYSQL);
	if ( ADOOK!=tAdo.OpenAdo())
	{
		return false;
	}	
	HideTaskBar(true);

	//AutoCheckCompanyName(char *szCompany)
    //ʶ���������
	g_bExpireData=true;
	//CreateThread(NULL, 0, GetSQLThread, NULL, 0, NULL); //��Ӽ������ݿ����
	
	HXMLRES  hRes = XXmlRes_Load(L"skin\\mainFrame.res");	//������Դ
	if (NULL == hRes)
	{
		return 0;
	}

	HWINDOW hWindow = XC_LoadXML_Window(L"skin\\mainFrame.xml", hRes); //��������
	if (NULL == hWindow)
	{
		return 0;
	}
	//������ҳ
	XC_LoadXML_Page(hWindow, L"skin\\mainPage2.xml", hRes);
	XC_LoadXML_Page(hWindow, L"skin\\mainPage3.xml", hRes);
	XC_LoadXML_Page(hWindow, L"skin\\mainPage4.xml", hRes);
	XC_LoadXML_Page(hWindow, L"skin\\mainPage5.xml", hRes);
	XC_LoadXML_Page(hWindow, L"skin\\mainPage6.xml", hRes);
	XC_LoadXML_Page(hWindow, L"skin\\mainPage8.xml", hRes);
	XC_LoadXML_Page(hWindow, L"skin\\mainPage9.xml", hRes);
	XC_LoadXML_Page(hWindow, L"skin\\mainPage10.xml", hRes);
	XC_LoadXML_Page(hWindow, L"skin\\mainPage11.xml", hRes);
	XC_LoadXML_Page(hWindow, L"skin\\mainPage12.xml", hRes);

	if (false == XC_LoadXML_Style(hWindow, L"skin\\mainFrame.css", hRes))	//������ʽ�ļ�
	{
		return 0;
	}

	g_hWindow = hWindow;			//������
	//BuildQRGenerator();
	if(1 ==::GetPrivateProfileInt("TerminalSet", "HtmlReceiptTips",0, CONFIG_FILE))
	{
		if(!StartPrintTipsPB())
			XMessageBox(g_hWindow, L"�޷�����PB��ӡ���������������װ���⣡", L"����");
	}

	if(1 ==::GetPrivateProfileInt("TerminalSet", "bUptoServer",0, CONFIG_FILE))
	{
		if(!StartUptoServer())
			XMessageBox(g_hWindow, L"�޷������ϴ���������������װ���⣡", L"����");
	}

	if (GetPrivateProfileInt("TerminalSet", "bFtpOpen",0, CONFIG_FILE)==1)
	{
        FtpConnect();
	}

	if (GetPrivateProfileInt("TerminalSet", "bSSO",0, CONFIG_FILE)==1)
	{
		// SSOѡ��򿪣������˵��ֻ��ţ����ţ����ü��𣬷���ȥ��
        g_bSSO = true;

	}
	
	if (GetPrivateProfileInt("TerminalSet", "bVisitorType",1, CONFIG_FILE)!=1)
	{
		g_bVisitorType = false;
	}
	
	
	HMODULE hIns=GetModuleHandle(NULL);
	HICON hIcon=(HICON)LoadImage(hIns,"visitor.ico",IMAGE_ICON,16,16,LR_LOADFROMFILE);
	XWnd_SetIcon(hWindow,hIcon,false);

    memset((char *)&g_Printvisitor, 0, sizeof(VISITORINFO));
	memset((char *)&g_VisitorPic, 0, sizeof(PICPATH));

	g_hTab1 = XWnd_GetEle(hWindow, IDB_TAB1);
	g_hTab2 = XWnd_GetEle(hWindow, IDB_TAB2);
	g_hTab3 = XWnd_GetEle(hWindow, IDB_TAB3);
//	g_hTab4 = XWnd_GetEle(hWindow, IDB_TAB4);
	g_hTab5 = XWnd_GetEle(hWindow, IDB_TAB5);
	g_hTab6 = XWnd_GetEle(hWindow, IDB_TAB6);
	g_hTab7 = XWnd_GetEle(hWindow, IDB_TAB7);
	g_hTab8 = XWnd_GetEle(hWindow, IDB_TAB8);
	g_hTab9 = XWnd_GetEle(hWindow, IDB_TAB9);
	g_hTab10 = XWnd_GetEle(hWindow, IDB_TAB10);

	g_hTab21 = XWnd_GetEle(hWindow, IDB_TAB21);
	g_hTab22 = XWnd_GetEle(hWindow, IDB_TAB22);
	g_hTab23 = XWnd_GetEle(hWindow, IDB_TAB23);
	g_hTab24 = XWnd_GetEle(hWindow, IDB_TAB24);
	g_hTab25 = XWnd_GetEle(hWindow, IDB_TAB25);
	g_hTab26 = XWnd_GetEle(hWindow, IDB_TAB26);
	g_hTab27 = XWnd_GetEle(hWindow, IDB_TAB27);
	g_hTab28 = XWnd_GetEle(hWindow, IDB_TAB28);

	g_hTab32 = XWnd_GetEle(hWindow, IDB_TAB32);
	g_hTab33 = XWnd_GetEle(hWindow, IDB_TAB33);
	g_hTab34 = XWnd_GetEle(hWindow, IDB_TAB34);
//	g_hTab35 = XWnd_GetEle(hWindow, IDB_TAB35);
	g_hTab37 = XWnd_GetEle(hWindow, IDB_TAB37);
	g_hTab38 = XWnd_GetEle(hWindow, IDB_TAB38);
	g_hTab39 = XWnd_GetEle(hWindow, IDB_TAB39);

	g_hPage1 = XWnd_GetEle(hWindow, ID_PAGE1);
	g_hPage2 = XWnd_GetEle(hWindow, ID_PAGE2);
	g_hPage3 = XWnd_GetEle(hWindow, ID_PAGE3);    //������Ϣҳ��
	g_hPage4 = XWnd_GetEle(hWindow, ID_PAGE4);    //������ҳ��
	g_hPage5 = XWnd_GetEle(hWindow, ID_PAGE5);
	g_hPage6 = XWnd_GetEle(hWindow, ID_PAGE6);		//��ѯͳ��ҳ��
	g_hPage8 = XWnd_GetEle(hWindow, ID_PAGE8);
	g_hPage9 = XWnd_GetEle(hWindow, ID_PAGE9);
	g_hPage10 = XWnd_GetEle(hWindow, ID_PAGE10);
	g_hPage11 = XWnd_GetEle(hWindow, ID_PAGE11);
	g_hPage12 = XWnd_GetEle(hWindow, ID_PAGE12);

	g_bICReg = false;

    XEle_ShowEle(g_hPage1, false);
	XEle_ShowEle(g_hPage2, false);
	XEle_ShowEle(g_hPage3, false);
	XEle_ShowEle(g_hPage4, false);
	XEle_ShowEle(g_hPage5, false);
	XEle_ShowEle(g_hPage6, false);
	XEle_ShowEle(g_hPage8, false);
	XEle_ShowEle(g_hPage9, false);
	XEle_ShowEle(g_hPage11, false);
	XEle_ShowEle(g_hPage12, false);
	XEle_ShowEle(g_hPage10, true);

	HELE hE = GETELE(IDE_LOGIN_CZYMM);
	XEdit_EnablePassBox(hE, true);

	HFONT hFont = ::CreateFont(20, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, 
		GB2312_CHARSET, 
		OUT_DEFAULT_PRECIS, 
		CLIP_DEFAULT_PRECIS, 
		PROOF_QUALITY, 
		DEFAULT_PITCH & FF_ROMAN, 
		"����");		//΢���ź� 20
	//HFONTX hFont = XFont_Create2(L"����",20,true,false,false,false);
	XEle_SetFont(hE, hFont);

	XEle_SetFont(g_hPage1, hFont);
	XEle_SetFont(g_hPage2, hFont);
	hE = GETELE(IDE_LOGIN_CZYID);
	XEle_SetFont(hE, hFont);

	XEdit_EnablePassBox(GETELE(IDE_FTPPWD), true);
	//g_hCurSelTab = g_hTab1;

	//������ע�ᰴť��Ӧ
	XEle_RegisterEvent(g_hTab1, XE_BNCLICK, OnTakeVisitorPic);
	XEle_RegisterEvent(g_hTab2, XE_BNCLICK, OnScanVisitorCertification);	//֤��ɨ��
	XEle_RegisterEvent(g_hTab21, XE_BNCLICK, OnScanID2);			//ɨ�����֤

	//XEle_RegisterEvent(g_hTab22, XE_BNCLICK, OnScanID);				//ɨ��һ�����֤
    XEle_RegisterEvent(g_hTab22, XE_BNCLICK, OnScanLawyer);         //һ��֤��Ϊ��ʦ֤

	XEle_RegisterEvent(g_hTab23, XE_BNCLICK, OnScanPassPort);		//ɨ�軤��
	XEle_RegisterEvent(g_hTab24, XE_BNCLICK, OnScanToHKPassPort);	//ɨ��۰�ͨ��֤
	//XEle_RegisterEvent(g_hTab25, XE_BNCLICK, OnScanHKID);			//ɨ�����֤
	XEle_RegisterEvent(g_hTab25, XE_BNCLICK,OnScanBusinessCard);    //ɨ����Ƭ

	if(::GetPrivateProfileInt("TerminalSet", "bCarID",1, CONFIG_FILE)==1)
	{
		XBtn_SetText(g_hTab26, L"��ʻ֤");
		XEle_RegisterEvent(g_hTab26, XE_BNCLICK,OnScanDriverLicense);  //ɨ���ʻ֤		
	}
	else
	{
		XBtn_SetText(g_hTab26, L"̨��֤");
		XEle_RegisterEvent(g_hTab26, XE_BNCLICK, OnScanTaiWanPassPort);	//ɨ��̨��֤
	}

	XEle_RegisterEvent(g_hTab27, XE_BNCLICK, OnScanOther);			//ɨ������֤��
	XEle_RegisterEvent(g_hTab28, XE_BNCLICK, OnReturnMain);			//����

	XEle_RegisterEvent(g_hTab3, XE_BNCLICK, OnSendOfficialRequest);
	XEle_RegisterEvent(g_hTab5, XE_BNCLICK, OnChaxunTongji);
	for (int i=5071;i<5075;i++)
	{
		XEle_RegisterEvent(XWnd_GetEle(g_hWindow, i), XE_SETFOCUS, OnOtherSetFocus);
	}

    XEle_RegisterEvent(XWnd_GetEle(g_hWindow, IDB_TONGJICHAXUNRETURN), XE_BNCLICK, OnTongjiChaxunReturn);		//ͳ�Ʋ�ѯҳ��ķ��ذ�ť
	XEle_RegisterEvent(XWnd_GetEle(g_hWindow, IDB_TONGJI), XE_BNCLICK, OnTongji);		//ͳ�Ʋ�ѯҳ���ͳ�ư�ť
	XEle_RegisterEvent(XWnd_GetEle(g_hWindow, IDB_CHAXUN), XE_BNCLICK, OnChaxun);		//ͳ�Ʋ�ѯҳ��Ĳ�ѯ��ť

	XEle_RegisterEvent(g_hTab8, XE_BNCLICK, OnExitApp);

	XEle_RegisterEvent(g_hTab10, XE_BNCLICK, OnSysSet);
	XEle_RegisterEvent(XWnd_GetEle(g_hWindow, IDL_LISTOFFICIAL), XE_LIST_SELECT, OnOfficialList);
	for(int j=5030;j<5038;j++)
	{
        XEle_RegisterEvent(XWnd_GetEle(g_hWindow, j), XE_SETFOCUS, OnOfficial1SetFocus);
	}

	XEle_RegisterEvent(XWnd_GetEle(g_hWindow, 5101), XE_SETFOCUS, OnOfficial1SetFocus); //IP

	XEle_RegisterEvent(XWnd_GetEle(g_hWindow, IDB_ADDOFFICIAL), XE_BNCLICK, OnAddOfficialInfo);			//ϵͳ�����е�������Ա��Ϣ
	XEle_RegisterEvent(XWnd_GetEle(g_hWindow, IDB_QUERYALLOFFICIAL), XE_BNCLICK, OnQueryEmployerForList);	//ϵͳ�����еĲ�ѯ��Ա��Ϣ
	XEle_RegisterEvent(XWnd_GetEle(g_hWindow, IDB_DELETEOFFICIAL), XE_BNCLICK, OnDeleteOfficial);			//ϵͳ�����е�ɾ����Ա��Ϣ
	XEle_RegisterEvent(XWnd_GetEle(g_hWindow, IDB_IMPORT_OFFICIAL), XE_BNCLICK, OnImportOfficial);
	XEle_RegisterEvent(XWnd_GetEle(g_hWindow, IDB_EXPORT_OFFICIAL), XE_BNCLICK, OnExportOfficial);
	XEle_RegisterEvent(XWnd_GetEle(g_hWindow, IDB_SAVE_OFFICIAL  ), XE_BNCLICK, OnSaveOfficial);
	XEle_RegisterEvent(XWnd_GetEle(g_hWindow, IDB_ADDBINDERIC  ), XE_BNCLICK, OnICUnReg);

	XEle_RegisterEvent(g_hTab32, XE_BNCLICK, OnOfficialSet);		//������Ա��Ϣҳ��
	XEle_RegisterEvent(g_hTab33, XE_BNCLICK, OnBlackSet);			//���Ӻ�������Ϣҳ��
	for (int k=5038;k<5047;k++)
	{
		XEle_RegisterEvent(XWnd_GetEle(g_hWindow, k), XE_SETFOCUS, OnBlackNameSetFocus);
	}
    XEle_RegisterEvent(XWnd_GetEle(g_hWindow, IDB_ADDBLACK), XE_BNCLICK, OnAddBlack);			//ϵͳ�����е����Ӻ�����
	XEle_RegisterEvent(XWnd_GetEle(g_hWindow, IDB_QUERYBLACK), XE_BNCLICK, OnQueryBlack);		//ϵͳ�����еĲ�ѯ������
	XEle_RegisterEvent(XWnd_GetEle(g_hWindow, IDB_DELETEBLACK), XE_BNCLICK, OnDeleteBlack);	//ϵͳ�����е�ɾ��������
	XEle_RegisterEvent(XWnd_GetEle(g_hWindow, IDB_IMPORT_BLACKNAME), XE_BNCLICK, OnImportBlackName);
	XEle_RegisterEvent(XWnd_GetEle(g_hWindow, IDB_EXPORT_BLACKNAME), XE_BNCLICK, OnExportBlackName);
    XEle_RegisterEvent(XWnd_GetEle(g_hWindow, IDL_LISTBLACK), XE_LIST_SELECT, OnBlackNameList);	

	XEle_RegisterEvent(g_hTab34, XE_BNCLICK, OnGoodsSet);			//������Ʒ��Ϣҳ��
    XEle_RegisterEvent(XWnd_GetEle(g_hWindow, IDB_ADDGOODS), XE_BNCLICK, OnAddGoods);			//ϵͳ�����е�������Ʒ
	XEle_RegisterEvent(XWnd_GetEle(g_hWindow, IDB_QUERYGOODS), XE_BNCLICK, OnQueryGoods);		//ϵͳ�����еĲ�ѯ��Ʒ
	XEle_RegisterEvent(XWnd_GetEle(g_hWindow, IDB_DELETEGOODS), XE_BNCLICK, OnDeleteGoods);	//ϵͳ�����е�ɾ����Ʒ
	XEle_RegisterEvent(XWnd_GetEle(g_hWindow, IDB_IMPORT_GOODS), XE_BNCLICK, OnImportGoods);
	XEle_RegisterEvent(XWnd_GetEle(g_hWindow, IDB_EXPORT_GOODS), XE_BNCLICK, OnExportGoods);
	XEle_RegisterEvent(XWnd_GetEle(g_hWindow, IDB_SAVE_GOODS), XE_BNCLICK, OnSaveGoods);
	
	XEle_RegisterEvent(XWnd_GetEle(g_hWindow, IDB_ADDREASON), XE_BNCLICK, OnAddReason);			//ϵͳ�����е���������
	XEle_RegisterEvent(XWnd_GetEle(g_hWindow, IDB_QUERYREASON), XE_BNCLICK, OnQueryReason);		//ϵͳ�����еĲ�ѯ����
	XEle_RegisterEvent(XWnd_GetEle(g_hWindow, IDB_DELETEREASON), XE_BNCLICK, OnDeleteReason);		//ϵͳ�����е�ɾ������
	XEle_RegisterEvent(XWnd_GetEle(g_hWindow, IDB_IMPORT_REASON), XE_BNCLICK, OnImportReason);
	XEle_RegisterEvent(XWnd_GetEle(g_hWindow, IDB_EXPORT_REASON), XE_BNCLICK, OnExportReason);
	XEle_RegisterEvent(XWnd_GetEle(g_hWindow, IDB_SAVE_REASON), XE_BNCLICK, OnSaveReason);
	
	XEle_RegisterEvent(XWnd_GetEle(g_hWindow, IDB_ADDSTATION), XE_BNCLICK, OnAddStation);			//ϵͳ�����е������Ÿ�
	XEle_RegisterEvent(XWnd_GetEle(g_hWindow, IDB_QUERYSTATION), XE_BNCLICK, OnQueryStation);		//ϵͳ�����еĲ�ѯ�Ÿ�
	XEle_RegisterEvent(XWnd_GetEle(g_hWindow, IDB_DELETESTATION), XE_BNCLICK, OnDeleteStation);	//ϵͳ�����е�ɾ���Ÿ�
	XEle_RegisterEvent(XWnd_GetEle(g_hWindow, IDB_IMPORT_STATION), XE_BNCLICK, OnImportStation);
	XEle_RegisterEvent(XWnd_GetEle(g_hWindow, IDB_EXPORT_STATION), XE_BNCLICK, OnExportStation);
	XEle_RegisterEvent(XWnd_GetEle(g_hWindow, IDB_SAVE_STATION), XE_BNCLICK, OnSaveStation);

	XEle_RegisterEvent(XWnd_GetEle(g_hWindow, IDB_ADD_ADDCZY), XE_BNCLICK, OnAddCzy);				//ϵͳ�����е����Ӳ���Ա
	XEle_RegisterEvent(XWnd_GetEle(g_hWindow, IDB_ADD_QUERYCZY), XE_BNCLICK, OnQueryCzy);				//ϵͳ�����еĲ�ѯ����Ա
	XEle_RegisterEvent(XWnd_GetEle(g_hWindow, IDB_ADD_DELETECZY), XE_BNCLICK, OnDeleteCzy);			//ϵͳ�����е�ɾ������Ա
	XEle_RegisterEvent(XWnd_GetEle(g_hWindow, IDB_IMPORT_CZY), XE_BNCLICK, OnImportCZY);
	XEle_RegisterEvent(XWnd_GetEle(g_hWindow, IDB_EXPORT_CZY), XE_BNCLICK, OnExportCZY);
	XEle_RegisterEvent(XWnd_GetEle(g_hWindow, IDB_SAVE_CZY), XE_BNCLICK, OnSaveCZY);
	
    XEle_RegisterEvent(XWnd_GetEle(g_hWindow, IDL_LISTGOODSORREASONORSTATION), XE_LIST_SELECT, OnGoodsReasonStationUser);
	for (int l=5047;l<5050;l++)
	{
		XEle_RegisterEvent(XWnd_GetEle(g_hWindow, l), XE_SETFOCUS, OnOtherSetFocus);
	}

	for (int m=5064;m<5067;m++)
	{
		if(m==5066)
		{ 
             XEle_RegisterEvent(XWnd_GetEle(g_hWindow, m), XE_SETFOCUS, OnClearPWD);
		}
		else
		     XEle_RegisterEvent(XWnd_GetEle(g_hWindow, m), XE_SETFOCUS, OnOtherSetFocus);
	}
	XEle_RegisterEvent(XWnd_GetEle(g_hWindow, IDE_ADD_CZYRIGHT), XE_SETFOCUS, OnHideKeyBdSetFocus);

	XEle_RegisterEvent(g_hTab37, XE_BNCLICK, OnSysConfig);			//ϵͳ��������ҳ��
    XEle_RegisterEvent(XWnd_GetEle(g_hWindow, IDB_CFG_SAVE), XE_BNCLICK, OnConfigSave);			//�������ñ���
	XEle_RegisterEvent(XWnd_GetEle(g_hWindow, IDB_CHK_SAVE), XE_BNCLICK, OnSaveMustInput);		//���������ñ���
	XEle_RegisterEvent(XWnd_GetEle(g_hWindow, IDB_OTH_SAVE), XE_BNCLICK, OnSaveOther);
	for (int a=5050;a<5058;a++)
	{
		XEle_RegisterEvent(XWnd_GetEle(g_hWindow, a), XE_SETFOCUS, OnOtherSetFocus);
	}
	for (int b=5716;b<5720;b++)
	{
		XEle_RegisterEvent(XWnd_GetEle(g_hWindow, b), XE_SETFOCUS, OnOtherSetFocus);
	}

	XEle_RegisterEvent(g_hTab38, XE_BNCLICK, OnBaseInfo);			//������Ϣҳ��
	XEle_RegisterEvent(g_hTab39, XE_BNCLICK, OnReturnSet);			//ϵͳ����(����ά��)ҳ���ϵķ���

	XEle_RegisterEvent(XWnd_GetEle(g_hWindow, IDB_ADDOFFICE), XE_BNCLICK, OnAddOffice);			//ϵͳ�����е����ӿ���
	XEle_RegisterEvent(XWnd_GetEle(g_hWindow, IDB_QUERYOFFICE), XE_BNCLICK, OnQueryOffice);		//ϵͳ�����еĲ�ѯ����
	XEle_RegisterEvent(XWnd_GetEle(g_hWindow, IDB_DELETEOFFICE), XE_BNCLICK, OnDeleteOffice);		//ϵͳ�����е�ɾ������
	XEle_RegisterEvent(XWnd_GetEle(g_hWindow, IDB_IMPORT_OFFICE), XE_BNCLICK, OnImportOffice);
	XEle_RegisterEvent(XWnd_GetEle(g_hWindow, IDB_EXPORT_OFFICE), XE_BNCLICK, OnExportOffice);
	XEle_RegisterEvent(XWnd_GetEle(g_hWindow, IDB_SAVE_OFFICE), XE_BNCLICK, OnSaveOffice);

	XEle_RegisterEvent(XWnd_GetEle(g_hWindow, IDB_SAVE_LEVEL), XE_BNCLICK, OnSaveLevel);
	XEle_RegisterEvent(XWnd_GetEle(g_hWindow, IDB_ADDLEVEL), XE_BNCLICK, OnAddLevel);				//ϵͳ�����е����Ӽ���
	XEle_RegisterEvent(XWnd_GetEle(g_hWindow, IDB_QUERYLEVEL), XE_BNCLICK, OnQueryLevel);			//ϵͳ�����еĲ�ѯ����
	XEle_RegisterEvent(XWnd_GetEle(g_hWindow, IDB_DELETELEVEL), XE_BNCLICK, OnDeleteLevel);		//ϵͳ�����е�ɾ������
	XEle_RegisterEvent(XWnd_GetEle(g_hWindow, IDL_LISTOFFICEORLEVEL), XE_LIST_SELECT, OnOfficeLevel);
	for (int c=5060;c<5064;c++)
	{
		XEle_RegisterEvent(XWnd_GetEle(g_hWindow, c), XE_SETFOCUS, OnOtherSetFocus);
	}
	XEle_RegisterEvent(XWnd_GetEle(g_hWindow, IDE_ADD_LEVELNAME), XE_SETFOCUS, OnOtherSetFocus);

//  XEle_RegisterEvent(g_hTab1, XE_BNCLICK, OnAutoLogOut);			//����ǩ��
//	XEle_RegisterEvent(g_hTab4, XE_BNCLICK, OnSearchPage);      //��д�� 
//	XEle_RegisterEvent(g_hTab35, XE_BNCLICK, OnCallHandInput);		//ϵͳ��������ҳ�����д
	XEle_RegisterEvent(XWnd_GetEle(hWindow, IDB_NEWVISITOR), XE_BNCLICK, OnNewVisitor);		//������
//  XEle_RegisterEvent(XWnd_GetEle(hWindow, IDB_TAKEPIC), XE_BNCLICK, OnTakeVisitorPic);	//����

	///////�����水ť����Ӧ
	XEle_RegisterEvent(g_hTab6, XE_BNCLICK, OnCallPhone);		//����칫�绰
	XEle_RegisterEvent(g_hTab7, XE_BNCLICK, OnCallMobile);		//�����ƶ��绰
	XWnd_RegisterMessage(g_hWindow, WM_USB_PNP, OnUsbPhoneEvent);
	//XEle_RegisterEvent(XWnd_GetEle(hWindow, IDB_HANGUP), XE_BNCLICK, OnHangUp);		//�Ҷϵ绰
	XEle_ShowEle(XWnd_GetEle(hWindow, IDB_HANGUP), false);
	XEle_RegisterEvent(XWnd_GetEle(hWindow, IDB_INFORMATION), XE_BNCLICK, OnVisitInformationList);	//������������ϸ��Ϣ
	XEle_RegisterEvent(XWnd_GetEle(hWindow, IDB_QUERYOFFICIAL), XE_BNCLICK, OnQueryEmployer);		//ƥ�䱻������Ϣ
	XEle_RegisterEvent(XWnd_GetEle(hWindow, IDL_VISTORLIST), XE_LIST_SELECT, OnVisitorList);		//ѡ���������б��е���

	XEle_RegisterMessage(XWnd_GetEle(hWindow, IDP_CAMERA), XM_LBUTTONDOWN, OnCameraLButtonDown);		//��ʼ��ȡ��Ļ
	XEle_RegisterMessage(XWnd_GetEle(hWindow, IDP_CAMERA), XM_LBUTTONUP, OnCameraLButtonUp);			//������ȡ��Ļ
	XEle_RegisterMessage(XWnd_GetEle(hWindow, IDP_CAMERA), XM_MOUSEMOVE, OnCameraMouseMove);			//������ȡ��Ļ

	XWnd_RegisterMessage(g_hWindow, WM_VISITORLOGOUT, OnVisitorLogOut);
	XWnd_RegisterMessage(g_hWindow, WM_OutResponse, OnOutResponse);
	XWnd_RegisterMessage(g_hWindow, WM_READID2_OVER, OnReadID2Over);		//�Ķ�����֤��Ϣ
	XWnd_RegisterMessage(g_hWindow, WM_COMPARE_FACE, OnCompareFace);      //����ʶ��Ա�
	
	XWnd_RegisterMessage(g_hWindow, WM_READICA_OVER, OnReadICAOver);    //�Ķ�A��IC�����к�

	XWnd_RegisterMessage(g_hWindow, WM_LBUTTONDOWN, OnCapture);

	XWnd_RegisterMessage(g_hWindow, WM_OFFICIALNAME_SELECTED, OnOfficialNameSelected);

	for (int d = 5003; d <= 5012; d++)		//��������Ϣ�������ԴID
	{
		//XEle_RegisterEvent(XWnd_GetEle(hWindow, d), XE_SETFOCUS, OnVisitorSetFocus);
		XEle_RegisterMessage(XWnd_GetEle(hWindow, d), XM_LBUTTONDOWN, OnVisitorSetFocus2);
	}
	for (int e = 5020; e <= 5023; e++)		//��������Ϣ�������ԴID
	{
		//XEle_RegisterEvent(XWnd_GetEle(hWindow, e), XE_SETFOCUS, OnVisitorSetFocus);
		XEle_RegisterMessage(XWnd_GetEle(hWindow, e), XM_LBUTTONDOWN, OnVisitorSetFocus2);
	}

	for (int f = 5013; f <= 5019; f++)		//���÷�����Ϣ�������ԴID
	{
		XEle_RegisterMessage(XWnd_GetEle(hWindow, f), XM_LBUTTONDOWN, OnOfficialSetFocus);
	}
	XEle_RegisterMessage(XWnd_GetEle(hWindow,IDC_MASTERSSO), XM_LBUTTONDOWN, OnOfficialSetFocus);
	//XEle_RegisterEvent(XWnd_GetEle(hWindow, i), XE_KILLFOCUS, OnOfficialKillFocus);

	XEle_RegisterEvent(XWnd_GetEle(hWindow, IDC_VISITORSEX), XE_SETFOCUS, OnHideKeyBdSetFocus);

	XEle_RegisterEvent(XWnd_GetEle(hWindow, IDC_VISITORIDTYPE), XE_SETFOCUS, OnShowIDType);
	if(g_bVisitorType)
	{
		XEle_RegisterEvent(XWnd_GetEle(hWindow, IDE_VISITORTYPE), XE_SETFOCUS, OnShowVisitorType);
	}
	else
	{
		XEdit_EnableNumber(XWnd_GetEle(g_hWindow, IDE_VISITORTOTAL),true);
	}
    XEle_RegisterEvent(XWnd_GetEle(hWindow, IDC_VISITORFOLK), XE_SETFOCUS, OnShowFolk);

	XEle_RegisterEvent(XWnd_GetEle(hWindow, IDC_MASTERSEX), XE_SETFOCUS, OnHideKeyBdSetFocus);
	XEle_RegisterEvent(XWnd_GetEle(hWindow, IDE_LOGIN_CZYID), XE_SETFOCUS, OnHideKeyBdSetFocus);
	//XEle_RegisterEvent(XWnd_GetEle(hWindow, IDE_ROOMNUM), XE_SETFOCUS, OnOfficialSetFocus);
	
	///////��¼���水ť����Ӧ
	XEle_RegisterEvent(XWnd_GetEle(hWindow, IDB_LOGIN_OK), XE_BNCLICK, OnLoginOK);		//��¼���ڵ�ȷ����ť
	XEle_RegisterEvent(XWnd_GetEle(hWindow, IDB_LOGIN_NO), XE_BNCLICK, OnLoginNo);		//��¼���ڵ�ȡ����ť
	XEle_RegisterEvent(XWnd_GetEle(hWindow, IDE_LOGIN_CZYMM), XE_SETFOCUS,OnShowPwdKeyBd); //��ȡ����	

	XEle_RegisterEvent(XWnd_GetEle(g_hWindow,IDB_EXPORT_VISITOR),XE_BNCLICK,OnExportVisitor);
	XEle_RegisterEvent(XWnd_GetEle(hWindow, IDL_LISTCHAXUNTONGJI), XE_LIST_SELECT, OnChaxunTongjiList);		//��ѯͳ��ҳ����ѡ������Ϣ�б��е�һ��
	XEle_RegisterEvent(XWnd_GetEle(hWindow, ID_IDType_Comb), XE_COMBOBOX_SELECT, OnIDTypeCombSelect);
	//XEle_ShowEle(XWnd_GetEle(g_hWindow,IDB_EXPORT_VISITOR),false);
	///////////////////////////////

    //if(::GetPrivateProfileInt("TerminalSet", "bNoReg",0, CONFIG_FILE)!=1)
	{
		int ll = CheckIsReg();
		if(ll != 0)//ll!=0
		{
			//XMessageBox(L"�������������к�",L"��ʾ"); 
			MessageBox(NULL,"���ǵ����û��ܺ��ߣ���������Ա��ȡ�������к�","����",MB_ICONEXCLAMATION);
			HideTaskBar(false);
			ExecPro(".\\Reg.exe", true, NULL);
			return false;
		}
	}


	XWnd_SetBorderStrokeOuterColor(hWindow, RGB(102, 0, 0), RGB(0, 0, 0));
	XWnd_SetCaptionHeight(hWindow,0);
	XWnd_MaximizeWnd(hWindow,true);
	
	XWnd_ShowWindow(hWindow, SW_SHOW);
	
	GetPrivateProfileString("TerminalSet", "StationName","",g_szStation, sizeof(g_szStation), CONFIG_FILE);	
	CreateWorkFolder();

	if (::GetPrivateProfileInt("TerminalSet", "bVisitorReadOnly",0, CONFIG_FILE)==1)  //���õǼ�֤������ֻ��
	{
		SetVisitorReadOnly(true);
	}
	else
	{
	    SetSexInfo(IDC_VISITORSEX);	//�����Ա���Ϣ
		SetID2InfoFont();
	    SetVisitorReadOnly(false);
	}

	if (::GetPrivateProfileInt("TerminalSet", "bSaveOfficial",0, CONFIG_FILE)==1)  //�����±�����
	{
		SetOfficialReadOnly(false);  //�ɱ༭
		SetSexInfo(IDC_MASTERSEX);	 //�����Ա���Ϣ
	}
	else
	{
		SetOfficialReadOnly(true);
	}

	if (::GetPrivateProfileInt("TerminalSet", "bAutoCallOfficials",0, CONFIG_FILE)==1)
	{
		g_bAutoCallOfficials = true;
	}
	else
	{
		g_bAutoCallOfficials = false;
	}


	if(g_bSSO)
	{
		//�칫�绰
		RECT rc;
		XEle_GetRect(XWnd_GetEle(hWindow,IDE_OFFICEPHONE),&rc); 
		rc.right = 360;
		XEle_SetRect(XWnd_GetEle(hWindow,IDE_OFFICEPHONE),&rc);
		//�ƶ��绰
		XEle_ShowEle(XWnd_GetEle(hWindow,IDS_MOBILE),false);
		XEle_ShowEle(XWnd_GetEle(hWindow,IDE_MASTERMOBILE),false);

		XEle_ShowEle(g_hTab7,false); //�ƶ��绰��ť

		//���ü���
		XEle_ShowEle(XWnd_GetEle(hWindow,IDS_ZHIWU),false);
		XEle_ShowEle(XWnd_GetEle(hWindow,IDC_MASTERZHIWU),false);

		//����
		XEle_ShowEle(XWnd_GetEle(hWindow,IDS_ROOMNUM),false);
		XEle_ShowEle(XWnd_GetEle(hWindow,IDE_ROOMNUM),false);

		//�������б�
		XEle_ShowEle(XWnd_GetEle(hWindow,IDS_ADD_ZHIWU),false);
		XEle_ShowEle(XWnd_GetEle(hWindow,IDC_ADD_OFFICIALZHIWU),false);

		XEle_ShowEle(XWnd_GetEle(hWindow,IDS_ADD_ROOMNUM),false);
		XEle_ShowEle(XWnd_GetEle(hWindow,IDE_ADD_ROOMNUM),false);
		
		XEle_ShowEle(XWnd_GetEle(hWindow,IDB_ADDBINDERIC),false);

		XEle_ShowEle(XWnd_GetEle(hWindow,ID_IDType_Comb),false);
		XEle_ShowEle(XWnd_GetEle(hWindow,IDS_IDType_Comb),false);

	}
	else
	{
		
		XEle_ShowEle(XWnd_GetEle(hWindow,IDS_SSO),false);
		XEle_ShowEle(XWnd_GetEle(hWindow,IDC_MASTERSSO),false);

		XEle_ShowEle(XWnd_GetEle(hWindow,IDB_ADDBINDERIC),false);
		XEle_ShowEle(XWnd_GetEle(hWindow,IDS_ADD_SSO),false);
		XEle_ShowEle(XWnd_GetEle(hWindow,IDE_ADD_SSO),false);

		XEle_ShowEle(XWnd_GetEle(hWindow,IDS_VisitorType_Comb),false);
		XEle_ShowEle(XWnd_GetEle(hWindow,ID_VisitorType_Comb),false);
	}

	SetIDtypeComb(ID_IDType_Comb);
	SetVisitortypeComb(ID_VisitorType_Comb);
	HELE hEle = NULL;
	HIMAGE hImg = NULL;
	hEle = XWnd_GetEle(hWindow, IDP_ID2PIC);
	hImg = XImage_LoadFile(L".\\skin\\image\\id2.bmp", true);
    XPic_SetImage(hEle, hImg);
    ::WritePrivateProfileString("TerminalSet", "DataInputPage", "3", CONFIG_FILE);

	hE = XWnd_GetEle(hWindow, IDP_ID2PIC);
	XEle_ShowEle(hE, true);
	hE = XWnd_GetEle(hWindow, IDG_ID2);	//����֤ͼ���
	XEle_ShowEle(hE, true);
	hE = XWnd_GetEle(hWindow, IDG_PASSPORT);	//�������
	XEle_ShowEle(hE, false);
		
	::DeleteFile(USER_HEAD_PIC);            //ɾ���ϴ�ͷ��
	::DeleteFile(USER_ID_PIC); 
	::DeleteFile(USER_VIDEO_PIC);
	::DeleteFile(USER_BARCODE_PIC);

	if(1 ==::GetPrivateProfileInt("TerminalSet", "ZT",0, CONFIG_FILE))
	{
		if(!StartZTScanner())
			XMessageBox(g_hWindow, L"�޷�����֤ͨɨ���ǳ����������װ���⣡", L"����");
		else
		{
			g_bScanerISOK = true;
			XEle_ShowEle(g_hTab21, false);
		}
	}
	else if(2 ==::GetPrivateProfileInt("TerminalSet", "ZT",0, CONFIG_FILE))
	{
		if(!StartYunMaiOCR())
			XMessageBox(g_hWindow, L"�޷���������ʶ������������װ���⣡", L"����");
		else
		{
			long ret = LoadIdcardLibrary();
			if (ret!=0)
			{
				XMessageBox(g_hWindow, L"��ʼ��ɨ���Ǵ���", L"����");
				g_bScanerISOK = false;
			}
			else
			{
				g_bScanerISOK = true;
			}
		}
	}
	else if (::GetPrivateProfileInt("TerminalSet", "FS533",0, CONFIG_FILE)==1) 
	{
		g_bScanerISOK = CallFS533Bin("test");
		if(!g_bScanerISOK)
			XMessageBox(g_hWindow, L"��ʼ��FS533ɨ���Ǵ���", L"����");
	}
	else
	{
		long ret = LoadIdcardLibrary();
		if (ret!=0)
		{
			XMessageBox(g_hWindow, L"��ʼ��ɨ���Ǵ���", L"����");
			g_bScanerISOK = false;
		}
		else
		{
			g_bScanerISOK = true;
		}
	}

	//����
	//::WritePrivateProfileString("Info", "bStopRead", "0", ".\\FaceCompare\\faceCompare.ini");
	g_bStopCompare = true;
	StartFaceScaner();
	//StartFaceCompare();


    InitWJUsb(0, GETHWND);
	SetLoginUser();

    InitalPrintTipsCfg();

	if (g_PrintTipsCfg.ePrintType == PRINT_NOFRAME_TYPE)
	{
		//XEle_RegisterEvent(g_hTab9, XE_BNCLICK, OnSavePrint2);
		//XWnd_RegisterMessage(g_hWindow, WM_SAVEANDPRINT, OnSavePrintBySoftKeyBD2);		//�Զ��屣���ӡ����Ϣ
	}
	else
	{
		XEle_RegisterEvent(g_hTab9, XE_BNCLICK, OnSavePrint);
		XWnd_RegisterMessage(g_hWindow, WM_SAVEANDPRINT, OnSavePrintBySoftKeyBD);		//�Զ��屣���ӡ����Ϣ
	}

	if(::GetPrivateProfileInt("TerminalSet", "bJob",0, CONFIG_FILE)==1)
	{
		
		//XWnd_GetEle(hWindow, IDG_PASSPORT);
		XStatic_SetText(XWnd_GetEle(hWindow, IDS_FZJG),L"�ÿ�ְ��");
		g_bJob = true;

	}	
	else
	{
		g_bJob = false;
	}

    InitData();
    InitalVisitorExp();         //���������˱�����Ŀ
	ListTodayVistorsInfo();		//�г��������������ϸ��Ϣ
	InitIDTypeAndFolk();		//��ʼ������֤��������Ϣ���Ա���Ϣ
	char szT[1024] = {'\0'};
	VisitorTotalInfo(szT);

    SetTimer(XWnd_GetHWnd(g_hWindow),TimerHandOn,4000,TimerPro); 
	SetTimer(XWnd_GetHWnd(g_hWindow),TimerReFresh,1000,TimerPro);
	//SetTimer(XWnd_GetHWnd(g_hWindow),TimerCapture, 1000, TimerPro); //����ͷ����
	g_HWDThreadID2ID = CreateThread(NULL, 0, ReadID2Thread2, NULL, 0, NULL);		    //����֤�Ķ���פ�߳�
	/*int iPort = GetPrivateProfileInt("TerminalSet", "ID2Port",1001, CONFIG_FILE);  //����֤�˿ں�
    if(!OpenID2Reader(1001,(long)XWnd_GetHWnd(g_hWindow)))
       MessageBox(NULL,"����֤�Ķ����˿ڴ�ʧ��","����",MB_OK);*/
	
    //   g_ThreadReadBarcode = CreateThread(NULL, 0, ReadBarCodeThread2, NULL, 0, NULL);
//	else
//	   g_ThreadReadBarcode = CreateThread(NULL, 0, ReadBarCodeThread, NULL, 0, NULL);	//�����Ķ���פ�߳�

	if(::GetPrivateProfileInt("TerminalSet", "bManulLaser",0, CONFIG_FILE)==1)
       g_ThreadReadBarcode = CreateThread(NULL, 0, ReadBarCodeThread2, NULL, 0, NULL);
	else
	   g_ThreadReadBarcode = CreateThread(NULL, 0, ReadBarCodeThread, NULL, 0, NULL);	//�����Ķ���פ�߳�


	CreateThread(NULL, 0, GetDataThread, NULL, 0, NULL);		//Ϊ���̳����ȡ���ݵĳ�פ�߳�
	if(::GetPrivateProfileInt("TerminalSet", "bFaceCompare",0, CONFIG_FILE)==1)
		CreateThread(NULL, 0, FaceCompareThread, NULL, 0, NULL); 

	StartKeybd();
	HideSoftKeybd();
	XWnd_SetWindowPos(hWindow, HWND_BOTTOM, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	XXmlRes_Destroy(hRes);
	XRunXCGUI();
	return 0;
}

BOOL CALLBACK OnSelectCombox(HELE hEle,HELE hEventEle)
{
    XEdit_SetReadOnly(hEle,false);
    //XComboBox_SetSelectItem(hEle, index);
	/*wchar_t wBuf[256] = {0};
	memset((char *)wBuf, 0, sizeof(wBuf));
	//hE = XWnd_GetEle(XEle_GetHWindow(hEle), IDE_CX_VISITORNAME);
	XEdit_GetText(hEle, wBuf, 128);*/

	XEdit_SetReadOnly(hEle,true);
	return true;
}



BOOL OperateRegCard(char* szIp,char* szCard,int nOperateType)
{
	/*g_hDllJBC=LoadLibrary(_T("JBC7200.dll"));
	if (g_hDllJBC==NULL)
	{
		FreeLibrary(g_hDllJBC);
		return false;
	}
	typedef int (PASCAL *pFun1)();
    pFun1 pFun;
	pFun=(pFun1)GetProcAddress(g_hDllJBC, "JBC7K2_OpenSocket");   
	int ret=pFun();
	if (ret!=0)
	{
		return false;
	}*/

	J7100REGISTRY UserReg;
	//strcpy(UserReg.card1,"02260697");   //����
	strcpy(UserReg.card1,szCard);   //����
	strcpy(UserReg.card2,"");   //����
	for (int i=0;i<23;i++)            //�Ž�ͨ��Ȩ��
	{
		UserReg.acce[i]='1';
	}
	strcpy(UserReg.psw,"");           //����
	UserReg.security=10;
	UserReg.team=0;
	/*UserReg.year =2012;               //��Ч��¼��
	UserReg.month= 11;
	UserReg.day= 23;*/
	
	typedef int (PASCAL *pJBC7100_Single)(J7100REGISTRY *,char *,int,int);
	pJBC7100_Single JBC7100_Single;
	JBC7100_Single=(pJBC7100_Single)GetProcAddress(g_hDllJBC, "JBC7100_Single");
	/*if (nOperateType==2)
	{
        JBC7100_Single(&UserReg,"192.168.1.116",5101,1);
	}*/
	int ret=JBC7100_Single(&UserReg,"192.168.1.116",5101,nOperateType);
    //JBC7100_Single(&UserReg,"192.168.1.116",5101,1);
    //ret=JBC7100_Single(&UserReg,"192.168.1.116",5101,2);
    /*typedef int (PASCAL *pJBC7K2_CloseSocket)();
    pJBC7K2_CloseSocket JBC7K2_CloseSocket;
	JBC7K2_CloseSocket=(pJBC7K2_CloseSocket)GetProcAddress(g_hDllJBC, "JBC7K2_CloseSocket");   
	JBC7K2_CloseSocket();*/
	if (ret!=0)
	{
		if (nOperateType==2)
		{
             JBC7100_Single(&UserReg,"192.168.1.116",5101,1);
		}
		ret=JBC7100_Single(&UserReg,"192.168.1.116",5101,nOperateType);
		if (ret!=0)
		{
			if (ret==-2)
			{
				XMessageBox(g_hWindow,L"ע��ʧ��[-2]",L"��ʾ");
			}
			else if (ret==-21)
			{
				XMessageBox(g_hWindow,L"����������Ӧ[-21]",L"��ʾ");
			}	
			return false;
		}
	
	}
	//FreeLibrary(g_hDllJBC);
	return true;
}

BOOL OpenRegCardCom(int nPort)
{
	CString str;
	str.Format("%d",nPort);
	str="COM"+str;
	g_hCom=CreateFile(str,//COM1��
		GENERIC_READ|GENERIC_WRITE, //�������д
		0, //��ռ��ʽ
		NULL,
		OPEN_EXISTING, //�򿪶����Ǵ���
		0, //ͬ����ʽ
		NULL);
	if(g_hCom==(HANDLE)-1)
	{
		//XMessageBox(g_hWindow,L"�������˿ڴ�ʧ��",L"����");
		return FALSE;
	}		
	SetupComm(g_hCom,1000,1000); //���뻺����������������Ĵ�С����100		
	COMMTIMEOUTS TimeOuts;
	//�趨����ʱ
	TimeOuts.ReadIntervalTimeout=MAXDWORD;
	TimeOuts.ReadTotalTimeoutMultiplier=0;
	TimeOuts.ReadTotalTimeoutConstant=0;
	//�ڶ�һ�����뻺���������ݺ���������������أ�
	//�������Ƿ������Ҫ����ַ���						
	//�趨д��ʱ
	TimeOuts.WriteTotalTimeoutMultiplier=100;
	TimeOuts.WriteTotalTimeoutConstant=500;
	SetCommTimeouts(g_hCom,&TimeOuts); //���ó�ʱ
	
	DCB dcb;
	GetCommState(g_hCom,&dcb);
	dcb.BaudRate=9600; //������Ϊ9600
	dcb.ByteSize=8; //ÿ���ֽ���8λ
	dcb.Parity=NOPARITY; //����żУ��λ
	dcb.StopBits=TWOSTOPBITS; //����ֹͣλ
	SetCommState(g_hCom,&dcb);		
	PurgeComm(g_hCom,PURGE_TXCLEAR|PURGE_RXCLEAR);
	return true;
}

void SetTitleFont()
{
	return;
	HFONT hFont = ::CreateFont(14, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, 
							GB2312_CHARSET, 
							OUT_DEFAULT_PRECIS, 
							CLIP_DEFAULT_PRECIS, 
							DEFAULT_QUALITY, 
							DEFAULT_PITCH & FF_ROMAN, 
							"����");		//΢���ź� 20
    //HFONTX hFont = XFont_Create2(L"����",14,false,false,false,false);
	for (int i = 0; i < sizeof(g_nTitleFontID) / sizeof(int); i++)
	{
		if (g_nTitleFontID[i] != 0)
		{
			HELE hEle = XWnd_GetEle(g_hWindow, g_nTitleFontID[i]);
			XEle_SetFont(hEle, hFont);
		}
	}
	return;
}

void SetID2InfoFont()
{
	HFONT hFont = ::CreateFont(16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, 
		GB2312_CHARSET, 
		OUT_DEFAULT_PRECIS, 
		CLIP_DEFAULT_PRECIS, 
		DEFAULT_QUALITY, 
		DEFAULT_PITCH & FF_ROMAN, 
		"����ϸ��");
	
	for (int i = 0; i < sizeof(g_nID2FontID) / sizeof(int); i++)
	{
		if (g_nID2FontID[i] != 0)
		{
			HELE hEle = XWnd_GetEle(g_hWindow, g_nID2FontID[i]);
			//XEle_SetFont(hEle, hFont);
		}
	}
	return;
}

void W_2_S(wchar_t *pws, char *psz, int pwslen)
{
	WideCharToMultiByte(CP_ACP, 0, pws, -1, psz, 2 * pwslen + 1, NULL, NULL);
	return;
}

void S_2_W(wchar_t *pws, char *psz, int slen)
{
	MultiByteToWideChar(CP_ACP, 0, psz, -1, pws, slen);
	return;
}

void SetInfo(int nResID, wchar_t *pwsInfo)
{
	return;

	HELE hEle = XWnd_GetEle(g_hWindow, nResID);
	XStatic_SetText(hEle, pwsInfo);
	XStatic_SetTextAlign(hEle, DT_CENTER | DT_EDITCONTROL | DT_WORDBREAK);

	return;
}

//������������Ϣ���������
void SetVisitorInfo(IDCARD_ALL &visitorID2Card)
{
	wchar_t wBuf[256 + 1] = {0};
	VISITORINFO visitor;
	memset((char *)&visitor, 0, sizeof(VISITORINFO));
	strcpy(visitor.szName, visitorID2Card.name);
	strcpy(visitor.szID, visitorID2Card.number);
	//if (CheckIsBlackName(visitor))
	char szNotes[200] = {'\0'},szMsg[500] = {'\0'};
	if (CheckBlackNameNotes(visitor,szNotes))
	{
		if (strlen(szNotes) > 0)
		{
			sprintf(szMsg, "���������ں�������,����ϸȷ��������!\r\n\r\n�澯��%s",szNotes);
			S_2_W(wBuf, szMsg, 256);
			CSelectPrinttips s(GETHWND);
			s.CreateWindow_XC(wBuf,L"��ܰ��ʾ");
			s.DoModal_XC();
		}
		else
		{
			sprintf(szMsg, "���������ں�������,����ϸȷ��������!");
			S_2_W(wBuf, szMsg, 256);
			XMessageBox(g_hWindow, wBuf, L"����");
			//CSelectPrinttips s(GETHWND);
			//s.CreateWindow_XC(wBuf);
			//s.DoModal_XC();
		}

		return ;
	}
    
    SetVisitorReadOnly(false);
	
	memset((char *)wBuf, 0, sizeof(wBuf));
	MultiByteToWideChar(CP_ACP, 0, visitorID2Card.name, -1, wBuf, 256);
	XEdit_SetText(XWnd_GetEle(g_hWindow, IDE_VISITORNAME), wBuf);

	memset((char *)wBuf, 0, sizeof(wBuf));
	MultiByteToWideChar(CP_ACP, 0, visitorID2Card.sex, -1, wBuf, 256);
	XEdit_SetText(XWnd_GetEle(g_hWindow, IDC_VISITORSEX), wBuf);

	memset((char *)wBuf, 0, sizeof(wBuf));
	MultiByteToWideChar(CP_ACP, 0, visitorID2Card.number, -1, wBuf, 256);
	XEdit_SetText(XWnd_GetEle(g_hWindow, IDE_VISITORIDNUM), wBuf);

	memset((char *)wBuf, 0, sizeof(wBuf));
	MultiByteToWideChar(CP_ACP, 0, visitorID2Card.address, -1, wBuf, 256);
	XEdit_SetText(XWnd_GetEle(g_hWindow, IDE_VISITORADDR), wBuf);

	memset((char *)wBuf, 0, sizeof(wBuf));
	MultiByteToWideChar(CP_ACP, 0, visitorID2Card.szSignDep, -1, wBuf, 256);		//��֤����
    if(g_bJob)
	{
		MultiByteToWideChar(CP_ACP, 0, visitorID2Card.SurnameEN, -1, wBuf, 256);		//"Ӣ����"��Ƭְ��
		XEdit_SetText(XWnd_GetEle(g_hWindow, IDE_FZJG), wBuf);
	}
	else
	{
		MultiByteToWideChar(CP_ACP, 0, visitorID2Card.szSignDep, -1, wBuf, 256);		//��֤����
		XEdit_SetText(XWnd_GetEle(g_hWindow, IDE_FZJG), wBuf);
	}
	/*
	switch (g_nIDType)
	{
	case ID_TYPE_ID2:
        XEdit_SetText(XWnd_GetEle(g_hWindow, IDC_VISITORIDTYPE), L"�������֤");
		//g_nIDType = ;
		break;
	case ID_TYPE_ID:
        XEdit_SetText(XWnd_GetEle(g_hWindow, IDC_VISITORIDTYPE), L"һ�����֤");
		//g_nIDType = ID_TYPE_ID;
		break;
	case ID_TYPE_PASSPORT:
        XEdit_SetText(XWnd_GetEle(g_hWindow, IDC_VISITORIDTYPE), L"����");
		//g_nIDType = ID_TYPE_PASSPORT;
		break;
	case ID_TYPE_TOHKPASSPORT:
        XEdit_SetText(XWnd_GetEle(g_hWindow, IDC_VISITORIDTYPE), L"��½����۰�ͨ��֤");
		//g_nIDType = ID_TYPE_TOHKPASSPORT;
		break;
	case ID_TYPE_HKID:
        XEdit_SetText(XWnd_GetEle(g_hWindow, IDC_VISITORIDTYPE), L"�������֤");
        //g_nIDType = ID_TYPE_HKID;
		break;
	case ID_TYPE_TAIWAN:
        XEdit_SetText(XWnd_GetEle(g_hWindow, IDC_VISITORIDTYPE), L"̨��֤");
        //g_nIDType = ID_TYPE_TAIWAN;
		break;
	case ID_TYPE_DriverLicense:
        XEdit_SetText(XWnd_GetEle(g_hWindow, IDC_VISITORIDTYPE), L"��ʻ֤");
		//g_nIDType = ID_TYPE_CARID;
		break;
	case ID_TYPE_Lawyer:
        XEle_ShowEle(XWnd_GetEle(g_hWindow, IDS_Notice), true);
		XEle_ShowEle(XWnd_GetEle(g_hWindow, IDC_Notice_Comb), true);
		XEle_ShowEle(XWnd_GetEle(g_hWindow, IDS_LawyerLetter), true);
		XEle_ShowEle(XWnd_GetEle(g_hWindow, IDC_LawyerLetter_Comb), true);
		XComboBox_DeleteItemAll(GETELE(IDC_Notice_Comb));
		memset((char *)wBuf, 0, sizeof(wBuf));
		MultiByteToWideChar(CP_ACP, 0, visitor.szUnit, -1, wBuf, 256);
		XEdit_SetText(XWnd_GetEle(g_hWindow, IDC_Notice_Comb), wBuf);
		XEle_EnableEle(GETELE(IDC_Notice_Comb), false);
	    //XComboBox_AddString(GETELE(IDC_Notice_Comb), L"��");
	    //XComboBox_AddString(GETELE(IDC_Notice_Comb), L"��");
		//XComboBox_SetSelectItem(GETELE(IDC_Notice_Comb), 0);

		XComboBox_DeleteItemAll(GETELE(IDC_LawyerLetter_Comb));
		memset((char *)wBuf, 0, sizeof(wBuf));
		MultiByteToWideChar(CP_ACP, 0, visitor.szCarNum, -1, wBuf, 256);
		XEdit_SetText(XWnd_GetEle(g_hWindow, IDC_LawyerLetter_Comb), wBuf);
		XEle_EnableEle(GETELE(IDC_LawyerLetter_Comb), false);
	    //XComboBox_AddString(GETELE(IDC_LawyerLetter_Comb), L"��");
	   // XComboBox_AddString(GETELE(IDC_LawyerLetter_Comb), L"��");
		//XComboBox_SetSelectItem(GETELE(IDC_LawyerLetter_Comb), 0);

		XEle_ShowEle(XWnd_GetEle(g_hWindow, IDS_VISITORUNIT), false);
		XEle_ShowEle(XWnd_GetEle(g_hWindow, IDE_VISITORUNIT), false);
		XEle_ShowEle(XWnd_GetEle(g_hWindow, IDS_CARNUM), false);
		XEle_ShowEle(XWnd_GetEle(g_hWindow, IDE_VISITORCARNUM), false);
        XEdit_SetText(XWnd_GetEle(g_hWindow, IDC_VISITORIDTYPE), L"��ʦ֤");
        //g_nIDType = ID_TYPE_Lawyer;
		break;
	default:
		g_nIDType = ID_TYPE_OTHER;
        XEdit_SetText(XWnd_GetEle(g_hWindow, IDC_VISITORIDTYPE), L"����֤��");
		break;
	}
    */

    memset((char *)wBuf, 0, sizeof(wBuf));
	int nFolkID = atoi(visitorID2Card.people);
	if (nFolkID>60)
	{
		if(nFolkID==97)
		{
            MultiByteToWideChar(CP_ACP, 0, sFolk[60], -1, wBuf, 256);		//����
		}
		else
            MultiByteToWideChar(CP_ACP, 0, sFolk[61], -1, wBuf, 256);		//����
	} 
	else
		MultiByteToWideChar(CP_ACP, 0, sFolk[nFolkID], -1, wBuf, 256);		//����
	XEdit_SetText(XWnd_GetEle(g_hWindow, IDC_VISITORFOLK), wBuf);

	/*int nFolkID = atoi(visitorID2Card.people);
	if (nFolkID > 0)
	{
		XComboBox_SetSelectItem(XWnd_GetEle(g_hWindow, IDC_VISITORFOLK), nFolkID - 1);
	}
	else
	{
		XComboBox_SetSelectItem(XWnd_GetEle(g_hWindow, IDC_VISITORFOLK), 0);
	}*/

	char szT[256] = {'\0'};
	VisitorTotalTimes(visitorID2Card.name, szT);

	if (::GetPrivateProfileInt("TerminalSet", "bVisitorReadOnly",0, CONFIG_FILE)==1)  //���õǼ�֤������ֻ��
	   SetVisitorReadOnly(true);
	return;
}

//������������Ϣ���������
void SetVisitorInfo(const VISITORINFO &visitor)
{
	wchar_t wBuf[256 + 1] = {0};
	//if (CheckIsBlackName(visitor))
	char szNotes[200] = {'\0'},szMsg[500] = {'\0'};
	if (CheckBlackNameNotes(visitor,szNotes))
	{
		HideSoftKeybd();
		if (strlen(szNotes) > 0)
		{
			sprintf(szMsg, "���������ں�������,����ϸȷ��������!\r\n\r\n�澯��%s",szNotes);
			S_2_W(wBuf, szMsg, 256);
			CSelectPrinttips s(GETHWND);
			s.CreateWindow_XC(wBuf,L"��ܰ��ʾ");
			s.DoModal_XC();
		}
		else
		{
			sprintf(szMsg, "���������ں�������,����ϸȷ��������!");
			S_2_W(wBuf, szMsg, 256);
			XMessageBox(g_hWindow, wBuf, L"����");
			//CSelectPrinttips s(GETHWND);
			//s.CreateWindow_XC(wBuf);
			//s.DoModal_XC();
		}
		return ;
	}

	for (int i = 5003; i <= 5024; i++)		//�������
	{
		//HELE hE = GETELE(i);
		XEdit_SetReadOnly(GETELE(i),false);
		 //XEdit_SetText(hE, L"");
	}

	
	
	memset((char *)wBuf, 0, sizeof(wBuf));
	MultiByteToWideChar(CP_ACP, 0, visitor.szName, -1, wBuf, 256);
	XEdit_SetText(XWnd_GetEle(g_hWindow, IDE_VISITORNAME), wBuf);
	
	memset((char *)wBuf, 0, sizeof(wBuf));
	MultiByteToWideChar(CP_ACP, 0, visitor.szSex, -1, wBuf, 256);
	XEdit_SetText(XWnd_GetEle(g_hWindow, IDC_VISITORSEX), wBuf);

	
	memset((char *)wBuf, 0, sizeof(wBuf));
	MultiByteToWideChar(CP_ACP, 0, visitor.szID, -1, wBuf, 256);
	XEdit_SetText(XWnd_GetEle(g_hWindow, IDE_VISITORIDNUM), wBuf);
	
	memset((char *)wBuf, 0, sizeof(wBuf));
	MultiByteToWideChar(CP_ACP, 0, visitor.szAddr, -1, wBuf, 256);
	XEdit_SetText(XWnd_GetEle(g_hWindow, IDE_VISITORADDR), wBuf);
	
	memset((char *)wBuf, 0, sizeof(wBuf));	
    if(g_bJob)
	{
		MultiByteToWideChar(CP_ACP, 0, visitor.szJob, -1, wBuf, 256);		
		//XEdit_SetText(XWnd_GetEle(g_hWindow, IDE_FZJG), L"");
	}
	else
	{
		MultiByteToWideChar(CP_ACP, 0, visitor.szSignDepartment, -1, wBuf, 256);		//��֤����		
	}
	XEdit_SetText(XWnd_GetEle(g_hWindow, IDE_FZJG), wBuf);

	XEle_ShowEle(XWnd_GetEle(g_hWindow, IDS_Notice), false);
	XEle_ShowEle(XWnd_GetEle(g_hWindow, IDC_Notice_Comb), false);
	XEle_ShowEle(XWnd_GetEle(g_hWindow, IDS_LawyerLetter), false);
	XEle_ShowEle(XWnd_GetEle(g_hWindow, IDC_LawyerLetter_Comb), false);

    XEle_ShowEle(XWnd_GetEle(g_hWindow, IDS_VISITORUNIT), true);
	XEle_ShowEle(XWnd_GetEle(g_hWindow, IDE_VISITORUNIT), true);
	XEle_ShowEle(XWnd_GetEle(g_hWindow, IDS_CARNUM), true);
	XEle_ShowEle(XWnd_GetEle(g_hWindow, IDE_VISITORCARNUM), true);

	memset((char *)wBuf, 0, sizeof(wBuf));
	MultiByteToWideChar(CP_ACP, 0, visitor.szCarNum, -1, wBuf, 256);
	XEdit_SetText(XWnd_GetEle(g_hWindow, IDE_VISITORCARNUM), wBuf);

	memset((char *)wBuf, 0, sizeof(wBuf));
	MultiByteToWideChar(CP_ACP, 0, visitor.szUnit, -1, wBuf, 256);
	XEdit_SetText(XWnd_GetEle(g_hWindow, IDE_VISITORUNIT), wBuf);
	
    //int n=atoi(visitor.szIDType);
	switch (atoi(visitor.szIDType))
	{
	case 1:
        XEdit_SetText(XWnd_GetEle(g_hWindow, IDC_VISITORIDTYPE), L"�������֤");
		g_nIDType = ID_TYPE_ID2;
		break;
	case 2:
        XEdit_SetText(XWnd_GetEle(g_hWindow, IDC_VISITORIDTYPE), L"һ�����֤");
		g_nIDType = ID_TYPE_ID;
		break;
	case 3:
        XEdit_SetText(XWnd_GetEle(g_hWindow, IDC_VISITORIDTYPE), L"����");
		g_nIDType = ID_TYPE_PASSPORT;
		break;
	case 4:
        XEdit_SetText(XWnd_GetEle(g_hWindow, IDC_VISITORIDTYPE), L"��½����۰�ͨ��֤");
		g_nIDType = ID_TYPE_TOHKPASSPORT;
		break;
	case 5:
        XEdit_SetText(XWnd_GetEle(g_hWindow, IDC_VISITORIDTYPE), L"�������֤");
        g_nIDType = ID_TYPE_HKID;
		break;
	case 6:
        XEdit_SetText(XWnd_GetEle(g_hWindow, IDC_VISITORIDTYPE), L"̨��֤");
        g_nIDType = ID_TYPE_TAIWAN;
		break;
	case 7:
        XEdit_SetText(XWnd_GetEle(g_hWindow, IDC_VISITORIDTYPE), L"��ʻ֤");
		g_nIDType = ID_TYPE_DriverLicense;
		break;
	case 8:
        XEle_ShowEle(XWnd_GetEle(g_hWindow, IDS_Notice), true);
		XEle_ShowEle(XWnd_GetEle(g_hWindow, IDC_Notice_Comb), true);
		XEle_ShowEle(XWnd_GetEle(g_hWindow, IDS_LawyerLetter), true);
		XEle_ShowEle(XWnd_GetEle(g_hWindow, IDC_LawyerLetter_Comb), true);
		XComboBox_DeleteItemAll(GETELE(IDC_Notice_Comb));
		memset((char *)wBuf, 0, sizeof(wBuf));
		MultiByteToWideChar(CP_ACP, 0, visitor.szUnit, -1, wBuf, 256);
		XEdit_SetText(XWnd_GetEle(g_hWindow, IDC_Notice_Comb), wBuf);
		XEle_EnableEle(GETELE(IDC_Notice_Comb), false);
	    //XComboBox_AddString(GETELE(IDC_Notice_Comb), L"��");
	    //XComboBox_AddString(GETELE(IDC_Notice_Comb), L"��");
		//XComboBox_SetSelectItem(GETELE(IDC_Notice_Comb), 0);

		XComboBox_DeleteItemAll(GETELE(IDC_LawyerLetter_Comb));
		memset((char *)wBuf, 0, sizeof(wBuf));
		MultiByteToWideChar(CP_ACP, 0, visitor.szCarNum, -1, wBuf, 256);
		XEdit_SetText(XWnd_GetEle(g_hWindow, IDC_LawyerLetter_Comb), wBuf);
		XEle_EnableEle(GETELE(IDC_LawyerLetter_Comb), false);
	    //XComboBox_AddString(GETELE(IDC_LawyerLetter_Comb), L"��");
	   // XComboBox_AddString(GETELE(IDC_LawyerLetter_Comb), L"��");
		//XComboBox_SetSelectItem(GETELE(IDC_LawyerLetter_Comb), 0);

		XEle_ShowEle(XWnd_GetEle(g_hWindow, IDS_VISITORUNIT), false);
		XEle_ShowEle(XWnd_GetEle(g_hWindow, IDE_VISITORUNIT), false);
		XEle_ShowEle(XWnd_GetEle(g_hWindow, IDS_CARNUM), false);
		XEle_ShowEle(XWnd_GetEle(g_hWindow, IDE_VISITORCARNUM), false);
        XEdit_SetText(XWnd_GetEle(g_hWindow, IDC_VISITORIDTYPE), L"��ʦ֤");
        g_nIDType = ID_TYPE_Lawyer;
		break;
	case 9:
        XEdit_SetText(XWnd_GetEle(g_hWindow, IDC_VISITORIDTYPE), L"��Ƭ");
		g_nIDType = ID_TYPE_BusinessCard;
		break;
	default:
		g_nIDType = ID_TYPE_OTHER;
        XEdit_SetText(XWnd_GetEle(g_hWindow, IDC_VISITORIDTYPE), L"����֤��");
		break;
	}

	/*if (atoi(visitor.szIDType) != 99)
	{
		XComboBox_SetSelectItem(XWnd_GetEle(g_hWindow, IDC_VISITORIDTYPE), atoi(visitor.szIDType) - 1);
	}
	else
	{
		XComboBox_SetSelectItem(XWnd_GetEle(g_hWindow, IDC_VISITORIDTYPE), 7);
	}*/
	
/*	int nFolkID = atoi(visitor.szFolk);
	if (nFolkID > 0)
	{
		XComboBox_SetSelectItem(XWnd_GetEle(g_hWindow, IDC_VISITORFOLK), nFolkID - 1);		
	}
	else
	{
		XComboBox_SetSelectItem(XWnd_GetEle(g_hWindow, IDC_VISITORFOLK), 0);
	}*/
	memset((char *)wBuf, 0, sizeof(wBuf));
	MultiByteToWideChar(CP_ACP, 0, visitor.szFolk, -1, wBuf, 256);
	XEdit_SetText(XWnd_GetEle(g_hWindow, IDC_VISITORFOLK), wBuf);
	

	/*

	*/
	if(g_bVisitorType)
	{
		memset((char *)wBuf, 0, sizeof(wBuf));
		MultiByteToWideChar(CP_ACP, 0, visitor.szType, -1, wBuf, 256);
		XEdit_SetText(XWnd_GetEle(g_hWindow, IDE_VISITORTYPE), wBuf);
	}
	else
	{
		memset((char *)wBuf, 0, sizeof(wBuf));
		MultiByteToWideChar(CP_ACP, 0, visitor.szTotalPeople, -1, wBuf, 256);
		XEdit_SetText(XWnd_GetEle(g_hWindow, IDE_VISITORTOTAL), wBuf);
	}

	
	
	memset((char *)wBuf, 0, sizeof(wBuf));
	MultiByteToWideChar(CP_ACP, 0, visitor.szPhone, -1, wBuf, 256);
	XEdit_SetText(XWnd_GetEle(g_hWindow, IDE_VISITORMOBILE), wBuf);

	memset((char *)wBuf, 0, sizeof(wBuf));
	MultiByteToWideChar(CP_ACP, 0, visitor.szReason, -1, wBuf, 256);
	XEdit_SetText(XWnd_GetEle(g_hWindow, IDE_VISITORREASON), wBuf);

	memset((char *)wBuf, 0, sizeof(wBuf));
	MultiByteToWideChar(CP_ACP, 0, visitor.szWithGoods, -1, wBuf, 256);
	XEdit_SetText(XWnd_GetEle(g_hWindow, IDE_VISITORTHING), wBuf);

	char szT[256] = {'\0'};
	VisitorTotalTimes((char *)visitor.szName, szT);

	if (::GetPrivateProfileInt("TerminalSet", "bVisitorReadOnly",0, CONFIG_FILE)==1)  //���õǼ�֤������ֻ��
	   SetVisitorReadOnly(true);
	else
	   SetVisitorReadOnly(false);
	return;
}

//���ñ���������Ϣ���������
void SetOfficialInfo(const OFFICIALINFO &official)
{
	wchar_t wBuf[256 + 1] = {0};
	SetOfficialReadOnly(false);
	S_2_W(wBuf, (char *)official.szName, 128);
	//if (wcslen(wBuf) > 0)
	{
		XEdit_SetText(GETELE(IDE_MASTERNAME), wBuf);
	}

	memset((char *)wBuf, 0, sizeof(wBuf));
	S_2_W(wBuf, (char *)official.szSex, 128);
    XEdit_SetText(GETELE(IDC_MASTERSEX), wBuf);
	/*if (strcmp(official.szSex, "Ů") == 0)
	{
		XComboBox_SetSelectItem(GETELE(IDC_MASTERSEX), 1);
	}
	else
	{
		XComboBox_SetSelectItem(GETELE(IDC_MASTERSEX), 0);
	}*/

	memset((char *)wBuf, 0, sizeof(wBuf));
	S_2_W(wBuf, (char *)official.szOfficePhone, 128);
	//if (wcslen(wBuf) > 0)
	{
		XEdit_SetText(GETELE(IDE_OFFICEPHONE), wBuf);
	}

	memset((char *)wBuf, 0, sizeof(wBuf));
	S_2_W(wBuf, (char *)official.szMobile, 128);
	//if (wcslen(wBuf) > 0)
	{
		XEdit_SetText(GETELE(IDE_MASTERMOBILE), wBuf);
	}

	memset((char *)wBuf, 0, sizeof(wBuf));
	S_2_W(wBuf, (char *)official.szOfficeName, 128);
	//if (wcslen(wBuf) > 0)
	{
		XEdit_SetText(GETELE(IDE_OFFICENAME), wBuf);
	}

	memset((char *)wBuf, 0, sizeof(wBuf));
	S_2_W(wBuf, (char *)official.szOfficeAddr, 128);
	//if (wcslen(wBuf) > 0)
	{
		XEdit_SetText(GETELE(IDE_OFFICENUM), wBuf);
	}

	memset((char *)wBuf, 0, sizeof(wBuf));
	S_2_W(wBuf, (char *)official.szOfficialDuty, 128);
	//if (wcslen(wBuf) > 0)
	{
		XEdit_SetText(GETELE(IDC_MASTERZHIWU), wBuf);
	}

	memset((char *)wBuf, 0, sizeof(wBuf));
	S_2_W(wBuf, (char *)official.szRoom, 128);
	//if (wcslen(wBuf) > 0)
	{
		XEdit_SetText(GETELE(IDE_ROOMNUM), wBuf);
	}

	if (::GetPrivateProfileInt("TerminalSet", "bSaveOfficial",0, CONFIG_FILE)==1)  //�����±�����
	   SetOfficialReadOnly(false);
	else
	   SetOfficialReadOnly(true);

	return;
}
/*void CALLBACK VisitorList_DrawItem(HELE hEle,list_drawItem_ *pDrawItem)
{
    //���Ʊ���
    if(STATE_SELECT==pDrawItem->state)
    {
        XDraw_FillSolidRect_(pDrawItem->hDraw,&pDrawItem->rect,RGB(0,255,0));
    }else
    {
        if(0==pDrawItem->index%2)
            XDraw_FillSolidRect_(pDrawItem->hDraw,&pDrawItem->rect,RGB(197,255,197));
        else
            XDraw_FillSolidRect_(pDrawItem->hDraw,&pDrawItem->rect,RGB(255,202,202));
    }
    //����ͼ��
    HXCGUI hImageList=XList_GetImageList(hEle);
    RECT rect=pDrawItem->rect;
    if(hImageList)
    {
        rect.left+=3;
        XImageList_DrawImage(hImageList,pDrawItem->hDraw,pDrawItem->imageId,rect.left,pDrawItem->rect.top+3);
        rect.left+=19;
    }else
        rect.left+=3;
    //�����ı�
    XDraw_DrawText_(pDrawItem->hDraw,pDrawItem->pText,wcslen(pDrawItem->pText),&rect,DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS);
}*/
//��ʾ���������ߵ���Ϣ
void ListTodayVistorsInfo()
{
	char szT[512] = {'\0'};
	long FldCount = 0, ret = 0;
	CString outParas[FLD_MAX], outP1[FLD_MAX];
	CWinAdo tAdo(sCONNSTR_MYSQL);
	CWinAdo tAdo1(sCONNSTR_MYSQL);

	HELE hList = XWnd_GetEle(g_hWindow, IDL_VISTORLIST);

	XList_SetItemHeight(hList,40);

	XList_DeleteAllColumns(hList);
	XList_DeleteAllItems(hList);

	XList_SetHeaderHeight(hList, 30);
	XList_EnableGrid(hList, false);
	XList_EnableCheckBox(hList, true);
	XList_EnableMultilineSelect(hList, false);
	
	XList_AddColumn(hList, 50, L"���");
	XList_AddColumn(hList, 40, L"�Ÿ�");
	XList_AddColumn(hList, 60, L"�ÿ�����");
	XList_AddColumn(hList, 35, L"�Ա�");
	XList_AddColumn(hList, 120, L"֤������");
	XList_AddColumn(hList, 125, L"����ʱ��");
	XList_AddColumn(hList, 90, L"��������");
	XList_AddColumn(hList, 90, L"Я����Ʒ");
	
	XList_AddColumn(hList, 60, L"������");
	XList_AddColumn(hList, 100, L"��ȥ����");

	if(g_bAutoCallOfficials)
		XList_AddColumn(hList, 125, L"ȷ�����");
	else
		XList_AddColumn(hList, 125, L"�뿪ʱ��");
	
	ret = tAdo.OpenAdo();
	if (ret != ADOOK)
	{
		return;
	}

	ret = tAdo1.OpenAdo();
	if (ret != ADOOK)
	{
		return;
	}

	int n = 0, i = 0;

	for (i = 0; i < FLD_MAX; i++)
	{
		outParas[i].Empty();
		outP1[i].Empty();
	}

	//ѡ������ʱ��Ϊ��������м�¼
	sprintf(szT, "select * from t_visitor where visitor_station_id = \'%s\' and visitor_status > 1 and visitor_lefttime <= 0 order by visitor_visitetime desc",g_szStation);
	ret = tAdo.execSelect(szT, &FldCount, outParas);
	if (ret == ADOERR || (FldCount == 0 && outParas[0].IsEmpty()))
	{
		return;
	}
	do
	{
		wchar_t wBuf[128 + 1] = {0};
		memset(szT, 0, sizeof(szT));
		sprintf(szT, "%d", n + 1);		//���
		S_2_W(wBuf, szT, 128);
		XList_AddItem(hList, wBuf, n);

		//list visitor information
		L_V_I(outParas[18], n, 1)		//�Ÿ�
		L_V_I(outParas[0], n, 2)
		L_V_I(outParas[1], n, 3)
		L_V_I(outParas[5], n, 4)

		if (atol(outParas[16]) <= 0)
		{
			L_V_I("", n, 8)
		}
		else
		{
			//sprintf(szT, "select FROM_UNIXTIME(%s)", outParas[16]);	//����ʱ��
			if (g_nSqlConnectType == SQL_CONNECT_SERVER2005)
			{
				sprintf(szT, "SELECT DATEADD(s,%d,'1970-01-01 08:00:00')", atol(outParas[16]));	//����ʱ��
			}
			else
			{
				sprintf(szT, "select FROM_UNIXTIME(%s)", outParas[16]);	//����ʱ��
			}
			ret = tAdo1.execSelect(szT, &FldCount, outP1);
			if (ret == ADOERR || (FldCount == 0 && outP1[0].IsEmpty()))
			{
				L_V_I("", n, 5)
			}
			else
			{
				L_V_I(outP1[0], n, 5)			//����ʱ��
			}
		}

		L_V_I(outParas[8], n, 6)			//��������
		L_V_I(outParas[7], n, 7)			//Я����Ʒ

		if(g_bAutoCallOfficials)
		{
			int ret = 0;
			ret = atoi(outParas[39]);
			if(ret == 99)
			{
				outParas[39] = "�����";
			}
			else if(ret == -1)
			{
				outParas[39] = "δͨ��";
			}
			else if(ret > -1 && ret < 3)
			{
				outParas[39] = "������:" + outParas[39];
			}
			else
			{
				outParas[39] = "��ʱ��Ӧ��:" + outParas[39];
			}
			L_V_I(outParas[39], n, 10)
		}	
		else
		{
			if (atol(outParas[17]) <= 0)
			{
				L_V_I("", n, 10)
			}
			else
			{
				//sprintf(szT, "select FROM_UNIXTIME(%s)", outParas[17]);	//�뿪ʱ��
				if (g_nSqlConnectType == SQL_CONNECT_SERVER2005)
				{
					sprintf(szT, "SELECT DATEADD(s,%d,'1970-01-01 08:00:00')", atol(outParas[17]));	//�뿪ʱ��
				}
				else
				{
					sprintf(szT, "select FROM_UNIXTIME(%s)", outParas[17]);	//�뿪ʱ��
				}
				ret = tAdo1.execSelect(szT, &FldCount, outP1);
				if (ret == ADOERR || (FldCount == 0 && outP1[0].IsEmpty()))
				{
					L_V_I("", n, 10)
				}
				else
				{
					L_V_I(outP1[0], n, 10)			//�뿪ʱ��
				}
			}
		}


		L_V_I(outParas[14], n, 8)			//����������
		L_V_I(outParas[15], n, 9)			//�����˰칫��
		n++;
	} while (tAdo.getNext(outParas) == ADOOK) ;
    //XList_SetUserDrawItem(hList,VisitorList_DrawItem);
	return;
}

//���ñ�������Ϣֻ��
void SetOfficialReadOnly(BOOL isReadOnly)
{
    XEdit_SetReadOnly(XWnd_GetEle(g_hWindow, IDE_MASTERNAME),isReadOnly);
    XComboBox_EnableEdit(XWnd_GetEle(g_hWindow, IDC_MASTERSEX),false);
	XEdit_SetReadOnly(XWnd_GetEle(g_hWindow, IDE_OFFICEPHONE),isReadOnly);
	XEdit_SetReadOnly(XWnd_GetEle(g_hWindow, IDE_OFFICENAME),isReadOnly);
	XEdit_SetReadOnly(XWnd_GetEle(g_hWindow, IDE_MASTERMOBILE),isReadOnly);
    XEdit_SetReadOnly(XWnd_GetEle(g_hWindow, IDE_OFFICENUM),isReadOnly);
	XEdit_SetReadOnly(XWnd_GetEle(g_hWindow, IDC_MASTERZHIWU),isReadOnly);
	XEdit_SetReadOnly(XWnd_GetEle(g_hWindow, IDE_ROOMNUM),isReadOnly);
}
//�����������ı���ֻ����
void SetVisitorReadOnly(BOOL isReadOnly)
{
	//XEdit_SetReadOnly(XWnd_GetEle(g_hWindow, 5004),false);  //�����Ա�
    XEdit_SetReadOnly(XWnd_GetEle(g_hWindow, 5004),isReadOnly);
    XEdit_SetReadOnly(XWnd_GetEle(g_hWindow, 5005),false);
	XEdit_SetReadOnly(XWnd_GetEle(g_hWindow, 5006),isReadOnly);  //֤������
    XEdit_SetReadOnly(XWnd_GetEle(g_hWindow, IDC_VISITORFOLK),isReadOnly);//��������
    XEdit_SetReadOnly(XWnd_GetEle(g_hWindow, 5023),true);
	XEdit_SetReadOnly(XWnd_GetEle(g_hWindow, 5003),isReadOnly);
	if(g_bVisitorType)
		XEdit_SetReadOnly(XWnd_GetEle(g_hWindow, IDE_VISITORTOTAL),isReadOnly);
	else
		XEdit_SetReadOnly(XWnd_GetEle(g_hWindow, IDE_VISITORTOTAL),false);

	for (int i = 5007; i <= 5008; i++)		//��������Ϣ�������ԴID
	{
		XEdit_SetReadOnly(XWnd_GetEle(g_hWindow, i),isReadOnly);
	}

	if(g_bJob)
		XEdit_SetReadOnly(XWnd_GetEle(g_hWindow, 5022),false);
	else
		XEdit_SetReadOnly(XWnd_GetEle(g_hWindow, 5022),isReadOnly);

}

void SetIDtypeComb(int nRes)
{
	HELE hE = XWnd_GetEle(g_hWindow, nRes);

	XComboBox_DeleteItemAll(hE);
	XComboBox_AddString(hE, L" ");
	XComboBox_AddString(hE, L"���֤");
	//XComboBox_AddString(hE, L"һ��֤");
	XComboBox_AddString(hE, L"����");
	XComboBox_AddString(hE, L"��½����۰�ͨ��֤");
	XComboBox_AddString(hE, L"�۰ľ������֤");
	XComboBox_AddString(hE, L"̨��֤");
	XComboBox_AddString(hE, L"��ʻ֤");
	XComboBox_AddString(hE, L"��Ƭ");
	XComboBox_AddString(hE, L"����֤��");
	XComboBox_SetSelectItem(hE,0);
	return;
}

void SetVisitortypeComb(int nRes)
{
	HELE hE = XWnd_GetEle(g_hWindow, nRes);

	XComboBox_DeleteItemAll(hE);
	XComboBox_AddString(hE, L" ");
	XComboBox_AddString(hE, L"�ڲ��ÿ�");
	//XComboBox_AddString(hE, L"һ��֤");
	XComboBox_AddString(hE, L"�ⲿ�ÿ�");
	XComboBox_AddString(hE, L"ʩ����Ա");
	XComboBox_AddString(hE, L"��ѵ��Ա");
	XComboBox_AddString(hE, L"�ͻ���Ա");
	XComboBox_AddString(hE, L"�����Ա");
	XComboBox_AddString(hE, L"��ˮ��Ա");
	XComboBox_AddString(hE, L"����");

	XComboBox_SetSelectItem(hE,0);
	return;
}

//�����Ա���ʾ
void SetSexInfo(int nRes)
{
	HELE hE = XWnd_GetEle(g_hWindow, nRes);

	XComboBox_DeleteItemAll(hE);
	XComboBox_AddString(hE, L"��");
	XComboBox_AddString(hE, L"Ů");
	//OnQueryOffice(XWnd_GetEle(g_hWindow, IDB_QUERYOFFICIAL), XWnd_GetEle(g_hWindow, IDB_QUERYOFFICIAL)); //Ĭ�ϲ�ѯ���п�����Ա
	return;
}

//�������ݿ�����������֤��������Ϣ
void SetIDType(int nResID)
{
	char szT[512] = {'\0'};
	long FldCount = 0, ret = 0;;
	CString outParas[FLD_MAX];
	CWinAdo tAdo(sCONNSTR_MYSQL);
	
	ret = tAdo.OpenAdo();
	if (ret != ADOOK)
	{
		return;
	}

	HELE hE = XWnd_GetEle(g_hWindow, nResID);
	XComboBox_DeleteItemAll(hE);
	
	for (int i = 0; i < FLD_MAX; i++)
	{
		outParas[i].Empty();
	}
	
	strcpy(szT, "select * from t_id_type");
	ret = tAdo.execSelect(szT, &FldCount, outParas);
	if (ret == ADOERR || (FldCount == 0 && outParas[0].IsEmpty()))
	{
		return;
	}
	do
	{
		sprintf(szT, "%s - %s", (LPSTR)(LPCTSTR)outParas[0], (LPSTR)(LPCTSTR)outParas[1]);
		wchar_t wBuf[FLD_MAX + 1] = {0};
		MultiByteToWideChar(CP_ACP, 0, szT, -1, wBuf, FLD_MAX);
		XComboBox_AddString(hE, wBuf);
	} while (tAdo.getNext(outParas) == ADOOK) ;
	XComboBox_SetSelectItem(hE, 0);
	
	return;
}

//�������ݿ������Ÿ���Ϣ
void SetStationInfo(int nResID)
{
	char szT[512] = {'\0'};
	long FldCount = 0, ret = 0;;
	CString outParas[FLD_MAX];
	CWinAdo tAdo(sCONNSTR_MYSQL);
	
	ret = tAdo.OpenAdo();
	if (ret != ADOOK)
	{
		return;
	}
	
	HELE hE = XWnd_GetEle(g_hWindow, nResID);
	XComboBox_DeleteItemAll(hE);
	
	for (int i = 0; i < FLD_MAX; i++)
	{
		outParas[i].Empty();
	}

	g_nComboBoxCount = 0;
	
	strcpy(szT, "select * from t_station");
	ret = tAdo.execSelect(szT, &FldCount, outParas);
	if (ret == ADOERR || (FldCount == 0 && outParas[0].IsEmpty()))
	{
		return;
	}
	do
	{
		sprintf(szT, "%s", (LPSTR)(LPCTSTR)outParas[0]);
		wchar_t wBuf[FLD_MAX + 1] = {0};
		MultiByteToWideChar(CP_ACP, 0, szT, -1, wBuf, FLD_MAX);
		XComboBox_AddString(hE, wBuf);
		g_nComboBoxCount++;
	} while (tAdo.getNext(outParas) == ADOOK) ;
	XComboBox_SetSelectItem(hE, 0);

	return;
}

//�������ݿ��ʼ������������֤��������Ϣ��������������Ϣ
void InitIDTypeAndFolk()
{
	char szT[512] = {'\0'};
	long FldCount = 0, ret = 0;;
	CString outParas[FLD_MAX];
	CWinAdo tAdo(sCONNSTR_MYSQL);

	ret = tAdo.OpenAdo();
	if (ret != ADOOK)
	{
		return;
	}

	//HELE hE = XWnd_GetEle(g_hWindow, IDC_VISITORIDTYPE);
	//XComboBox_DeleteItemAll(hE);

	/*for (int i = 0; i < FLD_MAX; i++)
	{
		outParas[i].Empty();
	}

	strcpy(szT, "select * from t_id_type");
	ret = tAdo.execSelect(szT, &FldCount, outParas);
	if (ret == ADOERR || (FldCount == 0 && outParas[0].IsEmpty()))
	{
		return;
	}
	do
	{
		sprintf(szT, "%s - %s", (LPSTR)(LPCTSTR)outParas[0], (LPSTR)(LPCTSTR)outParas[1]);
		//wchar_t wBuf[FLD_MAX + 1] = {0};
		//MultiByteToWideChar(CP_ACP, 0, szT, -1, wBuf, FLD_MAX);
		//XComboBox_AddString(hE, wBuf);
	} while (tAdo.getNext(outParas) == ADOOK);*/
	//XComboBox_SetSelectItem(hE, 0);*/

	//����������������Ϣ
	/*hE = XWnd_GetEle(g_hWindow, IDC_VISITORFOLK);
	XComboBox_DeleteItemAll(hE);*/
    int i;
	for (i = 0; i < FLD_MAX; i++)
	{
		outParas[i].Empty();
	}
	i=0;
	strcpy(szT, "select folk_name from t_folk");
	ret = tAdo.execSelect(szT, &FldCount, outParas);
	if (ret == ADOERR || (FldCount == 0 && outParas[0].IsEmpty())) 
	{
		return;
	}
	do
	{
	   //sprintf(szT, "%s - %s", (LPSTR)(LPCTSTR)outParas[0], (LPSTR)(LPCTSTR)outParas[1]);
      sFolk[i]=outParas[0];
	  i++;
	} while (tAdo.getNext(outParas) == ADOOK);


	return;
}

//�����������Ҳఴť����ʾ����ʼ����ص�����, hCurEleΪ��ǰҳ�棬Ҳ��ʱ˵hCurEle����־��ҳ������Ҫ���ص�
void ShowMainMenu(HELE hCurEle, BOOL bShow)
{
	if (hCurEle != NULL)
	{
		XEle_ShowEle(hCurEle, false);
	}

	XEle_ShowEle(g_hTab1, bShow);
	XEle_ShowEle(g_hTab2, bShow);
	XEle_ShowEle(g_hTab3, bShow);
//	XEle_ShowEle(g_hTab4, bShow);
	XEle_ShowEle(g_hTab5, bShow);
	//XEle_ShowEle(g_hTab6, bShow);
	XEle_ShowEle(g_hTab8, bShow);
	XEle_ShowEle(g_hTab9, bShow);
	XEle_ShowEle(g_hTab10, bShow);

	if (bShow)
	{
		//SetInfo(IDS_DEVINFO, L"");

		XEle_ShowEle(g_hPage1, true);
		XBtn_SetCheck(g_hTab1, true);
	}
	return;
}
//�Ķ�ic���к�
DWORD WINAPI ReadICThread(LPVOID lpVoid)
{
    SuspendThread(g_HWDThreadID2ID);
    int nICPort = GetPrivateProfileInt("TerminalSet", "ICPort",0, CONFIG_FILE); //IC�˿ں�
	if (true!=OpenRegCardCom(nICPort))
	{
		 XMessageBox(g_hWindow, L"IC�������˿ڴ�ʧ��!", L"����");
		 ResumeThread(g_HWDThreadID2ID);
		 return 0;
	}

	while (1)
	{    
	    char str[20];
	    memset(str,0,sizeof(str));
		memset(g_szICCode,0,sizeof(g_szICCode));
	    DWORD wCount=9;//��ȡ���ֽ���
	    BOOL bReadStat;
		bReadStat=ReadFile(g_hCom,str,wCount,&wCount,NULL);
		if(!bReadStat)
		{
			Sleep(0);
			continue;
		}
		PurgeComm(g_hCom, PURGE_TXABORT|PURGE_RXABORT|PURGE_TXCLEAR|PURGE_RXCLEAR);
		int x=strlen(str);
		if (x==9)
		{
			unsigned int buff[3],iRet;
			char szRet[20];
			CString stemp;
			buff[0]=(unsigned char)str[4];
			buff[1]=(unsigned char)str[5];
			buff[2]=(unsigned char)str[6];
			//iRet=buff[1]<<16+buff[2];
			int i;
			sprintf(szRet, "%d", buff[0]);
			int n=strlen(szRet);
			
			for (i=0;i<3-n;i++)
			{
				stemp+="0";
			}
			stemp+=szRet;
			
			memset(szRet,0,sizeof(szRet));
			iRet=buff[1]*256;
			iRet=iRet+buff[2];
			sprintf(szRet,"%d",iRet);
			n=strlen(szRet);
			for (i=0;i<5-n;i++)
			{
				stemp+="0";
			}
			stemp+=szRet;
			strcpy(g_szICCode,stemp);
			CloseHandle(g_hCom);
			g_hCom = NULL;
			break; //��������
		}
		else if (x>0)
		{
		   XMessageBox(g_hWindow, L"����ʧ��!������", L"����");
		}
		else
			Sleep(500);
	}
	Sleep(100);
    ResumeThread(g_HWDThreadID2ID);
	ResumeThread(g_HWDThreadID2ID);
    CloseHandle(g_ThreadID2IC);
	g_ThreadID2IC = NULL;
	return 0;
}
DWORD WINAPI ReadID2Thread2(LPVOID lpVoid)
{
    int ret;
	int iPort = GetPrivateProfileInt("TerminalSet", "ID2Port",1001, CONFIG_FILE);  //����֤�˿ں�
	bool bReadID2Notfy = GetPrivateProfileInt("TerminalSet", "bReadID2Notfy",0, CONFIG_FILE); 
	bool bReadAIC = GetPrivateProfileInt("TerminalSet", "bReadAIC",0, CONFIG_FILE);
    ret=InitComm(iPort);
	g_bICReg = false;
	if ( ret )
	{
		while (1)
		{	
			ret= Authenticate();
			if (ret){

			char Name[31] = {0};
			char Gender[3] = {0};
			char Folk[11] = {0};
			char BirthDay[9] = {0};
			char Code[19] = {0};
			char Address[71] = {0};
			char Agency[31] = {0};
			char ExpireStart[9] = {0};
			char ExpireEnd[9] = {0};
			
			ret = ReadBaseInfos(Name, Gender,Folk,BirthDay,Code,Address,Agency,ExpireStart,ExpireEnd);

				unsigned char Msg[200];
				int len;
				//ret= ReadBaseMsg(Msg,&len);
				//ret= ReadBaseMsgPhoto(Msg,&len,"D:\\");
				if (ret > 0 ){
					//��ʾ���ּ�ͼƬ��Ϣ
					int n = 0;
					char szT[128] = {'\0'};	
					//n = 7;
					//USER_ID2_TMP
					::CopyFile("photo.bmp", USER_ID2_TMP, false);
					::DeleteFile("photo.bmp");
					CString s;
					CString e;
					CString y,m,d,y2,m2,d2;
					g_nIDType = ID_TYPE_ID2;
					InitVisitorData();
					
					strcpy(g_stIDCard.name, Name);
					strcpy(g_stIDCard.sex, Gender);
					strcpy(g_stIDCard.people, Folk);
					strcpy(g_stIDCard.birthday, BirthDay);
					strcpy(g_stIDCard.address, Address);
					strcpy(g_stIDCard.number, Code);		
					strcpy(g_stIDCard.szSignDep, Agency);

					/*memcpy(g_stIDCard.name, &Msg[n], 31); n += 31;
					memcpy(g_stIDCard.sex, &Msg[n], 3); n += 3;
					memcpy(g_stIDCard.people, &Msg[n], 10); n += 10;
					memcpy(g_stIDCard.birthday, &Msg[n], 9); n += 9;
					memcpy(g_stIDCard.address, &Msg[n], 71); n += 71;
					memcpy(g_stIDCard.number, &Msg[n], 19); n += 19;
					memcpy(g_stIDCard.szSignDep, &Msg[n], 31); n += 31;
					memcpy(szT, &Msg[n], 9); n += 9;*/
                    s=ExpireStart;
					//strcpy(g_stIDCard.szSignDep, Agency);

					//memset(szT, 0, sizeof(szT));
					//memcpy(szT, &Msg[n], 9); n += 9;
					e=ExpireEnd;

                    memset(szT, 0, sizeof(szT));
					y=s.Left(4);
					s=s.Right(4);
					m=s.Left(2);
					d=s.Right(2);
					
					y2=e.Left(4);
					e=e.Right(4);
					m2=e.Left(2);
					d2=e.Right(2);
					sprintf(szT,"%s.%s.%s-%s.%s.%s",y,m,d,y2,m2,d2);
					strcpy(g_stIDCard.validterm, szT);
					
                    ::PostMessage(XWnd_GetHWnd(g_hWindow), WM_READID2_OVER, NULL, NULL);
                    if (bReadID2Notfy)
                    {
						sndPlaySound("Media\\Windows Ding.wav",SND_ASYNC);
                    }
					
					::PostMessage(XWnd_GetHWnd(g_hWindow), WM_COMPARE_FACE, NULL, NULL);
				}



				Sleep(1);
				/*unsigned char Msg1[200];
				int num;
				ret= ReadNewAppMsg(Msg1, &num );
				if (ret > 0 ){
					//��ʾ׷�ӵ�ַ��Ϣ
				}*/
				
			}

			if(bReadAIC)
			{
				char csn[1024]={0};
				//if (Routon_IC_HL_ReadCardSN(g_stIDCard.szICSerial))//��A������
				
				if (Routon_IC_HL_ReadCardSN(csn))
				{
					::WritePrivateProfileString("IC", "IDSerial", csn, CONFIG_FILE);
					if(g_bICReg)
					{
						::PostMessage(XWnd_GetHWnd(g_hWindow), WM_READICA_OVER, IC_REG, NULL);
					}
					else
					{
						::PostMessage(XWnd_GetHWnd(g_hWindow), WM_READICA_OVER, IC_SEARCH, NULL);
					}
					
					
					Routon_BeepLED(true,true,100);
				}
			}

		}
	}
	else
	{
		
		XEle_ShowEle(g_hTab21, true);
		XMessageBox(g_hWindow, L"����֤��������ʧ��!", L"����");
	}
	
	ret= CloseComm();
	
	return true;
}
//�Ķ�����֤
DWORD WINAPI ReadID2Thread(LPVOID lpVoid)
{
	char szData[128] = {'\0'};
	BOOL bRet = false;
	g_bStopID2 = false;
	int nID2Port = GetPrivateProfileInt("TerminalSet", "ID2Port",0, CONFIG_FILE);  //����֤�˿ں�
	bRet= OpenCardReader(1001,0);
	if (bRet!=0)
	{
		 XMessageBox(g_hWindow, L"����֤��������ʧ��!", L"����");
		 return 0;
	}
	while (1)
	{	
		PERSONINFOA Info;
		memset((char *)&Info, 0, sizeof(PERSONINFOA));


		bRet = GetPersonMsgA(&Info,USER_ID2_TMP);
		if (bRet==0)
		{
			//InitData();
			InitVisitorData();
			char szT[128] = {'\0'};		
			strcpy(g_stIDCard.name, Info.name);
			strcpy(g_stIDCard.sex, Info.sex);
			strcpy(g_stIDCard.people, Info.nation);
    		strcpy(g_stIDCard.birthday, Info.birthday);
			strcpy(g_stIDCard.address, Info.address);
			strcpy(g_stIDCard.number, Info.cardId);		
			strcpy(g_stIDCard.szSignDep, Info.police);
			memset(szT, 0, sizeof(szT));
			CString s=Info.validStart;
			CString e=Info.validEnd;
			CString y,m,d,y2,m2,d2;
			y=s.Left(4);
			s=s.Right(4);
			m=s.Left(2);
			d=s.Right(2);

			y2=e.Left(4);
			e=e.Right(4);
			m2=e.Left(2);
			d2=e.Right(2);
			sprintf(szT,"%s.%s.%s-%s.%s.%s",y,m,d,y2,m2,d2);
			strcpy(g_stIDCard.validterm, szT);
			::PostMessage(XWnd_GetHWnd(g_hWindow), WM_READID2_OVER, NULL, NULL);
			//Sleep(1000);
			//ResetCardReader();
		}
	}
	return 0;
}

DWORD WINAPI AutoCallOfficialsThread(LPVOID lpVoid)
{
	char szT[4096] = {'\0'};
	long FldCount = 0, ret = 0;
	wchar_t wBuf[256 + 1] = {0};
	CString outParas[FLD_MAX];
	CWinAdo tAdo(sCONNSTR_MYSQL);
	VISITORINFO visitor;
	

	ret = tAdo.OpenAdo();
	if (ret != ADOOK)
	{
		MessageBox(XWnd_GetHWnd(g_hWindow),"AutoCallOfficialsThread connect1 db failed!","Alarm",MB_OK);
		return false;
	}


	CWinAdo tAdo1(sCONNSTR_MYSQL);
	ret = tAdo1.OpenAdo();
	if (ret != ADOOK)
	{
		MessageBox(XWnd_GetHWnd(g_hWindow),"AutoCallOfficialsThread connect2 db failed!","Alarm",MB_OK);
		return false;
	}

	while(g_bAutoCallOfficials)
	{
		FldCount = 0;
		for (int i = 0; i < FLD_MAX; i++)
		{
			outParas[i].Empty();
		}
		//ѡ������ʱ��Ϊ��������м�¼
		//sprintf(szT, "select * from t_visitor where visitor_station_id = \'%s\' and visitor_status > 0 and visitor_lefttime <= 0 order by visitor_calls desc",g_szStation);//visitor_visitetime
		memset(szT, 0, sizeof(szT));
		sprintf(szT, "select \
							   v.visitor_name,v.visitor_id,v.visitor_reason,v.visitor_user_video_pic,v.visitor_user_id_pic,\
							   v.visitor_user_idfull_pic,o.official_IP,v.visitor_visitetime,v.visitor_user_head_pic,\
							   v.visitor_sex,v.visitor_folk,v.visitor_phone,v.visitor_unit,v.visitor_job,v.visitor_addr,\
							   v.visitor_withgoods,o.official_room,o.official_name,o.official_office_name,v.visitor_barcode from \
							   t_visitor as v inner join t_official as o on \
							   v.visitor_to_official_name=o.official_name and v.visitor_to_office_name=o.official_office_name where \
							   v.visitor_station_id = \'%s\' and v.visitor_status > 1 and v.visitor_lefttime <= 0 and v.visitor_calls < 3 and v.visitor_calls > -1 \
							   order by v.visitor_calls asc limit 1",g_szStation);
		::WritePrivateProfileString("TerminalSet", "test", szT, CONFIG_FILE);
		ret = tAdo.execSelect(szT, &FldCount, outParas);
		if (ret == ADOERR )
		{
			MessageBox(XWnd_GetHWnd(g_hWindow),"AutoCallOfficialsThread select visitors failed!","Alarm",MB_OK);
			return false;
		}
		else if(FldCount == 0 && outParas[0].IsEmpty())
		{

			Sleep(3000); //��������Ϣ3��

		}
		else
		{

			char ip[32] = {'\0'};
			CSendTcpSocket socket,socket2;
			


			memset((char *)&visitor, 0, sizeof(VISITORINFO));
			memset(g_szXmlInfo, 0, sizeof(g_szXmlInfo));
			strcpy(visitor.szName,outParas[0]);
			strcpy(visitor.szID,outParas[1]);
			strcpy(visitor.szReason,outParas[2]);
			outParas[3].Replace("#","//");
			strcpy(visitor.szUserVideoPic,outParas[3]);
			outParas[4].Replace("#","//");
			strcpy(visitor.szUserIDPic,outParas[4]);
			outParas[5].Replace("#","//");
			strcpy(visitor.szUserIDFullPic,outParas[5]);
			outParas[8].Replace("#","//");
			strcpy(visitor.szUserHeadPic,outParas[8]);
			visitor.lVisiteTime = atol(outParas[7]);

			strcpy(visitor.szSex,outParas[9]);
			strcpy(visitor.szFolk,outParas[10]);
			strcpy(visitor.szPhone,outParas[11]);
			strcpy(visitor.szUnit,outParas[12]);
			strcpy(visitor.szJob,outParas[13]);
			strcpy(visitor.szAddr,outParas[14]);
			
			strcpy(visitor.szWithGoods,outParas[15]);
			strcpy(visitor.szToRoom,outParas[16]);

			strcpy(visitor.szToOfficialName,outParas[17]);
			strcpy(visitor.szToOfficeName,outParas[18]);
			strcpy(visitor.szBarCode,outParas[19]);


			if(!getVisitor2Xml(g_szXmlInfo,visitor,(LPSTR)(LPCTSTR)outParas[6]))
			{
				//Messagebox(L"Memerlukan foto Pengunjung, baik \r\ndalam ukuran kecil atau Normal");
				goto erros;
			}
			

			memset(szT, 0, sizeof(szT));
			if(!getNodeXmlInfo(g_szXmlInfo,ip,"//visitor//ip"))
			{
				wcscpy(wBuf,L"No official IP!");
				goto popvisitor;
			}
			
			if(socket.Connect(Ask_Pic_Port,ip)!=0)
			{
				wcscpy(wBuf,L"PIC can not connect to official!");
				goto erros;
			}
			
			memset(szT, 0, sizeof(szT));
			if(!getNodeXmlInfo(g_szXmlInfo,szT,"//visitor//Pic"))
			{
				wcscpy(wBuf,L"No picture file!");
				//goto erros;
			}
			//CString cstr;
			//cstr.Replace("#", "\\");
			//memset(szT, 0, sizeof(szT));
			//strcpy(szT, cstr);
			if(!socket.SendFile(szT))	
			{
				wcscpy(wBuf,L"Send picture file failed!");
				goto try_again;
			}
			
			if(socket2.Connect(Ask_Txt_Port,ip)!=0)		
			{
				wcscpy(wBuf,L"TXT can not connect to official!");
				goto erros;
			}
			
			if(socket2.SendText(g_szXmlInfo)<0)
			{
				wcscpy(wBuf,L"Send message failed!");
				goto try_again;
			}
			
			memset(szT, 0, sizeof(szT));
			if(socket2.Recv(szT,30)<0)
			{
				wcscpy(wBuf,L"Recive timeout?");
				goto try_again;
			}
			else
			{
				if(strcmp(szT,"OK")==0)
				{
					//g_bPrintAfterAllow=true;
					
					//g_bAcceptVisite = true;
					wcscpy(wBuf,L"Request is allowed!");
					//��ӡƾ��
					GetBarCodePic(visitor.szBarCode);
					CreateReceiptTipsPB(visitor);
					goto popvisitor;
					
				}
				else if(strcmp(szT,"NO")==0)
				{
					wcscpy(wBuf,L"Request is refused!");
					//���ʱ��ܾ�������״̬Ϊ-1
					sprintf(szT, "update t_visitor set visitor_status = -1,visitor_calls = -1 where visitor_visitetime = %ld", visitor.lVisiteTime);
					ret = tAdo.execUpdateInsert(szT);
					if (ret != ADOOK)
					{
						XMessageBox(g_hWindow,L"���������˷���״̬visitor_status = -1,visitor_calls = -1ʱʧ��",L"��ʾ");
						//Messagebox(L"Gagal untuk mengupdate informasi \r\nkunjungan sekitar 2 jam");
						return false;
					}
					memset(szT, 0, sizeof(szT));
					ListTodayVistorsInfo();
					VisitorTotalInfo(szT);
				}
				else
				{
					//wcscpy(wBuf,L"δ������Ϣ��");
					S_2_W(wBuf,szT,128);
					goto try_again; // ����ʧ�ܣ������ۼƻ���
					
				}
				
			}
try_again:	
			//g_bRequestThreadIsExist = false;
			memset(szT, 0, sizeof(szT));
			sprintf(szT, "update t_visitor set visitor_calls = visitor_calls + 1 where visitor_visitetime = %ld", visitor.lVisiteTime);
			ret = tAdo.execUpdateInsert(szT);
			if (ret != ADOOK)
			{
				XMessageBox(g_hWindow,L"���������˷���״̬visitor_calls = visitor_calls + 1ʱʧ��",L"��ʾ");
				//Messagebox(L"Gagal untuk mengupdate informasi \r\nkunjungan sekitar 2 jam");
				return false;
			}
			memset(szT, 0, sizeof(szT));
			ListTodayVistorsInfo();
			VisitorTotalInfo(szT);
			continue;
			
erros:	
			//g_bRequestThreadIsExist = false;
			memset(szT, 0, sizeof(szT));
			sprintf(szT, "update t_visitor set visitor_calls = -1 where visitor_visitetime = %ld", visitor.lVisiteTime);
			ret = tAdo.execUpdateInsert(szT);
			if (ret != ADOOK)
			{
				XMessageBox(g_hWindow,L"���������˷���״̬visitor_calls = -1ʱʧ��",L"��ʾ");
				//Messagebox(L"Gagal untuk mengupdate informasi \r\nkunjungan sekitar 2 jam");
				return false;
			}
			memset(szT, 0, sizeof(szT));
			ListTodayVistorsInfo();
			VisitorTotalInfo(szT);
			continue;
			//XWnd_RedrawWnd(g_hWindow);
			
popvisitor:
			memset(szT, 0, sizeof(szT));
			sprintf(szT, "update t_visitor set visitor_calls = 99 where visitor_visitetime = %ld", visitor.lVisiteTime);
			ret = tAdo.execUpdateInsert(szT);
			if (ret != ADOOK)
			{
				XMessageBox(g_hWindow,L"���������˷���״̬visitor_calls = 99ʱʧ��",L"��ʾ");
				//Messagebox(L"Gagal untuk mengupdate informasi \r\nkunjungan sekitar 2 jam");
				return false;
			}
			memset(szT, 0, sizeof(szT));
			ListTodayVistorsInfo();
			VisitorTotalInfo(szT);
			continue;

	}

	
	}
	return 0;
}

BOOL AutoCheckJob(char *szJob)
{
	if(strlen(szJob)<1)
		return true;

	bool bDelete = false;
	char szT[4096] = {'\0'};
	long FldCount = 0, ret = 0;
	CString outParas[FLD_MAX];
	CWinAdo tAdo(sCONNSTR_MYSQL);

	ret = tAdo.OpenAdo();
	if (ret != ADOOK)
	{
		return false;
	}

	for (int i = 0; i < FLD_MAX; i++)
	{
		outParas[i].Empty();
	}

	sprintf(szT, "select job_weigth from t_job where job_name = \'%s\'", szJob);
	ret = tAdo.execSelect(szT, &FldCount, outParas);
	if (ret == ADOERR )
	{        
		return false;
	}
	else if(FldCount == 0 && outParas[0].IsEmpty())
	{
		//û���ҵ������ƣ���Ҫ���
		CWinAdo tAdo1(sCONNSTR_MYSQL);
		ret = tAdo1.OpenAdo();
		if (ret != ADOOK)
		{
			return false;
		}
		sprintf(szT, "insert into t_job (job_name)\
        values(\'%s\')", \
		szJob);
		ret = tAdo1.execUpdateInsert(szT);
		if (ret != ADOOK)
		{
			return false;
		}
		bDelete = true;  //������¹�˾��ע����Ҫɾ���ϵĹ�˾
	}
	else
	{
		//�ҵ�ͬ����λ����Ҫ��Ȩ
		CWinAdo tAdo1(sCONNSTR_MYSQL);
		ret = tAdo1.OpenAdo();
		if (ret != ADOOK)
		{
			return false;
		}
		sprintf(szT, "update t_job set job_weigth = %d \
        where job_name = \'%s\'", \
		atol(outParas[0])+1,szJob);
		ret = tAdo1.execUpdateInsert(szT);
		if (ret != ADOOK)
		{
			return false;
		}
	}
	//MessageBox(NULL,szT,"����",MB_ICONEXCLAMATION);
	if(bDelete)
	{
		int count = 0;
		int id = -1;
		sprintf(szT, "select job_id from t_job order by job_id asc");
		ret = tAdo.execSelect(szT, &FldCount, outParas);
		if (ret == ADOERR )
		{        
			return false;
		}
		id = atol(outParas[0]);
		do
		{
			count ++ ;
		}while(tAdo.getNext(outParas) == ADOOK);

		if(count > 8)
		{
			//ɾ��������
			CWinAdo tAdo1(sCONNSTR_MYSQL);
			ret = tAdo1.OpenAdo();
			if (ret != ADOOK)
			{
				return false;
			}
			sprintf(szT, "delete from t_job where job_id = %d", id);
			ret = tAdo1.execUpdateInsert(szT);
			if (ret != ADOOK)
			{
				return false;
			}
		}
	}

	return true;
}

BOOL AutoCheckCompanyName(char *szCompany)
{
	if(strlen(szCompany)<1)
		return true;

	bool bDelete = false;
	char szT[4096] = {'\0'};
	long FldCount = 0, ret = 0;
	CString outParas[FLD_MAX];
	CWinAdo tAdo(sCONNSTR_MYSQL);

	ret = tAdo.OpenAdo();
	if (ret != ADOOK)
	{
		return false;
	}

	for (int i = 0; i < FLD_MAX; i++)
	{
		outParas[i].Empty();
	}

	sprintf(szT, "select company_weigth from t_company where company_name = \'%s\'", szCompany);
	ret = tAdo.execSelect(szT, &FldCount, outParas);
	if (ret == ADOERR )
	{        
		return false;
	}
	else if(FldCount == 0 && outParas[0].IsEmpty())
	{
		//û���ҵ������ƣ���Ҫ���
		CWinAdo tAdo1(sCONNSTR_MYSQL);
		ret = tAdo1.OpenAdo();
		if (ret != ADOOK)
		{
			return false;
		}
		sprintf(szT, "insert into t_company (company_name)\
        values(\'%s\')", \
		szCompany);
		ret = tAdo1.execUpdateInsert(szT);
		if (ret != ADOOK)
		{
			return false;
		}
		bDelete = true;  //������¹�˾��ע����Ҫɾ���ϵĹ�˾
	}
	else
	{
		//�ҵ�ͬ����λ����Ҫ��Ȩ
		CWinAdo tAdo1(sCONNSTR_MYSQL);
		ret = tAdo1.OpenAdo();
		if (ret != ADOOK)
		{
			return false;
		}
		sprintf(szT, "update t_company set company_weigth = %d \
        where company_name = \'%s\'", \
		atol(outParas[0])+1,szCompany);
		ret = tAdo1.execUpdateInsert(szT);
		if (ret != ADOOK)
		{
			return false;
		}
	}
	//MessageBox(NULL,szT,"����",MB_ICONEXCLAMATION);
	if(bDelete)
	{
		int count = 0;
		int id = -1;
		sprintf(szT, "select company_id from t_company order by company_id asc");
		ret = tAdo.execSelect(szT, &FldCount, outParas);
		if (ret == ADOERR )
		{        
			return false;
		}
		id = atol(outParas[0]);
		do
		{
			count ++ ;
		}while(tAdo.getNext(outParas) == ADOOK);

		if(count > 8)
		{
			//ɾ��������
			CWinAdo tAdo1(sCONNSTR_MYSQL);
			ret = tAdo1.OpenAdo();
			if (ret != ADOOK)
			{
				return false;
			}
			sprintf(szT, "delete from t_company where company_id = %d", id);
			ret = tAdo1.execUpdateInsert(szT);
			if (ret != ADOOK)
			{
				return false;
			}
		}
	}


	return true;
}
//����������Ϣ�Ǽ������ݿ�
BOOL SaveVisitorInfo(VISITORINFO &visitor)
{
	char szT[4096] = {'\0'};
	long FldCount = 0, ret = 0;
	//CString outParas[FLD_MAX];
	CWinAdo tAdo(sCONNSTR_MYSQL);

	ret = tAdo.OpenAdo();
	if (ret != ADOOK)
	{
		return false;
	}

	/*
	for (int i = 0; i < FLD_MAX; i++)
	{
		outParas[i].Empty();
	}
	*/

	sprintf(szT, "insert into t_visitor (visitor_name,visitor_sex,visitor_folk,visitor_phone,visitor_idtype,visitor_id,visitor_signdepartment,visitor_withgoods,visitor_reason,\
                  visitor_addr,visitor_totalpeople,visitor_unit,visitor_carnum,visitor_barcode,visitor_to_official_name,visitor_to_office_name,visitor_visitetime,visitor_lefttime,\
                  visitor_station_id,visitor_user_video_pic,visitor_user_id_pic,visitor_user_head_pic,visitor_validdate,visitor_id2_readtype,visitor_birth,visitor_status,visitor_user_idfull_pic,\
				  visitor_job,visitor_type)\
        values(\'%s\', \'%s\', \'%s\', \'%s\', \'%s\', \'%s\', \'%s\', \'%s\', \'%s\', \'%s\', \'%s\', \'%s\', \'%s\', \'%s\', \'%s\', \'%s\', %ld, %ld, \'%s\', \'%s\', \'%s\', \'%s\', \'%s\', \'%s\', \'%s\',\
		%d, \'%s\', \'%s\', \'%s\')", \
		visitor.szName, visitor.szSex, visitor.szFolk, visitor.szPhone, visitor.szIDType, visitor.szID, visitor.szSignDepartment,\
		visitor.szWithGoods, visitor.szReason, visitor.szAddr, visitor.szTotalPeople, visitor.szUnit, visitor.szCarNum,\
		visitor.szBarCode, visitor.szToOfficialName, visitor.szToOfficeName, visitor.lVisiteTime, visitor.lLeftTime, visitor.szVisitStation, visitor.szUserVideoPic,\
		visitor.szUserIDPic, visitor.szUserHeadPic, visitor.szValidDate, visitor.szID2Flag, visitor.szBirth, visitor.nStatus,visitor.szUserIDFullPic,\
		visitor.szJob,visitor.szType);

	//MessageBox(NULL,szT,"����",MB_ICONEXCLAMATION);

	ret = tAdo.execUpdateInsert(szT);
	if (ret != ADOOK)
	{
		return false;
	}

	return true;
}

BOOL IsDirExist(const char *pszPath)
{
	BOOL bRet = FALSE;
	char szCurrentDiretory[512] = {'\0'};

	GetCurrentDirectory(512, szCurrentDiretory);
	bRet = SetCurrentDirectory(pszPath);
	SetCurrentDirectory(szCurrentDiretory);
	
	return bRet;
}

void MakeFolder(const char *pszFolder)
{
	BOOL bRet = FALSE;
	
	bRet = IsDirExist(pszFolder);
	if (!bRet)
	{
		::CreateDirectory(pszFolder, NULL);
	}
	return;
}

//����������ļ���
void CreateWorkFolder()
{
	char szT[256] = {'\0'}, szYear[8] = {'\0'}, szMon[8] = {'\0'}, szDay[8] = {'\0'};

	MakeFolder(".\\pic");		//����û���Ƭ���ļ���
	MakeFolder(".\\send");

	GetDate(szT);
	memcpy(szYear, szT, 4);			//��
	memcpy(szMon, szT + 4, 2);		//��
	memcpy(szDay, szT + 4 + 2, 2);	//��

	memset(szT, 0, sizeof(szT));
	sprintf(szT, ".\\pic\\%s%s", szYear, szMon);
	MakeFolder(szT);			//���½����ļ���

	sprintf(szT, ".\\pic\\%s%s\\%s", szYear, szMon, szDay);
	MakeFolder(szT);			//���ս����ļ���

	return;
}

void GetDate(char *pszDate)
{
	struct tm *tim = NULL;
	long i = 0;
	time((time_t *)&i);
	tim = localtime((time_t *)&i);
	sprintf(pszDate, "%04ld%02ld%02ld", tim->tm_year + 1900, tim->tm_mon + 1, tim->tm_mday);
	return;
}

void GetDate_Out(char *pszDate)
{
	struct tm *tim = NULL;
	long i = 0;
	time((time_t *)&i);
	tim = localtime((time_t *)&i);
	sprintf(pszDate, "%04ld-%02ld-%02ld", tim->tm_year + 1900, tim->tm_mon + 1, tim->tm_mday);
	return;
}

void GetDayFolder(char *pszPath)
{
	char szT[256] = {'\0'}, szYear[8] = {'\0'}, szMon[8] = {'\0'}, szDay[8] = {'\0'};

	GetDate(szT);
	memcpy(szYear, szT, 4);			//��
	memcpy(szMon, szT + 4, 2);		//��
	memcpy(szDay, szT + 4 + 2, 2);	//��

	sprintf(szT, ".\\pic\\%s%s", szYear, szMon);
	MakeFolder(szT);			//���½����ļ���

	if (g_bFtpOpened)
	{
		m_pFtpConnection->CreateDirectory(szT);            //Ϊftp�����ļ���
	}
	

	sprintf(szT, ".\\pic\\%s%s\\%s", szYear, szMon, szDay);
	MakeFolder(szT);			//���ս����ļ���
	if (g_bFtpOpened)
	{
		m_pFtpConnection->CreateDirectory(szT);            //Ϊftp�����ļ���
	}
    

	sprintf(pszPath, ".\\pic\\%s%s\\%s", szYear, szMon, szDay);

	return;
}

//�����ݿ���û�еı�������Ϣ�Ǽ������ݿ�
BOOL SaveOfficialInfo(OFFICIALINFO &official)
{
	char szT[4096] = {'\0'};
	long FldCount = 0, ret = 0;;
	CString outParas[FLD_MAX];	
	CWinAdo tAdo(sCONNSTR_MYSQL);
	
	ret = tAdo.OpenAdo();
	if (ret != ADOOK)
	{
		return false;
	}
	
	for (int i = 0; i < FLD_MAX; i++)
	{
		outParas[i].Empty();
	}
	
	sprintf(szT, "insert into t_official values(\'%s\', \'%s\', \'%s\', \'%s\', \'%s\', \'%s\', \'%s\')", \
			official.szName, official.szSex, official.szOfficePhone, official.szMobile, official.szOfficeName,\
			official.szOfficeAddr, official.szOfficialDuty);
	ret = tAdo.execUpdateInsert(szT);
	if (ret != ADOOK)
	{
		return false;
	}
	
	return true;
}

//�ÿ�ǩ��
BOOL VisitorLogOut(char *pszBarCode2)
{
	char szT[4096] = {'\0'}, szVisitorName[32] = {'\0'};
	long FldCount = 0, ret = 0,lefttime;
	wchar_t wBuf[256] = {0};
	CString outParas[FLD_MAX],outParasGo[FLD_MAX];	
	CWinAdo tAdo(sCONNSTR_MYSQL);

	ret = tAdo.OpenAdo();
	if (ret != ADOOK)
	{
		return false;
	}

	for (int i = 0; i < FLD_MAX; i++)
	{
		outParas[i].Empty();
		outParasGo[i].Empty();
	}

	char pszBarCode[4096] = {'\0'};
	::GetPrivateProfileString("Bar", "BarNumber", "",pszBarCode,sizeof(pszBarCode), CONFIG_FILE);
	
	char szTime[40] = {'\0'};
	time((time_t *)&lefttime);
	sprintf(szTime, "%ld", lefttime);
	CallLeft2Server("","","",szTime,pszBarCode); //Զ��ǩ�����

	HideSoftKeybd();
	sprintf(szT, "select * from t_visitor where visitor_barcode = \'%s\'", pszBarCode);
	ret = tAdo.execSelect(szT, &FldCount, outParas);
	if (ret == ADOERR || (FldCount == 0 && outParas[0].IsEmpty()))
	{

		memset(szT, 0, sizeof(szT));
		sprintf(szT, "û�и÷ÿ͵�������Ϣ \r\n�����:%s", pszBarCode);
        S_2_W(wBuf, szT, 256);
		XMessageBox(g_hWindow, wBuf, L"��ʾ");
		return false;
	}
    FldCount = 0;
	sprintf(szT, "select * from t_visitor where visitor_barcode = \'%s\' and visitor_lefttime > 0 and visitor_status>1", pszBarCode);
	ret = tAdo.execSelect(szT, &FldCount, outParasGo);
	if (!outParasGo[0].IsEmpty())
	{
		/*wchar_t wBuf[256] = {0};
		memset(szT, 0, sizeof(szT));
		sprintf(szT, "[%s] �Ѿ�ǩ��", outParas[0]);
		S_2_W(wBuf, szT, 256);
		XMessageBox(g_hWindow, wBuf, L"��ʾ");*/
		//�ÿ���ǩ���
		return false;
	}
	
	sprintf(szT, "update t_visitor set visitor_lefttime = %ld where visitor_barcode = '\%s\'", lefttime, pszBarCode);
	ret = tAdo.execUpdateInsert(szT);
	if (ret == ADOOK)
	{		
		memset(szT, 0, sizeof(szT));
		sprintf(szT, "[%s] \r\nͨ������ǩ��:%s", outParas[0],pszBarCode);
		S_2_W(wBuf, szT, 256);
		//SetInfo(IDS_DEVINFO, wBuf);
		//XMessageBox(g_hWindow, wBuf, L"��ʾ");
		::DeleteFile(USER_HEAD_PIC);            //ɾ���ϴ�ͷ��
	    ::DeleteFile(USER_ID_PIC); 
	    ::DeleteFile(USER_VIDEO_PIC);
	    ::DeleteFile(USER_BARCODE_PIC);
		g_hLogoutWindow = XWnd_CreateWindow(200, 150,200, 150,L"��ʾ",XWnd_GetHWnd(g_hWindow),XC_SY_CAPTION | XC_SY_BORDER | 	XC_SY_ROUND | 	XC_SY_CENTER | XC_SY_DRAG_BORDER | XC_SY_DRAW_CAPTION_ICON | XC_SY_DRAW_CAPTION_TITLE);
				
		::SetWindowPos(XWnd_GetHWnd(g_hLogoutWindow), XWnd_GetHWnd(g_hWindow), 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
		XStatic_Create(15,40,180,40,wBuf,g_hLogoutWindow);
		XWnd_SetBorderSize(g_hLogoutWindow, 3, 3, 3, 3);
		SetTimer(XWnd_GetHWnd(g_hWindow),1,2000,TimerMsgBox);
		//XModalWnd_DoModal(g_hLogoutWindow);
		XWnd_ShowWindow(g_hLogoutWindow,SW_SHOW);

		ListTodayVistorsInfo();		//ˢ����������Ϣ���
		VisitorTotalInfo(szT);
	}

	return (ret == ADOOK ? true : false);
}

//���յ�������������Ϣ
void VisitorTotalInfo(char *pszInfo)
{
	char szT[4096] = {'\0'};
	long FldCount = 0, ret = 0;
	CString outParas[FLD_MAX];	
	CWinAdo tAdo(sCONNSTR_MYSQL);
	
	ret = tAdo.OpenAdo();
	if (ret != ADOOK)
	{
		return;
	}
	
	for (int i = 0; i < FLD_MAX; i++)
	{
		outParas[i].Empty();
	}
	
	char szTotal[16] = {'\0'}, szLeft[16] = {'\0'}, szNoLeft[16] = {'\0'};

	//�����ܵ�������
	//strcpy(szT, "select count(*) from t_visitor where TO_DAYS(NOW()) - TO_DAYS(FROM_UNIXTIME(visitor_visitetime)) = 0");
	//sprintf(szT, "select count(*) from t_visitor where TO_DAYS(NOW()) - TO_DAYS(FROM_UNIXTIME(visitor_visitetime)) = 0 and visitor_status >1 and visitor_station_id = \'%s\'",g_szStation);
	if (g_nSqlConnectType == SQL_CONNECT_SERVER2005)
	{
	    sprintf(szT, "select count(*) from t_visitor where DateDiff(d, getdate(),DATEADD(s,visitor_visitetime,'1970-01-01 08:00:00'))=0 and visitor_status >1 and visitor_station_id = \'%s\'",g_szStation);
	}
	else
	    sprintf(szT, "select count(*) from t_visitor where TO_DAYS(NOW()) - TO_DAYS(FROM_UNIXTIME(visitor_visitetime)) = 0 and visitor_status >1 and visitor_station_id = \'%s\'",g_szStation);

	ret = tAdo.execSelect(szT, &FldCount, outParas);
	if (ret == ADOERR || (FldCount == 0 && outParas[0].IsEmpty()))
	{
		return;
	}
	strcpy(szTotal, outParas[0]);

	//�����Ѿ�ǩ������
	//strcpy(szT, "select count(*) from t_visitor where (TO_DAYS(NOW()) - TO_DAYS(FROM_UNIXTIME(visitor_visitetime)) = 0) and visitor_lefttime > 0 and visitor_station_id = \'%s\'",g_szStation);
	//sprintf(szT, "select count(*) from t_visitor where (TO_DAYS(NOW()) - TO_DAYS(FROM_UNIXTIME(visitor_visitetime)) = 0) and visitor_lefttime > 0 and visitor_station_id = \'%s\'",g_szStation);
	if (g_nSqlConnectType == SQL_CONNECT_SERVER2005)
	{
	    sprintf(szT, "select count(*) from t_visitor where DateDiff(d, getdate(),DATEADD(s,visitor_visitetime,'1970-01-01 08:00:00'))=0 and visitor_lefttime > 0 and visitor_station_id = \'%s\'",g_szStation);
	}
	else
	    sprintf(szT, "select count(*) from t_visitor where (TO_DAYS(NOW()) - TO_DAYS(FROM_UNIXTIME(visitor_visitetime)) = 0) and visitor_lefttime > 0 and visitor_station_id = \'%s\'",g_szStation);

	ret = tAdo.execSelect(szT, &FldCount, outParas);
	if (ret == ADOERR || (FldCount == 0 && outParas[0].IsEmpty()))
	{
		return;
	}
	strcpy(szLeft, outParas[0]);

	//����δǩ������
	//strcpy(szT, "select count(*) from t_visitor where (TO_DAYS(NOW()) - TO_DAYS(FROM_UNIXTIME(visitor_visitetime)) = 0) and visitor_status >1 and visitor_lefttime <= 0");
	//sprintf(szT, "select count(*) from t_visitor where (TO_DAYS(NOW()) - TO_DAYS(FROM_UNIXTIME(visitor_visitetime)) = 0) and visitor_status >1 and visitor_lefttime <= 0 and visitor_station_id = \'%s\'",g_szStation);
	if (g_nSqlConnectType == SQL_CONNECT_SERVER2005)
	{
	    sprintf(szT, "select count(*) from t_visitor where DateDiff(d, getdate(),DATEADD(s,visitor_visitetime,'1970-01-01 08:00:00'))=0 and visitor_status >1 and visitor_lefttime <= 0 and visitor_station_id = \'%s\'",g_szStation);
	}
	else
	    sprintf(szT, "select count(*) from t_visitor where (TO_DAYS(NOW()) - TO_DAYS(FROM_UNIXTIME(visitor_visitetime)) = 0) and visitor_status >1 and visitor_lefttime <= 0 and visitor_station_id = \'%s\'",g_szStation);

	ret = tAdo.execSelect(szT, &FldCount, outParas);
	if (ret == ADOERR || (FldCount == 0 && outParas[0].IsEmpty()))
	{
		return;
	}
	strcpy(szNoLeft, outParas[0]);

	sprintf(pszInfo, "%-100s�����ܵ��� [%s] ��, ��ǩ�� [%s] ��, δǩ�� [%s] ��", "��������Ϣ", szTotal, szLeft, szNoLeft);

	wchar_t wBuf[256] = {'\0'};
	HELE hE = XWnd_GetEle(g_hWindow, IDG_VISTOR);
	S_2_W(wBuf, pszInfo, 256);
	XGBox_SetText(hE, wBuf);

	return;
}

//ĳ���ܵĵ��ô���
void VisitorTotalTimes(char *pszName, char *pszOut)
{
	char szT[4096] = {'\0'};
	long FldCount = 0, ret = 0;;
	CString outParas[FLD_MAX];	
	CWinAdo tAdo(sCONNSTR_MYSQL);

	ret = tAdo.OpenAdo();
	if (ret != ADOOK)
	{
		return;
	}
	
	for (int i = 0; i < FLD_MAX; i++)
	{
		outParas[i].Empty();
	}

	//sprintf(szT, "select count(*) from t_visitor where visitor_name = \'%s\' and visitor_status >1 and TO_DAYS(NOW()) - TO_DAYS(FROM_UNIXTIME(visitor_visitetime)) <= 365", pszName);
	if (g_nSqlConnectType == SQL_CONNECT_SERVER2005)
	{
	    sprintf(szT, "select count(*) from t_visitor where  visitor_name = \'%s\' and visitor_status >1 and DateDiff(d, DATEADD(s,visitor_visitetime,'1970-01-01 08:00:00'),getdate())<= 365",pszName);
	}
	else
	    sprintf(szT, "select count(*) from t_visitor where visitor_name = \'%s\' and visitor_status >1 and TO_DAYS(NOW()) - TO_DAYS(FROM_UNIXTIME(visitor_visitetime)) <= 365", pszName);

	ret = tAdo.execSelect(szT, &FldCount, outParas);
	if (ret == ADOERR || (FldCount == 0 && outParas[0].IsEmpty()))
	{
		return;
	}
	strcpy(pszOut, outParas[0]);

	wchar_t wBuf[256 + 1] = {0};
	HELE hE = XWnd_GetEle(g_hWindow, IDE_VISITTIMES);
	XEdit_SetReadOnly(hE,false);
	memset((char *)wBuf, 0, sizeof(wBuf));
	S_2_W(wBuf, pszOut, 256);
	XEdit_SetText(hE, wBuf);

	if (::GetPrivateProfileInt("TerminalSet", "bVisitorReadOnly",0, CONFIG_FILE)==1)  //���õǼ�֤������ֻ��
	   SetVisitorReadOnly(true);
	else
	   SetVisitorReadOnly(false);

	return;
}

DWORD WINAPI FlashFrameThread(LPVOID lpVoid)
{
	XEle_SetBorderColor(GETELE(IDP_CAMERA),RGB(255, 0, 0));
	XEle_EnableBorder(GETELE(IDP_CAMERA),true);
	XWnd_RedrawWnd(XEle_GetHWindow(GETELE(IDP_CAMERA)));
    wchar_t wBuf[256 + 1] = {0};
	wcscpy(wBuf,(wchar_t *)lpVoid);
	char *pcstr = (char *)new char[2 * wcslen(wBuf) + 1];
	WideCharToMultiByte(CP_ACP, 0, wBuf, -1, pcstr, 2 * wcslen(wBuf) + 1, NULL, NULL);	
	
	//b = capFileSaveDIB(g_hCapWnd, pcstr);
	TakePhoto(pcstr);
	delete []pcstr;
	//Sleep(500);
	//TakePhoto("C:\\123.JPG");
    XEle_SetBorderColor(GETELE(IDP_CAMERA),RGB(0, 0, 0));
	XEle_EnableBorder(GETELE(IDP_CAMERA),false);
	XWnd_RedrawWnd(XEle_GetHWindow(GETELE(IDP_CAMERA)));
	return 0;
}
//�����Ķ��߳�
DWORD WINAPI ReadBarCodeThread(LPVOID lpVoid)
{
	CBarCode tBarCode,tInfrared;
	int iBarCom,iInfrareCom;
	long ret = 0;
	char szT[FLD_MAX] = {'\0'},szBarPort[4]={'\0'},szInfrarePort[4]={'\0'};
	BOOL bRet = false;
	//static char szPreBarCode[FLD_MAX] = {'\0'};

		::GetPrivateProfileString("BarCode", "InfrarePort", "3", szInfrarePort, sizeof(szInfrarePort), CONFIG_FILE);
		iInfrareCom=atoi(szInfrarePort);
		bRet = tInfrared.OpenBarCode(iInfrareCom);
		if (!bRet)
		{
			XMessageBox(g_hWindow, L"�����Ӧ��ʧ�ܣ��������������Ƿ���ȷ", L"����");
			return 0;
		}

		::GetPrivateProfileString("BarCode", "Port", "9", szBarPort, sizeof(szBarPort), CONFIG_FILE);
		iBarCom=atoi(szBarPort);
		bRet = tBarCode.OpenBarCode(iBarCom);
		if (!bRet)
		{
			XMessageBox(g_hWindow, L"ɨ��ǹ��ʧ�ܣ��������������Ƿ���ȷ", L"����");
		}
		tBarCode.CloseBarCode();
	
	while (!g_bStopBarCode)
	{
		bRet = tInfrared.InfraredCode();
		if (bRet)
		{
			tBarCode.OpenBarCode(iBarCom);
			memset(szT, 0, sizeof(szT));
			bRet = tBarCode.ReadBarCode(szT);
            if (bRet)
			{
				::WritePrivateProfileString("Bar", "BarNumber", szT, CONFIG_FILE);
				::PostMessage(XWnd_GetHWnd(g_hWindow), WM_VISITORLOGOUT, NULL, (LPARAM)szT);
			}
		    tBarCode.CloseBarCode();			
		}
		tInfrared.OpenBarCode(iInfrareCom);		
	}
	bRet = tBarCode.CloseBarCode();

	return 0;
}

DWORD WINAPI ReadBarCodeThread2(LPVOID lpVoid)
{
	CBarCode tBarCode;
	int iBarCom;
	char szT[FLD_MAX] = {'\0'},szBarPort[4]={'\0'};
	BOOL bRet = false;

	::GetPrivateProfileString("BarCode", "Port", "9", szBarPort, sizeof(szBarPort), CONFIG_FILE);
	iBarCom=atoi(szBarPort);
	bRet = tBarCode.OpenBarCode(iBarCom);
	if (!bRet)
	{
		XMessageBox(g_hWindow, L"ɨ��ǹ��ʧ�ܣ��������������Ƿ���ȷ", L"����");
		return bRet;
	}
	
	while (!g_bStopBarCode)
	{
		memset(szT, 0, sizeof(szT));
		bRet = tBarCode.ReadBarCode2(szT);
		if (bRet)
		{		
			memset(g_szComBarcode, 0, sizeof(g_szComBarcode));
			strcpy(g_szComBarcode,szT);
			::WritePrivateProfileString("Bar", "BarNumber", szT, CONFIG_FILE);
			::PostMessage(XWnd_GetHWnd(g_hWindow), WM_VISITORLOGOUT, NULL, (LPARAM)szT);			
		}
		Sleep(1);
	}
	bRet = tBarCode.CloseBarCode();

	return 0;
}

HBITMAP CopyScreenToBitmap(LPRECT lpRect)
{
	HDC hScreenDC = NULL, hMemDC = NULL;
	HBITMAP hBitmap = NULL, hOldBitmap = NULL; 
	int x1 = 0, x2 = 0, y1 = 0, y2 = 0;
	int nWidth = 0, nHeight = 0;
	int nScreenWidth = 0, nScreenHeight = 0;
	
	if (IsRectEmpty(lpRect))
	{
		return NULL;
	}
	x1 = lpRect->left;
	x2 = lpRect->right;
	y1 = lpRect->top;
	y2 = lpRect->bottom;
	nScreenWidth = GetSystemMetrics(SM_CXSCREEN);
	nScreenHeight = GetSystemMetrics(SM_CYSCREEN);
	
	if (x1 < 0)
	{
		x1 = 0;
	}
	if (x2 > nScreenWidth)
	{
		x2 = nScreenWidth;
	}
	if (y1 < 0)
	{
		y1 = 0;
	}
	if (y2 > nScreenHeight)
	{
		y2 = nScreenHeight;
	}
	nWidth = x2 - x1;
	nHeight = y2 - y1;
	hScreenDC = CreateDC(_T("DISPLAY"), NULL, NULL, NULL);
	hMemDC = CreateCompatibleDC(hScreenDC);
	hBitmap = CreateCompatibleBitmap(hScreenDC, nWidth, nHeight);
	hOldBitmap = (HBITMAP)SelectObject(hMemDC, hBitmap);
	BitBlt(hMemDC, 0, 0, nWidth, nHeight, hScreenDC, x1, y1, SRCCOPY);
	hBitmap = (HBITMAP)SelectObject(hMemDC, hOldBitmap);
	DeleteDC(hMemDC);
	DeleteDC(hScreenDC);
	return hBitmap;
}

HBITMAP CopyDCToBitmap(LPRECT lpRect, HDC hSrcDC)
{
	HDC hMemDC;
	HBITMAP hDestBitmap, hOldBitmap; 
	int x1 = 0, x2 = 0, y1 = 0, y2 = 0;
	int nWidth = 0, nHeight = 0;
	int nScreenWidth = 0, nScreenHeight = 0;

	if (IsRectEmpty(lpRect))
	{
		return NULL;
	}
	x1 = lpRect->left;
	x2 = lpRect->right;
	y1 = lpRect->top;
	y2 = lpRect->bottom;

	nScreenWidth = GetSystemMetrics(SM_CXSCREEN);
	nScreenHeight = GetSystemMetrics(SM_CYSCREEN);
	
	if (x1 < 0)
	{
		x1 = 0;
	}
	if (x2 > nScreenWidth)
	{
		x2 = nScreenWidth;
	}
	if (y1 < 0)
	{
		y1 = 0;
	}
	if (y2 > nScreenHeight)
	{
		y2 = nScreenHeight;
	}
	nWidth = x2 - x1;
	nHeight = y2 - y1;

	hMemDC = CreateCompatibleDC(hSrcDC);
	hDestBitmap = CreateCompatibleBitmap(hSrcDC, nWidth, nHeight);
	hOldBitmap = (HBITMAP)SelectObject(hMemDC, hDestBitmap);
	BitBlt(hMemDC, 0, 0, nWidth, nHeight, hSrcDC, x1, y1, SRCCOPY);
	hDestBitmap = (HBITMAP)SelectObject(hMemDC, hOldBitmap);
	DeleteDC(hMemDC);
	return hDestBitmap;
}

void SetHeadPic(short n)
{
	HELE hE = XWnd_GetEle(g_hWindow, IDP_HEADPIC);
	if (n == PIC_BLANK)
	{
		XPic_SetImage(hE, NULL);
		return;
	}
	wchar_t wBuf[256] = {0};
	S_2_W(wBuf, USER_HEAD_PIC, 256);
	HIMAGE hImg = XImage_LoadFile(wBuf, true);
	XPic_SetImage(hE, hImg);
	return;
}

void SetSysInfo()
{
	long ret = 0;
	struct tm *tim = NULL;	
    int bSend;
	time((time_t *)&ret);
	tim = localtime((time_t *)&ret);
	char szT[256]= {'\0'}, szUnitName[128] = {'\0'}, szStation[32] = {'\0'};
	char szUnitAddr[128] = {'\0'}, szUnitPhone[32] = {'\0'}, szUnitFax[32] = {'\0'},szUserName[20] = {'\0'};
	wchar_t wBuf[256] = {0};

	::GetPrivateProfileString("TerminalSet", "UnitName", "", szUnitName, sizeof(szUnitName), CONFIG_FILE);
	::GetPrivateProfileString("TerminalSet", "UnitAddr", "", szUnitAddr, sizeof(szUnitAddr), CONFIG_FILE);
	::GetPrivateProfileString("TerminalSet", "UnitPhone", "", szUnitPhone, sizeof(szUnitPhone), CONFIG_FILE);
	::GetPrivateProfileString("TerminalSet", "StationName", "", szStation, sizeof(szStation), CONFIG_FILE);
    ::GetPrivateProfileString("TerminalSet", "UserName", "", szUserName, sizeof(szUserName), CONFIG_FILE);
	bSend=::GetPrivateProfileInt("TerminalSet", "bSendRequest",0, CONFIG_FILE);
	if (bSend==1)  //�򿪷�����Ƶ���ܣ���ʼΪ�����ܷ���״̬
	{
		g_bAcceptVisite=false;
	}
	else
		XEle_EnableEle(g_hTab3, false);
	sprintf(szT, "��λ: %s  ��ַ: %s  �绰: %s  �Ÿ�: %s  ����: %04ld��%02ld��%02ld��  ��¼��: %s", szUnitName, szUnitAddr, szUnitPhone, szStation, tim->tm_year + 1900, tim->tm_mon + 1, tim->tm_mday,szUserName);
	HELE hE = XWnd_GetEle(g_hWindow, IDS_SYSINFO);
	S_2_W(wBuf, szT, 256);
	XStatic_SetText(hE, wBuf);

	//XWnd_GetEle(g_hWindow, IDE_VISITORTOTAL);		//�ÿ�����
	//XEdit_EnableNumber(XWnd_GetEle(g_hWindow, IDE_VISITORTOTAL),true);
    XEdit_EnableNumber(XWnd_GetEle(g_hWindow, IDE_MASTERMOBILE),true);
    XEdit_EnableNumber(XWnd_GetEle(g_hWindow, IDE_ADD_OFFICIALMOBILE),true);
	return;
}

/*
BOOL PrintReceipt(const VISITORINFO &visitor)
{
	long ret = 0;
	char szT[128] = {'\0'};
	struct tm *tim = NULL;

	::GetPrivateProfileString("ReceiptPrinter", "Port", "0", szT, sizeof(szT), CONFIG_FILE);
	ret = RP_InitPrinter(atol(szT));
	ret = RP_StatusPrinter();
	if (ret != 0)
	{
		if (ret == 4010)
		{
			SetInfo(IDS_DEVINFO, L"��ӡ��ȱֽ");
			ret = RP_ClosePrinter();
			return false;
		}
		if (ret == 4011)
		{
			SetInfo(IDS_DEVINFO, L"��ӡ����ֽ������");
		}
		else
		{
			SetInfo(IDS_DEVINFO, L"��ӡ����������");
			ret = RP_ClosePrinter();
			return false;
		}
	}
	ret = RP_PrintLine(1 | 2, "         ��͵�");
	ret = RP_PrintLine(0, " ");

	time(&ret);
	tim = localtime(&ret);
	sprintf(szT, "����ʱ��: %04ld-%02ld-%02ld %02ld:%02ld:%02ld", tim->tm_year + 1900, tim->tm_mon + 1, tim->tm_mday,\
		tim->tm_hour, tim->tm_min, tim->tm_sec);
	ret = RP_PrintLine(0, szT);
	ret = RP_PrintLine(0, "�����������ש�����������������������������������");
	sprintf(szT, "���ÿ�������%-34s��", visitor.szName);
	ret = RP_PrintLine(0, szT);
	ret = RP_PrintLine(0, "�ǩ��������贈����������������������������������");
	sprintf(szT, "����    ��%-34s��", visitor.szSex);
	ret = RP_PrintLine(0, szT);
	ret = RP_PrintLine(0, "�ǩ��������贈����������������������������������");
	sprintf(szT, "����    �婧%-34s��", visitor.szFolk);
	ret = RP_PrintLine(0, szT);
	ret = RP_PrintLine(0, "�ǩ��������贈����������������������������������");
	sprintf(szT, "����    ����%-34s��", visitor.szPhone);
	ret = RP_PrintLine(0, szT);
	ret = RP_PrintLine(0, "�ǩ��������贈����������������������������������");
	sprintf(szT, "������������%-34s��", visitor.szTotalPeople);
	ret = RP_PrintLine(0, szT);
	ret = RP_PrintLine(0, "�ǩ��������贈����������������������������������");
	sprintf(szT, "���������ɩ�%-34s��", visitor.szReason);
	ret = RP_PrintLine(0, szT);
	ret = RP_PrintLine(0, "�ǩ��������贈����������������������������������");
	sprintf(szT, "��Я����Ʒ��%-34s��", visitor.szWithGoods);
	ret = RP_PrintLine(0, szT);
	ret = RP_PrintLine(0, "�ǩ��������贈����������������������������������");
	sprintf(szT, "����    λ��%-34s��", visitor.szUnit);
	ret = RP_PrintLine(0, szT);
	ret = RP_PrintLine(0, "�ǩ��������贈����������������������������������");
	sprintf(szT, "��֤�����멧%-34s��", visitor.szID);
	ret = RP_PrintLine(0, szT);
	ret = RP_PrintLine(0, "�ǩ��������贈����������������������������������");
	sprintf(szT, "�����ò��ũ�%-34s��", visitor.szToOfficeName);
	ret = RP_PrintLine(0, szT);
	ret = RP_PrintLine(0, "�ǩ��������贈����������������������������������");
	sprintf(szT, "��������Ա��%-34s��", visitor.szToOfficialName);
	ret = RP_PrintLine(0, szT);
	ret = RP_PrintLine(0, "�ǩ��������贈����������������������������������");
	sprintf(szT, "���� �� �ũ�%-34s��", visitor.szToRoom);
	ret = RP_PrintLine(0, szT);
	ret = RP_PrintLine(0, "�����������ߩ�����������������������������������");
	ret = RP_PrintLine(0, "  ������ǩ��");
	ret = RP_PrintLine(0, " ");
	ret = RP_PrintBarCode(0, (char *)visitor.szBarCode);
	ret = RP_PrintLine(0, "*�����Ʊ���,����ʱ��������*");
	ret = RP_PrintLine(0, " ");
	ret = RP_PrintLine(0, " ");
	ret = RP_CutPaper();
	ret = RP_ClosePrinter();
	return true;
}
*/

BOOL ExecPro(char * cmdline, BOOL bAsyncFlag, PROCESS_INFORMATION *ppi)
{
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(STARTUPINFO);
	si.dwFlags = STARTF_USESTDHANDLES;
	si.wShowWindow = SW_SHOW;
	si.lpReserved2 = NULL;
	si.cbReserved2 = 0;
	if (CreateProcess(NULL, cmdline, NULL, NULL, FALSE, NORMAL_PRIORITY_CLASS, NULL, NULL, &si, &pi))
	{
		if (!bAsyncFlag)
		{
			WaitForSingleObject(pi.hProcess, INFINITE);
			CloseHandle(pi.hThread);
			CloseHandle(pi.hProcess);
		}
		else
		{
			if (!ppi)
			{
				CloseHandle(pi.hProcess);
				CloseHandle(pi.hThread);
			}
			else
			{
				ppi->hProcess = pi.hProcess ;
				ppi->hThread = pi.hThread ;
			}
		}
		return TRUE;
	}
	return FALSE;
}

DWORD WINAPI ReadChannelRegThread(LPVOID lpVoid)
{
	/*int ret;
	char szip[32],RealCheck[20], watch_value[512];
	unsigned char szCard[10];
    DeviceWatch dev;
    DWORD nPort=5201;
	typedef int (PASCAL *pALL_Watch)(char *,char *,char *,DWORD &);
	pALL_Watch ALL_Watch;
	ALL_Watch=(pALL_Watch)GetProcAddress(g_hDllJBC, "ALL_Watch");
	//int ret=ALL_Watch();
	
	typedef int (PASCAL *pAC100_ConfirmRealData1)(char *,char *,DWORD &);
	pAC100_ConfirmRealData1 AC100_ConfirmRealData1;
	AC100_ConfirmRealData1=(pAC100_ConfirmRealData1)GetProcAddress(g_hDllJBC, "AC100_ConfirmRealData1");


	typedef int (PASCAL *pJBC7K2_OpenWatch)();
	pJBC7K2_OpenWatch JBC7K2_OpenWatch;
	JBC7K2_OpenWatch=(pJBC7K2_OpenWatch)GetProcAddress(g_hDllJBC, "JBC7K2_OpenWatch");
	ret=JBC7K2_OpenWatch();
	if (ret!=0)
	{
		return false;
	}

	//JBC7K2_OpenWatch()
	
    strcpy(szip,"255.255.255.255");
    ZeroMemory(RealCheck,sizeof(RealCheck));
    ZeroMemory(watch_value,sizeof(watch_value));
	char temp[10],szRet[10];
	for(;;)
	{
        ret = ALL_Watch(watch_value, RealCheck, szip, nPort);
        if(3 == ret)
        {
            AC100_ConfirmRealData1(RealCheck, szip, nPort);
			memcpy(dev.card,watch_value,sizeof(watch_value));
			if (strlen(dev.card)==0)
			{
				continue;
			}
			szCard[0]=(unsigned char)dev.card[0];
			szCard[1]=(unsigned char)dev.card[1];
			szCard[2]=(unsigned char)dev.card[2];
			szCard[3]=(unsigned char)dev.card[3];

			memset(szRet,0,sizeof(szRet));
			for(int i=0;i<4;i++)
			{
				memset(temp,0,sizeof(temp));
                if(szCard[i]==0)
					strcpy(temp,"00");
				else if (szCard[i]<16)
				{
                    sprintf(temp,"0%1x",szCard[i]);
				}
				else
                    sprintf(temp,"%2x",szCard[i]);
                strcat(szRet,temp);
			}
			if (dev.card_source==1)  //��1��ͨ��ˢ��������
			{
                if (OperateRegCard("192.168.1.116",szRet,2))//����
                {
					char szT[512] = {'\0'};
					long FldCount = 0, ret = 0;;
					CString outParas[2];
					CWinAdo tAdo(sCONNSTR_MYSQL);			
					ret = tAdo.OpenAdo();
					if (ret != ADOOK)
					{
						continue;
					}			
					for (i = 0; i < 2; i++)
					{
						outParas[i].Empty();
					}
					sprintf(szT, "update t_card set card_status = \'%d\' where card_cardFactory = \'%s\'", 1,szRet);
					ret = tAdo.execSelect(szT, &FldCount, outParas);
					//XMessageBox(g_hWindow,L"�����ɹ���",L"��ʾ");
                }   
				else
                    XMessageBox(g_hWindow,L"�����쳣",L"��ʾ");
			}
			
        }
		else
		{
			Sleep(0);
		}
	}*/
	char szRet[10];
		char szT[512] = {'\0'};
					long FldCount = 0, ret = 0;
					CString outParas[2];
					CWinAdo tAdo(sCONNSTR_MYSQL);			
					ret = tAdo.OpenAdo();
					if (ret != ADOOK)
					{
						//continue;
					}			
					for (int i = 0; i < 2; i++)
					{
						outParas[i].Empty();
					}
					sprintf(szT, "update t_card set card_status = \'%d\' where card_cardFactory = \'%s\'", 1,szRet);
					ret = tAdo.execSelect(szT, &FldCount, outParas);
					return false;
}

DWORD WINAPI SendTimeThread(LPVOID lpVoid)
{		
	//CNovelSocket tSock(SOCK_DGRAM);
	long ret = 0;
	char szBuf[128] = {'\0'}, szSourceID[8] = {'\0'}, szTargetID[8] = {'\0'};
	int nPort = 25006;
	char szIP[32] = {'\0'},szIP1[32] = {'\0'};
	int i = 0;

	/*ret = tSock.StartServer(nPort);
	if (ret != 0)
	{
		XMessageBox(g_hWindow, L"����ʱ��Ķ˿ڱ�ռ��!", L"����");
		return 0;
	}*/
	i = 0;
	while (1)
	{
		
		memset(szBuf, 0, sizeof(szBuf));
		//ret = tSock.RecvDataFrom2(szBuf, 128, szIP, nPort,0);
		WaitForSingleObject(g_ThreadSocketMutex,INFINITE);
        ret = g_tSock->RecvDataFrom2(szBuf, 128, szIP, nPort);
		ReleaseMutex(g_ThreadSocketMutex);
		if (ret > 0)
		{
			if ((unsigned char)szBuf[0] == 0xa2)
			{
				if ((unsigned char)szBuf[1] == 0x02)		//ͬ��ÿ�����
				{
					memset(szBuf, 0, sizeof(szBuf));
					i=0;
					szBuf[i++]=0xa2;
					szBuf[i++]=0x03;
					szBuf[i++]=0x06;
					
                    struct tm *tim = NULL;
	                long l = 0;
                    time((time_t *)&l);
					tim = localtime((time_t *)&l);

					l=tim->tm_year + 1900;
					//szBuf[i++]=l/100;
					szBuf[i++]=l%100;

					szBuf[i++]=tim->tm_mon+1;
					szBuf[i++]=tim->tm_mday;
					szBuf[i++]=tim->tm_hour;
					szBuf[i++]=tim->tm_min;
					szBuf[i++]=tim->tm_sec;
					//szBuf[i++]=0x00;
                    //tSock.SendDataTo(szBuf, i, szIP, 25006);
					WaitForSingleObject(g_ThreadSocketMutex,INFINITE);
					g_tSock->SendDataTo(szBuf, i, szIP, 25006);
					ReleaseMutex(g_ThreadSocketMutex);
					//XMessageBox(g_hWindow,L"�յ�ʱ������",L"��ʾ");
				}
				else
					Sleep(0);
			}
			else
				Sleep(0);
		}
		else
		{
			Sleep(5000);
		}
	}	
    g_tSock->CloseSock();
	//tSock.CloseSock();
	return 0;
}

DWORD WINAPI GetSQLThread(LPVOID lpVoid)
{
	//CNovelSocket tSock(SOCK_DGRAM);
	long FldCount = 0, ret = 0;
	CString outParas[FLD_MAX];
	int iDataReservation;
    char szTime[512] = {'\0'};
    char szDataReservation[FLD_MAX] = {'\0'};
	//g_bExpireData=true;
	CWinAdo tAdo(sCONNSTR_MYSQL);
	ret = tAdo.OpenAdo();
	if ( ret != ADOOK)
	{
		return false;
	}	
    while(g_bExpireData)
	{
		GetPrivateProfileString("TerminalSet", "DataReservation", "", szDataReservation, sizeof(szDataReservation), CONFIG_FILE);
	    //iDataReservation=atoi(szDataReservation);
		sprintf(szTime, "select * from t_visitor where DATE_SUB(CURDATE(),INTERVAL %s DAY) >= from_unixtime(visitor_visitetime);", szDataReservation);
		ret = tAdo.execSelect(szTime, &FldCount, outParas);
		if (ret == ADOERR)
		{
			return false;
		}
		if(FldCount!=0)
		{
			sprintf(szTime, "delete from t_visitor where DATE_SUB(CURDATE(),INTERVAL %s DAY) >= from_unixtime(visitor_visitetime);", szDataReservation);
			ret = tAdo.execSelect(szTime, &FldCount, outParas);
			if (ret == ADOERR)
			{
				return false;
			}
			else
			{
				Sleep(20);
			}
		}
		Sleep(20);
	}
	return 0;
}

DWORD WINAPI GetDataThread(LPVOID lpVoid)
{
	CNovelSocket tSock(SOCK_DGRAM);
	long ret = 0;
	char szT[128] = {'\0'};
	char szR[128] = {'\0'},szIP1[32] = {'\0'};;
	int nPort=13666;
	HELE hEle = NULL;
	wchar_t wBuf[256] = {0};

	ret = tSock.StartServer(13666);
	if (ret != 0)
	{
		return -1;
	}
	while (1)
	{
		/*ret = tSock.Accept();
		if (ret != 0)
		{
			continue;
		}*/
		memset(szT, 0, sizeof(szT));
		//ret = tSock.RecvData(szT, 128);
		ret = tSock.RecvDataFrom(szT, 128, szIP1, nPort);
		if (ret <= 0)
		{
			continue;
		}
		if (atoi(szT) == 199)		//�Ѿ�ƥ�䵽����������Ϣ
		{
			SetOfficialInfo();

		}
		else if (atoi(szT) == 198)	//�������ݲ���ӡ
		{
			::PostMessage(GETHWND, WM_SAVEANDPRINT, NULL, NULL);		//Ͷ�ݱ����ӡ����Ϣ
			Sleep(10);
		}
		else if (atoi(szT) == 200)
		{
			hEle = GETELE(IDE_VISITORNAME);
			XEle_SetFocus(hEle, true);
			XWnd_RedrawWnd(g_hWindow);
		}
		else if (atoi(szT) == 201)
		{
			hEle = GETELE(IDE_VISITORIDNUM);
			XEle_SetFocus(hEle, true);
			XWnd_RedrawWnd(g_hWindow);
		}
		else if (atoi(szT) == 202)	//��д������������Ϣ
		{
			memset((char *)wBuf, 0, sizeof(wBuf));
			memset(szT, 0, sizeof(szT));
			::GetPrivateProfileString("VisitorInfo", "Reason", "", szT, sizeof(szT), CONFIG_FILE);
			S_2_W(wBuf, szT, 256);
			hEle = XWnd_GetEle(g_hWindow, IDE_VISITORREASON);
			XEdit_SetText(hEle, wBuf);
			XWnd_RedrawWnd(g_hWindow);
		}
		else if (atoi(szT) == 203)	//��д������Я����Ʒ
		{
			memset((char *)wBuf, 0, sizeof(wBuf));
			memset(szT, 0, sizeof(szT));
			::GetPrivateProfileString("VisitorInfo", "Goods", "", szT, sizeof(szT), CONFIG_FILE);
			S_2_W(wBuf, szT, 256);
			hEle = XWnd_GetEle(g_hWindow, IDE_VISITORTHING);
			XEdit_SetText(hEle, wBuf);
			XWnd_RedrawWnd(g_hWindow);
		}
		else if (atoi(szT) == 204)
		{
			hEle = GETELE(IDE_VISITORADDR);
			XEle_SetFocus(hEle, true);
			XWnd_RedrawWnd(g_hWindow);
		}
		else if (atoi(szT) == 205) //���ù�˾
		{
			//hEle = GETELE(IDE_VISITORUNIT);
			//XEle_SetFocus(hEle, true);

			memset((char *)wBuf, 0, sizeof(wBuf));
			memset(szT, 0, sizeof(szT));
			::GetPrivateProfileString("VisitorInfo", "Company", "", szT, sizeof(szT), CONFIG_FILE);
			S_2_W(wBuf, szT, 256);
			hEle = XWnd_GetEle(g_hWindow, IDE_VISITORUNIT);
			XEdit_SetText(hEle, wBuf);
			XWnd_RedrawWnd(g_hWindow);
		}
		else if (atoi(szT) == 206)
		{
			hEle = GETELE(IDE_VISITORMOBILE);
			XEle_SetFocus(hEle, true);
			XWnd_RedrawWnd(g_hWindow);
		}
		else
		{
			memset(szR, 0, sizeof(szR));
			GetEditData(atoi(szT), szR);		//����ָ���ȡ����
			ret = tSock.SendData(szR, strlen(szR));
		}
	}
	tSock.CloseSock();
	return 0;
}

//���������ı�����Ϣ
BOOL SendBackScreenAppData(char *pszData)
{
	long ret = 0;
	char szT[128] = {'\0'};
	
	CNovelSocket tSock;
	ret = tSock.Connect("127.0.0.1", 13668);
	if (ret != 0)
	{
		return false;
	}
	ret = tSock.SendData(pszData, strlen(pszData));
	tSock.CloseSock();
	if (ret != strlen(pszData))
	{
		return false;
	}
	return true;
}

BOOL SendKeyBdAppData(char *pszData)
{
	long ret = 0;
	char szT[128] = {'\0'};
	
	CNovelSocket tSock;
	ret = tSock.Connect("127.0.0.1", 13667);
	if (ret != 0)
	{
		return false;
	}
	ret = tSock.SendData(pszData, strlen(pszData));
	tSock.CloseSock();
	if (ret != strlen(pszData))
	{
		return false;
	}
	return true;
}

//�����������ͻ���������ĵ�ǰ����
BOOL GetEditData(int nType, char *pszOut)
{
	wchar_t wBuf[128] = {0};
	switch (nType)
	{
	case BY_OFFICIAL_NAME:
		XEdit_GetText(GETELE(IDE_MASTERNAME), wBuf, 128);
		break;
	case BY_OFFICE_PHONE:
		XEdit_GetText(GETELE(IDE_OFFICEPHONE), wBuf, 128);
		break;
	case BY_OFFICIAL_MOBILE:
		XEdit_GetText(GETELE(IDE_MASTERMOBILE), wBuf, 128);
		break;
	case BY_OFFICE_NAME:
		XEdit_GetText(GETELE(IDE_OFFICENAME), wBuf, 128);
		break;
	case BY_OFFICIAL_LEVEL:
		XEdit_GetText(GETELE(IDC_MASTERZHIWU), wBuf, 128);
		break;
	case BY_ROOM:
		XEdit_GetText(GETELE(IDE_ROOMNUM), wBuf, 128);
		break;
	default:
		break;
	}
	if (wcslen(wBuf) <= 0)			//����Ϊ��
	{
		strcpy(pszOut, "####");
	}
	else
	{
		W_2_S(wBuf, pszOut, 128);
	}
	return true;
}

void SetOfficialInfo()
{
	char szT[256] = {'\0'};
	wchar_t wBuf[256] = {0};
	SetOfficialReadOnly(false);

	::GetPrivateProfileString("OfficialInfo", "Name", "", szT, sizeof(szT), CONFIG_FILE);
	memset((char *)wBuf, 0, sizeof(wBuf));
	MultiByteToWideChar(CP_ACP, 0, szT, -1, wBuf, 256);		//������������
	HELE hE = XWnd_GetEle(g_hWindow, IDE_MASTERNAME);
	XEdit_SetText(hE, wBuf);

	::GetPrivateProfileString("OfficialInfo", "Sex", "", szT, sizeof(szT), CONFIG_FILE);
	memset((char *)wBuf, 0, sizeof(wBuf));
	MultiByteToWideChar(CP_ACP, 0, szT, -1, wBuf, 256);		//���������Ա�
	hE = XWnd_GetEle(g_hWindow, IDC_MASTERSEX);
	XEdit_SetText(hE, wBuf);
/*	if (strcmp(szT, "��") == 0)
	{
		XComboBox_SetSelectItem(hE, 0);
	}
	else
	{
		XComboBox_SetSelectItem(hE, 1);
	}*/
	
	::GetPrivateProfileString("OfficialInfo", "OfficePhone", "", szT, sizeof(szT), CONFIG_FILE);
	memset((char *)wBuf, 0, sizeof(wBuf));
	MultiByteToWideChar(CP_ACP, 0, szT, -1, wBuf, 256);		//�������˰칫�绰
	hE = XWnd_GetEle(g_hWindow, IDE_OFFICEPHONE);
	XEdit_SetText(hE, wBuf);
	
	::GetPrivateProfileString("OfficialInfo", "Mobile", "", szT, sizeof(szT), CONFIG_FILE);
	memset((char *)wBuf, 0, sizeof(wBuf));
	MultiByteToWideChar(CP_ACP, 0, szT, -1, wBuf, 256);		//���������ֻ�
	hE = XWnd_GetEle(g_hWindow, IDE_MASTERMOBILE);
	XEdit_SetText(hE, wBuf);
	
	::GetPrivateProfileString("OfficialInfo", "OfficeName", "", szT, sizeof(szT), CONFIG_FILE);
	memset((char *)wBuf, 0, sizeof(wBuf));
	MultiByteToWideChar(CP_ACP, 0, szT, -1, wBuf, 256);		//�������˰칫������
	hE = XWnd_GetEle(g_hWindow, IDE_OFFICENAME);
	XEdit_SetText(hE, wBuf);
	
	::GetPrivateProfileString("OfficialInfo", "OfficeAddr", "", szT, sizeof(szT), CONFIG_FILE);
	memset((char *)wBuf, 0, sizeof(wBuf));
	MultiByteToWideChar(CP_ACP, 0, szT, -1, wBuf, 256);		//�������˰칫�ҵ�ַ
	hE = XWnd_GetEle(g_hWindow, IDE_OFFICENUM);
	XEdit_SetText(hE, wBuf);
	
	::GetPrivateProfileString("OfficialInfo", "Level", "", szT, sizeof(szT), CONFIG_FILE);
	memset((char *)wBuf, 0, sizeof(wBuf));
	MultiByteToWideChar(CP_ACP, 0, szT, -1, wBuf, 256);		//�������˼���
	hE = XWnd_GetEle(g_hWindow, IDC_MASTERZHIWU);
	XEdit_SetText(hE, wBuf);
	
	::GetPrivateProfileString("OfficialInfo", "RoomNum", "", szT, sizeof(szT), CONFIG_FILE);
	memset((char *)wBuf, 0, sizeof(wBuf));
	MultiByteToWideChar(CP_ACP, 0, szT, -1, wBuf, 256);		//�������˷���
	hE = XWnd_GetEle(g_hWindow, IDE_ROOMNUM);
	XEdit_SetText(hE, wBuf);


	if (::GetPrivateProfileInt("TerminalSet", "bSaveOfficial",0, CONFIG_FILE)==1)  //�����±�����
	   SetOfficialReadOnly(false);
	else
	   SetOfficialReadOnly(true);
//    BackScreenCheckInfo();
    BackScreenSelectOfficial();  //��ʾ���������ѯ������
	XWnd_RedrawWnd(g_hWindow);
	return;
}

//
void PrintHtmlPage(char *pszURL)
{
	if(1 ==::GetPrivateProfileInt("TerminalSet", "HtmlReceiptTips",0, CONFIG_FILE))
	{		
		return ;
	}
	wchar_t wBuf[256] = {'\0'};

	if (g_hWeb == NULL)
	{
		g_hWeb = XWeb_Create(g_hWindow, -10, -10, 10, 10);
		XWeb_Show(g_hWeb, true);
		XWeb_RegEvent(g_hWeb, IE_DOCUMENTCOMPLETE, OnDocComplete);
	}
	S_2_W(wBuf, pszURL, 256);
	XWeb_OpenURL(g_hWeb, wBuf);

	return;
}

BOOL CreateNoFrameReceiptHtml(const VISITORINFO &visitor, char *pszFile)
{
	long i = 0;
	struct tm *tim = NULL;
	char HeadWords[40] = {'\0'};
	GetPrivateProfileString("HtmlPageTips", "HeadWords","��͵�",HeadWords, sizeof(HeadWords), CONFIG_FILE);
	int iFont=::GetPrivateProfileInt("HtmlPageTips", "HeadFont",6, CONFIG_FILE);
	if (iFont<1)
	{
		iFont=1;
	}
	else if (iFont>6)
	{
		iFont=6;
	}

	FILE *fp = fopen(pszFile, "wt");
	if (fp == NULL)
	{
		return false;
	}

	time((time_t *)&i);
	tim = localtime((time_t *)&i);

	fprintf(fp, "%s\r\n", "<html>");

	fprintf(fp, "%s\r\n", "<head>");
	fprintf(fp, "%s\r\n", "<meta http-equiv=\"Content-Type\" content=\"text/html; charset=gb2312\">");
	fprintf(fp, "%s\r\n", "<title></title>");
	fprintf(fp, "%s\r\n", "<style type=\"text/css\">");
	fprintf(fp, "%s\r\n", "table");
	fprintf(fp, "%s\r\n", "{");
	fprintf(fp, "%s\r\n", "	cellpadding: 0;");
	fprintf(fp, "%s\r\n", "	cellspacing: 0;");
	fprintf(fp, "%s\r\n", "}");
	fprintf(fp, "%s\r\n", "tr");
	fprintf(fp, "%s\r\n", "{");
	fprintf(fp, "%s\r\n", "	height: 25");
	fprintf(fp, "%s\r\n", "}");
	fprintf(fp, "%s\r\n", "td");
	fprintf(fp, "%s\r\n", "{");
	fprintf(fp, "%s\r\n", "	font-size: 14px");
	fprintf(fp, "%s\r\n", "}");
	fprintf(fp, "%s\r\n", "</style>");
	fprintf(fp, "%s\r\n", "</head>");

	fprintf(fp, "%s\r\n", "<body topmargin=\"0\" bottommargin=\"0\" style=\"overflow-y:hidden\">");

	fprintf(fp, "%s\r\n", "<div align=\"center\">");
	fprintf(fp, "%s\r\n", "	<table border=\"0\" style=\"border-collapse: collapse\" width=\"240\" cellpadding=\"0\">");
	fprintf(fp, "%s\r\n", "		<tr>");
	fprintf(fp, "%s\r\n", "			<td colspan=\"2\">");
	fprintf(fp, "			<p align=\"center\"><b><font size=\"%d\"> %s </font></b></td>",iFont,HeadWords);
	fprintf(fp, "%s\r\n", "		</tr>");
	fprintf(fp, "%s\r\n", "	</table>");

	//ͼƬ
    char szUserPic [256];
	WIN32_FIND_DATA FindFileData;
	//if (!_access(g_Printvisitor.szUserIDPic,0))//if ( _access(file,0) )
	FindClose(FindFirstFile(g_Printvisitor.szUserIDPic,&FindFileData));
	if (FindFileData.nFileSizeLow>0&&!_access(g_Printvisitor.szUserIDPic,0))
	{
		strcpy(szUserPic,g_Printvisitor.szUserIDPic);
	}
	else if (!_access(g_Printvisitor.szUserHeadPic,0))
	{
		strcpy(szUserPic,g_Printvisitor.szUserHeadPic);
	}
	else
		strcpy(szUserPic,"��ͼƬ");

	fprintf(fp, "%s\r\n", "	<table border=\"0\" cellpadding=\"0\" cellspacing=\"0\" width=\"240\" style=\"border-width: 0\">");
    //fprintf(fp, "	        <td align=\"center\"><img src=\"%s\"></td>\r\n", "%", szUserPic);
	fprintf(fp, "%s\r\n", "		<tr>");
	fprintf(fp, "			<td align=\"center\"><img src=\"%s\"></td>\r\n", szUserPic);
	fprintf(fp, "%s\r\n", "		</tr>");
    fprintf(fp, "%s\r\n", "	</table>");

	fprintf(fp, "%s\r\n", "	<table border=\"0\" style=\"border-collapse: collapse; table-layout:fixed\" width=\"240\" bordercolor=\"#000000\" cellpadding=\"0\">");
	fprintf(fp, "%s\r\n", "		<tr>");
	fprintf(fp, "%s\r\n", "			<td width=\"33\%\" align=\"right\">����:</td>");
	if (strlen(visitor.szName)>12)
	{
        fprintf(fp, "			<td width=\"67%s\" align=\"left\"><font size=\"1\">%s</font></td>\r\n", "%", visitor.szName);
	}
	else
	    fprintf(fp, "			<td width=\"67%s\" align=\"left\">%s</td>\r\n", "%", visitor.szName);


	fprintf(fp, "%s\r\n", "		</tr>");
	fprintf(fp, "%s\r\n", "		<tr>");
	fprintf(fp, "%s\r\n", "		<td width=\"26\%\" align=\"right\">���֤��:</td>");
    CString str=visitor.szID;
	if (::GetPrivateProfileInt("TerminalSet", "bHideLast4ID",0, CONFIG_FILE)==1)
	{
		int n=strlen(visitor.szID);
		if (n == 18)
		{
			str=str.Left(14)+"****";
		}
	}
	fprintf(fp, "			<td width=\"35%s\" colspan=\"2\" align=\"left\">%s</td>\r\n", "%", str);
	fprintf(fp, "%s\r\n", "		</tr>");

	fprintf(fp, "%s\r\n", "		<tr>");
	fprintf(fp, "%s\r\n", "			<td width=\"26\%\" align=\"right\">��λ:</td>");
	fprintf(fp, "			<td width=\"30%s\" align=\"left\">%s</td>\r\n", "%", visitor.szUnit);
	fprintf(fp, "%s\r\n", "		</tr>");

	fprintf(fp, "%s\r\n", "		<tr>");
	fprintf(fp, "%s\r\n", "			<td width=\"26\%\" align=\"right\">ְ��:</td>");
	fprintf(fp, "%s\r\n", "			<td width=\"30%s\" align=\"left\">��</td>\r\n");
	fprintf(fp, "%s\r\n", "		</tr>");

	fprintf(fp, "%s\r\n", "		<tr>");
	fprintf(fp, "%s\r\n", "			<td width=\"26\%\" align=\"right\">��Ч��:</td>"); //ʱ ��19ʱ
	fprintf(fp, "%s\r\n", "			<td>");
	fprintf(fp, "			<p align=\"left\">%04ld-%02ld-%02ld %02ldʱ ��19ʱ</td>\r\n", tim->tm_year + 1900, tim->tm_mon + 1, tim->tm_mday, tim->tm_hour);
	fprintf(fp, "%s\r\n", "		</tr>");


	fprintf(fp, "%s\r\n", "	<table border=\"0\" cellpadding=\"0\" cellspacing=\"0\" width=\"240\" style=\"border-width: 0\">");
	fprintf(fp, "%s\r\n", "		<tr>");
	fprintf(fp, "			<td align=\"center\"><img src=\"%s\" height=50 width=160></td>\r\n", USER_BARCODE_PIC);
	fprintf(fp, "%s\r\n", "		</tr>");

	fprintf(fp, "%s\r\n", "		<tr>");
	char szUnitName[128] = {'\0'};
	::GetPrivateProfileString("TerminalSet", "PrintFootLog", "", szUnitName, sizeof(szUnitName), CONFIG_FILE);
	fprintf(fp, "			<td width=\"100%s\"><b>%s</b></td>\r\n", "%", szUnitName);
	fprintf(fp, "%s\r\n", "		</tr>");
	
//	fprintf(fp, "%s\r\n", "		<tr>");
//	fprintf(fp, "%s\r\n", "			<td width=\"100%\" align=\"left\">*�����Ʊ��ܣ����Ž�������*</td>");
//	fprintf(fp, "%s\r\n", "		</tr>");
	fprintf(fp, "%s\r\n", "		</table>");
	fprintf(fp, "%s\r\n", "</div>");

	fprintf(fp, "%s\r\n", "</body>");

	fprintf(fp, "%s\r\n", "</html>");

	fclose(fp);

	return true;
}

BOOL CreateVisitorInfTipsUptoServer(const VISITORINFO &visitor)
{
#define UPTOSERVERTIPS_FILE ".\\UptoServer\\profile.ini"
	HWND hWnd = ::FindWindow(NULL, "UptoServer-cc");
	if (hWnd == NULL)
	{
		//StartPrintTipsPB();
		StartUptoServer();
		XMessageBox(g_hWindow, L"��¼�ϴ��������û������,�����Ի���ϵ����Ա!", L"����");
		return false;
	}
	//����
	::WritePrivateProfileString("Tips", "Name", visitor.szName, UPTOSERVERTIPS_FILE);

	//�Ա�
	::WritePrivateProfileString("Tips", "Sex", visitor.szSex, UPTOSERVERTIPS_FILE);

	//����
	::WritePrivateProfileString("Tips", "Folk", visitor.szFolk, UPTOSERVERTIPS_FILE);

	//�绰
	::WritePrivateProfileString("Tips", "Phone", visitor.szPhone, UPTOSERVERTIPS_FILE);

	//��������
	::WritePrivateProfileString("Tips", "Reason", visitor.szReason, UPTOSERVERTIPS_FILE);
	::WritePrivateProfileString("Tips", "Goods", visitor.szWithGoods, UPTOSERVERTIPS_FILE);

	//ǩ֤����
	::WritePrivateProfileString("Tips", "Signdepartment", visitor.szSignDepartment, UPTOSERVERTIPS_FILE);
	
	::WritePrivateProfileString("Tips", "StationID", visitor.szVisitStation, UPTOSERVERTIPS_FILE);
	::WritePrivateProfileString("Tips", "CardTypeCode", visitor.szIDType, UPTOSERVERTIPS_FILE);
	::WritePrivateProfileString("Tips", "CardID", visitor.szID, UPTOSERVERTIPS_FILE);
	::WritePrivateProfileString("Tips", "Addr", visitor.szAddr, UPTOSERVERTIPS_FILE);
	::WritePrivateProfileString("Tips", "Birth", visitor.szBirth, UPTOSERVERTIPS_FILE);
	::WritePrivateProfileString("Tips", "Totalpeople", visitor.szTotalPeople, UPTOSERVERTIPS_FILE);
	//��ʦ��,���ƺ�
	::WritePrivateProfileString("Tips", "Carnum", visitor.szCarNum, UPTOSERVERTIPS_FILE);

	//֪ͨ�飬���õ�λ
	::WritePrivateProfileString("Tips", "Unit", visitor.szUnit, UPTOSERVERTIPS_FILE);
	//ְ��
	::WritePrivateProfileString("Tips", "Job", visitor.szJob, UPTOSERVERTIPS_FILE);

	::WritePrivateProfileString("Tips", "ID2Readtype", visitor.szID2Flag, UPTOSERVERTIPS_FILE);
	::WritePrivateProfileString("Tips", "Barcode", visitor.szBarCode, UPTOSERVERTIPS_FILE);
	::WritePrivateProfileString("Tips", "Validdate", visitor.szValidDate, UPTOSERVERTIPS_FILE);
	//֤����
    /*
    CString str=visitor.szID;
	if (::GetPrivateProfileInt("TerminalSet", "bHideLast4ID",0, CONFIG_FILE)==1)
	{
		int n=strlen(visitor.szID);
		if (n == 18)
		{
			str=str.Left(14)+"****";
		}
	}
	*/
	CString str;
	str.Format("%d",visitor.nStatus);
	::WritePrivateProfileString("Tips", "Status", str, UPTOSERVERTIPS_FILE);

	str.Format("%ld",visitor.lLeftTime);
	::WritePrivateProfileString("Tips", "Lefttime", str, UPTOSERVERTIPS_FILE);

	str.Format("%ld",visitor.lVisiteTime);
	::WritePrivateProfileString("Tips", "Visitetime", str, UPTOSERVERTIPS_FILE);


	//���ò���
	::WritePrivateProfileString("Tips", "ToOfficeName", visitor.szToOfficeName, UPTOSERVERTIPS_FILE);

	//������
	::WritePrivateProfileString("Tips", "ToOfficialName", visitor.szToOfficialName, UPTOSERVERTIPS_FILE);

	//�����
	::WritePrivateProfileString("Tips", "RoomNO", visitor.szToRoom, UPTOSERVERTIPS_FILE);

	//SSO
	::WritePrivateProfileString("Tips", "ToSSO", visitor.szToSSO, UPTOSERVERTIPS_FILE);
	::WritePrivateProfileString("Tips", "TotalPeople",visitor.szTotalPeople, UPTOSERVERTIPS_FILE);

	//ֵ����
	char szUserName[50] = {0};
	::GetPrivateProfileString("TerminalSet", "UserName", "", szUserName, sizeof(szUserName), CONFIG_FILE);
	::WritePrivateProfileString("Tips", "ExtUserID", szUserName, UPTOSERVERTIPS_FILE);





	//ͷ��
	//
    //char szUserPic [256];
	//WIN32_FIND_DATA FindFileData;
	//if (!_access(g_Printvisitor.szUserIDPic,0))//if ( _access(file,0) )
	//FindClose(FindFirstFile(visitor.szUserIDPic,&FindFileData));
	//if (FindFileData.nFileSizeLow>0&&!_access(visitor.szUserIDPic,0))
	{
		//strcpy(szUserPic,g_Printvisitor.szUserIDPic);
		//sprintf(szT, ".%s", szUserPic);
		::WritePrivateProfileString("Tips", "UserIDpic", visitor.szUserIDPic, UPTOSERVERTIPS_FILE);
	}
	//else if (!_access(g_Printvisitor.szUserHeadPic,0))
	{
		//strcpy(szUserPic,g_Printvisitor.szUserHeadPic);		
		//sprintf(szT, ".%s", szUserPic);
		::WritePrivateProfileString("Tips", "UserHeadPic", visitor.szUserHeadPic, UPTOSERVERTIPS_FILE);
	}
	//else
	{
		//strcpy(szUserPic,"��ͼƬ");
		::WritePrivateProfileString("Tips", "UserVideopic", visitor.szUserVideoPic, UPTOSERVERTIPS_FILE);
	}
	::WritePrivateProfileString("Tips", "UserIDfullpic", visitor.szUserIDFullPic, UPTOSERVERTIPS_FILE);

	if (::GetPrivateProfileInt("TerminalSet", "bWebOnline",0, CONFIG_FILE)==0) //�첽�ύ��Ϣ
	{
		PostMessage(hWnd, 1600, 0, 0);
		PostMessage(hWnd, 1700, 0, 0);
		PostMessage(hWnd, 1701, 0, 0);
		PostMessage(hWnd, 1702, 0, 0);
		PostMessage(hWnd, 1703, 0, 0);
	}
	return true;

}
BOOL CreateReceiptTipsPB(const VISITORINFO &visitor)
{
#define TIPS_FILE ".\\printtips\\profile.ini"
#define TIPS_HEAD_PIC ".\\printtips\\"
	long i = 0;
	struct tm *tim = NULL;
	char szT[200];

	HWND hWnd = ::FindWindow(NULL, "�ÿ�ƾ����ӡ");
	if (hWnd == NULL)
	{
		StartPrintTipsPB();
		XMessageBox(g_hWindow, L"PB������ӡ����û������,�����Ի���ϵ����Ա!", L"����");
		return false;
	}

	//��͵�
	::WritePrivateProfileString("Tips", "Head", "��͵�", TIPS_FILE);

	//ʱ��
	if (visitor.lVisiteTime > 0)
	{
		i = visitor.lVisiteTime;
	}
	else
		i = g_Printvisitor.lVisiteTime;
	tim = localtime((time_t *)&i);
	sprintf(szT, "����ʱ��: %04ld-%02ld-%02ld %02ld:%02ld:%02ld", tim->tm_year + 1900, tim->tm_mon + 1, tim->tm_mday, tim->tm_hour, tim->tm_min, tim->tm_sec);		
	::WritePrivateProfileString("Tips", "Datetime", szT, TIPS_FILE);

	//����
	::WritePrivateProfileString("Tips", "Name", visitor.szName, TIPS_FILE);

	//�Ա�
	::WritePrivateProfileString("Tips", "Sex", visitor.szSex, TIPS_FILE);

	//�ÿ�����
	::WritePrivateProfileString("Tips", "VisitorType", visitor.szType, TIPS_FILE);

	//����
	::WritePrivateProfileString("Tips", "Folk", visitor.szFolk, TIPS_FILE);

	//�绰
	::WritePrivateProfileString("Tips", "Telephone", visitor.szPhone, TIPS_FILE);

	//���ƺ�
	::WritePrivateProfileString("Tips", "CarNum", visitor.szCarNum, TIPS_FILE);

	//��������
	::WritePrivateProfileString("Tips", "Reason", visitor.szReason, TIPS_FILE);
	//Я����Ʒ
	::WritePrivateProfileString("Tips", "Goods", visitor.szWithGoods, TIPS_FILE);
	::WritePrivateProfileString("Tips", "TotalPeople",visitor.szTotalPeople, TIPS_FILE);

	//֤����
    CString str=visitor.szID;
	if (::GetPrivateProfileInt("TerminalSet", "bHideLast4ID",0, CONFIG_FILE)==1)
	{
		int n=strlen(visitor.szID);
		if (n == 18)
		{
			str=str.Left(14)+"****";
		}
	}
	::WritePrivateProfileString("Tips", "ID", str, TIPS_FILE);

	//���ò���
	::WritePrivateProfileString("Tips", "Office", visitor.szToOfficeName, TIPS_FILE);

	//������
	::WritePrivateProfileString("Tips", "Official", visitor.szToOfficialName, TIPS_FILE);

	//�����
	::WritePrivateProfileString("Tips", "RoomNO", visitor.szToRoom, TIPS_FILE);

	//SSO
	::WritePrivateProfileString("Tips", "SSO", visitor.szToSSO, TIPS_FILE);

	//ֵ����
	char szUserName[50] = {0};
	::GetPrivateProfileString("TerminalSet", "UserName", "", szUserName, sizeof(szUserName), CONFIG_FILE);
	::WritePrivateProfileString("Tips", "User", szUserName, TIPS_FILE);

	if(g_nIDType == ID_TYPE_Lawyer)
	{
		//��ʦ��
		::WritePrivateProfileString("Tips", "Lawyerletter", visitor.szCarNum, TIPS_FILE);

		//֪ͨ��
		::WritePrivateProfileString("Tips", "Notice", visitor.szUnit, TIPS_FILE);

		::WritePrivateProfileString("Tips", "TipsType", "2", TIPS_FILE);
	}
	else
	{
		//�����˵�λ
		::WritePrivateProfileString("Tips", "Unit", visitor.szUnit, TIPS_FILE);
		::WritePrivateProfileString("Tips", "TipsType", "1", TIPS_FILE);

		if (g_PrintTipsCfg.ePrintType == PRINT_SIMPLE_TYPE)
		{
			::WritePrivateProfileString("Tips", "TipsType", "3", TIPS_FILE);
		}
		else if(::GetPrivateProfileInt("TerminalSet", "TipsTypeDH7", 0, CONFIG_FILE)==1)
		{
			char szT[512] = {'\0'};
			long FldCount = 0, ret = 0;
			CString outParas[FLD_MAX];
			CWinAdo tAdo(sCONNSTR_MYSQL);
			
			ret = tAdo.OpenAdo();
			if (ret != ADOOK)
			{
				return false;
			}

			for (int i = 0; i < FLD_MAX; i++)
			{
				outParas[i].Empty();
			}
			sprintf(szT, "select count(*) from t_visitor where (TO_DAYS(NOW()) - TO_DAYS(FROM_UNIXTIME(visitor_visitetime))=0) and visitor_status >1");
			ret = tAdo.execSelect(szT, &FldCount, outParas);
			if (ret == ADOERR || (FldCount == 0 && outParas[0].IsEmpty()))
			{
				//return false;
				::WritePrivateProfileString("Tips", "no", "", TIPS_FILE);
			}
			else
				::WritePrivateProfileString("Tips", "no", outParas[0], TIPS_FILE);

			::WritePrivateProfileString("Tips", "TipsType", "7", TIPS_FILE);
		}
	}




	//ͷ��
	//
    char szUserPic [256];
	WIN32_FIND_DATA FindFileData;
	//if (!_access(g_Printvisitor.szUserIDPic,0))//if ( _access(file,0) )
	FindClose(FindFirstFile(g_Printvisitor.szUserIDPic,&FindFileData));
	if (FindFileData.nFileSizeLow>0&&!_access(g_Printvisitor.szUserIDPic,0))
	{
		strcpy(szUserPic,g_Printvisitor.szUserIDPic);
		sprintf(szT, ".%s", szUserPic);
		::WritePrivateProfileString("Tips", "Headpic", szT, TIPS_FILE);
	}
	else if (!_access(g_Printvisitor.szUserHeadPic,0))
	{
		strcpy(szUserPic,g_Printvisitor.szUserHeadPic);		
		sprintf(szT, ".%s", szUserPic);
		::WritePrivateProfileString("Tips", "Headpic", szT, TIPS_FILE);
	}
	else
	{
		strcpy(szUserPic,"��ͼƬ");
		::WritePrivateProfileString("Tips", "Headpic", "", TIPS_FILE);
	}

	//����
	if(1 ==::GetPrivateProfileInt("BarCode", "QR",0, CONFIG_FILE))
		::WritePrivateProfileString("Tips", "Bacoderpic", "..\\pic\\QR.bmp", TIPS_FILE);
	else
		::WritePrivateProfileString("Tips", "Bacoderpic", "..\\pic\\user_barcode_pic.bmp", TIPS_FILE);

    int ret;
	CSelectPrinttips s(GETHWND);
	s.CreateWindow_XC(L"�Ƿ��ӡ��͵�");
	ret = s.ShowTips();
	if(ret == 0)
		return true;

	SendMessage(hWnd, 1600, 0, 0);
	return true;
}

BOOL CreateReceiptHtml(const VISITORINFO &visitor, char *pszFile)
{
	if(1 ==::GetPrivateProfileInt("TerminalSet", "HtmlReceiptTips",0, CONFIG_FILE))
	{
		CreateReceiptTipsPB(visitor);
		return true;
	}


	long i = 0;
	struct tm *tim = NULL;
	char HeadWords[40] = {'\0'};
	GetPrivateProfileString("HtmlPageTips", "HeadWords","��͵�",HeadWords, sizeof(HeadWords), CONFIG_FILE);
	int iFont=::GetPrivateProfileInt("HtmlPageTips", "HeadFont",6, CONFIG_FILE);
	if (iFont<1)
	{
		iFont=1;
	}
	else if (iFont>6)
	{
		iFont=6;
	}

	FILE *fp = fopen(pszFile, "wt");
	if (fp == NULL)
	{
		return false;
	}

	if (visitor.lVisiteTime > 0)
	{
		i = visitor.lVisiteTime;
	}
	else
		i = g_Printvisitor.lVisiteTime;
	tim = localtime((time_t *)&i);

	fprintf(fp, "%s\r\n", "<html>");

	fprintf(fp, "%s\r\n", "<head>");
	fprintf(fp, "%s\r\n", "<meta http-equiv=\"Content-Type\" content=\"text/html; charset=gb2312\">");
	fprintf(fp, "%s\r\n", "<title></title>");
	fprintf(fp, "%s\r\n", "<style type=\"text/css\">");
	fprintf(fp, "%s\r\n", "table");
	fprintf(fp, "%s\r\n", "{");
	fprintf(fp, "%s\r\n", "	cellpadding: 0;");
	fprintf(fp, "%s\r\n", "	cellspacing: 0;");
	fprintf(fp, "%s\r\n", "}");
	fprintf(fp, "%s\r\n", "tr");
	fprintf(fp, "%s\r\n", "{");
	fprintf(fp, "%s\r\n", "	height: 25");
	fprintf(fp, "%s\r\n", "}");
	fprintf(fp, "%s\r\n", "td");
	fprintf(fp, "%s\r\n", "{");
	fprintf(fp, "%s\r\n", "	font-size: 14px");
	fprintf(fp, "%s\r\n", "}");
	fprintf(fp, "%s\r\n", "</style>");
	fprintf(fp, "%s\r\n", "</head>");

	fprintf(fp, "%s\r\n", "<body topmargin=\"0\" bottommargin=\"0\" style=\"overflow-y:hidden\">");

	fprintf(fp, "%s\r\n", "<div align=\"center\">");
	fprintf(fp, "%s\r\n", "	<table border=\"0\" style=\"border-collapse: collapse\" width=\"240\" cellpadding=\"0\">");
	fprintf(fp, "%s\r\n", "		<tr>");
	fprintf(fp, "%s\r\n", "			<td colspan=\"2\">");
	fprintf(fp, "			<p align=\"center\"><b><font size=\"%d\"> %s </font></b></td>",iFont,HeadWords);
	fprintf(fp, "%s\r\n", "		</tr>");
	fprintf(fp, "%s\r\n", "		<tr>");
	fprintf(fp, "%s\r\n", "			<td colspan=\"2\">");
	fprintf(fp, "			<p align=\"left\">����ʱ��: %04ld-%02ld-%02ld %02ld:%02ld:%02ld</td>\r\n", tim->tm_year + 1900, tim->tm_mon + 1, tim->tm_mday, tim->tm_hour, tim->tm_min, tim->tm_sec);
	fprintf(fp, "%s\r\n", "		</tr>");
	fprintf(fp, "%s\r\n", "		</table>");

	fprintf(fp, "%s\r\n", "	<table border=\"1\" style=\"border-collapse: collapse; table-layout:fixed\" width=\"240\" bordercolor=\"#000000\" cellpadding=\"0\">");
	fprintf(fp, "%s\r\n", "		<tr>");
	fprintf(fp, "%s\r\n", "			<td width=\"25\%\">�ÿ�����</td>");
	if (strlen(visitor.szName)>12)
	{
        fprintf(fp, "			<td width=\"30%s\"><font size=\"1\">%s</font></td>\r\n", "%", visitor.szName);
	}
	else
	    fprintf(fp, "			<td width=\"30%s\">%s</td>\r\n", "%", visitor.szName);

    char szUserPic [256];
	WIN32_FIND_DATA FindFileData;
	//if (!_access(g_Printvisitor.szUserIDPic,0))//if ( _access(file,0) )
	FindClose(FindFirstFile(g_Printvisitor.szUserIDPic,&FindFileData));
	if (FindFileData.nFileSizeLow>0&&!_access(g_Printvisitor.szUserIDPic,0))
	{
		strcpy(szUserPic,g_Printvisitor.szUserIDPic);
	}
	else if (!_access(g_Printvisitor.szUserHeadPic,0))
	{
		strcpy(szUserPic,g_Printvisitor.szUserHeadPic);
	}
	else
		strcpy(szUserPic,"��ͼƬ");
		
	fprintf(fp, "			<td width=\"44%s\" rowspan=\"5\"><img src=\"%s\" width=104 height=123></td>\r\n", "%", szUserPic);
	fprintf(fp, "%s\r\n", "		</tr>");
	fprintf(fp, "%s\r\n", "		<tr>");
	fprintf(fp, "%s\r\n", "			<td width=\"26\%\">��    ��</td>");
	fprintf(fp, "			<td width=\"30%s\">%s</td>\r\n", "%", visitor.szSex);
	fprintf(fp, "%s\r\n", "		</tr>");
	fprintf(fp, "%s\r\n", "		<tr>");
	fprintf(fp, "%s\r\n", "			<td width=\"26\%\">��    ��</td>");
	fprintf(fp, "			<td width=\"30%s\">%s</td>\r\n", "%", visitor.szFolk);
	fprintf(fp, "%s\r\n", "		</tr>");
	fprintf(fp, "%s\r\n", "		<tr>");
	fprintf(fp, "%s\r\n", "			<td width=\"26\%\">��    ��</td>");
	fprintf(fp, "			<td width=\"30%s\"><font size=\"1\">%s</font></td>\r\n", "%", visitor.szPhone);
	fprintf(fp, "%s\r\n", "		</tr>");
	fprintf(fp, "%s\r\n", "		<tr>");
	fprintf(fp, "%s\r\n", "			<td width=\"26\%\">��������</td>");
	fprintf(fp, "			<td width=\"30%s\">%s</td>\r\n", "%", visitor.szTotalPeople);
	fprintf(fp, "%s\r\n", "		</tr>");
	fprintf(fp, "%s\r\n", "		<tr>");
	fprintf(fp, "%s\r\n", "			<td width=\"26\%\">��������</td>");
	fprintf(fp, "			<td width=\"35%s\" colspan=\"2\">%s</td>\r\n", "%", visitor.szReason);
	fprintf(fp, "%s\r\n", "		</tr>");
	fprintf(fp, "%s\r\n", "		<tr>");
	fprintf(fp, "%s\r\n", "			<td width=\"26\%\">Я����Ʒ</td>");
	fprintf(fp, "			<td width=\"35%s\" colspan=\"2\">%s</td>\r\n", "%", visitor.szWithGoods);
	fprintf(fp, "%s\r\n", "		</tr>");


	if(g_nIDType == ID_TYPE_Lawyer)
	{
		fprintf(fp, "%s\r\n", "		<tr>");
		fprintf(fp, "%s\r\n", "			<td width=\"26\%\">֪ͨ��</td>");
		fprintf(fp, "%s\r\n", "			<td colspan=\"2\" align=\"center\">");
		fprintf(fp, "%s\r\n", "			<table border=\"1\" style=\"border-collapse: collapse; table-layout:fixed\" width=174 height=25 bordercolor=\"#000000\" cellpadding=\"0\">");
		fprintf(fp, "%s\r\n", "			<tr>");
		fprintf(fp, "			<td width=\"15%s\">%s</td>\r\n", "%", visitor.szUnit);
		fprintf(fp, "%s\r\n", "			<td width=\"70%s\" align=\"right\">��ʦ����������</td>");
		fprintf(fp, "			<td width=\"15%s\">%s</td>\r\n", "%", visitor.szCarNum);
		fprintf(fp, "%s\r\n", "			</tr>");
		fprintf(fp, "%s\r\n", "			</table>");
		fprintf(fp, "%s\r\n", "			</td>");
		fprintf(fp, "%s\r\n", "		<tr>");
	}
	else
	{
		fprintf(fp, "%s\r\n", "		<tr>");
		fprintf(fp, "%s\r\n", "			<td width=\"26\%\">��λ����</td>");
		fprintf(fp, "			<td width=\"35%s\" colspan=\"2\">%s</td>\r\n", "%", visitor.szUnit);
		fprintf(fp, "%s\r\n", "		<tr>");
	}

	fprintf(fp, "%s\r\n", "		<tr>");
	fprintf(fp, "%s\r\n", "			<td width=\"26\%\">֤������</td>");
    CString str=visitor.szID;
	if (::GetPrivateProfileInt("TerminalSet", "bHideLast4ID",0, CONFIG_FILE)==1)
	{
		int n=strlen(visitor.szID);
		if (n == 18)
		{
			str=str.Left(14)+"****";
		}
	}
	fprintf(fp, "			<td width=\"35%s\" colspan=\"2\">%s</td>\r\n", "%", str);
	fprintf(fp, "%s\r\n", "		</tr>");
	fprintf(fp, "%s\r\n", "		<tr>");
	fprintf(fp, "%s\r\n", "			<td width=\"26\%\">���ò���</td>");
	fprintf(fp, "			<td width=\"35%s\" colspan=\"2\">%s</td>\r\n", "%", visitor.szToOfficeName);
	fprintf(fp, "%s\r\n", "		</tr>");
	fprintf(fp, "%s\r\n", "		<tr>");
	fprintf(fp, "%s\r\n", "			<td width=\"26\%\">������Ա</td>");
	fprintf(fp, "			<td width=\"35%s\" colspan=\"2\">%s</td>\r\n", "%", visitor.szToOfficialName);
	fprintf(fp, "%s\r\n", "		</tr>");
	fprintf(fp, "%s\r\n", "		<tr>");
	fprintf(fp, "%s\r\n", "			<td width=\"26\%\">�� �� ��</td>");
	fprintf(fp, "			<td width=\"35%s\" colspan=\"2\">%s</td>\r\n", "%", visitor.szToRoom);
	fprintf(fp, "%s\r\n", "		</tr>");
	
	char szUserName[50] = {0};
	::GetPrivateProfileString("TerminalSet", "UserName", "", szUserName, sizeof(szUserName), CONFIG_FILE);	
	fprintf(fp, "%s\r\n", "		<tr>");
	fprintf(fp, "%s\r\n", "			<td width=\"26\%\">ֵ �� ��</td>");
	fprintf(fp, "			<td width=\"35%s\" colspan=\"2\">%s</td>\r\n", "%", szUserName);
	fprintf(fp, "%s\r\n", "		</tr>");

	fprintf(fp, "%s\r\n", "	</table>");

	fprintf(fp, "%s\r\n", "	<table border=\"0\" cellpadding=\"0\" cellspacing=\"0\" width=\"240\" style=\"border-width: 0\">");
	fprintf(fp, "%s\r\n", "		<tr>");
	fprintf(fp, "%s\r\n", "			<td width=\"100%\" align=\"left\">������ǩ��</td>"); //������ǩ��
	fprintf(fp, "%s\r\n", "		</tr>");
	fprintf(fp, "%s\r\n", "		<tr>");
	if(1 ==::GetPrivateProfileInt("BarCode", "QR",0, CONFIG_FILE))
		fprintf(fp, "			<td width=\"80%\" align=\"left\"><img src=\"%s\"></td>\r\n", USER_QRCODE_PIC);
	else
		fprintf(fp, "			<td width=\"80%\" align=\"left\"><img src=\"%s\"></td>\r\n", USER_BARCODE_PIC);
	fprintf(fp, "%s\r\n", "		</tr>");

	fprintf(fp, "%s\r\n", "		<tr>");
	char szUnitName[128] = {'\0'};
	::GetPrivateProfileString("TerminalSet", "PrintFootLog", "", szUnitName, sizeof(szUnitName), CONFIG_FILE);
	fprintf(fp, "			<td width=\"100%s\"><b>%s</b></td>\r\n", "%", szUnitName);
	fprintf(fp, "%s\r\n", "		</tr>");
	
//	fprintf(fp, "%s\r\n", "		<tr>");
//	fprintf(fp, "%s\r\n", "			<td width=\"100%\" align=\"left\">*�����Ʊ��ܣ����Ž�������*</td>");
//	fprintf(fp, "%s\r\n", "		</tr>");
	fprintf(fp, "%s\r\n", "		</table>");
	fprintf(fp, "%s\r\n", "</div>");

	fprintf(fp, "%s\r\n", "</body>");

	fprintf(fp, "%s\r\n", "</html>");

	fclose(fp);

	return true;
}

//�����͵�
BOOL CreateReceiptHtml2(const VISITORINFO &visitor, char *pszFile)
{
	if(1 ==::GetPrivateProfileInt("TerminalSet", "HtmlReceiptTips",0, CONFIG_FILE))
	{
		CreateReceiptTipsPB(visitor);
		return true;
	}

	long i = 0;
	struct tm *tim = NULL;
	char HeadWords[40] = {'\0'};
	GetPrivateProfileString("HtmlPageTips", "HeadWords","��͵�",HeadWords, sizeof(HeadWords), CONFIG_FILE);
	int iFont=::GetPrivateProfileInt("HtmlPageTips", "HeadFont",6, CONFIG_FILE);
	if (iFont<1)
	{
		iFont=1;
	}
	else if (iFont>6)
	{
		iFont=6;
	}

	FILE *fp = fopen(pszFile, "wt");
	if (fp == NULL)
	{
		return false;
	}

	//time((time_t *)&i);
	if (visitor.lVisiteTime > 0)
	{
		i = visitor.lVisiteTime;
	}
	else
		i = g_Printvisitor.lVisiteTime;
	tim = localtime((time_t *)&i);

	fprintf(fp, "%s\r\n", "<html>");

	fprintf(fp, "%s\r\n", "<head>");
	fprintf(fp, "%s\r\n", "<meta http-equiv=\"Content-Type\" content=\"text/html; charset=gb2312\">");
	fprintf(fp, "%s\r\n", "<title></title>");
	fprintf(fp, "%s\r\n", "<style type=\"text/css\">");
	fprintf(fp, "%s\r\n", "table");
	fprintf(fp, "%s\r\n", "{");
	fprintf(fp, "%s\r\n", "	cellpadding: 0;");
	fprintf(fp, "%s\r\n", "	cellspacing: 0;");
	fprintf(fp, "%s\r\n", "}");
	fprintf(fp, "%s\r\n", "tr");
	fprintf(fp, "%s\r\n", "{");
	fprintf(fp, "%s\r\n", "	height: 25");
	fprintf(fp, "%s\r\n", "}");
	fprintf(fp, "%s\r\n", "td");
	fprintf(fp, "%s\r\n", "{");
	fprintf(fp, "%s\r\n", "	font-size: 14px");
	fprintf(fp, "%s\r\n", "}");
	fprintf(fp, "%s\r\n", "</style>");
	fprintf(fp, "%s\r\n", "</head>");

	fprintf(fp, "%s\r\n", "<body topmargin=\"0\" bottommargin=\"0\" style=\"overflow-y:hidden\">");

	fprintf(fp, "%s\r\n", "<div align=\"center\">");
	fprintf(fp, "%s\r\n", "	<table border=\"0\" style=\"border-collapse: collapse\" width=\"240\" cellpadding=\"0\">");
	fprintf(fp, "%s\r\n", "		<tr>");
	fprintf(fp, "%s\r\n", "			<td colspan=\"2\">");
	fprintf(fp, "			<p align=\"center\"><b><font size=\"%d\"> %s </font></b></td>",iFont,HeadWords);
	fprintf(fp, "%s\r\n", "		</tr>");
	fprintf(fp, "%s\r\n", "		<tr>");
	fprintf(fp, "%s\r\n", "			<td colspan=\"2\">");
	fprintf(fp, "			<p align=\"left\">����ʱ��: %04ld-%02ld-%02ld %02ld:%02ld:%02ld</td>\r\n", tim->tm_year + 1900, tim->tm_mon + 1, tim->tm_mday, tim->tm_hour, tim->tm_min, tim->tm_sec);
	fprintf(fp, "%s\r\n", "		</tr>");
	fprintf(fp, "%s\r\n", "		</table>");

	fprintf(fp, "%s\r\n", "	<table border=\"1\" style=\"border-collapse: collapse; table-layout:fixed\" width=\"240\" bordercolor=\"#000000\" cellpadding=\"0\">");
	fprintf(fp, "%s\r\n", "		<tr>");
	fprintf(fp, "%s\r\n", "			<td width=\"25\%\">�ÿ�����</td>");
	if (strlen(visitor.szName)>12)
	{
        fprintf(fp, "			<td width=\"30%s\"><p align=\"center\"><font size=\"1\">%s</font></td>\r\n", "%", visitor.szName);
	}
	else
	    fprintf(fp, "			<td width=\"30%s\"><p align=\"center\">%s</td>\r\n", "%", visitor.szName);
    char szUserPic [256];
	WIN32_FIND_DATA FindFileData;
	//if (!_access(g_Printvisitor.szUserIDPic,0))//if ( _access(file,0) )
	FindClose(FindFirstFile(g_Printvisitor.szUserIDPic,&FindFileData));
	if (FindFileData.nFileSizeLow>0&&!_access(g_Printvisitor.szUserIDPic,0))
	{
		strcpy(szUserPic,g_Printvisitor.szUserIDPic);
	}
	else if (!_access(g_Printvisitor.szUserHeadPic,0))
	{
		strcpy(szUserPic,g_Printvisitor.szUserHeadPic);
	}
	else
		strcpy(szUserPic,"��ͼƬ");
		
	fprintf(fp, "			<td width=\"36%s\" rowspan=\"4\"><img src=\"%s\" width=95 height=98></td>\r\n", "%", szUserPic);
	fprintf(fp, "%s\r\n", "		</tr>");
	fprintf(fp, "%s\r\n", "		<tr>");
	fprintf(fp, "%s\r\n", "			<td width=\"26\%\">��    ��</td>");
	fprintf(fp, "			<td width=\"30%s\" align=\"center\">%s</td>\r\n", "%", visitor.szSex);
	fprintf(fp, "%s\r\n", "		</tr>");
	fprintf(fp, "%s\r\n", "		<tr>");
	fprintf(fp, "%s\r\n", "			<td width=\"26\%\">��    ��</td>");
	fprintf(fp, "			<td width=\"30%s\" align=\"center\">%s</td>\r\n", "%", visitor.szFolk);
	fprintf(fp, "%s\r\n", "		</tr>");
	fprintf(fp, "%s\r\n", "		<tr>");
	fprintf(fp, "%s\r\n", "			<td width=\"26\%\">��    ��</td>");
	fprintf(fp, "			<td width=\"30%s\" align=\"center\">%s</td>\r\n", "%", visitor.szPhone);
	fprintf(fp, "%s\r\n", "		</tr>");

    if (g_PrintTipsCfg.bTotalPeoplePrint)
    {
		fprintf(fp, "%s\r\n", "		<tr>");
		fprintf(fp, "%s\r\n", "			<td width=\"26\%\">��������</td>");
		char szTotalPeople[8];
		strcpy(szTotalPeople,visitor.szTotalPeople);
		if (strcmp(visitor.szTotalPeople,"")==0)
		{
			strcpy(szTotalPeople,"1");
		}
		fprintf(fp, "			<td width=\"30%s\">%s</td>\r\n", "%", szTotalPeople);
		fprintf(fp, "%s\r\n", "		</tr>");
    }

	fprintf(fp, "%s\r\n", "		<tr>");
	fprintf(fp, "%s\r\n", "			<td width=\"26\%\">��������</td>");
	fprintf(fp, "			<td width=\"35%s\" colspan=\"2\" align=\"center\">%s</td>\r\n", "%", visitor.szReason);
	fprintf(fp, "%s\r\n", "		</tr>");

	if (g_PrintTipsCfg.bHideGoodsPrint)
	{
		fprintf(fp, "%s\r\n", "		<tr>");
		fprintf(fp, "%s\r\n", "			<td width=\"26\%\">Я����Ʒ</td>");
		fprintf(fp, "			<td width=\"35%s\" colspan=\"2\" align=\"center\">%s</td>\r\n", "%", visitor.szWithGoods);
		fprintf(fp, "%s\r\n", "		</tr>");
	}

	if(g_nIDType == ID_TYPE_Lawyer)
	{
		fprintf(fp, "%s\r\n", "		<tr>");
		fprintf(fp, "%s\r\n", "			<td width=\"26\%\">֪ͨ��</td>");
		fprintf(fp, "%s\r\n", "			<td colspan=\"2\" align=\"center\">");
		fprintf(fp, "%s\r\n", "			<table border=\"1\" style=\"border-collapse: collapse; table-layout:fixed\" width=174 height=25 bordercolor=\"#000000\" cellpadding=\"0\">");
		fprintf(fp, "%s\r\n", "			<tr height=25>");
		fprintf(fp, "			<td width=\"15%s\">%s</td>\r\n", "%", visitor.szUnit);
		fprintf(fp, "%s\r\n", "			<td width=\"70%s\" align=\"right\">��ʦ����������</td>");
		fprintf(fp, "			<td width=\"15%s\">%s</td>\r\n", "%", visitor.szCarNum);
		fprintf(fp, "%s\r\n", "			</tr>");
		fprintf(fp, "%s\r\n", "			</table>");
		fprintf(fp, "%s\r\n", "			</td>");
		fprintf(fp, "%s\r\n", "		<tr>");
	}
	else
	{
		fprintf(fp, "%s\r\n", "		<tr>");
		fprintf(fp, "%s\r\n", "			<td width=\"26\%\">��λ����</td>");
		fprintf(fp, "			<td width=\"35%s\" colspan=\"2\" align=\"center\">%s</td>\r\n", "%", visitor.szUnit);
		fprintf(fp, "%s\r\n", "		<tr>");
	}



	fprintf(fp, "%s\r\n", "			<td width=\"26\%\">֤������</td>");

    CString str=visitor.szID;
	if (g_PrintTipsCfg.bHideLast4ID)
	{
		int n=strlen(visitor.szID);
		if (n == 18)
		{
			str=str.Left(14)+"****";
		}
	}
	fprintf(fp, "			<td width=\"35%s\" colspan=\"2\" align=\"center\">%s</td>\r\n", "%", str);
	fprintf(fp, "%s\r\n", "		</tr>");
	fprintf(fp, "%s\r\n", "		<tr>");
	fprintf(fp, "%s\r\n", "			<td width=\"26\%\">���ò���</td>");
	fprintf(fp, "			<td width=\"35%s\" colspan=\"2\" align=\"center\">%s</td>\r\n", "%", visitor.szToOfficeName);
	fprintf(fp, "%s\r\n", "		</tr>");
	fprintf(fp, "%s\r\n", "		<tr>");
	fprintf(fp, "%s\r\n", "			<td width=\"26\%\">������Ա</td>");
	fprintf(fp, "			<td width=\"35%s\" colspan=\"2\" align=\"center\">%s</td>\r\n", "%", visitor.szToOfficialName);
	fprintf(fp, "%s\r\n", "		</tr>");

	if (g_PrintTipsCfg.bToRoomPrint)
	{
		fprintf(fp, "%s\r\n", "		<tr>");
		fprintf(fp, "%s\r\n", "			<td width=\"26\%\">�� �� ��</td>");
		fprintf(fp, "			<td width=\"35%s\" colspan=\"2\" align=\"center\">%s</td>\r\n", "%", visitor.szToRoom);
		fprintf(fp, "%s\r\n", "		</tr>");
	}

	//if (g_PrintTipsCfg.bToRoomPrint)
	{
		char szUserName[50] = {0};
		::GetPrivateProfileString("TerminalSet", "UserName", "", szUserName, sizeof(szUserName), CONFIG_FILE);

		fprintf(fp, "%s\r\n", "		<tr>");
		fprintf(fp, "%s\r\n", "			<td width=\"26\%\">ֵ �� ��</td>");
		fprintf(fp, "			<td width=\"35%s\" colspan=\"2\" align=\"center\">%s</td>\r\n", "%", szUserName);
		fprintf(fp, "%s\r\n", "		</tr>");
	}

	fprintf(fp, "%s\r\n", "	</table>");

	fprintf(fp, "%s\r\n", "	<table border=\"0\" cellpadding=\"0\" cellspacing=\"0\" width=\"240\" style=\"border-width: 0\">");
	fprintf(fp, "%s\r\n", "		<tr>");
	fprintf(fp, "%s\r\n", "			<td width=\"100%\" align=\"left\">������ǩ��</td>"); //������ǩ��
	fprintf(fp, "%s\r\n", "		</tr>");
	fprintf(fp, "%s\r\n", "		<tr>");

	if(1 ==::GetPrivateProfileInt("BarCode", "QR",0, CONFIG_FILE))
		fprintf(fp, "			<td align=\"right\"><img src=\"%s\" height=50 width=160></td>\r\n", USER_QRCODE_PIC);
	else
		fprintf(fp, "			<td align=\"right\"><img src=\"%s\" height=50 width=160></td>\r\n", USER_BARCODE_PIC);

	fprintf(fp, "%s\r\n", "		</tr>");


	if (strlen(g_PrintTipsCfg.szUnitName)>0)
	{
		fprintf(fp, "%s\r\n", "		<tr>");
		fprintf(fp, "			<td width=\"100%s\"><b>%s</b></td>\r\n", "%", g_PrintTipsCfg.szUnitName);
		fprintf(fp, "%s\r\n", "		</tr>");
	}

	fprintf(fp, "%s\r\n", "		</table>");
	fprintf(fp, "%s\r\n", "</div>");
	fprintf(fp, "%s\r\n", "</body>");
	fprintf(fp, "%s\r\n", "</html>");

	fclose(fp);

	return true;
}

BOOL CreateVisitorRequestHtml(const VISITORINFO &visitor, char *pszFile)
{
	long i = 0;
	struct tm *tim = NULL;

	FILE *fp = fopen(pszFile, "wt");
	if (fp == NULL)
	{
		return false;
	}

	time((time_t *)&i);
	tim = localtime((time_t *)&i);

	fprintf(fp, "%s\r\n", "<html>");

	fprintf(fp, "%s\r\n", "<head>");
	fprintf(fp, "%s\r\n", "<meta http-equiv=\"Content-Type\" content=\"text/html; charset=gb2312\">");
	fprintf(fp, "%s\r\n", "<title></title>");
	fprintf(fp, "%s\r\n", "<style type=\"text/css\">");
	fprintf(fp, "%s\r\n", "table");
	fprintf(fp, "%s\r\n", "{");
	fprintf(fp, "%s\r\n", "	cellpadding: 0;");
	fprintf(fp, "%s\r\n", "	cellspacing: 0;");
	fprintf(fp, "%s\r\n", "}");
	fprintf(fp, "%s\r\n", "tr");
	fprintf(fp, "%s\r\n", "{");
	fprintf(fp, "%s\r\n", "	height: 25");
	fprintf(fp, "%s\r\n", "}");
	fprintf(fp, "%s\r\n", "td");
	fprintf(fp, "%s\r\n", "{");
	fprintf(fp, "%s\r\n", "	font-size: 14px");
	fprintf(fp, "%s\r\n", "}");
	fprintf(fp, "%s\r\n", "</style>");
	fprintf(fp, "%s\r\n", "</head>");

	fprintf(fp, "%s\r\n", "<body topmargin=\"0\" bottommargin=\"0\" style=\"overflow-y:hidden\">");

	fprintf(fp, "%s\r\n", "<div align=\"center\">");
	fprintf(fp, "%s\r\n", "	<table border=\"0\" style=\"border-collapse: collapse\" width=\"509\" cellpadding=\"0\">");
	fprintf(fp, "%s\r\n", "		<tr>");
	fprintf(fp, "%s\r\n", "			<td width=\"442\" colspan=\"2\">");
	fprintf(fp, "%s\r\n", "			<p align=\"center\"><b><font size=\"4\">�������Ϣ</font></b></td>");
	fprintf(fp, "%s\r\n", "		</tr>");
	fprintf(fp, "%s\r\n", "		<tr>");
	fprintf(fp, "%s\r\n", "			<td colspan=\"2\">");
	fprintf(fp, "			<p align=\"left\">����ʱ��: %04ld-%02ld-%02ld %02ld:%02ld:%02ld</td>\r\n", tim->tm_year + 1900, tim->tm_mon + 1, tim->tm_mday, tim->tm_hour, tim->tm_min, tim->tm_sec);
	fprintf(fp, "%s\r\n", "		</tr>");
	fprintf(fp, "%s\r\n", "		</table>");

	fprintf(fp, "%s\r\n", "	<table border=\"1\" style=\"border-collapse: collapse; table-layout:fixed\" width=\"522\" bordercolor=\"#000000\" cellpadding=\"0\">");
	fprintf(fp, "%s\r\n", "		<tr>");
	fprintf(fp, "%s\r\n", "			<td width=\"14\%\">�ÿ�����</td>");
	fprintf(fp, "			<td width=\"22%s\">%s</td>\r\n", "%", visitor.szName);
    char szUserPic [256];
	
	if (!_access(visitor.szUserIDPic,0))//if ( _access(file,0) )
	{
		strcpy(szUserPic,visitor.szUserIDPic);
	}
	else if (!_access(visitor.szUserHeadPic,0))
	{
		strcpy(szUserPic,visitor.szUserHeadPic);
	}
	else
		strcpy(szUserPic,"��ͼƬ");
		
	fprintf(fp, "			<td width=\"18%s\" rowspan=\"4\"><img src=\"%s\"></td>\r\n", "%", szUserPic);
	//fprintf(fp, "			<td width=\"40%s\" colspan=\"9\"><div align=\"center\">����ͼƬ</div></td>\r\n");
	fprintf(fp, "			<td width=\"200\" colspan=\"9\"><div align=\"center\">����ͼƬ</div></td>\r\n");
	fprintf(fp, "%s\r\n", "		</tr>");
	fprintf(fp, "%s\r\n", "		<tr>");
	fprintf(fp, "%s\r\n", "			<td width=\"14\%\">��    ��</td>");
	fprintf(fp, "			<td width=\"22%s\">%s</td>\r\n", "%", visitor.szSex);
	char szUserVideoPic[256];
	if (!_access(visitor.szUserVideoPic,0))//if ( _access(file,0) )
	{
		strcpy(szUserVideoPic,visitor.szUserVideoPic);
	}
	else
		strcpy(szUserVideoPic,"��ͼƬ");
    fprintf(fp, "			<td width=\"46%s\" rowspan=\"12\" colspan=\"9\"><img src=\"%s\" align=\"absmiddle\" width=240 height=290></td>","%",szUserVideoPic);

	fprintf(fp, "%s\r\n", "		</tr>");
	fprintf(fp, "%s\r\n", "		<tr>");
	fprintf(fp, "%s\r\n", "			<td width=\"14\%\">��    ��</td>");
	fprintf(fp, "			<td width=\"22%s\">%s</td>\r\n", "%",visitor.szFolk);
	fprintf(fp, "%s\r\n", "		</tr>");
	fprintf(fp, "%s\r\n", "		<tr>");
	fprintf(fp, "%s\r\n", "			<td width=\"14\%\" height=\"16\">��������</td>");
	fprintf(fp, "			<td width=\"22%s\">%s</td>\r\n", "%",visitor.szTotalPeople);
	fprintf(fp, "%s\r\n", "		</tr>");
	fprintf(fp, "%s\r\n", "		<tr>");
	fprintf(fp, "%s\r\n", "			<td width=\"14\%\">��    ��</td>");
	fprintf(fp, "			<td width=\"22%s\" colspan=\"2\">%s</td>\r\n", "%",visitor.szPhone);
	fprintf(fp, "%s\r\n", "		</tr>");
	fprintf(fp, "%s\r\n", "		<tr>");
	fprintf(fp, "%s\r\n", "			<td width=\"14\%\">��������</td>");
	fprintf(fp, "			<td colspan=\"2\">%s</td>\r\n", visitor.szReason);
	fprintf(fp, "%s\r\n", "		</tr>");
	fprintf(fp, "%s\r\n", "		<tr>");
	fprintf(fp, "%s\r\n", "			<td width=\"14\%\">Я����Ʒ</td>");
	fprintf(fp, "			<td colspan=\"2\">%s</td>\r\n",  visitor.szWithGoods);
	fprintf(fp, "%s\r\n", "		</tr>");
	fprintf(fp, "%s\r\n", "		<tr>");
	fprintf(fp, "%s\r\n", "			<td width=\"14\%\">��λ����</td>");
	fprintf(fp, "			<td colspan=\"2\">%s</td>\r\n",  visitor.szUnit);
	fprintf(fp, "%s\r\n", "		</tr>");
	fprintf(fp, "%s\r\n", "		<tr>");
	fprintf(fp, "%s\r\n", "			<td width=\"14\%\">֤������</td>");
	fprintf(fp, "			<td colspan=\"2\">%s</td>\r\n",  visitor.szID);
	fprintf(fp, "%s\r\n", "		</tr>");
	fprintf(fp, "%s\r\n", "		<tr>");
	fprintf(fp, "%s\r\n", "			<td width=\"14\%\">���ò���</td>");
	fprintf(fp, "			<td colspan=\"2\">%s</td>\r\n",  visitor.szToOfficeName);
	fprintf(fp, "%s\r\n", "		</tr>");
	fprintf(fp, "%s\r\n", "		<tr>");
	fprintf(fp, "%s\r\n", "			<td width=\"14\%\">������Ա</td>");
	fprintf(fp, "			<td colspan=\"2\">%s</td>\r\n",  visitor.szToOfficialName);
	fprintf(fp, "%s\r\n", "		</tr>");
	fprintf(fp, "%s\r\n", "		<tr>");
	fprintf(fp, "%s\r\n", "			<td width=\"14\%\">�� �� ��</td>");
	fprintf(fp, "			<td colspan=\"2\">%s</td>\r\n",  visitor.szToRoom);
	fprintf(fp, "%s\r\n", "		</tr>");
	fprintf(fp, "%s\r\n", "	</table>");

	fprintf(fp, "%s\r\n", "</div>");

	fprintf(fp, "%s\r\n", "</body>");
	fprintf(fp, "%s\r\n", "</html>");
	fclose(fp);
	return true;	
}

BOOL CheckBlackName(const VISITORINFO &visitor)
{
    VISITORINFO v;
	bool b = false;
	strcpy(v.szName,visitor.szName);
	strcpy(v.szSex,visitor.szSex);
	strcpy(v.szID,visitor.szID);

	if (::GetPrivateProfileInt("TerminalSet", "CheckBlackNameVersion",0, CONFIG_FILE)==1)
	{
		return CheckIsBlackName2(v);
	}
	else
		return CheckIsBlackName(v);
}

BOOL CheckBlackNameNotes(const VISITORINFO &visitor,char *note)
{
    VISITORINFO v;
	bool b = false;
	strcpy(v.szName,visitor.szName);
	strcpy(v.szSex,visitor.szSex);
	strcpy(v.szID,visitor.szID);
	if (::GetPrivateProfileInt("TerminalSet", "CheckBlackNameVersion",0, CONFIG_FILE)==1)
	{
		//return CheckIsBlackName2(visitor);
		b = CheckIsBlackName2(v);
	}
	else
	{
		b = CheckIsBlackName(v);
	}
	strcpy(note,v.szNote);
	return b;
}

BOOL CheckIsBlackName2(VISITORINFO &visitor)
{
	wchar_t wBuf[256] = {0};
	char szT[512] = {'\0'};
	long FldCount = 0, ret = 0;
	CString outParas[FLD_MAX];
	CWinAdo tAdo(sCONNSTR_MYSQL);
	
	ret = tAdo.OpenAdo();
	if (ret != ADOOK)
	{
		return false;
	}

	
	for (int i = 0; i < FLD_MAX; i++)
	{
		outParas[i].Empty();
	}
	
	if (strlen(visitor.szName)>0)
	{
		if (strlen(visitor.szSex)>0)
		{
			if (strlen(visitor.szID)>0) //���֣��Ա�ID
			{
				sprintf(szT, "select count(*),black_notes from t_blackname where black_name = \'%s\' and black_id = \'%s\' and black_sex = \'%s\'",
		        visitor.szName, visitor.szID, visitor.szSex);
			    ret = tAdo.execSelect(szT, &FldCount, outParas);
				if (atoi(outParas[0]) >= 1)  //���֣��Ա�ID��ƥ��
				{
                    strcpy(visitor.szNote,outParas[1]);
					return true;
				}
				else  //�˻�ID
				{
					sprintf(szT, "select count(*),black_id,black_notes from t_blackname where black_name = \'%s\'  and black_sex = \'%s\'",
						visitor.szName, visitor.szSex);
					ret = tAdo.execSelect(szT, &FldCount, outParas);
					if (atoi(outParas[0]) >= 1) //���֣��Ա�ƥ��
					{
						if(strlen(outParas[1])>0) //ID��ƥ��
						{
                             return false;
						}
						strcpy(visitor.szNote,outParas[2]);
						return true;
					}
					else  //�˻��Ա�
					{
						sprintf(szT, "select count(*),black_sex,black_notes from t_blackname where black_name = \'%s\'",
							visitor.szName);
						ret = tAdo.execSelect(szT, &FldCount, outParas);
						if (atoi(outParas[0]) >= 1&&strcmp(outParas[1],visitor.szSex)==0)
						{
							strcpy(visitor.szNote,outParas[2]);
							return true;
						}
						else
							return false;
					}
				}
			}
			else   //���֣��Ա�
			{
				sprintf(szT, "select count(*),black_notes from t_blackname where black_name = \'%s\'  and black_sex = \'%s\'",
		        visitor.szName, visitor.szSex);
			}
		}
		else //����
		{
			sprintf(szT, "select count(*),black_notes from t_blackname where black_name = \'%s\'",
		        visitor.szName);
		}


	}
	else
		return false;  //��ƥ����Ϣ


	
	//sprintf(szT, "select * from t_blackname where black_name = \'%s\' or black_alias = \'%s\' or black_id = \'%s\' or black_mobile = \'%s\'", visitor.szName, visitor.szName, visitor.szID, visitor.szPhone);

	ret = tAdo.execSelect(szT, &FldCount, outParas);
	if (atoi(outParas[0]) >= 1)
	{
		strcpy(visitor.szNote,outParas[1]);
		return true;
	}
/*	if (ret == ADOERR || (FldCount == 0 && outParas[0].IsEmpty()))
	{
		return false;
	}*/

	return false;
}
BOOL CALLBACK OnOfficial1SetFocus(HELE hEle, HELE hEventEle)
{
    StartKeybd();
	SetOfficialEditBorder();
	XEle_SetBorderColor(hEle,RGB(255,0,0));
	ShowStandSoftKeybd();
	XWnd_RedrawWnd(g_hWindow);
	return true;
}
BOOL CALLBACK OnBlackNameSetFocus(HELE hEle, HELE hEventEle)
{
    StartKeybd();
	ShowStandSoftKeybd();
	SetBlackNameEditBorder();
	XEle_SetBorderColor(hEle,RGB(255,0,0));
	XWnd_RedrawWnd(g_hWindow);
	return true;
}
BOOL CALLBACK OnClearPWD(HELE hEle, HELE hEventEle)
{
	XEdit_SetText(XWnd_GetEle(g_hWindow, IDE_ADD_CZYMM),L"");
	SetOtherDlgEditBorder();
    XEle_SetBorderColor(hEle,RGB(255,0,0));
	XWnd_RedrawWnd(g_hWindow);
	return true;
}
BOOL CALLBACK OnOtherSetFocus(HELE hEle, HELE hEventEle)
{
    StartKeybd();
	ShowStandSoftKeybd();
	
	SetOtherDlgEditBorder();
    XEle_SetBorderColor(hEle,RGB(255,0,0));
	XWnd_RedrawWnd(g_hWindow,TRUE);
	return true;
}

BOOL CALLBACK OnVisitorSetFocus2(HELE hEle, UINT flags, POINT *pPt)
{
	::WritePrivateProfileString("TerminalSet", "DataInputPage", "2", CONFIG_FILE);
	StartKeybd();

	SetMainDlgEditBorder();
    XEle_SetBorderColor(hEle,RGB(255,0,0));
	ShowVisitorSoftKeybd();
	if (XEle_GetId(hEle) == IDE_VISITORREASON) //����
	{
        ShowReasonbd();
	}
	else if (XEle_GetId(hEle) == IDE_VISITORTHING) //Я����Ʒ
	{
		ShowGoodsbd();
	}
	else if(XEle_GetId(hEle) == IDE_VISITORUNIT)  //�ÿ͵�λ
	{
		ShowCompanybd();
	}

	XWnd_RedrawWnd(g_hWindow);
	return false;
}

BOOL CALLBACK OnVisitorSetFocus(HELE hEle, HELE hEventEle) 
{
	::WritePrivateProfileString("TerminalSet", "DataInputPage", "2", CONFIG_FILE);
	StartKeybd();

	SetMainDlgEditBorder();
    XEle_SetBorderColor(hEle,RGB(255,0,0));
	ShowVisitorSoftKeybd();
	if (XEle_GetId(hEle) == IDE_VISITORREASON) //����
	{
        ShowReasonbd();
	}
	else if (XEle_GetId(hEle) == IDE_VISITORTHING) //Я����Ʒ
	{
		ShowGoodsbd();
	}
	else if(XEle_GetId(hEle) == IDE_VISITORUNIT)  //�ÿ͵�λ
	{
		ShowCompanybd();
	}

	XWnd_RedrawWnd(g_hWindow);
	return false;
}

BOOL CALLBACK OnShowVisitorType(HELE hEle, HELE hEventEle)
{
	/*
	StartKeybd();
	SetMainDlgEditBorder();
	SetOtherDlgEditBorder();
    XEle_SetBorderColor(hEle,RGB(255,0,0));
	HideSoftKeybd();
	if (::GetPrivateProfileInt("TerminalSet", "bVisitorReadOnly",0, CONFIG_FILE)==1)  //���õǼ�֤������ֻ��
	{
		XWnd_RedrawWnd(g_hWindow);
	    return true;
	}
    */
	XEle_SetFocus(hEle,false);
	g_hOfficialWin = XModalWnd_CreateWindow(650, 610, L"�ÿ������б�", XWnd_GetHWnd(g_hWindow));
	XWnd_SetBorderSize(g_hOfficialWin, 3, 3, 3, 3);

	HIMAGE hImg = XImage_LoadFile(L".\\skin\\image\\frame.jpg", true);
	XWnd_SetImageNC(g_hOfficialWin, hImg);
	HELE hList = XList_Create(10, 0, 620, 570, g_hOfficialWin);

	XEle_RegisterEvent(hList, XE_LIST_SELECT, OnSelectVisitorType);


	
	XList_SetHeaderHeight(hList, 30);	
	XList_EnableGrid(hList, false);

    XList_SetItemHeight(hList,40);
	//XList_AddColumn(hList, 40, L"���");
	XList_AddColumn(hList, 580, L"����");

    XList_AddItem(hList, L"�ڲ��ÿ�", 0);
	XList_AddItem(hList, L"�ⲿ�ÿ�", 1);
	XList_AddItem(hList, L"ʩ����Ա", 2);
	XList_AddItem(hList, L"��ѵ��Ա", 3);
	XList_AddItem(hList, L"�ͻ���Ա", 4);
	XList_AddItem(hList, L"�����Ա", 5);
	XList_AddItem(hList, L"��ˮ��Ա", 6);
	XList_AddItem(hList, L"����", 7);
//	XList_AddItem(hList, L"��ʦ֤", 8);
//	XList_AddItem(hList, L"�������֤", 8);

	/*int n = 0;
	
	do 
	{
		memset(szT, 0, sizeof(szT));
		memset((char *)wBuf, 0, sizeof(wBuf));
		
		sprintf(szT, "%d", n + 1);		//���
		S_2_W(wBuf, szT, 256);
		XList_AddItem(hList, wBuf, n);
			
		L_V_I(outParas[0], n, 1)
	//	L_V_I(outParas[1], n, 2)

		n++;
	} while (n<);*/
    XModalWnd_DoModal(g_hOfficialWin);
    return true;
}

BOOL CALLBACK OnShowIDType(HELE hEle, HELE hEventEle)
{
	StartKeybd();
	SetMainDlgEditBorder();
	SetOtherDlgEditBorder();
    XEle_SetBorderColor(hEle,RGB(255,0,0));
	HideSoftKeybd();
	if (::GetPrivateProfileInt("TerminalSet", "bVisitorReadOnly",0, CONFIG_FILE)==1)  //���õǼ�֤������ֻ��
	{
		XWnd_RedrawWnd(g_hWindow);
	    return true;
	}

	XEle_SetFocus(hEle,false);
	g_hOfficialWin = XModalWnd_CreateWindow(650, 610, L"֤�������б�", XWnd_GetHWnd(g_hWindow));
	XWnd_SetBorderSize(g_hOfficialWin, 3, 3, 3, 3);

	HIMAGE hImg = XImage_LoadFile(L".\\skin\\image\\frame.jpg", true);
	XWnd_SetImageNC(g_hOfficialWin, hImg);
	HELE hList = XList_Create(10, 0, 620, 570, g_hOfficialWin);

	XEle_RegisterEvent(hList, XE_LIST_SELECT, OnSelectIDType);


	
	XList_SetHeaderHeight(hList, 30);	
	XList_EnableGrid(hList, false);

    XList_SetItemHeight(hList,40);
	//XList_AddColumn(hList, 40, L"���");
	XList_AddColumn(hList, 580, L"֤��");

    XList_AddItem(hList, L"�������֤", 0);
	XList_AddItem(hList, L"һ�����֤", 1);
	XList_AddItem(hList, L"����", 2);
	XList_AddItem(hList, L"��½����۰�ͨ��֤", 3);
	XList_AddItem(hList, L"�������֤", 4);
	XList_AddItem(hList, L"̨��֤", 5);
	XList_AddItem(hList, L"��ʻ֤", 6);
	XList_AddItem(hList, L"����֤��", 7);
	XList_AddItem(hList, L"��ʦ֤", 8);
	XList_AddItem(hList, L"��Ƭ", 9);
	XList_AddItem(hList, L"����֤��", 10);
//	XList_AddItem(hList, L"�������֤", 8);

	/*int n = 0;
	
	do 
	{
		memset(szT, 0, sizeof(szT));
		memset((char *)wBuf, 0, sizeof(wBuf));
		
		sprintf(szT, "%d", n + 1);		//���
		S_2_W(wBuf, szT, 256);
		XList_AddItem(hList, wBuf, n);
			
		L_V_I(outParas[0], n, 1)
	//	L_V_I(outParas[1], n, 2)

		n++;
	} while (n<);*/
    XModalWnd_DoModal(g_hOfficialWin);
    return true;
}
BOOL CALLBACK OnShowFolk(HELE hEle, HELE hEventEle)
{
	StartKeybd();
	SetMainDlgEditBorder();
	SetOtherDlgEditBorder();
    XEle_SetBorderColor(hEle,RGB(255,0,0));
	HideSoftKeybd();
    if (::GetPrivateProfileInt("TerminalSet", "bVisitorReadOnly",0, CONFIG_FILE)==1)  //���õǼ�֤������ֻ��
	{
		XWnd_RedrawWnd(g_hWindow);
		return true;
	}

	XEle_SetFocus(hEle,false);
	g_hOfficialWin = XModalWnd_CreateWindow(650, 610, L"�����б�", XWnd_GetHWnd(g_hWindow));
	XWnd_SetBorderSize(g_hOfficialWin, 3, 3, 3, 3);

	HIMAGE hImg = XImage_LoadFile(L".\\skin\\image\\frame.jpg", true);
	XWnd_SetImageNC(g_hOfficialWin, hImg);
	HELE hList = XList_Create(10, 0, 620, 570, g_hOfficialWin);

	XEle_RegisterEvent(hList, XE_LIST_SELECT, OnSelectFolk);


	
	XList_SetHeaderHeight(hList, 30);	
	XList_EnableGrid(hList, false);


	XList_AddColumn(hList, 580, L"����");


	int n = 0;
	wchar_t wBuf[256] = {0};
	do 
	{
		/*memset(szT, 0, sizeof(szT));
		memset((char *)wBuf, 0, sizeof(wBuf));
		
		sprintf(szT, "%d", n + 1);		//���*/
		S_2_W(wBuf, (LPSTR)(LPCTSTR)sFolk[n], 256);
		XList_AddItem(hList, wBuf, n);
			
		//L_V_I(outParas[0], n, 1)
	//	L_V_I(outParas[1], n, 2)

		n++;
	} while (n<62);
    XModalWnd_DoModal(g_hOfficialWin);

    return true;
}
BOOL CALLBACK OnHideKeyBdSetFocus(HELE hEle, HELE hEventEle)
{
	StartKeybd();
	SetMainDlgEditBorder();
	SetOtherDlgEditBorder();
    XEle_SetBorderColor(hEle,RGB(255,0,0));
	HideSoftKeybd();
	XWnd_RedrawWnd(g_hWindow);
	return true;
}

BOOL CALLBACK OnOfficialKillFocus(HELE hEle, HELE hEventEle)
{
    int n=XEle_GetId(hEle);
	if (n==IDE_MASTERNAME)  //���������ͱ�����ȷ����Ϣ
	{
		//BackScreenSelectOfficial();  //��ʾ���������ѯ������
	}
	return true;
}

void SetOfficialEditBorder()
{
	for(int i=5030;i<5038;i++)
	{
        XEle_SetBorderColor(GETELE(i),RGB(0,0,0));
	}
	XEle_SetBorderColor(GETELE(5101),RGB(0,0,0)); 
}
void SetBlackNameEditBorder()
{
	for (int i=5038;i<5047;i++)
	{
		XEle_SetBorderColor(GETELE(i),RGB(0,0,0));
	}
}
void SetOtherDlgEditBorder()
{
	int i;
	for (i=5047;i<5058;i++)
	{
		XEle_SetBorderColor(GETELE(i),RGB(0,0,0));
	}

	for (i=5060;i<5067;i++)
	{
		 XEle_SetBorderColor(GETELE(i),RGB(0,0,0));
	}
	for (i=5070;i<5075;i++)
	{
		XEle_SetBorderColor(GETELE(i),RGB(0,0,0));
	}
	for (i=5716;i<5720;i++)
	{
		XEle_SetBorderColor(GETELE(i),RGB(0,0,0));
	}
    //XEle_SetBorderColor(htxtChaxunPwd,RGB(0,0,0));
}
void SetMainDlgEditBorder()
{
	int i;
	for (i = 5003; i <= 5012; i++)		//��������Ϣ�������ԴID
	{
		//XEle_RegisterEvent(XWnd_GetEle(g_hWindow, i), XE_SETFOCUS, OnVisitorSetFocus);
		XEle_SetBorderColor(GETELE(i),RGB(0,0,0));
	}
	for (i = 5020; i <= 5024; i++)		//��������Ϣ�������ԴID
	{
		XEle_SetBorderColor(GETELE(i),RGB(0,0,0));
	}

	for (i = 5013; i <= 5019; i++)		//���÷�����Ϣ�������ԴID
	{
		XEle_SetBorderColor(GETELE(i),RGB(0,0,0));
	}
}

BOOL CALLBACK OnOfficialSetFocus(HELE hEle, UINT flags, POINT *pPt) 
{

	::WritePrivateProfileString("TerminalSet", "DataInputPage", "1", CONFIG_FILE);
	StartKeybd();

	SetMainDlgEditBorder();
    XEle_SetBorderColor(hEle,RGB(255,0,0));
	ShowOfficialSoftKeybd();
    if (XEle_GetId(hEle) == IDE_MASTERNAME)
    {
        ShowOfficialbd();
    }
	else if (XEle_GetId(hEle) == IDE_OFFICENAME)
	{
		ShowOfficeNamebd();
	}
	else if (XEle_GetId(hEle) == IDE_OFFICEPHONE)
	{
		ShowOfficePhonebd();
	}
	else if (XEle_GetId(hEle) == IDC_MASTERZHIWU)
	{
		ShowDutybd();
	}
	else if (XEle_GetId(hEle) == IDE_ROOMNUM)
	{
		ShowRoombd();
	}
	else if (XEle_GetId(hEle) == IDC_MASTERSSO)
	{
		ShowSSObd();
	}
	XWnd_RedrawWnd(g_hWindow);
	return false;
}
BOOL EmptyOfficialPara()
{
    return true;
}

BOOL EmptyVisitorPara()
{
    HELE hEle = NULL;
	HIMAGE hImg = NULL;
	wchar_t wBuf[256] = {0};
	hEle = XWnd_GetEle(g_hWindow, IDP_ID2_HEADPIC);	//�ͷŶ���֤�ϵ�user_head_pic.bmpͼƬ��Դ
	XPic_SetImage(hEle, NULL);
	
	hEle = XWnd_GetEle(g_hWindow, IDP_HEADPIC);		//�ͷ�user_head_pic.bmpͼƬ��Դ
	XPic_SetImage(hEle, NULL);
	
	hEle = XWnd_GetEle(g_hWindow, IDP_HEADPIC2);		//�ͷ�user_head_pic.bmpͼƬ��Դ
	XPic_SetImage(hEle, NULL);

	hEle = XWnd_GetEle(g_hWindow, IDS_ID2_NAME);
	XStatic_SetText(hEle, L"");
	
	hEle = XWnd_GetEle(g_hWindow, IDS_ID2_SEX);
	XStatic_SetText(hEle, L"");
	
	hEle = XWnd_GetEle(g_hWindow, IDS_ID2_FOLK);
	XStatic_SetText(hEle, L"");
	
	hEle = XWnd_GetEle(g_hWindow, IDS_ID2_BIRTHYEAR);
	XStatic_SetText(hEle, L"");
	
	hEle = XWnd_GetEle(g_hWindow, IDS_ID2_BIRTHMONTH);
	XStatic_SetText(hEle, L"");
	
	hEle = XWnd_GetEle(g_hWindow, IDS_ID2_BIRTHDAY);
	XStatic_SetText(hEle, L"");
	
	hEle = XWnd_GetEle(g_hWindow, IDS_ID2_ADDR);
	//XEdit_SetText(hEle, L"");
	XStatic_SetText(hEle, L"");
	
	hEle = XWnd_GetEle(g_hWindow, IDS_ID2_ID);
	XStatic_SetText(hEle, L"");
	
	hEle = XWnd_GetEle(g_hWindow, IDS_ID2_SIGNDEPARTMENT);
	XStatic_SetText(hEle, L"");
	
	hEle = XWnd_GetEle(g_hWindow, IDS_ID2_VALIDTERM);
	XStatic_SetText(hEle, L"");
	
	hEle = XWnd_GetEle(g_hWindow, IDG_ID2);			//����֤���
	XEle_ShowEle(hEle, false);
		
	hEle = XWnd_GetEle(g_hWindow, IDG_PASSPORT);	//�������
	XEle_ShowEle(hEle, true);

	g_nID2ReadType = ID2_NO;

	BOOL bSend=::GetPrivateProfileInt("TerminalSet", "bSendRequest",0, CONFIG_FILE);
	if (bSend==1)  //�򿪷�����Ƶ���ܣ���ʼΪ�����ܷ���״̬
	{
		g_bAcceptVisite=false;
	}
	else
		g_bAcceptVisite=true;

	if(g_nIDType == ID_TYPE_Lawyer) //ɨ����ʦ֤ʱ��ʾ��ʦ�����Ŀ
	{
        XEle_ShowEle(XWnd_GetEle(g_hWindow, IDS_Notice), true);
		XEle_ShowEle(XWnd_GetEle(g_hWindow, IDC_Notice_Comb), true);
		XEle_ShowEle(XWnd_GetEle(g_hWindow, IDS_LawyerLetter), true);
		XEle_ShowEle(XWnd_GetEle(g_hWindow, IDC_LawyerLetter_Comb), true);
		XComboBox_DeleteItemAll(GETELE(IDC_Notice_Comb));
	    XComboBox_AddString(GETELE(IDC_Notice_Comb), L"��");
	    XComboBox_AddString(GETELE(IDC_Notice_Comb), L"��");
		XComboBox_SetSelectItem(GETELE(IDC_Notice_Comb), 0);

		XComboBox_DeleteItemAll(GETELE(IDC_LawyerLetter_Comb));
	    XComboBox_AddString(GETELE(IDC_LawyerLetter_Comb), L"��");
	    XComboBox_AddString(GETELE(IDC_LawyerLetter_Comb), L"��");
		XComboBox_SetSelectItem(GETELE(IDC_LawyerLetter_Comb), 0);

		XEle_ShowEle(XWnd_GetEle(g_hWindow, IDS_VISITORUNIT), false);
		XEle_ShowEle(XWnd_GetEle(g_hWindow, IDE_VISITORUNIT), false);
		XEle_ShowEle(XWnd_GetEle(g_hWindow, IDS_CARNUM), false);
		XEle_ShowEle(XWnd_GetEle(g_hWindow, IDE_VISITORCARNUM), false);
	}
	else
	{
        XEle_ShowEle(XWnd_GetEle(g_hWindow, IDS_Notice), false);
		XEle_ShowEle(XWnd_GetEle(g_hWindow, IDC_Notice_Comb), false);
		XEle_ShowEle(XWnd_GetEle(g_hWindow, IDS_LawyerLetter), false);
		XEle_ShowEle(XWnd_GetEle(g_hWindow, IDC_LawyerLetter_Comb), false);

        XEle_ShowEle(XWnd_GetEle(g_hWindow, IDS_VISITORUNIT), true);
		XEle_ShowEle(XWnd_GetEle(g_hWindow, IDE_VISITORUNIT), true);
		XEle_ShowEle(XWnd_GetEle(g_hWindow, IDS_CARNUM), true);
		XEle_ShowEle(XWnd_GetEle(g_hWindow, IDE_VISITORCARNUM), true);

	}

	//::WritePrivateProfileString("Info", "bStopRead", "0", ".\\FaceCompare\\faceCompare.ini");
	g_bStopCompare = true;
	memset((char *)&g_Printvisitor, 0, sizeof(VISITORINFO));
	g_bFaceCompare = true;
	return true;
}

BOOL CheckID2(const VISITORINFO &visitor)
{
	HELE hEle = NULL;
	char szTmp[128] = {'\0'},szFolk[128] = {'\0'};
	wchar_t wBuf[256] = {0},wTmp[256]={0};
	CString sTips;

	hEle = XWnd_GetEle(g_hWindow, IDS_ID2_NAME);
	W_2_S(XStatic_GetText(hEle), szTmp, 128);
	if (strcmp(szTmp,visitor.szName)!=0) //����
	{
		sTips="��ȡֵ��";
		sTips+=szTmp;
		sTips+="����ʼֵ��";
		sTips+=visitor.szName;
		AfxMessageBox(sTips+"��һ�£�");
		return false;
	}

	memset((char *)wBuf, 0, sizeof(wBuf));
	hEle = XWnd_GetEle(g_hWindow, IDS_ID2_SEX);
	W_2_S(XStatic_GetText(hEle), szTmp, 128);
	if (strcmp(szTmp,visitor.szSex)!=0) //�ձ�
	{
		sTips="��ȡֵ��";
		sTips+=szTmp;
		sTips+="����ʼֵ��";
		sTips+=visitor.szSex;
		AfxMessageBox(sTips+"��һ�£�");
		return false;
	}

	memset((char *)wBuf, 0, sizeof(wBuf));
	hEle = XWnd_GetEle(g_hWindow, IDS_ID2_FOLK);
	W_2_S(XStatic_GetText(hEle), szTmp, 128);

	hEle = XWnd_GetEle(g_hWindow,IDC_VISITORFOLK);
	XEdit_GetText(hEle,wBuf,128);
	W_2_S(wBuf, szFolk, 128);
	
	if (strcmp(szTmp,szFolk)!=0) //����
	{
		sTips="��ȡֵ��";
		sTips+=szTmp;
		sTips+="����ʼֵ��";
		sTips+=szFolk;
		AfxMessageBox(sTips+"��һ�£�");
		return false;
	}

	memset((char *)wBuf, 0, sizeof(wBuf));
	hEle = XWnd_GetEle(g_hWindow, IDS_ID2_ADDR);
	W_2_S(XStatic_GetText(hEle), szTmp, 128);
	CString str=szTmp;
	if (strlen(str)>=44)
	{
		str.Delete(42,2);
        str.Delete(20,2);
	}
	else
		str.Delete(20,2);

	
	
	memset((char *)wBuf, 0, sizeof(wBuf));
	hEle = XWnd_GetEle(g_hWindow, IDS_ID2_ID);
	W_2_S(XStatic_GetText(hEle), szTmp, 128);
	if (strcmp(szTmp,visitor.szID)!=0) //֤������
	{
		sTips="��ȡֵ��";
		sTips+=szTmp;
		sTips+="����ʼֵ��";
		sTips+=visitor.szID;
		AfxMessageBox(sTips+"��һ�£�");
		return false;
	}

	memset((char *)wBuf, 0, sizeof(wBuf));
	hEle = XWnd_GetEle(g_hWindow, IDS_ID2_SIGNDEPARTMENT);
	W_2_S(XStatic_GetText(hEle), szTmp, 128);
	if (strcmp(szTmp,visitor.szSignDepartment)!=0) //��֤����
	{
		sTips="��ȡֵ��";
		sTips+=szTmp;
		sTips+="����ʼֵ��";
		sTips+=visitor.szSignDepartment;
		AfxMessageBox(sTips+"��һ�£�");
		return false;
	}
	
	if (atoi(visitor.szIDType) != 1)		//����֤
	{
		return false;
	}
	return true;
}

BOOL ShowID2(BOOL bShow, const VISITORINFO &visitor)
{
	HELE hEle = NULL;
	HIMAGE hImg = NULL;
	wchar_t wBuf[256] = {0},wAddr[256] = {0};
	if (bShow)  //��Ӧ�õ�����Ϣ
	{
		hEle = XWnd_GetEle(g_hWindow, IDP_ID2PIC);
		hImg = XImage_LoadFile(L".\\skin\\image\\id2.bmp", true);
		//S_2_W(wBuf, (char *)visitor.szUserVideoPic, 256);
        //hImg = XImage_LoadFile(wBuf, true);
		XPic_SetImage(hEle, hImg);

		hEle = XWnd_GetEle(g_hWindow, IDS_ID2_NAME);
		S_2_W(wBuf, (char *)visitor.szName, 128);
		XStatic_SetText(hEle, wBuf);

		memset((char *)wBuf, 0, sizeof(wBuf));
		hEle = XWnd_GetEle(g_hWindow, IDS_ID2_SEX);
		S_2_W(wBuf, (char *)visitor.szSex, 128);
		XStatic_SetText(hEle, wBuf);

		memset((char *)wBuf, 0, sizeof(wBuf));
		hEle = XWnd_GetEle(g_hWindow, IDS_ID2_FOLK);
		S_2_W(wBuf, (char *)visitor.szFolk, 128);
		XStatic_SetText(hEle, wBuf);

		char sz[128] = {'\0'};
		memset((char *)wBuf, 0, sizeof(wBuf));
		hEle = XWnd_GetEle(g_hWindow, IDS_ID2_BIRTHYEAR);
		memcpy(sz, (char *)visitor.szBirth, 4);
		S_2_W(wBuf, sz, 32);
		XStatic_SetText(hEle, wBuf);

		memset((char *)wBuf, 0, sizeof(wBuf));
		memset(sz, 0, sizeof(sz));
		hEle = XWnd_GetEle(g_hWindow, IDS_ID2_BIRTHMONTH);
		memcpy(sz, (char *)visitor.szBirth + 4 , 2);
		S_2_W(wBuf, sz, 32);
		XStatic_SetText(hEle, wBuf);

		memset((char *)wBuf, 0, sizeof(wBuf));
		memset(sz, 0, sizeof(sz));
		hEle = XWnd_GetEle(g_hWindow, IDS_ID2_BIRTHDAY);
		memcpy(sz, (char *)visitor.szBirth + 4 + 2 , 2);
		S_2_W(wBuf, sz, 32);
		XStatic_SetText(hEle, wBuf);

		memset((char *)wBuf, 0, sizeof(wBuf));
		memset(sz, 0, sizeof(sz));
		hEle = XWnd_GetEle(g_hWindow, IDS_ID2_ADDR);
        strcpy(sz,visitor.szAddr);
		S_2_W(wAddr, sz, 128);
		int len = 0, sLen = wcslen(wAddr);
		int n = sLen % 11;
		if (n == 0)
		{
			n = sLen / 11;
		}
		else
		{
			n = sLen / 11 + 1;
		}
		for (int i = 0; i < n; i++)
		{
			if (i == n - 1)
			{
				memcpy(wBuf + len, wAddr + 11 * i, 2*(sLen - 11 * (n - 1)));
				len += sLen - 10 * (n - 1);
			}
			else
			{
				memcpy(wBuf + len, wAddr + 11 * i, 22);
				len += 11;
				memcpy(wBuf + len, L"\r\n", 4);
				len += 2;
			}
		}
		//S_2_W(wBuf, sz, 128);
		XStatic_SetText(hEle, wBuf);

		memset((char *)wBuf, 0, sizeof(wBuf));
		hEle = XWnd_GetEle(g_hWindow, IDS_ID2_ID);
		S_2_W(wBuf, (char *)visitor.szID, 128);
		XStatic_SetText(hEle, wBuf);

		memset((char *)wBuf, 0, sizeof(wBuf));
		hEle = XWnd_GetEle(g_hWindow, IDS_ID2_SIGNDEPARTMENT);
		S_2_W(wBuf, (char *)visitor.szSignDepartment, 128);
		XStatic_SetText(hEle, wBuf);

		memset((char *)wBuf, 0, sizeof(wBuf));
		hEle = XWnd_GetEle(g_hWindow, IDS_ID2_VALIDTERM);
		S_2_W(wBuf, (char *)visitor.szValidDate, 128);
		XStatic_SetText(hEle, wBuf);


		memset((char *)wBuf, 0, sizeof(wBuf));
		hEle = XWnd_GetEle(g_hWindow, IDP_ID2_HEADPIC);
		S_2_W(wBuf, (char *)visitor.szUserIDPic, 256);
		hImg = XImage_LoadFile(wBuf, true);
		XPic_SetImage(hEle, hImg);

		memset((char *)wBuf, 0, sizeof(wBuf));
		hEle = XWnd_GetEle(g_hWindow, IDP_HEADPIC);
		S_2_W(wBuf, (char *)visitor.szUserIDPic, 256);
		hImg = XImage_LoadFile(wBuf, true);
		XPic_SetImage(hEle, hImg);

		if (strlen(visitor.szUserHeadPic)>0)
		{
			memset((char *)wBuf, 0, sizeof(wBuf));
			hEle = XWnd_GetEle(g_hWindow, IDP_HEADPIC2);
			S_2_W(wBuf, (char *)visitor.szUserHeadPic, 256);
			hImg = XImage_LoadFile(wBuf, true);
			XPic_SetImage(hEle, hImg);
		}
		else
		{
			 hEle = XWnd_GetEle(g_hWindow, IDP_HEADPIC2);		
	         XPic_SetImage(hEle, NULL);
		}
	
        if (strlen(visitor.szUserVideoPic)>0)
		{
			memset((char *)wBuf, 0, sizeof(wBuf));
			hEle = XWnd_GetEle(g_hWindow, IDP_CAMERA);
			S_2_W(wBuf, (char *)visitor.szUserVideoPic, 256);
			hImg = XImage_LoadFile(wBuf, true);
			XPic_SetImage(hEle, hImg);
		}
		else
		{
			hEle = XWnd_GetEle(g_hWindow, IDP_CAMERA);		
			XPic_SetImage(hEle, NULL);
		}

		hEle = XWnd_GetEle(g_hWindow, IDG_ID2);			//����֤���
		XEle_ShowEle(hEle, bShow);
		
		hEle = XWnd_GetEle(g_hWindow, IDG_PASSPORT);	//�������
		XEle_ShowEle(hEle, !bShow);
	}
	else    //ɨ��õ�����Ϣ
	{
		hEle = XWnd_GetEle(g_hWindow, IDG_ID2);
		XEle_ShowEle(hEle, bShow);

		hEle = XWnd_GetEle(g_hWindow, IDG_PASSPORT);
		XEle_ShowEle(hEle, !bShow);

		memset((char *)wBuf, 0, sizeof(wBuf));
		hEle = XWnd_GetEle(g_hWindow, IDP_CAMERA);
		S_2_W(wBuf, (char *)visitor.szUserVideoPic, 256);
		hImg = XImage_LoadFile(wBuf, true);
		XPic_SetImage(hEle, hImg);

		memset((char *)wBuf, 0, sizeof(wBuf));
		hEle = XWnd_GetEle(g_hWindow, IDP_HEADPIC);		//֤��ͷ��ͼ
		S_2_W(wBuf, (char *)visitor.szUserIDPic, 256);
		hImg = XImage_LoadFile(wBuf, true);
		XPic_SetImage(hEle, hImg);

		memset((char *)wBuf, 0, sizeof(wBuf));
		hEle = XWnd_GetEle(g_hWindow, IDP_HEADPIC2);
		S_2_W(wBuf, (char *)visitor.szUserHeadPic, 256);
		hImg = XImage_LoadFile(wBuf, true);
		XPic_SetImage(hEle, hImg);

		memset((char *)wBuf, 0, sizeof(wBuf));
		hEle = XWnd_GetEle(g_hWindow, IDP_PASSPORTPIC);
		S_2_W(wBuf, (char *)visitor.szUserIDFullPic, 256);
		hImg = XImage_LoadFile(wBuf, true);
		XPic_SetImage(hEle, hImg);
	}

	HWND hWnd = ::FindWindow(NULL, "����");
	if (hWnd != NULL)
	{
		//CString str=visitor.szUserIDPic;
		::WritePrivateProfileString("OfficialInfo", "VisistorPic", visitor.szUserIDPic, CONFIG_FILE);
		::PostMessage(hWnd, WM_SHOW_HEADPIC, NULL, NULL);
	}
	return true;
}

BOOL CALLBACK OnIDTypeCombSelect(HELE hEle, HELE hEventEle, int nIndex) 
{
	return false;
}
//��ѯͳ��ҳ���ϵ���б����һ��
BOOL CALLBACK OnChaxunTongjiList(HELE hEle, HELE hEventEle, int nIndex) 
{
	if (hEle != hEventEle)
	{
		return false;
	}

	if (nIndex == -1)
	{
		return false;
	}

	if (g_nChaXunTongJiType == BY_TONGJI)		//ͳ�ƽ���в�������Ӧ
	{
		return false;
	}

	wchar_t wBuf[256] = {0};
	char szName[32] = {'\0'}, szID[32] = {'\0'}, szVisiteTime[32] = {'\0'};
	HELE hE = GETELE(IDL_LISTCHAXUNTONGJI);
	wcscpy(wBuf, XList_GetHeaderItemText(hE,0));	
    W_2_S(wBuf, szName, 32);
	if (strcmp(szName,"��ʾ")==0)
	{
		return false;
	}

	wcscpy(wBuf, XList_GetItemText(hE, nIndex, 2));		//����
	W_2_S(wBuf, szName, 32);

	memset((char *)wBuf, 0, sizeof(wBuf));
	wcscpy(wBuf, XList_GetItemText(hE, nIndex, 6));		//֤������
	W_2_S(wBuf, szID, 32);

	memset((char *)wBuf, 0, sizeof(wBuf));
	wcscpy(wBuf, XList_GetItemText(hE, nIndex, 12));		//����ʱ��
	W_2_S(wBuf, szVisiteTime, 32);

	HXMLRES  hRes = XXmlRes_Load(L"skin\\mainFrame7.res");	//������Դ
	if (NULL == hRes)
	{
		return false;
	}

	g_hQueryDetail = XC_LoadXML_Window(L"skin\\mainPage7.xml", hRes,XEle_GetHWnd(hEle)); //��������
	if (NULL == g_hQueryDetail)
	{
		return false;
	}
	
	if (false == XC_LoadXML_Style(g_hQueryDetail, L"skin\\mainFrame7.css", hRes))	//������ʽ�ļ�
	{
		return false;
	}

	XEle_RegisterEvent(XWnd_GetEle(g_hQueryDetail, IDB_SETBLACKNAME), XE_BNCLICK, OnSetBlackName);		//����Ϊ������
	XEle_RegisterEvent(XWnd_GetEle(g_hQueryDetail, IDB_DELBLACKNAME), XE_BNCLICK, OnDelBlackName);		//ȡ��������
	XEle_RegisterEvent(XWnd_GetEle(g_hQueryDetail, IDB_CloseBLACKNAME), XE_BNCLICK, OnCloseBlackName);


	XWnd_EnableCloseButton(g_hQueryDetail, false);
	XWnd_EnableMaxButton(g_hQueryDetail, false);
	XWnd_SetBorderStrokeOuterColor(g_hQueryDetail, RGB(102, 0, 0), RGB(0, 0, 0));
	//XWnd_MaximizeWnd(g_hQueryDetail,true);
	XWnd_ShowWindow(g_hQueryDetail, SW_SHOW);
	XWnd_SetWindowPos(g_hQueryDetail, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	 //XWnd_SetWindowPos(hWindow, HWND_BOTTOM, 0, 0, cx, cy, SWP_NOMOVE | SWP_NOSIZE);

    XEle_EnableEle(GETELE(IDB_CHAXUN), false);
	XEle_EnableEle(GETELE(IDB_TONGJI), false);
	XEle_EnableEle(GETELE(IDB_EXPORT_VISITOR), false);
	XEle_EnableEle(GETELE(IDB_TONGJICHAXUNRETURN), false);

	char szT[512] = {'\0'};
	long FldCount = 0, ret = 0;;
	CString outParas[FLD_MAX], outP1[FLD_MAX];
	CWinAdo tAdo(sCONNSTR_MYSQL);
	int i;
	
	ret = tAdo.OpenAdo();
	if (ret != ADOOK)
	{
		return false;
	}
	
	for (i = 0; i < FLD_MAX; i++)
	{
		outParas[i].Empty();
		outP1[i].Empty();
	}

	if (g_nSqlConnectType == SQL_CONNECT_SERVER2005)
	{	
		//sprintf(szT, "select DATEDIFF(s, '1970-01-01 08:00:00', \'%s\')", szVisitTime);
		sprintf(szT, "select * from t_visitor where visitor_name = \'%s\' and visitor_id = \'%s\' and visitor_visitetime = DATEDIFF(s, '1970-01-01 08:00:00', \'%s\')", szName, szID, szVisiteTime);
	}
	else
	    sprintf(szT, "select * from t_visitor where visitor_name = \'%s\' and visitor_id = \'%s\' and visitor_visitetime = UNIX_TIMESTAMP(\'%s\')", szName, szID, szVisiteTime);

	ret = tAdo.execSelect(szT, &FldCount, outParas);
	if (ret == ADOERR || (FldCount == 0 && outParas[0].IsEmpty()))
	{
		return false;
	}

	VISITORINFO visitor;
	memset((char *)&visitor, 0, sizeof(VISITORINFO));

	SET_EDIT(0, IDE_CX_VISITORNAME);
	SET_EDIT(1, IDE_CX_VISITORSEX);
	SET_EDIT(2, IDE_CX_VISITORFOLK);
	SET_EDIT(3, IDE_CX_VISITORMOBILE);

	int n=atoi(outParas[4]);
	memset((char *)wBuf, 0, sizeof(wBuf));
    switch(n)
	{
	     case 1:
         S_2_W(wBuf, "�������֤", 128);
		 break;
         case 2:
         S_2_W(wBuf, "һ�����֤", 128);
		 break;
		 case 3:
         S_2_W(wBuf, "����", 128);
		 break;
		 case 4:
         S_2_W(wBuf, "��½����۰�ͨ��֤", 128);
		 break;
		 case 5:
         S_2_W(wBuf, "�������֤", 128);
		 break;
		 case 6:
         S_2_W(wBuf, "̨��֤", 128);
		 break;
		 case 7:
         S_2_W(wBuf, "��ʻ֤", 128);   
		 break;
		 case 8:
         S_2_W(wBuf, "��ʦ֤", 128);  
		 break;
		 case 9:
         S_2_W(wBuf, "��Ƭ", 128); 
		 break;
	default:
		 S_2_W(wBuf, "����֤��", 128);
		break;
	}			
	hE = XWnd_GetEle(g_hQueryDetail, IDE_CX_VISITORIDTYPE);
	XEdit_SetText(hE, wBuf);


	SET_EDIT(5, IDE_CX_VISITORIDNUM);
	SET_EDIT(7, IDE_CX_VISITORTHING);
	SET_EDIT(8, IDE_CX_VISITORREASON);
	SET_EDIT(9, IDE_CX_VISITORADDR);
	SET_EDIT(10, IDE_CX_VISITORTOTAL);
	SET_EDIT(11, IDE_CX_VISITORUNIT);
	SET_EDIT(12, IDE_CX_VISITORCARNUM);

	char szUserVideoPic[256] = {'\0'}, szUseerIDPic[256] = {'\0'}, szUserHeadPic[256] = {'\0'};
	outParas[19].Replace("#", "\\");
	outParas[20].Replace("#", "\\");
	outParas[21].Replace("#", "\\");
	outParas[26].Replace("#", "\\");
	strcpy(visitor.szUserIDFullPic, outParas[26]);
	strcpy(szUserVideoPic, outParas[19]);
	strcpy(szUseerIDPic, outParas[20]);
	strcpy(szUserHeadPic, outParas[21]);

	strcpy(visitor.szName, outParas[0]);
	strcpy(visitor.szSex, outParas[1]);
	strcpy(visitor.szFolk, outParas[2]);
    strcpy(visitor.szIDType, outParas[4]);
	strcpy(visitor.szID, outParas[5]);
	strcpy(visitor.szAddr, outParas[9]);
	strcpy(visitor.szSignDepartment, outParas[6]);
	strcpy(visitor.szUserVideoPic, szUserVideoPic);
	strcpy(visitor.szUserIDPic, szUseerIDPic);
	strcpy(visitor.szUserHeadPic, szUserHeadPic);
	strcpy(visitor.szUserIDFullPic,outParas[26]);
	strcpy(visitor.szID2Flag, outParas[23]);		//����֤�Ķ���ʽ
	strcpy(visitor.szBirth, outParas[24]);
	strcpy(visitor.szJob, outParas[27]);


	if (atoi(visitor.szID2Flag) == 1)		//�Ķ��Ķ���֤
	{
		strcpy(visitor.szValidDate, outParas[22]);		//����֤��Ч��
		ShowID2_CX(g_hQueryDetail, true, visitor);
	}
	else
	{
		ShowID2_CX(g_hQueryDetail, false, visitor);
	}

//	if (CheckIsBlackName(visitor))
	if (CheckBlackName(visitor))
	{
		XBtn_SetCheck(XWnd_GetEle(g_hQueryDetail, IDK_ISBLACKNAME), true);
	}
	else
	{
		XBtn_SetCheck(XWnd_GetEle(g_hQueryDetail, IDK_ISBLACKNAME), false);
	}

	char szOfficial[32] = {'\0'}, szOffice[32] = {'\0'};
	strcpy(szOfficial, outParas[14]);
	strcpy(szOffice, outParas[15]);

	//sprintf(szT, "select FROM_UNIXTIME(%ld)", atol(outParas[16]));		//����ʱ��
	if (g_nSqlConnectType == SQL_CONNECT_SERVER2005)
	{
		sprintf(szT, "SELECT DATEADD(s,%d,'1970-01-01 08:00:00')", atol(outParas[16]));	//����ʱ��
	}
	else
	{
		sprintf(szT, "select FROM_UNIXTIME(%s)", outParas[16]);	//����ʱ��
	}
	ret = tAdo.execSelect(szT, &FldCount, outP1);
	if (ret == ADOERR || (FldCount == 0 && outP1[0].IsEmpty()))
	{
		outParas[16] = "";
	}
	else
	{
		outParas[16] = outP1[0];
	}
	SET_EDIT(16, IDE_CX_VISITETIME);

	if (atol(outParas[17]) != 0)
	{
		//sprintf(szT, "select FROM_UNIXTIME(%ld)", atol(outParas[17]));				//�뿪ʱ��
		if (g_nSqlConnectType == SQL_CONNECT_SERVER2005)
		{
			sprintf(szT, "SELECT DATEADD(s,%d,'1970-01-01 08:00:00')", atol(outParas[17]));	//�뿪ʱ��
		}
		else
		{
			sprintf(szT, "select FROM_UNIXTIME(%s)", outParas[17]);	//�뿪ʱ��
		}
		ret = tAdo.execSelect(szT, &FldCount, outParas);
		if (ret == ADOERR || (FldCount == 0 && outParas[0].IsEmpty()))
		{
			outParas[17] = "";
		}
		else
		{
			outParas[17] = outParas[0];
		}
	}
	SET_EDIT(17, IDE_CX_LEFTTIME);

	for (i = 0; i < FLD_MAX; i++)
	{
		outParas[i].Empty();
		outP1[i].Empty();
	}
	sprintf(szT, "select * from t_official where official_name = \'%s\' and official_office_name = \'%s\'", szOfficial, szOffice);
	ret = tAdo.execSelect(szT, &FldCount, outParas);
	if (ret == ADOERR || (FldCount == 0 && outParas[0].IsEmpty()))
	{
		XXmlRes_Destroy(hRes);
		XWnd_RedrawWnd(g_hQueryDetail);
		return false;
	}

	memset((char *)wBuf, 0, sizeof(wBuf));
	S_2_W(wBuf, (LPSTR)(LPCTSTR)outParas[0], 128);
	hE = XWnd_GetEle(g_hQueryDetail, IDE_CX_OFFICIALNAME);
	XEdit_SetText(hE, wBuf);

	SET_EDIT(0, IDE_CX_OFFICIALNAME);
	SET_EDIT(1, IDE_CX_OFFICIALSEX);
	SET_EDIT(2, IDE_CX_OFFICEPHONE);
	SET_EDIT(3, IDE_CX_OFFICIALMOBILE);
	SET_EDIT(4, IDE_CX_OFFICENAME);
	SET_EDIT(5, IDE_CX_OFFICEADDR);
	SET_EDIT(6, IDC_CX_OFFICIALZHIWU);
	SET_EDIT(7, IDE_CX_ROOMNUM);

	memset((char *)wBuf, 0, sizeof(wBuf));
	S_2_W(wBuf, (LPSTR)(LPCTSTR)outParas[13], 128);
	hE = XWnd_GetEle(g_hQueryDetail, IDE_CX_OFFICESSO);
	XEdit_SetText(hE, wBuf);
	if(g_bSSO)
	{

		XEle_ShowEle(XWnd_GetEle(g_hQueryDetail,IDC_CX_OFFICIALZHIWU),false);
		XEle_ShowEle(XWnd_GetEle(g_hQueryDetail,IDS_CX_OFFICIALZHIWU),false);
		XEle_ShowEle(XWnd_GetEle(g_hQueryDetail,IDE_CX_ROOMNUM),false);
		XEle_ShowEle(XWnd_GetEle(g_hQueryDetail,IDS_CX_ROOMNUM),false);
	}
	else
	{
		XEle_ShowEle(XWnd_GetEle(g_hQueryDetail,IDS_CX_OFFICESSO),false);
		XEle_ShowEle(XWnd_GetEle(g_hQueryDetail,IDE_CX_OFFICESSO),false);
	}


	XXmlRes_Destroy(hRes);
	return false;
}

BOOL ShowID2_CX(HWINDOW hWindow, BOOL bShow, const VISITORINFO &visitor)
{
	HELE hEle = NULL;
	HIMAGE hImg = NULL;
	wchar_t wBuf[256] = {0},wAddr[256] = {0};
	if (bShow)
	{
		hEle = XWnd_GetEle(hWindow, IDP_CX_ID2PIC);
		hImg = XImage_LoadFile(L".\\skin\\image\\id2.bmp", true);
		XPic_SetImage(hEle, hImg);

		hEle = XWnd_GetEle(hWindow, IDS_CX_ID2_NAME);
		S_2_W(wBuf, (char *)visitor.szName, 128);
		XStatic_SetText(hEle, wBuf);

		memset((char *)wBuf, 0, sizeof(wBuf));
		hEle = XWnd_GetEle(hWindow, IDS_CX_ID2_SEX);
		S_2_W(wBuf, (char *)visitor.szSex, 128);
		XStatic_SetText(hEle, wBuf);

		memset((char *)wBuf, 0, sizeof(wBuf));
		hEle = XWnd_GetEle(hWindow, IDS_CX_ID2_FOLK);
		S_2_W(wBuf, (char *)visitor.szFolk, 128);
		XStatic_SetText(hEle, wBuf);

		char sz[128] = {'\0'};
		memset((char *)wBuf, 0, sizeof(wBuf));
		hEle = XWnd_GetEle(hWindow, IDS_CX_ID2_BIRTHYEAR);
		memcpy(sz, (char *)visitor.szBirth, 4);
		S_2_W(wBuf, sz, 32);
		XStatic_SetText(hEle, wBuf);

		memset((char *)wBuf, 0, sizeof(wBuf));
		memset(sz, 0, sizeof(sz));
		hEle = XWnd_GetEle(hWindow, IDS_CX_ID2_BIRTHMONTH);
		memcpy(sz, (char *)visitor.szBirth + 4, 2);
		S_2_W(wBuf, sz, 32);
		XStatic_SetText(hEle, wBuf);

		memset((char *)wBuf, 0, sizeof(wBuf));
		memset(sz, 0, sizeof(sz));
		hEle = XWnd_GetEle(hWindow, IDS_CX_ID2_BIRTHDAY);
		memcpy(sz, (char *)visitor.szBirth + 4 + 2, 2);
		S_2_W(wBuf, sz, 32);
		XStatic_SetText(hEle, wBuf);

		memset((char *)wBuf, 0, sizeof(wBuf));
		memset(sz, 0, sizeof(sz));
		hEle = XWnd_GetEle(hWindow, IDS_CX_ID2_ADDR);
	    //strcpy(sz,"SDFSADFSADFASDFASDFASDFASDFSADFASDFSAD����ʡ�ⶫ��1ʯ�������Ŵ�13�����ʡ�ⶫ��1ʯ�������Ŵ�13�����ʡ�ⶫ��1ʯ�������Ŵ�13��");
        strcpy(sz,visitor.szAddr);

		S_2_W(wAddr, sz, 128);
		int len = 0, sLen = wcslen(wAddr);
		int n = sLen % 11;
		if (n == 0)
		{
			n = sLen / 11;
		}
		else
		{
			n = sLen / 11 + 1;
		}
		for (int i = 0; i < n; i++)
		{
			if (i == n - 1)
			{
				memcpy(wBuf + len, wAddr + 11 * i, 2*(sLen - 11 * (n - 1)));
				len += sLen - 11 * (n - 1);
			}
			else
			{
				memcpy(wBuf + len, wAddr + 11 * i, 22);
				len += 11;
				memcpy(wBuf + len, L"\r\n", 4);
				len += 2;
			}
		}
		//S_2_W(wBuf, sz, 128);
		XEdit_SetText(hEle, wBuf);

		memset((char *)wBuf, 0, sizeof(wBuf));
		hEle = XWnd_GetEle(hWindow, IDS_CX_ID2_ID);
		S_2_W(wBuf, (char *)visitor.szID, 128);
		XStatic_SetText(hEle, wBuf);

		memset((char *)wBuf, 0, sizeof(wBuf));
		hEle = XWnd_GetEle(hWindow, IDS_CX_ID2_SIGNDEPARTMENT);
		S_2_W(wBuf, (char *)visitor.szSignDepartment, 128);
		XStatic_SetText(hEle, wBuf);

		memset((char *)wBuf, 0, sizeof(wBuf));
		hEle = XWnd_GetEle(hWindow, IDS_CX_ID2_VALIDTERM);
		S_2_W(wBuf, (char *)visitor.szValidDate, 128);
		XStatic_SetText(hEle, wBuf);

		memset((char *)wBuf, 0, sizeof(wBuf));
		hEle = XWnd_GetEle(hWindow, IDP_CX_ID2_HEADPIC);
		S_2_W(wBuf, (char *)visitor.szUserIDPic, 256);
		hImg = XImage_LoadFile(wBuf, true);
		XPic_SetImage(hEle, hImg);

		memset((char *)wBuf, 0, sizeof(wBuf));
		hEle = XWnd_GetEle(hWindow, IDP_CX_HEADPIC);
		S_2_W(wBuf, (char *)visitor.szUserHeadPic, 256);
		hImg = XImage_LoadFile(wBuf, true);
		XPic_SetImage(hEle, hImg);

		memset((char *)wBuf, 0, sizeof(wBuf));
		hEle = XWnd_GetEle(hWindow, IDP_CX_CAMERA);
		S_2_W(wBuf, (char *)visitor.szUserVideoPic, 256);
		hImg = XImage_LoadFile(wBuf, true);
		XPic_SetImage(hEle, hImg);

		hEle = XWnd_GetEle(hWindow, IDG_CX_ID2);
		XEle_ShowEle(hEle, bShow);
		
		hEle = XWnd_GetEle(hWindow, IDG_CX_PASSPORT);
		XEle_ShowEle(hEle, !bShow);
	}
	else
	{
		hEle = XWnd_GetEle(hWindow, IDG_CX_ID2);
		XEle_ShowEle(hEle, bShow);

		hEle = XWnd_GetEle(hWindow, IDG_CX_PASSPORT);
		XEle_ShowEle(hEle, !bShow);

		memset((char *)wBuf, 0, sizeof(wBuf));
		hEle = XWnd_GetEle(hWindow, IDP_CX_CAMERA);
		S_2_W(wBuf, (char *)visitor.szUserVideoPic, 256);
		hImg = XImage_LoadFile(wBuf, true);
		XPic_SetImage(hEle, hImg);

		memset((char *)wBuf, 0, sizeof(wBuf));
		hEle = XWnd_GetEle(hWindow, IDP_CX_HEADPIC);
		S_2_W(wBuf, (char *)visitor.szUserHeadPic, 256);
		hImg = XImage_LoadFile(wBuf, true);
		XPic_SetImage(hEle, hImg);

		memset((char *)wBuf, 0, sizeof(wBuf));
		hEle = XWnd_GetEle(hWindow, IDP_CX_PASSPORTPIC);
		S_2_W(wBuf, (char *)visitor.szUserIDFullPic, 256);
		hImg = XImage_LoadFile(wBuf, true);
		XPic_SetImage(hEle, hImg);
	}

	return true;
}

//�����͵�
BOOL PrintVisitorNoteAgain() 
{	
	if(g_bAutoCallOfficials == true)
	{
		XMessageBox(g_hWindow, L"�쵼ȷ��ģʽ�²����ֹ���ӡ��͵�!", L"����");
		return true;
	}

	wchar_t wBuf[256] = {0};
	char szT[512] = {'\0'};
	long FldCount = 0, ret = 0;;
	CString outParas[FLD_MAX];
	CWinAdo tAdo(sCONNSTR_MYSQL);

	HELE hList = GETELE(IDL_VISTORLIST);
	int nIndex = XList_GetSelectItem(hList);		//��ȡѡ����
	if (!XList_IsItemCheck(hList, nIndex))
	{
		XMessageBox(g_hWindow, L"����ѡ���¼���ٴ�ӡ!", L"����");
		return false;
	}
	
	ret = tAdo.OpenAdo();
	if (ret != ADOOK)
	{
		return false;
	}

	for (int i = 0; i < FLD_MAX; i++)
	{
		outParas[i].Empty();
	}

	VISITORINFO visitor;
	memset((char *)&visitor, 0, sizeof(VISITORINFO));
	HELE hE = XWnd_GetEle(g_hWindow, IDE_VISITORNAME);	//�ÿ�����
	if (XEdit_GetTextLength(hE) <= 0)
	{
		XMessageBox(g_hWindow, L"�ÿ���������Ϊ��", L"����");
		return false;
	}
	memset((char *)wBuf, 0, sizeof(wBuf));
	XEdit_GetText(hE, wBuf, 128);
	W_2_S(wBuf, visitor.szName, 128);


	memset((char *)wBuf, 0, sizeof(wBuf));
	XWnd_GetEle(g_hWindow, IDP_ID2_HEADPIC);
	S_2_W(wBuf, (char *)visitor.szUserHeadPic, 256);
		
	memset((char *)wBuf, 0, sizeof(wBuf));
	hE = XWnd_GetEle(g_hWindow, IDC_VISITORSEX);		//�ÿ��Ա�
	XEdit_GetText(hE, wBuf, 128);
    W_2_S(wBuf, visitor.szSex, 128);

	memset((char *)wBuf, 0, sizeof(wBuf));
	hE = XWnd_GetEle(g_hWindow, IDE_VISITORMOBILE);		//�ÿ͵绰
	XEdit_GetText(hE, wBuf, 128);
	W_2_S(wBuf, visitor.szPhone, 128);

	memset((char *)wBuf, 0, sizeof(wBuf));
	hE = XWnd_GetEle(g_hWindow, IDE_VISITORIDNUM);		//�ÿ�֤������
	XEdit_GetText(hE, wBuf, 128);
	W_2_S(wBuf, visitor.szID, 128);

	memset((char *)wBuf, 0, sizeof(wBuf));
	hE = XWnd_GetEle(g_hWindow, IDE_VISITORTHING);		//�ÿ���Ʒ
	XEdit_GetText(hE, wBuf, 128);
	W_2_S(wBuf, visitor.szWithGoods, 128);

	/*memset((char *)wBuf, 0, sizeof(wBuf));
	hE = XWnd_GetEle(g_hWindow, IDC_VISITORIDTYPE);		//�ÿ�֤������
	n = XComboBox_GetSelectItem(hE);
	sprintf(visitor.szIDType, "%ld", n + 1);*/

	memset((char *)wBuf, 0, sizeof(wBuf));
	hE = XWnd_GetEle(g_hWindow, IDC_VISITORFOLK);		//�ÿ�����
	//n = XComboBox_GetSelectItem(hE);
	//sprintf(visitor.szFolk, "%ld", n + 1);
	//wcscpy(wBuf, XComboBox_GetItemText(hE, n));
	XEdit_GetText(hE, wBuf, 128);
	W_2_S(wBuf, visitor.szFolk, 32);

	memset((char *)wBuf, 0, sizeof(wBuf));
	hE = XWnd_GetEle(g_hWindow, IDE_VISITORREASON);		//�ÿ�ԭ��
	XEdit_GetText(hE, wBuf, 128);
	W_2_S(wBuf, visitor.szReason, 128);

	/*memset((char *)wBuf, 0, sizeof(wBuf));
	hE = XWnd_GetEle(g_hWindow, IDE_VISITORADDR);		//�ÿ͵�ַ
	XEdit_GetText(hE, wBuf, 128);
	W_2_S(wBuf, visitor.szAddr, 128);*/

	if(g_bVisitorType)
	{
		memset((char *)wBuf, 0, sizeof(wBuf));
		hE = XWnd_GetEle(g_hWindow, IDE_VISITORTYPE);		//�ÿ�����
		XEdit_GetText(hE, wBuf, 128);
		W_2_S(wBuf, visitor.szType, 128);
	}
	else
	{
		memset((char *)wBuf, 0, sizeof(wBuf));
		hE = XWnd_GetEle(g_hWindow, IDE_VISITORTOTAL);		//�ÿ�����
		XEdit_GetText(hE, wBuf, 128);
		W_2_S(wBuf, visitor.szTotalPeople, 128);
	}






	if(g_nIDType == ID_TYPE_Lawyer)
	{
		memset((char *)wBuf, 0, sizeof(wBuf));
		hE = XWnd_GetEle(g_hWindow, IDC_Notice_Comb);		//֪ͨ��
		XEdit_GetText(hE, wBuf, 128);
		W_2_S(wBuf, visitor.szUnit, 128);

		memset((char *)wBuf, 0, sizeof(wBuf));
		hE = XWnd_GetEle(g_hWindow, IDC_LawyerLetter_Comb);		//����
		XEdit_GetText(hE, wBuf, 128);
		W_2_S(wBuf, visitor.szCarNum, 128);
	}
	else
	{
		memset((char *)wBuf, 0, sizeof(wBuf));
		hE = XWnd_GetEle(g_hWindow, IDE_VISITORUNIT);		//�ÿ͵�λ
		XEdit_GetText(hE, wBuf, 128);
		W_2_S(wBuf, visitor.szUnit, 128);
	}

	/*memset((char *)wBuf, 0, sizeof(wBuf));
	hE = XWnd_GetEle(g_hWindow, IDE_VISITORCARNUM);		//���ƺ�
	XEdit_GetText(hE, wBuf, 128);
	W_2_S(wBuf, visitor.szCarNum, 128);

	memset((char *)wBuf, 0, sizeof(wBuf));
	hE = XWnd_GetEle(g_hWindow, IDE_FZJG);				//��֤����
	XEdit_GetText(hE, wBuf, 128);
	W_2_S(wBuf, visitor.szSignDepartment, 128);*/

	memset((char *)wBuf, 0, sizeof(wBuf));
	hE = XWnd_GetEle(g_hWindow, IDE_MASTERNAME);		//������������
	XEdit_GetText(hE, wBuf, 128);
	W_2_S(wBuf, visitor.szToOfficialName, 128);

	memset((char *)wBuf, 0, sizeof(wBuf));
	hE = XWnd_GetEle(g_hWindow, IDE_OFFICENAME);			//�����˰칫��
	XEdit_GetText(hE, wBuf, 128);
	W_2_S(wBuf, visitor.szToOfficeName, 128);

	memset((char *)wBuf, 0, sizeof(wBuf));
	hE = XWnd_GetEle(g_hWindow, IDE_ROOMNUM);				//�����˷����
	XEdit_GetText(hE, wBuf, 128);
	W_2_S(wBuf, visitor.szToRoom, 128);

	memset((char *)wBuf, 0, sizeof(wBuf));
	hE = XWnd_GetEle(g_hWindow, IDC_MASTERSSO);				//������SSO
	XEdit_GetText(hE, wBuf, 128);
	W_2_S(wBuf, visitor.szToSSO, 128);

    //��������ͼ
  
	//if (strlen(g_Printvisitor.szBarCode)==13)  //13en��
	{
		GetBarCodePic(g_Printvisitor.szBarCode);
		//HDC hDC = ::GetDC(GETHWND);
		//DrawBarCode(hDC, g_Printvisitor.szBarCode, USER_BARCODE_PIC);
		//::ReleaseDC(GETHWND, hDC);
	}
 

	if (::GetPrivateProfileInt("TerminalSet", "bSaveNoPrint",0, CONFIG_FILE)==0)
	{
		char szPath[256] = {'\0'};
		::GetCurrentDirectory(256, szPath);
		//sprintf(szT, "%s\\%s", szPath, RECEIPT_HTML);
		sprintf(szT, "%s\\%s", szPath, INDEX_HTML);
		//CreateReceiptHtml(g_Printvisitor, szT);
		/*if (g_PrintTipsCfg.ePrintType==PRINT_FULL_TYPE&&g_PrintTipsCfg.bTotalPeoplePrint)
		{
			if(!CreateReceiptHtml(g_Printvisitor, szT))
			{
				XMessageBox(g_hWindow,L"�޷�������͵�",L"����");
			}
		}
		else
		{
			//if(!CreateReceiptHtml2(visitor, szT))
			if(!CreateNoFrameReceiptHtml(visitor, szT))
			{
				XMessageBox(g_hWindow,L"�޷�������͵�",L"����");
			}
		}*/
		if (g_PrintTipsCfg.ePrintType==PRINT_FULL_TYPE&&g_PrintTipsCfg.bTotalPeoplePrint)
		{
			if(!CreateReceiptHtml(visitor, szT))
			{
				XMessageBox(g_hWindow,L"�޷�������͵�",L"����");
			}
		}
		else if (g_PrintTipsCfg.ePrintType==PRINT_NOFRAME_TYPE)
		{
			if(!CreateNoFrameReceiptHtml(visitor, szT))
			{
				XMessageBox(g_hWindow,L"�޷�������͵�",L"����");
			}
		}
		else
		{
			if(!CreateReceiptHtml2(visitor, szT))
			//if(!CreateNoFrameReceiptHtml(visitor, szT))
			{
				XMessageBox(g_hWindow,L"�޷�������͵�",L"����");
			}
		}

		PrintHtmlPage(szT);					//��ӡ��͵�
	}

	if (::GetPrivateProfileInt("TerminalSet", "bLeaveOfficial",0, CONFIG_FILE)==1)
	{
		//�������������
		if (::GetPrivateProfileInt("TerminalSet", "bSaveAfterScan",0, CONFIG_FILE)!=1)
			InitVisitorData();
	}
	else
	{
		//�����������
		InitData();
	}

	if(g_nIDType == ID_TYPE_Lawyer) 
		strcpy(g_Printvisitor.szName,"");

	::DeleteFile(USER_HEAD_PIC);            //ɾ���ϴ�ͷ��
	::DeleteFile(USER_ID_PIC); 
	::DeleteFile(USER_ID2_PIC);
	::DeleteFile(USER_ID2_TMP);
	::DeleteFile(USER_VIDEO_PIC);

	XWnd_RedrawWnd(g_hWindow);
	return false;
}

BOOL CALLBACK OnDocComplete(HXCGUI hWebBrowser,in_ IDispatch *pDisp,in_ wchar_t *pUrl)
{
    int ret;
	CSelectPrinttips s(GETHWND);
	s.CreateWindow_XC(L"�Ƿ��ӡ��͵�");
	ret = s.ShowTips();

	if(ret == 0)
		return false;

	if (g_hWeb != NULL)
	{
		XWeb_ExecWB(g_hWeb, OLECMDID_PRINT, OLECMDEXECOPT_DONTPROMPTUSER, NULL, NULL);
	}
	return false;
}

BOOL CALLBACK OnSaveOther(HELE hEle, HELE hEventEle)
{
	HELE hE = GETELE(IDB_OPEN_SENDREQUEST);
	if (XBtn_IsCheck(hE))
	{
        ::WritePrivateProfileString("TerminalSet", "bSendRequest", "1", CONFIG_FILE);
	}
	else
        ::WritePrivateProfileString("TerminalSet", "bSendRequest", "0", CONFIG_FILE);

	hE = GETELE(IDK_IMPORT_CZY);
	if (XBtn_IsCheck(hE))
	{
        ::WritePrivateProfileString("TerminalSet", "bVisitorReadOnly", "1", CONFIG_FILE);
	}
	else
        ::WritePrivateProfileString("TerminalSet", "bVisitorReadOnly", "0", CONFIG_FILE);

	hE = GETELE(IDC_VISITORPIC);
    if (XBtn_IsCheck(hE))
	{
        ::WritePrivateProfileString("TerminalSet", "bVisitorPicMust", "1", CONFIG_FILE);
	}
	else
        ::WritePrivateProfileString("TerminalSet", "bVisitorPicMust", "0", CONFIG_FILE);

	hE = GETELE(IDC_HIDELAST4ID);
    if (XBtn_IsCheck(hE))
	{
        ::WritePrivateProfileString("TerminalSet", "bHideLast4ID", "1", CONFIG_FILE);
	}
	else
        ::WritePrivateProfileString("TerminalSet", "bHideLast4ID", "0", CONFIG_FILE);

	hE = GETELE(IDC_GOODS_Print);
    if (XBtn_IsCheck(hE))
	{
        ::WritePrivateProfileString("TerminalSet", "bHideGoodsPrint", "1", CONFIG_FILE);
	}
	else
        ::WritePrivateProfileString("TerminalSet", "bHideGoodsPrint", "0", CONFIG_FILE);

	hE = GETELE(IDC_ROOM_Print);
    if (XBtn_IsCheck(hE))
	{
        ::WritePrivateProfileString("TerminalSet", "bHideToRoomPrint", "1", CONFIG_FILE);
	}
	else
        ::WritePrivateProfileString("TerminalSet", "bHideToRoomPrint", "0", CONFIG_FILE);

	hE = GETELE(IDC_People_Print);
    if (XBtn_IsCheck(hE))
	{
        ::WritePrivateProfileString("TerminalSet", "bHideTotalPeoplePrint", "1", CONFIG_FILE);
	}
	else
        ::WritePrivateProfileString("TerminalSet", "bHideTotalPeoplePrint", "0", CONFIG_FILE);

	hE = GETELE(IDC_Tips2_Print);
    if (XBtn_IsCheck(hE))
	{
        ::WritePrivateProfileString("TerminalSet", "bPrintTips2", "1", CONFIG_FILE);
	}
	else
        ::WritePrivateProfileString("TerminalSet", "bPrintTips2", "0", CONFIG_FILE);

	hE = GETELE(IDC_PRINTAFTERALLOW);
    if (XBtn_IsCheck(hE))
	{
        ::WritePrivateProfileString("TerminalSet", "bPrintAfterAllow", "1", CONFIG_FILE);
	}
	else
        ::WritePrivateProfileString("TerminalSet", "bPrintAfterAllow", "0", CONFIG_FILE);

	hE = GETELE(IDC_SAVENOPRINT);
    if (XBtn_IsCheck(hE))
	{
        ::WritePrivateProfileString("TerminalSet", "bSaveNoPrint", "1", CONFIG_FILE);
	}
	else
        ::WritePrivateProfileString("TerminalSet", "bSaveNoPrint", "0", CONFIG_FILE);

	hE = GETELE(IDC_SAVEOFFICIAL);
    if (XBtn_IsCheck(hE))
	{
        ::WritePrivateProfileString("TerminalSet", "bSaveOfficial", "1", CONFIG_FILE);
	}
	else
        ::WritePrivateProfileString("TerminalSet", "bSaveOfficial", "0", CONFIG_FILE);

	hE = GETELE(IDC_FTPOPEN);
    if (XBtn_IsCheck(hE))
	{
        ::WritePrivateProfileString("TerminalSet", "bFtpOpen", "1", CONFIG_FILE);
	}
	else
        ::WritePrivateProfileString("TerminalSet", "bFtpOpen", "0", CONFIG_FILE);
	
	//HWINDOW hModal=XModalWnd_CreateWindow(200,200,L"���ѱ���ɹ�����[ȷ��]���������Ч��",XEle_GetHWnd(hEle));
    //int result=XModalWnd_DoModal(hModal);
	int result=XMessageBox(g_hWindow,L"���ѱ���ɹ�����[ȷ��]���������Ч��",L"��ʾ",XMB_OK|XMB_CANCEL);
    if(XMB_OK==result)
    {
        //ȷ����ť
        if (g_hWeb != NULL)
		{
			XWeb_Destroy(g_hWeb);
		}
		OleUninitialize();
		g_bStopID2 = true;			//ֹͣ����֤��д�߳�
		g_bStopBarCode = true;		//ֹͣ�����Ķ��߳�	
		FreeIdcardLibrary();		//ж�ض���֤��д��
		EndWJUsb();
		CloseCardReader();
		CloseSoftKeybd();
		CloseXYB();
		CloseBackScreen();
		HideTaskBar(false);
		WaitForSingleObject(g_ThreadSocketMutex,INFINITE);
        g_tSock->CloseSock();
		ReleaseMutex(g_ThreadSocketMutex);
		XWnd_CloseWindow(XEle_GetHWindow(hEle));
    }

	return true;
}

BOOL CALLBACK OnSaveMustInput(HELE hEle, HELE hEventEle)
{
	char szT[32] = {'\0'};
	HELE hE = NULL;
	BOOL bRet = false;

	SET_INPUT(IDK_V_NAME, "VisitorName");
	SET_INPUT(IDK_V_ID, "VisitorID");
	SET_INPUT(IDK_V_ADDR, "VisitorAddr"); 
	SET_INPUT(IDK_V_UNIT, "VisitorUnit");
	SET_INPUT(IDK_V_TOTAL, "VisitorTotal");
	SET_INPUT(IDK_V_GOODS, "VisitorGoods");
	SET_INPUT(IDK_V_REASON, "VisitorReason");
	SET_INPUT(IDK_V_CARNUM, "VisitorCarNum");
	SET_INPUT(IDK_V_SIGNDEPARTMENT, "VisitorSignDepartment");
	SET_INPUT(IDK_V_MOBILE, "VisitorMobile");
	SET_INPUT(IDK_O_NAME, "OfficialName"); 
	SET_INPUT(IDK_O_PHONE, "OfficialPhone");
	SET_INPUT(IDK_O_MOBILE, "OfficialMobile");
	SET_INPUT(IDK_O_OFFICENAME, "OfficeName");
	SET_INPUT(IDK_O_OFFICEADDR, "OfficeAddr");
	SET_INPUT(IDK_O_LEVEL, "OfficialLevel"); 
	SET_INPUT(IDK_O_ROOM, "OfficialRoom");
	
	int result=XMessageBox(g_hWindow,L"���ѱ���ɹ�����[ȷ��]���������Ч��",L"��ʾ",XMB_OK|XMB_CANCEL);
    if(XMB_OK==result)
    {
        //ȷ����ť
        if (g_hWeb != NULL)
		{
			XWeb_Destroy(g_hWeb);
		}
		OleUninitialize();
		g_bStopID2 = true;			//ֹͣ����֤��д�߳�
		g_bStopBarCode = true;		//ֹͣ�����Ķ��߳�	
		FreeIdcardLibrary();		//ж�ض���֤��д��
		EndWJUsb();
		CloseCardReader();
		CloseSoftKeybd();
		CloseXYB();
		CloseBackScreen();
		HideTaskBar(false);
		WaitForSingleObject(g_ThreadSocketMutex,INFINITE);
        g_tSock->CloseSock();
		ReleaseMutex(g_ThreadSocketMutex);
		XWnd_CloseWindow(XEle_GetHWindow(hEle));
    }

	XWnd_RedrawWnd(g_hWindow);
	return false;
}

BOOL CheckAllMustInput()
{
	char szT[32] = {'\0'};
	HELE hE = NULL;

	CHK_INPUT(IDK_V_NAME, "VisitorName");
	CHK_INPUT(IDK_V_ID, "VisitorID");
	CHK_INPUT(IDK_V_ADDR, "VisitorAddr"); 
	CHK_INPUT(IDK_V_UNIT, "VisitorUnit");
	CHK_INPUT(IDK_V_TOTAL, "VisitorTotal");
	CHK_INPUT(IDK_V_GOODS, "VisitorGoods");
	CHK_INPUT(IDK_V_REASON, "VisitorReason");
	CHK_INPUT(IDK_V_CARNUM, "VisitorCarNum");
	CHK_INPUT(IDK_V_SIGNDEPARTMENT, "VisitorSignDepartment");
	CHK_INPUT(IDK_V_MOBILE, "VisitorMobile");
	CHK_INPUT(IDK_O_NAME, "OfficialName"); 
	CHK_INPUT(IDK_O_PHONE, "OfficialPhone");
	CHK_INPUT(IDK_O_MOBILE, "OfficialMobile");
	CHK_INPUT(IDK_O_OFFICENAME, "OfficeName");
	CHK_INPUT(IDK_O_OFFICEADDR, "OfficeAddr");
	CHK_INPUT(IDK_O_LEVEL, "OfficialLevel"); 
	CHK_INPUT(IDK_O_ROOM, "OfficialRoom");

	return true;
}

BOOL CALLBACK OnSetBlackName(HELE hEle, HELE hEventEle)
{
	wchar_t wBuf[256] = {0};
	char szT[512] = {'\0'};
	long FldCount = 0, ret = 0;
	CString outParas[FLD_MAX];
	CWinAdo tAdo(sCONNSTR_MYSQL);
	
	ret = tAdo.OpenAdo();
	if (ret != ADOOK)
	{
		return false;
	}
	
	for (int i = 0; i < FLD_MAX; i++)
	{
		outParas[i].Empty();
	}

	HELE hE = NULL;
	VISITORINFO visitor;
	memset((char *)&visitor, 0, sizeof(VISITORINFO));

	memset((char *)wBuf, 0, sizeof(wBuf));
	hE = XWnd_GetEle(XEle_GetHWindow(hEle), IDE_CX_VISITORNAME);
	XEdit_GetText(hE, wBuf, 128);
	W_2_S(wBuf, visitor.szName, 128);

	memset((char *)wBuf, 0, sizeof(wBuf));
	hE = XWnd_GetEle(XEle_GetHWindow(hEle), IDE_CX_VISITORSEX);
	XEdit_GetText(hE, wBuf, 128);
	W_2_S(wBuf, visitor.szSex, 128);

	memset((char *)wBuf, 0, sizeof(wBuf));
	hE = XWnd_GetEle(XEle_GetHWindow(hEle), IDE_CX_VISITORFOLK);
	XEdit_GetText(hE, wBuf, 128);
	W_2_S(wBuf, visitor.szFolk, 128);

	memset((char *)wBuf, 0, sizeof(wBuf));
	hE = XWnd_GetEle(XEle_GetHWindow(hEle), IDE_CX_VISITORADDR);
	XEdit_GetText(hE, wBuf, 128);
	W_2_S(wBuf, visitor.szAddr, 128);

	memset((char *)wBuf, 0, sizeof(wBuf));
	hE = XWnd_GetEle(XEle_GetHWindow(hEle), IDE_CX_VISITORMOBILE);
	XEdit_GetText(hE, wBuf, 128);
	W_2_S(wBuf, visitor.szPhone, 128);

	memset((char *)wBuf, 0, sizeof(wBuf));
	hE = XWnd_GetEle(XEle_GetHWindow(hEle), IDE_CX_VISITORIDTYPE);
	XEdit_GetText(hE, wBuf, 128);
	W_2_S(wBuf, visitor.szIDType, 128);

	memset((char *)wBuf, 0, sizeof(wBuf));
	hE = XWnd_GetEle(XEle_GetHWindow(hEle), IDE_CX_VISITORIDNUM);
	XEdit_GetText(hE, wBuf, 128);
	W_2_S(wBuf, visitor.szID, 128);

	memset((char *)wBuf, 0, sizeof(wBuf));
	hE = XWnd_GetEle(XEle_GetHWindow(hEle), IDE_CX_VISITORUNIT);
	XEdit_GetText(hE, wBuf, 128);
	W_2_S(wBuf, visitor.szUnit, 128);

	memset((char *)wBuf, 0, sizeof(wBuf));
	hE = XWnd_GetEle(XEle_GetHWindow(hEle), IDE_CX_VISITORCARNUM);
	XEdit_GetText(hE, wBuf, 128);
	W_2_S(wBuf, visitor.szCarNum, 128);

	memset((char *)wBuf, 0, sizeof(wBuf));
	hE = XWnd_GetEle(XEle_GetHWindow(hEle), IDE_CX_Notes);
	XEdit_GetText(hE, wBuf, 128);
	W_2_S(wBuf, visitor.szNote, 128);

	sprintf(szT, "insert into t_blackname values(\'%s\', \'%s\', \'%s\', \'%s\', \'%s\', \'%s\', \'%s\', \'%s\', \'%s\', \'%s\', \'%s\', \'%s\', \'%s\', \'%s\')",\
		visitor.szName, visitor.szName, visitor.szSex, visitor.szFolk, visitor.szAddr, visitor.szPhone,\
		visitor.szIDType, visitor.szID, visitor.szUnit, visitor.szCarNum, "", "", "",visitor.szNote);
	ret = tAdo.execUpdateInsert(szT);
	if (ret == ADOOK)
	{
		XMessageBox(XEle_GetHWindow(hEle), L"���ú������ɹ�!", L"��ʾ");
	}

	XWnd_RedrawWnd(XEle_GetHWindow(hEle));
	return false;
}

BOOL CALLBACK OnCloseBlackName(HELE hEle, HELE hEventEle)
{
	//XMessageBox(XEle_GetHWindow(hEle), L"sdfasfdsa!", L"��ʾ");
    XEle_EnableEle(GETELE(IDB_CHAXUN), true);
	XEle_EnableEle(GETELE(IDB_TONGJI), true);
	XEle_EnableEle(GETELE(IDB_TONGJICHAXUNRETURN), true);
	XEle_EnableEle(GETELE(IDB_EXPORT_VISITOR), true);
	XWnd_CloseWindow(XEle_GetHWindow(hEle));
    XWnd_RedrawWnd(g_hWindow);
	return true;
}

BOOL CALLBACK OnDelBlackName(HELE hEle, HELE hEventEle)
{
	wchar_t wBuf[256] = {0};
	char szT[512] = {'\0'};
	long FldCount = 0, ret = 0;;
	CString outParas[FLD_MAX];
	CWinAdo tAdo(sCONNSTR_MYSQL);
	
	ret = tAdo.OpenAdo();
	if (ret != ADOOK)
	{
		return false;
	}
	
	for (int i = 0; i < FLD_MAX; i++)
	{
		outParas[i].Empty();
	}
	
	HELE hE = NULL;
	VISITORINFO visitor;
	memset((char *)&visitor, 0, sizeof(VISITORINFO));
	
	memset((char *)wBuf, 0, sizeof(wBuf));
	hE = XWnd_GetEle(XEle_GetHWindow(hEle), IDE_CX_VISITORNAME);
	XEdit_GetText(hE, wBuf, 128);
	W_2_S(wBuf, visitor.szName, 128);
	
	memset((char *)wBuf, 0, sizeof(wBuf));
	hE = XWnd_GetEle(XEle_GetHWindow(hEle), IDE_CX_VISITORIDNUM);
	XEdit_GetText(hE, wBuf, 128);
	W_2_S(wBuf, visitor.szID, 128);
	
	sprintf(szT, "delete from t_blackname where black_name = \'%s\' and black_id = \'%s\'", visitor.szName, visitor.szID);
	ret = tAdo.execUpdateInsert(szT);
	if (ret == ADOOK)
	{
		XMessageBox(XEle_GetHWindow(hEle), L"ȡ���������ɹ�!", L"��ʾ");
	}
	
	XWnd_RedrawWnd(XEle_GetHWindow(hEle));	
	return false;
}

BOOL CheckIsBlackName(VISITORINFO &visitor)
{
	wchar_t wBuf[256] = {0};
	char szT[512] = {'\0'};
	long FldCount = 0, ret = 0;
	CString outParas[FLD_MAX];
	CWinAdo tAdo(sCONNSTR_MYSQL);
	
	ret = tAdo.OpenAdo();
	if (ret != ADOOK)
	{
		return false;
	}
	
	for (int i = 0; i < FLD_MAX; i++)
	{
		outParas[i].Empty();
	}

	sprintf(szT, "select count(*),black_notes from t_blackname where black_name = \'%s\' and black_id = \'%s\'", visitor.szName, visitor.szID);
	ret = tAdo.execSelect(szT, &FldCount, outParas);
	if (ret == ADOERR || (FldCount == 0 && outParas[0].IsEmpty()))
	{
		return false;
	}
	if (atoi(outParas[0]) >= 1)
	{
		strcpy(visitor.szNote,outParas[1]);
		return true;
	}
	return false;
}

//�����ֶ�ǩ��
BOOL CALLBACK OnOutResponse(HWINDOW hWindow, WPARAM wParam, LPARAM lParam)
{
	long msg = (long)wParam;
	switch(msg)
	{
	case 1:
		XMessageBox(g_hWindow, L"��ͨ������Զ��ǩ��!", L"��ʾ");
		break;
	case 2:
		XMessageBox(g_hWindow, L"��ͨ�����֤Զ��ǩ��!", L"��ʾ");
		break;
	case 3:
		XMessageBox(g_hWindow, L"ǩ������쳣!", L"��ʾ");
		break;
	default:
		break;
	}
	
	return false;
}

BOOL CALLBACK OnReCallBtnClick(HELE hEle, HELE hEventEle)
{
	wchar_t wBuf[256] = {0};
	char szT[512] = {'\0'};
	long FldCount = 0, ret = 0,lefttime;
	CString outParas[FLD_MAX];
	CWinAdo tAdo(sCONNSTR_MYSQL);
	HELE hList = GETELE(IDL_VISTORLIST);
	
	ret = tAdo.OpenAdo();
	if (ret != ADOOK)
	{
		return false;
	}
	int nChecked = 0;
	int nCount = XList_GetItemCount(hList);		//�ܼ�¼��

	//for (int j = 0; j <= nCount; j++)
	{
		/*if (!XList_IsItemCheck(hList, j))
		{
			continue;
		}
		nChecked++;	*/	
		int i;
		for (i = 0; i < FLD_MAX; i++)
		{
			outParas[i].Empty();
		}
		char szName[32] = {'\0'}, szID[64] = {'\0'}, szVisitTime[32] = {'\0'}, szOfficialName[32] = {'\0'}, szOfficeName[64] = {'\0'};
		char szDate[32] = {'\0'};
		int n = XList_GetSelectItem(hList);
		if (n<0)
		{
			XWnd_CloseWindow(XEle_GetHWindow(hEle));
			XWnd_RedrawWnd(g_hWindow);
			return false;
		}
		memset((char *)wBuf, 0, sizeof(wBuf));
		wcscpy(wBuf, XList_GetItemText(hList, n, 2));
		W_2_S(wBuf, szName, 128);
		
		memset((char *)wBuf, 0, sizeof(wBuf));
		wcscpy(wBuf, XList_GetItemText(hList, n, 4));
		W_2_S(wBuf, szID, 128);
		
		memset((char *)wBuf, 0, sizeof(wBuf));
		wcscpy(wBuf, XList_GetItemText(hList, n, 5));
		W_2_S(wBuf, szVisitTime, 128);
		
		//sprintf(szT, "select UNIX_TIMESTAMP(\'%s\')", szVisitTime);
		if (g_nSqlConnectType == SQL_CONNECT_SERVER2005)
		{	
			sprintf(szT, "select DATEDIFF(s, '1970-01-01 08:00:00', \'%s\')", szVisitTime);
		}
		else
			sprintf(szT, "select UNIX_TIMESTAMP(\'%s\')", szVisitTime);
		ret = tAdo.execSelect(szT, &FldCount, outParas);
		if (ret == ADOERR || (FldCount == 0 && outParas[0].IsEmpty()))
		{
			return false;
		}
		strcpy(szVisitTime, outParas[0]);		
		for (i = 0; i < FLD_MAX; i++)
		{
			outParas[i].Empty();
		}
		//char szTime[40] = {'\0'};
		//time((time_t *)&lefttime);
		//sprintf(szTime, "%ld", lefttime);

		//CallLeft2Server(szName,szID,szVisitTime,szTime,""); //Զ��ǩ�����
		//time((time_t *)&ret);
		//sprintf(szT, "update t_visitor set visitor_lefttime = %ld where visitor_name = \'%s\' and visitor_id = \'%s\' and visitor_visitetime = %ld", ret, szName, szID, atol(szVisitTime));
		sprintf(szT, "update t_visitor set visitor_calls = 0 where visitor_name = \'%s\' and visitor_id = \'%s\' and visitor_station_id = \'%s\' and visitor_visitetime = %ld",  szName, szID, g_szStation,atol(szVisitTime));
		ret = tAdo.execUpdateInsert(szT);

		::DeleteFile(USER_HEAD_PIC);            //ɾ���ϴ�ͷ��
	    ::DeleteFile(USER_ID_PIC); 
	    ::DeleteFile(USER_VIDEO_PIC);
	    ::DeleteFile(USER_BARCODE_PIC);
	}
	/*if (nChecked <= 0)
	{
		XMessageBox(g_hWindow, L"����ѡ���¼���ٽ���ǩ��!", L"����");
	}
	else 
	{
		XMessageBox(g_hWindow, L"��ѡ��ļ�¼ǩ�����!", L"��ʾ");
	}*/
	ListTodayVistorsInfo();
	VisitorTotalInfo(szT);	
	/*
	if (::GetPrivateProfileInt("TerminalSet", "bLeaveOfficial",0, CONFIG_FILE)==1)
	{
		//�������������
		InitVisitorData();
	}
	else
	{
		//�����������
		InitData();
	}*/
    XWnd_CloseWindow(XEle_GetHWindow(hEle));
	XWnd_RedrawWnd(g_hWindow);
	return false;
}

BOOL CALLBACK OnAllReCallBtnClick(HELE hEle, HELE hEventEle)
{
	wchar_t wBuf[256] = {0};
	char szT[512] = {'\0'};
	long FldCount = 0, ret = 0,lefttime;
	CString outParas[FLD_MAX];
	CWinAdo tAdo(sCONNSTR_MYSQL);
	HELE hList = GETELE(IDL_VISTORLIST);
	
	ret = tAdo.OpenAdo();
	if (ret != ADOOK)
	{
		return false;
	}

	//int nChecked = 0;
	int nCount = XList_GetItemCount(hList);		//�ܼ�¼��

	for (int j = 0; j < nCount; j++)
	{
		/*if (!XList_IsItemCheck(hList, j))
		{
			continue;
		}
		nChecked++;*/	
		int i ;
		for (i = 0; i < FLD_MAX; i++)
		{
			outParas[i].Empty();
		}
		char szName[32] = {'\0'}, szID[64] = {'\0'}, szVisitTime[32] = {'\0'}, szOfficialName[32] = {'\0'}, szOfficeName[64] = {'\0'};
		char szDate[32] = {'\0'};
		int n = j;
		memset((char *)wBuf, 0, sizeof(wBuf));
		wcscpy(wBuf, XList_GetItemText(hList, n, 2));
		W_2_S(wBuf, szName, 128);
		
		memset((char *)wBuf, 0, sizeof(wBuf));
		wcscpy(wBuf, XList_GetItemText(hList, n, 4));
		W_2_S(wBuf, szID, 128);
		
		memset((char *)wBuf, 0, sizeof(wBuf));
		wcscpy(wBuf, XList_GetItemText(hList, n, 5));
		W_2_S(wBuf, szVisitTime, 128);
		
		//sprintf(szT, "select UNIX_TIMESTAMP(\'%s\')", szVisitTime);
		if (g_nSqlConnectType == SQL_CONNECT_SERVER2005)
		{	
			sprintf(szT, "select DATEDIFF(s, '1970-01-01 08:00:00', \'%s\')", szVisitTime);
		}
		else
			sprintf(szT, "select UNIX_TIMESTAMP(\'%s\')", szVisitTime);
		ret = tAdo.execSelect(szT, &FldCount, outParas);
		if (ret == ADOERR || (FldCount == 0 && outParas[0].IsEmpty()))
		{
			return false;
		}
		strcpy(szVisitTime, outParas[0]);		
		for (i = 0; i < FLD_MAX; i++)
		{
			outParas[i].Empty();
		}

		//char szTime[40] = {'\0'};
		//time((time_t *)&lefttime);
		//sprintf(szTime, "%ld", lefttime);
		//CallLeft2Server(szName,szID,szVisitTime,szTime,""); //Զ��ǩ�����

		//sprintf(szT, "update t_visitor set visitor_lefttime = %ld where visitor_name = \'%s\' and visitor_id = \'%s\' and visitor_visitetime = %ld", ret, szName, szID, atol(szVisitTime));
        sprintf(szT, "update t_visitor set visitor_calls = 0 where visitor_name = \'%s\' and visitor_id = \'%s\' and visitor_station_id = \'%s\' and visitor_visitetime = %ld",  szName, szID,g_szStation, atol(szVisitTime));
		ret = tAdo.execUpdateInsert(szT);
		::DeleteFile(USER_HEAD_PIC);            //ɾ���ϴ�ͷ��
	    ::DeleteFile(USER_ID_PIC); 
	    ::DeleteFile(USER_VIDEO_PIC);
	    ::DeleteFile(USER_BARCODE_PIC);
	}
	/*if (nChecked <= 0)
	{
		XMessageBox(g_hWindow, L"����ѡ���¼���ٽ���ǩ��!", L"����");
	}
	else 
	{
		XMessageBox(g_hWindow, L"��ѡ��ļ�¼ǩ�����!", L"��ʾ");
	}*/
	ListTodayVistorsInfo();
	VisitorTotalInfo(szT);	
	/*
	if (::GetPrivateProfileInt("TerminalSet", "bLeaveOfficial",0, CONFIG_FILE)==1)
	{
		//�������������
		InitVisitorData();
	}
	else
	{
		//�����������
		InitData();
	}*/
	XWnd_CloseWindow(XEle_GetHWindow(hEle));
	XWnd_RedrawWnd(g_hWindow);
	return false;
}

BOOL CALLBACK OnCloseLogOutBtnClick(HELE hEle, HELE hEventEle)
{
	bQuit = false;
	HideSoftKeybd();

    XWnd_CloseWindow(XEle_GetHWindow(hEle));
	XWnd_RedrawWnd(g_hWindow);
	return false;
}

BOOL CALLBACK OnOKSysBtnClick(HELE hEle, HELE hEventEle)
{
	wchar_t wBuf[256] = {0};
	char szPWD[256] = {'\0'},szT[256]={'\0'};
	CString outParas[FLD_MAX];
	CWinAdo tAdo(sCONNSTR_MYSQL);
	long FldCount=0;

	long ret = tAdo.OpenAdo();
	if (ret != ADOOK)
	{
		return false;
	}
	
	for (int i = 0; i < FLD_MAX; i++)
	{
		outParas[i].Empty();
	}
	memset((char *)wBuf, 0, sizeof(wBuf));
	XEdit_GetText(htxtChaxunPwd,wBuf,128);
	W_2_S(wBuf, szPWD, 128);
	sprintf(szT, "select user_right_id from t_user where user_pwd = \'%s\'", szPWD);
	ret = tAdo.execSelect(szT, &FldCount, outParas);
	if (ret == ADOERR || (FldCount == 0 && outParas[0].IsEmpty()))
	{
		XMessageBox(g_hWindow, L"�������,��¼ʧ��!", L"����");
	//	XWnd_RedrawWnd(XEle_GetHWindow(hEle));
		return false;
	}
	//SuspendThread(g_HWDThreadID2ID);  //��ͣ����֤��̨�߳�
    SuspendThread(g_ThreadReadBarcode);
	XWnd_CloseWindow(XEle_GetHWindow(hEle));
	

	XEle_ShowEle(g_hPage1, false);
	XEle_ShowEle(g_hPage3, true);
	g_bICReg = true;
    ShowWindow(g_hCapWnd, false);
	Pause();
	//OnQueryEmployerForList(XWnd_GetEle(g_hWindow, IDB_QUERYOFFICIAL), XWnd_GetEle(g_hWindow, IDB_QUERYOFFICIAL));	
	XEle_ShowEle(XWnd_GetEle(g_hWindow, IDP_PAGE3_1), true);
	
	EnableButton(IDB_TAB32);

	SetSexInfo(IDC_ADD_OFFICIALSEX);
	HELE hE = XWnd_GetEle(g_hWindow, IDC_ADD_OFFICIALSEX);
	XComboBox_AddString(hE, L" ");

	HELE hList = XWnd_GetEle(g_hWindow, IDL_LISTOFFICIAL);		//����б�Ԫ��
	XList_DeleteAllItems(hList);
	XList_DeleteAllColumns(hList);
	
	XList_SetHeaderHeight(hList, 30);
	XList_EnableGrid(hList, false);

	XList_AddColumn(hList, 40, L"���");
	XList_AddColumn(hList, 100, L"����");
	XList_AddColumn(hList, 40, L"�Ա�");
	XList_AddColumn(hList, 100, L"�칫�绰");
	XList_AddColumn(hList, 100, L"�ƶ��绰");
	XList_AddColumn(hList, 140, L"���ſ���");
	XList_AddColumn(hList, 160, L"�칫�ص�");
	if(g_bSSO)
	{		
		XList_AddColumn(hList, 0, L"����");
		XList_AddColumn(hList, 0, L"����");
		XList_AddColumn(hList, 70, L"SSO");
		XList_AddColumn(hList, 70, L"IC��");
	}
	else
	{
		XList_AddColumn(hList, 70, L"����");
		XList_AddColumn(hList, 70, L"����");
		XList_AddColumn(hList, 0, L"SSO");
		XList_AddColumn(hList, 0, L"IC��");
	}
	XList_AddColumn(hList, 90, L"IP��ַ");
    OnQueryEmployerForList(XWnd_GetEle(g_hWindow, IDB_QUERYOFFICIAL), XWnd_GetEle(g_hWindow, IDB_QUERYOFFICIAL));
	XEle_SetFocus(g_hPage1,false);
    XEle_SetFocus(g_hPage3,true);
	//CloseSoftKeybd();
	//CloseXYB();
	HideSoftKeybd();
	ShowStandSoftKeybd();
	//::WritePrivateProfileString("TerminalSet", "DataInputPage", "3", CONFIG_FILE);
	//StartKeybd();
    //OnOfficial1SetFocus(XWnd_GetEle(g_hWindow, 5030), XWnd_GetEle(g_hWindow, 5030));
	XWnd_RedrawWnd(g_hWindow);
	return true;
}

BOOL CALLBACK OnOKBtnClick(HELE hEle, HELE hEventEle)
{
	wchar_t wBuf[256] = {0};
	char szPWD[256] = {'\0'},szT[256]={'\0'};
	CString outParas[FLD_MAX];
	CWinAdo tAdo(sCONNSTR_MYSQL);
	long FldCount=0;

	long ret = tAdo.OpenAdo();
	if (ret != ADOOK)
	{
		return false;
	}
	
	for (int i = 0; i < FLD_MAX; i++)
	{
		outParas[i].Empty();
	}
	memset((char *)wBuf, 0, sizeof(wBuf));
	XEdit_GetText(htxtChaxunPwd,wBuf,128);
	W_2_S(wBuf, szPWD, 128);
	sprintf(szT, "select user_right_id from t_user where user_pwd = \'%s\'", szPWD);
	ret = tAdo.execSelect(szT, &FldCount, outParas);
	if (ret == ADOERR || (FldCount == 0 && outParas[0].IsEmpty()))
	{
		XMessageBox(g_hWindow, L"�������,��¼ʧ��!", L"����");
	//	XWnd_RedrawWnd(XEle_GetHWindow(hEle));
		return false;
	}
	SuspendThread(g_HWDThreadID2ID);  //��ͣ����֤��̨�߳�
	SuspendThread(g_ThreadReadBarcode);
	XWnd_CloseWindow(XEle_GetHWindow(hEle));
	XBtn_SetCheck(GETELE(IDB_CHAXUN), true);
	XBtn_SetCheck(GETELE(IDB_TONGJI), false);
	XBtn_SetCheck(GETELE(IDB_TONGJICHAXUNRETURN), false);


    ShowWindow(g_hCapWnd, false);
	Pause();
	XEle_ShowEle(g_hPage1, false);
	XEle_ShowEle(g_hPage6, true);

	ChaXunToDayInfo();

 	HideSoftKeybd();
	ShowStandSoftKeybd();
	XWnd_RedrawWnd(g_hWindow);
	return true;
}

BOOL CALLBACK OnAllLogOutBtnClick(HELE hEle, HELE hEventEle)
{
	wchar_t wBuf[256] = {0};
	char szT[512] = {'\0'};
	long FldCount = 0, ret = 0,lefttime;
	CString outParas[FLD_MAX];
	CWinAdo tAdo(sCONNSTR_MYSQL);
	HELE hList = GETELE(IDL_VISTORLIST);
	
	ret = tAdo.OpenAdo();
	if (ret != ADOOK)
	{
		return false;
	}

	//int nChecked = 0;
	int nCount = XList_GetItemCount(hList);		//�ܼ�¼��

	for (int j = 0; j < nCount; j++)
	{
		/*if (!XList_IsItemCheck(hList, j))
		{
			continue;
		}
		nChecked++;*/	
		int i ;
		for (i = 0; i < FLD_MAX; i++)
		{
			outParas[i].Empty();
		}
		char szName[32] = {'\0'}, szID[64] = {'\0'}, szVisitTime[32] = {'\0'}, szOfficialName[32] = {'\0'}, szOfficeName[64] = {'\0'};
		char szDate[32] = {'\0'};
		int n = j;
		memset((char *)wBuf, 0, sizeof(wBuf));
		wcscpy(wBuf, XList_GetItemText(hList, n, 2));
		W_2_S(wBuf, szName, 128);
		
		memset((char *)wBuf, 0, sizeof(wBuf));
		wcscpy(wBuf, XList_GetItemText(hList, n, 4));
		W_2_S(wBuf, szID, 128);
		
		memset((char *)wBuf, 0, sizeof(wBuf));
		wcscpy(wBuf, XList_GetItemText(hList, n, 5));
		W_2_S(wBuf, szVisitTime, 128);
		
		//sprintf(szT, "select UNIX_TIMESTAMP(\'%s\')", szVisitTime);
		if (g_nSqlConnectType == SQL_CONNECT_SERVER2005)
		{	
			sprintf(szT, "select DATEDIFF(s, '1970-01-01 08:00:00', \'%s\')", szVisitTime);
		}
		else
			sprintf(szT, "select UNIX_TIMESTAMP(\'%s\')", szVisitTime);
		ret = tAdo.execSelect(szT, &FldCount, outParas);
		if (ret == ADOERR || (FldCount == 0 && outParas[0].IsEmpty()))
		{
			return false;
		}
		strcpy(szVisitTime, outParas[0]);		
		for (i = 0; i < FLD_MAX; i++)
		{
			outParas[i].Empty();
		}

		char szTime[40] = {'\0'};
		time((time_t *)&lefttime);
		sprintf(szTime, "%ld", lefttime);
		CallLeft2Server(szName,szID,szVisitTime,szTime,""); //Զ��ǩ�����
		//time((time_t *)&ret);
		//sprintf(szT, "update t_visitor set visitor_lefttime = %ld where visitor_name = \'%s\' and visitor_id = \'%s\' and visitor_visitetime = %ld", ret, szName, szID, atol(szVisitTime));
        sprintf(szT, "update t_visitor set visitor_lefttime = %ld where visitor_name = \'%s\' and visitor_id = \'%s\' and visitor_station_id = \'%s\' and visitor_visitetime = %ld", lefttime, szName, szID,g_szStation, atol(szVisitTime));
		ret = tAdo.execUpdateInsert(szT);
		::DeleteFile(USER_HEAD_PIC);            //ɾ���ϴ�ͷ��
	    ::DeleteFile(USER_ID_PIC); 
	    ::DeleteFile(USER_VIDEO_PIC);
	    ::DeleteFile(USER_BARCODE_PIC);
	}
	/*if (nChecked <= 0)
	{
		XMessageBox(g_hWindow, L"����ѡ���¼���ٽ���ǩ��!", L"����");
	}
	else 
	{
		XMessageBox(g_hWindow, L"��ѡ��ļ�¼ǩ�����!", L"��ʾ");
	}*/
	ListTodayVistorsInfo();
	VisitorTotalInfo(szT);	
	if (::GetPrivateProfileInt("TerminalSet", "bLeaveOfficial",0, CONFIG_FILE)==1)
	{
		//�������������
		InitVisitorData();
	}
	else
	{
		//�����������
		InitData();
	}
	XWnd_CloseWindow(XEle_GetHWindow(hEle));
	XWnd_RedrawWnd(g_hWindow);
	return false;
}

BOOL CALLBACK OnLSBtnClick(HELE hEle, HELE hEventEle)
{
	wchar_t wBuf[256] = {0};
	HELE hE;
    VISITORINFO visitor;
	memset((char *)&visitor, 0, sizeof(VISITORINFO));

	memset((char *)wBuf, 0, sizeof(wBuf));
	hE = XWnd_GetEle(g_hWindow, IDE_VISITORNAME);
	XEdit_GetText(hE, wBuf, 128);
	W_2_S(wBuf, visitor.szName, 128);

	memset((char *)wBuf, 0, sizeof(wBuf));
	hE = XWnd_GetEle(g_hWindow, IDE_VISITORIDNUM);
	XEdit_GetText(hE, wBuf, 128);
	W_2_S(wBuf, visitor.szID, 128);
	
	HideSoftKeybd();
	WritePrivateProfileString("HtmlPageTips", "HeadWords","��ʱ�ÿ͵�", CONFIG_FILE);

	int b = CheckVisitor(visitor);
	if (!b)  //������
	{
		BackScreenPlayADV();//��ӡ��͵�ʱ���ָ��������
	    SaveAndPrint();		
	}
	else if (b==-1)  //�����˸ı���δǩ��
	{
		return false;
	}
	else             //�����͵�
	{
		
		if (strlen(g_Printvisitor.szName)==0)
		{
              XMessageBox(g_hWindow, L"�޴�ӡ��Ϣ!", L"����");
		      return false;
		}
		PrintVisitorNoteAgain();

	}
    return false;
}

BOOL CALLBACK OnXCBtnClick(HELE hEle, HELE hEventEle)
{
	wchar_t wBuf[256] = {0};
	HELE hE;
    VISITORINFO visitor;
	memset((char *)&visitor, 0, sizeof(VISITORINFO));

	memset((char *)wBuf, 0, sizeof(wBuf));
	hE = XWnd_GetEle(g_hWindow, IDE_VISITORNAME);
	XEdit_GetText(hE, wBuf, 128);
	W_2_S(wBuf, visitor.szName, 128);

	memset((char *)wBuf, 0, sizeof(wBuf));
	hE = XWnd_GetEle(g_hWindow, IDE_VISITORIDNUM);
	XEdit_GetText(hE, wBuf, 128);
	W_2_S(wBuf, visitor.szID, 128);
	
	HideSoftKeybd();
	WritePrivateProfileString("HtmlPageTips", "HeadWords","Ѳ�鵥", CONFIG_FILE);

	int b = CheckVisitor(visitor);
	if (!b)  //������
	{
		BackScreenPlayADV();//��ӡ��͵�ʱ���ָ��������
	    SaveAndPrint();		
	}
	else if (b==-1)  //�����˸ı���δǩ��
	{
		return false;
	}
	else             //�����͵�
	{
		
		if (strlen(g_Printvisitor.szName)==0)
		{
              XMessageBox(g_hWindow, L"�޴�ӡ��Ϣ!", L"����");
		      return false;
		}
		PrintVisitorNoteAgain();

	}
    return false;
}

BOOL CALLBACK OnPTBtnClick(HELE hEle, HELE hEventEle)
{
	wchar_t wBuf[256] = {0};
	HELE hE;
    VISITORINFO visitor;
	memset((char *)&visitor, 0, sizeof(VISITORINFO));

	memset((char *)wBuf, 0, sizeof(wBuf));
	hE = XWnd_GetEle(g_hWindow, IDE_VISITORNAME);
	XEdit_GetText(hE, wBuf, 128);
	W_2_S(wBuf, visitor.szName, 128);

	memset((char *)wBuf, 0, sizeof(wBuf));
	hE = XWnd_GetEle(g_hWindow, IDE_VISITORIDNUM);
	XEdit_GetText(hE, wBuf, 128);
	W_2_S(wBuf, visitor.szID, 128);
	
	HideSoftKeybd();
	WritePrivateProfileString("HtmlPageTips", "HeadWords","��͵�", CONFIG_FILE);

	int b = CheckVisitor(visitor);
	if (!b)  //������
	{
		BackScreenPlayADV();//��ӡ��͵�ʱ���ָ��������
	    SaveAndPrint();		
	}
	else if (b==-1)  //�����˸ı���δǩ��
	{
		return false;
	}
	else             //�����͵�
	{
		
		if (strlen(g_Printvisitor.szName)==0)
		{
              XMessageBox(g_hWindow, L"�޴�ӡ��Ϣ!", L"����");
		      return false;
		}
		PrintVisitorNoteAgain();


	}
    return false;
}

BOOL CALLBACK OnQTBtnClick(HELE hEle, HELE hEventEle)
{
	wchar_t wBuf[256] = {0};
	HELE hE;
    VISITORINFO visitor;
	memset((char *)&visitor, 0, sizeof(VISITORINFO));

	memset((char *)wBuf, 0, sizeof(wBuf));
	hE = XWnd_GetEle(g_hWindow, IDE_VISITORNAME);
	XEdit_GetText(hE, wBuf, 128);
	W_2_S(wBuf, visitor.szName, 128);

	memset((char *)wBuf, 0, sizeof(wBuf));
	hE = XWnd_GetEle(g_hWindow, IDE_VISITORIDNUM);
	XEdit_GetText(hE, wBuf, 128);
	W_2_S(wBuf, visitor.szID, 128);
	
	HideSoftKeybd();
	WritePrivateProfileString("HtmlPageTips", "HeadWords","��͵�2", CONFIG_FILE);

	int b = CheckVisitor(visitor);
	if (!b)  //������
	{
		BackScreenPlayADV();//��ӡ��͵�ʱ���ָ��������
	    SaveAndPrint();		
	}
	else if (b==-1)  //�����˸ı���δǩ��
	{
		return false;
	}
	else             //�����͵�
	{
		
		if (strlen(g_Printvisitor.szName)==0)
		{
              XMessageBox(g_hWindow, L"�޴�ӡ��Ϣ!", L"����");
		      return false;
		}
		PrintVisitorNoteAgain();

	}
    return false;
}

BOOL CALLBACK OnLogOutBtnClick(HELE hEle, HELE hEventEle)
{
	wchar_t wBuf[256] = {0};
	char szT[512] = {'\0'};
	long FldCount = 0, ret = 0,lefttime;
	CString outParas[FLD_MAX];
	CWinAdo tAdo(sCONNSTR_MYSQL);
	HELE hList = GETELE(IDL_VISTORLIST);
	
	ret = tAdo.OpenAdo();
	if (ret != ADOOK)
	{
		return false;
	}
	int nChecked = 0;
	int nCount = XList_GetItemCount(hList);		//�ܼ�¼��

	//for (int j = 0; j <= nCount; j++)
	{
		/*if (!XList_IsItemCheck(hList, j))
		{
			continue;
		}
		nChecked++;	*/	
		int i;
		for (i = 0; i < FLD_MAX; i++)
		{
			outParas[i].Empty();
		}
		char szName[32] = {'\0'}, szID[64] = {'\0'}, szVisitTime[32] = {'\0'}, szOfficialName[32] = {'\0'}, szOfficeName[64] = {'\0'};
		char szDate[32] = {'\0'};
		int n = XList_GetSelectItem(hList);
		if (n<0)
		{
			XWnd_CloseWindow(XEle_GetHWindow(hEle));
			XWnd_RedrawWnd(g_hWindow);
			return false;
		}
		memset((char *)wBuf, 0, sizeof(wBuf));
		wcscpy(wBuf, XList_GetItemText(hList, n, 2));
		W_2_S(wBuf, szName, 128);
		
		memset((char *)wBuf, 0, sizeof(wBuf));
		wcscpy(wBuf, XList_GetItemText(hList, n, 4));
		W_2_S(wBuf, szID, 128);
		
		memset((char *)wBuf, 0, sizeof(wBuf));
		wcscpy(wBuf, XList_GetItemText(hList, n, 5));
		W_2_S(wBuf, szVisitTime, 128);
		
		//sprintf(szT, "select UNIX_TIMESTAMP(\'%s\')", szVisitTime);
		if (g_nSqlConnectType == SQL_CONNECT_SERVER2005)
		{	
			sprintf(szT, "select DATEDIFF(s, '1970-01-01 08:00:00', \'%s\')", szVisitTime);
		}
		else
			sprintf(szT, "select UNIX_TIMESTAMP(\'%s\')", szVisitTime);
		ret = tAdo.execSelect(szT, &FldCount, outParas);
		if (ret == ADOERR || (FldCount == 0 && outParas[0].IsEmpty()))
		{
			return false;
		}
		strcpy(szVisitTime, outParas[0]);		
		for (i = 0; i < FLD_MAX; i++)
		{
			outParas[i].Empty();
		}
		char szTime[40] = {'\0'};
		time((time_t *)&lefttime);
		sprintf(szTime, "%ld", lefttime);

		CallLeft2Server(szName,szID,szVisitTime,szTime,""); //Զ��ǩ�����
		//time((time_t *)&ret);
		//sprintf(szT, "update t_visitor set visitor_lefttime = %ld where visitor_name = \'%s\' and visitor_id = \'%s\' and visitor_visitetime = %ld", ret, szName, szID, atol(szVisitTime));
		sprintf(szT, "update t_visitor set visitor_lefttime = %ld where visitor_name = \'%s\' and visitor_id = \'%s\' and visitor_station_id = \'%s\' and visitor_visitetime = %ld", lefttime, szName, szID, g_szStation,atol(szVisitTime));
		ret = tAdo.execUpdateInsert(szT);

		::DeleteFile(USER_HEAD_PIC);            //ɾ���ϴ�ͷ��
	    ::DeleteFile(USER_ID_PIC); 
	    ::DeleteFile(USER_VIDEO_PIC);
	    ::DeleteFile(USER_BARCODE_PIC);
	}
	/*if (nChecked <= 0)
	{
		XMessageBox(g_hWindow, L"����ѡ���¼���ٽ���ǩ��!", L"����");
	}
	else 
	{
		XMessageBox(g_hWindow, L"��ѡ��ļ�¼ǩ�����!", L"��ʾ");
	}*/
	ListTodayVistorsInfo();
	VisitorTotalInfo(szT);	
	if (::GetPrivateProfileInt("TerminalSet", "bLeaveOfficial",0, CONFIG_FILE)==1)
	{
		//�������������
		InitVisitorData();
	}
	else
	{
		//�����������
		InitData();
	}
    XWnd_CloseWindow(XEle_GetHWindow(hEle));
	XWnd_RedrawWnd(g_hWindow);
	return true;
}

void SetUserRight()
{
	char szRight[10]={'\0'};
	HELE hE = GETELE(IDE_ADD_CZYRIGHT);
	XComboBox_DeleteItemAll(hE);
	::GetPrivateProfileString("TerminalSet", "UserRight","", szRight, sizeof(szRight),CONFIG_FILE);
	if (atoi(szRight) == 2)
	{
		XComboBox_AddString(hE, L"����Ա");
		XComboBox_AddString(hE, L"һ�����Ա");
	}
	else if(atoi(szRight) == 0)
	{
		//XComboBox_AddString(hE, L"����Ա");
		XComboBox_AddString(hE, L"һ�����Ա");
	}
	
	XComboBox_SetSelectItem(hE, 0);
	return;
}

BOOL CALLBACK OnLoginOK(HELE hEle, HELE hEventEle)
{
	//g_bAutoCallOfficials = true;
	wchar_t wBuf[256] = {0};
	char szT[512] = {'\0'};
	long FldCount = 0, ret = 0;
	CString outParas[FLD_MAX];
	CWinAdo tAdo(sCONNSTR_MYSQL);

	ret = tAdo.OpenAdo();
	if (ret != ADOOK)
	{
		return false;
	}
	
	for (int i = 0; i < FLD_MAX; i++)
	{
		outParas[i].Empty();
	}

	char szID[32] = {'\0'}, szMM[32] = {'\0'},szUserName[20] = {'\0'};
	//::GetPrivateProfileString("TerminalSet", "UserName", "", szUserName, sizeof(szUserName), CONFIG_FILE);
	HELE hE = GETELE(IDE_LOGIN_CZYID);
	/*if (strlen(szUserName)>0)
	{
        S_2_W(wBuf,szUserName,32);
		XEdit_SetText(hE,wBuf);
	}*/
	//strcpy(g_szOfficialIP,"cc");
	//SendRequestToOfficial("<visitor><name>ˮ �� ��</name><id>1234567890</id><reason>�;�</reason><Pic>c:\\head.bmp</Pic><ip>127.0.0.1</ip></visitor>");
	//CallLeft2Server("���","1234456678","","14693939382","");
	//CallLeft2Server("","","","14693939382","33445566");

	XEdit_GetText(hE, wBuf, 32);
	if (wcslen(wBuf) <= 0)
	{
		XMessageBox(g_hWindow, L"��¼�û�������Ϊ��!", L"����");
		XWnd_RedrawWnd(XEle_GetHWindow(hEle));
		return false;
	}
	W_2_S(wBuf, szID, 32);

	memset((char *)wBuf, 0, sizeof(wBuf));
	hE = GETELE(IDE_LOGIN_CZYMM);
	XEdit_GetText(hE, wBuf, 32);
	if (wcslen(wBuf) <= 0)
	{
		XMessageBox(g_hWindow, L"��¼�û����벻��Ϊ��!", L"����");
		XWnd_RedrawWnd(XEle_GetHWindow(hEle));
		return false;
	}
	W_2_S(wBuf, szMM, 32);

	sprintf(szT, "select user_right_id,user_id from t_user where user_id = \'%s\' and user_pwd = \'%s\'", szID, szMM);
	ret = tAdo.execSelect(szT, &FldCount, outParas);
	if (ret == ADOERR || (FldCount == 0 && outParas[0].IsEmpty()))
	{
		XMessageBox(g_hWindow, L"�û������������,��¼ʧ��!", L"����");
		XWnd_RedrawWnd(XEle_GetHWindow(hEle));
		return false;
	}

	::WritePrivateProfileString("TerminalSet", "UserRight", outParas[0], CONFIG_FILE);
	if (atoi(outParas[0]) == 1)
	{
		XEle_EnableEle(GETELE(IDB_TAB10), false);		//ϵͳ���ð�ťʧ��
	}
	::WritePrivateProfileString("TerminalSet", "UserName", outParas[1], CONFIG_FILE);
	SetSysInfo();

	//XEle_ShowEle(g_hPage1, false);
	//XEle_ShowEle(g_hPage3, true);
	//XEle_ShowEle(g_hPage10, false);
    XWnd_SetCaptionHeight(g_hWindow,28);
	XEle_ShowEle(g_hPage1, true);		
	XEle_ShowEle(g_hPage10, false);


    BOOL b = VideoPreview2();				//������ƵԤ��
	if (!b)
	{
		XMessageBox(g_hWindow, L"��������ͷʧ��", L"����");
	}
	HideSoftKeybd();
	StartBackScreen(); //�򿪱�������

	g_tSock = new CNovelSocket(SOCK_DGRAM);
	ret = g_tSock->StartServer(25006);
	if (ret != 0)
	{
		XMessageBox(g_hWindow, L"����ʱ��Ķ˿ڱ�ռ��!", L"����");
		return 0;
	}
	g_ThreadSocketMutex=CreateMutex(NULL,FALSE,NULL);
	g_HWDThreadTime=CreateThread(NULL, 0, SendTimeThread, NULL, 0, NULL);       //Ϊ�����ն˷���ʱ�䳣פ�߳�

	
	CreateThread(NULL, 0, AutoCallOfficialsThread, NULL, 0, NULL);
	//g_UDPSock = new CAsyncSocket();
	//g_UDPSock->Create(25008,SOCK_DGRAM);
    /*if (!AfxSocketInit())
	{
		AfxMessageBox("111");
		return FALSE;
	}
	CAsyncSocket s;
	s.Create(25008,SOCK_DGRAM);/
	/*g_hDllJBC=LoadLibrary(_T("JBC7200.dll"));
	if (g_hDllJBC==NULL)
	{
		FreeLibrary(g_hDllJBC);
		return false;
	}
	typedef int (PASCAL *pFun1)();
    pFun1 pFun;
	pFun=(pFun1)GetProcAddress(g_hDllJBC, "JBC7K2_OpenSocket");   
	ret=pFun();
	if (ret!=0)
	{
		return false;
	}	
	OpenRegCardCom(4);*/      //�򿪷������˿�
//	CreateThread(NULL, 0, ReadChannelRegThread, NULL, 0, NULL);
    //EmptyVisitorPara();
	XWnd_RedrawWnd(XEle_GetHWindow(hEle));
	return false;
}

BOOL CALLBACK OnLoginNo(HELE hEle, HELE hEventEle)
{
	g_bAutoCallOfficials = false;
	if (g_hWeb != NULL)
	{
		XWeb_Destroy(g_hWeb);
	}
	OleUninitialize();
	
	g_bStopID2 = true;			//ֹͣ����֤��д�߳�
	g_bStopBarCode = true;		//ֹͣ�����Ķ��߳�
	
	FreeIdcardLibrary();		//ж�ض���֤��д��
	EndWJUsb();
	CloseCardReader();
	HideTaskBar(false);
    CloseSoftKeybd();
	CloseXYB();
	XWnd_CloseWindow(XEle_GetHWindow(hEle));
	
	return false;
}

BOOL CALLBACK OnShowPwdKeyBd(HELE hEle, HELE hEventEle)
{
	/*if(!StartKeybd())
		return false;
	return true;*/
    //
	//StartKeybd();
	//SendKeyBdAppData("299");
	ShowStandSoftKeybd();
    //MoveBottomSoftKeybd();
	XWnd_RedrawWnd(g_hWindow);
	return false;
}

BOOL CALLBACK OnClosePwdKeybd(HELE hEle, HELE hEventEle)
{
	HideSoftKeybd();
    XWnd_RedrawWnd(g_hWindow);
	return false;
}

BOOL ImportOfficial(char *pszFileName)
{
	if (_access(pszFileName,0)) //����ͬ���ļ�
	{
		return false;
	}

	char szT[512] = {'\0'};
	long FldCount = 0, ret = 0;;
	//CString outParas[FLD_MAX];
	CWinAdo tAdo(sCONNSTR_MYSQL);

	//CString str = CONNSTR_EXCEL + CString(pszFileName);
	CString outXLS[FLD_MAX];
	
	ret = tAdo.OpenAdo();
	if (ret != ADOOK)
	{
		return false;
	}
	
	for (int i = 0; i < FLD_MAX; i++)
	{
		//outParas[i].Empty();
		outXLS[i].Empty();
	}
	
	CDatabase database;
    CString sDriver = "MICROSOFT EXCEL DRIVER (*.XLS)"; // Excel��װ����
    CString sExcelFile = pszFileName;

	    // �������д�ȡ���ַ���
        CString strSql;
        strSql.Format("DRIVER={%s};DSN='';FIRSTROWHASNAMES=1;READONLY=FALSE;CREATE_DB=\"%s\";DBQ=%s",sDriver, sExcelFile, sExcelFile);
        
        // �������ݿ� (��Excel����ļ�)
        if(!database.OpenEx(strSql,CDatabase::noOdbcDialog))
        {
            printf("Excel����ʧ��\n");
            return false;
        }
        CRecordset rs(&database);       
        // ���ö�ȡ�Ĳ�ѯ���
        //strSql = "SELECT * FROM t_official";
		strSql = "SELECT * FROM [t_official$A1:IV65536]";
        
		try
		{
			// ִ�в�ѯ���
			rs.Open(CRecordset::forwardOnly, strSql);
		}
		catch(_com_error &e)
		{
			AfxMessageBox(e.Description());//��ʾ����ԭ��
			database.Close();
			return false;
		}
		catch(...)
		{
			database.Close();
            AfxMessageBox("����޷��򿪣������ʽ�Ƿ���ȷ");
			return false;
		}
  
		CDBVariant varValue;
        while (!rs.IsEOF())
        {
            //��ȡExcel�ڲ���ֵ
            rs.GetFieldValue("official_name", outXLS[0]);
			if (outXLS[0]=="")
			{
				rs.MoveNext();
				continue;
			}
            rs.GetFieldValue("official_sex", outXLS[1]);
			//rs.GetFieldValue("official_office_phone", outXLS[2]);
			varValue.Clear();
			outXLS[2]= "";
			rs.GetFieldValue("official_office_phone", varValue);
			if (varValue.m_dwType==DBVT_DOUBLE)
			{
 				 outXLS[2].Format("%f",varValue.m_dblVal);
				 outXLS[2]=outXLS[2].Left(outXLS[2].Find("."));
                 //outXLS[2].Format("%d",(int)varValue.m_dblVal);
			}
			else if (varValue.m_dwType==DBVT_STRING)
			{
                 outXLS[2]= *varValue.m_pstring;
			}
			else            
			     outXLS[2]= "";


            varValue.Clear();
			rs.GetFieldValue("official_mobile", varValue);
			outXLS[3]= "";
			if (varValue.m_dwType==DBVT_DOUBLE)
			{
                 //int n=(int)varValue.m_dblVal;
                 //outXLS[3].Format("%d",varValue.m_dblVal);
				 outXLS[3].Format("%f",varValue.m_dblVal);
				 outXLS[3]=outXLS[3].Left(outXLS[3].Find("."));
			}
			else if (varValue.m_dwType==DBVT_STRING)
			{
                 outXLS[3]= *varValue.m_pstring;
			}
			else            
			     outXLS[3]= "";

            rs.GetFieldValue("official_office_name", outXLS[4]);
            rs.GetFieldValue("official_office_addr", outXLS[5]);
			rs.GetFieldValue("official_duty", outXLS[6]);
			//rs.GetFieldValue("official_room", outXLS[7]);

			varValue.Clear();
			outXLS[7]= "";
		    rs.GetFieldValue("official_room", varValue);
			if (varValue.m_dwType==DBVT_DOUBLE)
			{
				 outXLS[7].Format("%f",varValue.m_dblVal);
				 outXLS[7]=outXLS[7].Left(outXLS[7].Find("."));
			}
			else if (varValue.m_dwType==DBVT_STRING)
			{
                 outXLS[7]= *varValue.m_pstring;
			}
			else            
			     outXLS[7]= "";

            rs.GetFieldValue("official_IP", outXLS[8]);
			rs.GetFieldValue("official_SSO", outXLS[9]);


            sprintf(szT, "insert into t_official (official_name,official_sex,official_office_phone,official_mobile,official_office_name,official_office_addr,official_duty,official_room,official_IP,official_state,official_pwd,official_NO,official_SSO) \
		    values('\%s\', '\%s\', '\%s\', '\%s\', '\%s\', '\%s\', '\%s\', '\%s\', '\%s\',0,'123','', '\%s\')",\
			outXLS[0], outXLS[1], outXLS[2], outXLS[3], outXLS[4], outXLS[5], outXLS[6], outXLS[7], outXLS[8], outXLS[9]);
		    ret = tAdo.execUpdateInsert(szT);
            // �Ƶ���һ��
            rs.MoveNext();
        }
      rs.Close();
    database.Close();

	return true;
}

BOOL ExportOfficial(char *pszFileName)
{
	if (!_access(pszFileName,0)) //����ͬ���ļ�
	{
		return false;
	}
	char szT[512] = {'\0'};
	long FldCount = 0, ret = 0;
	CString outParas[FLD_MAX];
	CWinAdo tAdo(sCONNSTR_MYSQL);

	CDatabase database;
    CString sDriver = "MICROSOFT EXCEL DRIVER (*.XLS)"; // Excel��װ����
    CString sExcelFile = pszFileName;

	    // �������д�ȡ���ַ���
        CString strSql;
        strSql.Format("DRIVER={%s};DSN='';FIRSTROWHASNAMES=1;READONLY=FALSE;CREATE_DB=\"%s\";DBQ=%s",sDriver, sExcelFile, sExcelFile);
        
        // �������ݿ� (��Excel����ļ�)
        if(!database.OpenEx(strSql,CDatabase::noOdbcDialog))
        {
            printf("Excel����ʧ��\n");
            return false;
        }
        strSql = "CREATE TABLE t_official (official_name char(20),official_sex char(4),official_office_phone char(10),official_mobile char(20),official_office_name char(40), official_office_addr char(30),official_duty char(20),official_room char(20),official_IP char(20),official_SSO char(32))";
        database.ExecuteSQL(strSql);

	
	ret = tAdo.OpenAdo();
	if (ret != ADOOK)
	{
		return false;
	}
	
	for (int i = 0; i < FLD_MAX; i++)
	{
		outParas[i].Empty();
	}
	
	ret = tAdo.execSelect("select official_name,official_sex,official_office_phone,official_mobile,official_office_name,official_office_addr,official_duty,official_room,official_IP,official_SSO from t_official", &FldCount, outParas);
	//ret = xls.execSelect("select * from [Sheet1$]", &FldCount, outParas);
	if (ret == ADOERR || (FldCount == 0 && outParas[0].IsEmpty()))
	{
		return true;
	}
	do 
	{
		sprintf(szT, "insert into t_official values (\'%s\',\'%s\',\'%s\',\'%s\',\'%s\',\'%s\',\'%s\',\'%s\',\'%s\',\'%s\')", outParas[0], outParas[1],\
			outParas[2], outParas[3], outParas[4], outParas[5], outParas[6], outParas[7], outParas[8], outParas[9]);
		//ret = xls.execUpdateInsert("delete from [Sheet1$]");
		//ret = xls.execUpdateInsert(szT);
		//database.ExecuteSQL("insert into demo (a,b) values('asfe', '42')");
		database.ExecuteSQL(szT);      
	} while(tAdo.getNext(outParas) == ADOOK);
	 database.Close();
	//fclose(fp);
	
	return true;
}

BOOL ImportBlackName(char *pszFileName)
{
	if (_access(pszFileName,0)) //����ͬ���ļ�
	{
		return false;
	}

	char szT[512] = {'\0'};
	long FldCount = 0, ret = 0;;
	CString outParas[FLD_MAX];
	CWinAdo tAdo(sCONNSTR_MYSQL);
	
	CString str = CONNSTR_EXCEL + CString(pszFileName);
	CString outXLS[FLD_MAX];
	CWinAdo xls(str);
	
	/*ret = xls.OpenAdo();
	if (ret != ADOOK)
	{
		return false;
	}*/
	
	ret = tAdo.OpenAdo();
	if (ret != ADOOK)
	{
		return false;
	}
	
	for (int i = 0; i < FLD_MAX; i++)
	{
		outParas[i].Empty();
		outXLS[i].Empty();
	}
	
	CDatabase database;
    CString sDriver = "MICROSOFT EXCEL DRIVER (*.XLS)"; // Excel��װ����
    CString sExcelFile = pszFileName;

	    // �������д�ȡ���ַ���
        CString strSql;
        strSql.Format("DRIVER={%s};DSN='';FIRSTROWHASNAMES=1;READONLY=FALSE;CREATE_DB=\"%s\";DBQ=%s",sDriver, sExcelFile, sExcelFile);
        
        // �������ݿ� (��Excel����ļ�)
        if(!database.OpenEx(strSql,CDatabase::noOdbcDialog))
        {
            printf("Excel����ʧ��\n");
            return false;
        }
	// ��ȡ���е�ֵ������ǰ����������ͣ������д�뵽call price����ĵ�Ԫ����
        CRecordset rs(&database);       
        // ���ö�ȡ�Ĳ�ѯ���
        strSql = "SELECT * FROM [t_blackname$A1:IV65536]";
        
		try
		{
			// ִ�в�ѯ���
			rs.Open(CRecordset::forwardOnly, strSql);
		}
		catch(_com_error &e)
		{
			AfxMessageBox(e.Description());//��ʾ����ԭ��
			database.Close();
			return false;
		}
		catch(...)
		{
			database.Close();
            AfxMessageBox("����޷��򿪣������ʽ�Ƿ���ȷ");
			return false;
		}
        
        // ��ȡ��ѯ�����������
        //CString strValue1, strValue2;
        CDBVariant varValue;
        while (!rs.IsEOF())
        {
            //��ȡExcel�ڲ���ֵ
            rs.GetFieldValue("black_name", outXLS[0]);
            rs.GetFieldValue("black_alias", outXLS[1]);
			rs.GetFieldValue("black_sex", outXLS[2]);
			rs.GetFieldValue("black_folk", outXLS[3]);
            rs.GetFieldValue("black_addr", outXLS[4]);
            //rs.GetFieldValue("black_mobile", outXLS[5]);
			varValue.Clear();
			outXLS[5]= "";
			rs.GetFieldValue("black_mobile", varValue);
			if (varValue.m_dwType==DBVT_DOUBLE)
			{
 				 outXLS[5].Format("%f",varValue.m_dblVal);
				 outXLS[5]=outXLS[5].Left(outXLS[5].Find("."));
                 //outXLS[2].Format("%d",(int)varValue.m_dblVal);
			}
			else if (varValue.m_dwType==DBVT_STRING)
			{
                 outXLS[5]= *varValue.m_pstring;
			}
			else            
			     outXLS[5]= "";

			//rs.GetFieldValue("black_id_type", outXLS[6]);
			varValue.Clear();
			outXLS[6]= "";
			rs.GetFieldValue("black_id_type", varValue);
			if (varValue.m_dwType==DBVT_DOUBLE)
			{
 				 outXLS[6].Format("%f",varValue.m_dblVal);
				 outXLS[6]=outXLS[6].Left(outXLS[6].Find("."));
                 //outXLS[2].Format("%d",(int)varValue.m_dblVal);
			}
			else if (varValue.m_dwType==DBVT_STRING)
			{
                 outXLS[6]= *varValue.m_pstring;
			}
			else            
			     outXLS[6]= "";

			//rs.GetFieldValue("black_id", outXLS[7]);
			varValue.Clear();
			outXLS[7]= "";
			rs.GetFieldValue("black_id", varValue);
			if (varValue.m_dwType==DBVT_DOUBLE)
			{
 				 outXLS[7].Format("%f",varValue.m_dblVal);
				 outXLS[7]=outXLS[7].Left(outXLS[7].Find("."));
                 //outXLS[2].Format("%d",(int)varValue.m_dblVal);
			}
			else if (varValue.m_dwType==DBVT_STRING)
			{
                 outXLS[7]= *varValue.m_pstring;
			}
			else            
			     outXLS[7]= "";

			rs.GetFieldValue("black_unit", outXLS[8]);

			//rs.GetFieldValue("black_carnum", outXLS[9]);
			varValue.Clear();
			outXLS[9]= "";
			rs.GetFieldValue("black_carnum", varValue);
			if (varValue.m_dwType==DBVT_DOUBLE)
			{
 				 outXLS[9].Format("%f",varValue.m_dblVal);
				 outXLS[9]=outXLS[9].Left(outXLS[9].Find("."));
			}
			else if (varValue.m_dwType==DBVT_STRING)
			{
                 outXLS[9]= *varValue.m_pstring;
			}
			else            
			     outXLS[9]= "";

			rs.GetFieldValue("black_user_video_pic", outXLS[10]);
			rs.GetFieldValue("black_user_id_pic", outXLS[11]);
			rs.GetFieldValue("black_user_head_pic", outXLS[12]);
			rs.GetFieldValue("black_notes", outXLS[13]);


            sprintf(szT, "insert t_blackname values (\'%s\', \'%s\', \'%s\', \'%s\', \'%s\', \'%s\', \'%s\', \'%s\', \'%s\', \'%s\', \'%s\', \'%s\', \'%s\', \'%s\')",\
			outXLS[0], outXLS[1], outXLS[2], outXLS[3], outXLS[4], outXLS[5], outXLS[6], outXLS[7], outXLS[8], outXLS[9], outXLS[10], outXLS[11], outXLS[12],outXLS[13]);
		    ret = tAdo.execUpdateInsert(szT);
            // �Ƶ���һ��
            rs.MoveNext();
        }
		rs.Close();
		database.Close();

	/*ret = xls.execSelect("select * from [Sheet1$]", &FldCount, outXLS);
	if (ret == ADOERR || (FldCount == 0 && outXLS[0].IsEmpty()))
	{
		return false;
	}
	do 
	{
		sprintf(szT, "insert t_blackname values (\'%s\', \'%s\', \'%s\', \'%s\', \'%s\', \'%s\', \'%s\', \'%s\', \'%s\', \'%s\', \'%s\', \'%s\', \'%s\')",\
			outXLS[0], outXLS[1], outXLS[2], outXLS[3], outXLS[5], outXLS[5], outXLS[6], outXLS[7], outXLS[8],\
			outXLS[9], outXLS[10], outXLS[11], outXLS[12]);
		ret = tAdo.execUpdateInsert(szT);
	} while(xls.getNext(outXLS) == ADOOK);*/

	
	return true;
}

BOOL ExportBlackName(char *pszFileName)
{
	if (!_access(pszFileName,0)) //����ͬ���ļ�
	{
		return false;
	}

	char szT[512] = {'\0'};
	long FldCount = 0, ret = 0;;
	CString outParas[FLD_MAX];
	CWinAdo tAdo(sCONNSTR_MYSQL);
	
	/*FILE *fp = fopen(pszFileName, "wt");
	if (fp == NULL)
	{
		return false;
	}*/
	// �������д�ȡ���ַ���
	CDatabase database;
    CString sDriver = "MICROSOFT EXCEL DRIVER (*.XLS)"; // Excel��װ����
    CString sExcelFile = pszFileName;

        CString strSql;
        strSql.Format("DRIVER={%s};DSN='';FIRSTROWHASNAMES=1;READONLY=FALSE;CREATE_DB=\"%s\";DBQ=%s",sDriver, sExcelFile, sExcelFile);
        
        // �������ݿ� (��Excel����ļ�)
        if(!database.OpenEx(strSql,CDatabase::noOdbcDialog))
        {
            printf("Excel����ʧ��\n");
            return false;
        }
        strSql = "CREATE TABLE t_blackname (black_name char(20), black_alias char(20), black_sex char(4), black_folk char(10), black_addr char(100),black_mobile char(20),black_id_type char(20),black_id char(40),black_unit char(60),black_carnum char(20),black_user_video_pic char(255),black_user_id_pic char(255),black_user_head_pic char(255),black_notes char(200))"; //
	
        database.ExecuteSQL(strSql);

	
	ret = tAdo.OpenAdo();
	if (ret != ADOOK)
	{
		return false;
	}
	
	for (int i = 0; i < FLD_MAX; i++)
	{
		outParas[i].Empty();
	}
	
	ret = tAdo.execSelect("select * from t_blackname", &FldCount, outParas);
	if (ret == ADOERR || (FldCount == 0 && outParas[0].IsEmpty()))
	{
		//XMessageBox(g_hWindow,L"����Ϣ����",L"��ʾ");
		return true;
	}
	do 
	{
		/*sprintf(szT, "%s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s\r\n", outParas[0], outParas[1],\
			outParas[2], outParas[3], outParas[4], outParas[5], outParas[6], outParas[7], outParas[8],\
			outParas[9], outParas[10], outParas[11], outParas[12]);*/

		sprintf(szT, "insert into t_blackname values (\'%s\', \'%s\', \'%s\', \'%s\', \'%s\', \'%s\', \'%s\',\'%s\',\'%s\', \'%s\', \'%s\', \'%s\', \'%s\', \'%s\')",\
			outParas[0], outParas[1], outParas[2], outParas[3], outParas[5], outParas[5], outParas[6], outParas[7], outParas[8],\
			outParas[9], outParas[10], outParas[11], outParas[12], outParas[13]);
		database.ExecuteSQL(szT);    
		
	} while(tAdo.getNext(outParas) == ADOOK);
	database.Close();
	//fclose(fp);
	
	return true;
}

BOOL ImportWithGoods(char *pszFileName)
{
	if (_access(pszFileName,0)) //����ͬ���ļ�
	{
		return false;
	}

	char szT[512] = {'\0'};
	long FldCount = 0, ret = 0;;
	CString outParas[FLD_MAX];
	CWinAdo tAdo(sCONNSTR_MYSQL);
	
	//CString str = CONNSTR_EXCEL + CString(pszFileName);
	CString outXLS[FLD_MAX];
	//CWinAdo xls(str);
	
	/*ret = xls.OpenAdo();
	if (ret != ADOOK)
	{
		return false;
	}*/
	
	ret = tAdo.OpenAdo();
	if (ret != ADOOK)
	{
		return false;
	}
	
	for (int i = 0; i < FLD_MAX; i++)
	{
		outParas[i].Empty();
		outXLS[i].Empty();
	}
	
	CDatabase database;
    CString sDriver = "MICROSOFT EXCEL DRIVER (*.XLS)"; // Excel��װ����
    CString sExcelFile = pszFileName;

	    // �������д�ȡ���ַ���
        CString strSql;
        strSql.Format("DRIVER={%s};DSN='';FIRSTROWHASNAMES=1;READONLY=FALSE;CREATE_DB=\"%s\";DBQ=%s",sDriver, sExcelFile, sExcelFile);
        
        // �������ݿ� (��Excel����ļ�)
        if(!database.OpenEx(strSql,CDatabase::noOdbcDialog))
        {
            printf("Excel����ʧ��\n");
            return false;
        }
	// ��ȡ���е�ֵ������ǰ����������ͣ������д�뵽call price����ĵ�Ԫ����
        CRecordset rs(&database);       
        // ���ö�ȡ�Ĳ�ѯ���
        strSql = "SELECT * FROM [t_goods$A1:IV65536]";
        
		try
		{
			// ִ�в�ѯ���
			rs.Open(CRecordset::forwardOnly, strSql);
		}
		catch(_com_error &e)
		{
			AfxMessageBox(e.Description());//��ʾ����ԭ��
			database.Close();
			return false;
		}
		catch(...)
		{
			database.Close();
            AfxMessageBox("����޷��򿪣������ʽ�Ƿ���ȷ");
			return false;
		}

        //printf("begin����\n");
        
        // ��ȡ��ѯ�����������
        //CString strValue1, strValue2;
        while (!rs.IsEOF())
        {
            //��ȡExcel�ڲ���ֵ
            rs.GetFieldValue("goods_name", outXLS[0]);           

            sprintf(szT, "insert t_goods values (\'%s\')",outXLS[0]);
		    ret = tAdo.execUpdateInsert(szT);
            // �Ƶ���һ��
            rs.MoveNext();
        }
		rs.Close();
		database.Close();

	
	/*ret = xls.execSelect("select * from [Sheet1$]", &FldCount, outXLS);
	if (ret == ADOERR || (FldCount == 0 && outXLS[0].IsEmpty()))
	{
		return false;
	}
	do 
	{
		sprintf(szT, "insert t_goods values (\'%s\')", outXLS[0]);
		ret = tAdo.execUpdateInsert(szT);
	} while(xls.getNext(outXLS) == ADOOK);*/
	
	return true;
}

BOOL ExportWithGoods(char *pszFileName)
{
	if (!_access(pszFileName,0)) //����ͬ���ļ�
	{
		return false;
	}
	char szT[512] = {'\0'};
	long FldCount = 0, ret = 0;;
	CString outParas[FLD_MAX];
	CWinAdo tAdo(sCONNSTR_MYSQL);
	
	/*FILE *fp = fopen(pszFileName, "wt");
	if (fp == NULL)
	{
		return false;
	}*/
	
	ret = tAdo.OpenAdo();
	if (ret != ADOOK)
	{
		return false;
	}
	
	for (int i = 0; i < FLD_MAX; i++)
	{
		outParas[i].Empty();
	}
	
	CDatabase database;
    CString sDriver = "MICROSOFT EXCEL DRIVER (*.XLS)"; // Excel��װ����
    CString sExcelFile = pszFileName;

	    // �������д�ȡ���ַ���
        CString strSql;
        strSql.Format("DRIVER={%s};DSN='';FIRSTROWHASNAMES=1;READONLY=FALSE;CREATE_DB=\"%s\";DBQ=%s",sDriver, sExcelFile, sExcelFile);
        
        // �������ݿ� (��Excel����ļ�)
        if(!database.OpenEx(strSql,CDatabase::noOdbcDialog))
        {
            printf("Excel����ʧ��\n");
            return false;
        }
        strSql = "CREATE TABLE t_goods (goods_name char(40))";
        database.ExecuteSQL(strSql);
		
	ret = tAdo.execSelect("select * from t_goods", &FldCount, outParas);
	if (ret == ADOERR || (FldCount == 0 && outParas[0].IsEmpty()))
	{
		return true;
	}
	do 
	{
		//sprintf(szT, "%s\r\n", outParas[0]);
		sprintf(szT, "insert into t_goods values (\'%s\')", outParas[0]);
		database.ExecuteSQL(szT);      
	} while(tAdo.getNext(outParas) == ADOOK);
	database.Close();
	//fclose(fp);
	
	return true;
}

BOOL ImportReason(char *pszFileName)
{
	if (_access(pszFileName,0)) //����ͬ���ļ�
	{
		return false;
	}

	char szT[512] = {'\0'};
	long FldCount = 0, ret = 0;;
	CString outParas[FLD_MAX];
	CWinAdo tAdo(sCONNSTR_MYSQL);
	
	//CString str = CONNSTR_EXCEL + CString(pszFileName);
	CString outXLS[FLD_MAX];
	//CWinAdo xls(str);
	
	/*ret = xls.OpenAdo();
	if (ret != ADOOK)
	{
		return false;
	}*/
	ret = tAdo.OpenAdo();
	if (ret != ADOOK)
	{
		return false;
	}
	
	for (int i = 0; i < FLD_MAX; i++)
	{
		outParas[i].Empty();
		outXLS[i].Empty();
	}
	
	CDatabase database;
    CString sDriver = "MICROSOFT EXCEL DRIVER (*.XLS)"; // Excel��װ����
    CString sExcelFile = pszFileName;

	    // �������д�ȡ���ַ���
        CString strSql;
        strSql.Format("DRIVER={%s};DSN='';FIRSTROWHASNAMES=1;READONLY=FALSE;CREATE_DB=\"%s\";DBQ=%s",sDriver, sExcelFile, sExcelFile);
        
        // �������ݿ� (��Excel����ļ�)
        if(!database.OpenEx(strSql,CDatabase::noOdbcDialog))
        {
            printf("Excel����ʧ��\n");
            return false;
        }
	// ��ȡ���е�ֵ������ǰ����������ͣ������д�뵽call price����ĵ�Ԫ����
        CRecordset rs(&database);       
        // ���ö�ȡ�Ĳ�ѯ���
        strSql = "SELECT * FROM [t_reason$A1:IV65536]";
        
		try
		{
			// ִ�в�ѯ���
			rs.Open(CRecordset::forwardOnly, strSql);
		}
		catch(_com_error &e)
		{
			AfxMessageBox(e.Description());//��ʾ����ԭ��
			database.Close();
			return false;
		}
		catch(...)
		{
			database.Close();
            AfxMessageBox("����޷��򿪣������ʽ�Ƿ���ȷ");
			return false;
		}

        //printf("begin����\n");
        
        // ��ȡ��ѯ�����������
        //CString strValue1, strValue2;
        while (!rs.IsEOF())
        {
            //��ȡExcel�ڲ���ֵ
            rs.GetFieldValue("reason_name", outXLS[0]);           

            sprintf(szT, "insert t_reason values (\'%s\')",outXLS[0]);
		    ret = tAdo.execUpdateInsert(szT);
            // �Ƶ���һ��
            rs.MoveNext();
        }
		rs.Close();
		database.Close();
		

	
	/*ret = xls.execSelect("select * from [Sheet1$]", &FldCount, outXLS);
	if (ret == ADOERR || (FldCount == 0 && outXLS[0].IsEmpty()))
	{
		return false;
	}
	do 
	{
		sprintf(szT, "insert t_reason values (\'%s\')", outXLS[0]);
		ret = tAdo.execUpdateInsert(szT);
	} while(xls.getNext(outXLS) == ADOOK);*/
	
	return true;
}

BOOL ExportReason(char *pszFileName)
{
	if (!_access(pszFileName,0)) //����ͬ���ļ�
	{
		return false;
	}
	char szT[512] = {'\0'};
	long FldCount = 0, ret = 0;;
	CString outParas[FLD_MAX];
	CWinAdo tAdo(sCONNSTR_MYSQL);
	
	/*FILE *fp = fopen(pszFileName, "wt");
	if (fp == NULL)
	{
		return false;
	}*/
	
	ret = tAdo.OpenAdo();
	if (ret != ADOOK)
	{
		return false;
	}

	CDatabase database;
    CString sDriver = "MICROSOFT EXCEL DRIVER (*.XLS)"; // Excel��װ����
    CString sExcelFile = pszFileName;

	    // �������д�ȡ���ַ���
        CString strSql;
        strSql.Format("DRIVER={%s};DSN='';FIRSTROWHASNAMES=1;READONLY=FALSE;CREATE_DB=\"%s\";DBQ=%s",sDriver, sExcelFile, sExcelFile);
        
        // �������ݿ� (��Excel����ļ�)
        if(!database.OpenEx(strSql,CDatabase::noOdbcDialog))
        {
            printf("Excel����ʧ��\n");
            return false;
        }
        strSql = "CREATE TABLE t_reason (reason_name char(40))";
        database.ExecuteSQL(strSql);
		
	ret = tAdo.execSelect("select * from t_reason", &FldCount, outParas);
	if (ret == ADOERR || (FldCount == 0 && outParas[0].IsEmpty()))
	{
		return true;
	}
	do 
	{
		//sprintf(szT, "%s\r\n", outParas[0]);
		sprintf(szT, "insert into t_reason values (\'%s\')", outParas[0]);
		database.ExecuteSQL(szT);      
	} while(tAdo.getNext(outParas) == ADOOK);
	database.Close();
	
	
	/*for (int i = 0; i < FLD_MAX; i++)
	{
		outParas[i].Empty();
	}
	
	ret = tAdo.execSelect("select * from t_reason", &FldCount, outParas);
	if (ret == ADOERR || (FldCount == 0 && outParas[0].IsEmpty()))
	{
		return false;
	}
	do 
	{
		fprintf(fp, "%s\r\n", outParas[0]);
	} while(tAdo.getNext(outParas) == ADOOK);
	
	fclose(fp);*/
	
	return true;
}

BOOL ImportStation(char *pszFileName)
{
	if (_access(pszFileName,0)) //����ͬ���ļ�
	{
		return false;
	}

	char szT[512] = {'\0'};
	long FldCount = 0, ret = 0;;
	CString outParas[FLD_MAX];
	CWinAdo tAdo(sCONNSTR_MYSQL);
	
	//CString str = CONNSTR_EXCEL + CString(pszFileName);
	CString outXLS[FLD_MAX];
	//CWinAdo xls(str);
	
	/*ret = xls.OpenAdo();
	if (ret != ADOOK)
	{
		return false;
	}*/
	
	ret = tAdo.OpenAdo();
	if (ret != ADOOK)
	{
		return false;
	}
	
	for (int i = 0; i < FLD_MAX; i++)
	{
		outParas[i].Empty();
		outXLS[i].Empty();
	}
	
	CDatabase database;
    CString sDriver = "MICROSOFT EXCEL DRIVER (*.XLS)"; // Excel��װ����
    CString sExcelFile = pszFileName;

	    // �������д�ȡ���ַ���
        CString strSql;
        strSql.Format("DRIVER={%s};DSN='';FIRSTROWHASNAMES=1;READONLY=FALSE;CREATE_DB=\"%s\";DBQ=%s",sDriver, sExcelFile, sExcelFile);
        
        // �������ݿ� (��Excel����ļ�)
        if(!database.OpenEx(strSql,CDatabase::noOdbcDialog))
        {
            printf("Excel����ʧ��\n");
            return false;
        }
	// ��ȡ���е�ֵ������ǰ����������ͣ������д�뵽call price����ĵ�Ԫ����
        CRecordset rs(&database);       
        // ���ö�ȡ�Ĳ�ѯ���
        strSql = "SELECT * FROM [t_station$A1:IV65536]";
        
		try
		{
			// ִ�в�ѯ���
			rs.Open(CRecordset::forwardOnly, strSql);
		}
		catch(_com_error &e)
		{
			AfxMessageBox(e.Description());//��ʾ����ԭ��
			database.Close();
			return false;
		}
		catch(...)
		{
			database.Close();
            AfxMessageBox("����޷��򿪣������ʽ�Ƿ���ȷ");
			return false;
		}

        //printf("begin����\n");
        
        // ��ȡ��ѯ�����������
        //CString strValue1, strValue2;
        while (!rs.IsEOF())
        {
            //��ȡExcel�ڲ���ֵ
            rs.GetFieldValue("station_name", outXLS[0]);           

            sprintf(szT, "insert t_station values (\'%s\')",outXLS[0]);
		    ret = tAdo.execUpdateInsert(szT);
            // �Ƶ���һ��
            rs.MoveNext();
        }
		rs.Close();
		database.Close();
		
	/*ret = xls.execSelect("select * from [Sheet1$]", &FldCount, outXLS);
	if (ret == ADOERR || (FldCount == 0 && outXLS[0].IsEmpty()))
	{
		return false;
	}
	do 
	{
		sprintf(szT, "insert t_station values (\'%s\')", outXLS[0]);
		ret = tAdo.execUpdateInsert(szT);
	} while(xls.getNext(outXLS) == ADOOK);	*/
	return true;
	
}

BOOL ExportStation(char *pszFileName)
{
	if (!_access(pszFileName,0)) //����ͬ���ļ�
	{
		return false;
	}
	char szT[512] = {'\0'};
	long FldCount = 0, ret = 0;;
	CString outParas[FLD_MAX];
	CWinAdo tAdo(sCONNSTR_MYSQL);
	
	/*FILE *fp = fopen(pszFileName, "wt");
	if (fp == NULL)
	{
		return false;
	}*/
	
	ret = tAdo.OpenAdo();
	if (ret != ADOOK)
	{
		return false;
	}
	
	for (int i = 0; i < FLD_MAX; i++)
	{
		outParas[i].Empty();
	}
	
	CDatabase database;
    CString sDriver = "MICROSOFT EXCEL DRIVER (*.XLS)"; // Excel��װ����
    CString sExcelFile = pszFileName;

	    // �������д�ȡ���ַ���
        CString strSql;
        strSql.Format("DRIVER={%s};DSN='';FIRSTROWHASNAMES=1;READONLY=FALSE;CREATE_DB=\"%s\";DBQ=%s",sDriver, sExcelFile, sExcelFile);
        
        // �������ݿ� (��Excel����ļ�)
        if(!database.OpenEx(strSql,CDatabase::noOdbcDialog))
        {
            printf("Excel����ʧ��\n");
            return false;
        }
        strSql = "CREATE TABLE t_station (station_name char(40))";
        database.ExecuteSQL(strSql);
		
	ret = tAdo.execSelect("select * from t_station", &FldCount, outParas);
	if (ret == ADOERR || (FldCount == 0 && outParas[0].IsEmpty()))
	{
		return true;
	}
	do 
	{
		//sprintf(szT, "%s\r\n", outParas[0]);
		sprintf(szT, "insert into t_station values (\'%s\')", outParas[0]);
		database.ExecuteSQL(szT);      
	} while(tAdo.getNext(outParas) == ADOOK);
	database.Close();	
	return true;
}

BOOL ImportOffice(char *pszFileName)
{
	if (_access(pszFileName,0)) //����ͬ���ļ�
	{
		return false;
	}

	char szT[512] = {'\0'};
	long FldCount = 0, ret = 0;;
	CString outParas[FLD_MAX];
	CWinAdo tAdo(sCONNSTR_MYSQL);
	
	CString str = CONNSTR_EXCEL + CString(pszFileName);
	CString outXLS[FLD_MAX];
	CWinAdo xls(str);
	
	/*ret = xls.OpenAdo();
	if (ret != ADOOK)
	{
		return false;
	}*/
	
	ret = tAdo.OpenAdo();
	if (ret != ADOOK)
	{
		return false;
	}
	
	for (int i = 0; i < FLD_MAX; i++)
	{
		outParas[i].Empty();
		outXLS[i].Empty();
	}
	
	CDatabase database;
    CString sDriver = "MICROSOFT EXCEL DRIVER (*.XLS)"; // Excel��װ����
    CString sExcelFile = pszFileName;

	    // �������д�ȡ���ַ���
        CString strSql;
        strSql.Format("DRIVER={%s};DSN='';FIRSTROWHASNAMES=1;READONLY=FALSE;CREATE_DB=\"%s\";DBQ=%s",sDriver, sExcelFile, sExcelFile);
        
        // �������ݿ� (��Excel����ļ�)
        if(!database.OpenEx(strSql,CDatabase::noOdbcDialog))
        {
            printf("Excel����ʧ��\n");
            return false;
        }
	// ��ȡ���е�ֵ������ǰ����������ͣ������д�뵽call price����ĵ�Ԫ����
        CRecordset rs(&database);       
        // ���ö�ȡ�Ĳ�ѯ���
        strSql = "SELECT * FROM [t_office$A1:IV65536]";
        
        // ִ�в�ѯ���
        rs.Open(CRecordset::forwardOnly, strSql);

        //printf("begin����\n");
        
        // ��ȡ��ѯ�����������
        //CString strValue1, strValue2;
        float sum = 0.0f;
        while (!rs.IsEOF())
        {
            //��ȡExcel�ڲ���ֵ
            rs.GetFieldValue("office_name", outXLS[0]);  		
			rs.GetFieldValue("office_phone", outXLS[1]); 
			rs.GetFieldValue("office_addr", outXLS[2]); 
			rs.GetFieldValue("office_roomnum", outXLS[3]); 

            sprintf(szT, "insert t_office values (\'%s\', \'%s\', \'%s\', \'%s\')",outXLS[0], outXLS[1], outXLS[2], outXLS[3]);
		    ret = tAdo.execUpdateInsert(szT);
            // �Ƶ���һ��
            rs.MoveNext();
        }
		rs.Close();
		database.Close();

	/*ret = xls.execSelect("select * from [Sheet1$]", &FldCount, outXLS);
	if (ret == ADOERR || (FldCount == 0 && outXLS[0].IsEmpty()))
	{
		return false;
	}
	do 
	{
		sprintf(szT, "insert t_office values (\'%s\', \'%s\', \'%s\', \'%s\')", outXLS[0], outXLS[1], outXLS[2], outXLS[3]);
		ret = tAdo.execUpdateInsert(szT);
	} while(xls.getNext(outXLS) == ADOOK);*/
	
	return true;
}

BOOL ExportOffice(char *pszFileName)
{
	if (!_access(pszFileName,0)) //����ͬ���ļ�
	{
		return false;
	}
	char szT[512] = {'\0'};
	long FldCount = 0, ret = 0;;
	CString outParas[FLD_MAX];
	CWinAdo tAdo(sCONNSTR_MYSQL);
	
	/*FILE *fp = fopen(pszFileName, "wt");
	if (fp == NULL)
	{
		return false;
	}*/
	
	ret = tAdo.OpenAdo();
	if (ret != ADOOK)
	{
		return false;
	}
	
	for (int i = 0; i < FLD_MAX; i++)
	{
		outParas[i].Empty();
	}
	
	CDatabase database;
    CString sDriver = "MICROSOFT EXCEL DRIVER (*.XLS)"; // Excel��װ����
    CString sExcelFile = pszFileName;

	    // �������д�ȡ���ַ���
        CString strSql;
        strSql.Format("DRIVER={%s};DSN='';FIRSTROWHASNAMES=1;READONLY=FALSE;CREATE_DB=\"%s\";DBQ=%s",sDriver, sExcelFile, sExcelFile);
        
        // �������ݿ� (��Excel����ļ�)
        if(!database.OpenEx(strSql,CDatabase::noOdbcDialog))
        {
            printf("Excel����ʧ��\n");
            return false;
        }
        strSql = "CREATE TABLE t_office(office_name char(40),office_phone char(16),office_addr char(40),office_roomnum char(10))";
        database.ExecuteSQL(strSql);
		
	ret = tAdo.execSelect("select * from t_office", &FldCount, outParas);
	if (ret == ADOERR || (FldCount == 0 && outParas[0].IsEmpty()))
	{
		return true;
	}
	do 
	{
		//sprintf(szT, "%s\r\n", outParas[0]);
		sprintf(szT, "insert into t_office values (\'%s\', \'%s\', \'%s\', \'%s\')", outParas[0], outParas[1], outParas[2], outParas[3]);
		database.ExecuteSQL(szT);      
	} while(tAdo.getNext(outParas) == ADOOK);
	database.Close();	

	/*ret = tAdo.execSelect("select * from t_office", &FldCount, outParas);
	if (ret == ADOERR || (FldCount == 0 && outParas[0].IsEmpty()))
	{
		return false;
	}
	do 
	{
		fprintf(fp, "%s, %s, %s, %s\r\n", outParas[0], outParas[1], outParas[2], outParas[3]);
	} while(tAdo.getNext(outParas) == ADOOK);

	fclose(fp);*/
	
	return true;
}

BOOL ImportUser(char *pszFileName)
{
	if (_access(pszFileName,0)) //����ͬ���ļ�
	{
		return false;
	}

	char szT[512] = {'\0'};
	long FldCount = 0, ret = 0;;
	CString outParas[FLD_MAX];
	CWinAdo tAdo(sCONNSTR_MYSQL);
	
	CString str = CONNSTR_EXCEL + CString(pszFileName);
	CString outXLS[FLD_MAX];
	CWinAdo xls(str);
	
	/*ret = xls.OpenAdo();
	if (ret != ADOOK)
	{
		return false;
	}*/
	
	ret = tAdo.OpenAdo();
	if (ret != ADOOK)
	{
		return false;
	}
	
	for (int i = 0; i < FLD_MAX; i++)
	{
		outParas[i].Empty();
		outXLS[i].Empty();
	}
	
	CDatabase database;
    CString sDriver = "MICROSOFT EXCEL DRIVER (*.XLS)"; // Excel��װ����
    CString sExcelFile = pszFileName;

	    // �������д�ȡ���ַ���
        CString strSql;
        strSql.Format("DRIVER={%s};DSN='';FIRSTROWHASNAMES=1;READONLY=FALSE;CREATE_DB=\"%s\";DBQ=%s",sDriver, sExcelFile, sExcelFile);
        
        // �������ݿ� (��Excel����ļ�)
        if(!database.OpenEx(strSql,CDatabase::noOdbcDialog))
        {
            printf("Excel����ʧ��\n");
            return false;
        }
	// ��ȡ���е�ֵ������ǰ����������ͣ������д�뵽call price����ĵ�Ԫ����
        CRecordset rs(&database);       
        // ���ö�ȡ�Ĳ�ѯ���
        strSql = "SELECT * FROM [t_user$A1:IV65536]";
        
		try
		{
			// ִ�в�ѯ���
			rs.Open(CRecordset::forwardOnly, strSql);
		}
		catch(_com_error &e)
		{
			AfxMessageBox(e.Description());//��ʾ����ԭ��
			database.Close();
			return false;
		}
		catch(...)
		{
			database.Close();
            AfxMessageBox("����޷��򿪣������ʽ�Ƿ���ȷ");
			return false;
		}

        //printf("begin����\n");
        
        // ��ȡ��ѯ�����������
        //CString strValue1, strValue2;
        while (!rs.IsEOF())
        {
            //��ȡExcel�ڲ���ֵ
            rs.GetFieldValue("user_id", outXLS[0]);  		
			rs.GetFieldValue("user_pwd", outXLS[1]); 
			rs.GetFieldValue("user_right_id", outXLS[2]); 

            sprintf(szT, "insert t_user values (\'%s\', \'%s\', \'%s\')",outXLS[0], outXLS[1], outXLS[2]);
		    ret = tAdo.execUpdateInsert(szT);
            // �Ƶ���һ��
            rs.MoveNext();
        }
		rs.Close();
		database.Close();
	
	return true;
}

BOOL ExportVisitor(char *pszFileName)
{
	//����xls��szpathDir
	if (!_access(pszFileName,0)) //����ͬ���ļ�
	{
		return false;
	}
	char szT[1024] = {'\0'};
	long FldCount = 0, ret = 0;;
	CString outParas[FLD_MAX];
	CWinAdo tAdo(sCONNSTR_MYSQL);

	/*long FldCount = 0, ret = 0;
	CString outParas[FLD_MAX], outP1[FLD_MAX];
	CWinAdo tAdo(sCONNSTR_MYSQL),tAdo1(sCONNSTR_MYSQL);*/
	ret = tAdo.OpenAdo();
	if (ret != ADOOK)
	{
		return false;
	}
	/*ret = tAdo1.OpenAdo();
	if (ret != ADOOK)
	{
		return false;
	}*/
	
	for (int i = 0; i < FLD_MAX; i++)
	{
		outParas[i].Empty();
		//outP1[i].Empty();
	}
	
	CDatabase database;
    CString sDriver = "MICROSOFT EXCEL DRIVER (*.XLS)"; // Excel��װ����
    CString sExcelFile = pszFileName;
	
	// �������д�ȡ���ַ���
	CString strSql;
	strSql.Format("DRIVER={%s};DSN='';FIRSTROWHASNAMES=1;READONLY=FALSE;CREATE_DB=\"%s\";DBQ=%s",sDriver, sExcelFile, sExcelFile);
	
	// �������ݿ� (��Excel����ļ�)
	if(!database.OpenEx(strSql,CDatabase::noOdbcDialog))
	{
		printf("Excel����ʧ��\n");
		return false;
	}
	bool bHaveStarted=false;
	strSql = "CREATE TABLE ���ñ��� (";
	if (g_VisitorExp.bStationExp)
	{
		bHaveStarted=true;
		strSql+="�Ÿ� char(10)";
	}
	if (g_VisitorExp.bVisitorExp)
	{
		if (bHaveStarted)
		{
			strSql+=",���� char(20)";
		}
		else
		{
			bHaveStarted=true;
			strSql+="���� char(20)";
		}
		
	}
	if (g_VisitorExp.bVSexExp)
	{
		if (bHaveStarted)
		{
			strSql+=",�Ա� char(6)";
		}
		else
		{
			bHaveStarted=true;
			strSql+="�Ա� char(6)";
		}
	}
	if (g_VisitorExp.bVFolkExp)
	{
		if (bHaveStarted)
		{
			strSql+=",���� char(20)";
		}
		else
		{
			bHaveStarted=true;
			strSql+="���� char(20)";
		}
	}
	if (g_VisitorExp.bIDTypeExp)
	{
		if (bHaveStarted)
		{
			strSql+=",֤������ char(20)";
		}
		else
		{
			bHaveStarted=true;
			strSql+="֤������ char(20)";
		}
	}
	if (g_VisitorExp.bIDExp)
	{
		if (bHaveStarted)
		{
			strSql+=",֤������ char(40)";
		}
		else
		{
			bHaveStarted=true;
			strSql+="֤������ char(40)";
		}
	}
	if (g_VisitorExp.bAddrExp)
	{
		if (bHaveStarted)
		{
			strSql+=",סַ char(120)";
		}
		else
		{
			bHaveStarted=true;
			strSql+="סַ char(120)";
		}
	}
	if (g_VisitorExp.bSigndepartmentExp)
	{
		if (bHaveStarted)
		{
			strSql+=",��֤���� char(50)";
		}
		else
		{
			bHaveStarted=true;
			strSql+="��֤���� char(50)";
		}
	}
	if (g_VisitorExp.bUnitExp)
	{
		if (bHaveStarted)
		{
			strSql+=",�ÿ͵�λ char(100)";
		}
		else
		{
			bHaveStarted=true;
			strSql+="�ÿ͵�λ char(100)";
		}
	}
	if (g_VisitorExp.bTelExp)
	{
		if (bHaveStarted)
		{
			strSql+=",�绰 char(40)";
		}
		else
		{
			bHaveStarted=true;
			strSql+="�绰 char(40)";
		}
	}
	if (g_VisitorExp.bCarNumExp)
	{
		if (bHaveStarted)
		{
			strSql+=",���ƺ��� char(20)";
		}
		else
		{
			bHaveStarted=true;
			strSql+="���ƺ��� char(20)";
		}
	}
	if (g_VisitorExp.bVisittimeExp)
	{
		if (bHaveStarted)
		{
			strSql+=",����ʱ�� char(40)";
		}
		else
		{
			bHaveStarted=true;
			strSql+="����ʱ�� char(40)";
		}
	}
	if (g_VisitorExp.bReasonExp)
	{
		if (bHaveStarted)
		{
			strSql+=",�������� char(40)";
		}
		else
		{
			bHaveStarted=true;
			strSql+="�������� char(40)";
		}
	}
	if (g_VisitorExp.bGoodsExp)
	{
		if (bHaveStarted)
		{
			strSql+=",Я��Ʒ char(40)";
		}
		else
		{
			bHaveStarted=true;
			strSql+="Я��Ʒ char(40)";
		}
	}
	if (g_VisitorExp.bLefttimeExp)
	{
		if (bHaveStarted)
		{
			strSql+=",�뿪ʱ�� char(40)";
		}
		else
		{
			bHaveStarted=true;
			strSql+="�뿪ʱ�� char(40)";
		}
	}
	if (g_VisitorExp.bTotalPeopleExp)
	{
		if (bHaveStarted)
		{
			strSql+=",�������� char(20)";
		}
		else
		{
			bHaveStarted=true;
			strSql+="�������� char(20)";
		}
	}
	if (g_VisitorExp.bOfficerExp)
	{
		if (bHaveStarted)
		{
			strSql+=",������ char(32)";
		}
		else
		{
			bHaveStarted=true;
			strSql+="������ char(32)";
		}
	}
	if (g_VisitorExp.bOfficeNameExp)
	{
		if (bHaveStarted)
		{
			strSql+=",��ȥ���� char(100)";
		}
		else
		{
			bHaveStarted=true;
			strSql+="��ȥ���� char(100)";
		}
	}
	if (g_VisitorExp.bOfficePhoneExp)
	{
		if (bHaveStarted)
		{
			strSql+=",���õ绰 char(20)";
		}
		else
		{
			bHaveStarted=true;
			strSql+="���õ绰 char(20)";
		}
	}

    if(strSql == "CREATE TABLE ���ñ��� (")
		return false;

	if (!bHaveStarted)
        return false;

	strSql+= ")";

	database.ExecuteSQL(strSql);

	HELE hList = GETELE(IDL_LISTCHAXUNTONGJI);
	char szName[32] = {'\0'}, szSex[6] = {'\0'} ,szFolk[20] = {'\0'},szIDType[20] = {'\0'},szID[40] = {'\0'}, szAddr[120] = {'\0'}, szUnit[100] = {'\0'},szTel[40] = {'\0'},szVisitTime[32] = {'\0'};
	char szGovrment[50] = {'\0'},szCarNum[20] = {'\0'},szStation[10] = {'\0'},szReason[40] = {'\0'},szGoods[40] = {'\0'},szLeftTime[24] = {'\0'}, szPeoples[10] = {'\0'},szOfficialName[32] = {'\0'};
	char szOffice[32] = {'\0'},szOfficePhone[32]={'\0'};
	wchar_t wBuf[256] = {0};
    CString sInsertValues;
	HELE hE = GETELE(IDL_LISTCHAXUNTONGJI);
	wcscpy(wBuf, XList_GetHeaderItemText(hE,0));
	if (wcscmp(wBuf,L"��ʾ")==0)
	{
		return false;
	}
	wcscpy(wBuf, XList_GetHeaderItemText(hE,1));	
    W_2_S(wBuf, szName, 32);
	if (strcmp(szName,"�Ÿ�")!=0)
	{
		return false;
	}

	int nCount = XList_GetItemCount(hList);		//�ܼ�¼��
	for (int j = 0; j < nCount; j++)
	{
		memset((char *)wBuf, 0, sizeof(wBuf));
		wcscpy(wBuf, XList_GetItemText(hList, j, 1));
		W_2_S(wBuf, szStation, 128);
		
		memset((char *)wBuf, 0, sizeof(wBuf));
		wcscpy(wBuf, XList_GetItemText(hList, j, 2));
		W_2_S(wBuf, szName, 128);
		
		memset((char *)wBuf, 0, sizeof(wBuf));
		wcscpy(wBuf, XList_GetItemText(hList, j, 3));
		W_2_S(wBuf, szSex, 128);

		memset((char *)wBuf, 0, sizeof(wBuf));
		wcscpy(wBuf, XList_GetItemText(hList, j, 4));
		W_2_S(wBuf, szFolk, 128);

				memset((char *)wBuf, 0, sizeof(wBuf));
		wcscpy(wBuf, XList_GetItemText(hList, j, 5));
		W_2_S(wBuf, szIDType, 128);

		switch (atoi(szIDType))
		{
		case 1:
			strcpy(szIDType,"�������֤");
			break;
		case 2:
			strcpy(szIDType,"һ�����֤");
			break;
		case 3:
            strcpy(szIDType,"����");
			break;
		case 4:
			strcpy(szIDType,"��½����۰�ͨ��֤");
			break;
		case 5:
			strcpy(szIDType,"�������֤");
			break;
		case 6:
			strcpy(szIDType,"̨��֤");
			break;
		case 7:
			strcpy(szIDType,"��ʻ֤");
			break;
		case 8:
			strcpy(szIDType,"��ʦ֤");
			break;
		case 9:
			strcpy(szIDType, "��Ƭ");
			break;
		default:
			strcpy(szIDType,"����֤��");
			break;
		}

				memset((char *)wBuf, 0, sizeof(wBuf));
		wcscpy(wBuf, XList_GetItemText(hList, j, 6));
		W_2_S(wBuf, szID, 128);

				memset((char *)wBuf, 0, sizeof(wBuf));
		wcscpy(wBuf, XList_GetItemText(hList, j, 7));
		W_2_S(wBuf, szAddr, 128);

				memset((char *)wBuf, 0, sizeof(wBuf));
		wcscpy(wBuf, XList_GetItemText(hList, j, 8));
		W_2_S(wBuf, szGovrment, 128);

				memset((char *)wBuf, 0, sizeof(wBuf));
		wcscpy(wBuf, XList_GetItemText(hList, j, 9));
		W_2_S(wBuf, szUnit, 128);

				memset((char *)wBuf, 0, sizeof(wBuf));
		wcscpy(wBuf, XList_GetItemText(hList, j, 10));
		W_2_S(wBuf, szTel, 128);
	
		memset((char *)wBuf, 0, sizeof(wBuf));
		wcscpy(wBuf, XList_GetItemText(hList, j, 11));
		W_2_S(wBuf, szCarNum, 128);

		memset((char *)wBuf, 0, sizeof(wBuf));
		wcscpy(wBuf, XList_GetItemText(hList, j, 12));
		W_2_S(wBuf, szVisitTime, 128);

		memset((char *)wBuf, 0, sizeof(wBuf));
		wcscpy(wBuf, XList_GetItemText(hList, j, 13));
		W_2_S(wBuf, szReason, 128);

		memset((char *)wBuf, 0, sizeof(wBuf));
		wcscpy(wBuf, XList_GetItemText(hList, j, 14));
		W_2_S(wBuf, szGoods, 128);

		memset((char *)wBuf, 0, sizeof(wBuf));
		wcscpy(wBuf, XList_GetItemText(hList, j, 15));
		W_2_S(wBuf, szLeftTime, 128);

		memset((char *)wBuf, 0, sizeof(wBuf));
		wcscpy(wBuf, XList_GetItemText(hList, j, 16));
		W_2_S(wBuf, szPeoples, 128);

		memset((char *)wBuf, 0, sizeof(wBuf));
		wcscpy(wBuf, XList_GetItemText(hList, j, 17));
		W_2_S(wBuf, szOfficialName, 128);

		memset((char *)wBuf, 0, sizeof(wBuf));
		wcscpy(wBuf, XList_GetItemText(hList, j, 18));
		W_2_S(wBuf, szOffice, 128);

		memset((char *)wBuf, 0, sizeof(wBuf));
		wcscpy(wBuf, XList_GetItemText(hList, j, 19));
		W_2_S(wBuf, szOfficePhone, 128);
		

        strSql="insert into ���ñ��� (";
		sInsertValues="values (";
        bHaveStarted=false;
		if (g_VisitorExp.bStationExp)
		{
		      bHaveStarted=true;
		      strSql+="�Ÿ�";
			  sInsertValues+="'";
			  sInsertValues+=szStation;
			  sInsertValues+="'";

		}

		if (g_VisitorExp.bVisitorExp)
		{
			if (bHaveStarted)
			{
				strSql+=",����";
                sInsertValues+=",";
			}
			else
			{
				bHaveStarted=true;
				strSql+="����";
			}
			  sInsertValues+="'";
			  sInsertValues+=szName;
			  sInsertValues+="'";
		}
		if (g_VisitorExp.bVSexExp)
		{
			if (bHaveStarted)
			{
				strSql+=",�Ա�";
				sInsertValues+=",";
			}
			else
			{
				bHaveStarted=true;
				strSql+="�Ա�";
			}
			  sInsertValues+="'";
			  sInsertValues+=szSex;
			  sInsertValues+="'";
		}		
		if (g_VisitorExp.bVFolkExp)
		{
			if (bHaveStarted)
			{
				strSql+=",����";
				sInsertValues+=",";
			}
			else
			{
				bHaveStarted=true;
				strSql+="����";
			}
			  sInsertValues+="'";
			  sInsertValues+=szFolk;
			  sInsertValues+="'";
		}
		if (g_VisitorExp.bIDTypeExp)
		{
			if (bHaveStarted)
			{
				strSql+=",֤������";
				sInsertValues+=",";
			}
			else
			{
				bHaveStarted=true;
				strSql+="֤������";
			}
			  sInsertValues+="'";
			  sInsertValues+=szIDType;
			  sInsertValues+="'";
		}
		if (g_VisitorExp.bIDExp)
		{
			if (bHaveStarted)
			{
				strSql+=",֤������";
				sInsertValues+=",";
			}
			else
			{
				bHaveStarted=true;
				strSql+="֤������";
			}
			  sInsertValues+="'";
			  sInsertValues+=szID;
			  sInsertValues+="'";
		}
		if (g_VisitorExp.bAddrExp)
		{
			if (bHaveStarted)
			{
				strSql+=",סַ";
				sInsertValues+=",";
			}
			else
			{
				bHaveStarted=true;
				strSql+="סַ";
			}
			  sInsertValues+="'";
			  sInsertValues+=szAddr;
			  sInsertValues+="'";
		}
		if (g_VisitorExp.bSigndepartmentExp)
		{
			if (bHaveStarted)
			{
				strSql+=",��֤����";
				sInsertValues+=",";
			}
			else
			{
				bHaveStarted=true;
				strSql+="��֤����";
			}
			  sInsertValues+="'";
			  sInsertValues+=szGovrment;
			  sInsertValues+="'";
		}
		if (g_VisitorExp.bUnitExp)
		{
			if (bHaveStarted)
			{
				strSql+=",�ÿ͵�λ";
				sInsertValues+=",";
			}
			else
			{
				bHaveStarted=true;
				strSql+="�ÿ͵�λ";
			}
			  sInsertValues+="'";
			  sInsertValues+=szUnit;
			  sInsertValues+="'";
		}
		if (g_VisitorExp.bTelExp)
		{
			if (bHaveStarted)
			{
				strSql+=",�绰";
				sInsertValues+=",";
			}
			else
			{
				bHaveStarted=true;
				strSql+="�绰";
			}
			  sInsertValues+="'";
			  sInsertValues+=szTel;
			  sInsertValues+="'";
		}
		if (g_VisitorExp.bCarNumExp)
		{
			if (bHaveStarted)
			{
				strSql+=",���ƺ���";
				sInsertValues+=",";
			}
			else
			{
				bHaveStarted=true;
				strSql+="���ƺ���";
			}
			  sInsertValues+="'";
			  sInsertValues+=szCarNum;
			  sInsertValues+="'";
		}
		if (g_VisitorExp.bVisittimeExp)
		{
			if (bHaveStarted)
			{
				strSql+=",����ʱ��";
				sInsertValues+=",";
			}
			else
			{
				bHaveStarted=true;
				strSql+="����ʱ��";
			}
			  sInsertValues+="'";
			  sInsertValues+=szVisitTime;
			  sInsertValues+="'";
		}
		if (g_VisitorExp.bReasonExp)
		{
			if (bHaveStarted)
			{
				strSql+=",��������";
				sInsertValues+=",";
			}
			else
			{
				bHaveStarted=true;
				strSql+="��������";
			}
			  sInsertValues+="'";
			  sInsertValues+=szReason;
			  sInsertValues+="'";
		}
		if (g_VisitorExp.bGoodsExp)
		{
			if (bHaveStarted)
			{
				strSql+=",Я��Ʒ";
				sInsertValues+=",";
			}
			else
			{
				bHaveStarted=true;
				strSql+="Я��Ʒ";
			}
			  sInsertValues+="'";
			  sInsertValues+=szGoods;
			  sInsertValues+="'";
		}
		if (g_VisitorExp.bLefttimeExp)
		{
			if (bHaveStarted)
			{
				strSql+=",�뿪ʱ��";
				sInsertValues+=",";
			}
			else
			{
				bHaveStarted=true;
				strSql+="�뿪ʱ��";
			}
			  sInsertValues+="'";
			  sInsertValues+=szLeftTime;
			  sInsertValues+="'";
		}
		if (g_VisitorExp.bTotalPeopleExp)
		{
			if (bHaveStarted)
			{
				strSql+=",��������";
				sInsertValues+=",";
			}
			else
			{
				bHaveStarted=true;
				strSql+="��������";
			}
			  sInsertValues+="'";
			  sInsertValues+=szPeoples;
			  sInsertValues+="'";
		}
		if (g_VisitorExp.bOfficerExp)
		{
			if (bHaveStarted)
			{
				strSql+=",������";
				sInsertValues+=",";
			}
			else
			{
				bHaveStarted=true;
				strSql+="������";
			}
			  sInsertValues+="'";
			  sInsertValues+=szOfficialName;
			  sInsertValues+="'";
		}
		if (g_VisitorExp.bOfficeNameExp)
		{
			if (bHaveStarted)
			{
				strSql+=",��ȥ����";
				sInsertValues+=",";
			}
			else
			{
				bHaveStarted=true;
				strSql+="��ȥ����";
			}
			  sInsertValues+="'";
			  sInsertValues+=szOffice;
			  sInsertValues+="'";
		}
		if (g_VisitorExp.bOfficePhoneExp)
		{
			if (bHaveStarted)
			{
				strSql+=",���õ绰";
				sInsertValues+=",";
			}
			else
			{
				bHaveStarted=true;
				strSql+="���õ绰";
			}
			  sInsertValues+="'";
			  sInsertValues+=szOfficePhone;
			  sInsertValues+="'";
		}
		//sprintf(szT, "insert into ���ñ��� values (\'%s\',\'%s\',\'%s\',\'%s\',\'%s\',\'%s\',\'%s\',\'%s\',\'%s\',\'%s\',\'%s\',\'%s\',\'%s\',\'%s\',\'%s\',\'%s\',\'%s\',\'%s\')",szStation, szName,szSex,szFolk,szIDType,szID,szAddr,szGovrment,szUnit,szTel,szCarNum,szVisitTime,szReason,szGoods,szLeftTime,szPeoples,szOfficialName,szOffice);
        strSql+=")";
		sInsertValues+=")";
		strSql+=sInsertValues;
		if(strSql=="insert into ���ñ��� (")
			return false;

		if(!bHaveStarted)
			return false;

		database.ExecuteSQL(strSql); 
	}
	database.Close();
	return true;
}

BOOL ExportUser(char *pszFileName)
{
	if (!_access(pszFileName,0)) //����ͬ���ļ�
	{
		return false;
	}
	char szT[512] = {'\0'};
	long FldCount = 0, ret = 0;;
	CString outParas[FLD_MAX];
	CWinAdo tAdo(sCONNSTR_MYSQL);
	
	/*FILE *fp = fopen(pszFileName, "wt");
	if (fp == NULL)
	{
		return false;
	}*/
	
	ret = tAdo.OpenAdo();
	if (ret != ADOOK)
	{
		return false;
	}
	
	for (int i = 0; i < FLD_MAX; i++)
	{
		outParas[i].Empty();
	}
	
	CDatabase database;
    CString sDriver = "MICROSOFT EXCEL DRIVER (*.XLS)"; // Excel��װ����
    CString sExcelFile = pszFileName;

	    // �������д�ȡ���ַ���
        CString strSql;
        strSql.Format("DRIVER={%s};DSN='';FIRSTROWHASNAMES=1;READONLY=FALSE;CREATE_DB=\"%s\";DBQ=%s",sDriver, sExcelFile, sExcelFile);
        
        // �������ݿ� (��Excel����ļ�)
        if(!database.OpenEx(strSql,CDatabase::noOdbcDialog))
        {
            printf("Excel����ʧ��\n");
            return false;
        }
        strSql = "CREATE TABLE t_user (user_id char(4),user_pwd char(20),user_right_id char(4))";
        database.ExecuteSQL(strSql);
		
	ret = tAdo.execSelect("select * from t_user", &FldCount, outParas);
	if (ret == ADOERR || (FldCount == 0 && outParas[0].IsEmpty()))
	{
		return true;
	}
	do 
	{
		//sprintf(szT, "%s\r\n", outParas[0]);
		sprintf(szT, "insert into t_user values (\'%s\', \'%s\', \'%s\')", outParas[0], outParas[1], outParas[2]);
		database.ExecuteSQL(szT);      
	} while(tAdo.getNext(outParas) == ADOOK);
	database.Close();	

	/*ret = tAdo.execSelect("select * from t_user", &FldCount, outParas);
	if (ret == ADOERR || (FldCount == 0 && outParas[0].IsEmpty()))
	{
		return false;
	}
	do 
	{
		fprintf(fp, "%s, %s, %s\r\n", outParas[0], outParas[1], outParas[2]);
	} while(tAdo.getNext(outParas) == ADOOK);
	
	fclose(fp);*/
	
	return true;
}

BOOL ImportLevel(char *pszFileName)
{
	if (_access(pszFileName,0)) //����ͬ���ļ�
	{
		return false;
	}

	char szT[512] = {'\0'};
	long FldCount = 0, ret = 0;;
	CString outParas[FLD_MAX];
	CWinAdo tAdo(sCONNSTR_MYSQL);
	
	CString str = CONNSTR_EXCEL + CString(pszFileName);
	CString outXLS[FLD_MAX];
	CWinAdo xls(str);
	
	/*ret = xls.OpenAdo();
	if (ret != ADOOK)
	{
		return false;
	}*/
	
	ret = tAdo.OpenAdo();
	if (ret != ADOOK)
	{
		return false;
	}
	
	for (int i = 0; i < FLD_MAX; i++)
	{
		outParas[i].Empty();
		outXLS[i].Empty();
	}

	CDatabase database;
    CString sDriver = "MICROSOFT EXCEL DRIVER (*.XLS)"; // Excel��װ����
    CString sExcelFile = pszFileName;

	    // �������д�ȡ���ַ���
        CString strSql;
        strSql.Format("DRIVER={%s};DSN='';FIRSTROWHASNAMES=1;READONLY=FALSE;CREATE_DB=\"%s\";DBQ=%s",sDriver, sExcelFile, sExcelFile);
        
        // �������ݿ� (��Excel����ļ�)
        if(!database.OpenEx(strSql,CDatabase::noOdbcDialog))
        {
            printf("Excel����ʧ��\n");
            return false;
        }
	// ��ȡ���е�ֵ������ǰ����������ͣ������д�뵽call price����ĵ�Ԫ����
        CRecordset rs(&database);       
        // ���ö�ȡ�Ĳ�ѯ���
        strSql = "SELECT * FROM [t_duty$A1:IV65536]";
        
        // ִ�в�ѯ���
        rs.Open(CRecordset::forwardOnly, strSql);

        //printf("begin����\n");
        
        // ��ȡ��ѯ�����������
        //CString strValue1, strValue2;
        float sum = 0.0f;
        while (!rs.IsEOF())
        {
            //��ȡExcel�ڲ���ֵ
            rs.GetFieldValue("user_id", outXLS[0]);  		
			rs.GetFieldValue("user_pwd", outXLS[1]); 
			rs.GetFieldValue("user_right_id", outXLS[2]); 

            sprintf(szT, "insert t_duty values (\'%s\', \'%s\', \'%s\')",outXLS[0], outXLS[1], outXLS[2]);
		    ret = tAdo.execUpdateInsert(szT);
            // �Ƶ���һ��
            rs.MoveNext();
        }
		rs.Close();
		database.Close();
	
	/*ret = xls.execSelect("select * from [Sheet1$]", &FldCount, outXLS);
	if (ret == ADOERR || (FldCount == 0 && outXLS[0].IsEmpty()))
	{
		return false;
	}
	do 
	{
		sprintf(szT, "insert t_duty values (\'%s\')", outXLS[0]);
		ret = tAdo.execUpdateInsert(szT);
	} while(xls.getNext(outXLS) == ADOOK);*/
	
	return true;
}

BOOL ExportLevel(char *pszFileName)
{
	if (!_access(pszFileName,0)) //����ͬ���ļ�
	{
		return false;
	}
	char szT[512] = {'\0'};
	long FldCount = 0, ret = 0;;
	CString outParas[FLD_MAX];
	CWinAdo tAdo(sCONNSTR_MYSQL);
	
	/*FILE *fp = fopen(pszFileName, "wt");
	if (fp == NULL)
	{
		return false;
	}*/
	
	ret = tAdo.OpenAdo();
	if (ret != ADOOK)
	{
		return false;
	}
	
	for (int i = 0; i < FLD_MAX; i++)
	{
		outParas[i].Empty();
	}
	
	CDatabase database;
    CString sDriver = "MICROSOFT EXCEL DRIVER (*.XLS)"; // Excel��װ����
    CString sExcelFile = pszFileName;

	    // �������д�ȡ���ַ���
        CString strSql;
        strSql.Format("DRIVER={%s};DSN='';FIRSTROWHASNAMES=1;READONLY=FALSE;CREATE_DB=\"%s\";DBQ=%s",sDriver, sExcelFile, sExcelFile);
        
        // �������ݿ� (��Excel����ļ�)
        if(!database.OpenEx(strSql,CDatabase::noOdbcDialog))
        {
            printf("Excel����ʧ��\n");
            return false;
        }
        strSql = "CREATE TABLE t_duty (duty_name char(40))";
        database.ExecuteSQL(strSql);
		
	ret = tAdo.execSelect("select * from t_user", &FldCount, outParas);
	if (ret == ADOERR || (FldCount == 0 && outParas[0].IsEmpty()))
	{
		return false;
	}
	do 
	{
		//sprintf(szT, "%s\r\n", outParas[0]);
		sprintf(szT, "insert into t_duty values (\'%s\')", outParas[0]);
		database.ExecuteSQL(szT);      
	} while(tAdo.getNext(outParas) == ADOOK);
	database.Close();	

	/*ret = tAdo.execSelect("select * from t_duty", &FldCount, outParas);
	if (ret == ADOERR || (FldCount == 0 && outParas[0].IsEmpty()))
	{
		return false;
	}
	do 
	{
		fprintf(fp, "%s\r\n", outParas[0]);
	} while(tAdo.getNext(outParas) == ADOOK);
	
	fclose(fp);*/
	
	return true;
}

BOOL ImportIDType(char *pszFileName)
{
	if (_access(pszFileName,0)) //����ͬ���ļ�
	{
		return false;
	}
		
	char szT[512] = {'\0'};
	long FldCount = 0, ret = 0;;
	CString outParas[FLD_MAX];
	CWinAdo tAdo(sCONNSTR_MYSQL);

	CString str = CONNSTR_EXCEL + CString(pszFileName);
	CString outXLS[FLD_MAX];
	//CWinAdo xls(str);
	
	/*ret = xls.OpenAdo();
	if (ret != ADOOK)
	{
		return false;
	}*/
	
	ret = tAdo.OpenAdo();
	if (ret != ADOOK)
	{
		return false;
	}
	
	for (int i = 0; i < FLD_MAX; i++)
	{
		outParas[i].Empty();
		outXLS[i].Empty();
	}
	
	CDatabase database;
    CString sDriver = "MICROSOFT EXCEL DRIVER (*.XLS)"; // Excel��װ����
    CString sExcelFile = pszFileName;

	    // �������д�ȡ���ַ���
        CString strSql;
        strSql.Format("DRIVER={%s};DSN='';FIRSTROWHASNAMES=1;READONLY=FALSE;CREATE_DB=\"%s\";DBQ=%s",sDriver, sExcelFile, sExcelFile);
        
        // �������ݿ� (��Excel����ļ�)
        if(!database.OpenEx(strSql,CDatabase::noOdbcDialog))
        {
            printf("Excel����ʧ��\n");
            return false;
        }
	// ��ȡ���е�ֵ������ǰ����������ͣ������д�뵽call price����ĵ�Ԫ����
        CRecordset rs(&database);       
        // ���ö�ȡ�Ĳ�ѯ���
        strSql = "SELECT * FROM [t_id_type$A1:IV65536]";
        
        // ִ�в�ѯ���
        rs.Open(CRecordset::forwardOnly, strSql);

        //printf("begin����\n");
        
        // ��ȡ��ѯ�����������
        //CString strValue1, strValue2;
        float sum = 0.0f;
        while (!rs.IsEOF())
        {
            //��ȡExcel�ڲ���ֵ
            rs.GetFieldValue("type_id", outXLS[0]);  		
			rs.GetFieldValue("type_name", outXLS[1]);  

            sprintf(szT, "insert t_id_type values (\'%s\', \'%s\')",outXLS[0], outXLS[1]);
		    ret = tAdo.execUpdateInsert(szT);
            // �Ƶ���һ��
            rs.MoveNext();
        }
		rs.Close();
		database.Close();

	/*ret = xls.execSelect("select * from [Sheet1$]", &FldCount, outXLS);
	if (ret == ADOERR || (FldCount == 0 && outXLS[0].IsEmpty()))
	{
		return false;
	}
	do 
	{
		sprintf(szT, "insert t_idtype values (\'%s\', \'%s\')", outXLS[0], outXLS[1]);
		ret = tAdo.execUpdateInsert(szT);
	} while(xls.getNext(outXLS) == ADOOK);*/
	
	return true;
}

BOOL ExportIDType(char *pszFileName)
{
	if (!_access(pszFileName,0)) //����ͬ���ļ�
	{
		return false;
	}
	char szT[512] = {'\0'};
	long FldCount = 0, ret = 0;;
	CString outParas[FLD_MAX];
	CWinAdo tAdo(sCONNSTR_MYSQL);
	
	/*FILE *fp = fopen(pszFileName, "wt");
	if (fp == NULL)
	{
		return false;
	}*/
	
	ret = tAdo.OpenAdo();
	if (ret != ADOOK)
	{
		return false;
	}
	
	for (int i = 0; i < FLD_MAX; i++)
	{
		outParas[i].Empty();
	}
	
	CDatabase database;
    CString sDriver = "MICROSOFT EXCEL DRIVER (*.XLS)"; // Excel��װ����
    CString sExcelFile = pszFileName;

	    // �������д�ȡ���ַ���
        CString strSql;
        strSql.Format("DRIVER={%s};DSN='';FIRSTROWHASNAMES=1;READONLY=FALSE;CREATE_DB=\"%s\";DBQ=%s",sDriver, sExcelFile, sExcelFile);
        
        // �������ݿ� (��Excel����ļ�)
        if(!database.OpenEx(strSql,CDatabase::noOdbcDialog))
        {
            printf("Excel����ʧ��\n");
            return false;
        }
        strSql = "CREATE TABLE t_id_type (type_id char(4),type_name char(20))";
        database.ExecuteSQL(strSql);
		
	ret = tAdo.execSelect("select * from t_id_type", &FldCount, outParas);
	if (ret == ADOERR || (FldCount == 0 && outParas[0].IsEmpty()))
	{
		return false;
	}
	do 
	{
		//sprintf(szT, "%s\r\n", outParas[0]);
		sprintf(szT, "insert into t_id_type values (\'%s\',\'%s\')", outParas[0],outParas[1]);
		database.ExecuteSQL(szT);      
	} while(tAdo.getNext(outParas) == ADOOK);
	database.Close();	
	
	/*ret = tAdo.execSelect("select * from t_idtype", &FldCount, outParas);
	if (ret == ADOERR || (FldCount == 0 && outParas[0].IsEmpty()))
	{
		return false;
	}
	do 
	{
		fprintf(fp, "%s, %s\r\n", outParas[0], outParas[1]);
	} while(tAdo.getNext(outParas) == ADOOK);
	
	fclose(fp);*/
	
	return true;
}

long GetFldStr(char* pszInbuf, const char* pszFilter, int iNo, char* pszOutBuf)
{
	long i = 0, j = 0, m = 0, n = 0;
	
	while (*(pszInbuf + i))
	{
		if (*(pszInbuf + i) == *pszFilter)
		{
			m++;
			if (m == iNo)
			{
				*(pszOutBuf + j) = 0x0;
				break;
			}
			else 
			{
				memset(pszOutBuf, 0, j);
				j = 0;
			}
		}
		else
		{
			*(pszOutBuf + j++) = *(pszInbuf + i);
		}
		i++;
	}
	
	return 0;
}

//xls��Ϣ�ļ��ĵ�һ�о������ݣ�û�б�������
BOOL CALLBACK OnImportXls(HELE hEle, HELE hEventEle)
{
	BOOL bRet = false;
	HELE hE = GETELE(IDK_IMPORT_OFFICIAL);
	if (XBtn_IsCheck(hE))
	{
		bRet = ImportOfficial(IMPORT_FILE_OFFICIAL);
		if (!bRet)
		{
			XMessageBox(g_hWindow, L"������Ա��Ϣʧ��!", L"����");
		}
	}

	hE = GETELE(IDK_IMPORT_OFFICE);
	if (XBtn_IsCheck(hE))
	{
		bRet = ImportOffice(IMPORT_FILE_OFFICE);
		if (!bRet)
		{
			XMessageBox(g_hWindow, L"���������Ϣʧ��!", L"����");
		}
	}

	hE = GETELE(IDK_IMPORT_GOODS);
	if (XBtn_IsCheck(hE))
	{
		bRet = ImportWithGoods(IMPORT_FILE_GOODS);
		if (!bRet)
		{
			XMessageBox(g_hWindow, L"����������Ʒ��Ϣʧ��!", L"����");
		}
	}

	hE = GETELE(IDK_IMPORT_REASON);
	if (XBtn_IsCheck(hE))
	{
		bRet = ImportReason(IMPORT_FILE_REASON);
		if (!bRet)
		{
			XMessageBox(g_hWindow, L"��������������Ϣʧ��!", L"����");
		}
	}

	hE = GETELE(IDK_IMPORT_STATION);
	if (XBtn_IsCheck(hE))
	{
		bRet = ImportStation(IMPORT_FILE_STATION);
		if (!bRet)
		{
			XMessageBox(g_hWindow, L"�����Ÿ���Ϣʧ��!", L"����");
		}
	}

	hE = GETELE(IDK_IMPORT_CZY);
	if (XBtn_IsCheck(hE))
	{
		bRet = ImportUser(IMPORT_FILE_USER);
		if (!bRet)
		{
			XMessageBox(g_hWindow, L"������Ա��Ϣʧ��!", L"����");
		}
	}

	hE = GETELE(IDK_IMPORT_BLACKNAME);
	if (XBtn_IsCheck(hE))
	{
		bRet = ImportBlackName(IMPORT_FILE_BLACKNAME);
		if (!bRet)
		{
			XMessageBox(g_hWindow, L"�����������Ϣʧ��!", L"����");
		}
	}

	hE = GETELE(IDK_IMPORT_LEVEL);
	if (XBtn_IsCheck(hE))
	{
		bRet = ImportLevel(IMPORT_FILE_LEVEL);
		if (!bRet)
		{
			XMessageBox(g_hWindow, L"������Ա������Ϣʧ��!", L"����");
		}
	}

	XMessageBox(g_hWindow, L"���ݵ������!", L"��ʾ");
	XWnd_RedrawWnd(g_hWindow);
	return true;
}

BOOL CALLBACK OnExportXls(HELE hEle, HELE hEventEle)
{
	BOOL bRet = false;
	HELE hE = GETELE(IDK_IMPORT_OFFICIAL);
	if (XBtn_IsCheck(hE))
	{
		bRet = ExportOfficial(EXPORT_FILE_OFFICIAL);
		if (!bRet)
		{
			XMessageBox(g_hWindow, L"������Ա��Ϣʧ��!", L"����");
		}
	}
	
	hE = GETELE(IDK_IMPORT_OFFICE);
	if (XBtn_IsCheck(hE))
	{
		bRet = ExportOffice(EXPORT_FILE_OFFICE);
		if (!bRet)
		{
			XMessageBox(g_hWindow, L"����������Ϣʧ��!", L"����");
		}
	}
	
	hE = GETELE(IDK_IMPORT_GOODS);
	if (XBtn_IsCheck(hE))
	{
		bRet = ExportWithGoods(EXPORT_FILE_GOODS);
		if (!bRet)
		{
			XMessageBox(g_hWindow, L"����������Ʒ��Ϣʧ��!", L"����");
		}
	}
	
	hE = GETELE(IDK_IMPORT_REASON);
	if (XBtn_IsCheck(hE))
	{
		bRet = ExportReason(EXPORT_FILE_REASON);
		if (!bRet)
		{
			XMessageBox(g_hWindow, L"��������������Ϣʧ��!", L"����");
		}
	}
	
	hE = GETELE(IDK_IMPORT_STATION);
	if (XBtn_IsCheck(hE))
	{
		bRet = ExportStation(EXPORT_FILE_STATION);
		if (!bRet)
		{
			XMessageBox(g_hWindow, L"�����Ÿ���Ϣʧ��!", L"����");
		}
	}
	
	hE = GETELE(IDK_IMPORT_CZY);
	if (XBtn_IsCheck(hE))
	{
		bRet = ExportUser(EXPORT_FILE_USER);
		if (!bRet)
		{
			XMessageBox(g_hWindow, L"������Ա��Ϣʧ��!", L"����");
		}
	}
	
	hE = GETELE(IDK_IMPORT_BLACKNAME);
	if (XBtn_IsCheck(hE))
	{
		bRet = ExportBlackName(EXPORT_FILE_BLACKNAME);
		if (!bRet)
		{
			XMessageBox(g_hWindow, L"������������Ϣʧ��!", L"����");
		}
	}
	
	hE = GETELE(IDK_IMPORT_LEVEL);
	if (XBtn_IsCheck(hE))
	{
		bRet = ExportLevel(EXPORT_FILE_LEVEL);
		if (!bRet)
		{
			XMessageBox(g_hWindow, L"������Ա������Ϣʧ��!", L"����");
		}
	}
	
	XMessageBox(g_hWindow, L"���ݵ������!", L"��ʾ");
	XWnd_RedrawWnd(g_hWindow);
	return true;
}


BOOL CALLBACK OnCallPhone(HELE hEle, HELE hEventEle)
{
	if (!g_bPhoneIsIni)
	{
		//SendCmd(CTL_HANDFREE_OFF);
		//Sleep(1000);
		//g_bPhoneIsBusy = false;
        XMessageBox(g_hWindow, L"����ժ��!", L"��ʾ");
		//return false;
	}

	wchar_t wBuf[64] = {0};
	char szNum[64] = {'\0'};
	HELE hE = GETELE(IDE_OFFICEPHONE);
	XEdit_GetText(hE, wBuf, 32);
	if (wcslen(wBuf) <= 0)
	{
		XMessageBox(g_hWindow, L"����İ칫�绰����Ϊ��!", L"����");
		XWnd_RedrawWnd(g_hWindow);
		return false;
	}
	W_2_S(wBuf, szNum, 32);

	SendCmd(CTL_HANDFREE_ON);
	SendCmd(ASK_DTMF_TX, strlen(szNum), szNum);
	Sleep(1000);
    SetTimer(XWnd_GetHWnd(g_hWindow),TimerHandOn,4000,TimerPro); 
    XEle_EnableEle(g_hTab6, false);
	XBtn_SetText(g_hTab6,L"������...");
	g_bPhoneIsBusy = true;
	XWnd_RedrawWnd(g_hWindow);
	return false;
}

BOOL CALLBACK OnUsbPhoneEvent(HWINDOW hWindow, WPARAM wParam, LPARAM lParam)
{
	UsbPhoneEvent event = static_cast<UsbPhoneEvent>(wParam);
	switch (event) {
    case EVT_HAND_HOOK_ON:
	case EVT_PARALL_HOOK_ON:
		{
			//MessageBox(_T("�һ��ɹ���"));
			g_bPhoneIsIni = true;	//����ժ��			
			//XMessageBox(g_hWindow, L"ժ�����ɹ���", L"����");
			break;
		}	
	case EVT_NEW_CID_TX:
	case EVT_OLD_CID_TX:
	case EVT_DIALED_TX:
    case EVT_HAND_HOOK_OFF:
	case EVT_PARALL_HOOK_OFF:
    case EVT_DIAL_END:
		{
			//XMessageBox(g_hWindow, L"�һ��ɹ���", L"����");
			XEle_EnableEle(g_hTab6, true);
			XEle_EnableEle(g_hTab7, true);
			XBtn_SetText(g_hTab6,L"�칫�绰");
			XBtn_SetText(g_hTab7,L"�ƶ��绰");
			XWnd_RedrawWnd(XEle_GetHWindow(g_hTab6),TRUE);
            XWnd_RedrawWnd(XEle_GetHWindow(g_hTab7),TRUE);
			SendCmd(CTL_HANDFREE_OFF);
			g_bPhoneIsIni = false;
			g_bPhoneIsBusy = false;
		    //Sleep(1000);
		}
		break;
	case EVT_REDIAL:
		{
			KillTimer(NULL,TimerDialEndChecker);
			if (g_bPhoneIsBusy)
			{
				Sleep(1000);
				SetTimer(NULL,TimerDialEndChecker, 6000, TimerProHandOff);
			}
		}
		break;
	case EVT_USB_ATTACH:
		{
					// ��ʼͨѶ
		SendCmd(CTL_USB_ON);
		// ���в�ѯ״̬/��ȡ��Ϣ�������ȷ���
		// {{{
		// �豸������λ
		//capability_ = Capability();
		SendCmd(ASK_HARDWARE_INF); // -> MSG_HW_TYPE

		SendCmd(ASK_READ_SN);      // -> MSG_SERIAL, ��������Ҫ
		SendCmd(ASK_GET_VERSION);  // -> EVT_VERSION, ��������Ҫ

		//memset(&syncedPhoneState_, 0, sizeof(syncedPhoneState_));
		SendCmd(ASK_PHONE_STATE);  // -> EVT_PHONE_STATE
		// �Լ�¼�ϴ���ϱ�־ͬ������, �����������
		SendCmd(ASK_RECORD_UP);    // -> EVT_NEW_CID_TX, EVT_OLD_CID_TX, EVT_DIALED_TX, EVT_HISTORY_FINISHED 
		// }}}
		// �رջ����Դ�(�����Զ�����)������
		// ������𲥷������ļ���Ϊ����
		SendCmd(CTL_RINGER_OFF);
		// ͬ��ʱ��
		/*{
			tm now;
			const time_t time = std::time(NULL); // ϵͳ��ǰʱ��
			localtime_s(&now, &time);            // ȡ����ʱ��������/ʱ��
			char timeBuffer[] = {
				(char) (now.tm_year % 100),      // ȡĩβ2λ, 00~99
				(char) (now.tm_mon + 1),         // 01~31 (tm_mon=00~30, ���� +1)
				(char) now.tm_mday,
				(char) now.tm_hour,
				(char) now.tm_min,
				(char) now.tm_sec
			};
			SendCmd(CTL_SET_TIME, countof(timeBuffer), timeBuffer);
		}*/

		SetTimer(XWnd_GetHWnd(g_hWindow),TimerHandOn,4000,TimerPro);
		}
        break;
	}
	return true;
}

BOOL CALLBACK OnCallMobile(HELE hEle, HELE hEventEle)
{
	if (!g_bPhoneIsIni)
	{
		//SendCmd(CTL_HANDFREE_OFF);
		//Sleep(1000);
		//g_bPhoneIsBusy = false;
        XMessageBox(g_hWindow, L"����ժ��!", L"��ʾ");
		//return false;
	}
	
	wchar_t wBuf[64] = {0};
	char szNum[64] = {'\0'},szT[64]={'\0'};
	HELE hE = GETELE(IDE_MASTERMOBILE);
	::GetPrivateProfileString("TerminalSet", "PrefixPhone", "", szNum, sizeof(szNum), CONFIG_FILE);
	XEdit_GetText(hE, wBuf, 32);
	if (wcslen(wBuf) <= 0)
	{
		XMessageBox(g_hWindow, L"������ƶ��绰����Ϊ��!", L"����");
		XWnd_RedrawWnd(g_hWindow);
		return false;
	}
	W_2_S(wBuf, szT, 32);
    strcat(szNum,szT);
	
	SendCmd(CTL_HANDFREE_ON);
	SendCmd(ASK_DTMF_TX, strlen(szNum), szNum);
	Sleep(1000);		
	SetTimer(XWnd_GetHWnd(g_hWindow),TimerHandOn,4000,TimerPro); 
    g_bPhoneIsBusy = true;
    XEle_EnableEle(g_hTab7, false);
	XBtn_SetText(g_hTab7,L"������...");
	XWnd_RedrawWnd(g_hWindow);
	return false;
}

BOOL CALLBACK OnHangUp(HELE hEle, HELE hEventEle)
{
	if (g_bPhoneIsBusy)
	{
		
		SetTimer(NULL,TimerDialEndChecker,6000,TimerProHandOff); 
		SendCmd(CTL_HANDFREE_OFF);
		Sleep(1000);
		//KillTimer(NULL,IDT_TIMEE); 
		g_bPhoneIsBusy = false;
	}

	return false;
}

int CheckVisitor(const VISITORINFO &visitor)
{
	wchar_t wBuf[128] = {0};
	char szT[512] = {'\0'},szOfficialName[50] = {'\0'};
	long FldCount = 0, ret = 0;
	CString outParas[FLD_MAX];
	CWinAdo tAdo(sCONNSTR_MYSQL);


	
	ret = tAdo.OpenAdo();
	if (ret != ADOOK)
	{
		return true;
	}
	
	for (int i = 0; i < FLD_MAX; i++)
	{
		outParas[i].Empty();
	}

	if(g_nIDType == ID_TYPE_Lawyer) //ɨ����ʦ֤ʱ��ʾ��ʦ�����Ŀ
	{

		if (strlen(g_Printvisitor.szName)==0)  //û�б���ʾ���飬��ʾ�µǼ���ʦ
		{
			return false;
		}


		sprintf(szT, "select * from t_visitor where visitor_name = \'%s\' and visitor_to_official_name = \'%s\' and visitor_lefttime <= 0",\
			visitor.szName, visitor.szToOfficialName);
		ret = tAdo.execSelect(szT, &FldCount, outParas);
		if (ret == ADOERR || (FldCount == 0 && outParas[0].IsEmpty()))
		{
			return false;               
		}
		//�Ұ����˵Ǽ���Ϣ
		return true; //��ʾ�÷ÿ��Ѿ��Ǽ�
	}

	
	sprintf(szT, "select * from t_visitor where visitor_name = \'%s\' and visitor_id = \'%s\' and visitor_status>1 and visitor_lefttime <= 0",\
		visitor.szName, visitor.szID);
	ret = tAdo.execSelect(szT, &FldCount, outParas);
	if (ret == ADOERR || (FldCount == 0 && outParas[0].IsEmpty()))
	{
		return false;               //�÷ÿ�δ�Ǽ�
	}

	memset((char *)wBuf, 0, sizeof(wBuf));
	//hE = XWnd_GetEle(g_hWindow, IDE_MASTERNAME);		//������������
	/*if (XEdit_GetTextLength(hE) <= 0 && atoi(sz) == 1)
	{
		XMessageBox(g_hWindow, L"����������������Ϊ��", L"����");
		return false;
	}*/
	XEdit_GetText(XWnd_GetEle(g_hWindow, IDE_MASTERNAME), wBuf, 128);
	W_2_S(wBuf, szOfficialName, 128);
	if (strcmp(outParas[14],szOfficialName)&&strlen(szOfficialName)>0)  //�����˸ı�ʱ��Ϊ������
	{
		//InitData();
        XMessageBox(g_hWindow, L"�������߸ı䱻���ˣ�����ǩ������µǼ�!", L"��ʾ");
		return -1;
	}
    //XMessageBox(g_hWindow, L"112!", L"����");
	return true;		//�÷ÿ��Ѿ��Ǽǣ���Ϊǩ��
}
//�������������
void InitVisitorData()
{
	char szPic[128] = {'\0'};
	memset(g_szOfficialIP, 0, sizeof(g_szOfficialIP));
	memset(g_szVisitorName, 0, sizeof(g_szVisitorName));
	memset(g_szRecord, 0, sizeof(g_szRecord));
    g_bCalled = false;
	memset((char *)&g_stIDCard, 0, sizeof(IDCARD_ALL));
	EmptyVisitorPara();

	HELE hE = XWnd_GetEle(g_hWindow, IDP_ID2_HEADPIC);
	XPic_SetImage(hE, NULL);
	hE = XWnd_GetEle(g_hWindow, IDP_CAMERA);
	XPic_SetImage(hE, NULL);
	XBtn_SetText(g_hTab1, L"����");
	hE = XWnd_GetEle(g_hWindow, IDP_PASSPORTPIC);
	XPic_SetImage(hE, NULL);
	hE = XWnd_GetEle(g_hWindow, IDP_HEADPIC);
	XPic_SetImage(hE, NULL);
	HELE hEle = NULL;
	HIMAGE hImg = NULL;
	hEle = XWnd_GetEle(g_hWindow, IDP_ID2PIC);
	hImg = XImage_LoadFile(L".\\skin\\image\\id2.bmp", true);
    XPic_SetImage(hEle, hImg);

	hE = XWnd_GetEle(g_hWindow, IDP_ID2PIC);
	XEle_ShowEle(hE, true);

	//hE = XWnd_GetEle(hWindow, IDP_ID2PIC);
	//XEle_ShowEle(hE, true);
	hE = XWnd_GetEle(g_hWindow, IDG_ID2);	//����֤ͼ���
	XEle_ShowEle(hE, true);

	HELE hList = GETELE(IDL_VISTORLIST);   //���ѡ���б���
	int i;
	for (i=0;i<XList_GetItemCount(hList);i++)
	{
		XList_SetItemCheck(hList,i,false);
	}
	
	for (i = 5003; i <= 5012; i++)		//�������
	{
		hE = GETELE(i);
		XEdit_SetText(hE, L"");
	}
	for (i = 5020; i <= 5023; i++)		//�������
	{
		hE = GETELE(i);
		XEdit_SetText(hE, L"");
	}
	//XEdit_SetText(GETELE(5024), L"");


	ShowWindow(g_hCapWnd, true);
	RePlayCapture();//RePlay();
	ResumeThread(g_HWDThreadID2ID);
	return;
}
//�����������
void InitData()
{
	char szPic[128] = {'\0'};
	memset(g_szOfficialIP, 0, sizeof(g_szOfficialIP));
	memset(g_szVisitorName, 0, sizeof(g_szVisitorName));
	memset(g_szRecord, 0, sizeof(g_szRecord));
    g_bCalled = false;
	g_bAcceptVisite=false;
	memset((char *)&g_stIDCard, 0, sizeof(IDCARD_ALL));
	EmptyVisitorPara();

	HELE hE = XWnd_GetEle(g_hWindow, IDP_ID2_HEADPIC);
	XPic_SetImage(hE, NULL);
	hE = XWnd_GetEle(g_hWindow, IDP_CAMERA);
	XPic_SetImage(hE, NULL);
	XBtn_SetText(g_hTab1, L"����");
	hE = XWnd_GetEle(g_hWindow, IDP_PASSPORTPIC);
	XPic_SetImage(hE, NULL);
	hE = XWnd_GetEle(g_hWindow, IDP_HEADPIC);
	XPic_SetImage(hE, NULL);
	HELE hEle = NULL;
	HIMAGE hImg = NULL;
	hEle = XWnd_GetEle(g_hWindow, IDP_ID2PIC);
	hImg = XImage_LoadFile(L".\\skin\\image\\id2.bmp", true);
    XPic_SetImage(hEle, hImg);

	hE = XWnd_GetEle(g_hWindow, IDP_ID2PIC);
	XEle_ShowEle(hE, true);

	//hE = XWnd_GetEle(hWindow, IDP_ID2PIC);
	//XEle_ShowEle(hE, true);
	hE = XWnd_GetEle(g_hWindow, IDG_ID2);	//����֤ͼ���
	XEle_ShowEle(hE, true);

	HELE hList = GETELE(IDL_VISTORLIST);   //���ѡ���б���
	int i;
	for (i=0;i<XList_GetItemCount(hList);i++)
	{
		XList_SetItemCheck(hList,i,false);
	}
	
	for (i = 5003; i <= 5024; i++)		//�������
	{
		hE = GETELE(i);
		XEdit_SetReadOnly(hE,false);
		XEdit_SetText(hE, L"");
	}

	if (::GetPrivateProfileInt("TerminalSet", "bVisitorReadOnly",0, CONFIG_FILE)==1)  //���õǼ�֤������ֻ��
	   SetVisitorReadOnly(true);
	else
	   SetVisitorReadOnly(false);

	if (::GetPrivateProfileInt("TerminalSet", "bSaveOfficial",0, CONFIG_FILE)==1)  //�����±�����
	   SetOfficialReadOnly(false);
	else
	   SetOfficialReadOnly(true);

	ShowWindow(g_hCapWnd, true);
	RePlayCapture();//RePlay();
	ResumeThread(g_HWDThreadID2ID);
    

	SuspendThread(g_ThreadID2IC);
	if (g_ThreadID2IC)
	{
        CloseHandle(g_ThreadID2IC);
	    g_ThreadID2IC = NULL;
	}
 	if (g_hCom)
	{
        CloseHandle(g_hCom);
	    g_hCom = NULL;
	}
	return;
}

bool SetLoginUser()
{
	char szUserName[32] = {'\0'};
	wchar_t wBuf[256 + 1] = {0};
	char szT[512] = {'\0'};
	long FldCount = 0, ret = 0;;
	CString outParas[FLD_MAX];
	CWinAdo tAdo(sCONNSTR_MYSQL);
	ret = tAdo.OpenAdo();
	if (ret != ADOOK)
	{
		return false;
	}	


	for (int i = 0; i < FLD_MAX; i++)
	{
		outParas[i].Empty();
	}

	strcpy(szT, "select user_id from t_user");
	ret = tAdo.execSelect(szT, &FldCount, outParas);
	if (ret == ADOERR || (FldCount == 0 && outParas[0].IsEmpty()))
	{
		return false;
	}

	HELE hE = GETELE(IDE_LOGIN_CZYID);
	//XComboBox_SetListHeight(hE,100);
	XComboBox_EnableEdit(hE, false);
	do
	{
		memset((char *)&wBuf, 0, sizeof(wBuf));
		S_2_W(wBuf, (LPSTR)(LPCTSTR)outParas[0], 16);
		XComboBox_AddString(hE, wBuf);
	} while (tAdo.getNext(outParas) == ADOOK);
	XComboBox_SetSelectItem(hE, 0);
		
	::GetPrivateProfileString("TerminalSet", "UserName", "", szUserName, sizeof(szUserName), CONFIG_FILE);
	hE = GETELE(IDE_LOGIN_CZYID);
	if (strlen(szUserName)>0)
	{
        S_2_W(wBuf,szUserName,32);
		XEdit_SetText(hE,wBuf);
	}
	XWnd_RedrawWnd(g_hWindow);
	return false;
}

void ShowMsgPage(int nResID, wchar_t *pwsInfo, BOOL bShow)
{
	if(!bShow)
	{
		XEle_ShowEle(g_hPage11, false);
		return;
	}
	HELE hEle = XWnd_GetEle(g_hWindow, nResID);
	XStatic_SetText(hEle, pwsInfo);
	XStatic_SetTextAlign(hEle, DT_CENTER | DT_EDITCONTROL | DT_WORDBREAK);

	HFONT hFont = ::CreateFont(24, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, 
		GB2312_CHARSET, 
		OUT_DEFAULT_PRECIS, 
		CLIP_DEFAULT_PRECIS, 
		DEFAULT_QUALITY, 
		DEFAULT_PITCH & FF_ROMAN, 
		"����");		//΢���ź� 20
	//HFONTX hFont = XFont_Create2(L"����",24,false,false,false,false);
	XEle_SetFont(hEle, hFont);

	XEle_ShowEle(g_hPage11, bShow);

	return;
}



//ģ��ƥ�䱻������Ϣ
BOOL PiPeiEmployer()
{
	char szOfficialName[32] = {'\0'};
	wchar_t wBuf[256 + 1] = {0};

	HELE hE = XWnd_GetEle(g_hWindow, IDE_MASTERNAME);
	/*if (XEdit_GetTextLength(hE) <= 0)
	{
		XMessageBox(g_hWindow, L"��������������Ϊ��!", L"��ʾ��Ϣ", XMB_OK);
		return false;
	}*/

	XEdit_GetText(hE, wBuf, 256);
	W_2_S(wBuf, szOfficialName, 256);

	char szT[512] = {'\0'};
	long FldCount = 0, ret = 0;;
	CString outParas[FLD_MAX];
	CWinAdo tAdo(sCONNSTR_MYSQL);

	ret = tAdo.OpenAdo();
	if (ret != ADOOK)
	{
		return false;
	}
	
	for (int i = 0; i < FLD_MAX; i++)
	{
		outParas[i].Empty();
	}
	
	sprintf(szT, "select * from t_official where official_name like \'%s%s%s\'", "%", szOfficialName, "%");	//��ѯ�����˵���Ϣ
	ret = tAdo.execSelect(szT, &FldCount, outParas);

//	HELE hList = GETELE(IDL_SELECTOFFICIAL);

	g_hOfficialWin = XModalWnd_CreateWindow(800, 600, L"�����˷�����Ϣ", XWnd_GetHWnd(g_hWindow));
	XWnd_SetBorderSize(g_hOfficialWin, 3, 3, 3, 3);

	HIMAGE hImg = XImage_LoadFile(L".\\skin\\image\\frame.jpg", true);
	XWnd_SetImageNC(g_hOfficialWin, hImg);
	HELE hList = XList_Create(0, 0, 795, 570, g_hOfficialWin);

	XEle_RegisterEvent(hList, XE_LIST_SELECT, OnSelectOfficialListClick);


	
	XList_SetHeaderHeight(hList, 30);	
	XList_EnableGrid(hList, false);


	XList_AddColumn(hList, 40, L"���");
	XList_AddColumn(hList, 80, L"����������");
	XList_AddColumn(hList, 40, L"�Ա�");
	XList_AddColumn(hList, 100, L"�칫�绰");
	XList_AddColumn(hList, 100, L"�ƶ��绰");
	XList_AddColumn(hList, 100, L"��������");
	XList_AddColumn(hList, 100, L"�칫��ַ");
	XList_AddColumn(hList, 100, L"����");
	XList_AddColumn(hList, 100, L"�����");
	if (ret == ADOERR || (FldCount == 0 && outParas[0].IsEmpty()))
	{
//		XMessageBox(g_hWindow,L"����Ϣ",L"��ʾ");
		XWnd_RedrawWnd(XEle_GetHWindow(hList));
		return false;
	}
	int n = 0;
	
	do 
	{
		memset(szT, 0, sizeof(szT));
		memset((char *)wBuf, 0, sizeof(wBuf));
		
		sprintf(szT, "%d", n + 1);		//���
		S_2_W(wBuf, szT, 256);
		XList_AddItem(hList, wBuf, n);
			
		L_V_I(outParas[0], n, 1)
		L_V_I(outParas[1], n, 2)
		L_V_I(outParas[2], n, 3)
		L_V_I(outParas[3], n, 4)
		L_V_I(outParas[4], n, 5)
		L_V_I(outParas[5], n, 6)
		L_V_I(outParas[6], n, 7)
		L_V_I(outParas[7], n, 8)

		n++;
	} while (tAdo.getNext(outParas) == ADOOK);
    XModalWnd_DoModal(g_hOfficialWin);
	return true;
}

//��ȡ�����
void GetBarCode(char *szBarCode)
{
	long n;
	int nBarCord[12];
	int odd=0,even=0,result=0;

	if (::GetPrivateProfileInt("TerminalSet", "bWebOnline",0, CONFIG_FILE)==0)
	{
		while (1)
		{
			time((time_t *)&n);
			nBarCord[0]=n/1000000000;
			nBarCord[1]=n/100000000-nBarCord[0]*10;
			nBarCord[2]=n/10000000-(n/100000000)*10;
			nBarCord[3]=n/1000000-(n/10000000)*10;
			nBarCord[4]=n/100000-(n/1000000)*10;
			nBarCord[5]=n/10000-(n/100000)*10;
			nBarCord[6]=n/1000-(n/10000)*10;
			nBarCord[7]=n/100-(n/1000)*10;
			nBarCord[8]=n/10-(n/100)*10;
			nBarCord[9]=n-(n/10)*10;
			nBarCord[10]=1;
			nBarCord[11]=2;
			nBarCord[12]=3;
			
			
			for(int i=1; i<=11; i+=2)
				odd += nBarCord[i];
			for(int j=0; j<=10; j+=2)
				even += nBarCord[j];
			result = 10 - (odd*3+even)%10;
			if (result<10)
			  break;
		}
			    //visitor.lVisiteTime = n;			//����ʱ��	
	sprintf(szBarCode, "%d%d%d%d%d%d%d%d%d%d%d%d%d", nBarCord[0], nBarCord[1], nBarCord[2], nBarCord[3], nBarCord[4], nBarCord[5], 
		nBarCord[6], nBarCord[7], nBarCord[8], nBarCord[9], nBarCord[10], nBarCord[11], result);	//���ʵ�������,�ԵǼ�ʱ����Ϊ������

	}
	else
	{
		GetTcpBarAndUpVisitorInfo(szBarCode);
	}

}

void FtpConnect()
{
	//�½��Ի�
	m_pInetSession=new CInternetSession("cc",1,PRE_CONFIG_INTERNET_ACCESS);
	try 
	{ 
		char szSvrName[40] = {'\0'},szUsr[40] = {'\0'},szPwd[40] = {'\0'};
		::GetPrivateProfileString("TerminalSet", "FtpSeverName", "", szSvrName, sizeof(szSvrName), CONFIG_FILE);
		::GetPrivateProfileString("TerminalSet", "FtpUser", "", szUsr, sizeof(szUsr), CONFIG_FILE);
		::GetPrivateProfileString("TerminalSet", "FtpPwd", "", szPwd, sizeof(szPwd), CONFIG_FILE);
		//�½����Ӷ���
		m_pFtpConnection=m_pInetSession->GetFtpConnection(szSvrName,szUsr,szPwd); 
	} 
	catch(CInternetException *pEx) 
	{
		//��ȡ����
		TCHAR szError[1024];
		if(pEx->GetErrorMessage(szError,1024))
			AfxMessageBox(szError);
		else  
			AfxMessageBox("There was an exception");
		pEx->Delete();
		m_pFtpConnection=NULL;

		return;
	}
	m_pRemoteFinder = new CFtpFileFind(m_pFtpConnection);
	g_bFtpOpened = true;
}

void InitalPrintTipsCfg()
{
    g_PrintTipsCfg.bHideGoodsPrint = false;
	g_PrintTipsCfg.bHideLast4ID = false;
	g_PrintTipsCfg.bToRoomPrint = false;
	g_PrintTipsCfg.bTotalPeoplePrint = false;
	g_PrintTipsCfg.ePrintType = PRINT_FULL_TYPE;
    //g_PrintTipsCfg.szUnitName[255] = {'\0'};
	memset(g_PrintTipsCfg.szUnitName, 0, sizeof(g_PrintTipsCfg.szUnitName));

	if (::GetPrivateProfileInt("TerminalSet", "bHideLast4ID",0, CONFIG_FILE)==1)
	{
         g_PrintTipsCfg.bHideLast4ID = true;
	}

	if (::GetPrivateProfileInt("TerminalSet", "bHideGoodsPrint",0, CONFIG_FILE)==0)
	{
         g_PrintTipsCfg.bHideGoodsPrint = true;
	}

	if (GetPrivateProfileInt("TerminalSet", "bHideToRoomPrint",0, CONFIG_FILE)==0)
	{
         g_PrintTipsCfg.bToRoomPrint = true;
	}

	if (GetPrivateProfileInt("TerminalSet", "bHideTotalPeoplePrint",0, CONFIG_FILE)==0)
	{
         g_PrintTipsCfg.bTotalPeoplePrint = true;
	}

	if (GetPrivateProfileInt("TerminalSet", "bPrintTips2",0, CONFIG_FILE)==1)
	{
        g_PrintTipsCfg.ePrintType = PRINT_SIMPLE_TYPE;
	}
	else if (GetPrivateProfileInt("TerminalSet", "bPrintTips2",0, CONFIG_FILE)==2)
	{
		g_PrintTipsCfg.ePrintType = PRINT_NOFRAME_TYPE;
	}
	else
        g_PrintTipsCfg.ePrintType = PRINT_FULL_TYPE;

	::GetPrivateProfileString("TerminalSet", "PrintFootLog", "", g_PrintTipsCfg.szUnitName, sizeof(g_PrintTipsCfg.szUnitName), CONFIG_FILE);

}

//��ʼ���������ñ�����Ŀ
void InitalVisitorExp()
{	
    g_VisitorExp.bStationExp=false;
	g_VisitorExp.bVisitorExp=false;
	g_VisitorExp.bVSexExp=false;
	g_VisitorExp.bVFolkExp=false;
	g_VisitorExp.bIDTypeExp=false;
	g_VisitorExp.bIDExp=false;
	g_VisitorExp.bAddrExp=false;
	g_VisitorExp.bSigndepartmentExp=false;
	g_VisitorExp.bUnitExp=false;
	g_VisitorExp.bTelExp=false;
	g_VisitorExp.bCarNumExp=false;
	g_VisitorExp.bVisittimeExp=false;
	g_VisitorExp.bReasonExp=false;
	g_VisitorExp.bGoodsExp=false;
	g_VisitorExp.bLefttimeExp=false;
	g_VisitorExp.bTotalPeopleExp=false;
	g_VisitorExp.bOfficerExp=false;
	g_VisitorExp.bOfficeNameExp=false;
	g_VisitorExp.bOfficePhoneExp=false;
	if (::GetPrivateProfileInt("TerminalSet", "bStationExp",0, CONFIG_FILE)==1)
	{
       g_VisitorExp.bStationExp=true;
	}
	if (::GetPrivateProfileInt("TerminalSet", "bVisitorExp",0, CONFIG_FILE)==1)
	{
		g_VisitorExp.bVisitorExp=true;
	}
	if (GetPrivateProfileInt("TerminalSet", "bVSexExp",0, CONFIG_FILE)==1)
	{
		g_VisitorExp.bVSexExp=true;
	}
	if (GetPrivateProfileInt("TerminalSet", "bVFolkExp",0, CONFIG_FILE)==1)
	{
		g_VisitorExp.bVFolkExp=true;
	}
	if (GetPrivateProfileInt("TerminalSet", "bIDTypeExp",0, CONFIG_FILE)==1)
	{
	    g_VisitorExp.bIDTypeExp=true;
	}
	if (GetPrivateProfileInt("TerminalSet", "bIDExp",0, CONFIG_FILE)==1)
	{
		g_VisitorExp.bIDExp=true;
	}
	if (GetPrivateProfileInt("TerminalSet", "bAddrExp",0, CONFIG_FILE)==1)
	{
		g_VisitorExp.bAddrExp=true;
	}
	if (GetPrivateProfileInt("TerminalSet", "bSigndepartmentExp",0, CONFIG_FILE)==1)
	{
		g_VisitorExp.bSigndepartmentExp=true;
	}
	if (GetPrivateProfileInt("TerminalSet", "bUnitExp",0, CONFIG_FILE)==1)
	{
		g_VisitorExp.bUnitExp=true;
	}
	if (GetPrivateProfileInt("TerminalSet", "bTelExp",0, CONFIG_FILE)==1)
	{
		g_VisitorExp.bTelExp=true;
	}
	if (GetPrivateProfileInt("TerminalSet", "bCarNumExp",0, CONFIG_FILE)==1)
	{
		g_VisitorExp.bCarNumExp=true;
	}
	if (GetPrivateProfileInt("TerminalSet", "bVisittimeExp",0, CONFIG_FILE)==1)
	{
		g_VisitorExp.bVisittimeExp=true;
	}
	if (GetPrivateProfileInt("TerminalSet", "bReasonExp",0, CONFIG_FILE)==1)
	{
		g_VisitorExp.bReasonExp=true;
	}
	if (GetPrivateProfileInt("TerminalSet", "bGoodsExp",0, CONFIG_FILE)==1)
	{
		g_VisitorExp.bGoodsExp=true;
	}
	if (GetPrivateProfileInt("TerminalSet", "bLefttimeExp",0, CONFIG_FILE)==1)
	{
		g_VisitorExp.bLefttimeExp=true;
	}
	if (GetPrivateProfileInt("TerminalSet", "bTotalPeopleExp",0, CONFIG_FILE)==1)
	{
		g_VisitorExp.bTotalPeopleExp=true;
	}
	if (GetPrivateProfileInt("TerminalSet", "bOfficerExp",0, CONFIG_FILE)==1)
	{
		g_VisitorExp.bOfficerExp=true;
	}
	if (GetPrivateProfileInt("TerminalSet", "bOfficeNameExp",0, CONFIG_FILE)==1)
	{
		g_VisitorExp.bOfficeNameExp=true;
	}
	if (GetPrivateProfileInt("TerminalSet", "bOfficePhoneExp",0, CONFIG_FILE)==1)
	{
		g_VisitorExp.bOfficePhoneExp=true;
	}
}

