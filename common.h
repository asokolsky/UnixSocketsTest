
int init_sockets()
{
#ifdef __linux__
	return 0;
#else
	WSADATA WsaData;
	return WSAStartup(MAKEWORD(2, 2), &WsaData);
#endif
}

void shut_sockets()
{
#ifdef __linux__
	;
#else
	WSACleanup();
#endif
}

char *socket_path =
  "c:/temp/test.socket";
//"\0hidden";
