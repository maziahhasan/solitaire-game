
Solitaire Game (Console Edition) - C++
Welcome to the Console-Based Solitaire Game, implemented entirely in C++ using the data structures like linked lists and stacks. This is a text-based version of the classic Solitaire card game, where you interact with the game using simple console commands. The objective is to move all cards to the foundation piles, following the classic rules of Solitaire.

Features
Text-Based Interface: The game runs in a console window with no graphics, making it lightweight and easy to run.
Card Representation: Cards are displayed with their values and suits, with face-up cards showing their full details and face-down cards represented as [ ].
Tableau Setup: The game starts with seven tableau columns, each containing a mixture of face-up and face-down cards.
Movement Commands: Players can move cards between tableau columns, the waste pile, and foundation piles using specific commands.
Undo Feature: Allows players to undo their last move, enabling more flexible gameplay.
Classic Rules: The goal is to move all the cards to the foundation piles in ascending order by suit.
Gameplay
The game begins with a shuffled deck of 52 cards, and you will need to move them across the tableau columns, waste pile, and foundation piles.

Basic Commands:
Move Cards:
m <source>, <destination>, <number of cards>
Example: m c6, c1, 2 will move two cards from column 6 to column 1.

Undo Last Move:
Use u to undo the last move made.

Quit the Game:
Use q to quit the game at any time.

Winning the Game:
The game is won once all the cards are moved to the foundation piles in ascending order, starting from Ace to King, for each suit.

Installation
Clone the repository:

bash
Copy code
git clone https://github.com/maziahhasan/solitaire-game.git
Navigate to the project folder:

bash
Copy code
cd solitaire-game
Compile the code: Use a C++ compiler (e.g., g++) to compile the program.

bash
Copy code
g++ solitaire.cpp -o solitaire
Run the game: After compilation, run the executable to start the game.

bash
Copy code
./solitaire
Contributing
Feel free to fork this repository, submit issues, or create pull requests if you'd like to contribute to improving the game!

License
This project is open source and available under the MIT License.

