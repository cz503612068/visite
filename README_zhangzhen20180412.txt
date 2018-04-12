1.在visitorapp.h头文件中添加全局变量g_bQRcode，设置初始值为false；

2.BOOL CALLBACK OnVisitorLogOut(HWINDOW hWindow, WPARAM wParam, LPARAM lParam)
函数中添加判断，先判断是不是二维码，是，比较来访时间和预约时间是否一致，不是二维码则走签离流程，时间不一致则退出；
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
        cstr.Format(g_szQRvisitime);
		//cstr.Replace("T"," ");
		cstr = cstr.Left(cstr.GetLength() - 14);
        CTime time = CTime::GetCurrentTime();
		CString m_strTime = time.Format("%Y-%m-%d");
		//CString m_strTime = time.Format("%Y-%m-%d %H:%M:%S");
		//m_strTime = m_strTime.Left(m_strTime.GetLength() - 9)
		//CTimeSpan span=m_strTime-g_szQRvisitime;
		if (cstr != m_strTime)
		{
			XMessageBox(g_hWindow, L"来访日期和预约日期不是同一天!", L"错误");
			ResumeThread(g_ThreadReadBarcode);
			return false;
		}
		else
		{
			InitVisitorData();
			VISITORINFO visitor;
			memset((char *)&visitor, 0, sizeof(VISITORINFO));
			OFFICIALINFO official;
			memset((char *)&official, 0, sizeof(OFFICIALINFO));
			getYueJSon(json,visitor);
			SetVisitorInfo(visitor);
			strcpy(official.szName,visitor.szToOfficialName);
			SetOfficialInfo(official);
			strcpy(official.szMobile,visitor.szToOfficialPhone);
			SetOfficialInfo(official);
			XWnd_RedrawWnd(g_hWindow);
			ResumeThread(g_ThreadReadBarcode);
		
		 	//设置二维码类型为true
			g_bQRcode = true;
			return true;
		}
		
	}
2.在InitVisitorData函数中添加判断：判断 访客手机号、来访事由及车牌号码；如果是二维码不清空改内容；
if(g_bQRcode) //判断清空数据
	{
		for(i = 5003; i < 5012; i++)
		{
			if((i != 5005) && (i != 5009))
			{
				hE = GETELE(i);
				XEdit_SetText(hE, L"");
			}	
		}
	}
	else
	{
		for (i = 5003; i <= 5012; i++)
		{
			hE = GETELE(i);
			XEdit_SetText(hE, L"");
		}
	}
3.在BOOL CALLBACK OnReadID2Over函数中添加判断：访客手机、来访事由、车牌号码是否有值，如果有把内容赋值给visitor数组下对应参数；
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
