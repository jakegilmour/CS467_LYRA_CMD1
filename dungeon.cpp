#include <iostream>
#include <string>
#include <vector>
#include "game.h"
#include <fstream>

#define NUMROOMS 16



using namespace std;


void DungeonRoom(struct inventory &playerInventory){


	string description1 = "First description";
	string description2 = "second description";
	string descripttion3 = "third description";
	
	struct item key;
	struct item guard;
	struct item window;
	
	int state = 0;
	
	vector <string> textResult;
	textResult.resize(2);
	
	cout << description1;
	
	while (1)
	{
		//textResult = textParse();
		
		
		cin >> textResult[0];
		cin >> textResult[1];
		
		
		if ( textResult[0].compare("take") == 0 && textResult[1].compare("dungeon key") == 0 )
		{
			takeItem(key, playerInventory);
			state = 1;
		}
		
		
		if ( textResult[0].compare("use") == 0 && textResult[1].compare("dungeon key") == 0 )
		{
			if ( useItem(key, playerInventory, 1) == 1)
				state = 2;
			
		}
			
			
		if ( textResult[0].compare("use") == 0 && textResult[1].compare("dungeon key") == 0 && state = 2 )
			break;

		
		if (state == 1)
			cout << description2 << endl;
		else
			cout << description3 << endl;
		
		
		
	}


	
	//BasementRoom();



}