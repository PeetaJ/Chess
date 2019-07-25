//
//  Queen.cpp
//  Chess
//
//  Created by Petr Janousek on 04/05/2019.
//  Copyright © 2019 Petr Janousek. All rights reserved.
//

#include "Queen.hpp"


void Queen :: print() const
{
    if (color == Color::black)
    {
        printf("\x1B[31mQ\033[0m");
    }
    if (color == Color::white)
        std :: cout << "Q";
}

std::shared_ptr<Piece> Queen :: clone() const
{
    return std::make_shared<Queen>(*this);
}

std::vector<Position> Queen::MoveSet()
{
    std::vector<Position> moveSet;
    
    for ( auto & move : Moves )
    {
        Position tmp = move;
        while( abs(tmp.height) != 8 && abs(tmp.width) != 8 )
        {
            moveSet.push_back(tmp);
            tmp = tmp + move;
        }
    }
    
    return moveSet;
}

void Queen :: Set_AlreadyMoved(bool moved)
{
    alreadyMoved = moved;
}
