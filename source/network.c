/*
	TailTale - Multiplayer PoC

	Note: This code work just with X86 Device
	that can handle the SDL_Net code.

	Rikku2000: This is an Early Alpha Code
	for the Multiplay of TailTale...
*/

#ifdef NETWORK
/*-------------------------------*/
/* include                       */
/*-------------------------------*/

#include "default.h"

/*-------------------------------*/
/* define                        */
/*-------------------------------*/

TCPsocket client, server;
IPaddress ip;

char network_buffer[512], network_words[16][128], netlist_words[16][128];
int client_connected, is_server, winner, loser;
int network_game, network_run, wait_slist;
int network_game_score, network_game_block, network_game_level;
int network_game_win_score, network_game_win_block;
char network_read_board[80], network_write_board[80];
char net_msg[64], net_msgprev[64]; // Chatlog
int evolution;
char slist[SERVERLIST][16];

/*-------------------------------*/

#ifdef PSP
/*
	Exit callback
*/
int exit_callback (int arg1, int arg2, void *common) {
	sceKernelExitGame ();
	return 0;
}

/*
	Callback thread
*/
int CallbackThread(SceSize args, void *argp) {
	int cbid;

	cbid = sceKernelCreateCallback ("Exit Callback", exit_callback, NULL);
	sceKernelRegisterExitCallback (cbid);
	sceKernelSleepThreadCB ();

	return 0;
}

/*
	Sets up the callback thread and returns its thread id
*/
int SetupCallbacks(void) {
	int thid = 0;

	thid = sceKernelCreateThread ("update_thread", CallbackThread, 0x11, 0xFA0, PSP_THREAD_ATTR_USER, 0);
	if(thid >= 0) {
		sceKernelStartThread (thid, 0, 0);
	}

	return thid;
}

int connectTo (int connectNum) {
	int err;
	int stateLast = -1;

	// WLAN
	err = pspSdkInetInit ();
	if (err != 0) {
		printf ("Error pspSdkInetInit() \n");
		sceKernelDelayThread (2000 * 1000);
	}

	err = sceNetApctlConnect (connectNum);
	if (err != 0) {
		printf ("Error sceNetApctlConnect() \n");
		sceKernelDelayThread (2000 * 1000);
	}

	while (1) {
		int state;
		int err = sceNetApctlGetState (&state);
		if (err != 0) {
			printf ("Error at state %i \n", state);
			sceKernelDelayThread (2000 *1000);

			return 0; // connection failed
		}

		if (state > stateLast) {
			printf ("State %i \n", state);
			stateLast = state;
		}

		if (state == 4)
			break; // connected with static IP

		// wait a little before polling again
		sceKernelDelayThread (200 * 1000); // 200 ms
	}

	return 1;
}
#endif

/*
	TNetwork_Init
*/
void TNetwork_Init () {
#ifdef PSP
	if (sceUtilityLoadNetModule (PSP_NET_MODULE_COMMON) < 0) {
		printf ("Error, could not load PSP_NET_MODULE_COMMON\n");
		sceKernelSleepThread ();
	}

	if (sceUtilityLoadNetModule (PSP_NET_MODULE_INET) < 0) {
		printf ("Error, could not load PSP_NET_MODULE_INET\n");
		sceKernelSleepThread ();
	}

	connectTo (1);
#endif

	if (TNetwork_InitGame () < 0)
		printf ("TNetwork_Init failed!\n");

#ifndef X86
	SoundSE(25);
#endif

	network_game = 0, network_run = 0;
	client_connected = 0;
}

/*
	TNetwork_ReInit
*/
void TNetwork_ReInit () {
	TNetwork_Close ();
	TNetwork_Init ();
}

/*
	TNetwork_Close
*/
void TNetwork_Close () {
	network_run = 0;
	winner = loser = 0;
	if (is_server == 1)
		TNetwork_TCP_Close (server);
	else
		TNetwork_TCP_Close (client);
	is_server = 0, network_game = 0;
	client_connected = 0;
	TNetwork_QuitGame ();
}

/*
	TNetwork_Server
*/
void TNetwork_Server () {
	if (TNetwork_ResolveHost (&ip, NULL, 1606) < 0)
		printf ("TNetwork_ResolveHost failed!\n");

	server = TNetwork_TCP_Open (&ip);
	if (server == NULL)
		printf ("TNetwork_TCP_Open failed!\n");

	is_server = 1, network_game = 1;
	winner = loser = 0;
}

