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
HWINDOW g_hLogoutWindow; //签离
HWINDOW g_hExitWindow;   //退出
HWINDOW g_hOfficialWin; //查询被访人信息，详情
HWINDOW g_hQueryDetail; //查询详情
HELE g_hCurSelTab;		//当前选择的TAB
IDCARD_ALL g_stIDCard;
CNovelSocket *g_tSock;
CAsyncSocket *g_UDPSock;
CString sCONNSTR_MYSQL;
CString sFolk[62];

int g_nCaptureTick = 30;          //设置视屏计时器时长为60s
int g_nIDType;					//证件的种类
int g_nSqlConnectType;          //数据库连接方式
bool g_bStopID2;				//停止二代证读写器的寻卡
bool g_bStopBarCode;			//停止条码扫描七
bool g_bScanerISOK;				//扫描仪是否正常
char g_szHistVisitorName[50];   //历史来访列表访客名
char g_szVisitorName[50];		//来访人的姓名
char g_szOfficialIP[20];        //被访人IP
static char g_szXmlInfo[1024];
char g_szRecord[32];            //视频发送请求“来访编号”
char g_szICCode[10];            //门禁IC物理号
char g_szStation[50];           //门卫名
char g_szOfficialID[20];        //被访人ID
bool g_bStartClipScreen;
bool g_bShowCap;                //摄像头是否显示
bool g_bCalled;                 //保存访客信息
bool g_bFtpOpened;              //是否打开ftp
bool g_bJob;                    //是否保存日志信息
bool g_bICReg;                  //大华需求IC卡识别被访人
bool g_bVisitorType;             //来访人数 -> 来访类型

bool g_bSSO;                    //大华特殊需求SSO  

bool g_bFaceCompare;

int g_nComboBoxCount;			//列表框的总数据行数
int g_nTableType;				//删除等操作时的表类型
int g_nID2ReadType;				//二代证的操作方式(0：不是二代证；1：阅读器阅读的二代证；2：扫描的二代证)
int g_nChaXunTongJiType;		//查询统计页面的操作类型

POINT g_ptStart, g_ptEnd, g_ptPreEnd;
RECT g_recDrawing;
HDC g_compatibleHDC;
bool g_bRectStart;

HANDLE g_HWDThreadID2ID;   //二代证线程
HANDLE g_HWDThreadTime;
HANDLE g_ThreadSocketMutex;
HANDLE g_ThreadReadBarcode; //条码签离线程
HANDLE g_ThreadID2IC;   //二代证IC卡线程
HANDLE g_hCom;  //全局变量，串口句柄
HINSTANCE  g_hDllJBC; //JBC7200门禁接口库句柄
HBRUSH g_brush;
HBITMAP g_hScreenBmp;
extern HBITMAP CopyDCToBitmap(LPRECT lpRect, HDC hSrcDC);

HXCGUI g_hWeb;
char g_szLastID2[32];
bool g_bPhoneIsBusy;
bool g_bPhoneIsIni;
bool g_bAcceptVisite;
bool g_bSendVidoe;                  //发送视频到确认端
bool g_bPrintAfterAllow;
bool g_bRequestThreadIsExist;
bool g_bAutoCallOfficials;

CInternetSession* m_pInetSession;	//会话对象
CFtpConnection* m_pFtpConnection;	//连接对象
CFtpFileFind* m_pRemoteFinder;		//远程查找文件对象
CFileFind m_LocalFinder;			//本地查找文件对象


//菜单的元素句柄
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

//页面视图的元素句柄
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

//需要设置为标题字体的资源ID，由SetTitleFont调用
int g_nTitleFontID[128] = {IDG_VISTOR, IDG_PASSPORT, IDG_VIDEO, IDG_VISTORREG, IDG_MASTERINFO, IDS_DEVINFO, IDL_VISTORLIST, \
						IDS_VISITORNAME, IDS_VISITORSEX, IDS_VISITORMOBILE, IDS_VISITORIDTYPE, IDS_VISITORIDNUM,\
						IDS_VISITORADDRESS, IDS_VISITORREASON, IDS_VISITORTOTAL, IDS_VISITORUNIT, IDS_CARNUM,\
						IDS_NAME, IDS_MASTERSEX, IDS_OFFICEPHONE, IDS_OFFICENAME, IDS_MOBILE, IDS_ZHIWU, IDS_OFFICENUM,\
						IDS_VISITORFOLK, IDS_VISITORTHING, IDS_FZJG, IDS_VISITTIMES, IDS_ROOMNUM, IDS_SYSINFO};
