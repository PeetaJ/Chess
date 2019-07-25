//
//  Human.cpp
//  Chess
//
//  Created by Petr Janousek on 22/05/2019.
//  Copyright © 2019 Petr Janousek. All rights reserved.
//

#include "Human.hpp"


void Human :: Input(std::string & from, std::string & to, Board & board)
{
    std::cin >> from;
    // just to be not unused :(
    board.Get_boardHeight();
    
    if ( from == "q" ) return;
    
    std::cin >> to;
}

std::shared_ptr<Player> Human :: clone() const
{
    return std::make_shared<Human>(*this);
}

int Human :: Get_Difficulty() const
{
    return difficulty;
}
