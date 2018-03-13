//
//  load_game.cpp
//  capstone_text_adventure
//
//  Created by Kevin Conner on 2/13/18.
//  Copyright © 2018 Kevin Conner. All rights reserved.
//

#include "load_game.hpp"
#include <string>
#include <dirent.h>
#include <fstream>
#include <iostream>
#include <sys/stat.h>
#include "save_game.hpp"
#include <vector>

/*
     Usage: Find index of matching string in a vector.
     Returns: Index of matching vector. -1 if not found.
*/
int find_index(vector<std::string> avector, std::string astring) {
    int i = 0;
    for (i = 0; i < avector.size(); i++) {
        if (astring.compare(avector[i]) == 0) {
            return i;
        }
    }
    
    return -1;
}

/*
     Usage: Displays a menu of saved files and opening if there are none.
            User selects int 1-3 of the file they want to load.
     Returns: int 1-3 selected by user.
 */
int load_menu() {
    std::string user_input;
    char n = '\0';
    int result = 0;
    int dir_status = mkdir("./saved_games", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    
    if (dir_status == 0) {
        // directory was created, print menu
        printf("Please enter the file number (1-3) of the game you would like to load.\n");
        printf("1. -------------\n");
        printf("2. -------------\n");
        printf("3. -------------\n");
        
        // keep looping until a valid file number is received
        while (1) {
            std::getline(std::cin, user_input);
            
            // check string length
            if (user_input.length() != 1) {
                printf("Too many characters entered. Please enter a single file number 1-3\n");
                continue;
            }
            else {
                n = user_input[0];
            }
            
            // calculate using ascii to get int
            if (n > 48 && n < 52) {
                result = n - '0';
                return result;
            }
            else {
                printf("Invalid character(s) received.\n");
                printf("Please enter the file number (1-3) of the game you would like to load.\n");
            }
        }
    }
    else if (errno == EEXIST) {
        /* directory already exists so check for possible
         saved already created so user can choose to overwrite  */
        FILE *f;
        bool sg1_present = false;
        bool sg2_present = false;
        bool sg3_present = false;
        char sg1_tm[50] = {0};
        char sg2_tm[50] = {0};
        char sg3_tm[50] = {0};
        
        // mark the file present if it's found
        if (file_present("saved_game_1.txt")) {
            sg1_present = true;
        }
        if (file_present("saved_game_2.txt")) {
            sg2_present = true;
        }
        if (file_present("saved_game_3.txt")) {
            sg3_present = true;
        }
        
        // get timestamp when file was last saved which is first line in file
        if (sg1_present) {
            f = fopen("./saved_games/saved_game_1.txt", "r");
            if (!f) {
                perror("Failed to open file");
                return -1;
            }
            else {
                int c = 0;
                int index = 0;
                
                // get each character in file until a new line is encountered
                while ((c = fgetc(f)) != EOF) {
                    if (c == '\n') {
                        break;
                    }
                    sg1_tm[index] = char(c);
                    index++;
                }
                fclose(f);
                sg1_tm[index] = '\0';
            }
        }
        if (sg2_present) {
            f = fopen("./saved_games/saved_game_2.txt", "r");
            if (!f) {
                perror("Failed to open file");
                return -1;
            }
            else {
                int c = 0;
                int index = 0;
                
                // get each character in file until a new line is encountered
                while ((c = fgetc(f)) != EOF) {
                    if (c == '\n') {
                        break;
                    }
                    sg2_tm[index] = char(c);
                    index++;
                }
                fclose(f);
                sg2_tm[index] = '\0';
            }
        }
        if (sg3_present) {
            f = fopen("./saved_games/saved_game_3.txt", "r");
            if (!f) {
                perror("Failed to open file");
                return -1;
            }
            else {
                int c = 0;
                int index = 0;
                
                // get each character in file until a new line is encountered
                while ((c = fgetc(f)) != EOF) {
                    if (c == '\n') {
                        break;
                    }
                    sg3_tm[index] = char(c);
                    index++;
                }
                fclose(f);
                sg3_tm[index] = '\0';
            }
        }
        
        // begin to make menu and show saved files if they exist
        printf("Please enter the file number (1-3) of the game you would like to load.\n");
        
        // write out the file if it exists or blank lines if not
        if (sg1_present) {
            printf("1. %s\n", sg1_tm);
        }
        else {
            printf("1. -------------\n");
        }
        if (sg2_present) {
            printf("2. %s\n", sg2_tm);
        }
        else {
            printf("2. -------------\n");
        }
        if (sg3_present) {
            printf("3. %s\n", sg3_tm);
        }
        else {
            printf("3. -------------\n");
        }
        
        // keep looping until a valid file number is received
        while (1) {
            std::getline(std::cin, user_input);
            
            // check string length
            if (user_input.length() != 1) {
                printf("Too many characters entered. Please enter a single file number 1-3\n");
                continue;
            }
            else {
                n = user_input[0];
            }
            
            // ensure file exists to load
            if (n == '1' and !sg1_present) {
                printf("That file doesn't exist. Please try again.\n");
                continue;
            }
            else if (n == '2' and !sg2_present) {
                printf("That file doesn't exist. Please try again.\n");
                continue;
            }
            else if (n == '3' and !sg3_present) {
                printf("That file doesn't exist. Please try again.\n");
                continue;
            }
            
            // calculate using ascii to get int
            if (n > 48 && n < 52) {
                result = n - '0';
                return result;
            }
            else {
                printf("Invalid character(s) received.\n");
                printf("Please enter the file number (1-3) of the game you would like to load.\n");
            }
        }
    }
    else {
        perror("Error");
    }
    
    return -1;
}

/*
    Usage: Initializes state variables passed to load the current game.
           load_menu() is used within this function to find the file to load.
    Args:  All by reference... current room number, player inventory, vector of rooms
 */
void load_game(int &currentRoomNum, struct inventory &playerInventory, vector <struct room> &rooms) {
    std::ifstream afile;
    std::vector<std::string> mydata;
    std::string state_var = "";
    char c;
    int filen = load_menu();
    std::string fname = "saved_game_" + std::to_string(filen) + ".txt";
    int data_index = 0;
    int struct_item_counter = 0;
    int struct_room_counter = 0;
    item temp_item;
    room temp_room;
    
    // open and write content to file
    afile.open("./saved_games/" + fname);
    
    if ( !afile.is_open() ) {
        printf("Unable to open file.\n");
    }
    else {
        while ( afile.get(c) ) {
            // each line has only one variable ending with a \n
            if (c == '\n') {
                // pushback the completed var w/o \n and reset state_var
                mydata.push_back(state_var);
                state_var = "";
            }
            else {
                // build the state_var string one character at a time
                state_var += c;
            }
        }
        afile.close();
    }
    
    // assign game state variables using mydata
    currentRoomNum = std::stoi(mydata[1]);
    
    // ensure inventory is empty before filling content
    playerInventory.items.clear();
    
    playerInventory.numItems = std::stoi(mydata[3]);
    
    // start player inventory section
    data_index = 5;
    while (mydata[data_index].compare("end player inventory") != 0) {
        
        // item complete. pushback and start on next one.
        if (mydata[data_index].compare("end item") == 0) {
            data_index++;
            struct_item_counter = 0;
            playerInventory.items.push_back(temp_item);
            continue;
        }
        else if (mydata[data_index].compare("start item") == 0) {
            data_index++;
            continue;
        }
        
        if (struct_item_counter == 0) {
            temp_item.itemNum = std::stoi(mydata[data_index]);
            data_index++;
            struct_item_counter++;
        }
        else if (struct_item_counter == 1) {
            temp_item.name = mydata[data_index];
            data_index++;
            struct_item_counter++;
        }
        else if (struct_item_counter == 2) {
            temp_item.description = mydata[data_index];
            data_index++;
            struct_item_counter++;
        }
        else if (struct_item_counter == 3) {
            temp_item.usedInRoom = std::stoi(mydata[data_index]);
            data_index++;
            struct_item_counter++;
        }
        else if (struct_item_counter == 4) {
            temp_item.canTake = std::stoi(mydata[data_index]);
            data_index++;
            struct_item_counter++;
        }
        else if (struct_item_counter == 5) {
            temp_item.roomNum = std::stoi(mydata[data_index]);
            data_index++;
            struct_item_counter++;
        }
        else if (struct_item_counter == 6) {
            temp_item.useText1 = mydata[data_index];
            data_index++;
            struct_item_counter++;
        }
        else if (struct_item_counter == 7) {
            temp_item.useText2 = mydata[data_index];
            data_index++;
            struct_item_counter++;
        }
        else if (struct_item_counter == 8) {
            temp_item.roomdescription = mydata[data_index];
            data_index++;
            struct_item_counter++;
        }
    }
    
    // start room data section
    struct_item_counter = 0;
    data_index = find_index(mydata, "start room data");
    
    while (mydata[data_index].compare("end room data") != 0) {
        
        // room complete. pushback and start on next one.
        if (mydata[data_index].compare("end room") == 0) {
            data_index++;
            struct_room_counter = 0;
            rooms.push_back(temp_room);
            continue;
        }
        else if (mydata[data_index].compare("start item") == 0) {
            data_index++;
            continue;
        }
        else if (mydata[data_index].compare("start room items") == 0) {
            data_index++;
            continue;
        }
        else if (mydata[data_index].compare("start room data") == 0) {
            data_index++;
            continue;
        }
        else if (mydata[data_index].compare("start room") == 0) {
            data_index++;
            continue;
        }
        
        if (struct_room_counter == 0) {
            temp_room.name = mydata[data_index];
            data_index++;
            struct_room_counter++;
        }
        else if (struct_room_counter == 1) {
            temp_room.items.clear();
            while (mydata[data_index].compare("end room items") != 0) {
                
                // item complete. pushback and start on next one.
                if (mydata[data_index].compare("end item") == 0) {
                    data_index++;
                    struct_item_counter = 0;
                    temp_room.items.push_back(temp_item);
                    continue;
                }
                else if (mydata[data_index].compare("start item") == 0) {
                    data_index++;
                    continue;
                }
                
                if (struct_item_counter == 0) {
                    temp_item.itemNum = std::stoi(mydata[data_index]);
                    data_index++;
                    struct_item_counter++;
                }
                else if (struct_item_counter == 1) {
                    temp_item.name = mydata[data_index];
                    data_index++;
                    struct_item_counter++;
                }
                else if (struct_item_counter == 2) {
                    temp_item.description = mydata[data_index];
                    data_index++;
                    struct_item_counter++;
                }
                else if (struct_item_counter == 3) {
                    temp_item.usedInRoom = std::stoi(mydata[data_index]);
                    data_index++;
                    struct_item_counter++;
                }
                else if (struct_item_counter == 4) {
                    temp_item.canTake = std::stoi(mydata[data_index]);
                    data_index++;
                    struct_item_counter++;
                }
                else if (struct_item_counter == 5) {
                    temp_item.roomNum = std::stoi(mydata[data_index]);
                    data_index++;
                    struct_item_counter++;
                }
                else if (struct_item_counter == 6) {
                    temp_item.useText1 = mydata[data_index];
                    data_index++;
                    struct_item_counter++;
                }
                else if (struct_item_counter == 7) {
                    temp_item.useText2 = mydata[data_index];
                    data_index++;
                    struct_item_counter++;
                }
                else if (struct_item_counter == 8) {
                    temp_item.roomdescription = mydata[data_index];
                    data_index++;
                    struct_item_counter++;
                }
            }
            struct_room_counter++;
            data_index++;
        }
        else if (struct_room_counter == 2) {
            temp_room.numItems = std::stoi(mydata[data_index]);
            data_index++;
            struct_room_counter++;
        }
        else if (struct_room_counter == 3) {
            temp_room.roomState = std::stoi(mydata[data_index]);
            data_index++;
            struct_room_counter++;
        }
        else if (struct_room_counter == 4) {
            temp_room.description1 = mydata[data_index];
            data_index++;
            struct_room_counter++;
        }
        else if (struct_room_counter == 5) {
            temp_room.description2 = mydata[data_index];
            data_index++;
            struct_room_counter++;
        }
        else if (struct_room_counter == 6) {
            temp_room.description3 = mydata[data_index];
            data_index++;
            struct_room_counter++;
        }
        else if (struct_room_counter == 7) {
            temp_room.prevRoom = std::stoi(mydata[data_index]);
            data_index++;
            struct_room_counter++;
        }
        else if (struct_room_counter == 8) {
            temp_room.nextRoom = std::stoi(mydata[data_index]);
            data_index++;
            struct_room_counter++;
        }
        else if (struct_room_counter == 9) {
            temp_room.door = std::stoi(mydata[data_index]);
            data_index++;
            struct_room_counter++;
        }
        else if (struct_room_counter == 10) {
            temp_room.nextDoor = mydata[data_index];
            data_index++;
            struct_room_counter++;
        }
        else if (struct_room_counter == 11) {
            temp_room.prevDoor = mydata[data_index];
            data_index++;
            struct_room_counter++;
        }
        else if (struct_room_counter == 12) {
            temp_room.hint = mydata[data_index];
            data_index++;
            struct_room_counter++;
        }
    }
}

//int main(int argc, const char * argv[]) {
//    
//    int currentRoomNum;
//    struct inventory playerInventory;
//    vector <struct room> rooms;
//
//    load_game(currentRoomNum, playerInventory, rooms);
//
//    return 0;
//}

