//
//  Human.hpp
//  Chess
//
//  Created by Petr Janousek on 22/05/2019.
//  Copyright © 2019 Petr Janousek. All rights reserved.
//

#ifndef Human_hpp
#define Human_hpp

#include "Player.hpp"

#include <iomanip>
#include <iostream>
#include <memory>
#include <utility>
#include <algorithm>

/** Representation of human player. Inherits from Player class */
class Human : public Player
{
public:
    Human(int difficulty = 0) : Player(difficulty) {};
    virtual ~Human() {};
    virtual void Input(std::string & from, std::string & to, Board & board);
    virtual std::shared_ptr<Player> clone() const;
    
    virtual int Get_Difficulty() const;
    
};

#endif /* Human_hpp */
