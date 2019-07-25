//
//  Horse.cpp
//  Chess
//
//  Created by Petr Janousek on 04/05/2019.
//  Copyright © 2019 Petr Janousek. All rights reserved.
//

#include "Horse.hpp"


void Horse :: print() const
{
    if (color == Color::black)
    {
        printf("\x1B[31mH\033[0m");
    }
    if (color == Color::white)
        std :: cout << "H";
}

std::shared_ptr<Piece> Horse :: clone() const
{
    return std::make_shared<Horse>(*this);
}

std::vector<Position> Horse::MoveSet()
{
    std::vector<Position> tmp;
    
    for ( auto & move : Moves )
        tmp.push_back(move);
    
    return tmp;
}

void Horse :: Set_AlreadyMoved(bool moved)
{
    alreadyMoved = moved;
}
