1.��visitorapp.hͷ�ļ������ȫ�ֱ���g_bQRcode�����ó�ʼֵΪfalse��

2.BOOL CALLBACK OnVisitorLogOut(HWINDOW hWindow, WPARAM wParam, LPARAM lParam)
����������жϣ����ж��ǲ��Ƕ�ά�룬�ǣ��Ƚ�����ʱ���ԤԼʱ���Ƿ�һ�£����Ƕ�ά������ǩ�����̣�ʱ�䲻һ�����˳���
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
			XMessageBox(g_hWindow, L"�������ں�ԤԼ���ڲ���ͬһ��!", L"����");
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
		
		 	//���ö�ά������Ϊtrue
			g_bQRcode = true;
			return true;
		}
		
	}
2.��InitVisitorData����������жϣ��ж� �ÿ��ֻ��š��������ɼ����ƺ��룻����Ƕ�ά�벻��ո����ݣ�
if(g_bQRcode) //�ж��������
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
3.��BOOL CALLBACK OnReadID2Over����������жϣ��ÿ��ֻ����������ɡ����ƺ����Ƿ���ֵ������а����ݸ�ֵ��visitor�����¶�Ӧ������
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
