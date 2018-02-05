#include <iostream>
#include <string>
#include <vector>

#define NUMROOMS 16



using namespace std;


int main()
{
	struct inventory playerInventory = initializeInventory();
	vector <struct room> visitedRooms;
	int roomsVisited = 0;
	
	
	while (roomVisited < NUMROOMS)
	{
		bool nextRoomOpen = 0;
		importRoomData(visitedRooms, roomsVisited);
		
		while (nextRoomOpen == 0)
		{
			
			
			/* Get player interactions here with text parser */
			
			
			
			if ( visitedRooms[roomsVisited].interaction1 == 1 && visitedRooms[roomsVisited].interaction2 == 1 && visitedRooms[roomsVisited].interaction3 == 1 )
			{
				nextRoomOpen = 1;
			}
			
			
			
		}
		
		while ( nextRoomOpen == 1 && /*Player hasn't selected move forward*/ )
		{
			/* Get player interactions with text parser */
			
		}
		
		
		
		/* Going to need to update this logic so that if a player goes backwards they aren't increasing the count of visited rooms */
		
		roomsVisited++
		
		
		
	}


	

	return 0;
}




void importRoomData(vector <struct room> visitedRooms, int numRoomsVisited)
{

		string roomFileName = "room";
		string fileLine = "";
		roomFileName += to_string(numRoomsVisited + 1);
		ifstream inFile;
		inFile.open(roomFileName);
		
		
		/* Check for error loading the room file */
		if (!inFile) {
			cerr << "Unable to open file room" << numRoomsVisited + 1 << ".txt";
			exit(1);   // call system to stop
		}			
		
		
		/* Set the room number in the room structure based on the amount of rooms visited */
		visitedRooms[numRoomsVisited].roomNumber = numRoomsVisited + 1;
		
		/* Load all of the items in the room into the room structure */
		getline (inFile,fileLine);
		visitedRooms[numRoomsVisited].item1.name = fileLine;
		getline (inFile,fileLine);
		visitedRooms[numRoomsVisited].item2.name = fileLine;
		getline (inFile,fileLine);
		visitedRooms[numRoomsVisited].item3.name = fileLine;
		
		/* Load all of the descriptions for the different room states into the room structure */
		getline (inFile,fileLine);
		visitedRooms[numRoomsVisited].description1 = fileLine;
		getline (inFile,fileLine);
		visitedRooms[numRoomsVisited].description1 = fileLine;
		getline (inFile,fileLine);
		visitedRooms[numRoomsVisited].description1 = fileLine;
					
	
	

	
}


struct inventory initializeInventory()
{
	
	struct inventory playerInventory;
	playerInventory.numItems = 0;
	return playerInventory;
	
	
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