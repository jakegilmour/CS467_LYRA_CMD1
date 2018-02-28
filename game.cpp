#include <iostream>
#include <string>
#include <vector>
#include "game.h"
#include <fstream>

#define NUMROOMS 15
#define NUMITEMS 35



using namespace std;


int main()
{
	struct inventory playerInventory = initializeInventory();
	vector <struct room> rooms(NUMROOMS);
	initializeRooms(rooms);
	importItemData(rooms);
	
	
	for (int i = 0; i < NUMROOMS; i++)
	{
		
		cout << endl;
		cout << "**** ROOM # " << i << " ****" << endl;
		cout << rooms[i].name << endl;
		cout << rooms[i].items[0].name << endl;
		cout << rooms[i].items[1].name << endl;
		cout << rooms[i].items[2].name << endl;
		cout << rooms[i].description1  << endl;
		cout << rooms[i].description2  << endl;
		cout << rooms[i].description3  << endl;
		
	}
	

	
	string window = "window";
	int test1 = itemIsInRoom(window, rooms, 0);
	int test2 = itemIsInRoom("window1", rooms, 0);
	
	cout << window.length() << " " << rooms[0].items[0].name.length() << endl;
	
	cout << endl;
	cout << "***** Testing itemIsInRoom ******" << endl;
	cout << "Expected value for first test = 1.  Actual = " << test1 << endl;
	cout << "Expected value for second test = -1.  Actual = " << test2 << endl;
	
	
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
	inspectItem(window, rooms, playerInventory, 4);
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
		
		cout << " item found  in inv" << playerInventory.items[itemInInv].description << endl;
		
	}
	else if (isInRoom >= 0)
	{

		cout << " item found in room" << rooms[currentRoomNum].items[isInRoom].description << endl;
		
	}
	else
	{
		cout << "You are looking for something...but it doesn't appear to be in this room or in your inventory" << endl;
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

int dropItem(string itemName, struct inventory &playerInventory, vector <struct room> &rooms, int currentRoomNum)
{
	
	int itemInInv = hasItem(itemName, playerInventory);
	
	if (itemInInv == -1)
	{
		cout << "You cannot drop an item that you don't have in your inventory" << endl;
		
	}
	else
	{
		
		rooms[currentRoomNum].items.push_back(playerInventory.items[itemInInv]);
		rooms[currentRoomNum].numItems += 1;
		playerInventory.items.erase( playerInventory.items.begin() + itemInInv);
		playerInventory.numItems -= 1;
		
	}
	
	
	
	
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
		cout << "Hmmm....It appears there is no " << itemName << " in this room.  Are you sure that is what you were looking for?" << endl;
		return 0;
		
	}
	else if (rooms[currentRoomNum].items[isInRoom].canTake == 0)
	{
		cout << "It doesn't seem like you can take that." << endl;
		return 0;
		
	}
	else
	{
		playerInventory.items[playerInventory.numItems] = rooms[currentRoomNum].items[isInRoom];
		playerInventory.numItems += 1;
		rooms[currentRoomNum].items.erase(rooms[currentRoomNum].items.begin() + isInRoom);
		rooms[currentRoomNum].numItems -= 1;
		return 1;

	}

	
}


/*
Function: useItem()
Args: item to use, player inventory, number of the room that the player is in
Returns: 0 if item wasn't successfully used, 1 if it was
*/
int useItem(string itemName, struct inventory &playerInventory, vector <struct room> &rooms, int currentRoomNum)
{
	

	int isInRoom = itemIsInRoom(itemName, rooms, currentRoomNum);
	int itemInInv = hasItem(itemName, playerInventory);
	
	
	if (isInRoom >= 0)
	{
		if (rooms[currentRoomNum].items[isInRoom].usedInRoom == currentRoomNum)
		{
			cout << "item succesfully used" << endl;
			rooms[currentRoomNum].roomState == 1;
			return 1;
			
		}
		else
		{
			cout << "You can't use that here" << endl;
			return 0;
			
		}
	}
	else if (itemInInv >= 0)
	{
		if (playerInventory.items[itemInInv].usedInRoom == currentRoomNum)
		{
			cout << "item succesfully used" << endl;
			rooms[currentRoomNum].roomState == 1;
			return 1;
			
		}
		else
		{
			cout << "You can't use that here" << endl;
			return 0;
			
		}
		
	}
	else
	{
		cout << "The item you are trying to use is not in the room or in your inventory" << endl;		
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
				rooms[roomNum].description1 = fileLine;
				rooms[roomNum].description1.erase(rooms[roomNum].description1.length() - 1);
			}
			else if (lineCounter == 3)
			{
				rooms[roomNum].description2 = fileLine;
				rooms[roomNum].description2.erase(rooms[roomNum].description2.length() - 1);
			}
			else if (lineCounter == 4)
			{
				rooms[roomNum].description3 = fileLine;
				rooms[roomNum].description3.erase(rooms[roomNum].description3.length() - 1);
			}
			
			lineCounter++;
			
		}
		
		rooms[roomNum].numItems = 0;
		rooms[roomNum].items.resize(15);
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
		visitedRooms[currentRoomNum].roomState = 2;
		currentRoomNum++;
		return 1;
		
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
Returns: -1 if the item is not in the room.  Otherwise returns index of where the item is in the room's items vector


