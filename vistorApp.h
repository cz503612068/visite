#ifndef _VISTORAPP_H
#define _VISTORAPP_H

#include "StdAfx.h"
#include "CardAll.h"
#include "SocketManage.h"
#include "VistorRes.h"
#include "cardapi.h"
#include <vector>
#include <afxsock.h>
#include "sdtapi.h"
#include "cardapi.h"

#pragma comment(lib, "Drv_CardAll.lib")
#pragma comment(lib, "Sdtapi.lib")
#pragma comment(lib, "cardapi2.lib")

HWINDOW g_hWindow;
HWINDOW g_hMsgWindow;
HWINDOW g_hLogoutWindow; //ǩ��
HWINDOW g_hExitWindow;   //�˳�
HWINDOW g_hOfficialWin; //��ѯ��������Ϣ������
HWINDOW g_hQueryDetail; //��ѯ����
HELE g_hCurSelTab;		//��ǰѡ���TAB
IDCARD_ALL g_stIDCard;
CNovelSocket *g_tSock;
CAsyncSocket *g_UDPSock;
CString sCONNSTR_MYSQL;
CString sFolk[62];

int g_nCaptureTick = 30;          //����������ʱ��ʱ��Ϊ60s
int g_nIDType;					//֤��������
int g_nSqlConnectType;          //���ݿ����ӷ�ʽ
bool g_bStopID2;				//ֹͣ����֤��д����Ѱ��
bool g_bStopBarCode;			//ֹͣ����ɨ����
bool g_bScanerISOK;				//ɨ�����Ƿ�����
bool g_bQRcode;                 //��ά���ȡ��¼
char g_szHistVisitorName[50];   //��ʷ�����б�ÿ���
char g_szVisitorName[50];		//�����˵�����
char g_szOfficialIP[20];        //������IP
static char g_szXmlInfo[1024];
static char g_szComBarcode[10000];
char g_szRecord[32];            //��Ƶ�����������ñ�š�
char g_szICCode[10];            //�Ž�IC�����
char g_szStation[50];           //������
char g_szOfficialID[20];        //������ID
char g_szQRvisitime[512] = {'\0'}; //��ά��ԤԼ����ʱ��
bool g_bStartClipScreen;
bool g_bShowCap;                //����ͷ�Ƿ���ʾ
bool g_bCalled;                 //����ÿ���Ϣ
bool g_bFtpOpened;              //�Ƿ��ftp
bool g_bJob;                    //�Ƿ񱣴���־��Ϣ
bool g_bICReg;                  //������IC��ʶ�𱻷���
bool g_bVisitorType;             //�������� -> ��������

bool g_bSSO;                    //����������SSO  

bool g_bFaceCompare;
bool g_bExpireData;              //���ݹ�������

int g_nComboBoxCount;			//�б�������������
int g_nTableType;				//ɾ���Ȳ���ʱ�ı�����
int g_nID2ReadType;				//����֤�Ĳ�����ʽ(0�����Ƕ���֤��1���Ķ����Ķ��Ķ���֤��2��ɨ��Ķ���֤)
int g_nChaXunTongJiType;		//��ѯͳ��ҳ��Ĳ�������

POINT g_ptStart, g_ptEnd, g_ptPreEnd;
RECT g_recDrawing;
HDC g_compatibleHDC;
bool g_bRectStart;

HANDLE g_HWDThreadID2ID;   //����֤�߳�
HANDLE g_HWDThreadTime;
HANDLE g_ThreadSocketMutex;
HANDLE g_ThreadReadBarcode; //����ǩ���߳�
HANDLE g_ThreadID2IC;   //����֤IC���߳�
HANDLE g_hCom;  //ȫ�ֱ��������ھ��
HINSTANCE  g_hDllJBC; //JBC7200�Ž��ӿڿ���
HBRUSH g_brush;
HBITMAP g_hScreenBmp;
extern HBITMAP CopyDCToBitmap(LPRECT lpRect, HDC hSrcDC);

HXCGUI g_hWeb;
char g_szLastID2[32];
bool g_bPhoneIsBusy;
bool g_bPhoneIsIni;
bool g_bAcceptVisite;
bool g_bSendVidoe;                  //������Ƶ��ȷ�϶�
bool g_bPrintAfterAllow;
bool g_bRequestThreadIsExist;
bool g_bAutoCallOfficials;

