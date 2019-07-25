//
//  Computer.hpp
//  Chess
//
//  Created by Petr Janousek on 22/05/2019.
//  Copyright © 2019 Petr Janousek. All rights reserved.
//

#ifndef Computer_hpp
#define Computer_hpp

#include "Player.hpp"
#include <iostream>
#include <memory>
#include <utility>
#include <algorithm>

/** Representation of the computer player. Inherits from Player class. */
class Computer : public Player
{
private:
     /** Returns the move the algorithm chose to use as his next
     @param[in] board The reference to the board it's played on so it can make decisions based on the board.
     @return pair of Positions that represents the next move of the computer.
     */
    std::pair<Position, Position> RandomMoves(Board & board);
    
    /** Returns the move the algorithm chose to use as his next
     @param[in] board The reference to the board it's played on so it can make decisions based on the board.
     @return pair of Positions that represents the next move of the computer.
     */
    std::pair<Position, Position> MinMaxMoves(Board & board);
    
    //    HELPER FUNCTIONS
    
    /** Recursive Minimax algorithm to choose the next move
     @param[in] depth The parameter that sets how many turns into future will the algorithm base it's choosing of move.
     @param[in] playerColor Chooses which player's turn it is in current depth of recursion.
     @param[in] maximizingPlayer The parameter that sets which player to maximize it's results.
     @param[in] board The board it currently bases it's moves on.
     @param[out] bestMove If it finds the best move, it saves the move to this pair.
     @return integer value of the evaluated player's side. (Each piece has it's value, and the algoritms tries to maximize/minimize this value.)
     */
    int MinMax (int depth, const Color & playerColor, bool maximizingPlayer, Board & board, std::pair<Position, Position> & bestMove);
    
    /** Helper function to convert Position to string
     @param[in] pos The Position to be converted.
     @return string representation of the converted Position.
     */
    std::string MoveToString(Position & pos);
    
    /** Evaluates the player's side (all of the pieces of one color) and returns the integer value of it.
     @param[in] side Which color to be evaluated.
     @param[in] board Board the value will be based off.
     @return integer value of the player's pieces.
     */
    int EvaluateSideValue(const Color side, Board & board);
    
public:
    Computer(int difficulty = 1) : Player(difficulty) {};
    virtual ~Computer() {};
    
    virtual void Input(std::string & from, std::string & to, Board & board);
    
    virtual std::shared_ptr<Player> clone() const;
    
    virtual int Get_Difficulty() const;
};

#endif /* Computer_hpp */
