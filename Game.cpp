//
//  Game.cpp
//  Chess
//
//  Created by Petr Janousek on 14/05/2019.
//  Copyright © 2019 Petr Janousek. All rights reserved.
//

#include "Game.hpp"

#include <unistd.h>
#include <stdlib.h>
#include <cstdio>
#include <sys/ioctl.h>
#include <termios.h>



void Game :: RunGame()
{
    Position from(0,0), to(0,0);
    print();
    while ( !board.Get_CheckMate() )
    {
        bool tmp = false;
        while (!tmp)
        {
            if ( !InputMove(from, to) || !board.Move(from, to) )
            {
                board.Set_LastMove("Wrong Move");
                print();
                continue;
            }
            tmp = true;
        }
        board.ChangeTurn();
        print();
    }
    system("clear");
    print();
    if (board.Get_Turn() == Color::black) std::cout << "White Wins!" << std::endl;
    else                                  std::cout << "Black Wins!" << std::endl;
}

// INPUT PLAYER MOVES

bool Game :: InputMove(Position & from, Position & to)
{
    Color TurnColor = board.Get_Turn();
    
    std::string firstMove;
    std::string secondMove;
    
    if ( TurnColor == Color::white )
    {
        std::cout << "White Player move: ";
        players.first -> Input(firstMove, secondMove, board);
    }
    else
    {
        std::cout << "Black player move: ";
        players.second -> Input(firstMove, secondMove, board);
    }
    
    if (firstMove == "q")
    {
        SaveGame();
        exit(0);
    }
    
    Position firstPos = StringToPos(firstMove);
    Position secondPos = StringToPos(secondMove);
    
    if ( firstPos == Position(-1,-1) || secondPos == Position(-1,-1) ) return false;
    
    from = firstPos;
    to = secondPos;
    return true;
}


// PRINT OUT THE BOARD

void Game:: printLine(bool Print_EliminatedPiecesWhite, bool Print_LastMove)
{
    std::cout << "  ";
    for (int i = 0; i < 49 ; i++)
        std::cout << "#";
    
    if ( Print_EliminatedPiecesWhite )
    {
        std::cout << "\t" << "Eliminated White: ";
        for (const auto & piece : board.Get_EliminatedPieces(Color::white) )
            piece->print();
    }
    
    if ( Print_LastMove )
    {
        std::cout << "\t" << "Last Move: " << board.Get_LastMove();
    }
    std::cout << std::endl;
}

void Game::printSemiLine( bool Print_EliminatedPiecesBlack, int lineNumber, bool PieceLine, bool Print_Turn, bool Print_Check)
{
    int columnNumber = 0;
    int piece = 3;
    if (PieceLine)
        std::cout << lineNumber + 1 << " ";
    else
        std:: cout << "  ";
    
    for (int i = 0 ; i <= 49 ; i++ )
    {
        if (PieceLine)
        {
            if ( i == piece)
            {
                if ( board.PieceAt(Position(lineNumber, columnNumber)) != nullptr )
                    board.PieceAt(Position(lineNumber, columnNumber))->print();
                else
                    std::cout << " ";
                
                piece += 6;
                columnNumber++;
                continue;
            }
        }
        if (i % 6 == 0)
        {
            std::cout << "#";
            continue;
        }
        std::cout << " ";
    }
    
    if ( Print_EliminatedPiecesBlack )
    {
        std::cout << "\t" << "Eliminated Black: ";
        for ( const auto & piece : board.Get_EliminatedPieces(Color::black) )
            piece->print();
    }
    
    if ( Print_Turn )
    {
        std::cout << "\t" << "Turn: ";
        if (board.Get_Turn() == Color::white) std::cout << "White";
        else                                  std::cout << "Black";
    }
    
    if ( Print_Check )
    {
        std::cout << "\t" << "Check: ";
        if ( board.Get_Check() ) std::cout << "True";
        else                     std::cout << "False";
    }
    
    std::cout << std::endl;
}

