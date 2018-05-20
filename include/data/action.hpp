// Copyright (C) 1997, 1999-2001, 2008 Nathan Lamont
// Copyright (C) 2015-2017 The Antares Authors
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

#ifndef ANTARES_DATA_ACTION_HPP_
#define ANTARES_DATA_ACTION_HPP_

#include <stdint.h>
#include <memory>
#include <pn/string>
#include <sfz/sfz.hpp>
#include <vector>

#include "data/enums.hpp"
#include "data/field.hpp"
#include "data/handle.hpp"
#include "math/fixed.hpp"
#include "math/geometry.hpp"
#include "math/units.hpp"

namespace antares {

class SpaceObject;
struct Level;
struct Initial;
struct Condition;

//
// ActionBase:
//  Defines any action that an object can take.  Conditions that can cause an action to execute
//  are:
//  destroy, expire, create, collide, activate, or message.
//

struct ActionBase {
    ActionType type;

    bool reflexive;  // does it apply to object executing verb?

    struct Filter {
        uint32_t                   attributes = 0;
        std::map<pn::string, bool> tags;
        Owner                      owner = Owner::ANY;
    } filter;

    ticks delay = ticks(0);

    struct Override {
        sfz::optional<Handle<const Initial>> subject;
        sfz::optional<Handle<const Initial>> object;
    } override_;
};

struct AgeAction : public ActionBase {
    bool         relative;  // if true, add value to age; if false, set age to value
    Range<ticks> value;     // age range
};

struct AssumeAction : public ActionBase {
    int64_t which;  // which initial to become
                    // Note: player 1’s score 0 is added to this number
};

struct CapSpeedAction : public ActionBase {
    sfz::optional<Fixed> value;  // if absent set to base type’s default
};

struct CaptureAction : public ActionBase {
    sfz::optional<Handle<Admiral>>
            player;  // if present, set focus’s owner to `*player`
                     // if absent and reflexive, set subject’s owner to object’s
                     // if absent and non-reflexive, set object’s owner to subject’s
};

struct CloakAction : public ActionBase {};

struct ConditionAction : public ActionBase {
    HandleList<const Condition> enable;
    HandleList<const Condition> disable;
};

struct CreateAction : public ActionBase {
    NamedHandle<const BaseObject> base;                         // what type
    Range<int64_t>                count              = {1, 2};  // # to make randomly
    bool                          relative_velocity  = false;   // is velocity relative to creator?
    bool                          relative_direction = false;   // determines initial heading
    int64_t                       distance = 0;      // create at this distance in random direction
    bool                          inherit  = false;  // if false, gets creator as target
                                                     // if true, gets creator’s target as target
    bool legacy_random = false;                      // if true, consume a random number from
                                                     // subject even if not necessary
};

struct DisableAction : public ActionBase {
    Range<Fixed> value;
};

struct EnergizeAction : public ActionBase {
    int64_t value;
};

struct EquipAction : public ActionBase {
    Weapon                        which;
    NamedHandle<const BaseObject> base;
};

struct FireAction : public ActionBase {
    Weapon which;
};

struct FlashAction : public ActionBase {
    int64_t length;  // length of color flash
    Hue     hue;     // hue of flash
    uint8_t shade;   // brightness of flash
};

struct HealAction : public ActionBase {
    int64_t value;
};

struct HoldPositionAction : public ActionBase {};

struct KeyAction : public ActionBase {
    uint32_t disable = 0;  // keys to disable
    uint32_t enable  = 0;  // keys to enable
};

struct KillAction : public ActionBase {
    KillKind kind;
};

struct LandAction : public ActionBase {
    int64_t speed;
};

struct MessageAction : public ActionBase {
    int64_t                 id;     // identifies the message to a "message" condition
    std::vector<pn::string> pages;  // pages of message bodies to show
};

struct MorphAction : public ActionBase {
    bool                          keep_ammo;
    NamedHandle<const BaseObject> base;
};

struct MoveAction : public ActionBase {
    MoveOrigin     origin;
    coordPointType to;
    int64_t        distance;
};

struct OccupyAction : public ActionBase {
    int64_t value;
};

struct OrderAction : public ActionBase {};

struct PayAction : public ActionBase {
    Fixed                          value;   // amount to pay; not affected by earning power
    sfz::optional<Handle<Admiral>> player;  // if not present pay focus’s owner.
};

struct PushAction : public ActionBase {
    PushKind kind;
    Fixed    value;
};

struct RevealAction : public ActionBase {
    HandleList<const Initial> initial;
};

struct ScoreAction : public ActionBase {
    sfz::optional<Handle<Admiral>>
            player;  // which player’s score to change; absent = owner of focus
    int64_t which;   // 0-2; each player has three "scores"
    int64_t value;   // amount to change by
};

struct SelectAction : public ActionBase {
    Screen  screen;
    int64_t line;
};

struct PlayAction : public ActionBase {
    uint8_t priority;     // 1-5; takes over a channel playing a lower-priority sound
    ticks   persistence;  // time before a lower-priority sound can take channel
    bool    absolute;     // plays at same volume, regardless of distance from player
    int64_t volume;       // 1-255; volume at focus