CInternetSession* m_pInetSession;	//�Ự����
CFtpConnection* m_pFtpConnection;	//���Ӷ���
CFtpFileFind* m_pRemoteFinder;		//Զ�̲����ļ�����
CFileFind m_LocalFinder;			//���ز����ļ�����


//�˵���Ԫ�ؾ��
HELE g_hTab1 = NULL;
HELE g_hTab2 = NULL;
HELE g_hTab3 = NULL;
HELE g_hTab4 = NULL;
HELE g_hTab5 = NULL;
HELE g_hTab6 = NULL;
HELE g_hTab7 = NULL;
HELE g_hTab8 = NULL;
HELE g_hTab9 = NULL;
HELE g_hTab10 = NULL;

HELE g_hTab21 = NULL;
HELE g_hTab22 = NULL;
HELE g_hTab23 = NULL;
HELE g_hTab24 = NULL;
HELE g_hTab25 = NULL;
HELE g_hTab26 = NULL;
HELE g_hTab27 = NULL;
HELE g_hTab28 = NULL;

HELE g_hTab32 = NULL;
HELE g_hTab33 = NULL;
HELE g_hTab34 = NULL;
//HELE g_hTab35 = NULL;
HELE g_hTab36 = NULL;
HELE g_hTab37 = NULL;
HELE g_hTab38 = NULL;
HELE g_hTab39 = NULL;
//////////////////////////

//ҳ����ͼ��Ԫ�ؾ��
HELE g_hPage1 = NULL;
HELE g_hPage2 = NULL;
HELE g_hPage3 = NULL;
HELE g_hPage4 = NULL;
HELE g_hPage5 = NULL;
HELE g_hPage6 = NULL;
HELE g_hPage7 = NULL;
HELE g_hPage8 = NULL;
HELE g_hPage9 = NULL;
HELE g_hPage10 = NULL;
HELE g_hPage11 = NULL;
HELE g_hPage12 = NULL;
//////////////////////////
HELE htxtChaxunPwd= NULL;

//��Ҫ����Ϊ�����������ԴID����SetTitleFont����
int g_nTitleFontID[128] = {IDG_VISTOR, IDG_PASSPORT, IDG_VIDEO, IDG_VISTORREG, IDG_MASTERINFO, IDS_DEVINFO, IDL_VISTORLIST, \
						IDS_VISITORNAME, IDS_VISITORSEX, IDS_VISITORMOBILE, IDS_VISITORIDTYPE, IDS_VISITORIDNUM,\
						IDS_VISITORADDRESS, IDS_VISITORREASON, IDS_VISITORTOTAL, IDS_VISITORUNIT, IDS_CARNUM,\
						IDS_NAME, IDS_MASTERSEX, IDS_OFFICEPHONE, IDS_OFFICENAME, IDS_MOBILE, IDS_ZHIWU, IDS_OFFICENUM,\
						IDS_VISITORFOLK, IDS_VISITORTHING, IDS_FZJG, IDS_VISITTIMES, IDS_ROOMNUM, IDS_SYSINFO};
//��Ҫ�����������ԴID����SetID2InfoFont����
int g_nID2FontID[32] = {630, 631, 632, 633, 634, 635, 636, 637, 638, 639};
extern void SetTitleFont();
extern void SetID2InfoFont();
extern void SetTextFont();
////////////////////////////////

////////////////////////////////
extern void SetIDtypeComb(int nRes);
extern void SetVisitortypeComb(int nRes);
extern void SetSexInfo(int nRes);
extern void SetVisitorReadOnly(BOOL isReadOnly=false);
extern void SetOfficialReadOnly(BOOL isReadOnly=false);
extern void InitIDTypeAndFolk();
extern void ShowMainMenu(HELE hCurEle, BOOL bShow);
////////////////////////////////

//////����ͷ����///////////////
HWND g_hCapWnd;
bool g_bVideoOpened;		//����ͷ�Ƿ��Ѿ���
extern BOOL VideoPreview();
extern BOOL VideoPreview2();
extern BOOL StopVideoPreview();
extern BOOL TakePhoto(wchar_t *pszFileName);
extern BOOL TakePhoto2(wchar_t *pszFileName);
///////////////////////////////

//������Ϣ��(static)��ʾ����Ϣ
extern void SetInfo(int nResID, wchar_t *pwsInfo);
///////////////////////////////

