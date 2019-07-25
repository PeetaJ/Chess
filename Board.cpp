//
//  Board.cpp
//  Chess
//
//  Created by Petr Janousek on 13/04/2019.
//  Copyright © 2019 Petr Janousek. All rights reserved.
//
#include <iostream>
#include <sys/ioctl.h>
#include <unistd.h>

#include "Board.hpp"


void Board :: Prirad(const Piece &piece, int row, int col)
{
    board[row][col] = piece.clone();
}

// GETTERS
int Board :: Get_boardHeight()
{
    return board_height;
}

int Board :: Get_boardWidth()
{
    return board_width;
}

std::vector< std::shared_ptr<Piece> > & Board :: Get_EliminatedPieces(const Color & color)
{
    if (color == Color :: white) return EliminatedPiecesWhite;
    else                         return EliminatedPiecesBlack;
}

Color Board :: Get_Turn()
{
    return turn;
}

bool Board :: Get_Check()
{
    return check;
}

std::string Board :: Get_LastMove()
{
    return lastMove;
}

//std::pair< std::shared_ptr<Player>, std::shared_ptr<Player> > Board :: Get_Players()
//{
//    return players;
//}

bool Board :: Get_CheckMate()
{
    return checkMate;
}
// ------------------------------------------------

// SETTERS
void Board :: Append_EliminatedPieces(const Color & color, const std::shared_ptr<Piece> & piece)
{
    if (color == Color::white) EliminatedPiecesWhite.push_back(piece);
    else                       EliminatedPiecesBlack.push_back(piece);
}

void Board :: Set_Turn(const Color & color)
{
    turn = color;
}

void Board :: Set_LastMove(const std::string & move)
{
    lastMove = move;
}

void Board :: ChangeTurn()
{
    if (turn == Color::white) turn = Color::black;
    else                      turn = Color::white;
}
// ------------------------------------------------

// INPUT

void Board :: InputMove()
{
    Position from(0,0);
    Position to(0,0);
    std::cin >> from.height >> from.width >> to.height >> to.width;
    
    if ( from.height < 0 || from.width < 0 || to.height < 0 || to.width < 0 ) return;
    Move(from, to);
}


bool Board :: Move(const Position & from, const Position & to)
{
    // Checks if 'from' square is empty
    if (PieceAt(from) == nullptr) return false;
    
    // Checks if the color of piece at 'from' and turn are equal
    if ( ! Check_TurnPieceColor(PieceAt(from)) ) return false;
    
    // All the moves that can be made with 'from' piece at the moment
    std::vector<Position> Piece_LegalMoves = Piece_GenerateLegalMoves(from);
    
    // Castling
    if (PieceAt(from)->pieceType == PieceType::king &&
        CheckCastling(from, to) )
    {
        lastMove = MoveToString(from, to);
        return true;
    }
    
    // Checks if the 'to' position is in legalMoves of the piece
    if (std::count(Piece_LegalMoves.begin(), Piece_LegalMoves.end(), to) == 0 ) return false;
    
    // Adds eliminated piece to Eliminated vector
    if (PieceAt(to) != nullptr && PieceAt(from)->color != PieceAt(to)->color)
    {
        if ( PieceAt(to)->color == Color::white ) EliminatedPiecesWhite.push_back( PieceAt(to) );
        if ( PieceAt(to)->color == Color::black ) EliminatedPiecesBlack.push_back( PieceAt(to) );
    }
    
    // Check if move would lead to check of playing player
    if (TryIfCheck(from, to) ) return false;
    
    // Actual move (by copying the shared_ptr and then deleting it) from 'from' position to 'to' position
    Set_PieceTo( PieceAt(from), to);
    PieceAt(to) -> Set_AlreadyMoved();
    
    RemoveAt(from);
    
    bool prom = CheckPromotion(to);
    
    lastMove = MoveToString(from, to, prom);
    
    // Checks if king is in check
    Color colorNextTurn;
    if (PieceAt(to)->color == Color::white) colorNextTurn = Color::black;
    else                                    colorNextTurn = Color::white;
    
    if ( isKingAttacked(colorNextTurn) )   check = true;
    else                                   check = false;
    if ( isCheckMate(colorNextTurn) )      checkMate = true; // TODO MAYBE ADD isCheckMate to isKingAttacked and change if isKingAttacked then return false
    return true;
}


