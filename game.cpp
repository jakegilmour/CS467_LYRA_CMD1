#include <iostream>
#include <string>
#include <vector>
#include "game.h"
#include <fstream>
#include "textparse.hpp"
#include <sys/ioctl.h>
#include "load_game.hpp"
#include <dirent.h>
#include <fstream>
#include <sys/stat.h>
#include "save_game.hpp"
#include <unistd.h>
#include <sstream>
#include <boost/algorithm/string.hpp>
#include <time.h>


#define NUMROOMS 15
#define NUMITEMS 48



using namespace std;


int main()
{
	struct inventory playerInventory = initializeInventory();
	vector <struct room> rooms(NUMROOMS);
	initializeRooms(rooms);
	importItemData(rooms);
	int currentRoomNum = 0;
	string userInput;
	
	
	struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
	string windowError = "For the best gaming experience this game requires a window with a row size of 40 characters or greater and a column size of 120 characters or greater.  Please resize your window to play the game.";
	
	/* Check to make sure that console meets minimum size requirements of 120x40 */
	
	if (w.ws_row < 40 || w.ws_col < 120)
	{
		windowError = formatText(windowError, w.ws_col);
		cout << windowError << endl;
		exit(1);
	}
	
	
	
	
	
	
	
	do{
		cout << "Welcome to the Castle!" << endl;
		cout << "Enter 1 to start a new game" << endl;
		cout << "Enter 2 to load a saved game" << endl;
		
		
		
		getline(cin, userInput);
		
		if (userInput == "1")
		{
			cout << endl;
			cout << "Welcome to the dungeon.  Press help if you need some assistance.  Other than that, you are on your own..." << endl;
			cout << endl;
			cout << endl;

			
			printRoomDescription(rooms, currentRoomNum);
			rooms[0].roomState = 1;
			playGame(rooms, playerInventory, currentRoomNum);
			
		}
		else if (userInput == "2")
		{
			
			load_game(currentRoomNum, playerInventory, rooms);
			printRoomDescription(rooms, currentRoomNum);
			playGame(rooms, playerInventory, currentRoomNum);
			
		}
		else
		{
			cout << "You did not enter a valid command." << endl << endl;
			
		}
	
	}while (userInput != "1" || userInput != "2");
	
	
	
	
	
	
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
	
	
	struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
	
	string description; 
	
	

	
	if (itemInInv >= 0)
	{
		
		description = playerInventory.items[itemInInv].description;
		description = formatText(description, w.ws_col);
		cout << description << endl << endl;
		
	}
	else if (isInRoom >= 0)
	{

		description = rooms[currentRoomNum].items[isInRoom].description;
		description = formatText(description, w.ws_col);
		cout << description << endl << endl;
		
	}
	else
	{
		cout << "You aren't having any luck looking at that." << endl << endl;
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
		cout << "You cannot drop an item that you don't have in your inventory" << endl << endl;
		
	}
	else
	{
		cout << "You successfully dropped " << itemName << ".  It is now in the " << rooms[currentRoomNum].name << endl << endl;
		rooms[currentRoomNum].items[rooms[currentRoomNum].numItems] = playerInventory.items[itemInInv];
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
		cout << "You already have " << itemName << " in your inventory" << endl << endl;
		return 0;
		
	}
	else if (isInRoom == -1)
	{
		cout << "Hmmm....It appears there is no " << itemName << " in this room.  Are you sure that is what you were looking for?" << endl << endl;
		return 0;
		
	}
	else if (rooms[currentRoomNum].items[isInRoom].canTake == 0)
	{
		cout << "It doesn't seem like you can take that." << endl << endl;
		return 0;
		
	}
	else
	{
		cout << "You successfully grabbed " << itemName << ".  It is now in your inventory" << endl << endl;
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
	

	
	int itemInInv = hasItem(itemName, playerInventory);

	
	if (itemInInv >= 0)
	{
		
		if (playerInventory.items[itemInInv].usedInRoom == 40)
		{
			//Castle Map
			if (currentRoomNum < 14)
			{
				
				if (rooms[currentRoomNum].nextRoom == 0)
				{
					cout << playerInventory.items[itemInInv].useText1 << " " << rooms[currentRoomNum + 1].name << " to the North" << endl << endl;
					return 1;
					
				}
				else if (rooms[currentRoomNum].nextRoom == 1)
				{
					cout << playerInventory.items[itemInInv].useText1 << " " << rooms[currentRoomNum + 1].name << " to the East" << endl << endl;
					return 1;
					
				}
				else if (rooms[currentRoomNum].nextRoom == 2)
				{
					cout << playerInventory.items[itemInInv].useText1 << " " << rooms[currentRoomNum + 1].name << " to the West" << endl << endl;
					return 1;
					
				}
				else
				{
					cout << playerInventory.items[itemInInv].useText1 << " " << rooms[currentRoomNum + 1].name << " to the South" << endl << endl;
					return 1;
					
				}
				
				
			}
			else
			{
				cout << playerInventory.items[itemInInv].useText2 << endl << endl;
				return 0;
			}
			
		}
		else if (playerInventory.items[itemInInv].usedInRoom == 50)
		{
			//sword
			if (currentRoomNum == 5 || currentRoomNum == 6 || currentRoomNum == 9)
			{
				cout << playerInventory.items[itemInInv].useText1 << endl << endl;
				rooms[currentRoomNum].roomState = 2;
				return 1;
			}
			else
			{
				cout << "Using that here doesn't seem wise" << endl << endl;
				return 0;
			}
			
		}
		else if (playerInventory.items[itemInInv].usedInRoom == 100)
		{
			//sword
			if (currentRoomNum == 1 || currentRoomNum == 4 || currentRoomNum == 12)
			{
				cout << playerInventory.items[itemInInv].useText1 << endl << endl;
				
				/*for (int i = 0; i < rooms[currentRoomNum].numItems; i++)
				{
					
					if (rooms[currentRoomNum].items[i].name == "guard")
					{
						
						rooms[currentRoomNum].items.erase(rooms[currentRoomNum].items.begin() + i);
						rooms[currentRoomNum].numItems--;
					}
					
					
				}*/
				
				rooms[currentRoomNum].roomState = 2;
				return 1;
			}
			else
			{
				cout << "Using that here doesn't seem wise" << endl << endl;
				return 0;
			}
			
		}
		else if (playerInventory.items[itemInInv].usedInRoom == 60)
		{
			//bow and arrow
			if (currentRoomNum == 8 || currentRoomNum == 10 || currentRoomNum == 13)
			{
				cout << playerInventory.items[itemInInv].useText1 << endl << endl;
				rooms[currentRoomNum].roomState = 2;
				
				/*for (int i = 0; i < rooms[currentRoomNum].numItems; i++)
				{
					
					if (rooms[currentRoomNum].items[i].name == "guard")
					{
						
						rooms[currentRoomNum].items.erase(rooms[currentRoomNum].items.begin() + i);
						rooms[currentRoomNum].numItems--;
					}
					
					
				}*/
					
					
				return 1;
			}
			else if (currentRoomNum == 14)
			{
				
				cout << playerInventory.items[itemInInv].useText2 << endl << endl;
				rooms[currentRoomNum].roomState = 2;
				return 1;
				
			}
			else
			{
				cout << "Using that here doesn't seem wise" << endl << endl;
				return 0;
			}
			
		}
		else if (playerInventory.items[itemInInv].usedInRoom == currentRoomNum)
		{
			
				cout << playerInventory.items[itemInInv].useText1 << endl << endl;
				rooms[currentRoomNum].roomState = 2;
				return 1;

		}
		else
		{
			cout << "You can't use that here" << endl << endl;
			return 0;
			
		}
		
	}
	else
	{
		cout << "The item you are trying to use is not in your inventory" << endl << endl;		
		return 0;
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
			else if (lineCounter == 5)
			{
				fileLine.erase(fileLine.length() - 1);
				rooms[roomNum].prevRoom = stoi(fileLine);
			}
			else if (lineCounter == 6)
			{
				fileLine.erase(fileLine.length() - 1);
				rooms[roomNum].nextRoom = stoi(fileLine);
			}
			else if (lineCounter == 7)
			{
				fileLine.erase(fileLine.length() - 1);
				rooms[roomNum].door = stoi(fileLine);
				
			}
			else if (lineCounter == 8)
			{
				rooms[roomNum].nextDoor = fileLine;
				rooms[roomNum].nextDoor.erase(rooms[roomNum].nextDoor.length() - 1);
			}
			else if (lineCounter == 9)
			{
				rooms[roomNum].prevDoor = fileLine;
				rooms[roomNum].prevDoor.erase(rooms[roomNum].prevDoor.length() - 1);
			}
			else if (lineCounter == 10)
			{
				rooms[roomNum].hint = fileLine;
				rooms[roomNum].hint.erase(rooms[roomNum].hint.length() - 1);
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
	
	if (visitedRooms[currentRoomNum].roomState == 0 || visitedRooms[currentRoomNum].roomState == 1)
	{
		cout << "Hmmm, you aren't able to go there yet.  Maybe there is something that you must do first..." << endl << endl;
		return 0;				
	}	
	else
	{
		currentRoomNum++;
		printRoomDescription(visitedRooms, currentRoomNum);
		if (visitedRooms[currentRoomNum].roomState == 0)
		{
			visitedRooms[currentRoomNum].roomState = 1;
		}
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
		cout << "You can't go backwards from here." << endl << endl;
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
Function: printLongRoomDescription()
Args: vector <struct room> rooms, int currentRoomNum
Prints the long form description of the room

*/
void printLongRoomDescription(vector <struct room> &rooms, int currentRoomNum)
{
	
	
	struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
	
	string description = rooms[currentRoomNum].description1;
	
	
	for (int i = 0; i < rooms[currentRoomNum].numItems; i++)
	{
		description += "  ";
		description += rooms[currentRoomNum].items[i].roomdescription;
		

	}
	
	description = formatText(description, w.ws_col);
	
	cout << description << endl << endl;
	
	
}






/*
Function: printRoomDescription()
Args: vector <struct room> rooms, int currentRoomNum
Prints the description of the room the player is in based on room state


*/
void printRoomDescription(vector <struct room> &rooms, int currentRoomNum)
{
	
	struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
	
	string description;
	
	switch (rooms[currentRoomNum].roomState)
	{
		case 0: 
			description = rooms[currentRoomNum].description1;
			break;
		case 1: 
			description = rooms[currentRoomNum].description2;
			break;
		case 2: 
			description = rooms[currentRoomNum].description3;
			break;

	}
	

		
		for (int i = 0; i < rooms[currentRoomNum].numItems; i++)
		{
				
				if (currentRoomNum == 0 && rooms[currentRoomNum].items[i].name == "dungeon key")
				{
					
				}
				else
				{
					description += "  ";
					description += rooms[currentRoomNum].items[i].roomdescription;
				}
					
					

			
		}

	description = formatText(description, w.ws_col);
	
	cout << description << endl << endl;
	
}

/*
Function: playGame()
Args: string item name, vector <struct room> rooms, currentRoomNum
Returns: -1 if the item is not in the room.  Otherwise returns index of where the item is in the room's items vector


*/

void playGame(vector <struct room> &rooms, struct inventory &playerInventory, int &currentRoomNum)
{

	
	string userInput;
	vector <string> commands;
	
	
	while(1){
		getline(cin, userInput);
		
		cout << endl;
		commands = textParse(userInput);
	
		

		
		if (commands.size() == 0)
		{
			cout << "Please enter a new command" << endl << endl;

		}	
		else if (commands[0] == "hint")
		{
			cout << rooms[currentRoomNum].hint << endl << endl;
			
		}
		else if (commands[0] == "look")
		{
			printLongRoomDescription(rooms, currentRoomNum);
			
		}
		else if (commands[0] == "inspect" || commands[0] == "look at")
		{
			inspectItem(commands[1], rooms, playerInventory, currentRoomNum);

		}
		else if (commands[0] == "go")
		{
				
			if (currentRoomNum == 0)
			{
				
				if (commands[1] == "next" || commands[1] == rooms[currentRoomNum].nextDoor || commands[1] == "east" || commands[1] == "door")
				{
					goToNextRoom(rooms, currentRoomNum);
						
				}
				else
				{
					cout << "It seems like you are trying to go the wrong way" << endl << endl;
				}

			}
			else
			{
				if (rooms[currentRoomNum].door == 1){		
					if (commands[1] == "door")
					{
						if (rooms[currentRoomNum - 1].door == 1)
						{
							cout << "You need to be more specific.  There is more than one door here." << endl << endl;
						}
						else
						{
							goToNextRoom(rooms, currentRoomNum);
						}
						
					}			
					else if (commands[1] == "next")
					{
						goToNextRoom(rooms, currentRoomNum);
						
					}
					else if (commands[1] == "prev")
					{
						goToPrevRoom(currentRoomNum);
						printRoomDescription(rooms, currentRoomNum);
					}
					else if (commands[1] ==  rooms[currentRoomNum + 1].name)
					{
						goToNextRoom(rooms, currentRoomNum);
						
					}
					else if (commands[1] == rooms[currentRoomNum - 1].name)
					{
						goToPrevRoom(currentRoomNum);
						printRoomDescription(rooms, currentRoomNum);
						
					}
					else if (commands[1] == rooms[currentRoomNum].nextDoor)
					{
						goToNextRoom(rooms, currentRoomNum);
						
					}
					else if (commands[1] == rooms[currentRoomNum].prevDoor)
					{
						goToPrevRoom(currentRoomNum);
						printRoomDescription(rooms, currentRoomNum);
						
					}
					else if (commands[1] == "north" && rooms[currentRoomNum].prevRoom == 0)
					{
						goToPrevRoom(currentRoomNum);
						printRoomDescription(rooms, currentRoomNum);
					}
					else if (commands[1] == "east" && rooms[currentRoomNum].prevRoom == 1)
					{
						goToPrevRoom(currentRoomNum);
						printRoomDescription(rooms, currentRoomNum);
					}
					else if (commands[1] == "south" && rooms[currentRoomNum].prevRoom == 3)
					{
						goToPrevRoom(currentRoomNum);
						printRoomDescription(rooms, currentRoomNum);
					}
					else if (commands[1] == "west" && rooms[currentRoomNum].prevRoom == 2)
					{
						goToPrevRoom(currentRoomNum);
						printRoomDescription(rooms, currentRoomNum);
					}
					else if (commands[1] == "north" && rooms[currentRoomNum].nextRoom == 0)
					{
						goToNextRoom(rooms, currentRoomNum);
					}
					else if (commands[1] == "east" && rooms[currentRoomNum].nextRoom == 1)
					{
						goToNextRoom(rooms, currentRoomNum);
					}
					else if (commands[1] == "south" && rooms[currentRoomNum].nextRoom == 3)
					{
						goToNextRoom(rooms, currentRoomNum);
					}
					else if (commands[1] == "west" && rooms[currentRoomNum].nextRoom == 2)
					{
						goToNextRoom(rooms, currentRoomNum);
					}
					else
					{
						cout << "It seems like you are trying to go the wrong way" << endl << endl;
					}
				}
				else
				{
					if (commands[1] == "door")
					{
						goToPrevRoom(currentRoomNum);
						printRoomDescription(rooms, currentRoomNum);
						
					}	
					else if (commands[1] == "tunnel" || commands[1] == "staircase")
					{
						goToNextRoom(rooms, currentRoomNum);
						
					}
					else if (commands[1] == "next")
					{
						cout << "The location of the next room does not seem obvious from here.  You may need to take an unconventional route" << endl << endl;
					}
					else if (commands[1] == "prev")
					{
						goToPrevRoom(currentRoomNum);
						printRoomDescription(rooms, currentRoomNum);
					}
					else if (commands[1] == rooms[currentRoomNum + 1].name)
					{
						goToNextRoom(rooms, currentRoomNum);
						
					}
					else if (commands[1] == rooms[currentRoomNum - 1].name)
					{
						goToPrevRoom(currentRoomNum);
						printRoomDescription(rooms, currentRoomNum);
						
					}
					else if (commands[1] == rooms[currentRoomNum].nextDoor)
					{
						goToNextRoom(rooms, currentRoomNum);
						
					}
					else if (commands[1] == rooms[currentRoomNum].prevDoor)
					{
						goToPrevRoom(currentRoomNum);
						printRoomDescription(rooms, currentRoomNum);
						
					}
					else if (commands[1] == "north" && rooms[currentRoomNum].prevRoom == 0)
					{
						goToPrevRoom(currentRoomNum);
						printRoomDescription(rooms, currentRoomNum);
					}
					else if (commands[1] == "east" && rooms[currentRoomNum].prevRoom == 1)
					{
						goToPrevRoom(currentRoomNum);
						printRoomDescription(rooms, currentRoomNum);
					}
					else if (commands[1] == "south" && rooms[currentRoomNum].prevRoom == 3)
					{
						goToPrevRoom(currentRoomNum);
						printRoomDescription(rooms, currentRoomNum);
					}
					else if (commands[1] == "west" && rooms[currentRoomNum].prevRoom == 2)
					{
						goToPrevRoom(currentRoomNum);
						printRoomDescription(rooms, currentRoomNum);
					}
					else if (commands[1] == "north" && rooms[currentRoomNum].nextRoom == 0)
					{
						goToNextRoom(rooms, currentRoomNum);
					}
					else if (commands[1] == "east" && rooms[currentRoomNum].nextRoom == 1)
					{
						goToNextRoom(rooms, currentRoomNum);
					}
					else if (commands[1] == "south" && rooms[currentRoomNum].nextRoom == 3)
					{
						goToNextRoom(rooms, currentRoomNum);
					}
					else if (commands[1] == "west" && rooms[currentRoomNum].nextRoom == 2)
					{
						goToNextRoom(rooms, currentRoomNum);
					}
					else
					{
						cout << "It seems like you are trying to go the wrong way" << endl << endl;
					}
				}
			}

		}
		else if (commands[0] == "take")
		{
			takeItem(commands[1], playerInventory, rooms, currentRoomNum);

		}
		else if (commands[0] == "use")
		{
			useItem(commands[1], playerInventory, rooms, currentRoomNum);

		}
		else if (commands[0] == "help")
		{
			cout << "help commands" << endl;

		}
		else if (commands[0] == "savegame")
		{
			save_game(currentRoomNum, playerInventory, rooms);

		}
		else if (commands[0] == "loadgame")
		{
			//load_game(currentRoomNum, playerInventory, rooms);

		}
		else if (commands[0] == "inventory")
		{
			string inventoryContents = "Inventory contents: ";
			
			for (int i = 0; i < playerInventory.numItems; i++)
			{
				if (i == 0)
				{
					inventoryContents += playerInventory.items[i].name;
				}
				else
				{
					inventoryContents += ", ";
					inventoryContents += playerInventory.items[i].name;
				}
				
				cout << inventoryContents << endl << endl;
				
			}

		}
		else if (commands[0] == "shoot king" && commands[1] == "bow" && currentRoomNum == 14)
		{
			useItem("bow and arrow", playerInventory, rooms, currentRoomNum);
		}
		else if (commands[0] == "shoot guard" && commands[1] == "bow" || commands[1] == "bow and arrow" && currentRoomNum != 14)
		{
			useItem("bow and arrow", playerInventory, rooms, currentRoomNum);
		}
		else if (commands[0] == "read" && commands[1] == "castle map" || commands[1] == "guard duty roster")
		{
			useItem(commands[1], playerInventory, rooms, currentRoomNum);
		}
		else if (commands[0] == "jump" && commands[1] == "window" && currentRoomNum == 7)
		{
			goToNextRoom(rooms, currentRoomNum);
		}
		else if (commands[0] == "climb" && currentRoomNum == 3 && (commands[1] == "window" || commands[1] == "rope") )
		{
			
			if (currentRoomNum == 4)
			{
				goToPrevRoom(currentRoomNum);
				printRoomDescription(rooms, currentRoomNum);
				
			}
			else if (currentRoomNum == 3)
			{
				goToNextRoom(rooms, currentRoomNum);
			}
			
			
		}
		else if (commands[0] == "drink" && commands[1] == "mead")
		{
			useItem(commands[1], playerInventory, rooms, currentRoomNum);
		}
		else if (commands[0] == "tie" && commands[1] == "rope")
		{
			useItem(commands[1], playerInventory, rooms, currentRoomNum);
		}
		else if (commands[0] == "swing" && commands[1] == "sword")
		{
			useItem(commands[1], playerInventory, rooms, currentRoomNum);
		}
		else if (commands[0] == "stab guard" && commands[1] == "sword")
		{
			useItem(commands[1], playerInventory, rooms, currentRoomNum);
		}
		else if (commands[0] == "swing sword" && commands[1] == "guard")
		{
			useItem("sword", playerInventory, rooms, currentRoomNum);
		}
		else if (commands[0] == "shine" && commands[1] == "lamp")
		{
			//useItem(commands[1], playerInventory, rooms, currentRoomNum);
			
			save_game(currentRoomNum, playerInventory, rooms);
			
		}
		else
		{
			
			
			
			
			
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
		
		for (int k = 0; k < NUMITEMS; k++)
		{
		
			string itemFileName = "./itemdata/item";
			itemFileName += to_string(k);
			itemFileName += ".txt";
			ifstream inFile;
			inFile.open(itemFileName);
			int lineCounter = 1;
			stringstream ss;
			
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
					fileLine.erase(fileLine.length() - 1);
					newItem.itemNum = stoi(fileLine);
				}
				else if (lineCounter == 2)
				{
					newItem.name = fileLine;
					newItem.name.erase(newItem.name.length() - 1);
				}
				else if (lineCounter == 3)
				{
					newItem.description = fileLine;
					newItem.description.erase(newItem.description.length() - 1);
				}
				else if (lineCounter == 4)
				{
					fileLine.erase(fileLine.length() - 1);
					newItem.usedInRoom = stoi(fileLine);
				}
				else if (lineCounter == 5)
				{
					fileLine.erase(fileLine.length() - 1);
					newItem.canTake = stoi(fileLine);
				}
				else if (lineCounter == 6)
				{
					fileLine.erase(fileLine.length() - 1);
					newItem.roomNum = stoi(fileLine);
				}
				else if (lineCounter == 7)
				{
					fileLine.erase(fileLine.length() - 1);
					newItem.useText1 = fileLine;
				}
				else if (lineCounter == 8)
				{
					fileLine.erase(fileLine.length() - 1);
					newItem.useText2 = fileLine;
				}
				else if (lineCounter == 9)
				{
					fileLine.erase(fileLine.length() - 1);
					newItem.roomdescription = fileLine;
				}
				
				lineCounter++;
				
			}

			inFile.close();
			
			rooms[newItem.roomNum].items[rooms[newItem.roomNum].numItems] = newItem;
			rooms[newItem.roomNum].numItems++;
		
		}

	
}



/*
formatText()
Args: string to format, width of console window
Returns: formatted string 
Function: formats string with linebreaks so that new lines are not inserted mid-word

Text formatter adapted from https://stackoverflow.com/questions/6891652/formatting-a-string-into-multiple-lines-of-a-specific-length-in-c-c

*/

string formatText(string source, size_t width)
{
	
	string whitespace = " \t\r";
    size_t  currIndex = width - 1;
    size_t  sizeToElim;
    while ( currIndex < source.length() )
    {
        currIndex = source.find_last_of(whitespace,currIndex + 1); 
        if (currIndex == std::string::npos)
            break;
        currIndex = source.find_last_not_of(whitespace,currIndex);
        if (currIndex == std::string::npos)
            break;
        sizeToElim = source.find_first_not_of(whitespace,currIndex + 1) - currIndex - 1;
        source.replace( currIndex + 1, sizeToElim , "\n");
        currIndex += (width + 1); //due to the recently inserted "\n"
    }
    return source;
}





