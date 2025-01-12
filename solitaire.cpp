#include<iostream>
#include <cstdlib> // for rand() and srand() for string to int
#include<ctime>
#include <string>   //for getline
using namespace std;
////////////////////////////////////card class//////////////////////////////////////////////
class card {
public:
    char suit;   
    int rank;   
    bool faceUp; 

    card(char s=' ', int r=0, bool face = false) : suit(s), rank(r), faceUp(face) {}
    ~card(){}

    card& operator=(const card& other) {
        if (this != &other) { 
            this->suit = other.suit;
            this->rank = other.rank;
            this->faceUp = other.faceUp;
        }
        return *this; 
    }
        friend ostream& operator<<(ostream& os, const card& c) {
            if (!c.faceUp) {
                os << "[ ]";
                return os;
            }
            // ANSI color codes for red
            string RED = "\033[31m";
            string RESET = "\033[0m";

            if (c.suit == 'H' || c.suit == 'D') {
                os << RED; 
            }
            switch (c.rank) {
            case 1:
                os << "A"; 
                break;
            case 11:
                os << "J";  
                break;
            case 12:
                os << "Q";  
                break;
            case 13:
                os << "K";  
                break;
            default:
                os << c.rank;  
            }
            switch (c.suit) {

            case 'H':
                cout << "\x03";  
                break;
            case 'D':
                cout << "\x04";  
                break;
            case 'S':
                cout << "\x06";
                break;
            case 'C':
                cout << "\x05";  
                break;
            default:
                cout << "?";  
                break;
            }
            if (c.suit == 'H' || c.suit == 'D') {
                os << RESET;  
            }
            return os;
        }
    };
///////////////////////////////////////doubly linked list with iterators////////////////////////////////////////////
template<typename T>
class DLL {
public:
    class node {
    public:
        T data;         //obj to store cards
        node* next;
        node* prev;
        node(T c) : data(c), next(nullptr), prev(nullptr) {}
    };
    node* head;
    node* tail;
    int size;

    DLL() : head(nullptr), tail(nullptr),size(0) {}

    class iterator {
        friend class DLL;
        iterator(node* p) : curr(p) {}
        node* curr;
    public:
        iterator() : curr(nullptr) {}
        node* getcurr() {
            return curr;
        }
        T& operator*() {
            return curr->data;
        }
        iterator& operator=(const iterator& rhs) {
            if (this != &rhs) {  // Check for self-assignment
                this->curr = rhs.curr;
            }
            return *this;
        }
        iterator& operator++() {   // pre-increment
            curr = curr->next;
            return *this;
        }
        iterator operator++(int) {    // post-increment
            iterator old = *this;
            curr = curr->next;
            return old;
        }
        iterator& operator--() {   // pre-decrement
            curr = curr->prev;
            return *this;
        }
        iterator operator--(int) {   // post-decrement
            iterator old = *this;
            curr = curr->prev;
            return old;
        }
        bool operator==(iterator rhs) const {
            return curr == rhs.curr;
        }
        bool operator!=(iterator rhs) const {
            return curr != rhs.curr;
        }
    };
    iterator begin() {
        return iterator(head);
    }
    iterator end() {
        return iterator(tail);  
    }
    void insertatstart(card c) {
        node* t = new node(c);
        if (head == nullptr) {  // List is empty
            head = tail = t;
        }
        else {
            t->next = head;
            head->prev = t;
            head = t;
        }
        size++;
    }
    void insertatend(T c) {
        node* t = new node(c);
        if (head == nullptr) {  // List is empty
            head = tail = t;
        }
        else {
            t->prev = tail;
            tail->next = t;
            tail = t;
        }
        size++;
    }
    int getsize() {
        return size;
    }
    void deletestart() {
        if (head == nullptr) {
            cout << "emptyyyyyy";
            return;
        }  // List is empty

        node* t = head;
        if (head == tail) {  // Only one element in the list
            head = tail = nullptr;
        }
        else {
            head = head->next;
            head->prev = nullptr;
        }
        size--;
        delete t;
    }
    T deleteend() {                                  //this function removes a card from column end and returns it so can e added to other column
        if (head == nullptr)
            cout<<"empty";  
        T removed = tail->data;  // Store the card to return later

        if (head == tail) {  // Only one node in the list
            delete tail;
            head = tail = nullptr;
        }
        else {
            node* temp = tail;
            tail = tail->prev;
            tail->next = nullptr;
            delete temp;
        }
        size--;
        return removed;  // Return the card that was removed
    }
    T gettail() {
        return tail->data;
    }
    T gethead() {
        return head->data;
    }
    void printlinkedlist() {
        node* t = head;
        cout << size << " cards" << endl;
        while (t != nullptr) {
            cout << t->data <<endl;  
            t = t->next;
        }
    }
    bool isEmpty() {
        return size == 0;
    }
    void erase(iterator& it) {
        if (it.curr == nullptr) return;  // Iterator is null, nothing to erase
        node* toDelete = it.curr;
        if (toDelete == head) {
            head = head->next;
            if (head) head->prev = nullptr;
            else tail = nullptr;  // If the list becomes empty
        }
        else if (toDelete == tail) {
            tail = tail->prev;
            if (tail) tail->next = nullptr;
            else head = nullptr;  // If the list becomes empty
        }
        else {
            toDelete->prev->next = toDelete->next;
            toDelete->next->prev = toDelete->prev;
        }
        if (toDelete == tail) {
            it.curr = nullptr;
        }
        else {
            it.curr = toDelete->next;
        }
        delete toDelete;
        size--;       
    }
    ~DLL() {
        while (head != nullptr) {
            deletestart();
        }
    }
};
/////////////////////////////////////////////////////////stack class////////////////////////////////////////////////////
template<class T>
class stack {
private:
    DLL<T> s;
public:
    stack() {}
    int getsize() {
        return s.getsize();
    }
    void push(T data) {
        s.insertatend(data);
    }
    bool pop(T& result) {  // result is an output parameter to store the popped value
        if (s.getsize() == 0) {
            return false;  // Stack is empty
        }
        result = s.deleteend();  // Remove and assign the last element to result
        return true;  // Successfully popped an element
    }
    bool top(T& result) {  // result is an output parameter to store the top card
        if (s.getsize() == 0) {
            return false;  // Stack is empty
        }
        result = s.tail->data;  // Assign the top element to result
        return true;  // Successfully retrieved the top element
    }
    bool isEmpty() {
        return s.getsize() == 0;
    }
    void printStack() {
        if (s.getsize() == 0) {
            cout << "[   ]" << endl;
            cout << "( " << s.getsize() << " cards )" << endl;
        }
        else {
            card temp = top();        //get last card
            cout << temp << endl;
            cout << "( " << s.getsize() << " cards )" << endl;

        }
    }
    ~stack() {}
};
////////////////////////////COMMAND CLASS////////////////////////////////////
class command {
public:
    string cmd;
    string source;
    string destination;
    int numCards;