//�����ı���߿���ɫ
extern void SetMainDlgEditBorder();
extern void SetOtherDlgEditBorder();
extern void SetOfficialEditBorder();
extern void SetBlackNameEditBorder();
///////////////////////////////

extern void ListTodayVistorsInfo();
//extern void SetVisitorInfo();
extern void SetVisitorInfo(IDCARD_ALL &visitorID2Card);
DWORD WINAPI ReadID2Thread2(LPVOID lpVoid);
DWORD WINAPI ReadID2Thread(LPVOID lpVoid);
DWORD WINAPI ReadBarCodeThread(LPVOID lpVoid);
DWORD WINAPI ReadBarCodeThread2(LPVOID lpVoid);
DWORD WINAPI FlashFrameThread(LPVOID lpVoid);
DWORD WINAPI ReadICThread(LPVOID lpVoid);


typedef struct tagVisitorInfo
{
	char szName[32];			//�ÿ�����
	char szSex[4];				//�ÿ��Ա�
	char szFolk[10];			//�ÿ�����
	char szPhone[16];			//�ÿ͵绰
	char szIDType[32];			//֤������
	char szID[30];				//֤������
	char szSignDepartment[60];	//�ÿ�֤����ǩ������
	char szWithGoods[60];		//Я������Ʒ
	char szReason[40];			//����ԭ��
	char szAddr[100];			//�ÿ͵�ַ
	char szTotalPeople[8];		//��������
	char szUnit[100];			//�ÿ͵�λ
	char szJob[50];             //�ÿ�ְҵ   ������ 20160808 cc
	char szCarNum[20];			//�ÿͳ��ƺ���
	char szBarCode[32];			//���ʵ�������
	char szToOfficialName[32];	//������������
	char szToOfficialPhone[16]; //�����˵绰����
	long lVisiteTime;			//����ʱ��
	long lLeftTime;				//�뿪ʱ��
	char szVisitStation[32];	//�ÿ;����Ǹ��Ÿڽ���
	char szUserVideoPic[256];	//�ÿ���Ƶ��Ƭ�Ĵ�ŵ�ַ
	char szUserIDPic[256];		//�ÿ�֤����Ƭ�Ĵ�ŵ�ַ
	char szUserHeadPic[256];	//�ÿ�ͷ����Ƭ�Ĵ�ŵ�ַ
	char szUserIDFullPic[256];  //֤��ɨ��ͼƬȫͼ

	char szToOfficeName[64];	//�����ʲ���
	char szToRoom[32];			//�����˷����
	char szValidDate[64];		//��Ч����
	char szID2Flag[8];			//����֤�Ķ�д��ʽ(0�����Ƕ���֤��1���Ķ����Ķ��Ķ���֤��2��ɨ��Ķ���֤)
	char szBirth[32];			//����
	int  nStatus;               //״̬
	char szNote[200];           //����˵��
	char szType[50];            //�ÿ�����

	char szToSSO[30];
} VISITORINFO;

VISITORINFO g_Printvisitor;

typedef struct tagPicPath
{
	char szUserVideoPic[256];	//�ÿ���Ƶ��Ƭ�Ĵ�ŵ�ַ
	char szUserIDPic[256];		//�ÿ�֤����Ƭ�Ĵ�ŵ�ַ
	char szUserHeadPic[256];	//�ÿ�ͷ����Ƭ�Ĵ�ŵ�ַ
	char szUserIDFullPic[256];  //֤��ɨ��ͼƬȫͼ
	//char szUserBarCodePic[256]; //����
}PICPATH;

PICPATH g_VisitorPic;

typedef struct tagOfficialInfo
{
	char szName[32];
	char szSex[8];
	char szOfficePhone[32];
	char szMobile[32];
	char szOfficeName[32];
	char szOfficeAddr[64];
	char szOfficialDuty[32];
	char szRoom[16];
	char szIP[20];
	char szNO[50];
	char szSSO[30];
} OFFICIALINFO;
typedef struct tagVISITORExp
{
	bool bStationExp;
	bool bVisitorExp;
    bool bVSexExp;
	bool bVFolkExp;
	bool bIDTypeExp;
	bool bIDExp;
	bool bAddrExp;
	bool bSigndepartmentExp;
	bool bUnitExp;
	bool bTelExp;
	bool bCarNumExp;
	bool bVisittimeExp;
	bool bReasonExp;
	bool bGoodsExp;
	bool bLefttimeExp;
	bool bTotalPeopleExp;
	bool bOfficerExp;
	bool bOfficeNameExp;
	bool bOfficePhoneExp;
} VISITORExp;
VISITORExp g_VisitorExp;