/*
	TNetwork_Client

	Note: host are the IP Address we enter on Multiplayer menu
*/
void TNetwork_Client (const char* host) {
	if (TNetwork_ResolveHost (&ip, host, 1606) < 0)
		printf ("TNetwork_ResolveHost failed!\n");

	client = TNetwork_TCP_Open (&ip);
	if (client == NULL)
		printf ("TNetwork_TCP_Open failed!\n");

	is_server = 0, network_game = 1;
	winner = loser = 0;
}

/*
	TNetwork_Check

	Check the Client to Server connection.
*/
void TNetwork_Check () {
	TNetwork_ServerList (); // Send Server open
	while (client == NULL) {
		SDL_Delay (1000); // wait for 1 sec.
		client = TNetwork_TCP_Accept (server);
		if (client) {
			client_connected = 1;
			break;
		}
	}
}

/*
	TNetwork_Status

	This is like a Scoreboard for Client/Server

	0 = Name
	1 = Score
	2 = Level
	3 = Gameboard
	4 = Gamescore (Win Score)
	5 = Gamescore (Win Block)
	6 = Winner
	7 = Loser
	8 = Block
	9 = EvoMode
	10 = MSG
*/
void TNetwork_Status (char *buffer) {
	explode (network_words, buffer, '|'); // Split Buffer

	// This code are for the Gameboard "disp.c"...
	sprintf (network_write_board, "%s", network_words[3]);

	// Check who is Winner and Loser...
	if (atoi (network_words[9])) {
		if ((atoi (network_words[2]) > 31) && (network_game_level < 31))
			winner = 0, loser = 1; // Player 2 WIN!
		else if ((network_game_level > 31) && (atoi (network_words[2]) < 31))
			winner = 1, loser = 0; // Player 1 WIN!
	} else {
		if ((atoi (network_words[1]) > network_game_win_score) && (network_game_score < network_game_win_score))
			winner = 0, loser = 1; // Player 2 WIN!
		else if ((network_game_score > network_game_win_score) && (atoi (network_words[1]) < network_game_win_score))
			winner = 1, loser = 0; // Player 1 WIN!

		if ((atoi (network_words[8]) > network_game_win_block) && (network_game_score < network_game_win_block))
			winner = 0, loser = 1; // Player 2 WIN!
		else if ((network_game_score > network_game_win_block) && (atoi (network_words[8]) < network_game_win_block))
			winner = 1, loser = 0; // Player 1 WIN!
	}

// Clean Chatlog
	// net_msg[0] = '\0';
}

/*
	TNetwork_UpdateDisp

	Note: Here are a bug Server and Client not 100% Communicating.
*/
void TNetwork_UpdateDisp () {
	int len, result;

	sprintf (network_buffer, "%s|%d|%d|%s|%d|%d|%d|%d|%d|%d|%s", \
		net_name, \
		network_game_score, \
		network_game_level, \
		network_read_board, \
		network_game_win_score, \
		network_game_win_block, \
		winner, loser, \
		network_game_block, \
		evolution, \
		net_msg
	);

	len = strlen (network_buffer) + 1;

	result = TNetwork_TCP_Send (client, network_buffer, len);
	if (result < len)
		printf ("TNetwork_TCP_Send failed!\n");
	else {
		result = TNetwork_TCP_Recv (client, network_buffer, 1024);
		if (result <= 0)
			printf ("TNetwork_TCP_Recv failed!\n");
	}

	TNetwork_Status (network_buffer);
}

/*
	TNetwork_ServerList
*/
void TNetwork_ServerList () {
	int get, set, sock, work = 1;
	struct sockaddr_in client, server;
	socklen_t size;
	char result[128], buffer[512];

	gethostip ();

	sock = socket (AF_INET, SOCK_DGRAM, 0);
	if (sock < 0)
		printf ("Error: socket() failed!\n");
	memset (&server, 0, sizeof(server));

	server.sin_family = AF_INET;
	server.sin_addr.s_addr = htonl (INADDR_ANY);
	server.sin_port = htons (1604);

	set = bind (sock, (struct sockaddr *)&server, sizeof(server));
	if (set < 0)
		printf ("Error: bind() failed!\n");

	while (work) {
		size = sizeof(client);
		get = recvfrom (sock, buffer, 512, 0, (struct sockaddr *)&client, &size);
		buffer[get] = 0;
		if (strcmp(buffer, "quit") == 0)  {
			printf ("Server: %s!\n", buffer);
			work = 0;
		} else {
			if (get == -1)
				printf ("Error: recvfrom() failed!\n");
			else {
				sprintf (result, "%d.%d.%d.%d|%s|%s", ip_dat[0], ip_dat[1], ip_dat[2], ip_dat[3], net_name, NETSYS);
				set = sendto (sock, result, get, 0, (struct sockaddr *)&client, sizeof(client));
				if (set == -1)
					printf ("Error: sendto() failed!\n");
				else
					printf ("%s\n", buffer);
			}
		}
	}
}

