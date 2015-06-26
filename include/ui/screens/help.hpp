// Copyright (C) 1997, 1999-2001, 2008 Nathan Lamont
// Copyright (C) 2008-2012 The Antares Authors
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

#ifndef ANTARES_UI_SCREENS_HELP_HPP_
#define ANTARES_UI_SCREENS_HELP_HPP_

#include <sfz/sfz.hpp>

#include "drawing/styled-text.hpp"
#include "ui/screen.hpp"

namespace antares {

class HelpScreen : public InterfaceScreen {
  public:
    HelpScreen();
    ~HelpScreen();

    virtual void key_down(const KeyDownEvent& event);
    virtual void overlay() const;

  protected:
    virtual void handle_button(Button& button);

  private:
    enum Item {
        DONE = 0,

        BOX = 1,
    };

    Rect _bounds;
    StyledText _text;

    DISALLOW_COPY_AND_ASSIGN(HelpScreen);
};

}  // namespace antares

#endif  // ANTARES_UI_SCREENS_HELP_HPP_