void Game :: print()
{
    const int boardSize = 8;
    
    system("clear");
    
    for (int i = 0; i < boardSize ; i++)
    {
        if (i == 0)
        {
            printLine(true);
            printSemiLine(true);
            printSemiLine(false, i, true, true);
            printSemiLine(false, i, false, false, true);
        }
        else
        {
            if (i == 1)
                printLine(false,true);
            else
                printLine();
            
            printSemiLine();
            printSemiLine(false, i, true);
            printSemiLine();
        }
        if ( i == boardSize - 1 )
        {
            printLine();
            int letter = 3;
            int counter = 0;
            char letters[8] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H'};
            
            std::cout << "  ";
            for (int i = 0; i < 49 ; i++)
            {
                if (i == letter)
                {
                    std::cout << letters[counter];
                    counter++;
                    letter += 6;
                }
                else
                    std::cout << " ";
            }
            std::cout << std::endl;
        }
    }
}



int GetChar()
{
    int ch;
    struct termios oldt;
    struct termios newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    
    return ch;
}

// OUTPUT
bool Game :: SaveGame()
{
    char choice;
    while(true)
    {
        print();
        std::cout << "Do you want to save the game?(y/n): ";
        std::cin >> choice;
        if ( choice == 'y' ) break;
        if ( choice == 'n' ) return false;
    }
    
    std::ofstream file;
    
    std::string filename;
    std::cout << "Filename(savedgame.txt): ";
    std::cin >> filename;
    
    file.open(filename);
    
    SavePiecesOnBoard(file);
    file << "\n";
    SaveEliminatedPieces(file);
    file << "\n";
    SaveTurnsPlayers(file);
    file << "\n";
    file.close();
    
    return true;
}

bool Game :: SavePiecesOnBoard(std::ofstream & ss)
{
    for (int i = 0; i < board.Get_boardHeight() ; i ++)
    {
        for ( int k = 0 ; k < board.Get_boardWidth() ; k++)
        {
            if ( board.PieceAt(Position(i,k)) != nullptr )
            {
                    ss << PieceToString(i, k) << ",";
            }
        }
    }
    
    return true;
}

bool Game :: SaveEliminatedPieces(std::ofstream & ss)
{
    for ( const auto & piece : board.Get_EliminatedPieces(Color::white) )
    {
        ss << EliminatedPieceToString(piece);
        ss << ",";
    }
    for ( const auto & piece : board.Get_EliminatedPieces(Color::black) )
    {
        ss << EliminatedPieceToString(piece);
        ss << ",";
    }
    return true;
}

bool Game :: SaveTurnsPlayers(std::ofstream & ss)
{
    if (board.Get_Turn() == Color::white) ss << "w";
    else                                  ss << "b";
    
    ss << ",";
    if (players.first -> Get_Difficulty() == 0)
        ss << "H0";
    else if ( players.first -> Get_Difficulty() == 1 )
        ss << "C1";
    else
        ss << "C2";
    
    ss << ",";
    if (players.second -> Get_Difficulty() == 0)
        ss << "H0";
    else if ( players.second -> Get_Difficulty() == 1 )
        ss << "C1";
    else
        ss << "C2";
    
    ss << ",";
    if ( board.Get_Check() )
        ss << "T";
    else
        ss << "F";
    
    return true;
}
// ---------


