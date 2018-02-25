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
     Displays a menu of saved files and opening if there are none.
     User selects int 1-3 of the file they want to load.
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
    Opens a file selected by the user and loads the data by
    assigning state variables
 */
std::vector<std::string> load_data() {
    std::ifstream afile;
    std::vector<std::string> mydata;
    std::string state_var = "";
    char c;
    int filen = load_menu();
    std::string fname = "saved_game_" + std::to_string(filen) + ".txt";
    
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
    
    return mydata;
}

//int main(int argc, const char * argv[]) {
//    int i;
//    std::vector<std::string> data = load_data();
//    
//    for(i = 0; i < data.size(); ++i)
//        std::cout << data[i] << '\n';
//
//    return 0;
//}

