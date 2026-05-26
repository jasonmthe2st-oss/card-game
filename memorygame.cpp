/**
 * File: memorygame.cpp
 * Description: Creates a memory matching game using visual representations of a 52-card deck.
 * Date: 24 May 2026
 * TODO:
 *      1. Make the random generation always winnable
 *          (currently, there can be cards with no matches)
 *          (Done!!)
 *      2. Add a title screen and menu asking how many cards
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
            int i = 0;
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
            //if it gets through the whole list without finding one, returns error.
            return "No Card Found";
        }
};

struct GameValues {
    std::vector<std::string> allGameCards;
    std::vector<std::string> visibleCards;
    std::vector<std::string> temporarilyVisibleCards;
    std::vector<int> revealedCardIndexes;
    int userInputCardsToPlay;
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
void printCards(std::vector<std::string> cardsToPrint) {
    const double rootOnePointFive = sqrt(3.0 / 2.0);
    /**
     * The cards per row is defined by this function:
     * y = sqrt(1.5) * x^0.5.
     * 6 -> 3, 24 -> 6, 96 -> 12
     * Takes the ceiling of it so it's not less than 1
     */
    const int cardsPerRow = ceil(rootOnePointFive * pow(cardsToPrint.size(), 0.5));
    //finds the amount of rows of 5 cards. ex. if 15, then 3
    const int normalRows = cardsToPrint.size() / cardsPerRow;
    //If the amount of cards is not a multiple of 5, it stores the remainder here
    const int cardsInLastRow = cardsToPrint.size() % cardsPerRow;

    //repeats for the amount of rows of 5 cards
    for (int rowIndex = 0; rowIndex < normalRows; rowIndex++) {
        //Makes an index of the 5 cards we're working with
        std::vector<std::string> listOfFiveCards;
        int startIndex = rowIndex * cardsPerRow;
        for (int j = startIndex; j < startIndex + cardsPerRow; j++) {
            listOfFiveCards.push_back(cardsToPrint.at(j));
        }

        //prints each chunk of each card
        for (int row = 1; row <= 5; row++) {
            for (int card = 0; card < cardsPerRow; card++) {
                std::cout << printCardChunk(listOfFiveCards.at(card), row) << " ";
            }
            std::cout << "\n";
        }
        std::cout << '\n';
    }

    //then, it prints the last cards of the remaining uneven row
    int startIndex = normalRows * cardsPerRow;
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
    std::cout << "\x1b[2J\x1b[H" << "Cards are counted from left to right, top to bottom." << std::endl;
    printCards(playerdata.temporarilyVisibleCards);
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

//input the original values
void gameLoop(GameValues& playerdata) {
    //sets temporary visibility to all visible cards
    playerdata.temporarilyVisibleCards = playerdata.visibleCards;
    printInterface(playerdata);

    //ask for first card to check
    std::cout << "Enter your first card to check: ";
    //gets user input. This is the index
    const int firstCardIndex = (inputAndValidateForBoard(1, playerdata.allGameCards.size(), playerdata) - 1);
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
    const int secondCardIndex = (inputAndValidateForBoard(1, playerdata.allGameCards.size(), playerdata) - 1);
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

    //Find a card that does match, and replace the card after
    while (0 < cardsWithoutMatch.size()) {
        //Look for a card in the remaining deck which matches the rank of the first card.
        std::string firstCard = cardsWithoutMatch.at(0);
        std::string newCard = deck.findFromRankAndDelete(firstCard.at(1));

        //erases the second card without match from the original deck
        playerdata.allGameCards.erase(find(playerdata.allGameCards.begin(), playerdata.allGameCards.end(), cardsWithoutMatch.at(1)));
        //adds the new card
        playerdata.allGameCards.push_back(newCard);

        //erase the first two cards
        cardsWithoutMatch.erase(cardsWithoutMatch.begin());
        cardsWithoutMatch.erase(cardsWithoutMatch.begin());
    }

    return 0;
}

void initializeAndAsk(GameValues& playerdata) {
    std::cout << "Welcome to the memory card matching game!\nHow many cards do you want to match for?" << std::endl;
    std::cout << "Enter a number betweeen 2 and 54. Odd numbers are subtracted by 1: ";
    GameValues null;
    int userInput = inputAndValidateForBoard(2, 54, playerdata);
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