    command(string c=" ", string s = " ", string d = " ", int num = 0) :cmd(c), source(s), destination(d), numCards(num) {}
    ~command(){}
};
////////////////////////////////GAME CLASS/////////////////////////////////////////////
class game {
private:
    DLL<card>* tableau;          
    stack<card>* foundation;   
    stack<card>* stock;          
    stack<card>* waste;        
    stack<command>* commands;
    card* allcards;
public:
    game() {
        tableau = new DLL<card>[7];
        foundation = new stack<card>[4];
        stock = new stack<card>();
        waste = new stack<card>();
        commands = new stack<command>();
        allcards = new card[52];
    }
    ~game() {
        delete[] tableau;                     
        delete[] foundation;                  
        delete stock;                         
        delete waste;                         
        delete commands;  
        delete[] allcards;
    }
    void initializeCards(card* c) {
        int index = 0;
        char suits[4] = { 'H', 'D', 'S', 'C' }; // Define the 4 suits
        for (int i = 0; i < 4; ++i) {         // Loop through each suit
            for (int rank = 1; rank <= 13; ++rank) { // Loop through each rank (1-13)
                c[index] = card(suits[i], rank);
                index++; // Increment index to fill the next card in the deck
            }
        }
    }
    void shuffleCards(card* c,int size) {
        srand(static_cast<unsigned int>(time(nullptr)));
        // Shuffle the deck using the Fisher-Yates algorithm
        for (int i = size - 1; i > 0; --i) {
            int j = rand() % (i + 1);
            swap(c[i], c[j]);
        }
    }
    void distributeCards() {
        int cardIndex = 0;  // to track the next card from the shuffled deck
        for (int i = 0; i < 7; ++i) {  // for each column (i-th column has i+1 cards)
            for (int j = 0; j <= i; ++j) {
                bool isFaceUp = (j==i);  // The last card in each column should be face-up
                allcards[cardIndex].faceUp = isFaceUp;  // Set the face-up status of the card
                tableau[i].insertatend(allcards[cardIndex]);  // Insert the card into the i-th column
                cardIndex++;  // Move to the next card in the shuffled deck
            }
        }
        // The remaining cards go into the stock pile
        while (cardIndex < 52) {
            stock->push(allcards[cardIndex]);  // Push remaining cards to stock
            cardIndex++;
        }
    }
    void initializeGame() {
       // no need to intialize the foundation and stock as empty because by default constructer of game they will be empty

        //now make cards array 
        initializeCards(allcards);

        //shuffle them 
        shuffleCards(allcards,52);

       //distribution of cards into tableau and stock pile
        distributeCards();
    }
    void gameDisplay() {

        cout << "- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -  \n\n";
        // Display Stock, Waste, and Foundations side by side
        cout << "Stock        Waste         Foundation 1          Foundation 2          Foundation 3          Foundation 4" << endl;

        // Display Stock, Waste, and Foundations on the same line
        if (stock->isEmpty()) {
            cout << "[ ";
            cout << "]  ";
        }
        else {
            card temp;
            if (stock->top(temp)) {
                cout << temp << "  ";
            }
        }
        cout << "     ";  // Space between Stock and Waste
        // Display Waste
        if (waste->isEmpty()) {
            cout << "   [ ";
            cout << "]     ";
        }
        else {
            card temp;
            bool found = waste->top(temp);
            temp.faceUp = true;   //so that last card is always facing up
            cout << "  " << temp << "        ";
        }
        // Display Foundations side by side
        for (int i = 0; i < 4; ++i) {
            if (foundation[i].isEmpty()) {
                cout << "            [ ";
                cout << "]      ";
            }
            else {
                card temp;
                bool f = foundation[i].top(temp);
                temp.faceUp = true;
                cout <<"             "<< temp << "      ";
            }
        }
        cout << endl;
        // Display card counts directly under Stock, Waste, and Foundations names
        cout << "(" << stock->getsize() << " cards)   (" << waste->getsize() << " cards)   ";
        for (int i = 0; i < 4; ++i) {
            cout << "     (" << foundation[i].getsize() << " cards)        ";
        }
        cout << endl << endl;  // Extra line to separate Stock/Waste/Foundation from Tableau

        // Display Tableau column headers
        cout << "Column 1     Column 2     Column 3     Column 4     Column 5     Column 6     Column 7" << endl;

        // Find the maximum number of cards in any tableau column
        int maxHeight = 0;
        for (int i = 0; i < 7; ++i) {
            if (tableau[i].getsize() > maxHeight) {
                maxHeight = tableau[i].getsize();
            }
        }
        // Print each row of cards across all columns directly under their column names
        for (int row = 0; row < maxHeight; ++row) {
            for (int col = 0; col < 7; ++col) {
                int columnSize = tableau[col].getsize();
                if (row < columnSize) {
                    // Use an iterator to move through the linked list
                    DLL<card>::iterator it = tableau[col].begin();
                    for (int k = 0; k < row; ++k) {
                        ++it; // Move the iterator to the current row in this column
                    }
                    cout << *it;  // Print the card at this row in the column
                }
                else {
                    cout << "   ";  // Print empty spaces if no card is present in this row for this column
                }
                // Space between columns
                if (col != 6) {
                    cout << "           ";
                }
            }
            cout << endl;  // Move to the next row
        }
        cout << endl;
        // Print card counts directly under each column name
        for (int i = 0; i < 7; ++i) {
            cout << "(" << tableau[i].getsize() << " cards)";
            if (i != 6) {  // Ensure no extra space after the last column
                cout << "    ";
            }
        }
        cout << endl;
    }

    bool drawCard(string cmd) {

        command temp(cmd);
        commands->push(temp);           //pushed the command onto commands stack

        card tempcard;
        if (stock->pop(tempcard)) {        //got last card from stock pile
            waste->push(tempcard);
            return true;
        }
        else {
            return false;
        }
    }

    void refreshStock() {
        card tempCard;
        while (!waste->isEmpty()) {
            waste->pop(tempCard);    
            tempCard.faceUp = false; 
            stock->push(tempCard);   
        }
    }

