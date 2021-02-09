#pragma once
#include<atomic>
/*
재정의한 데이터 타입용 헤더
*/

//Atomic Type
using AtomicBool            = std::atomic_bool;
using AtomicInt32           = std::atomic_int32_t;
using AtomicChar            = std::atomic_char;
using AtomicUchar           = std::atomic_uchar;
using Atomicshort           = std::atomic_short;
using AtomicUshort          = std::atomic_ushort;

//Object Infor
using PositionType          = unsigned short;
using HpType                = unsigned char;
using AtomicHpType          = AtomicUchar;
using LevelType             = unsigned char;
using ExpType               = unsigned char;
using AttackPowerType       = unsigned char;
using ObjectIDType          = short; //Object ID(Index)

//Character Texture Direction(Don't Modifly)
constexpr int CHARACTER_DOWN    = 0;
constexpr int CHARACTER_LEFT    = 1;
constexpr int CHARACTER_RIGHT   = 2;
constexpr int CHARACTER_UP      = 3;

//Socket
using SocketIndexType       = unsigned int;
using TextureDirection      = unsigned char;    //0:아래, 1:좌, 2:우, 3:위

using BoolType              = bool;
using uInt32Type            = unsigned int;
using uInt64Type            = unsigned long long;

//Packet Type
using ObjectClass           = char;  //Class(Monster, User, NPC)
using PacketSize            = char;
using PacketType            = char;

