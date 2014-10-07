/*
	TailTale - Netcode Protocol

	This is the Netcode for Windows and Linux.
	The code are from SDL_net with small edits.
*/

#ifdef NETWORK
/*-------------------------------*/
/* include                       */
/*-------------------------------*/

#include "default.h"

/*-------------------------------*/
/* define                        */
/*-------------------------------*/

#ifdef WIN32
#ifndef EINTR
#define EINTR WSAEINTR
#endif
#ifndef FIONBIO
#define FIONBIO 0x5421
#endif

#define TNetwork_GetLastError WSAGetLastError
#define TNetwork_SetLastError WSASetLastError
#endif

static int TNetwork_started = 0;

/*-------------------------------*/

void gethostip () {
	struct hostent *host;

#ifdef GEKKO
	char WIIIPAddr[16];
	if (if_config (WIIIPAddr, NULL, NULL, true) >= 0)
		printf ("IP: %s\n", WIIIPAddr);
	else // If no IP get set Localhost...
		strcpy (WIIIPAddr, "127.0.0.1");

	host = gethostbyname (WIIIPAddr);
#elif PSP
	char PSPIPAddr[32];

	if (sceNetApctlGetInfo (8, PSPIPAddr) != 0)   
		strcpy (PSPIPAddr, "127.0.0.1"); // If no IP get set Localhost...

	host = gethostbyname (PSPIPAddr);
#else
	char ipaddr[255];

#ifdef WIN32
	if (gethostname (ipaddr, sizeof(ipaddr)) == 0) {
#endif
		host = gethostbyname (ipaddr);
#endif
		if (host) {
			u_int32_t *x = (u_int32_t *) host->h_addr_list[0];
			ip_dat[0] = (ntohl (*x) & 0xff000000) >> 24;
			ip_dat[1] = (ntohl (*x) & 0x00ff0000) >> 16;
			ip_dat[2] = (ntohl (*x) & 0x0000ff00) >> 8;
			ip_dat[3] = (ntohl (*x) & 0x000000ff);
		}
#ifdef WIN32
	} else
		printf ("Error: Unable to get Hostname.\n");
#endif
}

/*-------------------------------*/

void TNetwork_TCP_Close (TCPsocket sock) {
	if (sock != NULL) {
		if (sock->channel != INVALID_SOCKET) {
			closesocket (sock->channel);
		}

		free (sock);
	}
}

int TNetwork_InitGame (void) {
	if (!TNetwork_started) {
#ifdef WIN32
		WORD version_wanted = MAKEWORD (1, 1);
		WSADATA wsaData;
		if (WSAStartup (version_wanted, &wsaData) != 0) {
			printf ("Couldn't initialize Winsock 1.1\n");
			return (-1);
		}
#else
		void (*handler)(int);
		handler = signal (SIGPIPE, SIG_IGN);
		if (handler != SIG_DFL) {
			signal (SIGPIPE, handler);
		}
#endif
	}

	++TNetwork_started;
	return (0);
}

void TNetwork_QuitGame (void)
{
	if (TNetwork_started == 0) {
		return;
	}

	if (--TNetwork_started == 0) {
#ifdef WIN32
		if (WSACleanup () == SOCKET_ERROR) {
			if (WSAGetLastError () == WSAEINPROGRESS) {
				WSACleanup ();
			}
		}
#else
		void (*handler)(int);
		handler = signal (SIGPIPE, SIG_DFL);
		if (handler != SIG_IGN) {
			signal (SIGPIPE, handler);
		}
#endif
	}
}

int TNetwork_ResolveHost (IPaddress *address, const char *host, Uint16 port) {
	int retval = 0;

	if (host == NULL) {
#ifdef WIN32
		address->host = 0x00000000;
#else
		address->host = ((in_addr_t) 0x00000000);
#endif
	} else {
		address->host = inet_addr (host);
		if (address->host == 0xffffffff) {
			struct hostent *hp;

			hp = gethostbyname (host);
			if (hp) {
				memcpy (&address->host, hp->h_addr, hp->h_length);
			} else {
				retval = -1;
			}
		}
	}
	address->port = SDL_SwapBE16 (port);

	return (retval);
}

#ifndef WIN32
int TNetwork_GetLastError (void) {
	return errno;
}

void TNetwork_SetLastError (int err) {
	errno = err;
}
#endif

