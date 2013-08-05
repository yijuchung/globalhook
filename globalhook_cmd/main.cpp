#include <WinSock2.h>
#include <stdio.h>
#include <windows.h>
#include <tchar.h>
#include "net.h"

#define IsBitSet(val, bit) ((val) & (1 << (bit)))

typedef BOOL (*pFunc) ();


int _tmain(int argc, TCHAR** argv) //this_program.exe working_dir wow_exe
{
	static HINSTANCE hinstDLL; 

	FILE *log;
	log = fopen("D:\\log\\main.txt","a+");

	hinstDLL = LoadLibrary(TEXT("globalhook_dll.dll")); 
	
	fprintf(log,"Start hooking!!\r\n");
	pFunc set = (pFunc)GetProcAddress(hinstDLL,"SetHook");
	pFunc un = (pFunc)GetProcAddress(hinstDLL,"UnHook");

	set();
	char temp = getchar();
	un();
	FreeLibrary(hinstDLL);
	fclose(log);

	return 0;
}