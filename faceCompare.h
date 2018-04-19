#ifndef _FACE_H
#define _FACE_H

bool static g_bStopCompare= true;
bool static g_bExitCompare= false;

DWORD WINAPI FaceCompareThread(LPVOID lpVoid);

#endif	//end _FACE_H