typedef struct tagPrintTipsCfg
{
    enum PRINT_TIPS_TYPE ePrintType;
    bool bHideLast4ID;
	bool bHideGoodsPrint;
	bool bTotalPeoplePrint;
	bool bToRoomPrint;
	char szUnitName[255];
} PrintTipsCfg;
PrintTipsCfg g_PrintTipsCfg;

typedef struct _j7100_registry_
{
    char 	card1[16], 			//����1
        	card2[16], 			//��Ϊ����ע�ᣬ����Ҫ
        	psw[8];    			//����ע��ʱ���Ž�����
     int  	security,  
        	team;       		//�ֿ��˷���������, ����16��, ֵ��Χ:0-15.
    char 	acce[32];
    int 		year, month,day;	//����ע�����Ч��
}J7100REGISTRY;
typedef struct _devicewatch_//ʵʱ�������
{
    char card[10],      //ˢ������
    mgs[64],    //�����������.
    date[12],   //ˢ������
    time[10];   //ˢ��ʱ��
    int card_source;    //ˢ��������.
    int err;    //����ֵ.=0�Ϸ�����.<0�Ƿ�����.
}DeviceWatch;




extern BOOL SaveVisitorInfo(VISITORINFO &st);
extern BOOL AutoCheckCompanyName(char *szCompany);
extern BOOL AutoCheckJob(char *szJob);

extern BOOL SaveOfficialInfo(OFFICIALINFO &official);
extern void MakeFolder(const char *pszFolder);
extern void GetDate(char *pszDate);
extern void CreateWorkFolder();
extern void GetDayFolder(char *pszPath);
extern BOOL VisitorLogOut(char *pszBarCode);
extern void VisitorTotalInfo(char *pszInfo);
extern void VisitorTotalTimes(char *pszName, char *pszOut);
extern void SysSetWin();
extern void SetIDType(int nResID);
extern void SetHeadPic(short n);
extern BOOL PrintReceipt(const VISITORINFO &visitor);
extern void SetSysInfo();
extern void TrackDraw();
extern BOOL QueryEmployer(HELE hEle);

extern HBITMAP CopyScreenToBitmap(LPRECT lpRect);
extern BOOL ExecPro(char * cmdline, BOOL bAsyncFlag, PROCESS_INFORMATION *ppi);
extern BOOL getYueJSon(char *js,VISITORINFO &visitor);

DWORD WINAPI GetDataThread(LPVOID lpVoid);
DWORD WINAPI SendTimeThread(LPVOID lpVoid);
DWORD WINAPI ReadChannelRegThread(LPVOID lpVoid);
extern BOOL SendKeyBdAppData(char *pszData);
extern BOOL SendBackScreenAppData(char *pszData);
extern BOOL GetEditData(int nType, char *pszOut);
extern void SetOfficialInfo();
extern BOOL CreateReceiptHtml(const VISITORINFO &visitor, char *pszFile);
extern BOOL CreateReceiptHtml2(const VISITORINFO &visitor, char *pszFile);  //�����͵�
extern BOOL CreateReceiptTipsPB(const VISITORINFO &visitor);   //ͨ��PB��ӡƾ��
extern BOOL CreateVisitorInfTipsUptoServer(const VISITORINFO &visitor); //�����ϴ��ķÿ���Ϣ

extern BOOL CreateNoFrameReceiptHtml(const VISITORINFO &visitor, char *pszFile);

extern BOOL CreateVisitorRequestHtml(const VISITORINFO &visitor, char *pszFile); //�����ն˵Ļ������
extern void PrintHtmlPage(char *pszURL);
extern BOOL SaveAndPrint();
extern void SetStationInfo(int nResID);

//extern long CALLBACK SendRequestToOfficial(char *pszFile);
extern long SendRequestToOfficial(char *pszFile);
extern void UpdateAddressTable();
extern void SetVisitorInfo(const VISITORINFO &visitor);
extern void SetOfficialInfo(const OFFICIALINFO &official);

extern BOOL CALLBACK OnVisitorSetFocus2(HELE hEle, UINT flags, POINT *pPt);