    void undofoundtoCols(command last,int f) {
        //from foundations  to colsss
                    card temp;
                    if (last.destination == "c1") {

                        temp = tableau[0].deleteend();
                        foundation[f].push(temp);
                    }
                    else if (last.destination == "c2") {

                        temp = tableau[1].deleteend();
                        foundation[f].push(temp);
                    }
                    else if (last.destination == "c3") {

                        temp = tableau[2].deleteend();
                        foundation[f].push(temp);
                    }
                    else if (last.destination == "c4") {

                        temp = tableau[3].deleteend();
                        foundation[f].push(temp);
                    }
                    else if (last.destination == "c5") {

                        temp = tableau[4].deleteend();
                        foundation[f].push(temp);
                    }
                    else if (last.destination == "c6") {

                        temp = tableau[5].deleteend();
                        foundation[f].push(temp);
                    }
                    else if (last.destination == "c7") {

                        temp = tableau[6].deleteend();
                        foundation[f].push(temp);
                    }

    }
        bool undomovecolumntoColumn(int s, int d, int numCards) {
            int sourceSize = tableau[s].getsize();
            int startIdx = sourceSize - numCards;  
            // Ensure that the last card in the dest column is face-down (if any card remains)
            if (!tableau[d].isEmpty()) {
                tableau[d].tail->data.faceUp = false;  
            }
            if (startIdx < 0) {
                cout << "Invalid undo operation! Not enough cards in the column to undo." << endl;
                return false;
            }
            DLL<card>::iterator it = tableau[s].begin();

            // Move the iterator to the (sourceSize - numCards)th card
            for (int i = 0; i < startIdx; ++i) {
                ++it;
            }

            for (int i = 0; i < numCards; ++i) {
                DLL<card>::iterator nextIt = it;
                if (i < numCards - 1) {  // Only increment if there are more cards to move
                    ++nextIt;
                }
                tableau[d].insertatend(*it);
                tableau[s].erase(it);

                // Update the iterator to the next card, only if it's not the last card being moved
                if (i < numCards - 1) {
                    it = nextIt;
                }
            }

            if (!tableau[s].isEmpty() && tableau[s].tail->prev->data.faceUp!=true) {
                tableau[s].tail->data.faceUp = true;  // Ensure the last card in the source column is face-up
            }

            return true;
        }
    void undo() {

        command lastCommand;
        if (commands->pop(lastCommand)) {
            if (lastCommand.cmd == "s") {
                card tempcard;
                if (waste->pop(tempcard)) {        //got last card from waste pile
                    tempcard.faceUp = false;
                    stock->push(tempcard);
                    
                }
            }
            else {
                if (lastCommand.source == "w") {
                    bool fazool1;
                    card temp;
                    if (lastCommand.destination == "f1") {
                       fazool1= foundation[0].pop(temp);
                       waste->push(temp);
                    }
                    else if (lastCommand.destination == "f2") {
                        fazool1 = foundation[1].pop(temp);
                        waste->push(temp);
                    }
                    else if (lastCommand.destination == "f3") {
                        fazool1 = foundation[2].pop(temp);
                        waste->push(temp);
                    }
                    else if (lastCommand.destination == "f4") {
                        fazool1 = foundation[3].pop(temp);
                        waste->push(temp);
                    }
                    else if (lastCommand.destination == "c1") {
                        temp=tableau[0].deleteend();
                        waste->push(temp);
                    }
                    else if (lastCommand.destination == "c2") {
                        temp = tableau[1].deleteend();
                        waste->push(temp);
                    }
                    else if (lastCommand.destination == "c3") {
                        temp = tableau[2].deleteend();
                        waste->push(temp);
                    }
                    else if (lastCommand.destination == "c4") {
                        temp = tableau[3].deleteend();
                        waste->push(temp);
                    }
                    else if (lastCommand.destination == "c5") {
                        temp = tableau[4].deleteend();
                        waste->push(temp);
                    }
                    else if (lastCommand.destination == "c6") {
                        temp = tableau[5].deleteend();
                        waste->push(temp);
                    }
                    else if (lastCommand.destination == "c7") {
                        temp = tableau[6].deleteend();
                        waste->push(temp);
                    }
                }
                //from found to cols
                else if (lastCommand.source == "f1") {
                    undofoundtoCols(lastCommand,0);
                }
                else if (lastCommand.source == "f2") {
                    undofoundtoCols(lastCommand , 1);
                }
                else if (lastCommand.source == "f3") {
                    undofoundtoCols(lastCommand,2);
                }
                else if (lastCommand.source == "f4") {
                    undofoundtoCols(lastCommand,3);
                }
                //from cols to foundation and other col
                else if (lastCommand.source == "c1") {
                    card temp;
                    if (lastCommand.destination == "f1") {
                        foundation[0].pop(temp);    //got the last card
                        tableau[0].tail->data.faceUp = false;
                        tableau[0].insertatend(temp); //inserted at col1 end
                    }
                    else  if (lastCommand.destination == "f2") {
                        foundation[1].pop(temp);    //got the last card
                        tableau[0].tail->data.faceUp = false;
                        tableau[0].insertatend(temp); //inserted at col1 end
                    }
                    else if (lastCommand.destination == "f3") {
                        foundation[2].pop(temp);    //got the last card
                        tableau[0].tail->data.faceUp = false;
                        tableau[0].insertatend(temp); //inserted at col1 end
                    }
                    else if (lastCommand.destination == "f4") {
                        foundation[3].pop(temp);    //got the last card
                        tableau[0].tail->data.faceUp = false;
                        tableau[0].insertatend(temp); //inserted at col1 end
                    }
                    //add for cols to cols
                    else if (lastCommand.destination == "c2") {
                       undomovecolumntoColumn(1,0, lastCommand.numCards);
                    }
                    else if (lastCommand.destination == "c3") {
                        undomovecolumntoColumn(2,0, lastCommand.numCards);
                    }
                    else if (lastCommand.destination == "c4") {
                        undomovecolumntoColumn(3,0, lastCommand.numCards);
                    }
                    else if (lastCommand.destination == "c5") {
                        undomovecolumntoColumn(4,0, lastCommand.numCards);
                    }
                    else if (lastCommand.destination == "c6") {
                        undomovecolumntoColumn(5,0, lastCommand.numCards);
                    }
                    else if (lastCommand.destination == "c7") {
                       undomovecolumntoColumn(6,0, lastCommand.numCards);
                    }
                }

                else if (lastCommand.source == "c2") {
                    card temp;
                    if (lastCommand.destination == "f1") {
                        foundation[0].pop(temp);    //got the last card
                        tableau[1].tail->data.faceUp = false;
                        tableau[1].insertatend(temp); //inserted at col2 end
                    }
                    else  if (lastCommand.destination == "f2") {
                        foundation[1].pop(temp);    //got the last card
                        tableau[1].tail->data.faceUp = false;
                        tableau[1].insertatend(temp); //inserted at col2 end
                    }
                    else if (lastCommand.destination == "f3") {
                        foundation[2].pop(temp);    //got the last card
                        tableau[1].tail->data.faceUp = false;
                        tableau[1].insertatend(temp); //inserted at col2 end
                    }
                    else if (lastCommand.destination == "f4") {
                        foundation[3].pop(temp);    //got the last card
                        tableau[1].tail->data.faceUp = false;
                        tableau[1].insertatend(temp); //inserted at col2 end
                    }
                    //add for cols to cols
                    else if (lastCommand.destination == "c1") {
                        undomovecolumntoColumn(0,1, lastCommand.numCards);
                    }
                    else if (lastCommand.destination == "c3") {
                        undomovecolumntoColumn(2,1, lastCommand.numCards);
                    }
                    else if (lastCommand.destination == "c4") {
                        undomovecolumntoColumn(3,1, lastCommand.numCards);
                    }
                    else if (lastCommand.destination == "c5") {
                        undomovecolumntoColumn(4,1, lastCommand.numCards);
                    }
                    else if (lastCommand.destination == "c6") {
                        undomovecolumntoColumn(5,1, lastCommand.numCards);
                    }
                    else if (lastCommand.destination == "c7") {
                        undomovecolumntoColumn(6,1, lastCommand.numCards);
                    }
                }
                else if (lastCommand.source == "c3") {
                    card temp;
                    if (lastCommand.destination == "f1") {
                        foundation[0].pop(temp);    //got the last card
                        tableau[2].tail->data.faceUp = false;
                        tableau[2].insertatend(temp); //inserted at col1 end
                    }
                    else  if (lastCommand.destination == "f2") {
                        foundation[1].pop(temp);    //got the last card
                        tableau[2].tail->data.faceUp = false;
                        tableau[2].insertatend(temp); //inserted at col1 end
                    }
                    else if (lastCommand.destination == "f3") {
                        foundation[2].pop(temp);    //got the last card
                        tableau[2].tail->data.faceUp = false;
                        tableau[2].insertatend(temp); //inserted at col1 end
                    }
                    else if (lastCommand.destination == "f4") {
                        foundation[3].pop(temp);    //got the last card
                        tableau[2].tail->data.faceUp = false;
                        tableau[2].insertatend(temp); //inserted at col1 end
                    }
                    //add for cols to cols
                    else if (lastCommand.destination == "c1") {
                        undomovecolumntoColumn(0,2, lastCommand.numCards);
                    }
                    else if (lastCommand.destination == "c2") {
                        undomovecolumntoColumn(1,2, lastCommand.numCards);
                    }
                    else if (lastCommand.destination == "c4") {
                        undomovecolumntoColumn(3, 2, lastCommand.numCards);
                    }
                    else if (lastCommand.destination == "c5") {
                        undomovecolumntoColumn(4,2, lastCommand.numCards);
                    }
                    else if (lastCommand.destination == "c6") {
                        undomovecolumntoColumn(5,2, lastCommand.numCards);
                    }
                    else if (lastCommand.destination == "c7") {
                        undomovecolumntoColumn(6,2, lastCommand.numCards);
                    }
                    }
                else if (lastCommand.source == "c4") {
                        card temp;
                        if (lastCommand.destination == "f1") {
                            foundation[0].pop(temp);    //got the last card
                            tableau[3].tail->data.faceUp = false;
                            tableau[3].insertatend(temp); //inserted at col1 end
                        }
                        else  if (lastCommand.destination == "f2") {
                            foundation[1].pop(temp);    //got the last card
                            tableau[3].tail->data.faceUp = false;
                            tableau[3].insertatend(temp); //inserted at col1 end
                        }
                        else if (lastCommand.destination == "f3") {
                            foundation[2].pop(temp);    //got the last card
                            tableau[3].tail->data.faceUp = false;
                            tableau[3].insertatend(temp); //inserted at col1 end
                        }
                        else if (lastCommand.destination == "f4") {
                            foundation[3].pop(temp);    //got the last card
                            tableau[3].tail->data.faceUp = false;
                            tableau[3].insertatend(temp); //inserted at col1 end
                        }
                        //add for cols to cols
                        else if (lastCommand.destination == "c1") {
                            undomovecolumntoColumn(0,3, lastCommand.numCards);
                        }
                        else if (lastCommand.destination == "c2") {
                            undomovecolumntoColumn(1,3, lastCommand.numCards);
                        }
                        else if (lastCommand.destination == "c3") {
                            undomovecolumntoColumn(2,3, lastCommand.numCards);
                        }
                        else if (lastCommand.destination == "c5") {
                            undomovecolumntoColumn(4,3, lastCommand.numCards);
                        }
                        else if (lastCommand.destination == "c6") {
                            undomovecolumntoColumn(5,3, lastCommand.numCards);
                        }
                        else if (lastCommand.destination == "c7") {
                            undomovecolumntoColumn(6,3, lastCommand.numCards);
                        }
                }
                else if (lastCommand.source == "c5") {
                    card temp;
                    if (lastCommand.destination == "f1") {
                        foundation[0].pop(temp);    //got the last card
                        tableau[4].tail->data.faceUp = false;
                        tableau[4].insertatend(temp); //inserted at col1 end
                    }
                    else  if (lastCommand.destination == "f2") {
                        foundation[1].pop(temp);    //got the last card
                        tableau[4].tail->data.faceUp = false;
                        tableau[4].insertatend(temp); //inserted at col1 end
                    }
                    else if (lastCommand.destination == "f3") {
                        foundation[2].pop(temp);    //got the last card
                        tableau[4].tail->data.faceUp = false;
                        tableau[4].insertatend(temp); //inserted at col1 end
                    }
                    else if (lastCommand.destination == "f4") {
                        foundation[3].pop(temp);    //got the last card
                        tableau[4].tail->data.faceUp = false;
                        tableau[4].insertatend(temp); //inserted at col1 end
                    }
                    //add for cols to cols
                    else if (lastCommand.destination == "c1") {
                        undomovecolumntoColumn(0,4, lastCommand.numCards);
                    }
                    else if (lastCommand.destination == "c2") {
                        undomovecolumntoColumn(1,4, lastCommand.numCards);
                    }
                    else if (lastCommand.destination == "c3") {
                        undomovecolumntoColumn(2, 4, lastCommand.numCards);
                    }
                    else if (lastCommand.destination == "c4") {
                        undomovecolumntoColumn(3,4, lastCommand.numCards);
                    }
                    else if (lastCommand.destination == "c6") {
                        undomovecolumntoColumn(5,4, lastCommand.numCards);
                    }
                    else if (lastCommand.destination == "c7") {
                        undomovecolumntoColumn(6,4, lastCommand.numCards);
                    }
                    }
                else if (lastCommand.source == "c6") {
                    card temp;
                    if (lastCommand.destination == "f1") {
                        foundation[0].pop(temp);    //got the last card
                        tableau[5].tail->data.faceUp = false;
                        tableau[5].insertatend(temp); //inserted at col1 end
                    }
                    else  if (lastCommand.destination == "f2") {
                        foundation[1].pop(temp);    //got the last card
                        tableau[5].tail->data.faceUp = false;
                        tableau[5].insertatend(temp); //inserted at col1 end
                    }
                    else if (lastCommand.destination == "f3") {
                        foundation[2].pop(temp);    //got the last card
                        tableau[5].tail->data.faceUp = false;
                        tableau[5].insertatend(temp); //inserted at col1 end
                    }
                    else if (lastCommand.destination == "f4") {
                        foundation[3].pop(temp);    //got the last card
                        tableau[5].tail->data.faceUp = false;
                        tableau[5].insertatend(temp); //inserted at col1 end
                    }
                    //add for cols to cols
                    else if (lastCommand.destination == "c1") {
                        undomovecolumntoColumn(0,5, lastCommand.numCards);
                    }
                    else if (lastCommand.destination == "c2") {
                        undomovecolumntoColumn(1,5, lastCommand.numCards);
                    }
                    else if (lastCommand.destination == "c3") {
                        undomovecolumntoColumn(2,5, lastCommand.numCards);
                    }
                    else if (lastCommand.destination == "c4") {
                        undomovecolumntoColumn(3,5, lastCommand.numCards);
                    }
                    else if (lastCommand.destination == "c5") {
                        undomovecolumntoColumn(4, 5, lastCommand.numCards);
                    }
                    else if (lastCommand.destination == "c7") {
                        undomovecolumntoColumn(6,5, lastCommand.numCards);
                    }
                    }
                else if (lastCommand.source == "c7") {
                    card temp;
                    if (lastCommand.destination == "f1") {
                        foundation[0].pop(temp);    //got the last card
                        tableau[6].tail->data.faceUp = false;
                        tableau[6].insertatend(temp); //inserted at col1 end
                    }
                    else  if (lastCommand.destination == "f2") {
                        foundation[1].pop(temp);    //got the last card
                        tableau[6].tail->data.faceUp = false;
                        tableau[6].insertatend(temp); //inserted at col1 end
                    }
                    else if (lastCommand.destination == "f3") {
                        foundation[2].pop(temp);    //got the last card
                        tableau[6].tail->data.faceUp = false;
                        tableau[6].insertatend(temp); //inserted at col1 end
                    }
                    else if (lastCommand.destination == "f4") {
                        foundation[3].pop(temp);    //got the last card
                        tableau[6].tail->data.faceUp = false;
                        tableau[6].insertatend(temp); //inserted at col1 end
                    }
                    //add for cols to cols
                    else if (lastCommand.destination == "c1") {
                        undomovecolumntoColumn(0,6, lastCommand.numCards);
                    }
                    else if (lastCommand.destination == "c2") {
                        undomovecolumntoColumn(1,6, lastCommand.numCards);
                    }
                    else if (lastCommand.destination == "c3") {
                        undomovecolumntoColumn(2,6, lastCommand.numCards);
                    }
                    else if (lastCommand.destination == "c4") {
                        undomovecolumntoColumn(3,6, lastCommand.numCards);
                    }
                    else if (lastCommand.destination == "c5") {
                        undomovecolumntoColumn(4,6, lastCommand.numCards);
                    }
                    else if (lastCommand.destination == "c6") {
                        undomovecolumntoColumn(5,6, lastCommand.numCards);
                    }
                    }
            }
        }
    }

