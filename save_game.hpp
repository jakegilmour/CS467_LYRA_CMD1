//
//  save_game.hpp
//  capstone_text_adventure
//
//  Created by Kevin Conner on 2/3/18.
//  Copyright © 2018 Kevin Conner. All rights reserved.
//

#ifndef load_hpp
#define load_hpp

#include <stdio.h>
#include <string>
#include "game.h"

void save_game(int, struct inventory, vector <struct room>);
bool file_present(std::string);
int save_menu();
void write_to_file(int, struct inventory, vector <struct room>);

#endif /* load_hpp */
