/**
 * File: memorygame.cpp
 * Description: Creates a memory matching game using visual representations of a 52-card deck.
 * Date: 24 May 2026
 * TODO:
 *      1. Make the random generation always winnable
 *          (currently, there can be cards with no matches)
 *          (Done!!)
 *      2. Add a title screen and menu asking how many cards
 *          (Done!!)
 *      3. Create a better way of picking cards
 *          (the numbered system does not work very well. An ordered pair system would be nice)
 */

#include <iostream>
#include <string>

//these two for list handling
#include <vector>
#include <map>

//for sqrt and pow
#include <cmath>

//this for input validation
#include <limits>

//these two for time
#include <stdlib.h>
#include <ctime>


class CardDeck {
    private:
        //if you want a funky ass deck of cards, you can just add more suits or ranks in this list
        //dont forget to also make visuals for your card in the printCard function!
        const char suits[4] = {'c', 's', 'h', 'd'};
        const char ranks[13] = {'1', '2', '3', '4', '5', '6', '7', '8', '9', 'J', 'Q', 'K', 'A'};
        std::vector<std::string> deck;
    public:
        //goes through the suits and ranks and builds the deck from them
        int makeDeck() {
            deck = {}; //clears the current deck
            std::string currentCard;
            for (char x : suits) {
                for (char y : ranks) {
                    currentCard = "";
                    currentCard.push_back(x);
                    currentCard.push_back(y);
                    deck.push_back(currentCard);
                }
            }
            return 0;
        }

        //prints every card in the deck
        void printDeck() {
            int i = 1;
            for (std::string& x : deck) {
                std::cout << x << " " << i << std::endl;
                i++;
            }
        }

        /**
         * Function: drawRandomCards
         * Parameters: int, number of cards you want to draw
         * Description: Picks a specified number of random cards from the deck,
         *      deletes them from the deck, and returns them.
         * Outputs: A string vector of cards
         */
        std::vector<std::string> drawRandomCards(int numberOfCards) {
            std::srand(time(nullptr));
            std::vector<std::string> output;
            for (int i = 0; i < numberOfCards; i++) {
                //gets the index of a random card
                int randomCardIndex = (std::rand() % deck.size());
                //gets the actual random card
                std::string randomCard = deck.at(randomCardIndex);
                //erases the card from the deck
                deck.erase(deck.begin() + randomCardIndex);

                //puts the random card in our list of random cards.
                output.push_back(randomCard);
            }
            
            return output;
        }

        int shuffleDeck() {
            std::vector<std::string> deckThatHasBeenShuffled = drawRandomCards(deck.size());
            deck = deckThatHasBeenShuffled;
            return 0;
        }

        std::string findFromRankAndDelete(char rank) {
            std::string foundCard;
            for (std::string x : deck) {
                if (x.at(1) == rank) {
                    foundCard = x;
                    deck.erase(find(deck.begin(), deck.end(), x));
                    return foundCard;
                }
            }
            //if it gets through the whole list without finding one, it just finds a card and returns it.
            //(i do not know why it does not find one sometimes)
            foundCard = deck.at(0);
            deck.erase(deck.begin());
            return foundCard;
        }

        int findDeckSize() {
            return deck.size();
        }
};

struct GameValues {
    std::vector<std::string> allGameCards;
    std::vector<std::string> visibleCards;
    std::vector<std::string> temporarilyVisibleCards;
    std::vector<int> revealedCardIndexes;
    int userInputCardsToPlay;
    int cardsPerRow;
};

/**
 * Function: printCardChunk
 * Parameters: a card, and the row of the card you want it to print
 * Description: This function creates visual representations of a card.
 *      The printCards function draws this to build cards.
 *      You specify which of 5 parts of the card to print.
 */