*/

int itemIsInRoom(string itemName, vector <struct room> &rooms, int currentRoomNum)
{
	
	for (int i = 0; i < rooms[currentRoomNum].numItems; i++)
	{
		if (itemName.compare(rooms[currentRoomNum].items[i].name) == 0)
			return i;
		
	}
	
		return -1;

}

/*
Function: printRoomDescription()
Args: vector <struct room> rooms, int currentRoomNum
Prints the description of the room the player is in based on room state


*/
void printRoomDescription(vector <struct room> &rooms, int currentRoomNum)
{
	
	switch (rooms[currentRoomNum].roomState)
	{
		case 1: 
			cout << rooms[currentRoomNum].description1 << endl;
			break;
		case 2: 
			cout << rooms[currentRoomNum].description2 << endl;
			break;
		case 3: 
			cout << rooms[currentRoomNum].description3 << endl;
			break;

	}
	
	cout << "In this room you notice a ";
	
	for (int i = 0; i < rooms[currentRoomNum].numItems; i++)
	{
		if (i == 0)
		{
			cout << rooms[currentRoomNum].items[i].name;
		}
		else
		{
			cout << ", a " << rooms[currentRoomNum].items[i].name;
		}
		
		
	}
	
	cout << endl;
	
	
}

/*
Function: playGame()
Args: string item name, vector <struct room> rooms, currentRoomNum
Returns: -1 if the item is not in the room.  Otherwise returns index of where the item is in the room's items vector


*/

void playGame(vector <struct room> &rooms, struct inventory &playerInventory)
{
	
	int currentRoomNum = 0;
	vector <string> commands;
	
	while(1){
		commands = textParse();

			
		if (commands[0] == "look")
		{
			printRoomDescription(rooms, currentRoomNum);
			
		}
		else if (commands[0] == "inspect")
		{
			inspectItem(commands[1], rooms, playerInventory, currentRoomNum);

		}
		else if (commands[0] == "go")
		{
			if (commands[1] == "next")
			{
				goToNextRoom(rooms, currentRoomNum);
			}
			else if (commands[1] == "prev")
			{
				goToPrevRoom(currentRoomNum);
			}

		}
		else if (commands[0] == "take")
		{
			takeItem(commands[1], playerInventory, rooms, currentRoomNum);

		}
		else if (commands[0] == "use")
		{
			int useItem(struct item item, struct inventory &playerInventory, int currentRoomNum);

		}
		else if (commands[0] == "help")
		{
			cout << "help commands" << endl;

		}
		else if (commands[0] == "savegame")
		{
			// gamestate {currentRoomNum, currentRoomState, 

		}
		else if (commands[0] == "loadgame")
		{
			cout << "help commands" << endl;

		}
		else if (commands[0] == "inventory")
		{
			cout << "Inventory contents: ";
			
			for (int i = 0; i < playerInventory.numItems; i++)
			{
				if (i == 0)
				{
					cout << playerInventory.items[i].name;
				}
				else
				{
					cout << ", " << playerInventory.items[i].name;
				}
				cout << endl;
				
			}

		}
		else
		{
			/*Special handler verbs here */
			
			
			
			
		}

			
			
		} // end while (1)



	
}

/*
Function: importItemData()
Args: vector storing rooms previously visited by player, number of rooms they have visited.
*/


void importItemData(vector <struct room> &rooms)
{

		string itemFileName = "./itemdata/item";
		string fileLine;
		
		for (int k = 0; k < 1; k++)
		{
		
			itemFileName += to_string(k);
			itemFileName += ".txt";
			ifstream inFile;
			inFile.open(itemFileName);
			int lineCounter = 1;
			
			struct item newItem;
			
			/* Check for error loading the room file */
			
			if (!inFile) {
				cerr << "Unable to open " << itemFileName;
				exit(1);   // call system to stop
			}			


			while( getline(inFile,fileLine) )
			{
				
				if (lineCounter == 1)
				{
					newItem.name = fileLine;
					newItem.name.erase(newItem.name.length() - 1);
				}

				else if (lineCounter == 2)
				{
					newItem.description = fileLine;
					newItem.description.erase(newItem.description.length() - 1);
				}
				else if (lineCounter == 3)
				{
					fileLine.erase(fileLine.length() - 1);
					newItem.usedInRoom = stoi(fileLine);
				}
				else if (lineCounter == 4)
				{
					fileLine.erase(fileLine.length() - 1);
					newItem.canTake = stoi(fileLine);
				}
				else if (lineCounter == 5)
				{
					fileLine.erase(fileLine.length() - 1);
					newItem.roomNum = stoi(fileLine);
				}
				else if (lineCounter == 6)
				{
					newItem.command = fileLine;
					newItem.command.erase(newItem.command.length() - 1);
				}
				
				lineCounter++;
				
			}

			inFile.close();
			
			rooms[newItem.roomNum].items[rooms[newItem.roomNum].numItems] = newItem;
			rooms[newItem.roomNum].numItems++;
		
		}

	
}