//需要设置字体的资源ID，由SetID2InfoFont调用
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

//////摄像头操作///////////////
HWND g_hCapWnd;
bool g_bVideoOpened;		//摄像头是否已经打开
extern BOOL VideoPreview();
extern BOOL VideoPreview2();
extern BOOL StopVideoPreview();
extern BOOL TakePhoto(wchar_t *pszFileName);
extern BOOL TakePhoto2(wchar_t *pszFileName);
///////////////////////////////

//设置消息框(static)显示的消息
extern void SetInfo(int nResID, wchar_t *pwsInfo);
///////////////////////////////

//设置文本框边框颜色
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
	char szName[32];			//访客姓名
	char szSex[4];				//访客性别
	char szFolk[10];			//访客民族
	char szPhone[16];			//访客电话
	char szIDType[32];			//证件类型
	char szID[30];				//证件号码
	char szSignDepartment[60];	//访客证件的签发机关
	char szWithGoods[60];		//携带的物品
	char szReason[40];			//来访原因
	char szAddr[100];			//访客地址
	char szTotalPeople[8];		//来访人数
	char szUnit[100];			//访客单位
	char szJob[50];             //访客职业   大华需求 20160808 cc
	char szCarNum[20];			//访客车牌号码
	char szBarCode[32];			//访问单的条码
	char szToOfficialName[32];	//被访问人姓名
	long lVisiteTime;			//来访时间
	long lLeftTime;				//离开时间
	char szVisitStation[32];	//访客经由那个门岗进入
	char szUserVideoPic[256];	//访客视频照片的存放地址
	char szUserIDPic[256];		//访客证件照片的存放地址
	char szUserHeadPic[256];	//访客头部照片的存放地址
	char szUserIDFullPic[256];  //证件扫描图片全图

	char szToOfficeName[64];	//被访问部门
	char szToRoom[32];			//被访人房间号
	char szValidDate[64];		//有效期限
	char szID2Flag[8];			//二代证的读写方式(0：不是二代证；1：阅读器阅读的二代证；2：扫描的二代证)
	char szBirth[32];			//生日
	int  nStatus;               //状态
	char szNote[200];           //其他说明
	char szType[50];            //访客类型

	char szToSSO[30];
} VISITORINFO;

VISITORINFO g_Printvisitor;

