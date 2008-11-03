/*
Ares, a tactical space combat game.
Copyright (C) 1997, 1999-2001, 2008 Nathan Lamont

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

#ifndef ANTARES_ADMIRAL_HPP_
#define ANTARES_ADMIRAL_HPP_

// Admiral.h

#include "Scenario.h"
#include "AnyChar.h"

#pragma options align=mac68k

#define kAIsHuman                   0x00000001
#define kAIsRemote                  0x00000002
#define kAIsComputer                0x00000004
#define kABit4                      0x00000008
#define kABit5                      0x00000010
#define kABit6                      0x00000020
#define kABit7                      0x00000040
#define kABit8                      0x00000080
#define kABit9                      0x00000100
#define kABit10                     0x00000200
#define kABit11                     0x00000400
#define kABit12                     0x00000800
#define kABit13                     0x00001000
#define kABit14                     0x00002000
#define kABit15                     0x00004000
#define kABit16                     0x00008000
#define kABit17                     0x00010000
#define kABit18                     0x00020000
#define kABit19                     0x00040000
#define kABit20                     0x00080000
#define kABit21                     0x00100000
#define kABit22                     0x00200000
#define kABit23                     0x00400000
#define kABit24                     0x00800000
#define kABit25                     0x01000000
#define kABit26                     0x02000000
#define kABit27                     0x04000000
#define kABit28                     0x08000000
#define kABit29                     0x10000000
#define kABit30                     0x20000000
#define kABit31                     0x40000000
#define kABit32                     0x80000000

#define kNoFreeAdmiral              -1

#define kMaxDestObject              10 // we keep special track of dest objects for AI
#define kMaxNumAdmiralCanBuild      (kMaxDestObject * kMaxTypeBaseCanBuild)

#define kDestinationNameLen         17
#define kAdmiralNameLen             31

#define kAdmiralScoreNum            3

typedef enum
{
    kNoDestinationType,
    kObjectDestinationType,
    kCoordinateDestinationType
} destinationType;

typedef struct
{
    long            whichObject;
    long            canBuildType[kMaxTypeBaseCanBuild];
    long            occupied[kScenarioPlayerNum];
    smallFixedType  earn;
    long            buildTime;
    long            totalBuildTime;
    long            buildObjectBaseNum;
    anyCharType     name[kDestinationNameLen + 1];
} destBalanceType;

typedef struct
{
    baseObjectType  *base;
    long            baseNum;
    smallFixedType  chanceRange;
} admiralBuildType;

typedef struct
{
    unsigned long       attributes;
    long                destinationObject;
    long                destinationObjectID;
    long                flagship;
    long                flagshipID;
    long                considerShip;
    long                considerShipID;
    long                considerDestination;
    long                buildAtObject; // # of destination object to build at
    long                race;
    destinationType     destType;
    smallFixedType      cash;
    smallFixedType      saveGoal;
    smallFixedType      earningPower;
    long                kills;
    long                losses;
    long                shipsLeft;
    long                score[kAdmiralScoreNum];
    long                blitzkrieg;
    smallFixedType      lastFreeEscortStrength;
    smallFixedType      thisFreeEscortStrength;
    admiralBuildType    canBuildType[kMaxNumAdmiralCanBuild];
    smallFixedType      totalBuildChance;
    long                hopeToBuild;
    unsigned char       color;
    Boolean             active;
    anyCharType         name[kAdmiralNameLen + 1];
} admiralType;


#define mGetDestObjectBalancePtr( whichObject) (destBalanceType *)*gAresGlobal->gDestBalanceData + (long)(whichObject)
#define mGetAdmiralPtr( mwhichAdmiral) (admiralType *)*gAresGlobal->gAdmiralData + (long)(mwhichAdmiral)

int AdmiralInit( void);
void AdmiralCleanup( void);
void ResetAllAdmirals( void);
void ResetAllDestObjectData( void);
long MakeNewAdmiral( long, long, destinationType, unsigned long, long, short, short, smallFixedType);
long MakeNewDestination( long, long *, smallFixedType, short, short);
void RemoveDestination( long);
void RecalcAllAdmiralBuildData( void);
void SetAdmiralAttributes( long, unsigned long);
void SetAdmiralColor( long, unsigned char);
long GetAdmiralRace( long);
void SetAdmiralEarningPower( long, smallFixedType);
smallFixedType GetAdmiralEarningPower( long);
unsigned char GetAdmiralColor( long);
void SetAdmiralFlagship( long, long);
spaceObjectType *GetAdmiralFlagship( long);
void SetAdmiralDestinationObject( long, long, destinationType);
long GetAdmiralDestinationObject( long);
void SetAdmiralConsiderObject( long, long);
long GetAdmiralConsiderObject( long);
long GetAdmiralBuildAtObject( long);
void SetAdmiralBuildAtObject( long, long);
Boolean BaseHasSomethingToBuild( long);
anyCharType *GetAdmiralBuildAtName( long);
void SetAdmiralBuildAtName( long, StringPtr);
anyCharType *GetDestBalanceName( long);
anyCharType *GetAdmiralName( long);
void SetAdmiralName( long, anyCharType *);
void SetObjectLocationDestination( spaceObjectType *, coordPointType *);
void SetObjectDestination( spaceObjectType *, spaceObjectType *);
void RemoveObjectFromDestination( spaceObjectType *);
void AdmiralThink( void);
smallFixedType HackGetObjectStrength( spaceObjectType *);
void AdmiralBuildAtObject( long, long, long);
Boolean AdmiralScheduleBuild( long, long);
void StopBuilding( long);
void PayAdmiral( long, long);
void PayAdmiralAbsolute( long, long);
void AlterAdmiralScore( long, long, long);
long AlterDestinationObjectOccupation( long, long, long);
void ClearAllOccupants( long, long, long);
long GetAdmiralScore( long, long);
long GetAdmiralShipsLeft( long);
void AddKillToAdmiral( spaceObjectType *);
long GetAdmiralLoss( long);
long GetAdmiralKill( long);

/* for copying into other #define files
#define kABit1                      0x00000001
#define kABit2                      0x00000002
#define kABit3                      0x00000004
#define kABit4                      0x00000008
#define kABit5                      0x00000010
#define kABit6                      0x00000020
#define kABit7                      0x00000040
#define kABit8                      0x00000080
#define kABit9                      0x00000100
#define kABit10                     0x00000200
#define kABit11                     0x00000400
#define kABit12                     0x00000800
#define kABit13                     0x00001000
#define kABit14                     0x00002000
#define kABit15                     0x00004000
#define kABit16                     0x00008000
#define kABit17                     0x00010000
#define kABit18                     0x00020000
#define kABit19                     0x00040000
#define kABit20                     0x00080000
#define kABit21                     0x00100000
#define kABit22                     0x00200000
#define kABit23                     0x00400000
#define kABit24                     0x00800000
#define kABit25                     0x01000000
#define kABit26                     0x02000000
#define kABit27                     0x04000000
#define kABit28                     0x08000000
#define kABit29                     0x10000000
#define kABit30                     0x20000000
#define kABit31                     0x40000000
#define kABit32                     0x80000000
*/


#pragma options align=reset

#endif // ANTARES_ADMIRAL_HPP_