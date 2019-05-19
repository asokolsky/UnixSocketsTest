#include <stdio.h>

#if defined( __linux__ ) || defined( __CYGWIN__ )
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#else
//#include <windows.h>
#include <winsock2.h>
#include <afunix.h>
#endif
#include <stdlib.h>
#include "common.h"



static int talk_to_client(int cl)
{
	int iRead = 0;
	for (;;)
	{
		char buf[256];
		int rc = recv( cl, buf, sizeof(buf), 0 );
		if (rc > 0)
		{
			//printf("got %d bytes\n", rc);
			iRead += rc;
			continue;
		}
		else if (rc == 0)
		{
			printf("EOF\n");
			break;
		}
		//perror("read");
		//exit(-1);
		break;
	}
	printf("got %d bytes from client\n", iRead);
	closesocket(cl);
	return 0;
}

int main(int argc, char *argv[]) 
{
	int iRes = init_sockets();
	if (iRes != 0)
	{
		perror("Failed to init sockets");
		exit(-1);
	}
	if (argc > 1)
		socket_path = argv[1];
	int fd = (int)socket(AF_UNIX, SOCK_STREAM, 0);
	if (fd == -1) 
	{
		perror("socket error");
		exit(-1);
	}
	struct sockaddr_un addr;
	memset(&addr, 0, sizeof(addr));
	addr.sun_family = AF_UNIX;
	if (*socket_path == '\0') 
	{
		*addr.sun_path = '\0';
		strncpy(addr.sun_path + 1, socket_path + 1, sizeof(addr.sun_path) - 2);
	}
	else 
	{
		strncpy(addr.sun_path, socket_path, sizeof(addr.sun_path) - 1);
		_unlink(socket_path);
	}

	if (bind(fd, (struct sockaddr*)&addr, sizeof(addr)) == -1) 
	{
		perror("bind error");
		exit(-1);
	}

	if (listen(fd, 5) == -1) 
	{
		perror("listen error");
		exit(-1);
	}
	printf("Ready to accept connections on %s\n", socket_path);

	for(;;) 
	{
		int cl = accept(fd, NULL, NULL);
		if (cl == -1) 
		{
			perror("accept error");
			continue;
		}
		printf("Talking to client...\n");
		talk_to_client(cl);
	}
	shut_sockets();
	return 0;
}
