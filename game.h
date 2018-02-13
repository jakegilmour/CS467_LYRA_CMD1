#include <string>
#include <vector>

#ifndef GAME
#define GAME

using namespace std;

struct item
{
	string name;
	string description;
	int usedInRoom;
	int canTake;
	int roomNum;
	string command;

};



struct room
{
	string name;
	struct item item1;
	struct item item2;
	struct item item3;
	int roomState;
	string description1;
	string description2;
	string description3;
	
};



struct inventory
{
	int numItems;
	
	vector <item> items;
	
};

void inspectItem(string itemName, vector <struct room> &rooms, struct inventory &playerInventory, int currentRoomNum);
void initializeRooms(vector <struct room> &rooms);
struct inventory initializeInventory();
int hasItem(string itemName, struct inventory &playerInventory);
int takeItem(string itemName, struct inventory &playerInventory, vector <struct room> &rooms, int currentRoomNum);
int useItem(struct item item, struct inventory &playerInventory, int currentRoomNum);
void saveGame(struct inventory playerInventory, int numRoomsVisited);
void importRoomData(vector <struct room> &rooms, int roomNum);
int goToNextRoom(vector <struct room> &visitedRooms, int &currentRoomNum);
int goToPrevRoom(int &currentRoomNum);
int itemIsInRoom(string itemName, vector <struct room> &rooms, int currentRoomNum);




#endif