    bool validCommand(string input) {
        string cmd = " ";
        string sourceStr = " ", destStr = " ";
        int numCards = 0;
        int tokenCount = 0;  // Keep track of the number of tokens processed
        int tokenStart = 0;  // Index to keep track of token positions

        // Manually parse the string by spaces and commas
        for (int i = 0; i <= input.size(); ++i) {
            if (input[i] == ' ' || input[i] == ',' || i == input.size()) {
                string token = input.substr(tokenStart, i - tokenStart);
                tokenStart = i + 1;

                // Process tokens based on their position
                switch (tokenCount) {
                case 0:
                    cmd = token;
                    break;
                case 1:
                    // Parse source: cX, w, or fX
                    sourceStr = token;
                    if (sourceStr[0] == 'c') {
                        int source = atoi(sourceStr.substr(1).c_str()); // Extract column number
                        if (source < 1 || source > 7) {
                            cout << "Invalid source column! It must be between 1 and 7." << endl;
                            return false;
                        }
                    }
                    else if (sourceStr == "w") {
                        // Valid wastepile
                    }
                    else if (sourceStr[0] == 'f') {
                        int source = atoi(sourceStr.substr(1).c_str());
                    }
                    else {
                        cout << "Invalid source! Must be 'cX', 'w', or 'fX'." << endl;
                        return false;
                    }
                    break;
                case 2:
                    // Parse destination: cX or fX
                    destStr = token;
                    if (destStr[0] == 'c') {
                        int destination = atoi(destStr.substr(1).c_str()); // Extract column number
                        if (destination < 1 || destination > 7) {
                            cout << "Invalid destination column! It must be between 1 and 7." << endl;
                            return false;
                        }
                    }
                    else if (destStr[0] == 'f') {
                        int destination = atoi(destStr.substr(1).c_str());
                    }
                    else {
                        cout << "Invalid destination! Must be 'cX' or 'fX'." << endl;
                        return false;
                    }
                    break;
                case 3:
                    // Convert numCards to integer
                    numCards = atoi(token.c_str());
                    if (numCards <= 0) {
                        cout << "Invalid number of cards! It must be greater than 0." << endl;
                        return false;
                    }
                    break;
                default:
                    cout << "Invalid input! Too many arguments." << endl;
                    return false;
                }
                ++tokenCount;
            }
        }
        // Perform command completion checks
        if (cmd == " " || sourceStr == " " || destStr == " " || numCards == 0) {
            cout << "Incomplete command! Check cmd, source, destination, or number of cards." << endl;
            return false;
        }
        //source=waste then dest!=waste and numcards==1
        if (sourceStr == "w") {
            if (destStr == "w" || numCards != 1) {
                cout << "Invalid command ! ";
                return false;
            }
        }
        //source=foundation then dest!=w and numcards==1
        if (sourceStr == "f1" || sourceStr == "f2" || sourceStr == "f3" || sourceStr == "f4") {
            if (destStr == "w" && numCards != 1) {
                cout << "Invalid command ! ";
                return false;
            }
        }
        //source=cols 
        if (sourceStr == "c1" || sourceStr == "c2" || sourceStr == "c3" || sourceStr == "c4" || sourceStr == "c5" || sourceStr == "c6" || sourceStr == "c7") {
            if (destStr == "w") {
                cout << "Invalid command ! ";
                return false;
            }
            else if ((destStr == "f1" || destStr == "f2" || destStr == "f3" || destStr == "f4") && numCards != 1) {
                cout << "Invalid command ! blah ";
                return false;
            }
        }
        // If all checks pass, store the command
        command newCommand(cmd, sourceStr, destStr, numCards);
        commands->push(newCommand);

        return true;
    }

