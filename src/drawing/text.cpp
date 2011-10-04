// Copyright (C) 1997, 1999-2001, 2008 Nathan Lamont
// Copyright (C) 2008-2011 Ares Central
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
// License along with this program.  If not, see
// <http://www.gnu.org/licenses/>.

#include "drawing/text.hpp"

#include <algorithm>
#include <sfz/sfz.hpp>

#include "data/resource.hpp"
#include "drawing/color.hpp"

using sfz::Bytes;
using sfz::BytesSlice;
using sfz::String;
using sfz::StringSlice;
using sfz::read;
using sfz::scoped_ptr;

namespace macroman = sfz::macroman;

namespace antares {

directTextType* gDirectText = NULL;
long gWhichDirectText = 0;
scoped_ptr<directTextType>* gDirectTextData;

namespace {

const int kDirectFontNum = 6;

enum {
    kTacticalFontResID      = 5000,
    kComputerFontResID      = 5001,
    kButtonFontResID        = 5002,
    kMessageFontResID       = 5003,
    kTitleFontResID         = 5004,
    kButtonSmallFontResID   = 5005,
};

uint8_t to_mac_roman(uint32_t code) {
    String string(1, code);
    Bytes bytes(macroman::encode(string));
    return bytes.at(0);
}

}  // namespace

directTextType::directTextType(int32_t id) {
    Resource defn_rsrc("font-descriptions", "nlFD", id);
    BytesSlice in(defn_rsrc.data());

    in.shift(4);
    read(in, resID);
    in.shift(2);
    read(in, logicalWidth);
    read(in, physicalWidth);
    read(in, height);
    read(in, ascent);

    Resource data_rsrc("font-bitmaps", "nlFM", resID);
    charSet.assign(data_rsrc.data());
}

directTextType::~directTextType() { }

void InitDirectText() {
    gDirectTextData = new scoped_ptr<directTextType>[kDirectFontNum];
    gDirectTextData[0].reset(new directTextType(kTacticalFontResID));
    gDirectTextData[1].reset(new directTextType(kComputerFontResID));
    gDirectTextData[2].reset(new directTextType(kButtonFontResID));
    gDirectTextData[3].reset(new directTextType(kMessageFontResID));
    gDirectTextData[4].reset(new directTextType(kTitleFontResID));
    gDirectTextData[5].reset(new directTextType(kButtonSmallFontResID));

    gDirectText = gDirectTextData[0].get();
    gWhichDirectText = 0;
}

void DirectTextCleanup() {
    delete[] gDirectTextData;
}

void mDirectCharWidth(unsigned char& width, uint32_t mchar) {
    const uint8_t* widptr = gDirectText->charSet.data()
        + gDirectText->height * gDirectText->physicalWidth * to_mac_roman(mchar)
        + to_mac_roman(mchar);
    width = *widptr;
}

void mSetDirectFont(long whichFont) {
    gWhichDirectText = whichFont;
    gDirectText = gDirectTextData[gWhichDirectText].get();
}

int mDirectFontHeight() {
    return gDirectText->height;
}

int mDirectFontAscent() {
    return gDirectText->ascent;
}

void mGetDirectStringDimensions(const StringSlice& string, long& width, long& height) {
    height = gDirectText->height;
    width = 0;
    for (size_t i = 0; i < string.size(); ++i) {
        const uint8_t* widptr = gDirectText->charSet.data()
            + gDirectText->height * gDirectText->physicalWidth * to_mac_roman(string.at(i))
            + to_mac_roman(string.at(i));
        width += *widptr;
    }
}

void DrawDirectTextStringClipped(
        Point origin, StringSlice string, RgbColor color, PixMap* pix, const Rect& clip) {
    // move the pen to the resulting location
    origin.v -= gDirectText->ascent;

    // Top and bottom boundaries of where we draw.
    int topEdge = std::max(0, clip.top - origin.v);
    int bottomEdge = gDirectText->height - std::max(
            0, origin.v + gDirectText->height - clip.bottom + 1);

    int rowBytes = pix->row_bytes();

    // set hchar = place holder for start of each char we draw
    RgbColor* hchar = pix->mutable_bytes() + (origin.v + topEdge) * rowBytes + origin.h;

    for (size_t i = 0; i < string.size(); ++i) {
        const uint8_t* sbyte = gDirectText->charSet.data()
            + gDirectText->height * gDirectText->physicalWidth * to_mac_roman(string.at(i))
            + to_mac_roman(string.at(i));

        int width = *sbyte;
        ++sbyte;

        if ((origin.h + width >= clip.left) || (origin.h < clip.right)) {
            // Left and right boundaries of where we draw.
            int leftEdge = std::max(0, clip.left - origin.h);
            int rightEdge = width - std::max(0, origin.h + width - clip.right);

            // skip over the clipped top rows
            sbyte += topEdge * gDirectText->physicalWidth;

            // dbyte = destination pixel
            RgbColor* dbyte = hchar;

            // repeat for every unclipped row
            for (int y = topEdge; y < bottomEdge; ++y) {
                // repeat for every byte of data
                for (int x = leftEdge; x < rightEdge; ++x) {
                    int byte = x / 8;
                    int bit = 0x80 >> (x & 0x7);
                    if (sbyte[byte] & bit) {
                        dbyte[x] = color;
                    }
                }
                sbyte += gDirectText->physicalWidth;
                dbyte += rowBytes;
            }
        }
        // else (not on screen) just increase the current character

        // for every char clipped or no:
        // increase our character pixel starting point by width of this character
        hchar += width;

        // increase our hposition (our position in pixels)
        origin.h += width;
    }
    MoveTo(origin.h, origin.v + gDirectText->ascent);
}

}  // namespace antares