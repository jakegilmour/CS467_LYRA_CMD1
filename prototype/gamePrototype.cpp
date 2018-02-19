#include <iostream>
using namespace std;

void endGame(){
	return;
}

void roomSeven(){
string x;
	x="";
	cout << "What would you like to do?\n" << endl;
	cin >> x;
if (x == "help")
{
	cout << "Possible inputs: Inspect, go, inventory, shoot, grab.\n" << endl;
roomSeven();
}

if (x=="inspect")
{
	string y;
	cout << "You see a weak spot in his armor. Shoot the king at the weak point?" << endl;
	cin >> y;

if (y== "yes")
{
	cout << "You have slain the King and freed the princess. You win! Game over." << endl;
	void endGame();
}

if (y=="no")
{
	roomSeven();
}

if (y!= "no" && y!="yes")
{
	cout << "Invalid input" << endl;
	roomSeven();
}
}

if (x=="go")
{
	cout << "There's no where to go to." << endl;
	roomSeven();
}

if (x== "inventory")
{
	cout << "Weapons" << endl;
	roomSeven();
}

if (x== "shoot")
{
	cout << "You try to shoot him... his armor deflects it. He engulfs you in a giant fireball spell.\n Loading from start of room." <<endl;
roomSeven();
}

if (x=="grab")
{
	cout << "There's nothing to grab." << endl;
	roomSeven();
}


if (x!="shoot" && x!= "Moria" && x!="help" && x != "inspect" && x!="go" && x!="inventory" && x!="grab")
{
cout << "Unexpected response. Type inspect to see inputs." << endl;
roomSeven();
}


}






void roomSix(){
string x;
	x="";
	cout << "What would you like to do?\n" << endl;
	cin >> x;

if (x == "help")
{
	cout << "Possible inputs: Inspect, Moria, go, inventory, shoot, grab.\n" << endl;
roomSix();
}

if (x == "inspect")
{
	cout << "You find yourself in the most maginicient Great Hall you've ever seen.\n There are countless things that catch your eye, but one thing above all else.\n You see a giant wooden door that is barred off.\n It seems like it's magically locked... as if it's waiting for a password.\n" <<endl;
roomSix();
}

if (x=="go")
{
	cout <<"The door is barred off. You faintly remember hearing about what to do to get into the throne room." << endl;
	roomSix();
}

if (x=="Moria")
{
	cout << "The magic is lifted from the door. You walk with a sense of purpose into it.\n You have reached the Throne Room.\n As you enter, far down on a magnicient throne, you see the king in all gold armor. He says: you can't defeat me.\n" << endl;
	roomSeven();
}

if (x=="inventory")
{
	cout << "You have weapons and a note saying to say Moria." << endl;
	roomSix();
}

if (x=="shoot")
{
	cout << "You shoot your bow at nothing." << endl;
	roomSix();
}

if (x!="shoot" && x!= "Moria" && x!="help" && x != "inspect" && x!="go" && x!="inventory" && x!="grab")
{
cout << "Unexpected response. Type inspect to see inputs." << endl;
roomSix();
}


}




void roomFive(){
string x;
	x="";
	cout << "What would you like to do?\n" << endl;
	cin >> x;

if (x == "help")
{
	cout << "Possible inputs: Inspect, go, inventory, grab.\n" << endl;
roomFive();
}

if (x== "inspect")
{
cout << "You see a nice shiny BOW AND ARROW and KNIFE." << endl;
roomFive();
}
if (x== "grab")
{
	cout << "You grab the BOW AND ARROW and KNIFE \n(They will be added next room to your inventory)." <<endl;
roomFive();
}

if (x=="go")
{
	cout << "ABRIDGED FOR MIDPOINT CHECK. (you grab a bow) Moving forward to Great Hall Room."<< endl;
	roomSix();
}

if (x=="inventory")
{
	cout << "You have a note saying to say Moria at the Throne Room." << endl;
roomFive();
}
if (x!="help" && x != "inspect" && x!="go" && x!="inventory" && x!="grab")
{
cout << "Unexpected response. Type inspect to see inputs." << endl;
roomFive();
}

}


void roomFour(){
string x;
	x="";
	cout << "What would you like to do?\n" << endl;
	cin >> x;

if (x == "help")
{
	cout << "Possible inputs: Inspect, go, inventory, grab.\n" << endl;
roomFour();
}

if (x== "inspect")
{
	cout << "You find yourself in a room full of pigeons and birds and the like.\n You see a note attached to a pigeons foot.\n You carefully remove the note from the pigeon and read it.\n It says: When in the throne room, to get to the majesty, say his name three times." << endl;
roomFour();
}

if (x== "grab")
{
	cout << "There is nothing to grab." << endl;
	roomFour();
}

if (x=="inventory")
{
	cout << "You have nothing." << endl;
	roomFour();
}

if (x=="go")
{
	cout << "You move on to the next room. The Place of Arms." << endl;
	roomFive();
}

if (x!="help" && x != "inspect" && x!="go" && x!="inventory" && x!="grab")
{
cout << "Unexpected response. Type inspect to see inputs." << endl;
roomFour();
}


}