    bool wastetoFoundation() {

        if (waste->isEmpty()) {
            cout << "Waste pile is empty ! Move cannot be made" << endl;
            return false;
        }
        //top the command
        command comm;
        commands->top(comm);

        if (comm.destination == "f1") {
            card temp;
            waste->top(temp);

            if (foundation[0].isEmpty()) {
                if (temp.rank != 1) {
                    cout << "The card must be an ACE !" << endl;
                    return false;
                }
                else {
                    card move;
                    waste->pop(move);
                   foundation[0].push(move);
                }
            }

            //when not empty we will check if we can move
            else {
                card check;
               bool fazool= foundation[0].top(check);
               if (temp.suit != check.suit) {
                   cout << "Suits do not match !" << endl;
                   return false;

               }
               else if ((temp.rank - 1) != check.rank) {
                   cout << "Rank match is invalid !" << endl;
                   return false;
               }
             
               //means we can now perform the move
               card tempo;
               fazool= waste->pop(tempo);

               foundation[0].push(tempo);
            }
       }

        if (comm.destination == "f2") {
            card temp;
            waste->top(temp);

            if (foundation[1].isEmpty()) {
                if (temp.rank != 1) {
                    cout << "The card must be an ACE !" << endl;
                    return false;

                }
                else {
                    card move;
                    waste->pop(move);
                    foundation[1].push(move);
                }
            }

            //when not empty we will check if we can move
            else {
                card check;
                bool fazool = foundation[1].top(check);
                if (temp.suit != check.suit) {
                    cout << "Suits do not match !" << endl;
                    return false;
                }
                else if ((temp.rank - 1) != check.rank) {
                    cout << "Rank match is invalid !" << endl;
                    return false;
                }
                //means we can now perform the move
                card tempo;
                fazool = waste->pop(tempo);

                foundation[1].push(tempo);
            }
        }

        if (comm.destination == "f3") {
            card temp;
            waste->top(temp);
            if (foundation[2].isEmpty()) {
                if (temp.rank != 1) {
                    cout << "The card must be an ACE !" << endl;
                    return false;
                }
                else {
                    card move;
                    waste->pop(move);
                    foundation[2].push(move);
                }
            }

            //when not empty we will check if we can move
            else {
                card check;
                bool fazool = foundation[2].top(check);
                if (temp.suit != check.suit) {
                    cout << "Suits do not match !" << endl;
                    return false;
                }
                else if ((temp.rank - 1) != check.rank) {
                    cout << "Rank match is invalid !" << endl;
                    return false;
                }
                //means we can now perform the move
                card tempo;
                fazool = waste->pop(tempo);
                foundation[2].push(tempo);
            }
        }

        if (comm.destination == "f4") {
            card temp;
            waste->top(temp);

            if (foundation[3].isEmpty()) {
                if (temp.rank != 1) {
                    cout << "The card must be an ACE !" << endl;
                    return false;
                }
                else {
                    card move;
                    waste->pop(move);
                    foundation[3].push(move);
                }
            }
           
            //when not empty we will check if we can move
            else {
                card check;
                bool fazool = foundation[3].top(check);
                if (temp.suit != check.suit) {
                    cout << "Suits do not match !" << endl;
                    return false;
                }
                else if ((temp.rank - 1) != check.rank) {
                    cout << "Rank match is invalid !" << endl;
                    return false;
                }
                //means we can now perform the move
                card tempo;
                fazool = waste->pop(tempo);
                foundation[3].push(tempo);
            }
        }
        return true;
    }

