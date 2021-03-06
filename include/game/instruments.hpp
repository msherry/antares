// Copyright (C) 1997, 1999-2001, 2008 Nathan Lamont
// Copyright (C) 2008-2017 The Antares Authors
//
// This file is part of Antares, a tactical space combat game.
//
// Antares is free software: you can redistribute it and/or modify it
// under the terms of the Lesser GNU General Public License as published
// by the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Antares is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with Antares.  If not, see http://www.gnu.org/licenses/

#ifndef ANTARES_GAME_INSTRUMENTS_HPP_
#define ANTARES_GAME_INSTRUMENTS_HPP_

#include "drawing/sprite-handling.hpp"
#include "math/scale.hpp"
#include "math/units.hpp"

namespace antares {

class PlayerShip;

const int32_t kMiniBuildTimeHeight = 25;

void    InstrumentInit();
int32_t instrument_top();
void    UpdateRadar(ticks units);
void    InstrumentCleanup();
void    ResetInstruments();
void    set_up_instruments();
void    draw_instruments();
void    EraseSite();
bool    update_site();
void    draw_site(const PlayerShip& player);
bool    update_sector_lines();
void    draw_sector_lines();
void    draw_arbitrary_sector_lines(
           const Point& corner, Scale scale, int32_t minSectorSize, const Rect& bounds);

}  // namespace antares

#endif  // ANTARES_GAME_INSTRUMENTS_HPP_