extern BOOL CALLBACK OnVisitorSetFocus(HELE hEle, HELE hEventEle);
//extern BOOL CALLBACK OnOfficialSetFocus(HELE hEle, HELE hEventEle);
extern BOOL CALLBACK OnOfficialSetFocus(HELE hEle, UINT flags, POINT *pPt);
extern BOOL CALLBACK OnOfficial1SetFocus(HELE hEle, HELE hEventEle);
extern BOOL CALLBACK OnBlackNameSetFocus(HELE hEle, HELE hEventEle);
extern BOOL CALLBACK OnOtherSetFocus(HELE hEle, HELE hEventEle);
extern BOOL CALLBACK OnHideKeyBdSetFocus(HELE hEle, HELE hEventEle);
extern BOOL CALLBACK OnOfficialKillFocus(HELE hEle, HELE hEventEle);
extern BOOL CALLBACK OnClearPWD(HELE hEle, HELE hEventEle);

extern BOOL CALLBACK OnShowIDType(HELE hEle, HELE hEventEle);
extern BOOL CALLBACK OnShowVisitorType(HELE hEle, HELE hEventEle);
extern BOOL CALLBACK OnShowFolk(HELE hEle, HELE hEventEle);

extern BOOL CALLBACK OnIDTypeCombSelect(HELE hEle, HELE hEventEle, int nIndex);
extern BOOL CALLBACK OnChaxunTongjiList(HELE hEle, HELE hEventEle, int nIndex);
extern BOOL CALLBACK OnTongjiChaxunReturn(HELE hEle, HELE hEventEle);
extern BOOL CALLBACK OnTongji(HELE hEle, HELE hEventEle);		//ͳ�Ʋ�ѯҳ���ͳ�ư�ť
extern BOOL CALLBACK OnChaxun(HELE hEle, HELE hEventEle);		//ͳ�Ʋ�ѯҳ��Ĳ�ѯ��ť
extern BOOL CALLBACK OnExportVisitor(HELE hEle, HELE hEventEle);

extern BOOL CALLBACK OnDocComplete(HXCGUI hWebBrowser,in_ IDispatch *pDisp,in_ wchar_t *pUrl);
extern BOOL CALLBACK OnSaveMustInput(HELE hEle, HELE hEventEle);
extern BOOL CALLBACK OnSetBlackName(HELE hEle, HELE hEventEle);
extern BOOL CALLBACK OnDelBlackName(HELE hEle, HELE hEventEle);
extern BOOL CALLBACK OnCloseBlackName(HELE hEle, HELE hEventEle);

extern BOOL CALLBACK OnOutResponse(HWINDOW hWindow, WPARAM wParam, LPARAM lParam);  //��ʾ�����Ϣ
extern BOOL CALLBACK OnLogOutBtnClick(HELE hEle, HELE hEventEle); //�ֶ�ǩ�뵥����¼

extern BOOL CALLBACK OnLSBtnClick(HELE hEle, HELE hEventEle);
extern BOOL CALLBACK OnXCBtnClick(HELE hEle, HELE hEventEle);
extern BOOL CALLBACK OnPTBtnClick(HELE hEle, HELE hEventEle);
extern BOOL CALLBACK OnQTBtnClick(HELE hEle, HELE hEventEle);

extern BOOL CALLBACK OnReCallBtnClick(HELE hEle, HELE hEventEle);
extern BOOL CALLBACK OnAllReCallBtnClick(HELE hEle, HELE hEventEle);

extern BOOL CALLBACK OnAllLogOutBtnClick(HELE hEle, HELE hEventEle);//ȫ��ǩ��
extern BOOL CALLBACK OnCloseLogOutBtnClick(HELE hEle, HELE hEventEle); //�ر�����Ի���
extern BOOL CALLBACK OnOKBtnClick(HELE hEle, HELE hEventEle);      //�����ѯ��������ȷ��
extern BOOL CALLBACK OnOKSysBtnClick(HELE hEle, HELE hEventEle);   //�����̨����ҳ��
 
extern BOOL CALLBACK OnCloseDevice(HELE hEle, HELE hEventEle);  //�رյ����豸
extern BOOL CALLBACK OnCloseApp(HELE hEle, HELE hEventEle); //�رճ���

extern BOOL CALLBACK OnLoginOK(HELE hEle, HELE hEventEle);
extern BOOL CALLBACK OnLoginNo(HELE hEle, HELE hEventEle);
extern BOOL CALLBACK OnShowPwdKeyBd(HELE hEle, HELE hEventEle);
extern BOOL CALLBACK OnClosePwdKeybd(HELE hEle, HELE hEventEle);
extern BOOL CALLBACK OnImportXls(HELE hEle, HELE hEventEle);
extern BOOL CALLBACK OnCallPhone(HELE hEle, HELE hEventEle);
extern BOOL CALLBACK OnCallMobile(HELE hEle, HELE hEventEle);
extern BOOL CALLBACK OnUsbPhoneEvent(HWINDOW hWindow, WPARAM wParam, LPARAM lParam);
extern BOOL CALLBACK OnHangUp(HELE hEle, HELE hEventEle);
extern BOOL CALLBACK OnExportXls(HELE hEle, HELE hEventEle);
extern BOOL CALLBACK OnSelectOfficialListClick(HELE hEle, HELE hEventEle, int nIndex);
extern BOOL CALLBACK OnSelectIDType(HELE hEle, HELE hEventEle, int nIndex);
extern BOOL CALLBACK OnSelectVisitorType(HELE hEle, HELE hEventEle, int nIndex);
extern BOOL CALLBACK OnSelectFolk(HELE hEle, HELE hEventEle, int nIndex);

extern BOOL CALLBACK OnImportOfficial(HELE hEle, HELE hEventEle);
extern BOOL CALLBACK OnExportOfficial(HELE hEle, HELE hEventEle);
extern BOOL CALLBACK OnSaveOfficial(HELE hEle, HELE hEventEle);
extern BOOL CALLBACK OnOfficialList(HELE hEle, HELE hEventEle, int nIndex);

extern BOOL CALLBACK OnICUnReg(HELE hEle, HELE hEventEle);  //�����󣬽��IC����

extern BOOL CALLBACK OnImportBlackName(HELE hEle, HELE hEventEle);
extern BOOL CALLBACK OnExportBlackName(HELE hEle, HELE hEventEle);
extern BOOL CALLBACK OnBlackNameList(HELE hEle, HELE hEventEle, int nIndex);

extern BOOL CALLBACK OnImportGoods(HELE hEle, HELE hEventEle);
extern BOOL CALLBACK OnExportGoods(HELE hEle, HELE hEventEle);
extern BOOL CALLBACK OnSaveGoods(HELE hEle, HELE hEventEle);
extern BOOL CALLBACK OnGoodsReasonStationUser(HELE hEle, HELE hEventEle, int nIndex);
	
extern BOOL CALLBACK OnImportReason(HELE hEle, HELE hEventEle);
extern BOOL CALLBACK OnExportReason(HELE hEle, HELE hEventEle);
extern BOOL CALLBACK OnSaveReason(HELE hEle, HELE hEventEle);
	
extern BOOL CALLBACK OnImportStation(HELE hEle, HELE hEventEle);
extern BOOL CALLBACK OnExportStation(HELE hEle, HELE hEventEle);
extern BOOL CALLBACK OnSaveStation(HELE hEle, HELE hEventEle);
	
extern BOOL CALLBACK OnImportCZY(HELE hEle, HELE hEventEle);
extern BOOL CALLBACK OnExportCZY(HELE hEle, HELE hEventEle);
extern BOOL CALLBACK OnSaveCZY(HELE hEle, HELE hEventEle);

extern BOOL CALLBACK OnImportOffice(HELE hEle, HELE hEventEle);
extern BOOL CALLBACK OnExportOffice(HELE hEle, HELE hEventEle);
extern BOOL CALLBACK OnSaveOffice(HELE hEle, HELE hEventEle);
extern BOOL CALLBACK OnSaveLevel(HELE hEle, HELE hEventEle);
extern BOOL CALLBACK OnOfficeLevel(HELE hEle, HELE hEventEle, int nIndex);

