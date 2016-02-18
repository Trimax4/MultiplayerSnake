#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <sstream>
#include <time.h>
#include <math.h>
#include "websocket.h"

using namespace std;



int player1Score = 0;
int player2Score = 0;
std::string Player1ID;
int clientIDPlayer1;
bool isFirstClient = true;
std::string Player2ID;
int clientIDPlayer2 = 1;
bool isSecondClient = true;

bool p1Ready = false;
bool p2Ready = false;

std::vector<std::pair<int, int>> snake_array1;
std::vector<std::pair<int, int>> snake_array2;
//canvas information
int width = 450;
int height = 450;
int cellWidth = 10;
int nx, ny, nx2, ny2;
int length = 5;
string direction;
string direction2;
std::pair<int, int> food;
std::pair<int, int> tail;
std::pair<int, int> tail2;

webSocket server;

void sendToAll(int clientID, string message)
{
	vector<int> clientIDs = server.getClientIDs();
	for (int i = 0; i < clientIDs.size(); i++) {
		//if (clientIDs[i] != clientID)
			server.wsSend(clientIDs[i], message);
	}
}
void sendToAll(string message)
{
	vector<int> clientIDs = server.getClientIDs();
	for (int i = 0; i < clientIDs.size(); i++) {
		//if (clientIDs[i] != clientID)
		server.wsSend(clientIDs[i], message);
	}
}
/* called when a client connects */
void openHandler(int clientID) {
	ostringstream os;
	os << "Player " << clientID << " has joined.";

	if (isFirstClient == true)
	{
		cout << "client 1 id set" << endl;
		clientIDPlayer1 = clientID;
		isFirstClient = false;
		// send clientID back to user for use in READY button
		server.wsSend(clientIDPlayer1, "ID" + std::to_string(clientIDPlayer1));
	}
	else if (isFirstClient == false)
	{
		cout << "client 2 id set" << endl;
		clientIDPlayer2 = clientID;
		isSecondClient = false;
		server.wsSend(clientIDPlayer2, "ID" + std::to_string(clientIDPlayer2));
	}
	else if (isSecondClient == false && isFirstClient == false)
	{
		os << "server full " << endl;
	}
	
	server.wsSend(clientID, "Welcome!");
	vector<int> clientIDs = server.getClientIDs();
	for (int i = 0; i < clientIDs.size(); i++) {
		if (clientIDs[i] != clientID)     // != ? seems like this should be ==
			server.wsSend(clientIDs[i], os.str());
	}
}

void create_snake1()
{	
	for (int i = length - 1; i >= 0; i--)
	{
		//This will create a horizontal snake starting from the top left
		std::pair<int, int> position;// = new std::pair<int, int>();
		position.first = 10;
		position.second = i;
		snake_array1.push_back(position);
	}
	nx = snake_array1[0].first;
	ny = snake_array1[0].second;
}
void create_snake2()
{
	for (int i = length - 1; i >= 0; i--)
	{
		//This will create a horizontal snake starting from the top left
		std::pair<int, int> position;// = new std::pair<int, int>();
		position.first = 30;
		position.second = i;
		snake_array2.push_back(position);
	}
	nx2 = snake_array2[0].first;
	ny2 = snake_array2[0].second;
}

void create_food()
{
	food.first = round(rand()%(width - cellWidth) / cellWidth);
	food.second = round(rand()%(height - cellWidth) / cellWidth);
	// create string to be interpreted by html script: "FOOD x,y"
	std::string foodLoc = "FOOD" + std::to_string(food.first) + "," + std::to_string(food.second) + ".";
	sendToAll(foodLoc);
	//This will create a cell with x/y between 0-44
	//Because there are 45(450/10) positions accross the rows and columns
}

/* called when a client disconnects */
void closeHandler(int clientID){
    ostringstream os;
    os << "Player " << clientID << " has le.";

    vector<int> clientIDs = server.getClientIDs();
    for (int i = 0; i < clientIDs.size(); i++){
        if (clientIDs[i] != clientID)
            server.wsSend(clientIDs[i], os.str());
    }
}

void snakeMovementLoop()
{
	if (direction.compare ("right")) nx++;
	else if (direction.compare( "left")) nx--;
	else if (direction.compare( "up")) ny--;
	else if (direction.compare("down")) ny++ ;

	if (direction2.compare("right")) nx2++;
	else if (direction2.compare("left")) nx2--;
	else if (direction2.compare("up")) ny2--;
	else if (direction2.compare("down")) ny2++;
}

