#include <iostream>
#include <vector>
#include <string>
#include <array>


class CardDeck {
    private:
        //if you want a funky ass deck of cards, you can just add more suits or ranks in this list
        char suits[4] = {'c', 's', 'h', 'd'};
        char ranks[13] = {'1', '2', '3', '4', '5', '6', '7', '8', '9', 'j', 'q', 'k', 'a'};
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



int main() {
    CardDeck a;
    a.makeDeck();
    a.printDeck();

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

Middle part:
│ 0_0 │ 1
│ *-* │ 2
│ o-o │ 3
│ -_- │ 4
│ o_0 │ 5
│ L-L │ 6
│ >_< │ 7
│ <o< │ 8
│ /-/ │ 9
│ 0o0 │ 0
│ 0w0 │ j
│ >w< │ q
│ OvO │ k
│ =-= │ a
*/