TCPsocket TNetwork_TCP_Open (IPaddress *ip) {
	TCPsocket sock;
	struct sockaddr_in sock_addr;
#ifdef WIN32
	unsigned long mode = 1;
#endif

	sock = (TCPsocket)malloc(sizeof(*sock));
	if (sock == NULL) {
		printf ("Out of memory");
		goto error_return;
	}

	sock->channel = socket(AF_INET, SOCK_STREAM, 0);
	if (sock->channel == INVALID_SOCKET) {
		printf ("Couldn't create socket");
		goto error_return;
	}
	
#ifdef WIN32
	if ((ip->host != 0xffffffff) && (ip->host != 0x00000000)) {
#else
	if ((ip->host != 0xffffffff) && (ip->host != ((in_addr_t) 0x00000000))) {
#endif
		memset(&sock_addr, 0, sizeof(sock_addr));
		sock_addr.sin_family = AF_INET;
		sock_addr.sin_addr.s_addr = ip->host;
		sock_addr.sin_port = ip->port;

		if (connect(sock->channel, (struct sockaddr *)&sock_addr, sizeof(sock_addr)) == SOCKET_ERROR ) {
			printf ("Couldn't connect to remote host");
			goto error_return;
		}
		sock->sflag = 0;
	} else {
		memset(&sock_addr, 0, sizeof(sock_addr));
		sock_addr.sin_family = AF_INET;

#ifdef WIN32
		sock_addr.sin_addr.s_addr = 0x00000000;
#else
		sock_addr.sin_addr.s_addr = ((in_addr_t) 0x00000000);
#endif
		sock_addr.sin_port = ip->port;

		if (bind(sock->channel, (struct sockaddr *)&sock_addr, sizeof(sock_addr)) == SOCKET_ERROR) {
			printf ("Couldn't bind to local port");
			goto error_return;
		}
		if (listen(sock->channel, 5) == SOCKET_ERROR) {
			printf ("Couldn't listen to local port");
			goto error_return;
		}

#ifdef WIN32
		ioctlsocket (sock->channel, FIONBIO, &mode);
#else
		int flags = fcntl (sock->channel, F_GETFL, 0);
		fcntl (sock->channel, F_SETFL, flags | O_NONBLOCK);
#endif
		sock->sflag = 1;
	}
	sock->ready = 0;

	sock->remoteAddress.host = sock_addr.sin_addr.s_addr;
	sock->remoteAddress.port = sock_addr.sin_port;

	return(sock);

error_return:
	TNetwork_TCP_Close (sock);
	return(NULL);
}

TCPsocket TNetwork_TCP_Accept (TCPsocket server) {
	TCPsocket sock;
	struct sockaddr_in sock_addr;
	socklen_t sock_alen;
#ifdef WIN32
	unsigned long mode = 0;
#endif

	if (!server->sflag) {
		printf ("Only server sockets can accept()");
		return (NULL);
	}
	server->ready = 0;

	sock = (TCPsocket)malloc(sizeof(*sock));
	if (sock == NULL) {
		printf ("Out of memory");
		goto error_return;
	}

	sock_alen = sizeof(sock_addr);
	sock->channel = accept (server->channel, (struct sockaddr *)&sock_addr, &sock_alen);
	if (sock->channel == INVALID_SOCKET) {
		printf ("accept() failed");
		goto error_return;
	}
	
#ifdef WIN32
	ioctlsocket (sock->channel, FIONBIO, &mode);
#elif defined(O_NONBLOCK)
	fcntl (sock->channel, F_SETFL, 0);
#endif

	sock->remoteAddress.host = sock_addr.sin_addr.s_addr;
	sock->remoteAddress.port = sock_addr.sin_port;

	sock->sflag = 0;
	sock->ready = 0;

	return (sock);

error_return:
	TNetwork_TCP_Close (sock);
	return (NULL);
}

int TNetwork_TCP_Send (TCPsocket sock, const void *datap, int len) {
	const Uint8 *data = (const Uint8 *)datap;
	int sent, left;

	if (sock->sflag) {
		printf ("Server sockets cannot send");
		return (-1);
	}

	left = len;
	sent = 0;
	TNetwork_SetLastError (0);
	do {
		len = send (sock->channel, (const char *) data, left, 0);
		if (len > 0) {
			sent += len;
			left -= len;
			data += len;
		}
	} while ((left > 0) && ((len > 0) || (TNetwork_GetLastError () == EINTR)));

	return (sent);
}

int TNetwork_TCP_Recv (TCPsocket sock, void *data, int maxlen) {
	int len;

	if (sock->sflag) {
		printf ("Server sockets cannot receive");
		return (-1);
	}

	TNetwork_SetLastError (0);
	do {
		len = recv (sock->channel, (char *) data, maxlen, 0);
	} while (TNetwork_GetLastError () == EINTR);

	sock->ready = 0;
	return (len);
}

#endif
