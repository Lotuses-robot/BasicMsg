#define WIN32_LEAN_AND_MEAN
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <bits/stdc++.h>
#include <cstring>
#include <windows.h>
#include <WinSock2.h>
#include <cstdio>
#include <pthread.h>
auto _cSock = INVALID_SOCKET;
char friendip[256];
bool enable_msg=1;
void SetTextColor(int ForgC,int BackC)
{
	/* this show the color code:
	   0 black (default Back)
	   1 blue
	   2 green
	   3 pale-green
	   4 red
	   5 purple
	   6 yellow
	   7 white
	   8 grey
	   9 light-blue
	   10 light-green
	   11 light-pale-green
	   12 light-red
	   13 light-purple
	   14 light-yellow
	   15 light-white (default Forg)
	*/
	WORD wColor = ((BackC & 0x0F) << 4) + (ForgC & 0x0F);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),wColor);
}
void System_Msg(char sysmsg[256])
{
	SetTextColor(2,0); 
	printf("BasicMsg");
	for(int i=1;i<=(strlen(friendip)-8);i++) printf(" ");
	SetTextColor(8,0);
	printf(" | ");
	SetTextColor(2,0);
	printf("%s\n",sysmsg);
	SetTextColor(15,0);
}
void* Server_Setup(void* args)
{
	// start Windows socket 2.x
	WORD ver = MAKEWORD(2, 2);
	WSADATA dat;
	WSAStartup(ver, &dat);
	//----------------

	// --use Socket API mk easy TCP server
	// make a socket
	SOCKET _sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	// Bind the network port used to accept client connections
	sockaddr_in _sin = {};
	_sin.sin_family = AF_INET;
	_sin.sin_port = htons(9527);
	_sin.sin_addr.S_un.S_addr = INADDR_ANY;
	if (SOCKET_ERROR == bind(_sock, (sockaddr*)&_sin, sizeof(_sin)))
		printf("ERROR,Binding the network port used to accept client connections failed...\n");

	// 监听端口
	if (SOCKET_ERROR == listen(_sock, 5))
		printf("Failed to listen to network port...\n");

	// 接收客户端连接
	sockaddr_in clientAddr = {};
	int nAddrLen = sizeof(sockaddr_in);

	// 向客户端发送一条数据
	char msgBuf[] = "HaHa, you join the server!";
	_cSock = accept(_sock, (sockaddr*)&clientAddr, &nAddrLen);
	if (INVALID_SOCKET == _cSock) printf("Invalid client received SOCKET...\n");
	//printf("新客户端加入：IP = %s:%d\n", inet_ntoa(clientAddr.sin_addr), clientAddr.sin_port);

    //send(_cSock, msgBuf, sizeof(m sgBuf), 0); //测试消息
	strcpy(friendip,inet_ntoa(clientAddr.sin_addr));
	char* text = "Client joined.";
	System_Msg(text);
	text = "Everything OK.";
	System_Msg(text);
	while(1);
	closesocket(_sock);
	WSACleanup();
}

void* rec(void* args)
{
	char r[256];
	int nlen;
	while(1)
	{
		nlen=recv(_cSock,r,1024,0);
		if(nlen>0 && enable_msg) 
		{
			SetTextColor(13,0);
			printf("%s",friendip);
			SetTextColor(8,0);
			printf(" | ");
			SetTextColor(15,0);
			printf("%s\n",r);
		}
	}
}

char msg[256];
std::string s;

int main()
{
	pthread_t tid,tid2;
	int ret= pthread_create(&tid, nullptr,Server_Setup,nullptr);
	if(ret!=0) printf("Wrong:%d\n",ret);
	ret= pthread_create(&tid2, nullptr,rec,nullptr);
	if(ret!=0) printf("Wrong:%d\n",ret);
	else
	{
		do
		{
			if(strcmp(msg,"")!=0)
			{
				s=msg;
				if(s=="cls") system("cls");
				else if(s=="enable msg")
				{
					enable_msg=1;
					char* text="Message enabled.You'll able to see the messages.";
					System_Msg(text);
				}
				else if(s=="disable msg")
				{
					enable_msg=0;
					char* text="Message disabled.You won't able to see the messages.";
					System_Msg(text);					
				}
				else send(_cSock,s.c_str(),sizeof(msg),0);
				
			}
			gets(msg);
		}
		while(strcmp(msg,"0")!=0);
	}
	return 0;
}