void roomThree(){
	string x;
	x="";
	cout << "What would you like to do?\n" << endl;
	cin >> x;

if (x == "help")
{
	cout << "Possible inputs: Inspect, go, inventory, grab.\n" << endl;
roomThree ();
}

	if (x== "go")
	{
		cout << "You loudly walk right out in front of the gaurds. Twenty guards swarm you.\n Loading from the start of the room." << endl;
		roomThree();
	}

	if (x == "inspect")
	{
	string y;
		cout << "Besides the gaurds, you see a rather hidden pathway along the book cases.\n The gaurds eating dinner would not have a direct eyeline.\n Would you like to go?" << endl;
	cin >> y;
	if (y == "yes"){
		cout << "You quietly sneak by the guards into a dovecote." << endl;
		roomFour();
	}
if (y== "no")
{
	roomThree();
}
if (y!= "yes" && y!="no")
{
	cout << "Unexpected response. Type inspect to see inputs." << endl;
roomThree();
}
	}

if (x== "inventory")
{
	cout << "You have nothing." << endl;
	roomThree();
}

if (x=="grab")
{
	cout << "There is nothing to grab." <<endl;
	roomThree();
}

	
if (x!="help" && x != "inspect" && x!="go" && x!="inventory" && x!="grab")
{
cout << "Unexpected response. Type inspect to see inputs." << endl;
roomThree();
}



}



void roomTwo (){
	string x;
	x="";
	cout << "What would you like to do?\n" << endl;
	cin >> x;
if (x == "inventory")
{
	cout << "You have nothing" << endl;
	roomTwo();
}

if (x == "help")
{
	cout << "Possible inputs: Inspect, go, inventory, grab.\n" << endl;
roomTwo ();
}

if(x== "inspect"){
	cout << "You find yourself in a basement mostly used for wine storage.\n There is a door cracked open across the room.\nThere is light coming out of it and you can hear chatter.\nYou don't find anything too interesting beyond that.\n" << endl;
roomTwo();
}

if(x=="drink")
{
	cout << "You start reaching for a bottle of wine, but then remember there are more pressing issues at hand.\n" << endl;
roomTwo();
}

if (x=="go")
{
	cout << "You enter the guard room.\n You see the shadows of numerous gaurds, and can hear their joyous meal.\n Careful to not alarm them." << endl;
roomThree();
}
if (x!="help" && x != "inspect" && x!="go" && x!="inventory" && x!="grab" && x!="drink")
{
cout << "Unexpected response. Type inspect to see inputs." << endl;
roomTwo();
}

}




void roomOne (){
	string x;
	x="";
	cout << "What would you like to do?\n";
	cin >> x;
if (x == "help")
{
	cout << "Possible inputs: Inspect, go, inventory, grab.\n" << endl;
roomOne ();
}
if (x == "inventory")
{
	cout << "You have nothing" << endl;
	roomOne();
}

if (x == "inspect")
{
	cout << "You are locked in a cell.\n You see a gaurd about 6 feet away from you sleeping on the job.\n He has KEYS hanging out of his pocket.\n There is a broom leaning agaisnt your jail cell.\n" << endl;
roomOne();
}

if (x== "go")
{
	cout << "You're locked in a cell; there is no where to move to. If only you could reach those keys.\n" << endl;
	roomOne();
}

if (x == "grab")
{
	cout << "You grab the keys using the broom and quietly escape the room.\n You leave the broom behind.\n You leave the room and find yourself in a basement.\n" << endl;
	roomTwo();
}


if (x != "inspect" && x!="go" && x!="inventory" && x!="grab" && x!="drink")
{
cout << "Unexpected response. Type inspect to see inputs." << endl;
roomOne();
}

}

void startGame ()
{
	string x;
	x="";
cout << "Would you like to start a game or load a game\n" << endl;
cin >> x;
if (x != "start" && x!= "load")
{
cout << "Invalid response. Please say start or load\n";
startGame();
}
if (x== "start")
{
	cout << "You find yourself locked in a dungeon cell.\n";
	roomOne();
}
if (x=="load")
{
	cout << "No saved game data\n";
	startGame();
}

}

int main (){
	cout << "Welcome to the game!"  << endl;
	startGame();
}


