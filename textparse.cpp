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
    std::string catword, aword, valid_word, next_word, dbl_word, trip_word, quad_word, prev_word;
    std::vector<std::string> true_words; // holds concatenated valid words and valid singles
    
    // compare each word in words for a match in array
    for (i = 0; i < words.size(); i++) {
        for (k = 0; k < size; k++) {
            aword = words.at(i);
            valid_word = array[k];
            if (aword.compare(valid_word) == 0) {
                // special case for "look" and "look at"
                if (aword.compare("look") == 0) {
                    // if it's not the last element. since last element has no next element.
                    if (i != words.size() - 1) {
                        next_word = words.at(i+1);
                        catword = aword + " " + next_word;
                        // ignore "look at" it'll be found in the next loop below
                        if (catword.compare("look at") == 0) {
                            continue;
                        }
                        // add only "look"
                        else { true_words.push_back(aword); }
                    }
                    // add "look" when it's the last element
                    else {
                        true_words.push_back(aword);
                    }
                }
                // special case for "dungeon window" and "window", "sleeping guard" and "guard"
                else if ( aword.compare("guard") == 0 || aword.compare("window") == 0 ) {
                    // if it's not the first element. since last element has no prev element.
                    if (i != 0) {
                        prev_word = words.at(i-1);
                        catword = prev_word + " " + aword;
                        // ignore "dungeon window" or "sleeping guard" it'll be found in the next loop below
                        if (catword.compare("dungeon window") == 0 || catword.compare("sleeping guard") == 0) {
                            continue;
                        }
                        // add only "guard" or "window"
                        else { true_words.push_back(aword); }
                    }
                    // add "guard" or "window" when it's the last element
                    else { true_words.push_back(aword); }
                    
                }
                // add a regular valid word
                else {
                    // don't pushback "dungeon" if it is followed by "window"
                    if ( aword.compare("dungeon") == 0 && i != words.size() - 1 ) {
                        next_word = words.at(i+1);
                        catword = aword + " " + next_word;
                        if (catword.compare("dungeon window") != 0) {
                            true_words.push_back(aword);
                        }
                    }
                    else {
                        true_words.push_back(aword);
                    }
                }
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
    locations and just returns {verb} such as "items" and "help". When only a location is selected
    {"go", location} is returned.
 */
std::vector<std::string> textParse(std::string astring)
{
    int i;
    std::vector<std::string> result;
    std::vector<std::string> words;
    std::string temp;
    const int verbsSize = 16;
    const int itemsSize = 44;
    const int placesSize = 30;

    std::string validVerbs[verbsSize] = {   "inspect", "open", "shoot", "jump",
                                            "tie", "climb", "drink", "read", "items",
                                            "move", "go", "move", "take", "help",
                                            "look", "look at" };

    std::string validItems[itemsSize] = {   "dungeon key", "castle map", "lamp", "rope", "food",
                                            "window", "sword", "treasure sack", "mead", "door",
                                            "guard", "flour", "dungeon window", "dragon tales encyclopedia", "rat",
                                            "guard duty roster", "suit of armor", "pigeons", "parcel", "shield",
                                            "spear", "barrel", "spider", "bed", "trophy",
                                            "toilet", "crack in the wall", "ways of knighthood book", "cook book", "fruit basket",
                                            "main door", "gold cup", "treasure chest", "holy book", "priest robes",
                                            "bed", "rotten food", "table", "chair", "sleeping guard",
                                            "king", "bow and arrow", "strange noise", "throne"
        
    };

    std::string validPlaces[placesSize] = { "north", "east", "west", "south",
                                            "dungeon", "treasure", "basement",
                                            "throne", "great hall", "chamber",
                                            "solar", "lavatory", "kitchen",
                                            "pantry", "guard", "chapel", "cellar",
                                            "arms", "dovecote", "cell door",
                                            "wooden door", "large door", "window",
                                            "staircase", "old door", "tunnel",
                                            "vaulted door", "gilded door", "double door",
                                            "royal door" };

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
    // {verb} must be "items" or "help"
    else if (nPlaceMatches == 0 && nItemMatches == 0 && nVerbMatches == 1) {
        if (verbMatches[0].compare("items") == 0 || verbMatches[0].compare("help") == 0) {
            result.push_back(verbMatches[0]);
        }
        else {
            printf("A verb must be used with a place or a item except for \"help\" and \"items\".\n");
            printf("The verb, %s, was used instead.\n", verbMatches[0].c_str());
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
//    // inspect lamp
//    // dungeon window
//    // window
//    // inspect sleeping guard
//    
//    std::string text = "inspect sleeping guard";
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