extern BOOL CALLBACK OnScanID2(HELE hEle, HELE hEventEle);			//ɨ�����֤
extern BOOL CALLBACK OnScanID(HELE hEle, HELE hEventEle);			//ɨ��һ�����֤
extern BOOL CALLBACK OnScanLawyer(HELE hEle, HELE hEventEle);		//ɨ����ʦ֤
extern BOOL CALLBACK OnScanPassPort(HELE hEle, HELE hEventEle);		//ɨ�軤��
extern BOOL CALLBACK OnScanToHKPassPort(HELE hEle, HELE hEventEle);	//ɨ��۰�ͨ��֤
extern BOOL CALLBACK OnScanHKID(HELE hEle, HELE hEventEle);			//ɨ�����֤
extern BOOL CALLBACK OnScanTaiWanPassPort(HELE hEle, HELE hEventEle);	//ɨ��̨��֤
extern BOOL CALLBACK OnScanOther(HELE hEle, HELE hEventEle);			//ɨ������֤��

extern BOOL CALLBACK OnScanBusinessCard(HELE hEle, HELE hEventEle);         //ɨ����Ƭ
extern BOOL CALLBACK OnScanDriverLicense(HELE hEle, HELE hEventEle);    //ɨ���ʻ֤
extern BOOL CALLBACK OnReturnMain(HELE hEle, HELE hEventEle);			//����

extern BOOL CALLBACK OnOfficialSet(HELE hEle, HELE hEventEle);		//������Ա��Ϣҳ��
extern BOOL CALLBACK OnBlackSet(HELE hEle, HELE hEventEle);			//���Ӻ�������Ϣҳ��
extern BOOL CALLBACK OnGoodsSet(HELE hEle, HELE hEventEle);			//������Ʒ��Ϣҳ��
extern BOOL CALLBACK OnSysConfig(HELE hEle, HELE hEventEle);			//ϵͳ��������ҳ��
extern BOOL CALLBACK OnBaseInfo(HELE hEle, HELE hEventEle);			//������Ϣҳ��
extern BOOL CALLBACK OnReturnSet(HELE hEle, HELE hEventEle);			//ϵͳ����(����ά��)ҳ���ϵķ���
extern BOOL CALLBACK OnQueryEmployerForList(HELE hEle, HELE hEventEle);

extern BOOL CALLBACK OnSaveOther(HELE hEle, HELE hEventEle); //ϵͳ����ҳ��������ı�������
extern BOOL CALLBACK OnConfigSave(HELE hEle, HELE hEventEle);
extern void CALLBACK TimerMsgBox(HWND   hwnd,UINT   message,UINT   iTIMERID,DWORD   dwTIMER);

extern BOOL CALLBACK OnSelectCombox(HELE hEle,HELE hEventEle);


extern LRESULT CALLBACK FrameCallBack(HWND hWnd, LPVIDEOHDR lpVHdr);

extern BOOL CALLBACK OnCompareFace(HWINDOW hWindow, WPARAM wParam, LPARAM lParam); //����ʶ��Ա�

extern BOOL CheckIsBlackName2(VISITORINFO &visitor);
extern BOOL CheckBlackName(const VISITORINFO &visitor);
extern BOOL CheckBlackNameNotes(const VISITORINFO &visitor,char *note);
extern void GetDate_Out(char *pszDate);
extern BOOL StartKeybd();
extern BOOL StartPrintTipsPB();
extern BOOL StartUptoServer();
extern BOOL CallLeft2Server(char *szName,char *szID,char *visitetime,char *lefttime,char *barcode); //Զ��ǩ��
extern BOOL BuildQRGenerator(char *szQRValue); //��ά��������
extern BOOL CallFS533Bin(char *szParam); //����fs533ɨ����

extern BOOL StartZTScanner();
extern BOOL StartYunMaiOCR();
extern BOOL StartFaceScaner();
extern BOOL StartFaceCompare();

extern int GetTcpBarAndUpVisitorInfo();
extern int  GetTcpCardScan(int type,IDCARD_ALL &visitorID2Card); //����TCP��ʽɨ����

extern bool getCardXmlInfo(char *xmlbuf,IDCARD_ALL &visitorID2Card);
extern bool getVisitor2Xml(char *xml,VISITORINFO &visitor,char *ip);

extern BOOL GetBarCodePic(char *szBarCode);

extern BOOL StartBackScreen();
extern BOOL BackScreenCheckInfo();
extern BOOL BackScreenPlayADV();
extern BOOL BackScreenSelectOfficial();

extern BOOL MoveTopSoftKeybd();
extern BOOL MoveBottomSoftKeybd();
extern BOOL ShowStandSoftKeybd();
extern BOOL ShowVisitorSoftKeybd();
extern BOOL ShowOfficialSoftKeybd();
extern BOOL HideSoftKeybd();

