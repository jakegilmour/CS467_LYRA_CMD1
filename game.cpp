#include <iostream>
#include <string>
#include <vector>
#include "game.h"
#include <fstream>

#define NUMROOMS 15



using namespace std;


int main()
{
	struct inventory playerInventory = initializeInventory();
	vector <struct room> rooms(NUMROOMS);
	initializeRooms(rooms);
	
	for (int i = 0; i < NUMROOMS; i++)
	{
		
		cout << endl;
		cout << "**** ROOM # " << i << " ****" << endl;
		cout << rooms[i].name << endl;
		cout << rooms[i].item1.name << endl;
		cout << rooms[i].item2.name << endl;
		cout << rooms[i].item3.name << endl;
		cout << rooms[i].description1  << endl;
		cout << rooms[i].description2  << endl;
		cout << rooms[i].description3  << endl;
		
	}
	
	
	string window = "window";
	int test1 = itemIsInRoom(window, rooms, 0);
	int test2 = itemIsInRoom("window1", rooms, 0);
	
	cout << window.length() << " " << rooms[0].item1.name.length() << endl;
	
	cout << endl;
	cout << "***** Testing itemIsInRoom ******" << endl;
	cout << "Expected value for first test = 2.  Actual = " << test1 << endl;
	cout << "Expected value for second test = 0.  Actual = " << test2 << endl;
	
	
	playerInventory.items[0].name = "test item";
	playerInventory.numItems = 1;
	
	
	
	cout << "***** Testing hasItem ******" << endl;
	cout << "Expected value for first test = 0.  Actual = " << hasItem("test item", playerInventory) << endl;
	cout << "Expected value for second test = -1.  Actual = " << hasItem("test item   2323", playerInventory) << endl;
	
	cout << "***** Testing inspectItem ******" << endl;
	cout << "Testing item that is inspectable in inventory ";
	cout << endl;
	inspectItem("test item", rooms, playerInventory, 4);
	cout << "Testing item that is inspectable in room ";
	cout << endl;
	inspectItem(window, rooms, playerInventory, 0);
	cout << "Testing item that cannot be inspected";
	cout << endl;
	inspectItem(window, rooms, playerInventory, 2);
	cout << endl;
	
	

	return 0;
}

/*Function: inspectItem()
Args: item to inspect, player inventory, current room number
Returns: N/A

Prints out the item description if the player has it or if it is in the current room.  If the player doesn't have the item or it isn't in the room
an error is printed

*/

void inspectItem(string itemName, vector <struct room> &rooms, struct inventory &playerInventory, int currentRoomNum)
{
	int isInRoom = itemIsInRoom(itemName, rooms, currentRoomNum);
	int itemInInv = hasItem(itemName, playerInventory);
	
	
	if (itemInInv >= 0)
	{
		
		cout << " item found " << playerInventory.items[itemInInv].description << endl;
		
	}
	else if (isInRoom > 0)
	{
		if (isInRoom == 1)
		{
			cout << " item found " << rooms[currentRoomNum].item1.description << endl;
		}
		else if (isInRoom == 2)
		{
			cout << " item found " << rooms[currentRoomNum].item2.description << endl;
		}
		else if (isInRoom == 3)
		{
			cout << " item found " << rooms[currentRoomNum].item3.description << endl;
		}
		
	}
	else
	{
		cout << "You are looking for something...but it doesn't appear to be here" << endl;
	}
	
	

}

/*
Function: initializeRooms()
Args: vector <struct room> rooms - vector that will store room data
Returns: N/A
*/

void initializeRooms(vector <struct room> &rooms)
{
	

	for (int i = 0; i < NUMROOMS; i++)
	{
		importRoomData(rooms, i);
		
	}

	
}

/*
Function: initializeInventory()
Args: None
Returns: allocates memory for the player inventory and returns the inventory structure.
*/

struct inventory initializeInventory()
{
	
	struct inventory playerInventory;
	playerInventory.numItems = 0;
	playerInventory.items.resize(10);
	return playerInventory;
	
	
}

/*
Function: hasItem()
Args: item to check, player inventory
Returns: -1 if the player doesn't have the item, or returns the index of where the item is stored in the inventory
*/

int hasItem(string itemName, struct inventory &playerInventory)
{
	
	int i = 0;
	for (i; i < playerInventory.numItems; i++)
	{
		
		if (itemName.compare(playerInventory.items[i].name) == 0)
			return i;
		
	}
	
	return -1;

	
}



/*
Function: takeItem()
Args: item to add to inventory, player inventory
Returns: 0 if item wasn't added to inventory, 1 if it was
*/
int takeItem(string itemName, struct inventory &playerInventory, vector <struct room> &rooms, int currentRoomNum)
{
	
	int isInRoom = itemIsInRoom(itemName, rooms, currentRoomNum);
	struct item item;
	
	/* Make sure that the player doesn't already have the item */
	if (hasItem(itemName, playerInventory) == 1)
	{
		cout << "You already have " << itemName << " in your inventory";
		return 0;
		
	}
	else if (isInRoom == -1)
	{
		cout << "Hmmm....It appears there is no " << itemName<< " in this room.  Are you sure that is what you were looking for?" << endl;
		return 0;
		
	}
	else if (rooms[currentRoomNum].item1.canTake == 0 && isInRoom == 1 || rooms[currentRoomNum].item2.canTake == 0 && isInRoom == 2 || rooms[currentRoomNum].item3.canTake == 0 && isInRoom == 3 )
	{
		cout << "It doesn't seem like you can take that." << endl;
		return 0;
		
	}
	else
	{
		if (isInRoom == 1)
		{
			playerInventory.items[playerInventory.numItems] = rooms[currentRoomNum].item1;
		}
		if (isInRoom == 2)
		{
			playerInventory.items[playerInventory.numItems] = rooms[currentRoomNum].item2;
		}
		if (isInRoom == 3)
		{
			playerInventory.items[playerInventory.numItems] = rooms[currentRoomNum].item3;
		}

		playerInventory.numItems++;
		return 1;

	}

	
}