    bool movewastetoColumns(int columnIndex) {

        if (waste->isEmpty()) {
            cout << "Waste pile is empty! Move cannot be made." << endl;
            return false;
        }

        card wasteCard;
        waste->top(wasteCard);  

        if (tableau[columnIndex].isEmpty()) {
            if (wasteCard.rank == 13) { 
                wasteCard.faceUp = true;               
                tableau[columnIndex].insertatend(wasteCard); 
                waste->pop(wasteCard);                 
                return true;
            }
            else {
                cout << "Only a King can be placed on an empty column." << endl;
                return false;
            }
        }
        else {
            card columnTopCard = tableau[columnIndex].gettail();

            if ((wasteCard.rank == columnTopCard.rank - 1) &&
                isOppositeColor(wasteCard.suit, columnTopCard.suit)) {
                wasteCard.faceUp = true;               
                tableau[columnIndex].insertatend(wasteCard); 
                waste->pop(wasteCard);                 
                return true;  
            }
            else {
                cout << "The waste card cannot be placed on this column. It must be one rank lower and of the opposite color." << endl;
                return false;
            }
        }
    }

    bool wasteToColumns() {
       
        command prompt;
        commands->top(prompt);

        bool check;

        if (prompt.destination == "c1") {
            check = movewastetoColumns(0);
        }
        else if (prompt.destination == "c2") {
            check = movewastetoColumns(1);
        }
        else if (prompt.destination == "c3") {
            check = movewastetoColumns(2);
        }
        else if (prompt.destination == "c4") {
            check = movewastetoColumns(3);
        }
        else if (prompt.destination == "c5") {
            check = movewastetoColumns(4);
        }
        else if (prompt.destination == "c6") {
            check = movewastetoColumns(5);
        }
        else if (prompt.destination == "c7") {
            check = movewastetoColumns(6);
        }

        return check;
    }

    bool isOppositeColor(char suit1, char suit2) {
        bool isRed1 = (suit1 == 'H' || suit1 == 'D');
        bool isRed2 = (suit2 == 'H' || suit2 == 'D');
        return isRed1 != isRed2;
    }

    bool movefoundationtoColumns(int indexf ,int indexc) {

        card temp;
        bool fazool;

        if (foundation[indexf].isEmpty()) {
            cout << "Foundation is empty !!!";
            return false;
        }
        //not empty
        else {
             foundation[indexf].top(temp);
            if (tableau[indexc].isEmpty()) {
                //only king can be moved
                if (temp.rank == 13) {
                    foundation[indexf].pop(temp);
                    temp.faceUp = true;                      //card must faceup
                    tableau[indexc].insertatend(temp);
                    return true;
                }
            }

            else {
                card columnTopCard = tableau[indexc].gettail();  //get last of col
                if ((temp.rank == columnTopCard.rank - 1) &&
                    isOppositeColor(temp.suit, columnTopCard.suit)) {
                    temp.faceUp = true;
                    tableau[indexc].insertatend(temp);
                    foundation[indexf].pop(temp);
                    return true; 
                }
                else {
                    cout << "Mismatch of cards !";
                    return false;
                }
            }
        }
    }

    bool foundationtoColumns() {

        command prompt;
        commands->top(prompt);

        bool check=true;
        if (prompt.source == "f1") {

            if (prompt.destination == "c1") {
                check = movefoundationtoColumns(0,0);
            }
            else if (prompt.destination == "c2") {
                check = movefoundationtoColumns(0, 1);
            }
            else if (prompt.destination == "c3") {
                check = movefoundationtoColumns(0, 2);
            }
            else if (prompt.destination == "c4") {
                check = movefoundationtoColumns(0, 3);
            }
            else if (prompt.destination == "c5") {
                check = movefoundationtoColumns(0, 4);
            }
            else if (prompt.destination == "c6") {
                check = movefoundationtoColumns(0, 5);
            }
            else if (prompt.destination == "c7") {
                check = movefoundationtoColumns(0, 6);
            }
        }
        else if (prompt.source == "f2") {

            if (prompt.destination == "c1") {
                check = movefoundationtoColumns(1, 0);
            }
            else if (prompt.destination == "c2") {
                check = movefoundationtoColumns(1, 1);
            }
            else if (prompt.destination == "c3") {
                check = movefoundationtoColumns(1, 2);
            }
            else if (prompt.destination == "c4") {
                check = movefoundationtoColumns(1, 3);
            }
            else if (prompt.destination == "c5") {
                check = movefoundationtoColumns(1, 4);
            }
            else if (prompt.destination == "c6") {
                check = movefoundationtoColumns(1, 5);
            }
            else if (prompt.destination == "c7") {
                check = movefoundationtoColumns(1, 6);
            }
        }
        else if (prompt.source == "f3") {

            if (prompt.destination == "c1") {
                check = movefoundationtoColumns(2, 0);
            }
            else if (prompt.destination == "c2") {
                check = movefoundationtoColumns(2, 1);
            }
            else if (prompt.destination == "c3") {
                check = movefoundationtoColumns(2, 2);
            }
            else if (prompt.destination == "c4") {
                check = movefoundationtoColumns(2, 3);
            }
            else if (prompt.destination == "c5") {
                check = movefoundationtoColumns(2, 4);
            }
            else if (prompt.destination == "c6") {
                check = movefoundationtoColumns(2, 5);
            }
            else if (prompt.destination == "c7") {
                check = movefoundationtoColumns(2, 6);
            }
        }
        else if (prompt.source == "f4") {

            if (prompt.destination == "c1") {
                check = movefoundationtoColumns(3, 0);
            }
            else if (prompt.destination == "c2") {
                check = movefoundationtoColumns(3, 1);
            }
            else if (prompt.destination == "c3") {
                check = movefoundationtoColumns(3, 2);
            }
            else if (prompt.destination == "c4") {
                check = movefoundationtoColumns(3, 3);
            }
            else if (prompt.destination == "c5") {
                check = movefoundationtoColumns(3, 4);
            }
            else if (prompt.destination == "c6") {
                check = movefoundationtoColumns(3, 5);
            }
            else if (prompt.destination == "c7") {
                check = movefoundationtoColumns(3, 6);
            }
        }
        return check;
    }

    bool movecolumntoFoundation(int indexc, int indexf) {

        card temp;
        bool fazool;

        if (tableau[indexc].isEmpty()) {
            cout << "Column is empty! No cards to move." << endl;
            return false;
        }
        card columnTopCard = tableau[indexc].gettail();

        if (foundation[indexf].isEmpty()) {
            if (columnTopCard.rank == 1) { 
                tableau[indexc].deleteend();  
                columnTopCard.faceUp = true;        
                foundation[indexf].push(columnTopCard);  
                if (!tableau[indexc].isEmpty()) {
                    tableau[indexc].tail->data.faceUp = true;
                }
                return true;
            }
            else {
                cout << "Only an Ace can be moved to an empty foundation!" << endl;
                return false;
            }
        }
        else {
            foundation[indexf].top(temp);
            if ((columnTopCard.rank == temp.rank + 1) && (columnTopCard.suit == temp.suit)) {
                tableau[indexc].deleteend();  
                columnTopCard.faceUp = true;        
                foundation[indexf].push(columnTopCard);  
                if (!tableau[indexc].isEmpty()) {
                    tableau[indexc].tail->data.faceUp = true;
                }
                return true;
            }
            else {
                cout << "Mismatch of cards! The card must be one rank higher and of the same suit to be moved to the foundation." << endl;
                return false;
            }
        }
    }

