#ifndef __NETWORK_H__
#define __NETWORK_H__

#ifdef NETWORK
/*-------------------------------*/
/* define                        */
/*-------------------------------*/

#ifdef WIN32
	#define NETSYS "Windows"
#elif WINPHONE
	#define NETSYS "WinPhone 8"
#elif LINUX
	#define NETSYS "Linux"
#elif __APPLE__
	#define NETSYS "Mac OSX"
#elif ANDROID
	#define NETSYS "Android"
#elif PSP
	#define NETSYS "PSP"
#elif __WII__
	#define NETSYS "WII"
#elif CAANOO
	#define NETSYS "GPH Caanoo"
#elif GP2X
	#define NETSYS "GPH GP2X"
#elif WIZ
	#define NETSYS "GPH Wiz"
#else
	#define NETSYS "Unknow"
#endif

#ifndef WIN32
#define SOCKET			int
#define INVALID_SOCKET	-1
#define SOCKET_ERROR	-1
#endif
#define closesocket		close

typedef struct {
	Uint32 host;
	Uint16 port;
} IPaddress;

typedef struct {
	int ready;
	SOCKET channel;
	IPaddress remoteAddress;
	IPaddress localAddress;
	int sflag;
} _TCPsocket, *TCPsocket;

extern int client_connected, is_server, win, wait_slist;
extern int network_game, network_game_score, network_game_block;
extern int network_game_level, network_game_win_score, network_game_win_block;
extern char network_read_board[80], network_write_board[80];
extern char network_words[16][128], netlist_words[16][128];
extern int evolution;
extern char net_msg[64], net_msgprev[64]; // Chatlog
extern int net_msgstart;

// Windows need to Understand...
#if defined(GEKKO) || defined(PSP) || defined(WIN32)
typedef unsigned char u_int8_t;
typedef unsigned int u_int32_t;
#endif

u_int8_t ip_dat[4];

#define SERVERLIST 10
extern char slist[SERVERLIST][16];

/*-------------------------------*/

extern void gethostip ();

extern void TNetwork_TCP_Close (TCPsocket sock);

extern int TNetwork_InitGame (void);
extern void TNetwork_QuitGame (void);

extern int TNetwork_ResolveHost (IPaddress *address, const char *host, Uint16 port);

#ifndef WIN32
extern int TNetwork_GetLastError (void);
extern void TNetwork_SetLastError (int err);
#endif

extern TCPsocket TNetwork_TCP_Open (IPaddress *ip);
extern TCPsocket TNetwork_TCP_Accept (TCPsocket server);

extern int TNetwork_TCP_Send (TCPsocket sock, const void *datap, int len);
extern int TNetwork_TCP_Recv (TCPsocket sock, void *data, int maxlen);

void TNetwork_Init ();
void TNetwork_ReInit ();

void TNetwork_Close ();

void TNetwork_Server ();
void TNetwork_Client (const char* host);

void TNetwork_Check ();

void TNetwork_Status (char *buffer);

void TNetwork_UpdateDisp ();

void TNetwork_ServerList ();
void TNetwork_ClientList ();

void TNetwork_Connect (const char* host, char *msg);

void TNetwork_ChatInput (SDL_KeyboardEvent *key);
#endif

#endif