std::shared_ptr<Piece> & Board:: PieceAt(const Position & at)
{
    return board[at.height][at.width];
}

void Board :: Set_PieceTo ( const std::shared_ptr<Piece> piece, const Position & pos)
{
    board[pos.height][pos.width] = piece;
}

void Board :: RemoveAt(const Position & square)
{
    board[square.height][square.width] = nullptr;
}

bool Board :: Check_TurnPieceColor(const std::shared_ptr<Piece> & piece)
{
    if ( piece->color == turn ) return true;
    return false;
}

Position Board :: FindPiece(const PieceType & type, const Color & color)
{
    for (int i = 0 ; i < board_height ; i ++)
    {
        for (int k = 0 ; k < board_width ; k++)
        {
            if ( board[i][k] != nullptr &&
                 board[i][k] -> pieceType == type &&
                 board[i][k] -> color == color )
                return Position(i,k);
        }
    }
    return Position(-1,-1);
}

std::vector<Position> Board :: findAllPiecesOf(const Color & color)
{
    // 16 is the maximum number of pieces that player can have
    std::vector<Position> pieces;
    for ( int i = 0 ; i < board_height ; i++)
    {
        for ( int k = 0; k < board_width ; k++ )
        {
            if ( PieceAt(Position(i,k)) != nullptr &&
                 PieceAt(Position(i,k))->color == color )
                pieces.emplace_back(i,k);
        }
    }
    return pieces;
}

bool Board :: TryIfCheck(const Position & from, const Position & to)
{
    std::shared_ptr<Piece> tmp;
    if (PieceAt(to) != nullptr)
        tmp = PieceAt(to);
    
    Set_PieceTo(PieceAt(from), to);
    RemoveAt(from);
    if ( isKingAttacked(PieceAt(to)->color) )
    {
        Set_PieceTo(PieceAt(to), from);
        RemoveAt(to);
        Set_PieceTo(tmp, to);
        return true;
    }
    else
    {
        Set_PieceTo(PieceAt(to), from);
        RemoveAt(to);
        Set_PieceTo(tmp, to);
        return false;
    }
}

bool Board :: isKingAttacked(const Color & color)
{
    Position KingPos = FindPiece(PieceType::king, color);
    
    std::vector<Position> attackedPositions;
    if (color == Color::black) attackedPositions = GenerateAttackedSquares(Color::white);
    if (color == Color::white) attackedPositions = GenerateAttackedSquares(Color::black);
    
    for (Position & attackedPos : attackedPositions)
    {
        if (KingPos == attackedPos) return true;
    }
    return false;
}

bool Board :: isEmptyBetween(const Position & first, const Position & second)
{
    Position tmp = first;
    
    while ( ! (tmp == second) )
    {
        if (tmp.height > second.height) tmp.height = tmp.height - 1;
        if (tmp.height < second.height) tmp.height = tmp.height + 1;
        
        if (tmp.width > second.width) tmp.width = tmp.width - 1;
        if (tmp.width < second.width) tmp.width = tmp.width + 1;
        
        if ( PieceAt(tmp) != nullptr && !(PieceAt(tmp) == PieceAt(second) ) ) return false;
    }
    
    return true;
}

