// Re-written simple SDL_net socket server example | Nov 2011 | r3dux
// Library dependencies: libSDL, libSDL_net

// IMPORTANT: This project will only build successfully in Debug mode on Windows!

#include <SDL.h>
#undef main

#include <stdio.h>
#include <iostream>
#include <string>
#include "include/SDL_net.h"
#include "ServerSocket.h"
#include "Cmd.h"
#include "Login.h"

using std::FILE;

int main(int argc, char *argv[])
{


	// Initialise SDL_net
	if (SDLNet_Init() == -1)
	{
		std::cerr << "Failed to intialise SDL_net: " << SDLNet_GetError() << std::endl;
		exit(-1);
	}

	// Create a pointer to a ServerSocket object
	ServerSocket *ss;
	Cmd* cmd;
	Login* l;
	
	try
	{
		// Not try to instantiate the server socket
		// Parameters: port number, buffer size (i.e. max message size), max sockets
		ss = new ServerSocket(1234, 2048, 3);
		cmd = new Cmd();
		l = new Login();
		l->userCount = 0;
		FILE* users = fopen("D:\\Semestralka UDSP\\ServerCpp\\users.txt", "r+");
		l->nacitajUserov(users);
		
	}
	catch (SocketException e)
	{
		std::cerr << "Something went wrong creating a SocketServer object." << std::endl;
		std::cerr << "Error is: " << e.what()   << std::endl;
		std::cerr << "Terminating application." << std::endl;
		exit(-1);
	}

	try
	{
		// Specify which client is active, -1 means "no client is active"
		int activeClient = -1;

		// Main loop...
		do
		{
			// Check for any incoming connections to the server socket
			ss->checkForConnections();

			// At least once, but as many times as necessary to process all active clients...
			do
			{
				// ..get the client number of any clients with unprocessed activity (returns -1 if none)
				activeClient = ss->checkForActivity();

				// If there's a client with unprocessed activity...
				if (activeClient != -1)
				{
					// ...then process that client!
					//ss->SetBuf(cmd->Command(ss->GetBuf()));

					ss->dealWithActivity(activeClient);
				}

			// When there are no more clients with activity to process, continue...
			} while (activeClient != -1);

		// ...until we've been asked to shut down.
		} while (ss->getShutdownStatus() == false);

	}
	catch (SocketException e)
	{
		cerr << "Caught an exception in the main loop..." << endl;
		cerr << e.what() << endl;
		cerr << "Terminating application." << endl;
	}

	// Shutdown SDLNet - our ServerSocket will clean up after itself on destruction
	SDLNet_Quit();

	return 0;
}
