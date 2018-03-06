//
//  load_game.hpp
//  capstone_text_adventure
//
//  Created by Kevin Conner on 2/13/18.
//  Copyright © 2018 Kevin Conner. All rights reserved.
//

#ifndef load_game_hpp
#define load_game_hpp

#include <stdio.h>
#include <vector>
#include <string>
#include "game.h"

int find_index(vector<std::string>, std::string);
int load_menu();
void load_game(int&, struct inventory&, vector <struct room>&);

#endif /* load_game_hpp */
