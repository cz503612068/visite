#include "StdAfx.h"

extern HWINDOW g_hWindow;
//g_bExitCompare = false;
//g_bStopCompare = true;

DWORD WINAPI FaceCompareThread(LPVOID lpVoid)
{
	int n=face_init();
	if(n>0)
	{
		//AfxMessageBox("face_init");
		while(!g_bExitCompare)
		{							
			while(g_bStopCompare)
			{
				Sleep(10);
				continue;
			}

			if (_access(USER_VIDEO_PIC,0)!=0) //USB拍照文件不存在
			{
				continue;
			}

			char buf1[8000];
			char buf2[8000];
			buf1[0]=buf2[0]=0;

			if (_access(USER_ID2_TMP,0)==0)
            {
                n=face_get_feature_from_image(USER_ID2_TMP, buf2);
				if(n<=0)
				{
					PostMessage(XWnd_GetHWnd(g_hWindow), WM_READID2_OVER, 6, -21);
					continue;
				}
            }
            else if (_access(USER_ID_PIC,0)==0)
            {
                //if (File.Exists("..\\pic\\user_id2_tmp.bmp"))
                //    File.Delete("..\\pic\\user_id2_tmp.bmp");
				::DeleteFile(USER_ID2_TMP);
                n=face_get_feature_from_image(USER_ID_PIC, buf2);
				if(n<=0)
				{
					PostMessage(XWnd_GetHWnd(g_hWindow), WM_READID2_OVER, 6, -22);
					continue;
				}
            }
            else
            {
                PostMessage(XWnd_GetHWnd(g_hWindow), WM_READID2_OVER, 6, -1);
                continue;
            }




			n=face_get_feature_from_image(USER_VIDEO_PIC,buf1);
			if(n<=0)
			{
				PostMessage(XWnd_GetHWnd(g_hWindow), WM_READID2_OVER, 6, -31);
				continue;
			}



			//	printf("人脸特征2:%s\n",buf1);

			//printf("ret %d\n",n);
			
			/*
			int r;
			r=face_get_feature_from_image(".\\b.bmp",buf1);
			if(r>0)
				printf("人脸特征1:%s\n",buf1);
			else
				printf("ret: %d\n",r);
			*/
		
			if(buf1[0] && buf2[0]){
				int score=face_comp_feature(buf1,buf2);
				printf("对比相似度: %d\n",score);
				PostMessage(XWnd_GetHWnd(g_hWindow), WM_READID2_OVER, 6, score);
			}

		}

	}
	//AfxMessageBox("sdfsdf");
	face_exit();
	printf("face init code : %d\n",n);
	return 0;
}