std::vector<Position> Board :: Piece_GenerateLegalMoves(const Position & from)
{
    std::vector<Position> possibleMoves = PieceAt(from) -> MoveSet();
    std::vector<Position> LegalMoves;
    
    // Generating legal moves only for pawn
    if ( PieceAt(from)->pieceType == PieceType::pawn )
    {
        for (auto & move : possibleMoves)
        {
            Position tmp = from + move;
            if ( PieceAt(tmp) == nullptr &&
                 !(tmp.height >= board_height) &&
                 !(tmp.width >= board_width) &&
                 !(tmp.height < 0) &&
                 !(tmp.width < 0) &&
                 isEmptyBetween(from, tmp) ) LegalMoves.push_back(tmp);
        }
        
        if ( PieceAt(from)->color == Color::white )
        {
            Position tmp1 = from + Position(-1,-1);
            Position tmp2 = from + Position(-1, 1);
            
            if ( PieceAt( tmp1 ) != nullptr &&
                !(tmp1.height < 0) &&
                !(tmp1.width >= board_width) &&
                !(tmp1.width < 0) &&
                PieceAt(tmp1) -> color != Color::white) LegalMoves.push_back(tmp1);
            
            if ( PieceAt( tmp2 ) != nullptr &&
                !(tmp2.height < 0) &&
                !(tmp2.width >= board_width) &&
                !(tmp2.width < 0) &&
                PieceAt(tmp2) -> color != Color::white)  LegalMoves.push_back(tmp2);
        }
        else
        {
            Position tmp1 = from + Position(1,1);
            Position tmp2 = from + Position(1, -1);
            
            if ( PieceAt( tmp1 ) != nullptr &&
                !(tmp1.height >= board_height) &&
                !(tmp1.width < 0) &&
                !(tmp1.width >= board_width) &&
                PieceAt(tmp1) -> color != Color::black)  LegalMoves.push_back(tmp1);
            
            if ( PieceAt( tmp2 ) != nullptr &&
                !(tmp2.height >= board_height) &&
                !(tmp2.width < 0) &&
                !(tmp2.width >= board_width) &&
                PieceAt(tmp2) -> color != Color::black)  LegalMoves.push_back(tmp2);
        }
        return LegalMoves;
    }
    
    // Generating legal moves for most of the pieces
    for (auto & move : possibleMoves)
    {
        Position tmp = from + move;
        
        if ( tmp.height < 0 || tmp.height >= board_height || tmp.width < 0 || tmp.width >= board_width) continue;
        if (! isEmptyBetween(from, tmp) && PieceAt(from)->pieceType != PieceType::horse) continue;
        if ( PieceAt(tmp) != nullptr && PieceAt(tmp)->color == PieceAt(from)-> color ) continue;
        LegalMoves.push_back(tmp);
    }
    return LegalMoves;
}

std::vector<Position> Board :: Piece_GeneratePosAttackedBy(const Position & from)
{
    std::vector<Position> possibleMoves = PieceAt(from) -> MoveSet();
    std::vector<Position> AttackedPos;
    
    if (PieceAt(from)->pieceType == PieceType::pawn)
    {
        if ( PieceAt(from)->color == Color::white )
        {
            Position tmp1 = from + Position(-1,-1);
            Position tmp2 = from + Position(-1, 1);
            
            if (tmp1.height >= 0 && tmp1.width >= 0 && tmp2.height >= 0 && tmp2.width < board_width)
            {
                if ( PieceAt(tmp1) == nullptr ||
                 (PieceAt(tmp1) != nullptr && PieceAt(tmp1)->color != PieceAt(from)->color) )
                    AttackedPos.push_back(tmp1);
                
                if ( PieceAt(tmp2) == nullptr ||
                    (PieceAt(tmp2) != nullptr && PieceAt(tmp2)->color != PieceAt(from)->color) )
                    AttackedPos.push_back(tmp2);
            }
        }
        else
        {
            Position tmp1 = from + Position(1,1);
            Position tmp2 = from + Position(1, -1);
            
            if (tmp1.height < board_height && tmp1.width < board_width && tmp2.height < board_height && tmp2.width >= 0 )
            {
                if ( PieceAt(tmp1) == nullptr ||
                    (PieceAt(tmp1) != nullptr && PieceAt(tmp1)->color != PieceAt(from)->color) )
                    AttackedPos.push_back(tmp1);
                
                if ( PieceAt(tmp2) == nullptr ||
                    (PieceAt(tmp2) != nullptr && PieceAt(tmp2)->color != PieceAt(from)->color) )
                    AttackedPos.push_back(tmp2);
            }
        }
        return AttackedPos;
    }
    
    for (auto & move : possibleMoves)
    {
        Position tmp = from + move;
        
        if ( tmp.height < 0 || tmp.height >= board_height || tmp.width < 0 || tmp.width >= board_width) continue;
        if (! isEmptyBetween(from, tmp) && PieceAt(from)->pieceType != PieceType::horse) continue;
        if ( PieceAt(tmp) != nullptr && PieceAt(tmp)->color == PieceAt(from)-> color ) continue;
        AttackedPos.push_back(tmp);
    }
    return AttackedPos;
}


