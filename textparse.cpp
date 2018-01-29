//
//  textparse.cpp
//  capstone_text_adventure
//
//  Created by Kevin Conner on 1/21/18.
//  Copyright © 2018 Kevin Conner. All rights reserved.
//

#include <iostream>
#include <string>
#include <vector>
#include "textparse.h"

/*
 Textparse helper function finds a matching string in vector that is also in the array
 Args - vector of words, array of strings to compare to, size of the array.
 Returns - string of first match found. "no match found" when no match found.
 */
std::string getMatch(std::vector<std::string> words, std::string array[], int size)
{
    int n = 0;
    int i, k;
    std::string catword;
    
    // compare each word in words for a match in array
    for (i = 0; i < words.size(); i++) {
        for (k = 0; k < size; k++) {
            if (words.at(i).compare(array[k]) == 0) {
                return array[k];
            }
        }
    }
    
    // if no single word matches were found then try possible words
    // with a space such as dungeon key
    if (n == 0) {
        for (i = 0; i < words.size() - 1; i++) {
            for (k = 0; k < size; k++) {
                // combine word ie. "dungeon" + " " + "key" = "dungeon key"
                catword = words.at(i) + " " + words.at(i+1);
                if (catword.compare(array[k]) == 0) {
                    return catword;
                }
            }
        }
    }
    
    return "no match found";
}

/*
    Textparse helper function finds the number of occurances of a vector of strings in a array.
    Args - vector of words, array of strings to compare to, size of the array.
    Returns - number of occurances of a word found in the array.
 */
int occurrences(std::vector<std::string> words, std::string array[], int size)
{
    int n = 0;
    int i, k;
    std::string catword;
    
    // compare each word in words for a match in array
    for (i = 0; i < words.size(); i++) {
        for (k = 0; k < size; k++) {
            if (words.at(i).compare(array[k]) == 0) {
                n++;
            }
        }
    }
    
    // if no single word matches were found then try possible words
    // with a space such as dungeon key
    if (n == 0) {
        for (i = 0; i < words.size() - 1; i++) {
            for (k = 0; k < size; k++) {
                // combine word ie. "dungeon" + " " + "key" = "dungeon key"
                catword = words.at(i) + " " + words.at(i+1);
                if (catword.compare(array[k]) == 0) {
                        n++;
                }
            }
        }
    }
    
    return n;
}

/*
    Parses a string to find a verb and a item that is used by the verb.
    Args - a std::string
    Returns - std::vector {verb, item}. Verb and item are lower case strings. Verbs go and move are
    associated with locations and returns {verb, location}. Some verbs are not associated with items or
    locations and just returns {verb}.
 */
std::vector<std::string> textParse(std::string astring)
{
    int i;
    std::vector<std::string> result;
    std::vector<std::string> words;
    std::string temp;
    const int verbsSize = 13;
    const int itemsSize = 14;
    const int placesSize = 19;
    
    std::string validVerbs[verbsSize] = {   "inspect", "open", "shoot", "jump",
                                            "tie", "climb", "drink", "read", "items"
                                            "move", "go", "move", "take", "help" };
    
    std::string validItems[itemsSize] = {   "dungeon key", "castle map", "lamp",
                                            "rope", "food", "window", "sword",
                                            "bow", "arrow", "treasure sack",
                                            "mead", "door", "guard", "flour" };
    
    std::string validPlaces[placesSize] = { "north", "east", "west", "south",
                                            "dungeon", "treasure", "basement",
                                            "throne", "great hall", "chamber",
                                            "solar", "lavatory", "kitchen",
                                            "pantry", "guard", "chapel", "cellar",
                                            "arms", "dovecote" };
    
    // add null bit if it doesn't have one
    if (astring.length() - 1 != '\0') {
        astring += '\0';
    }
    
    // check empty command
    if (astring.length() == 0) {
        printf("Nothing was entered.\n");
        return result;
    }
    // check command length
    else if (astring.length() > 50) {
        printf("Your command is too long.\n");
        return result;
    }
    
    for (i = 0; i < astring.length(); i++) {
        // Check for spaces, \n, and \0 for when a word completes
        if (astring[i] == ' ' || astring[i] == '\n' || astring[i] == '\0') {
            if (temp.length() > 0) {
                // a word has been formed to save
                words.push_back(temp);
                temp = "";
            }
        }
        // add lower case letters to temp until a word is formed
        else if (std::isalpha(astring[i])) {
            temp += std::tolower(astring[i]);
        }
    }
    
    // check for number of occurances in valid verbs
    int nVerbMatches = occurrences(words, validVerbs, verbsSize);
    int nItemMatches = occurrences(words, validItems, itemsSize);
    int nPlaceMatches = occurrences(words, validPlaces, placesSize);
    
    // Find out if it's {verb, item}, {verb, place}, {verb}, or no match (empty result vector)
    if (nVerbMatches == 0) {
        printf("No valid verbs were used.\n");
        return result;
    }
    else if (nVerbMatches > 1) {
        printf("Please use only one valid verb.\n");
        return result;
    }
    else if (nVerbMatches == 1) {
        // get the verb used
        std::string verbMatch = getMatch(words, validVerbs, verbsSize);
        
        // check places and items now.
        if (nPlaceMatches == 0 && nItemMatches == 0) {
            if (verbMatch == "help" || verbMatch == "items") {
                // only valid verb. return {verb}
                result.push_back(verbMatch);
                return result;
            }
            else {
                // if not help or items then a place or item
                printf("The command, %s, requires an item or place to be specified.\n", verbMatch.c_str());
            }
        }
        else if (nPlaceMatches >= 1 && nItemMatches >= 1) {
            printf("An item(s) and place(s) were both selected.\n");
            return result;
        }
        // case {verb, place}
        else if (nPlaceMatches == 1 && nItemMatches == 0) {
            result.push_back(verbMatch);
            result.push_back(getMatch(words, validPlaces, placesSize));
            return result;
        }
        // case {verb, item}
        else if (nPlaceMatches == 0 && nItemMatches == 1) {
            result.push_back(verbMatch);
            result.push_back(getMatch(words, validItems, itemsSize));
            return result;
        }
    }
    return result;
}

//int main(int argc, const char * argv[]) {
//
//    std::string text = "Inspect ze castle map";
//
//    std::vector<std::string> result = textParse(text);
//    std::cout << result.at(0) << " " << result.at(1) << std::endl;
//
//
//    return 0;
//}