// INPUT
bool Game :: LoadGame()
{
    std::ifstream is;
    while(true)
    {
        std::cout << "Enter the name of the saved game(savedgame.txt): ";
        
        std::string gameToBeLoaded = "savedGame.txt";
        std::cin >> gameToBeLoaded;
        
        is.open(gameToBeLoaded, std::ifstream::in);
        if ( ! is.fail() ) break;
    }
    
    if ( !LoadPiecesOnBoard(is) ) return false;
    if ( !LoadEliminatedPieces(is) ) return false;
    if ( !LoadTurnsPlayers(is) ) return false;
    
    
    return true;
}
bool Game :: LoadPiecesOnBoard(std::ifstream & is)
{
    Color color;
    bool moved = false;
    Position pos(0,0);
    
    std::string piecesOnBoard;
    std::string singlePiece;
    
    getline(is, piecesOnBoard, '\n');
    
    std::stringstream ss;
    ss << piecesOnBoard;
    
    while( getline(ss, singlePiece, ',') )
    {
        
        if      ( singlePiece[0] == 'w' ) color = Color::white;
        else if ( singlePiece[0] == 'b' ) color = Color::black;
        
        
        if ( singlePiece[2] == '1' ) moved = true;
        else if ( singlePiece[2] == '0' ) moved = false;
        
        pos.height = singlePiece[3] - '0';
        pos.width = singlePiece[4] - '0';
        
        // king, queen, rook, bishop, horse, pawn
        if      ( singlePiece[1] == '0' ) board.Set_PieceTo( std::shared_ptr<Piece>(King(color, PieceType::king, moved).clone() ), Position(pos.height, pos.width));
        else if ( singlePiece[1] == '1' ) board.Set_PieceTo( std::shared_ptr<Piece>(Queen(color, PieceType::queen, moved).clone() ), Position(pos.height, pos.width));
        else if ( singlePiece[1] == '2' ) board.Set_PieceTo( std::shared_ptr<Piece>(Rook(color, PieceType::rook, moved).clone() ), Position(pos.height, pos.width));
        else if ( singlePiece[1] == '3' ) board.Set_PieceTo( std::shared_ptr<Piece>(Bishop(color, PieceType::bishop, moved).clone() ), Position(pos.height, pos.width));
        else if ( singlePiece[1] == '4' ) board.Set_PieceTo( std::shared_ptr<Piece>(Horse(color, PieceType::horse, moved).clone() ), Position(pos.height, pos.width));
        else if ( singlePiece[1] == '5' ) board.Set_PieceTo( std::shared_ptr<Piece>(Pawn(color, PieceType::pawn, moved).clone() ), Position(pos.height, pos.width));
    }
    return true;
}

bool Game :: LoadEliminatedPieces ( std::ifstream & is )
{
    std::stringstream ss;
    std::string eliminatedPieces;
    
    std::string singlePiece;
    
    getline(is, eliminatedPieces, '\n');
    ss << eliminatedPieces;
    
    std::vector<std::shared_ptr<Piece>> a = board.Get_EliminatedPieces(Color::black);
    
    Color color;
    
    while( getline(ss, singlePiece, ',') )
    {
        if ( singlePiece[0] == 'w') color = Color::white;
        if ( singlePiece[0] == 'b') color = Color::black;
        else return false;
        
        if      ( singlePiece[1] == '0' ) board.Append_EliminatedPieces(color, std::shared_ptr<Piece>(King(color).clone()));
        else if ( singlePiece[1] == '1' ) board.Append_EliminatedPieces(color, std::shared_ptr<Piece>(Queen(color).clone()));
        else if ( singlePiece[1] == '2' ) board.Append_EliminatedPieces(color, std::shared_ptr<Piece>(Rook(color).clone()));
        else if ( singlePiece[1] == '3' ) board.Append_EliminatedPieces(color, std::shared_ptr<Piece>(Bishop(color).clone()));
        else if ( singlePiece[1] == '4' ) board.Append_EliminatedPieces(color, std::shared_ptr<Piece>(Horse(color).clone()));
        else if ( singlePiece[1] == '5' ) board.Append_EliminatedPieces(color, std::shared_ptr<Piece>(Pawn(color).clone()));
        else return false;
    }
    
    return true;
}

bool Game :: LoadTurnsPlayers( std::ifstream & is )
{
    std::stringstream ss;
    std::string turnsPlayers;
    
    std::string single;
    
    getline(is, turnsPlayers, '\n');
    ss << turnsPlayers;
    
    getline(ss, single, ',');
    if (single == "w") board.Set_Turn(Color::white);
    else               board.Set_Turn(Color::black);
    
    getline(ss, single, ',');
    if ( single[0] == 'H' )   players.first = Human().clone();
    else
    {
        if (single[1] == '1') players.first = Computer().clone();
        else                  players.first = Computer(2).clone();
    }
    
    getline(ss, single, ',');
    if ( single[0] == 'H' )   players.second = Human().clone();
    else
    {
        if (single[1] == '1') players.second = Computer().clone();
        else                  players.second = Computer(2).clone();
    }
    
    return true;
}
// ---------

// HELPER FUNCTIONS