std::vector< std::pair<Position,Position> > Board :: GenerateLegalMoves(const Color & color)
{
    std::vector < std::pair<Position,Position> > LegalMoves;
    
    for ( const Position & from : findAllPiecesOf(color) )
    {
        for ( const Position & to : Piece_GenerateLegalMoves(from) )
        {
            LegalMoves.push_back(std::pair<Position,Position>(from, to) );
        }
    }
    return LegalMoves;
}


std::vector<Position> Board :: GenerateAttackedSquares(const Color & color)
{
    std::vector<Position> AttackedPos;
    
    for (int i = 0 ; i < board_height ; i++)
    {
        for (int k = 0 ; k < board_width ; k++)
        {
            std::shared_ptr<Piece> tmpSquare = PieceAt( Position(i,k) );
            if ( tmpSquare != nullptr &&
                 tmpSquare -> color == color )
            {
                //std::vector<Position> PossibleMoves = tmpSquare -> MoveSet();
                std::vector<Position> LegalMoves = Piece_GeneratePosAttackedBy( Position(i,k) );
                
                for (auto & move : LegalMoves)
                    AttackedPos.push_back(move);
            }
        }
    }
    return AttackedPos;
}

bool Board :: isCheckMate(const Color &color)
{
    // Checks if King is in check
    if ( ! check ) return false;
    
    Position kingPos = FindPiece(PieceType::king, color);
    
    std::vector<Position> king_LegalMoves = Piece_GenerateLegalMoves( kingPos );
    
    std::vector<Position> attackedSquares;
    if (color == Color::white)
        attackedSquares = GenerateAttackedSquares(Color::black);
    else
        attackedSquares = GenerateAttackedSquares(Color::white);
    
    // Checks if king can move out of check on its own
    for ( Position & kingMove : king_LegalMoves )
    {
        if ( ! TryIfCheck(kingPos, kingMove) ) return false;
    }
    //-----
    
    // Checks if check can be blocked by friendly piece
    std::vector<Position> defendingPieces = findAllPiecesOf(color);
    
    for ( const Position & piece : defendingPieces )
    {
        std::vector<Position> piece_LegalMoves = Piece_GenerateLegalMoves(piece);
        for ( const Position & move : piece_LegalMoves )
        {
            if ( ! TryIfCheck(piece, move) ) return false;
        }
    }
    //------
    
    return true;
}

bool Board :: CheckCastling(const Position & from, const Position & to)
{
    if (PieceAt(from)->alreadyMoved) return false;
    
    if ( from.height != to.height ) return false;
    if ( from.width + 2 != to.width && from.width - 2 != to.width ) return false;
    
    Position tmp(-1,-1);
    
    if (from.width + 2 == to.width) tmp = Position(from.height, 7);
    else                            tmp = Position(from.height, 0);
    
    if (PieceAt(tmp) != nullptr &&
        PieceAt(tmp)->pieceType == PieceType::rook &&
        ! PieceAt(tmp)->alreadyMoved )
    {
        if ( ! isEmptyBetween(from, to) ) return false;
    
        Set_PieceTo(PieceAt(from), to);
        RemoveAt(from);
        
        if (tmp.width == 7)
            Set_PieceTo(PieceAt(tmp), Position(from.height, to.width - 1));
        else
            Set_PieceTo(PieceAt(tmp), Position(from.height, to.width + 1));
        RemoveAt(tmp);
        
        return true;
    }
    
    return false;
}


bool Board :: CheckPromotion( const Position & pos )
{
    if ( PieceAt(pos) -> pieceType != PieceType::pawn ) return false;
    if (PieceAt(pos) -> color == Color::white &&
        pos.height == 0)
    {
        RemoveAt(pos);
        Set_PieceTo( std::shared_ptr<Piece>( Queen(Color::white).clone() ), pos);
        return true;
    }
    else if (PieceAt(pos) -> color == Color::black &&
        pos.height == 7)
    {
        RemoveAt(pos);
        Set_PieceTo( std::shared_ptr<Piece>( Queen(Color::black).clone() ), pos);
        return true;
    }
    return false;
}