std::string printCardChunk(std::string cardToPrint,  int rowToPrint) {
    char suit = cardToPrint.at(0);
    char rank = cardToPrint.at(1);
    std::map <char, std::string> suitMap = {
        {'c', "♣"},
        {'s', "♠"},
        {'h', "♥"},
        {'d', "♦"},
        {'n', "^"}
    };
    std::map <char, std::string> mouthMap = {
        {'c', "_"},
        {'s', "."},
        {'h', "w"},
        {'d', "v"},
        {'n', "^"}
    };
    std::map <char, std::string> eyes = {
        {'1', "00"},
        {'2', "**"},
        {'3', "XX"},
        {'4', "--"},
        {'5', "o0"},
        {'6', "@@"},
        {'7', "><"},
        {'8', "UU"},
        {'9', "//"},
        {'J', "0-"},
        {'Q', ";;"},
        {'K', "$$"},
        {'A', "=="},
        {'n', "^^"}
    };

    //Finds visual representations based on the suits matched to the maps above
    std::string mouth = mouthMap.at(suit);
    std::string suitLabel = suitMap.at(suit);
    //finds the pair of eyes at that rank and puts them in separate variables
    char eye1 = eyes.at(rank).at(0);
    char eye2 = eyes.at(rank).at(1);

    //switch statement which returns what it should print
    switch(rowToPrint) {
        //1st row displaying top
        case 1:
            return "╭─────╮";
            break;
        //2nd row displaying rank
        case 2: {
            std::string line = "";
            line.append("│");
            line.push_back(rank);
            line.append("    │");
            return line;
            break;
        }
        //3rd row displaying face
        case 3: {
            std::string line = "";
            line.append("│ ");
            line.push_back(eye1);
            line.append(mouth);
            line.push_back(eye2);
            line.append(" │");
            return line;
            break;
        }
        //4th row displaying suit
        case 4: {
            std::string line = "";
            line.append("│    ");
            line.append(suitLabel);
            line.append("│");
            return line;
            break;
        }
        //5th row displaying bottom
        case 5:
            return "╰─────╯";
            break;
    }
    return "if you're seeing this, I made a mistake"; //shouldn't ever get here, but returns error if it does
}

/**
 * Function: printCards
 * Parameters: A vector containing a list of cards.
 * Description: Draws cards to the screen in rows of 5.
 */
void printCards(std::vector<std::string> cardsToPrint, GameValues& playerdata) {
    const double rootOnePointFive = sqrt(3.0 / 2.0);
    /**
     * The cards per row is defined by this function:
     * y = sqrt(1.5) * x^0.5.
     * 6 -> 3, 24 -> 6, 96 -> 12
     * Takes the ceiling of it so it's not less than 1
     */
    playerdata.cardsPerRow = ceil(rootOnePointFive * pow(cardsToPrint.size(), 0.5));
    //finds the amount of rows of 5 cards. ex. if 15, then 3
    const int normalRows = cardsToPrint.size() / playerdata.cardsPerRow;
    //If the amount of cards is not a multiple of 5, it stores the remainder here
    const int cardsInLeftoverRow = cardsToPrint.size() % playerdata.cardsPerRow;

    //repeats for the amount of rows of 5 cards
    for (int rowIndex = 0; rowIndex < normalRows; rowIndex++) {
        //Makes an index of the 5 cards we're working with
        std::vector<std::string> listOfFiveCards;
        int startIndex = rowIndex * playerdata.cardsPerRow;
        for (int j = startIndex; j < startIndex + playerdata.cardsPerRow; j++) {
            listOfFiveCards.push_back(cardsToPrint.at(j));
        }

        //prints each chunk of each card
        for (int row = 1; row <= 5; row++) {
            for (int card = 0; card < playerdata.cardsPerRow; card++) {
                std::cout << printCardChunk(listOfFiveCards.at(card), row) << " ";
            }
            std::cout << "\n";
        }
        std::cout << '\n';
    }

    //then, it prints the last cards of the remaining uneven row
    int startIndex = normalRows * playerdata.cardsPerRow;
    if (startIndex < static_cast<int>(cardsToPrint.size())) {
        for (int row = 1; row <= 5; row++) {
            for (int card = startIndex; card < static_cast<int>(cardsToPrint.size()); card++) {
                std::cout << printCardChunk(cardsToPrint.at(card), row) << " ";
            }
            std::cout << "\n";
        }
    }
}

/**
 * Function: printInterface
 * Parameters: An object of GameValues
 * Description: Clears the screen, displays a menu, and runs the function to draw the playing field. 
 */
void printInterface(GameValues& playerdata) {
    std::cout << "\x1b[2J\x1b[H" << "Input works on a row and column system.\nInput 'row,column' for the card you want to check." << std::endl;
    printCards(playerdata.temporarilyVisibleCards, playerdata);
    std::cout << std::endl;
}

