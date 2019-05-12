#ifdef __linux__
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#else
//#include <windows.h>
#include <winsock2.h>
#include <afunix.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include "common.h"


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

	int fd = socket(AF_UNIX, SOCK_STREAM, 0);
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
	}

	if (connect(fd, (struct sockaddr*)&addr, sizeof(addr)) == -1) 
	{
		perror("connect error");
		exit(-1);
	}
	printf("Starting write to the server at %s\n", socket_path);
	long unsigned iWritten = 0;
	for(; iWritten < (long unsigned)1024*1024*1024; )
	{
		char buf[1024];
		int rc = sizeof(buf); //  read(STDIN_FILENO, buf, sizeof(buf));
		if( rc <= 0 )
			break;
		int iW = send( fd, buf, rc, 0 );
		if (iW > 0)
		{
			//printf("wrote %d bytes\n", iW);
			//if (iW != rc) fprintf(stderr, "partial write");
			iWritten += (long unsigned)iW;
		}
		else
		{
			perror("write error");
			exit(-1);
		}

	}
	printf("wrote %d bytes\n", iWritten);
	shut_sockets();
	return 0;
}