/*
	TNetwork_ClientList
*/
void TNetwork_ClientList () {
	int i, get, list = 0, set, sock;
	struct sockaddr_in server;
	char result[128], buffer[512];
#ifdef WIN32
	u_long opt = 1;
#endif
	fd_set fds;
	struct timeval timeout;
	wait_slist = 0;

	gethostip ();

	for (i = 1; i < 255; i++) {
		sock = socket (AF_INET, SOCK_DGRAM, 0);
		if (sock < 0)
			printf ("Error: socket() failed!\n");
		memset (&server, 0, sizeof(server));

		sprintf (result, "%d.%d.%d.%d", ip_dat[0], ip_dat[1], ip_dat[2], i);
		printf ("IP: %d.%d.%d.%d\n", ip_dat[0], ip_dat[1], ip_dat[2], i);

		server.sin_family = AF_INET;
		server.sin_addr.s_addr = inet_addr (result);
		server.sin_port = htons (1604);

#ifdef WIN32
		ioctlsocket (sock, FIONBIO, &opt);
#else
		fcntl (sock, F_SETFL, O_NONBLOCK, 1);
#endif
		FD_ZERO (&fds);
		FD_SET (sock, &fds);
		timeout.tv_sec = 0;
		timeout.tv_usec = 30720;

		sprintf (result, "%d.%d.%d.%d", ip_dat[0], ip_dat[1], ip_dat[2], ip_dat[3]);
		set = sendto (sock, result, sizeof(result), 0, (struct sockaddr *)&server, sizeof(server));
		if (set == -1)
			printf ("Error: sendto() failed!\n");
		else {
			select (sock, &fds, NULL, NULL, &timeout);
			get = recvfrom (sock, buffer, 512, 0, NULL, NULL);
			buffer[get] = 0;
			if (get > -1) {
				explode (netlist_words, buffer, '|'); // Split Buffer
				sprintf (slist[list], "%s", netlist_words[0]);
				list++;
			}
			printf ("%s\n", buffer);
		}

		sock = 0;
	}

	sock = 0;

	wait_slist = 1;
	SoundSE (25);
}

/*
	TNetwork_Connect
*/
void TNetwork_Connect (const char* host, char *msg) {
	struct sockaddr_in server;
	int set, sock;

	sock = socket (AF_INET, SOCK_DGRAM, 0);
	if (sock < 0)
		printf ("Error: socket() failed!\n");
	memset (&server, 0, sizeof(server));

	server.sin_family = AF_INET;
	server.sin_addr.s_addr = inet_addr (host);
	server.sin_port = htons (1604);

	set = sendto (sock, msg, sizeof(msg), 0, (struct sockaddr *)&server, sizeof(server));
	if (set == -1)
		printf ("Error: sendto() failed!\n");

	TNetwork_Client (host);

	sock = 0;
}

void TNetwork_ChatInput (SDL_KeyboardEvent *key) {
	int l;

	switch (key->keysym.sym) { /* Hardware Keyboard for X86 */
		case SDLK_BACKSPACE:
			if (strlen (osk_buffer))
				osk_buffer[strlen (osk_buffer) - 1] = 0;
			break;

		case SDLK_ESCAPE:
			osk_buffer[0] = net_msgprev[0] = '\0';
			net_msgstart = 0;
			break;

		case SDLK_RETURN:
			sprintf (net_msg, "%s", net_msgprev);
			net_msgstart = 0;
			SoundSE(26);
			break;

		default:
			if (key->keysym.sym < 32 || key->keysym.sym > 127)
				break;

			l = strlen (osk_buffer);
			if (l < 52) {
				osk_buffer[l + 1] = 0;
				osk_buffer[l] = key->keysym.sym;
			}
			break;
	}

	sprintf (net_msgprev, "%s", osk_buffer);
}

#endif