/**
 * Function: inputAndValidateForBoard
 * Parameters: int minimum value, int maximum value, object of GameValues
 * Description: Takes user input for an integer and checks for validity.
 *      Then, it checks if it's within the specified values.
 *      Then, it checks if that value has already been guessed.
 */
int inputAndValidateForBoard(const int minValue, const int maxValue, GameValues& playerdata) {
    int userInput;
    while (true) {
        //checks for invalid integer
        if (!(std::cin >> userInput)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); //ignores some bullshit so it works
            std::cout << "Input error! Try again: ";
            continue;
        }
        //checks if integer is in valid range
        if ((userInput < minValue) || (userInput > maxValue)) {
            std::cout << "Input not in range! Try again: ";
            continue;
        }

        //we check if this is something the user has already guessed
        bool alreadyGuessed = false;
        for (int& x : playerdata.revealedCardIndexes) {
            //we subtract 1 because we subtract 1 from the input
            if ((userInput - 1) == x) {
                //this is inside a for loop, so we have to bring a variable outside of it
                //to affect the while loop.
                std::cout << "You've already guessed that! Try again: ";
                alreadyGuessed = true;
                break;
            }
        }
        if (alreadyGuessed) {
            continue;
        }

        //if it's none of those, it's valid
        break;
    }
    return userInput;
}

/**
 * Function: orderedPairValidate
 * Parameters: object of playerdata
 * Description: Takes user input and validates it for ordered pair input:
 *      1. Checks if the input is a valid string
 *      2. Checks if there are any characters other than numbers and commas
 *      3. Checks if there are zero or more than one comma
 *      4. Checks if the comma is at the end or beginning of the string
 *          (forcing users to input numbers before and after comma)
 *      5. Separates the string into the integers before and after the comma
 *      6. Converts these row and column values into the associated data index value
 *          (cards per row * (rows - 1)) + columns
 *      7. Checks if this index is between 1 and the size of its associated vector
 *      8. Checks if this index is in the list of already guessed values
 */
int orderedPairValidate(GameValues& playerdata) {
    //Take string input and separate them into int variables. 
    //(1 - row * cards per row) + column is the index
    //If it is outside the range of the size, throw error. 

    //Take string input
    std::string userInput;
    int finalindex;
    while (true) {
        //checks for invalid string
        if (!(std::cin >> userInput)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); //ignores some bullshit so it works
            std::cout << "Input error! Try again: ";
            continue;
        }

        //Checks if there are any characters other than numbers and one comma
        bool invalidCharacter = false;
        int numberOfCommas = 0;
        for (int i = 0; i < userInput.length(); i++) {
            if (userInput.at(i) == ',') {
                numberOfCommas += 1;
            }
            //if it's not a number and not a comma, it's invalid
            if (((userInput.at(i) < '1') || (userInput.at(i) > '9')) && (userInput.at(i) != ',')) {
                invalidCharacter = true;
                break;
            }
        }
        //If there is an invalid character or not 1 comma, it's invalid.
        if (invalidCharacter || (numberOfCommas != 1)) {
            std::cout << "Invalid character or input! Try again: ";
            continue;
        }

        //Checks if the comma is at the end or the beginning
        if ((userInput.at(0) == ',') || (userInput.at(userInput.size() - 1) == ',')) {
            std::cout << "Must input a value for x and y! Try again: ";
            continue;
        }

        //by now, it should be a valid input.
        //Now, search through and return the index.

        //Separate the input into stuff before comma, and stuff after.
        
        //Takes the value before the comma and sets it to the row
        std::string dummy = ""; //to store
        for (char& x : userInput) {
            if (x == ',') {
                break;
            } else {
                dummy.push_back(x);
            }
        }
        const int row = std::stoi(dummy);

        //Takes the value after the comma and sets it to the column
        dummy = "";
        bool foundComma = false;
        for (char& x : userInput) {
            if (foundComma) {
                dummy.push_back(x);
            }
            if (x == ',') {
                foundComma = true;
            }
        }
        const int column = std::stoi(dummy);

        //check to see if these values are in the valid range
        //2 cases: where the max column length is equal to cards per row, and one where it's in the leftover row
        const int numberOfFullRows = playerdata.allGameCards.size() / playerdata.cardsPerRow;
        const int cardsInLeftoverRow = playerdata.allGameCards.size() % playerdata.cardsPerRow;
        int numberOfRows;
        if (cardsInLeftoverRow == 0) {
            numberOfRows = numberOfFullRows;
        } else {
            numberOfRows = numberOfFullRows + 1;
        }
        //Checks if the row is in range
        if ((row < 1) || (row > numberOfRows)) {
            std::cout << "Row not in range! Try again: ";
            continue;
        }

        if ((row == numberOfRows) && (cardsInLeftoverRow != 0)) {
            //the columns must be less than the leftover cards
            if ((column > cardsInLeftoverRow) || (column <= 0)) {
                std::cout << "Column not in range! Try again: ";
                continue;
            }
        } else {
            //The columns must be less than the cards per row
            if ((column > playerdata.cardsPerRow) || (column <= 0)) {
                std::cout << "Column not in range! Try again: ";
                continue;
            }
        }

        //we subtract 1 from the row because we want to add rows * user specified rows - 1 + user specified column
        finalindex = (playerdata.cardsPerRow * (row - 1)) + column;

        //now, we check to see if the index is between the right values.
        //Should already be the case, but we check just in case.
        if ((finalindex < 1) || (finalindex > playerdata.allGameCards.size())) {
            std::cout << "Input not in range! Try again: ";
            continue;
        }

        //now, we check to see if the user already guessed the input.
        bool alreadyGuessed = false;
        for (int& x : playerdata.revealedCardIndexes) {
            //we subtract 1 because we subtract 1 from the input
            if ((finalindex - 1) == x) {
                //this is inside a for loop, so we have to bring a variable outside of it
                //to affect the while loop.
                std::cout << "You've already guessed that! Try again: ";
                alreadyGuessed = true;
                break;
            }
        }
        if (alreadyGuessed) {
            continue;
        }

        break; 
    }

    return finalindex;
}

