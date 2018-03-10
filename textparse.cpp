#include <iostream>
#include <string>
#include <vector>
#include "textparse.hpp"

/*
    Displays the contents of a vector
    Args - a vector of strings
*/
void print_vector(std::vector<std::string> avector) {
    int i;
    for (i = 0; i < avector.size(); i++) {
        printf("\"%s\" ", avector[i].c_str());
    }
    printf("\n");
}

/*
 Textparse helper function finds a matching string in vector that is also in the array
 Args - vector of words, array of strings to compare to, size of the array.
 Returns - a vector of strings of the matches
 */
std::vector<std::string> getMatch(std::vector<std::string> words, std::string array[], int size)
{
    int i, k, limit;
    int lastEle = int(words.size() - 1);
    std::string catword, aword, valid_word, next_word, dbl_word, trip_word, quad_word, prev_word;
    std::vector<std::string> true_words; // holds concatenated valid words and valid singles
    
    // compare each word in words for a match in array
    for (i = 0; i < words.size(); i++) {
        for (k = 0; k < size; k++) {
            aword = words.at(i);
            valid_word = array[k];
            if (aword.compare(valid_word) == 0) {
                // special case double concatenated words. check prev word.
                if ( aword.compare("guard") == 0 || aword.compare("window") == 0 || aword.compare("door") == 0 || aword.compare("food") == 0 ) {
                    // if it's not the first element. since last element has no previous element.
                    if (i != 0) {
                        prev_word = words.at(i-1);
                        catword = prev_word + " " + aword;
                        // ignore these strings it'll be found in the next loop below
                        if ( catword.compare("dungeon window") == 0 || catword.compare("cell door") == 0 || catword.compare("double door") == 0 ||catword.compare("gilded door") == 0 || catword.compare("large door") == 0 || catword.compare("main door") == 0 || catword.compare("old door") == 0 || catword.compare("rotten food") == 0 || catword.compare("royal door") == 0 || catword.compare("shoot guard") == 0 || catword.compare("sleeping guard") == 0 || catword.compare("stab guard") == 0 || catword.compare("vaulted door") == 0 || catword.compare("wooden door") == 0 ) {
                            continue;
                        }
                    }
                }
                // special case double concatenated words. check next word.
                if ( (aword.compare("dungeon") == 0 || aword.compare("bow") == 0 || aword.compare("guard") == 0 || aword.compare("look") == 0 || aword.compare("throne") == 0 || aword.compare("treasure") == 0 ) && (i < lastEle) ) {
                    next_word = words[i+1];
                    // check for triple words
                    if (i < lastEle - 1 ) {
                        catword = aword + " " + next_word + " " + words[i+2];
                        if ( catword.compare("bow and arrow") == 0 || catword.compare("guard duty roster") == 0 ) {
                            continue;
                        }
                    }
                    
                    // check for double word
                    catword = aword + " " + next_word;
                    // continue. append these in the next loop that checks for multiple words.
                    if ( catword.compare("dungeon window") == 0 || catword.compare("dungeon key") == 0 || catword.compare("guard room") == 0 || catword.compare("look at") == 0 || catword.compare("throne room") == 0 || catword.compare("treasure chest") == 0 || catword.compare("treasure room") == 0 || catword.compare("treasure sack") == 0 ) {
                        continue;
                    }
                }
                // pushback the single valid word
                true_words.push_back(aword);
            }
        }
        
    }
    
    // look for possible words with spaces such as "dungeon key"
    for (i = 0; i < words.size(); i++) {
        for (k = 0; k < size; k++) {
            // check for double, triple, and quad words
            limit = int(words.size() - 1);
            if (i < limit && limit >= 0) {
                dbl_word = words.at(i) + " " + words.at(i+1);
                if (dbl_word.compare(array[k]) == 0) {
                    true_words.push_back(dbl_word);
                }
            }
            limit = int(words.size() - 2);
            if (i < limit && limit >= 0) {
                trip_word = words.at(i) + " " + words.at(i+1) + " " + words.at(i+2);
                if (trip_word.compare(array[k]) == 0) {
                    true_words.push_back(trip_word);
                }
            }
            limit = int(words.size() - 3);
            if (i < limit && limit >= 0) {
                quad_word = words.at(i) + " " + words.at(i+1) + " " + words.at(i+2) + " " + words.at(i+3);
                if (quad_word.compare(array[k]) == 0) {
                    true_words.push_back(quad_word);
                }
            }
        }
    }
    
    return true_words;
}


/*
    Parses a string to find a verb and a item that is used by the verb.
    Args - a std::string
    Returns - std::vector {verb, item}. Verb and item are lower case strings. Verbs "go" and "move" are
    associated with locations and returns {verb, location}. Some verbs are not associated with items or
    locations and just returns {verb} such as "items", "help", and "hint". "look" can be used with a item, location, or stand-alone
    When only a location is selected {"go", location} is returned.
 */
