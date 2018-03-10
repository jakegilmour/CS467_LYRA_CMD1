//
//  save_game.cpp
//  capstone_text_adventure
//
//  Created by Kevin Conner on 2/3/18.
//  Copyright © 2018 Kevin Conner. All rights reserved.
//

#include "save_game.hpp"
#include <iostream>
#include <vector>
#include <sys/stat.h>
#include <dirent.h>
#include <time.h>
#include <fstream>

/*
    Writes the date and time then each element in the data array all
    separated by a 'n'.
    Args - data of strings to be written to a file, size of the data array,
           name of the file, current date and time to written
*/
void write_to_file(std::string fname, std::string dt, int currentRoomNum, struct inventory playerInventory, vector <struct room> rooms) {
    std::ofstream afile;
    struct room aroom;
    int i, k;
    
    // open and write content to file
    afile.open("./saved_games/" + fname);
    
    // write datetime and current room
    afile << dt;
    afile << currentRoomNum << '\n';
    
    // write player inventory
    afile << "start player inventory\n";
    afile << playerInventory.numItems << '\n';
    
    for (i = 0; i < playerInventory.numItems; i++) {
        afile << "start item" << '\n';
        afile << playerInventory.items[i].itemNum << '\n';
        afile << playerInventory.items[i].name << '\n';
        afile << playerInventory.items[i].description << '\n';
        afile << playerInventory.items[i].usedInRoom << '\n';
        afile << playerInventory.items[i].canTake << '\n';
        afile << playerInventory.items[i].roomNum << '\n';
        afile << playerInventory.items[i].useText1 << '\n';
        afile << playerInventory.items[i].useText2 << '\n';
        afile << "end item" << '\n';
    }
    
    afile << "end player inventory\n";
    
    // write room information
    afile << "start room data\n";
    
    for (i = 0; i < rooms.size(); i++) {
        aroom = rooms[i];
        
        afile << "start room" << '\n';
        afile << aroom.name << '\n';
        
        afile << "start room items\n";
        
        for (k = 0; k < aroom.items.size(); k++) {
            afile << "start item" << '\n';
            afile << aroom.items[k].itemNum << '\n';
            afile << aroom.items[k].name << '\n';
            afile << aroom.items[k].description << '\n';
            afile << aroom.items[k].usedInRoom << '\n';
            afile << aroom.items[k].canTake << '\n';
            afile << aroom.items[k].roomNum << '\n';
            afile << aroom.items[k].useText1 << '\n';
            afile << aroom.items[k].useText2 << '\n';
            afile << "end item" << '\n';
        }
        
        afile << "end room items\n";
        
        afile << aroom.numItems << '\n';
        afile << aroom.roomState << '\n';
        afile << aroom.description1 << '\n';
        afile << aroom.description2 << '\n';
        afile << aroom.description3 << '\n';
        afile << aroom.prevRoom << '\n';
        afile << aroom.nextRoom << '\n';
        afile << aroom.door << '\n';
        afile << aroom.nextDoor << '\n';
        afile << aroom.prevDoor << '\n';
        afile << aroom.hint << '\n';
        
        afile << "end room" << '\n';
    }
    
    afile << "end room data\n";
    
    afile.close();
}

/*
    Returns true if file exists, false otherwise
    Arg - file name to search for
*/
bool file_present(std::string fname) {
    DIR *sgdir;
    struct dirent *sdirent;
    
    // open saved_games directory
    sgdir = opendir("./saved_games");
    if (sgdir == NULL) {
        printf("Failed to open ./saved_games directory.");
        return -1;
    }
    
    // loop through files in saved_games directory
    while ((sdirent = readdir(sgdir)) != NULL) {
        
        // mark the file present if it's found
        if (strcmp(sdirent->d_name, fname.c_str()) == 0) {
            closedir(sgdir);
            return true;
        }
    }
    
    // close directory
    closedir(sgdir);
    
    return false;
}

/*
    Displays a menu of saved files and opening if there are none.
    Will allow up to 3 separate files to be saved. User selects
    1-3 to save a file and returns that choice.
*/
int save_menu() {
    std::string user_input;
    char n = '\0';
    int result = 0;
    int dir_status = mkdir("./saved_games", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    
    if (dir_status == 0) {
        // directory was created, print menu
        printf("Please enter the file number (1-3) of where you want to save the game.\n");
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
                printf("Please enter the file number (1-3) of where you want to save the game.\n");
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
        
        // get timestamp when file was last saved.
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
        printf("Please enter the file number (1-3) of where you want to save the game.\n");
        
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
            
            // calculate using ascii to get int
            if (n > 48 && n < 52) {
                result = n - '0';
                return result;
            }
            else {
                printf("Invalid character(s) received.\n");
                printf("Please enter the file number (1-3) of where you want to save the game.\n");
            }
        }
    }
    else {
        perror("Error");
    }
    
    return -1;
}

