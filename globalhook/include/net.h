#define MAX_IP_LEN			16
#define MAX_PORT_LEN		6
#define MAX_PACKET_SIZE		1024
#define MAX_NUM_VECTOR		10000
#define MAX_NUM_SERIAL		1000

struct MSG_H
{
	int		type;
	unsigned long	serial;
	DWORD	dwTime;
//	int		size;
};

struct MSG_INI
{
	MSG_H	header;
	DWORD	dwPad[5];
	MSG_INI(){
		header.type = 1;
//		header.size = sizeof(MSG_INI);
	};
};

struct MSG_KB
{
	MSG_H	header;
	DWORD	vk;
	DWORD	vc;
	DWORD	dwFlags;
	DWORD	dwExtraInfo;
	DWORD	dwBlank;

	MSG_KB(){
		header.type = 2;
//		header.size = sizeof(MSG_KB);
	};
};

struct MSG_MS
{
	MSG_H	header;
	LONG	lx;
	LONG	ly;
	DWORD	dwFlags;
	DWORD	dwExtraInfo;
	DWORD	mouseData;
	
	MSG_MS(){
		header.type = 3;
//		header.size = sizeof(MSG_MS);
	};
};