std::vector<std::string> textParse(std::string astring)
{
    int i;
    std::vector<std::string> result;
    std::vector<std::string> words;
    std::string temp;
    const int verbsSize = 21;
    const int itemsSize = 45;
    const int placesSize = 30;

    std::string validVerbs[verbsSize] = {   "climb", "drink", "go", "help", "hint",
                                            "inspect", "items", "jump", "look", "look at",
                                            "move", "open", "read", "shine", "shoot guard",
                                            "shoot king", "stab guard", "swing", "take", "tie",
                                            "use" };

    std::string validItems[itemsSize] = {   "barrel", "bed", "bow", "bow and arrow", "castle map",
                                            "chair", "cook book", "crack in the wall", "door", "dragon tales encyclopedia",
                                            "dungeon key", "dungeon window", "flour", "food", "fruit basket",
                                            "gold cup", "guard", "guard duty roster", "holy book", "king",
                                            "lamp", "main door", "mead", "parcel", "pigeons",
                                            "priest robes", "rat", "rope", "rotten food", "shield",
                                            "sleeping guard", "spear", "spider", "strange noise",
                                            "suit of armor", "sword", "table", "throne", "toilet",
                                            "treasure chest", "treasure sack", "trophy", "ways of knighthood book", "window" };

    std::string validPlaces[placesSize] = { "arms", "basement", "cell door", "cellar", "chamber",
                                            "chapel", "double door", "dovecote", "dungeon", "east",
                                            "gilded door", "great hall", "guard room", "kitchen", "large door",
                                            "lavatory", "north", "old door", "pantry", "royal door",
                                            "solar", "south", "staircase", "throne room", "treasure room",
                                            "tunnel", "vaulted door", "west", "window", "wooden door" };

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
    else if (astring.length() > 250) {
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
    
    // get matches
    std::vector<std::string> verbMatches = getMatch(words, validVerbs, verbsSize);
    std::vector<std::string> itemMatches = getMatch(words, validItems, itemsSize);
    std::vector<std::string> placeMatches = getMatch(words, validPlaces, placesSize);

    // check for number of occurances
    long nVerbMatches = verbMatches.size();
    long nItemMatches = itemMatches.size();
    long nPlaceMatches = placeMatches.size();

    // Find out if it's {verb, item}, {verb, place}, {verb}, {place} or no match (empty result vector)
    if (nVerbMatches > 1) {
        printf("Please use only one valid verb.\n");
        printf("The following valid verbs were found...\n");
        print_vector(verbMatches);
        return result;
    }
    else if (nItemMatches > 1) {
        printf("Please use only one valid item.\n");
        printf("The following valid items were found...\n");
        print_vector(itemMatches);
        return result;
    }
    else if (nPlaceMatches > 1) {
        printf("Please use only one valid location.\n");
        printf("The following valid locations were found...\n");
        print_vector(placeMatches);
        return result;
    }
    // can't have item and a place
    else if (nItemMatches == 1 && nPlaceMatches == 1) {
        printf("A valid place, \"%s\", and a valid item, \"%s\", cannot be used together. Please use {verb, item} or {verb, place}.\n", placeMatches[0].c_str(), itemMatches[0].c_str());
        return result;
    }
    // {place}
    else if (nPlaceMatches == 1 && nItemMatches == 0 && nVerbMatches == 0) {
        result.push_back("go");
        result.push_back(placeMatches[0]);
        return result;
    }
    // {verb}
    else if (nPlaceMatches == 0 && nItemMatches == 0 && nVerbMatches == 1) {
        if (verbMatches[0].compare("items") == 0 || verbMatches[0].compare("help") == 0 || verbMatches[0].compare("hint") == 0 || verbMatches[0].compare("look") == 0) {
            result.push_back(verbMatches[0]);
        }
        else {
            printf("No valid items or verbs found. \nA verb must be used with a place or a item except for \"help\" and \"items\".\n");
            printf("\"%s\" was used as a stand-alone verb instead.\n", verbMatches[0].c_str());
        }
        return result;
    }
    // {verb, item}
    else if (nVerbMatches == 1 && nItemMatches == 1) {
        result.push_back(verbMatches[0]);
        result.push_back(itemMatches[0]);
        return result;
    }
    // {verb, place}
    else if (nVerbMatches == 1 && nPlaceMatches == 1) {
        result.push_back(verbMatches[0]);
        result.push_back(placeMatches[0]);
        return result;
    }
    // no valid matches. return empty vector.
    printf("No valid matches found.\n");
    return result;
}

//int main(int argc, const char * argv[]) {
//    int i;
//    std::vector<std::string> result;
//
//    std::string validVerbs[16] = {   "inspect", "open", "shoot", "jump",
//                                     "tie", "climb", "drink", "read", "items",
//                                     "move", "go", "move", "take", "help",
//                                     "look", "look at" };
//
//    std::vector<std::string> words;
//    words.push_back("look");
//    words.push_back("look");
//    words.push_back("at");
//    words.push_back("the");
//    words.push_back("lamp");
//
////    printf("getMatch results...\n");
////
////    result = getMatch(words, validVerbs, 16);
////
////    for (i = 0; i < result.size(); i++) {
////        printf("%s\n", result[i].c_str());
////    }
//
//
//    std::string text = "hint dungeon key";
//
//    printf("\ntextParse results...\n");
//
//    result = textParse(text);
//
//    for (i = 0; i < result.size(); i++) {
//        printf("%s\n", result[i].c_str());
//    }
//
//    return 0;
//}

