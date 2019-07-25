//
//  King.cpp
//  Chess
//
//  Created by Petr Janousek on 04/05/2019.
//  Copyright © 2019 Petr Janousek. All rights reserved.
//

#include "King.hpp"


void King :: print() const
{
    if (color == Color::black)
    {
        printf("\x1B[31mK\033[0m");
    }
    if (color == Color::white)
        std :: cout << "K";
}

std::shared_ptr<Piece> King :: clone() const
{
    return std::make_shared<King>(*this);
}

std::vector<Position> King::MoveSet()
{
    std::vector<Position> tmp;
    
    for ( auto & move : Moves )
        tmp.push_back(move);
    
    return tmp;
}

void King :: Set_AlreadyMoved(bool moved)
{
    alreadyMoved = moved;
}