void foodCollisionCheck()
{
	//Lets write the code to make the snake eat the food
	//The logic is simple
	//If the new head position matches with that of the food,
	//Create a new head instead of moving the tail
	if (nx == food.first && ny == food.second)
	{
		std::pair<int, int> tail;
		tail.first = nx;
		tail.second = ny;
		player1Score++;
		//Create new food
		create_food();
	}
	else
	{
		tail = snake_array1.back();
		snake_array1.pop_back();//pops out the last cell
		tail.first = nx; tail.second = ny;
	}
	if (nx2 == food.first && ny2 == food.second)
	{
		std::pair<int, int> tail2;
		tail2.first = nx2;
		tail2.second = ny2;
		player2Score++;
		//Create new food
		create_food();
	}
	else
	{
		tail2 = snake_array2.back();
		snake_array2.pop_back();//pops out the last cell
		tail2.first = nx2; tail2.second = ny2;
	}
	//The snake can now eat the food.

	snake_array1.insert(snake_array1.begin(),tail); //puts back the tail as the first cell
	snake_array2.insert(snake_array2.begin(), tail2);
}

bool check_collision(int x, int y, std::vector<std::pair<int, int>> array)
{
	//This function will check if the provided x/y coordinates exist
	//in an array of cells or not
	for (int i = 0; i < array.size(); i++)
	{
		if (array[i].first == x && array[i].second == y)
		{
			return true;
		}
	}
	return false;
}

bool check_collision_array(std::vector<std::pair<int, int>> array, std::vector<std::pair<int, int>> array2)
{
	for (int i = 0; i < array.size(); i++)
	{
		for (int k = 0; k < array2.size(); k++)
		{
			if (array[i].first == array2[k].first  && array[i].second == array2[k].second)
			{
				return true;
			}
		}
	}
	return false;
}


bool gameOverChecker()
{
	vector<int> clientIDs = server.getClientIDs();

	if (nx == -1 || nx == width / cellWidth || ny == -1 || ny == height / cellWidth || check_collision(nx, ny, snake_array1))
	{
		return true;
	}
	if (nx2 == -1 || nx2 == width / cellWidth || ny2 == -1 || ny2 == height / cellWidth || check_collision(nx2, ny2, snake_array2))
	{
		return true;
	}
	if (check_collision_array(snake_array1, snake_array2))
	{
		return true;
	}
	return false;
}

// all players have clicked on the Ready button
bool allPlayersReady() {
	return p1Ready && p2Ready;
}

void gameOver() {
	if (gameOverChecker()) {
		// stop snakes
		p1Ready = false;
		p2Ready = false;
		sendToAll("GAMEOVER " + std::to_string(player1Score) + "," + std::to_string(player2Score));
	}
}

/* called when a client sends a message to the server */
void messageHandler(int clientID, string message){
	ostringstream os;
	cout << clientID << "Message in " << message << endl;
	vector<int> clientIDs = server.getClientIDs();
	cout << clientID  << "    < = current client || " << clientIDPlayer1 << " < player 1 ID and player 2 id > " << clientIDPlayer2 << endl;



	if (clientID == clientIDPlayer1)
	{
		os << "direction change p1" << endl ;
		if ((message.compare("left") == 0 && direction.compare("right") != 0) || 
			(message.compare("down") == 0 && direction.compare("up") != 0) || 
			(message.compare("up") == 0 && direction.compare("down") != 0) || 
			(message.compare("right") == 0) && direction.compare("left") != 0)
		{
			direction2 = message;
			sendToAll(clientID, "P1D" + message);
		}
	}
	else if (clientID == clientIDPlayer2)
	{
		os << "direction change p2" << endl;
		if ((message.compare("left") == 0 && direction.compare("right") != 0) ||
			(message.compare("down") == 0 && direction.compare("up") != 0) ||
			(message.compare("up") == 0 && direction.compare("down") != 0) ||
			(message.compare("right") == 0) && direction.compare("left") != 0)
		{
			direction2 = message;

			sendToAll(clientID, "P2D" + message);
		}
	}

	if (message.size() >= 3)
	{
		if (message.substr(2).compare("1ID"))
		{
			Player1ID = message;
			//os << "Player 1 ID = " + Player1ID.erase(0, 3) << endl;
			sendToAll(clientID, Player1ID);
		}


		if (message.substr(2).compare("2ID"))
		{
			Player2ID = message;
			//os << "Player 1 ID = " + Player1ID.erase(0, 3) << endl;
			sendToAll(clientID, Player2ID);
		}

		if (message.compare("READY") == 0) {
			if (clientID == 0) {
				p1Ready = true;
				cout << clientID << "players 1 ready" << endl;
			}
			else
			{
				p2Ready = true;
				cout << clientID << "players 2 ready" << endl;
			}
			if (allPlayersReady() == true)
			{
				cout << clientID << "players all ready"<< endl;
				create_food();
				sendToAll("START");
				cout << clientID << "GAME STARTED" << endl;
			}
		}
	}
	
		//vector<int> clientIDs = server.getClientIDs();
		for (int i = 0; i < clientIDs.size(); i++) {
			//if (clientIDs[i] != clientID)
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
