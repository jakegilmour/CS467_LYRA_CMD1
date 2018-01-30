#include <string>




struct room
{
	struct * room prevRoom;
	struct * room nextRoom;
	bool unlocked = 0;
	struct * item item1;
	struct * item item2;
	int roomState = 0;
	bool interaction1 = 0;
	bool interaction2 = 0;
	bool interaction3 = 0;
	string description1;
	string description2;
	string description3;
	
};

struct item
{
	string name;
	string description;
	int count;	

};

struct inventory
{
	int numItems;
	
	struct * item item1;
	struct * item item2;
	struct * item item3;
	struct * item item4;
	struct * item item5;
	struct * item item6;
	struct * item item7;
	struct * item item8;
	struct * item item9;
	struct * item item10;
	
};