extern BOOL ShowCompanybd();
extern BOOL ShowReasonbd();
extern BOOL ShowGoodsbd();
extern BOOL ShowRoombd();
extern BOOL ShowSSObd();
extern BOOL ShowDutybd();
extern BOOL ShowOfficePhonebd();
extern BOOL ShowOfficeNamebd();
extern BOOL ShowOfficialbd();

extern BOOL ShowID2(BOOL bShow, const VISITORINFO &visitor);
extern BOOL CheckID2(const VISITORINFO &visitor);
extern BOOL ShowID2_CX(HWINDOW hWindow, BOOL bShow, const VISITORINFO &visitor);
extern BOOL EmptyVisitorPara();
extern BOOL EmptyOfficialPara();
extern BOOL CheckAllMustInput();
extern BOOL CheckIsBlackName(VISITORINFO &visitor);
extern BOOL PrintVisitorNoteAgain();
extern void SetUserRight();
extern BOOL QueryCzy();
extern long GetFldStr(char* pszInbuf, const char* pszFilter, int iNo, char* pszOutBuf);
extern BOOL TongJiInfo();
extern void HideTaskBar(BOOL bHide);
extern void ChaXunToDayInfo();
extern int  CheckVisitor(const VISITORINFO &visitor);
extern void InitData();
extern void InitVisitorData();
extern bool SetLoginUser();
extern void ShowMsgPage(int nResID, wchar_t *pwsInfo, bool bShow);
extern BOOL PiPeiEmployer();
extern BOOL DialUsbPhone(const char *pszNum);
extern BOOL QueryBlack();
extern BOOL CloseSoftKeybd();
extern BOOL CloseBackScreen();
extern BOOL CloseXYB();
extern int  RePlayCapture();
extern void ReOpenUSBCamera();

extern BOOL ImportIDType(char *pszFileName);
extern BOOL ImportOfficial(char *pszFileName);
extern BOOL ImportOffice(char *pszFileName);
extern BOOL ImportUser(char *pszFileName);
extern BOOL ImportWithGoods(char *pszFileName);
extern BOOL ImportReason(char *pszFileName);
extern BOOL ImportStation(char *pszFileName);
extern BOOL ImportLevel(char *pszFileName);
extern BOOL ImportStation(char *pszFileName);
extern BOOL ImportBlackName(char *pszFileName);

extern BOOL ExportOfficial(char *pszFileName);
extern BOOL ExportBlackName(char *pszFileName);
extern BOOL ExportWithGoods(char *pszFileName);
extern BOOL ExportReason(char *pszFileName);
extern BOOL ExportStation(char *pszFileName);
extern BOOL ExportOffice(char *pszFileName);
extern BOOL ExportUser(char *pszFileName);
extern BOOL ExportLevel(char *pszFileName);
extern BOOL ExportIDType(char *pszFileName);

extern BOOL ExportVisitor(char *pszFileName);
extern void InitalVisitorExp();    //��ʼ��������͵���ʽ
extern void InitalPrintTipsCfg();  //��ʼ����͵���ʽ

extern BOOL OpenXlsDlg(char *szFileName);   //��XLS�ļ�·���Ի���
extern BOOL SaveXlsDlg(CString &m_pathDir);   //����XLS�ļ�����·��

extern BOOL OpenRegCardCom(int nPort);
extern BOOL OperateRegCard(char* szIp,char* szCard,int nOperateType);

extern void EnableScanBtns(BOOL isEnable); //ɨ��ʱ֤����ť����

extern void FtpConnect();

void YUVToRGB(int y,int u,int v,int &r,int &g,int &b);
/*
extern "C" _declspec(dllimport) void _stdcall DrawBarCode(HDC hDC, char *pszCode, char *pszFile);
extern void GetBarCode(char *szBarCode);
extern "C" _declspec(dllimport) bool _stdcall StartClip(HDC hDC, const POINT &point);
extern "C" _declspec(dllimport) HBITMAP _stdcall StopClip(HDC hDC, const POINT &point);
extern "C" _declspec(dllimport) bool _stdcall SaveBitmapToFile(HBITMAP hBitmap, char *pszFile);
extern "C" _declspec(dllimport) void _stdcall EndClip();
extern "C" _declspec(dllimport) void _stdcall TrackDraw(HDC hDC, const POINT &point);
*/
#endif			//end _VISTORAPP_H