//input the original values
void gameLoop(GameValues& playerdata) {
    //sets temporary visibility to all visible cards
    playerdata.temporarilyVisibleCards = playerdata.visibleCards;
    printInterface(playerdata);

    //ask for first card to check
    std::cout << "Enter your first card to check: ";
    //gets user input. This is the index
    const int firstCardIndex = (orderedPairValidate(playerdata) - 1);
    //Sets that card to a variable
    const std::string firstCard = playerdata.allGameCards.at(firstCardIndex);
    //Puts that card in temporary visibility
    playerdata.temporarilyVisibleCards.at(firstCardIndex) = firstCard;
    //puts it in your guessed cards list
    playerdata.revealedCardIndexes.push_back(firstCardIndex);

    printInterface(playerdata);

    //ask for second card to check
    std::cout << "Enter your second card to check: ";
    //get user input.
    const int secondCardIndex = (orderedPairValidate(playerdata) - 1);
    //put that card in a variable
    const std::string secondCard = playerdata.allGameCards.at(secondCardIndex);
    //puts that card in temporary visibility
    playerdata.temporarilyVisibleCards.at(secondCardIndex) = secondCard;
    //saves that to your guessed cards list
    playerdata.revealedCardIndexes.push_back(secondCardIndex);

    printInterface(playerdata);


    //compare the ranks of them. If they are the same, it says so.
    if (firstCard.at(1) == secondCard.at(1)) {
        //the user guessed right, so the values will be copied to the visible cards.
        playerdata.visibleCards.at(firstCardIndex) = firstCard;
        playerdata.visibleCards.at(secondCardIndex) = secondCard;
        //do NOT remove the revealed card indexes
        std::cout << "It's a match! Press any key to continue: ";
    } else {
        //Remove the guesses from your list of previously guessed cards
        playerdata.revealedCardIndexes.erase(find(playerdata.revealedCardIndexes.begin(), playerdata.revealedCardIndexes.end(), firstCardIndex));
        playerdata.revealedCardIndexes.erase(find(playerdata.revealedCardIndexes.begin(), playerdata.revealedCardIndexes.end(), secondCardIndex));
        std::cout << "Not a match. Press any key to continue: ";
    }

    //ignores the last input and stalls the screen until the user types
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::string dummyInput = "";
    std::getline(std::cin, dummyInput);
    std::cout << '\n';
}

