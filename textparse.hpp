//
//  textparse.h
//  capstone_text_adventure
//
//  Created by Kevin Conner on 1/26/18.
//  Copyright © 2018 Kevin Conner. All rights reserved.
//

#ifndef textparse_h
#define textparse_h

std::vector<std::string> textParse(std::string);
int occurrences(std::vector<std::string>, std::string [], int);
std::string getMatch(std::vector<std::string>, std::string [], int);

#endif /* textparse_h */