typedef struct tagPicPath
{
	char szUserVideoPic[256];	//访客视频照片的存放地址
	char szUserIDPic[256];		//访客证件照片的存放地址
	char szUserHeadPic[256];	//访客头部照片的存放地址
	char szUserIDFullPic[256];  //证件扫描图片全图
	//char szUserBarCodePic[256]; //条码
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
    char 	card1[16], 			//卡号1
        	card2[16], 			//如为单卡注册，不需要
        	psw[8];    			//单卡注册时的门禁密码
     int  	security,  
        	team;       		//持卡人分组管理序号, 最多分16组, 值范围:0-15.
    char 	acce[32];
    int 		year, month,day;	//该条注册的有效期
}J7100REGISTRY;
typedef struct _devicewatch_//实时监控数据
{
    char card[10],      //刷卡卡号
    mgs[64],    //监控数据描述.
    date[12],   //刷卡日期
    time[10];   //刷卡时间
    int card_source;    //刷卡卡类型.
    int err;    //错误值.=0合法数据.<0非法数据.
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

DWORD WINAPI GetDataThread(LPVOID lpVoid);
DWORD WINAPI SendTimeThread(LPVOID lpVoid);
DWORD WINAPI ReadChannelRegThread(LPVOID lpVoid);
extern BOOL SendKeyBdAppData(char *pszData);
extern BOOL SendBackScreenAppData(char *pszData);
extern BOOL GetEditData(int nType, char *pszOut);
extern void SetOfficialInfo();
extern BOOL CreateReceiptHtml(const VISITORINFO &visitor, char *pszFile);
extern BOOL CreateReceiptHtml2(const VISITORINFO &visitor, char *pszFile);  //简洁版会客单
extern BOOL CreateReceiptTipsPB(const VISITORINFO &visitor);   //通过PB打印凭条
extern BOOL CreateVisitorInfTipsUptoServer(const VISITORINFO &visitor); //创建上传的访客信息

extern BOOL CreateNoFrameReceiptHtml(const VISITORINFO &visitor, char *pszFile);

extern BOOL CreateVisitorRequestHtml(const VISITORINFO &visitor, char *pszFile); //发到终端的会客请求单
extern void PrintHtmlPage(char *pszURL);
extern BOOL SaveAndPrint();
extern void SetStationInfo(int nResID);

extern long CALLBACK SendRequestToOfficial(char *pszFile);
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
extern BOOL CALLBACK OnTongji(HELE hEle, HELE hEventEle);		//统计查询页面的统计按钮
extern BOOL CALLBACK OnChaxun(HELE hEle, HELE hEventEle);		//统计查询页面的查询按钮
extern BOOL CALLBACK OnExportVisitor(HELE hEle, HELE hEventEle);

extern BOOL CALLBACK OnDocComplete(HXCGUI hWebBrowser,in_ IDispatch *pDisp,in_ wchar_t *pUrl);
extern BOOL CALLBACK OnSaveMustInput(HELE hEle, HELE hEventEle);
extern BOOL CALLBACK OnSetBlackName(HELE hEle, HELE hEventEle);
extern BOOL CALLBACK OnDelBlackName(HELE hEle, HELE hEventEle);
extern BOOL CALLBACK OnCloseBlackName(HELE hEle, HELE hEventEle);

extern BOOL CALLBACK OnOutResponse(HWINDOW hWindow, WPARAM wParam, LPARAM lParam);  //显示外接消息
extern BOOL CALLBACK OnLogOutBtnClick(HELE hEle, HELE hEventEle); //手动签离单条记录

extern BOOL CALLBACK OnLSBtnClick(HELE hEle, HELE hEventEle);
extern BOOL CALLBACK OnXCBtnClick(HELE hEle, HELE hEventEle);
extern BOOL CALLBACK OnPTBtnClick(HELE hEle, HELE hEventEle);
extern BOOL CALLBACK OnQTBtnClick(HELE hEle, HELE hEventEle);

extern BOOL CALLBACK OnReCallBtnClick(HELE hEle, HELE hEventEle);
extern BOOL CALLBACK OnAllReCallBtnClick(HELE hEle, HELE hEventEle);

extern BOOL CALLBACK OnAllLogOutBtnClick(HELE hEle, HELE hEventEle);//全部签离
extern BOOL CALLBACK OnCloseLogOutBtnClick(HELE hEle, HELE hEventEle); //关闭自身对话框
extern BOOL CALLBACK OnOKBtnClick(HELE hEle, HELE hEventEle);      //进入查询界面密码确认
extern BOOL CALLBACK OnOKSysBtnClick(HELE hEle, HELE hEventEle);   //进入后台设置页面
 
extern BOOL CALLBACK OnCloseDevice(HELE hEle, HELE hEventEle);  //关闭电脑设备
extern BOOL CALLBACK OnCloseApp(HELE hEle, HELE hEventEle); //关闭程序

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

extern BOOL CALLBACK OnICUnReg(HELE hEle, HELE hEventEle);  //大华需求，解除IC卡绑定

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

extern BOOL CALLBACK OnScanID2(HELE hEle, HELE hEventEle);			//扫描二代证
extern BOOL CALLBACK OnScanID(HELE hEle, HELE hEventEle);			//扫描一代身份证
extern BOOL CALLBACK OnScanLawyer(HELE hEle, HELE hEventEle);		//扫描律师证
extern BOOL CALLBACK OnScanPassPort(HELE hEle, HELE hEventEle);		//扫描护照
extern BOOL CALLBACK OnScanToHKPassPort(HELE hEle, HELE hEventEle);	//扫描港澳通行证
extern BOOL CALLBACK OnScanHKID(HELE hEle, HELE hEventEle);			//扫描回乡证
extern BOOL CALLBACK OnScanTaiWanPassPort(HELE hEle, HELE hEventEle);	//扫描台胞证
extern BOOL CALLBACK OnScanOther(HELE hEle, HELE hEventEle);			//扫描其他证件

extern BOOL CALLBACK OnScanBusinessCard(HELE hEle, HELE hEventEle);         //扫描名片
extern BOOL CALLBACK OnScanDriverLicense(HELE hEle, HELE hEventEle);    //扫描驾驶证
extern BOOL CALLBACK OnReturnMain(HELE hEle, HELE hEventEle);			//返回

extern BOOL CALLBACK OnOfficialSet(HELE hEle, HELE hEventEle);		//增加人员信息页面
extern BOOL CALLBACK OnBlackSet(HELE hEle, HELE hEventEle);			//增加黑名单信息页面
extern BOOL CALLBACK OnGoodsSet(HELE hEle, HELE hEventEle);			//增加物品信息页面
extern BOOL CALLBACK OnSysConfig(HELE hEle, HELE hEventEle);			//系统参数设置页面
extern BOOL CALLBACK OnBaseInfo(HELE hEle, HELE hEventEle);			//基本信息页面
extern BOOL CALLBACK OnReturnSet(HELE hEle, HELE hEventEle);			//系统设置(参数维护)页面上的返回
extern BOOL CALLBACK OnQueryEmployerForList(HELE hEle, HELE hEventEle);

extern BOOL CALLBACK OnSaveOther(HELE hEle, HELE hEventEle); //系统设置页面其他项的保存数据
extern BOOL CALLBACK OnConfigSave(HELE hEle, HELE hEventEle);
extern void CALLBACK TimerMsgBox(HWND   hwnd,UINT   message,UINT   iTIMERID,DWORD   dwTIMER);

extern BOOL CALLBACK OnSelectCombox(HELE hEle,HELE hEventEle);


extern LRESULT CALLBACK FrameCallBack(HWND hWnd, LPVIDEOHDR lpVHdr);

extern BOOL CALLBACK OnCompareFace(HWINDOW hWindow, WPARAM wParam, LPARAM lParam); //人脸识别对比

extern BOOL CheckIsBlackName2(VISITORINFO &visitor);
extern BOOL CheckBlackName(const VISITORINFO &visitor);
extern BOOL CheckBlackNameNotes(const VISITORINFO &visitor,char *note);
extern void GetDate_Out(char *pszDate);
extern BOOL StartKeybd();
extern BOOL StartPrintTipsPB();
extern BOOL StartUptoServer();
extern BOOL CallLeft2Server(char *szName,char *szID,char *visitetime,char *lefttime,char *barcode); //远程签离
extern BOOL BuildQRGenerator(char *szQRValue); //二维码生成器
extern BOOL CallFS533Bin(char *szParam); //调用fs533扫描仪

extern BOOL StartZTScanner();
extern BOOL StartYunMaiOCR();
extern BOOL StartFaceScaner();
extern BOOL StartFaceCompare();

extern int GetTcpBarAndUpVisitorInfo();
extern int  GetTcpCardScan(int type,IDCARD_ALL &visitorID2Card); //调用TCP方式扫描仪

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
extern void InitalVisitorExp();    //初始化导出会客单格式
extern void InitalPrintTipsCfg();  //初始化会客单格式

extern BOOL OpenXlsDlg(char *szFileName);   //打开XLS文件路径对话框
extern BOOL SaveXlsDlg(CString &m_pathDir);   //保存XLS文件导出路径

extern BOOL OpenRegCardCom(int nPort);
extern BOOL OperateRegCard(char* szIp,char* szCard,int nOperateType);

extern void EnableScanBtns(BOOL isEnable); //扫描时证件按钮控制

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
