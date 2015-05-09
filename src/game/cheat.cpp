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

#include "game/cheat.hpp"

#include <sfz/sfz.hpp>

#include "data/string-list.hpp"
#include "game/admiral.hpp"
#include "game/globals.hpp"
#include "game/messages.hpp"
#include "game/player-ship.hpp"
#include "game/space-object.hpp"
#include "math/fixed.hpp"

using sfz::BytesSlice;
using sfz::PrintItem;
using sfz::Rune;
using sfz::String;
using sfz::StringSlice;
using std::unique_ptr;

namespace antares {

const int16_t kCheatStringListID    = 750;
const int16_t kCheatFeedbackOnID    = 751;
const int16_t kCheatFeedbackOffID   = 752;

const int32_t kCheatCodeValue       = 5;

const int16_t kActivateCheatCheat   = 1;
const int16_t kAutoPlayCheat        = 2;
const int16_t kPayMoneyCheat        = 3;
const int16_t kNameObjectCheat      = 4;
const int16_t kObserverCheat        = 5;  // makes your ship appear to not be engageable
const int16_t kBuildFastCheat       = 6;
const int16_t kRaisePayRateCheat    = 7;  // determines your payscale
const int16_t kLowerPayRateCheat    = 8;

static unique_ptr<StringList> gAresCheatStrings;

void CheatFeedback(int16_t whichCheat, bool activate, Handle<Admiral> whichPlayer);
void CheatFeedbackPlus(int16_t whichCheat, bool activate, Handle<Admiral> whichPlayer, PrintItem extra);

void AresCheatInit() {
    gAresCheatStrings.reset(new StringList(kCheatStringListID));
}

void CleanupAresCheat() {
    gAresCheatStrings.reset();
}

int16_t GetCheatNumFromString(const StringSlice& s) {
    String code_string;
    for (Rune r: s) {
        code_string.append(1, r + kCheatCodeValue);
    }
    return gAresCheatStrings.get()->index_of(code_string) + 1;
}

void ExecuteCheat(int16_t whichCheat, Handle<Admiral> whichPlayer) {
    int32_t                    i;

    if ( whichCheat == kNameObjectCheat)
    {
        whichPlayer->cheats() |= kNameObjectBit;
        CheatFeedback( whichCheat, true, whichPlayer);
        return;
    }

    switch( whichCheat)
    {
        case kActivateCheatCheat:
            for (auto adm: Admiral::all()) {
                adm->cheats() = 0;
            }
            CheatFeedback( whichCheat, false, whichPlayer);
            break;

        case kPayMoneyCheat:
            whichPlayer->pay_absolute(mLongToFixed(5000));
            whichPlayer->pay_absolute(mLongToFixed(5000));
            whichPlayer->pay_absolute(mLongToFixed(5000));
            CheatFeedback( whichCheat, true, whichPlayer);
            break;

        case kAutoPlayCheat:
            whichPlayer->cheats() ^= kAutoPlayBit;
            CheatFeedback(whichCheat, whichPlayer->cheats() & kAutoPlayBit, whichPlayer);
            break;

        case kBuildFastCheat:
            whichPlayer->cheats() ^= kBuildFastBit;
            CheatFeedback(whichCheat, whichPlayer->cheats() & kBuildFastBit, whichPlayer);
            break;

        case kObserverCheat:
            if (whichPlayer->flagship().get()) {
                whichPlayer->flagship()->attributes &= ~(kCanBeEngaged | kHated);
                CheatFeedback( whichCheat, true, whichPlayer);
            }
            break;

        case kRaisePayRateCheat:
            whichPlayer->set_earning_power(whichPlayer->earning_power() + 0x20);
            CheatFeedbackPlus(whichCheat, true, whichPlayer, fixed(whichPlayer->earning_power()));
            break;

        case kLowerPayRateCheat:
            whichPlayer->set_earning_power(whichPlayer->earning_power() - 0x20);
            CheatFeedbackPlus(whichCheat, true, whichPlayer, fixed(whichPlayer->earning_power()));
            break;
    }
}

void CheatFeedback(int16_t whichCheat, bool activate, Handle<Admiral> whichPlayer) {
    String admiral_name(GetAdmiralName(whichPlayer));
    String feedback;
    if (activate) {
        feedback.assign(StringList(kCheatFeedbackOnID).at(whichCheat - 1));
    } else {
        feedback.assign(StringList(kCheatFeedbackOffID).at(whichCheat - 1));
    }
    Messages::add(format("{0}{1}", admiral_name, feedback));
}

void CheatFeedbackPlus(
        int16_t whichCheat, bool activate, Handle<Admiral> whichPlayer, PrintItem extra) {
    String admiral_name(GetAdmiralName(whichPlayer));
    String feedback;
    if (activate) {
        feedback.assign(StringList(kCheatFeedbackOnID).at(whichCheat - 1));
    } else {
        feedback.assign(StringList(kCheatFeedbackOffID).at(whichCheat - 1));
    }
    Messages::add(format("{0}{1}{2}", admiral_name, feedback, extra));
}

}  // namespace antares
