//
//  Position.cpp
//  Chess
//
//  Created by Petr Janousek on 05/05/2019.
//  Copyright © 2019 Petr Janousek. All rights reserved.
//

#include "Position.hpp"

Position Position :: operator + (const Position & pos) const
{
    return Position(this->height + pos.height, this->width + pos.width);
}

bool Position :: operator == (const Position & pos) const
{
    if (this-> height == pos . height &&
        this-> width == pos. width)
        return true;
    return false;
}

bool Position :: operator < (const Position & pos) const
{
    if (this -> height < pos.height) return true;
    if (this -> width < pos.width) return true;
    return false;
}
