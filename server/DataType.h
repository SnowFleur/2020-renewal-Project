#pragma once
#include<atomic>
/*
�������� ������ Ÿ�Կ� ���
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
using DamageType            = unsigned char;
using ObjectIDType          = unsigned short; //Object ID(Index)

//Character Texture Direction
constexpr int CHARACTER_DOWN    = 0;
constexpr int CHARACTER_LEFT    = 1;
constexpr int CHARACTER_RIGHT   = 2;
constexpr int CHARACTER_UP      = 3;

//Socket
using SocketIndexType       = unsigned int;
using TextureDirection      = unsigned char;    //0:�Ʒ�, 1:��, 2:��, 3:��


using BoolType              = bool;
using uIntType              = unsigned int;

//Packet Type
using ObjectClass           = char;  //Class(Monster, User, NPC)
using PacketSize            = char;
using PacketType            = char;