std::string Game :: PieceToString(int height, int width)
{
    std::stringstream StringPiece;
    
    Color color = board.PieceAt(Position(height, width)) -> color;
    PieceType type = board.PieceAt(Position(height, width)) -> pieceType;
    bool moved = board.PieceAt(Position(height, width)) -> alreadyMoved;
    
    if (color == Color::white) StringPiece << "w";
    else                       StringPiece << "b";
    
    // king, queen, rook, bishop, horse, pawn
    if      (type == PieceType::king)   StringPiece << "0";
    else if (type == PieceType::queen)  StringPiece << "1";
    else if (type == PieceType::rook)   StringPiece << "2";
    else if (type == PieceType::bishop) StringPiece << "3";
    else if (type == PieceType::horse)  StringPiece << "4";
    else if (type == PieceType::pawn)   StringPiece << "5";
    
    if (moved) StringPiece << "1";
    else       StringPiece << "0";
    
    StringPiece << height << width;
    
    return StringPiece.str();
}

Position Game :: StringToPos(std::string & SMove)
{
    Position pos(-1,-1);
    char letters[8] = {'A','B','C','D','E','F','G','H'};
    
    int height = (SMove[1] - '0') - 1;
    
    if (height > 7 || height < 0)
        return pos;
    
    for (int i = 0 ; i < 8 ; i++)
    {
        if (SMove[0] == letters[i])
        {
            pos.width = i;
            pos.height = height;
            return pos;
        }
    }
    
    return pos;
}

std::string Game :: EliminatedPieceToString( const std::shared_ptr<Piece> & piece )
{
    std::stringstream StringPiece;
    
    Color color = piece -> color;
    PieceType type = piece -> pieceType;
    
    if (color == Color::white) StringPiece << "w";
    else                       StringPiece << "b";
    
    // king, queen, rook, bishop, horse, pawn
    if      (type == PieceType::king)   StringPiece << "0";
    else if (type == PieceType::queen)  StringPiece << "1";
    else if (type == PieceType::rook)   StringPiece << "2";
    else if (type == PieceType::bishop) StringPiece << "3";
    else if (type == PieceType::horse)  StringPiece << "4";
    else if (type == PieceType::pawn)   StringPiece << "5";
    
    return StringPiece.str();
}

int Game :: MainMenu()
{
    std::vector<std::string> MenuPieces = {"NEW GAME", "LOAD GAME", "EXIT"};
    int choice = ChooseFrom(MenuPieces);
    
    if      (choice == 0) NewGame();
    else if (choice == 1) LoadGame();
    else                  return 0;
    
    RunGame();
    
    return 1;
}

int Game :: ChooseFrom(const std::vector<std::string> options)
{
    int down_arrow = 115;
    int up_arrow = 119;
    int enter = 10;
    
    int counter = 0;
    
    while (true)
    {
        system("clear");
        
        for (int i = 0 ; i < (int)options.size() ; i ++)
        {
            if ( counter == i )
                std::cout << "-> ";
            else
                std::cout << "   ";
            
            std::cout << options[i] << std::endl;
        }
        
        int keyPressed = GetChar();
        
        if (keyPressed == down_arrow)
        {
            if (counter == (int)options.size() - 1 )
            {
                counter = 0;
                continue;
            }
            counter += 1;
        }
        else if (keyPressed == up_arrow)
        {
            if (counter == 0)
            {
                counter = (int)options.size() - 1;
                continue;
            }
            counter -= 1;
        }
        else if (keyPressed == enter)
        {
            return counter;
        }
    }
}

void Game :: NewGame()
{
    std::vector<std::string> MenuPieces = {"HUMAN VS HUMAN", "HUMAN VS AI"};
    
    int choice = ChooseFrom(MenuPieces);
    
    if ( choice == 0 )
    {
        players.first = Human().clone();
        players.second = Human().clone();
    }
    else
    {
        players.first = Human().clone();
        
        std::vector<std::string> MenuPiecesDifficulty = { "EASY", "HARD" };
        int choiceDifficulty = ChooseFrom(MenuPiecesDifficulty);
        
        if ( choiceDifficulty == 0 ) players.second = Computer().clone();
        else                         players.second = Computer(2).clone();
    }
    
    board.Init();
}
