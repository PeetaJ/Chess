//
//  Game.hpp
//  Chess
//
//  Created by Petr Janousek on 14/05/2019.
//  Copyright © 2019 Petr Janousek. All rights reserved.
//

#ifndef Game_hpp
#define Game_hpp

#include <iomanip>

#include "Board.hpp"
#include "Piece_utility.hpp"
#include "Player.hpp"
#include "Human.hpp"
#include "Computer.hpp"

#include <iostream>
#include <memory>
#include <utility>
#include <algorithm>
#include <fstream>
#include <string>
#include <sstream>

/** class that configures the board before the start to the players likings and runs the main loop of the game.
 * It contains the menu and the choices
 * The board and the 'game' runs in this class.
 */
class Game
{
private:
    std::pair <std::shared_ptr<Player>, std::shared_ptr<Player> > players;
public:
    Board board;
    
    /** Checks if the player want to start a new game, load game or exit.
     * after all the choices are made, the game is run from here.
     */
    int MainMenu();
    
    /** Configures if the player want to play against other human or against computer and start a new game. */
    void NewGame();
    
    /** The main game loop. it runs the game until checkmate or saved game. */
    void RunGame();
    
    /** Gets the move from player.
     @param[out] from The from Position of the move.
     @param[out] to The to Position of the move.
     @return true if the move is legal.
     */
    bool InputMove(Position & from, Position & to);
    
    
    // PRINT BOARD
    /** Prints the full line of '#', if one of the params are true, it also prints one of the informations about the game.
     @param[in] Print_EliminatedPiecesWhite = false If true, it prints the eliminated white pieces on the side.
     @param[in] Print_LastMove = false if true, it prints the last move on the side.
     */
    void printLine(bool Print_EliminatedPiecesWhite = false, bool Print_LastMove = false);
    
    /** Prints semi line and leaves spaces for piece or for the squares.
     @param[in] Print_EliminatedPiecesBlack = false If true, it prints the eliminated black pieces on the side.
     @param[in] lineNumber = 0 The current line it called on.
     @param[in] PieceLine = false If true, it prints the pieces to the spaces.
     @param[in] Print_Turn = false if true, it prints whose turn it is on the side.
     @param[in] Print_Check = flase if true, it prints if the current player is in check.
     */
    void printSemiLine(bool Print_EliminatedPiecesBlack = false, int lineNumber = 0, bool PieceLine = false, bool Print_Turn = false, bool Print_Check = false);
    
    /** Print the whole board with all the information and pieces inside */
    void print();
    
    
    // SAVE LOAD
    //  * SAVE
    /** Called if the player writes 'q' in the middle of the game.
     * Player will write the name of the file the game should be saved to and it saves all the current information and positions of the game.
     */
    bool SaveGame();
    
    bool SaveEliminatedPieces(std::ofstream & ss);
    bool SaveTurnsPlayers(std::ofstream & ss);
    bool SavePiecesOnBoard(std::ofstream & ss);
    
    //  * LOAD
    /** Loads the game from the file */
    bool LoadGame();
    
    bool LoadPiecesOnBoard( std::ifstream & is);
    bool LoadEliminatedPieces ( std::ifstream & is );
    bool LoadTurnsPlayers ( std::ifstream & is);
    
    //  * HELPER IO FUNCTIONS
    /** Converts the piece to string that can be saved in compact way to the file.
     @param[in] height The height position of the piece on board.
     @param[in] width The width position of the piece on board.
     @return string of the converted piece.
     */
    std::string PieceToString(int height, int width);
    
    /** Converts the string to piece.
     @param[in] SMove The string that is to be converted.
     @return Position that was gotten from the string.
     */
    Position StringToPos(std::string & SMove);
    std::string EliminatedPieceToString( const std::shared_ptr<Piece> & piece);
    
    /** Helper function for the choices in the menu. It reacts on the usage of keyboard without using enter. */
    int ChooseFrom(const std::vector<std::string> );
    
    // -----------------
};

#endif /* Game_hpp */