    bool columntoFoundation() {

        command prompt;
        commands->top(prompt);
        bool check = false;
        if (prompt.source == "c1") {
            if (prompt.destination == "f1") {
                check = movecolumntoFoundation(0, 0);
            }
            else if (prompt.destination == "f2") {
                check = movecolumntoFoundation(0, 1);
            }
            else if (prompt.destination == "f3") {
                check = movecolumntoFoundation(0, 2);
            }
            else if (prompt.destination == "f4") {
                check = movecolumntoFoundation(0, 3);
            }
        }
        else if (prompt.source == "c2") {

            if (prompt.destination == "f1") {
                check = movecolumntoFoundation(1, 0);
            }
            else if (prompt.destination == "f2") {
                check = movecolumntoFoundation(1, 1);
            }
            else if (prompt.destination == "f3") {
                check = movecolumntoFoundation(1, 2);
            }
            else if (prompt.destination == "f4") {
                check = movecolumntoFoundation(1, 3);
            }
        }
        else if (prompt.source == "c3") {
            if (prompt.destination == "f1") {
                check = movecolumntoFoundation(2, 0);
            }
            else if (prompt.destination == "f2") {
                check = movecolumntoFoundation(2, 1);
            }
            else if (prompt.destination == "f3") {
                check = movecolumntoFoundation(2, 2);
            }
            else if (prompt.destination == "f4") {
                check = movecolumntoFoundation(2, 3);
            }
        }
        else if (prompt.source == "c4") {
            if (prompt.destination == "f1") {
                check = movecolumntoFoundation(3, 0);
            }
            else if (prompt.destination == "f2") {
                check = movecolumntoFoundation(3, 1);
            }
            else if (prompt.destination == "f3") {
                check = movecolumntoFoundation(3, 2);
            }
            else if (prompt.destination == "f4") {
                check = movecolumntoFoundation(3, 3);
            }
        }
        else if (prompt.source == "c5") {
            if (prompt.destination == "f1") {
                check = movecolumntoFoundation(4, 0);
            }
            else if (prompt.destination == "f2") {
                check = movecolumntoFoundation(4, 1);
            }
            else if (prompt.destination == "f3") {
                check = movecolumntoFoundation(4, 2);
            }
            else if (prompt.destination == "f4") {
                check = movecolumntoFoundation(4, 3);
            }
        }
        else if (prompt.source == "c6") {
            if (prompt.destination == "f1") {
                check = movecolumntoFoundation(5, 0);
            }
            else if (prompt.destination == "f2") {
                check = movecolumntoFoundation(5, 1);
            }
            else if (prompt.destination == "f3") {
                check = movecolumntoFoundation(5, 2);
            }
            else if (prompt.destination == "f4") {
                check = movecolumntoFoundation(5, 3);
            }
        }
        else if (prompt.source == "c7") {

            if (prompt.destination == "f1") {
                check = movecolumntoFoundation(6, 0);
            }
            else if (prompt.destination == "f2") {
                check = movecolumntoFoundation(6, 1);
            }
            else if (prompt.destination == "f3") {
                check = movecolumntoFoundation(6, 2);
            }
            else if (prompt.destination == "f4") {
                check = movecolumntoFoundation(6, 3);
            }
        }
        return check;
    }

bool movecolumntoColumn(int s, int d, int numCards) {
    int sourceSize = tableau[s].getsize();  
    int startIdx = sourceSize - numCards;  

    if (startIdx < 0) {
        cout << "Invalid number of cards to move!" << endl;
        return false;
    }
    DLL<card>::iterator it = tableau[s].begin();
    for (int i = 0; i < startIdx; ++i) {
        ++it;
    }
    card sourceHeadCard = *it;  

    if (tableau[d].isEmpty()) {
        if (sourceHeadCard.rank != 13) {
            cout << "Invalid move! Only a King can be placed in an empty column." << endl;
            return false;
        }
    }
    else {
        card destTailCard = tableau[d].gettail();  

        if (destTailCard.rank != sourceHeadCard.rank + 1 ||
            !isOppositeColor(destTailCard.suit, sourceHeadCard.suit)) {
            cout << "Invalid move! The ranks or colors do not match." << endl;
            return false;  
        }
    }
    for (int i = 0; i < numCards; ++i) {
        DLL<card>::iterator nextIt = it;
        if (i < numCards - 1) {  
            ++nextIt;
        }
        tableau[d].insertatend(*it);
        tableau[s].erase(it);
        if (i < numCards - 1) {
            it = nextIt;
        }
    }
    if (!tableau[s].isEmpty()) {
        tableau[s].tail->data.faceUp = true;  
    }
    return true;
}

    bool columntoColumn() {

        command prompt;
        commands->top(prompt);
        int num = prompt.numCards;
        bool check = false;

        if (prompt.source == "c1") {
            if (prompt.destination == "c1") {
                cout << " Move cant be made from identical columns !";
                check = false;
            }
            else if (prompt.destination == "c2") {
                check = movecolumntoColumn(0, 1,num);

            }
            else if (prompt.destination == "c3") {
                check = movecolumntoColumn(0, 2, num);
            }
            else if (prompt.destination == "c4") {
                check = movecolumntoColumn(0, 3, num);
            }
            else if (prompt.destination == "c5") {
                check = movecolumntoColumn(0, 4, num);
            }
            else if (prompt.destination == "c6") {
                check = movecolumntoColumn(0, 5, num);
            }
            else if (prompt.destination == "c7") {
                check = movecolumntoColumn(0, 6, num);
            }
        }
        else if (prompt.source == "c2") {
            if (prompt.destination == "c1") {
                check = movecolumntoColumn(1, 0, num);
            }
            else if (prompt.destination == "c2") {
                cout << " Move cant be made from identical columns !";
                check = false;
            }
            else if (prompt.destination == "c3") {
                check = movecolumntoColumn(1, 2, num);
            }
            else if (prompt.destination == "c4") {
                check = movecolumntoColumn(1, 3, num);
            }
            else if (prompt.destination == "c5") {
                check = movecolumntoColumn(1, 4, num);
            }
            else if (prompt.destination == "c6") {
                check = movecolumntoColumn(1, 5, num);
            }
            else if (prompt.destination == "c7") {
                check = movecolumntoColumn(1, 6, num);
            }
        }
        else if (prompt.source == "c3") {
            if (prompt.destination == "c1") {
                check = movecolumntoColumn(2,0, num);
            }
            else if (prompt.destination == "c2") {
                check = movecolumntoColumn(2, 1, num);
            }
            else if (prompt.destination == "c3") {
                cout << " Move cant be made from identical columns !";
                check = false;
            }
            else if (prompt.destination == "c4") {
                check = movecolumntoColumn(2, 3, num);
            }
            else if (prompt.destination == "c5") {
                check = movecolumntoColumn(2, 4, num);
            }
            else if (prompt.destination == "c6") {
                check = movecolumntoColumn(2, 5, num);
            }
            else if (prompt.destination == "c7") {
                check = movecolumntoColumn(2, 6, num);
            }
        }
        else if (prompt.source == "c4") {
            if (prompt.destination == "c1") {
                check = movecolumntoColumn(3, 0, num);
            }
            else if (prompt.destination == "c2") {
                check = movecolumntoColumn(3, 1, num);
            }
            else if (prompt.destination == "c3") {
                check = movecolumntoColumn(3, 2, num);
            }
            else if (prompt.destination == "c4") {
                cout << " Move cant be made from identical columns !";
                check = false;
            }
            else if (prompt.destination == "c5") {
                check = movecolumntoColumn(3, 4, num);
            }
            else if (prompt.destination == "c6") {
                check = movecolumntoColumn(3, 5, num);
            }
            else if (prompt.destination == "c7") {
                check = movecolumntoColumn(3, 6, num);
            }
        }
        else if (prompt.source == "c5") {
            if (prompt.destination == "c1") {
                check = movecolumntoColumn(4, 0, num);
            }
            else if (prompt.destination == "c2") {
                check = movecolumntoColumn(4, 1, num);
            }
            else if (prompt.destination == "c3") {
                check = movecolumntoColumn(4, 2, num);
            }
            else if (prompt.destination == "c4") {
                check = movecolumntoColumn(4, 3, num);
            }
            else if (prompt.destination == "c5") {
                cout << " Move cant be made from identical columns !";
                check = false;
            }
            else if (prompt.destination == "c6") {
                check = movecolumntoColumn(4, 5, num);
            }
            else if (prompt.destination == "c7") {
                check = movecolumntoColumn(4, 6, num);
            }
        }
        else if (prompt.source == "c6") {
            if (prompt.destination == "c1") {
                check = movecolumntoColumn(5, 0, num);
            }
            else if (prompt.destination == "c2") {
                check = movecolumntoColumn(5, 1, num);
            }
            else if (prompt.destination == "c3") {
                check = movecolumntoColumn(5, 2, num);
            }
            else if (prompt.destination == "c4") {
                check = movecolumntoColumn(5, 3, num);
            }
            else if (prompt.destination == "c5") {
                check = movecolumntoColumn(5, 4, num);
            }
            else if (prompt.destination == "c6") {
                cout << " Move cant be made from identical columns !";
                check = false;
            }
            else if (prompt.destination == "c7") {
                check = movecolumntoColumn(5, 6, num);
            }
        }
        else if (prompt.source == "c7") {
                if (prompt.destination == "c1") {
                    check = movecolumntoColumn(6, 0, num);
                }
                else if (prompt.destination == "c2") {
                    check = movecolumntoColumn(6, 1, num);
                }
                else if (prompt.destination == "c3") {
                    check = movecolumntoColumn(6, 2, num);
                }
                else if (prompt.destination == "c4") {
                    check = movecolumntoColumn(6, 3, num);
                }
                else if (prompt.destination == "c5") {
                    check = movecolumntoColumn(6, 4, num);
                }
                else if (prompt.destination == "c6") {
                    check = movecolumntoColumn(6, 5, num);
                }
                else if (prompt.destination == "c7") {
                    cout << " Move cant be made from identical columns !";
                    check = false;
                }
        }
                return check;
    }

