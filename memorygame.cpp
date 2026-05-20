#include <iostream>
#include <vector>
#include <map>
#include <cmath>
#include <string>
#include <stdlib.h>
#include <ctime>


class CardDeck {
    private:
        //if you want a funky ass deck of cards, you can just add more suits or ranks in this list
        //dont forget to also make visuals for your card in the printCard function!
        char suits[4] = {'c', 's', 'h', 'd'};
        char ranks[13] = {'1', '2', '3', '4', '5', '6', '7', '8', '9', 'J', 'Q', 'K', 'A'};
        std::vector<std::string> deck;
    public:
        //goes through the suits and ranks and builds the deck from them
        int makeDeck() {
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
            for (std::string x : deck) {
                std::cout << x << std::endl;
            }
        }

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
};

/**
 * Function: printCardChunk
 * Parameters: a card, and the row of the card you want it to print
 * Description: This function creates visual representations of a card.
 *      The printCards function draws this to build cards.
 *      You specify which of 5 parts of the card to print.
 */
std::string printCardChunk(std::string cardToPrint, int rowToPrint) {
    char suit = cardToPrint.at(0);
    char rank = cardToPrint.at(1);
    std::map <char, std::string> suitMap = {
        {'c', "♣"},
        {'s', "♠"},
        {'h', "♥"},
        {'d', "♦"}
    };
    std::map <char, std::string> mouthMap = {
        {'c', "_"},
        {'s', "."},
        {'h', "w"},
        {'d', "v"}
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
        {'A', "=="}
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

int main() {
    CardDeck a;
    a.makeDeck();
    a.printDeck();

    std::cout << "Welcome! Please resize your terminal window to fit these cards: " << std::endl;


    std::vector<std::string> thirtyRandomCards = a.drawRandomCards(30);
    printCards(thirtyRandomCards);
    a.printDeck();
    /* printCard("cQ");
    printCard("d5");
    printCard("sJ");
    printCard("hA"); */




    return 0;
}


/*
Card design:
╭─────╮
│2    │
│ 0_0 │
│    ♣│
╰─────╯

Top part:
╭─────╮

Bottom part:
╰─────╯

Middle part eyes:
│ 0_0 │ 1
│ *_* │ 2
│ X_X │ 3
│ -_- │ 4
│ o_0 │ 5
│ L_L │ 6
│ >_< │ 7
│ <_< │ 8
│ /_/ │ 9
│ @_@ │ 0
│ O_- │ j
│ ;_; │ q
│ $_$ │ k
│ =_= │ a

mouth:
│ 0_0 │ c
│ 0.0 │ s
│ 0w0 │ h
│ 0v0 │ d
*/