#pragma once
#include<utility>
/*
�������� ������ Ÿ�Կ� ���
*/

//Object Infor
using PositionType          = unsigned short;
using HpType                = unsigned char;
using LevelType             = unsigned char;
using ExpType               = unsigned char;
using DamageType            = unsigned char;
using ObjectIDType          = unsigned short; //Object ID(Index)
using PairPositionType      = std::pair<PositionType, PositionType>; //x, y


using SocketIndexType       = unsigned int;
using TextureDirection      = unsigned char;


using BoolType              = bool;
using uIntType              = unsigned int;

//Packet Type
using ObjectClass           = char;  //Class(Monster, User, NPC)
using PacketSize            = char;
using PacketType            = char;