    struct Sound {
        pn::string sound;
    };
    sfz::optional<pn::string> sound;  // play this sound if present
    std::vector<Sound>        any;    // pick ID randomly
};

struct SparkAction : public ActionBase {
    int64_t count;     // number of sparks to create
    Hue     hue;       // hue of sparks; they start bright and fade with time
    int64_t decay;     // sparks will be visible for 17.05/decay seconds
    Fixed   velocity;  // sparks fly at at random speed up to this
};

struct SpinAction : public ActionBase {
    Range<Fixed> value;
};

struct ThrustAction : public ActionBase {
    Range<Fixed> value;  // range
};

struct WarpAction : public ActionBase {};

struct WinAction : public ActionBase {
    sfz::optional<Handle<Admiral>>          player;  // victor; absent = owner of focus
    sfz::optional<NamedHandle<const Level>> next;    // next chapter to play; absent = none
    pn::string                              text;    // "debriefing" text
};

struct ZoomAction : public ActionBase {
    Zoom value;
};

union Action {
    ActionBase base;
    ActionType type() const;

    AgeAction          age;
    AssumeAction       assume;
    CapSpeedAction     cap_speed;
    CaptureAction      capture;
    CloakAction        cloak;
    ConditionAction    condition;
    CreateAction       create;
    DisableAction      disable;
    EnergizeAction     energize;
    EquipAction        equip;
    FireAction         fire;
    FlashAction        flash;
    HealAction         heal;
    HoldPositionAction hold_position;
    KeyAction          key;
    KillAction         kill;
    LandAction         land;
    MessageAction      message;
    MorphAction        morph;
    MoveAction         move;
    OccupyAction       occupy;
    OrderAction        order;
    PayAction          pay;
    PushAction         push;
    RevealAction       reveal;
    ScoreAction        score;
    SelectAction       select;
    PlayAction         play;
    SparkAction        spark;
    SpinAction         spin;
    ThrustAction       thrust;
    WarpAction         warp;
    WinAction          win;
    ZoomAction         zoom;

    Action(AgeAction a);
    Action(AssumeAction a);
    Action(CapSpeedAction a);
    Action(CaptureAction a);
    Action(CloakAction a);
    Action(ConditionAction a);
    Action(CreateAction a);
    Action(DisableAction a);
    Action(EnergizeAction a);
    Action(EquipAction a);
    Action(FireAction a);
    Action(FlashAction a);
    Action(HealAction a);
    Action(HoldPositionAction a);
    Action(KeyAction a);
    Action(KillAction a);
    Action(LandAction a);
    Action(MessageAction a);
    Action(MorphAction a);
    Action(MoveAction a);
    Action(OccupyAction a);
    Action(OrderAction a);
    Action(PayAction a);
    Action(PushAction a);
    Action(RevealAction a);
    Action(ScoreAction a);
    Action(SelectAction a);
    Action(PlayAction a);
    Action(SparkAction a);
    Action(SpinAction a);
    Action(ThrustAction a);
    Action(WarpAction a);
    Action(WinAction a);
    Action(ZoomAction a);

    ~Action();
    Action(Action&&);
    Action& operator=(Action&&);
};

Action action(path_value x);

}  // namespace antares

#endif  // ANTARES_DATA_ACTION_HPP_
