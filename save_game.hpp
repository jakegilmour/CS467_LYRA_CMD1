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

void save_data(std::string [], int);
bool file_present(std::string);
int save_menu();
void write_to_file(std::string [], int, std::string, std::string);

#endif /* load_hpp */