std::string Board :: MoveToString(const Position & from, const Position & to, bool promotion)
{
    std::stringstream move;
    
    char letters[8] = { 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H' };
    
    char letterFrom = ' ';
    char letterTo = ' ';
    
    for (int i = 0; i < 8; i++)
    {
        if (i == from.width)    letterFrom = letters[i];
        if (i == to.width)      letterTo = letters[i];
    }
    
    move << letterFrom << from.height + 1 << " " << letterTo << to.height + 1;
    
    if ( promotion ) move << " -> Q";
    
    return move.str();
}


Board :: Board()
{
    // Resizing the vectors to adequate size (both Height and Width) so it can be operated on through [] operator
    board.resize(board_height, std::vector< std::shared_ptr<Piece> >(board_width));
    
    
//    std::pair<std::string, std::string> aaa;
//    aaa.first = "ahoj";
    
    turn = Color::white;
    lastMove = "";
    check = false;
    checkMate = false;
    
    for (int i = 0 ; i < 8 ; i++)
    {
        for (int k = 0; k < 8 ; k++)
            board[i][k] = nullptr;
    }
}

Board :: Board(Board & copiedBoard)
{
    board.resize(board_height, std::vector< std::shared_ptr<Piece> >(board_width));
    
    for ( int i = 0 ; i < board_height ; i++ )
    {
        for ( int k = 0 ; k < board_width ; k++)
        {
            if (copiedBoard.PieceAt(Position(i,k)) != nullptr)     Set_PieceTo(copiedBoard.PieceAt(Position(i,k))->clone(), Position(i,k));
            else                                                   board[i][k] = nullptr;
        }
    }
    
    for (auto piece : copiedBoard.EliminatedPiecesBlack)
        Append_EliminatedPieces(Color::black, piece);
    
    for (auto piece : copiedBoard.EliminatedPiecesWhite)
        Append_EliminatedPieces(Color::white, piece);
    
    turn = copiedBoard.turn;
    check = copiedBoard.check;
    checkMate = copiedBoard.checkMate;
}

void Board :: Init()
{
    // WHITE
    Set_PieceTo(std::shared_ptr<Piece>(Rook(Color::white).clone()), Position(7,0));
    Set_PieceTo(std::shared_ptr<Piece>(Rook(Color::white).clone()), Position(7,7));
    Set_PieceTo(std::shared_ptr<Piece>(Horse(Color::white).clone()), Position(7,1));
    Set_PieceTo(std::shared_ptr<Piece>(Horse(Color::white).clone()), Position(7,6));
    Set_PieceTo(std::shared_ptr<Piece>(Bishop(Color::white).clone()), Position(7,2));
    Set_PieceTo(std::shared_ptr<Piece>(Bishop(Color::white).clone()), Position(7,5));
    Set_PieceTo(std::shared_ptr<Piece>(King(Color::white).clone()), Position(7,3));
    Set_PieceTo(std::shared_ptr<Piece>(Queen(Color::white).clone()), Position(7,4));
    
    //BLACK
    Set_PieceTo(std::shared_ptr<Piece>(Rook(Color::black).clone()), Position(0,0));
    Set_PieceTo(std::shared_ptr<Piece>(Rook(Color::black).clone()), Position(0,7));
    Set_PieceTo(std::shared_ptr<Piece>(Horse(Color::black).clone()), Position(0,1));
    Set_PieceTo(std::shared_ptr<Piece>(Horse(Color::black).clone()), Position(0,6));
    Set_PieceTo(std::shared_ptr<Piece>(Bishop(Color::black).clone()), Position(0,2));
    Set_PieceTo(std::shared_ptr<Piece>(Bishop(Color::black).clone()), Position(0,5));
    Set_PieceTo(std::shared_ptr<Piece>(King(Color::black).clone()), Position(0,3));
    Set_PieceTo(std::shared_ptr<Piece>(Queen(Color::black).clone()), Position(0,4));
    
    
    for (int i = 0; i < board_width ; i++)
    {
        Set_PieceTo(std::shared_ptr<Piece>(Pawn(Color::white).clone()), Position(6,i));
        Set_PieceTo(std::shared_ptr<Piece>(Pawn(Color::black).clone()), Position(1,i));
    }
    
    
}

