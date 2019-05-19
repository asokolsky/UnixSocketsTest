#if defined( __linux__ ) || defined( __CYGWIN__ )

#define closesocket close
#define _unlink unlink

#endif

int init_sockets()
{
#if defined( __linux__ ) || defined( __CYGWIN__ )
	return 0;
#else
	WSADATA WsaData;
	return WSAStartup(MAKEWORD(2, 2), &WsaData);
#endif
}

void shut_sockets()
{
#if defined( __linux__ ) || defined( __CYGWIN__ )
	;
#else
	WSACleanup();
#endif
}

char *socket_path =
#if defined( __linux__ ) || defined( __CYGWIN__ )
  "/var/run/test.socket";
//"\0hidden";
#else
  "c:/temp/test.socket";
#endif

