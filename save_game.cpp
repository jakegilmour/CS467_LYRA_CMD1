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
void write_to_file(std::string data[], int size, std::string fname, std::string dt) {
    std::ofstream afile;
    int i;
    
    // open and write content to file
    afile.open("./saved_games/" + fname);
    afile << dt;
    
    for (i = 0; i < size; i++) {
        afile << data[i] << '\n';
    }
    
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
    Saves an array of data to a text file.
    Args - array of data, size of array
*/
void save_data(std::string data [], int size) {
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
                    write_to_file(data, size, fname, dt);
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
            write_to_file(data, size, fname, dt);
            printf("Save complete.\n");
        }
    }
    else {
        printf("Invalid file number received.\n");
    }
}

//int main(int argc, const char * argv[]) {
//    int size = 3;
//    std::string data[] = { "game2", "5", "dungeon" };
//    save_data(data, size);
//
//    return 0;
//}