/*
    Usage: This function should be called when saving the game. Saves game state to a text file.
    Args: - current room number, player inventory, each room
    Struct info in game.h
*/
void save_game(int currentRoomNum, struct inventory playerInventory, vector <struct room> rooms) {
    int filen = save_menu();
    time_t rawtime;
    struct tm *timeinfo;
    std::string fname = "saved_game_" + std::to_string(filen) + ".txt";
    
    // build timestamp string
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    std::string dt = asctime(timeinfo);
    
    // check if file exists
    if (filen >= 1 && filen <= 3) {
        // if it exists, does user want to overwrite
        if (file_present(fname)) {
            std::string overwrite;
            std::locale loc;
            
            printf("Confirm you wish to overwrite this file. Yes or No?\n");
            
            // loop until valid input is received, yes or no
            while (1) {
                std::getline(std::cin, overwrite);
                
                // lower case each letter in user input
                std::transform(overwrite.begin(), overwrite.end(), overwrite.begin(), ::tolower);
                
                if (overwrite.compare("y") == 0 || overwrite.compare("yes") == 0) {
                    write_to_file(fname, dt, currentRoomNum, playerInventory, rooms);
                    printf("Save complete.\n");
                    break;
                }
                else if (overwrite.compare("n") == 0 || overwrite.compare("no") == 0) {
                    break;
                }
                else {
                    printf("Invalid command received.\n");
                    printf("Confirm you wish to overwrite this file. Yes or No?\n");
                }
            }
        }
        // write datetime to file then each data item in string separated with \n
        else {
            write_to_file(fname, dt, currentRoomNum, playerInventory, rooms);
            printf("Save complete.\n");
        }
    }
    else {
        printf("Invalid file number received.\n");
    }
}

//int main(int argc, const char * argv[]) {
//    int currentRoomNum = 2;
//    struct inventory playerInventory;
//    vector<struct room> rooms;
//    item item1, item2;
//    room rm1, rm2;
//
//    // make 2 items to test
//    item1.itemNum = 7;
//    item1.name = "Item 1";
//    item1.description = "This is an Item 1 description.";
//    item1.usedInRoom = 1;
//    item1.canTake = 1;
//    item1.roomNum = 7;
//    item1.useText1 = "go";
//    item1.useText2 = "popcorn";
//
//    item2.itemNum = 6;
//    item2.name = "Item 2";
//    item2.description = "This is an Item 2 description.";
//    item2.usedInRoom = 0;
//    item2.canTake = 0;
//    item2.roomNum = 5;
//    item2.useText1 = "climb";
//    item2.useText2 = "ball game";
//
//    // setup playerInventory
//    playerInventory.numItems = 2;
//    playerInventory.items.push_back(item1);
//    playerInventory.items.push_back(item2);
//
//    // setup rooms
//    rm1.name = "Room 1";
//    rm1.items.push_back(item1);
//    rm1.items.push_back(item2);
//    rm1.numItems = 2;
//    rm1.roomState = 2;
//    rm1.description1 = "This is description 1";
//    rm1.description2 = "This is description 2";
//    rm1.description3 = "This is description 3";
//    rm1.prevRoom = 3;
//    rm1.nextRoom = 4;
//    rm1.door = 12;
//    rm1.nextDoor = "large door";
//    rm1.prevDoor = "hey door";
//    rm1.hint = "helpful hint";
//
//    rm2.name = "Room 2";
//    rm2.items.push_back(item2);
//    rm2.items.push_back(item1);
//    rm2.numItems = 2;
//    rm2.roomState = 1;
//    rm2.description1 = "This is description 1-2";
//    rm2.description2 = "This is description 2-2";
//    rm2.description3 = "This is description 3-2";
//    rm2.prevRoom = 5;
//    rm2.nextRoom = 8;
//    rm2.door = 14;
//    rm2.nextDoor = "ok door";
//    rm2.prevDoor = "later door";
//    rm2.hint = "a hint";
//
//
//    rooms.push_back(rm1);
//    rooms.push_back(rm2);
//
//    // test
//    save_game(currentRoomNum, playerInventory, rooms);
//
//    return 0;
//}