    bool performCommand() {

        command prompt;
        bool fazool= commands->top(prompt);
        bool check;

        //for waste to foundation
        if (prompt.source == "w" && (prompt.destination == "f1" || prompt.destination == "f2" || prompt.destination == "f3" || prompt.destination == "f4")) {
            check = wastetoFoundation();
            if (!check) {
                return false;
            }
            else {
                return true;
            }
        }
        //for waste to columnns
        else if (prompt.source == "w" && (prompt.destination == "c1" || prompt.destination == "c2" || prompt.destination == "c3" || prompt.destination == "c4" || prompt.destination == "c5" || prompt.destination == "c6" || prompt.destination == "c7")) {
            check = wasteToColumns();
            if (!check) {
                return false;
            }
            else {
                return true;
            }
        }
        //for foundations to cols
        else if ((prompt.source == "f1" || prompt.source == "f2" || prompt.source == "f3" || prompt.source == "f4") && (prompt.destination == "c1" || prompt.destination == "c2" || prompt.destination == "c3" || prompt.destination == "c4" || prompt.destination == "c5" || prompt.destination == "c6" || prompt.destination == "c7")) {
            check = foundationtoColumns();
            if (!check) {
                return false;
            }
            else {
                return true;
            }
        }
        //for cols to foundation
        else if ((prompt.destination== "f1" || prompt.destination == "f2" || prompt.destination == "f3" || prompt.destination == "f4") && (prompt.source == "c1" || prompt.source == "c2" || prompt.source == "c3" || prompt.source == "c4" || prompt.source == "c5" || prompt.source == "c6" || prompt.source == "c7")) {
            check = columntoFoundation();
            if (!check) {
                return false;
            }
            else {
                return true;
            }
        }
        //for cols to cols
        else if ((prompt.source == "c1" || prompt.source == "c2" || prompt.source == "c3" || prompt.source == "c4" || prompt.source == "c5" || prompt.source == "c6" || prompt.source == "c7") && (prompt.destination == "c1" || prompt.destination == "c2" || prompt.destination == "c3" || prompt.destination == "c4" || prompt.destination == "c5" || prompt.destination == "c6" || prompt.destination == "c7")) {
            check = columntoColumn();
            if (!check) {
                return false;
            }
            else {
                return true;
            }
        }
    }

    bool gameWon() {
        bool check = true;
        for (int i = 0; i < 4; i++) {
            if (foundation[i].getsize() != 13) {
                check = false;
                return check;
            }
        }
        return check;
    }
};

//////////////////////////////////MAIN FUNCTION///////////////////////////////////////////
int main() {
playagain:
    cout << "- - - - - - - - - - - - - - - - - - - - - - -  WELCOME TO SOLITAIRE - - - - - - - - - - - - - - - - - - - - - - -" << endl<<endl<<endl<<"- - - - - - - - - - - - - - - - - - - - - - - - - - GAME RULES - - - - - - - - - - - - - - - - - - - - - - - - - -"<<endl;
    cout << "1) For stock: use cmd 's'" << endl << "2) For moving cards: use cmd 'm (source) (destination) (number of Cards)'" << endl << "3) For undo: use 'z'" << endl << "4) To quit the game use 'quit' or 'end'" << endl<<"5) To win the game all cards must be in foundations"<<endl<<endl<<" BEST OF LUCK !!!"<<endl;
    cout << "- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -" << endl << endl;
    game solitaire;
    solitaire.initializeGame();
    solitaire.gameDisplay();

    while (true) {
        cout << "- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -  \n\n";
        start:
        string command;
        cout << "Enter a command:";
        getline(cin, command);
        cout << endl;
        cout << "- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -  \n\n";

        if (command == "z") {
            solitaire.undo();
        }
        else if (command == "s") {
           bool check=solitaire.drawCard(command);
           if (!check) {
               solitaire.refreshStock();
           }
        }
        else if (command == "quit" || command == "end") {

            cout << "- - - - - - - - - - - - - - - - - - - - USER HAS ENDED THE GAME - - - - - - - - - - - - - - - - - - - -" << endl;

            jump:
            string ans;
            cout << "DO YOU WANT TO PLAY AGAIN ? (Y/N)"<<endl;
            getline(cin, ans);
            if (ans == "Y" || ans == "y") {
                    system("cls");
                    goto playagain;
            }
            else if(ans=="N" || ans=="n") {
                    return 0;
            }
            else {
                cout << "Invalid answer !!!" << endl;
                goto jump;
            }
        }
        else {
            if (!solitaire.validCommand(command)) {
                goto start; 
            }
            else {
                bool ans=solitaire.performCommand();
                if (!ans) {
                    goto start;
                }
            }
        }

        solitaire.gameDisplay();

        if (solitaire.gameWon()) {
            cout << "- - - - - - - - - - - - - - - - - - - - THE GAME HAS BEEN WON...!!! - - - - - - - - - - - - - - - - - - - - " << endl;
            goto jump;
        }
    }
    system("pause");
    return 0;
}