/*
Function: useItem()
Args: item to use, player inventory, number of the room that the player is in
Returns: 0 if item wasn't successfully used, 1 if it was
*/
int useItem(struct item item, struct inventory &playerInventory, int currentRoomNum)
{
	

	int use = 0;
	if (item.usedInRoom == currentRoomNum)
	{
		use = 1;
	}


	return use;

}



void saveGame(struct inventory playerInventory, int numRoomsVisited)
{
	
	int i;
	
	/* Write to textfile numRooms Visited */
	
	for (i = 0; i < numRoomsVisited; i++)
	{
		/* write to textfile room interaction states, room state, and unlocked value */
		
		
	}
	
	
	
	for (i = 0; i < playerInventory.numItems; i++)
	{
		/* write to textfile num items and item names */
		
	}
	

	
}

/*
Function: importRoomData()
Args: vector storing rooms previously visited by player, number of rooms they have visited.
Returns: 1 if the player has the item, 0 if they don't
*/
void importRoomData(vector <struct room> &rooms, int roomNum)
{

		string roomFileName = "./roomdata/room";
		string fileLine;
		roomFileName += to_string(roomNum);
		roomFileName += ".txt";
		ifstream inFile;
		inFile.open(roomFileName);
		int lineCounter = 1;
		
		/* Check for error loading the room file */
		if (!inFile) {
			cerr << "Unable to open " << roomFileName;
			exit(1);   // call system to stop
		}			


		while( getline(inFile,fileLine) )
		{
			
			if (lineCounter == 1)
			{
				rooms[roomNum].name = fileLine;
				rooms[roomNum].name.erase(rooms[roomNum].name.length() - 1);
			}
			else if (lineCounter == 2)
			{
				rooms[roomNum].item1.name = fileLine;
				rooms[roomNum].item1.name.erase(rooms[roomNum].item1.name.length() - 1);
			}
			else if (lineCounter == 3)
			{
				rooms[roomNum].item2.name = fileLine;
				rooms[roomNum].item2.name.erase(rooms[roomNum].item2.name.length() - 1);
			}
			else if (lineCounter == 4)
			{
				rooms[roomNum].item3.name = fileLine;
				rooms[roomNum].item3.name.erase(rooms[roomNum].item3.name.length() - 1);
			}
			else if (lineCounter == 5)
			{
				rooms[roomNum].description1 = fileLine;
				rooms[roomNum].description1.erase(rooms[roomNum].description1.length() - 1);
			}
			else if (lineCounter == 6)
			{
				rooms[roomNum].description2 = fileLine;
				rooms[roomNum].description2.erase(rooms[roomNum].description2.length() - 1);
			}
			else if (lineCounter == 7)
			{
				rooms[roomNum].description3 = fileLine;
				rooms[roomNum].description3.erase(rooms[roomNum].description3.length() - 1);
			}
			
			lineCounter++;
			
		}
		
		
		rooms[roomNum].roomState = 0;
		
		inFile.close();

	
}

/*
Function: goToNextRoom()
Args: vector storing rooms previously visited by player, number of rooms they have visited.
Returns: 1 if the player successfully goes to the next room, 0 if they do not
*/
int goToNextRoom(vector <struct room> &visitedRooms, int &currentRoomNum)
{
	
	if (visitedRooms[currentRoomNum].roomState = 0)
	{
		cout << "Hmmm, you aren't able to go there yet.  Maybe there is something that you must do first..." << endl;
		return 0;				
	}	
	else
	{
		currentRoomNum++;
		
	}

}

/*
Function: goToPrevRoom()
Args: current room number
Returns: 1 if the player successfully goes to the previous room, 0 if they do not

Moves the player back 1 room if they aren't in the first room.

*/
int goToPrevRoom(int &currentRoomNum)
{
	
	if (currentRoomNum == 0)
	{
		cout << "You can't go backwards from here." << endl;
		return 0;				
	}	
	else
	{
		currentRoomNum--;
		return 1;
		
	}

}


/*
Function: itemIsInRoom()
Args: string item name, vector <struct room> rooms, currentRoomNum
Returns: 0 if the item is not in the room.  Returns 1, 2, or 3 if the item is in the room.  The return value corresponds
to the item number


*/

int itemIsInRoom(string itemName, vector <struct room> &rooms, int currentRoomNum)
{
	
	if (itemName.compare(rooms[currentRoomNum].item1.name) == 0)
		return 1;
	else if (itemName.compare(rooms[currentRoomNum].item2.name) == 0)
		return 2;
	else if (itemName.compare(rooms[currentRoomNum].item3.name) == 0)
		return 3;
	else
		return 0;

}

