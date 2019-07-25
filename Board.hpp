//
//  Board.hpp
//  Chess
//
//  Created by Petr Janousek on 13/04/2019.
//  Copyright © 2019 Petr Janousek. All rights reserved.
//

#ifndef Board_hpp
#define Board_hpp

#include <set>
#include <unordered_set>
#include <sstream>

#include "Piece_utility.hpp"
#include <iostream>
#include <memory>
#include <utility>
#include <algorithm>

/**
 Used as 'database' for the game and all it's movement.
 **/
class Board
{
private:
    const static int board_width = 8; /*!< value of how wide is the board */
    const static int board_height = 8; /*!< value of how high is the board */
    
    std:: vector< std::vector< std::shared_ptr<Piece> > > board; /*!< The 2D vector that represents the board  */
    
    std::vector< std::shared_ptr<Piece> > EliminatedPiecesWhite;
    std::vector< std::shared_ptr<Piece> > EliminatedPiecesBlack;
    
    Color turn;
    bool check; /*!< If check is true, the player on the turn has to deal with it */
    bool checkMate;
    std::string lastMove; /*!< Shows what was the opponents last move */
    
public:
    /** Board constructor
     * Resizes the 2D vector board to the proper size so it can be operated on with [] operator.
     * Sets all the positions inside 2D vector as nullptr.
     * Sets all the basic things (first turn is white, check and checkmate is false).
     */
    Board();
    
    /** Board constructor
     * @param[in] board The board to be copied to the newly created one.
     */
    Board(Board & board);
    
    /** board initialize
     * Sets all the pieces of both color to the proper starting positions.
     */
    void Init();
    
    // GETTERS
    //  * for width and height
    int Get_boardWidth();
    int Get_boardHeight();
    
    //  * for eliminatedPieces
    std::vector< std::shared_ptr<Piece> > & Get_EliminatedPieces(const Color & color);
    
    //  * for turn
    Color Get_Turn();
    
    //  * for check
    bool Get_Check();
    
    //  * for lastMove
    std::string Get_LastMove();
    
    
    bool Get_CheckMate();
    // ------------
    
    // SETTERS
    void Append_EliminatedPieces(const Color & color, const std::shared_ptr<Piece> & piece);
    
    void Set_Turn(const Color & color);
    
    void Set_LastMove(const std::string & move);
    
    void ChangeTurn();
    
    // INPUT (FOR NOW)
    void InputMove(); // DELETE
    
    // --------
    // HELPER FUNCTIONS FOR MOVE (FOR NOW)
    void Prirad( const Piece & piece, int row, int col );
    
    /**
     * Returns piece that is at the given position on board.
     @param[in] at The Position of the board.
     @return shared_ptr of the found piece or nullptr if there is no piece.
     */
    std::shared_ptr<Piece> & PieceAt(const Position & at);
    
    /**
     * Places the piece to a given position.
     @param[in] piece The shared_ptr of the piece we want to place.
     @param[in] pos The position at which we want to place the piece.
     */
    void Set_PieceTo ( const std::shared_ptr<Piece> piece, const Position & pos);
    
    /**
     * Removes whatever is on the position and assigns nullptr to it.
     @param[in] square The Position we want to clean.
     */
    void RemoveAt(const Position & square);
    
    /** Checks if the piece's turn and turn's turn are equal.
     @param[in] piece The piece we want to know if it has the same color.
     @return true if the are equal.
     */
    bool Check_TurnPieceColor(const std::shared_ptr<Piece> & piece);
    
    /** Finds the first piece it stumbles on of given type and color.
     * Usually used to find king as there is only one of each color in the game.
     @param[in] type The type of the piece we want to find.
     @param[in] color The color of the piece we want to find.
     */
    Position FindPiece(const PieceType & type, const Color & color);
    
    /** Finds all the pieces of given color in the vector.
     @param[in] color The color we want to find all of the pieces of.
     @return vector of positions of all the pieces of given color.
     */
    std::vector<Position> findAllPiecesOf(const Color & color); //todo
    
    /** Checks if it would be check if the move proceeded.
     @param[in] from The starting position of the move to be checked.
     @param[in] to The ending position of the move to be checked.
     @return true if there would be check after the move.
     */
    bool TryIfCheck(const Position & from, const Position & to);
    
    /** Checks if the king of given color is attacked.
     * It generates the legal moves of all the pieces of opposite color
     and checks if one of the moves is to the king's position.
     @param[in]  color of which is the king to be checked.
     @return true if king is attacked.
     */
    bool isKingAttacked(const Color & color);
    
    /** Checks if there is any piece between the two positions.
     @param[in] first The first positions from where the between leads.
     @param[in] second The second positions to where the between leads.
     @return true if there is nothing in between those two positions.
     */
    bool isEmptyBetween(const Position & first, const Position & second);
    // --------
    
    /** It moves the piece on the board from one position to another.
     * Main function of the Board class.
     * Before it does, it checks if it is possible to be moved with the piece.
     * It checks if there is check or checkmate after the move.
     @param[in] from The Position of the piece to be moved.
     @param[in] to The Position the piece should be moved to.
     @return true if the move was possible.
     */
    bool Move(const Position & from, const Position & to);
    
    /** Generates all the legal moves of the piece on position.
     * It takes possible moves from piece and checks it against the board.
     @param[in] from The Position of the piece we want to generate the moves.
     @return vector of all positions of the legal moves.
     */
    std::vector<Position> Piece_GenerateLegalMoves(const Position & from);
    
    /** Generates all the positions attacked by the piece on the position.
     @param[in] from The Position of the piece we want to know what is attacking.
     @return vector of all the attacked Positions by the piece.
     */
    std::vector<Position> Piece_GeneratePosAttackedBy(const Position & from);
    
    /** Generates all the legal moves of player.
     * vector of pairs, first Position is from and the second is to.
     @param[in] color The color of the player to be generated his possible moves.
     @return vector of pairs of the legal moves to be done.
     */
    std::vector< std::pair<Position,Position> > GenerateLegalMoves(const Color & color);
    
    /** Generates all the attacked positions by the player.
     @param[in] color The color of the player to be generated positions he is attacking.
     @return vector of the attacked Positions.
     */
    std::vector<Position> GenerateAttackedSquares(const Color & color);
    
    /** Checks if it is checkmate.
     * First it checks if king is in check.
     * Then if the king can get out of check on it's own.
     * If the check can be blocked by other pieces.
     @param[in] color The color of the player to be checked if is in the checkmate.
     @return true if king has nowhere else to move.
     */
    bool isCheckMate(const Color & color);
    
    /** Checks if castling is possible and if yes it castles.
     @param[in] from The Position of the king.
     @param[in] to The Position of the king to be castled to.
     @return true if the castling was possible.
     */
    bool CheckCastling(const Position & from, const Position & to);
    
    /** Checks if promotion is possible and if yes it promotes to queen.
     @param[in] pos The Position of the pawn to be checked.
     @return true if promotion happenned.
     */
    bool CheckPromotion( const Position & pos );
    
    // HELPER
    /** Converts move to string so it can be shown as last move.
     @param[in] from The Position from of the move.
     @param[in] to The Position to of the move.
     @param[in] promotion = false if the promotion happenned, it will also show.
     @return string of the converted move.
     */
    std::string MoveToString(const Position & from, const Position & to, bool promotion = false);
    
    
};

#endif /* Board_hpp */
