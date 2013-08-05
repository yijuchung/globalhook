#include <WinSock2.h>
#include <stdio.h>
#include <windows.h>
#include "net.h"

int sServerSocket;

fd_set	rset;
fd_set	temprset;
int maxfdp1;

struct Client
{
	bool	bConnect;
	int		iSocket;
};

bool ProcessPacket( char * pPacket)
{
	return true;
}

int main()
{
	//char srv_ip[MAX_IP_LEN] = "140.109.22.137";
	char srv_port[MAX_PORT_LEN] = "5050";

	WSADATA wsaData;
	if( SOCKET_ERROR == WSAStartup( WINSOCK_VERSION, &wsaData ) || wsaData.wVersion != WINSOCK_VERSION )
            return false;

	char ac[80];
    if (gethostname(ac, sizeof(ac)) == SOCKET_ERROR) {
		//printf("");
        return 1;
    }
    
    struct hostent *phe = gethostbyname(ac);

    char* localIP = inet_ntoa (*(struct in_addr *)*phe->h_addr_list);

	printf("start server : %s\n",localIP);

	sServerSocket = socket(AF_INET, SOCK_STREAM, 0);

	struct sockaddr_in addr;
	ZeroMemory( &addr, sizeof(addr) );
	addr.sin_family = AF_INET;
	addr.sin_port   = htons(atoi(srv_port));
	addr.sin_addr.S_un.S_addr   = inet_addr(localIP);
	
	if( SOCKET_ERROR == bind( sServerSocket, (struct sockaddr*)&addr, sizeof(struct sockaddr) ) )
		return -1;
	
	listen(sServerSocket,5);

	int nready;
	struct timeval timeout;

	timeout.tv_sec = 0;
	timeout.tv_usec = 0;
	FD_ZERO(&rset);
	FD_ZERO(&temprset);
	FD_SET( sServerSocket, &rset);
	maxfdp1 = sServerSocket + 1;

	bool connected = false;
	//int sockfd = 0;

	Client cUsers[10];
	for(int i =0;i<10;i++)
	{
		cUsers[i].bConnect = false;
		cUsers[i].iSocket = -1;
	}

	while(true)
	{
		timeout.tv_sec = 0;
		timeout.tv_usec = 0;
		FD_ZERO(&temprset);
		temprset = rset;

		if ( (nready = select(maxfdp1, &temprset, NULL, NULL, &timeout)) < 0)
		{
			break;

		}else if( nready == 0)
		{
			continue;
		}

		if (FD_ISSET(sServerSocket, &temprset))
		{
			//printf("recv connect from dll !!!!!\n");
			
			struct sockaddr address;
			int len = sizeof(struct sockaddr);
			
			int sockfd = accept ( sServerSocket, (struct sockaddr *)&address, &len );

			FD_SET( sockfd, &rset);
			maxfdp1 = max(maxfdp1-1,sockfd)+1;
			//printf("sock = %d !!!!!\n",sockfd);
			//int len = recv( sockfd, (char *)buf, sizeof(MessageHeader), 0);
			for(int i = 0;i<10;i++)
			{
				if(!cUsers[i].bConnect)
				{
					cUsers[i].iSocket = sockfd;
					cUsers[i].bConnect = true;
					break;
				}

			}
			//ProcessPacket( (char *)&buffer );
			continue;
		}

		for(int i = 0;i<10;i++)
		{
			if(cUsers[i].bConnect && FD_ISSET(cUsers[i].iSocket, &temprset) )
			{
					//printf("recv packet from sock=%d !!!!!\n",cUsers[i].iSocket);
					INPUT input;
					//MSG_H msgh;
					//ZeroMemory( input, sizeof(INPUT) );
					int len = recv( cUsers[i].iSocket, (char *)&input, sizeof(input), 0);


					if( input.type == INPUT_MOUSE )
					{
						input.mi.dx =  input.mi.dx*(65536/GetSystemMetrics(SM_CXSCREEN));//x being coord in pixels
						input.mi.dy =  input.mi.dy*(65536/GetSystemMetrics(SM_CYSCREEN));//y being coord in pixels
					}
					//SendInput( 1, &input, sizeof(INPUT));
			}

		}

	}

	closesocket( sServerSocket );
	WSACleanup();
   
	return 0;
}