#include <string>
#include <vector>
#include "textparse.hpp"
#ifndef GAME
#define GAME

using namespace std;

struct item
{
	int itemNum;
	string name;
	string description;
	int usedInRoom;
	int canTake;
	int roomNum;
	string useText1;
	string useText2;
	string roomdescription;

};



struct room
{
	string name;
	vector <struct item> items;
	int numItems;
	int roomState;
	string description1;
	string description2;
	string description3;
	/*
	Navigation: 
		0 = north
		1 = east
		2 = west
		3 = south

	*/
	
	int prevRoom;
	int nextRoom;
	// 0 if there isn't a door.  1 if there is.
	int door;
	string nextDoor;
	string prevDoor;
	string hint;

	
};



struct inventory
{
	int numItems;
	
	vector <item> items;
	
};

void inspectItem(string itemName, vector <struct room> &rooms, struct inventory &playerInventory, int currentRoomNum);
void printLongRoomDescription(vector <struct room> &rooms, int currentRoomNum);
void initializeRooms(vector <struct room> &rooms);
struct inventory initializeInventory();
int hasItem(string itemName, struct inventory &playerInventory);
int takeItem(string itemName, struct inventory &playerInventory, vector <struct room> &rooms, int currentRoomNum);
int useItem(string itemName, struct inventory &playerInventory, vector <struct room> &rooms, int currentRoomNum);
void importRoomData(vector <struct room> &rooms, int roomNum);
int goToNextRoom(vector <struct room> &visitedRooms, int &currentRoomNum);
int goToPrevRoom(int &currentRoomNum);
int itemIsInRoom(string itemName, vector <struct room> &rooms, int currentRoomNum);
void importItemData(vector <struct room> &rooms);
void printRoomDescription(vector <struct room> &rooms, int currentRoomNum);
int dropItem(string itemName, struct inventory &playerInventory, vector <struct room> &rooms, int currentRoomNum);
void playGame(vector <struct room> &rooms, struct inventory &playerInventory, int &currentRoomNum);
string formatText(string source, size_t width);

#endif