//make function which checks for an unsolvable board
//run through the entire game board and check if there is a matching card. If so, delete both.
//What's left are the cards which do not have a match.
//Then, if there are more than one, take one of the remaining cards and find another card in the remianing deck which does match.
//If not, then there is an odd amount of cards; delete it. 
//Returns 0
int checkForSolvability(GameValues& playerdata, CardDeck& deck) {
    std::vector<std::string> copyOfCards = playerdata.allGameCards;
    std::vector<std::string> cardsWithoutMatch;

    //Go through each entry in the list of cards. Check it with every other value. 
    //If there is a match, stop and delete both cards.
    //If not, then add it to cardsWithoutMatch and delete.
    while (0 < copyOfCards.size()) {
        char firstCardRank = copyOfCards.at(0).at(1); //the rank of the first card
        for (std::string x : copyOfCards) {
            if (x.at(1) == firstCardRank) {
                //only erase if x is not the card.
                if (x != copyOfCards.at(0)) {
                    //erase where x is 
                    copyOfCards.erase(find(copyOfCards.begin(), copyOfCards.end(), x));
                    //erase the first element
                    copyOfCards.erase(copyOfCards.begin());
                    break;
                }
            }
        }
        //if it gets through the for loop, there are no matching cards.
        //Add it to the list of cards without a match and erase it.
        cardsWithoutMatch.push_back(copyOfCards.at(0));
        copyOfCards.erase(copyOfCards.begin());
    }

    //checks if the number of cards is odd
    if ((playerdata.allGameCards.size() % 2) != 0) {
        //If it's odd, make it even by deleting a card without a match.
        //Finds where the first card without a match it and deletes it.
        playerdata.allGameCards.erase(find(playerdata.allGameCards.begin(), playerdata.allGameCards.end(), cardsWithoutMatch.at(0)));
        //then, delete it from the list of unmatched cards.
        cardsWithoutMatch.erase(cardsWithoutMatch.begin());
    }

    //Find a card that does match, and replace the card after)
    while ((0 < cardsWithoutMatch.size()) && (0 < deck.findDeckSize())) {
        //Look for a card in the remaining deck which matches the rank of the first card.
        std::string firstCard = cardsWithoutMatch.at(0);
        std::string newCard = deck.findFromRankAndDelete(firstCard.at(1));

        //erases the second card without match from the original deck
        playerdata.allGameCards.erase(find(playerdata.allGameCards.begin(), playerdata.allGameCards.end(), cardsWithoutMatch.at(1)));
        //adds the new card
        playerdata.allGameCards.push_back(newCard);

        //erase the first two cards
        cardsWithoutMatch.erase(cardsWithoutMatch.begin());
        //prevents the program from erasing a card that's not there
        if (0 < cardsWithoutMatch.size()) {
            cardsWithoutMatch.erase(cardsWithoutMatch.begin());
        }
    }

    return 0;
}

void initializeAndAsk(GameValues& playerdata) {
    std::cout << "Welcome to the memory card matching game!\nHow many cards do you want to match for?" << std::endl;
    std::cout << "Enter a number betweeen 2 and 52. Odd numbers are subtracted by 1: ";
    GameValues null;
    int userInput = inputAndValidateForBoard(2, 52, playerdata);
    if ((userInput % 2) != 0) {
        userInput -= 1;
    }
    playerdata.userInputCardsToPlay = userInput;
}

int main() {
    CardDeck a;
    GameValues playerdata;

    initializeAndAsk(playerdata);

    //initializes and creates the deck
    a.makeDeck();

    //makes game cards
    playerdata.allGameCards = a.drawRandomCards(playerdata.userInputCardsToPlay);

    //makes sure it's solvable and modifies it if it's not.
    checkForSolvability(playerdata, a);

    //creates a separate vector named visibleCards which is set to back facing cards
    for (std::string& x : playerdata.allGameCards) {
        playerdata.visibleCards.push_back("nn");
    }

    while (playerdata.visibleCards != playerdata.allGameCards) {
        gameLoop(playerdata);
    }

    std::cout << "ya did it ya dingus, ya really did it. ya get a thumbs up from me." << std::endl;
    return 0;
}