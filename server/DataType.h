#pragma once
#include<atomic>
/*
재정의한 데이터 타입용 헤더
*/

//Object Infor
using PositionType          = unsigned short;
using HpType                = unsigned char;
using LevelType             = unsigned char;
using ExpType               = unsigned char;
using DamageType            = unsigned char;
using ObjectIDType          = unsigned short; //Object ID(Index)


using SocketIndexType       = unsigned int;
using TextureDirection      = unsigned char;


using BoolType              = bool;
using uIntType              = unsigned int;

//Packet Type
using ObjectClass           = char;  //Class(Monster, User, NPC)
using PacketSize            = char;
using PacketType            = char;

//Atomic Type
using AtomicBool            = std::atomic_bool;
using AtomicInt32           = std::atomic_int32_t;
using AtomicChar            = std::atomic_char;
using AtomicUchar           = std::atomic_uchar;
using Atomicshort           = std::atomic_short;
using AtomicUshort          = std::atomic_ushort;

