# Chess
This is chess game. It is played through terminal and uses the official (classic) rules.
You can play either against another human being or against AI. The AI has two difficulties, one with random moves only (easy)
and the second one using Minimax algorithm (hard).
Current game can be saved and then loaded again from file.

## What I learned
* Polymorphism and inheritance
* Minimax algorithm
* Creating and reading my own format to save and load game from files

## How to play
- Download this repo, go into the folder and type `make all`, makefile will link all the dependencies and run the game.
* You can move through the menu by using 'W' and 'S' keys to move up and down and enter to confirm your choice.
* Each turn is played by writting the position of the piece you want to move followed by position of where to move the piece.
( Rook from A1 to A7 --> A1 A7 )
* If the move is wrong for whatever reason ( piece cant be moved that way, position is out of board, check would happen )
the game promptly tells you and you need to put in another move.
* To save the game in the middle you can type in 'q' instead of the move. That will prompt you to choose if you want to save
the game.
