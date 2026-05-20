#include <iostream>
#include <vector>
#include <string>


class CardDeck {
    private:
        //if you want a funky ass deck of cards, you can just add more suits or ranks in this list
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
};

void printCard(std::string cardToPrint) {
    char rank = cardToPrint.at(1);
    std::string suit;
    std::string mouth;
    std::string eye1;
    std::string eye2;

    switch(cardToPrint.at(0)) {
        case 'c': {
            suit = "♣";
            mouth = "_";
            break;
        }
        case 's':{
            suit = "♠";
            mouth = ".";
            break;
        }
        case 'h':{
            suit = "♥";
            mouth = "w";
            break;
        }
        case 'd':{
            suit = "♦";
            mouth = "v";
            break;
        }
    }
    switch(rank) {
        case '1': {
            eye1 = "0";
            eye2 = "0";
            break;
        }
        case '2': {
            eye1 = "*";
            eye2 = "*";
            break;
        }
        case '3': {
            eye1 = "X";
            eye2 = "X";
            break;
        }
        case '4': {
            eye1 = "-"; 
            eye2 = "-";
            break;
        }
        case '5': {
            eye1 = "o";
            eye2 = "0";
            break;
        }
        case '6': {
            eye1 = "L"; 
            eye2 = "L";
            break;
        }
        case '7': {
            eye1 = ">";
            eye2 = "<";
            break;
        }
        case '8': {
            eye1 = "<"; 
            eye2 = "<";
            break;
        }
        case '9': {
            eye1 = "/";
            eye2 = "/";
            break;
        }
        case '0': {
            eye1 = "@"; 
            eye2 = "@";
            break;
        }
        case 'J': {
            eye1 = "0";
            eye2 = "-";
            break;
        }
        case 'Q': {
            eye1 = ";"; 
            eye2 = ";";
            break;
        }
        case 'K': {
            eye1 = "$";
            eye2 = "$";
            break;
        }
        case 'A': {
            eye1 = "="; 
            eye2 = "=";
            break;
        }
    }

    //prints the top 2 parts of the card, displaying the rank
    std::cout << "╭─────╮\n";
    std::cout << "│" << rank << "    │\n";

    //prints a unique card design based on the rank and suit
    std::cout << "│ " << eye1 << mouth << eye2 << " │\n";

    //prints the bottom two parts
    std::cout << "│    " << suit << "│\n";
    std::cout << "╰─────╯\n";
}

int main() {
    CardDeck a;
    a.makeDeck();
    a.printDeck();
    printCard("cQ");
    printCard("d5");
    printCard("sJ");
    printCard("hA");
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