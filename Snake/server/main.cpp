#include <stdlib.h>
#include <iostream>
#include <string>
#include <sstream>
#include <time.h>
#include "websocket.h"

using namespace std;


int player1Score = 0;
int player2Score = 0;
std::string Player1ID;
std::string Player2ID;
webSocket server;

/* called when a client connects */
void openHandler(int clientID){
	ostringstream os;
	os << "Player " << clientID << " has joined.";

	vector<int> clientIDs = server.getClientIDs();
	for (int i = 0; i < clientIDs.size(); i++){
		if (clientIDs[i] != clientID)
			server.wsSend(clientIDs[i], os.str());
	}

	server.wsSend(clientID, "Welcome!");

	// send client ids to all connected clients
	//ostringstream os2;
	ostringstream osArray[2];
	for (int i = 0; i < clientIDs.size(); i++){
		osArray[i] << "__" << clientIDs[i] << " " << clientIDs[i];
	}
	for (int j = 0; j < clientIDs.size(); j++){
		for (int k = 0; k < clientIDs.size(); k++){
			server.wsSend(k, osArray[j].str());
		}
	}
}

/* called when a client disconnects */
void closeHandler(int clientID){
    ostringstream os;
    os << "Player " << clientID << " has leaved.";

    vector<int> clientIDs = server.getClientIDs();
    for (int i = 0; i < clientIDs.size(); i++){
        if (clientIDs[i] != clientID)
            server.wsSend(clientIDs[i], os.str());
    }
}

/* called when a client sends a message to the server */
void messageHandler(int clientID, string message){
	ostringstream os;
	os << "Player " << clientID << " says: " << message << endl;

	if (message.substr(2).compare("1ID"))
	{
		Player1ID = message;
		os << "Player 1 ID = " + Player1ID.erase(0, 3) << endl;
		server.wsSend(1,Player1ID);
		//server.wsSend(2, Player1ID);
	}

	if (message == "score")
	{
		player1Score++;
		player2Score++;
		os << "Score increase" << player1Score << "     " << player2Score;
	}

	vector<int> clientIDs = server.getClientIDs();
	for (int i = 0; i < clientIDs.size(); i++){
		if (clientIDs[i] != clientID)
			server.wsSend(clientIDs[i], os.str());
	}
}


/* called once per select() loop */
void periodicHandler(){
    static time_t next = time(NULL) + 10;
    time_t current = time(NULL);
    if (current >= next){
        ostringstream os;
        string timestring = ctime(&current);
        timestring = timestring.substr(0, timestring.size() - 1);
        os << timestring;

        vector<int> clientIDs = server.getClientIDs();
        for (int i = 0; i < clientIDs.size(); i++)
            server.wsSend(clientIDs[i], os.str());

        next = time(NULL) + 10;
    }

}

int main(int argc, char *argv[]){
    int port;

    cout << "Please set server port: ";
    cin >> port;

    /* set event handler */
    server.setOpenHandler(openHandler);
    server.setCloseHandler(closeHandler);
    server.setMessageHandler(messageHandler);
	
    //server.setPeriodicHandler(periodicHandler);

    /* start the chatroom server, listen to ip '127.0.0.1' and port '8000' */
    server.startServer(port);

    return 1;
}
