//
//  Player.hpp
//  Chess
//
//  Created by Petr Janousek on 24/04/2019.
//  Copyright © 2019 Petr Janousek. All rights reserved.
//

#ifndef Player_hpp
#define Player_hpp

#include <iostream>
#include <memory>

#include "Position.hpp"

#include "Board.hpp"

/** Abstract class representing the players that are playing the game. */
class Player
{
protected:
    int difficulty; /*!< How good the computer will play. Redundant in human player. */
public:
    Player(int difficulty = 1) : difficulty(difficulty) {};
    virtual ~Player() {};
    
    /** Returns the move chosen by the player.
     @param[out] from The string representation of Position where the player's move starts.
     @param[out] to The string representation of Position where the player's move ends.
     @param[in] board The reference to board the current game is played on. Only relevant for computer players.
     */
    virtual void Input(std::string & from, std::string & to, Board & board) = 0;
    
    /** Clones the player. */
    virtual std::shared_ptr<Player> clone() const = 0;
    
    virtual int Get_Difficulty() const = 0;
};

#endif /* Player_hpp */
