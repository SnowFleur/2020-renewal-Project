#pragma once

#include"DataType.h"


/*�����󿡼� ���� �߰� �� ������ �ٽ� �÷��� �����ϱ�*/
#pragma region NewDefineds

#define	WM_SOCKET				WM_USER + 1
#define BUF_SIZE		        1024

namespace OBJECT_DEFINDS {
    constexpr int MAX_USER          = 2500;
    constexpr int MAX_MONSER        = 400;
    constexpr int MAX_NPC           = 5;
    constexpr int MAX_GAMEOBJECT    = MAX_USER + MAX_MONSER + MAX_NPC;

    constexpr int CHARACTER_DOWN    = 0;
    constexpr int CHARACTER_LEFT    = 1;
    constexpr int CHARACTER_RIGHT   = 2;
    constexpr int CHARACTER_UP      = 3;

   /* enum TYPE {
        ME=1,
        OTHER_PLAYER,
        MONSTER,
    };*/
};


enum class PLAYER_ACTION {
    PA_ATTACK, PA_REQUEST_QUEST, PA_REQUEST_HEAL,
    PA_REQUEST_SHOP, PA_REQUEST_QUEST_CELAR,
};



constexpr int MAX_ID_SIZE = 10; // �� �ܼ�â �Է� �ϴ� ID



namespace MAP_DEFINDS {
    //MAP SIZE
    constexpr int WORLD_WIDTH       = 300;
    constexpr int WORLD_HEIGHT      = 300;
    constexpr int CELL_SIZE         = 15;
    constexpr int SECTOR_SIZE       = 30;                   //Navigation
    constexpr int SECTOR_X          = SECTOR_SIZE / 2;      //A* ���� �ڸ���
    constexpr int SECTOR_Y          = SECTOR_SIZE / 2;      //A* ���� �ڸ���
    constexpr int DIVISION_SECTOR   = 4;                    //Navigation
};

#pragma region serverinformation

#define SERVER_ENDPOINT  "127.0.0.1"
#define SERVER_PORT		 9000
#pragma endregion

#pragma region Server->Clinet
#define SC_LOGIN_OK			11
#define SC_LOGIN_FAIL		12
#define SC_ADD_OBJECT       13
#define SC_MOVE_OBJECT      14
#define SC_REMOVE_OBJECT    15
#define SC_HIT_OBJECT       16

#pragma pack(push ,1)
struct sc_packet_login_ok {
    PacketSize      size;
    PacketType      type;
    ObjectIDType    id;
    PositionType	x;
    PositionType    y;
    //int	HP, LEVEL, EXP;
    //int quest_step, quest_killed_monster;
};

struct sc_packet_add_object {
    PacketSize              size;
    PacketType              type;
    ObjectIDType            addID;
    PositionType            x;
    PositionType            y;
   // TextureDirection        textureDirection;
};

struct sc_packet_move_object {
    PacketSize	            size;
    PacketType	            type;
    ObjectIDType            movedID;
    PositionType            x;
    PositionType            y;
    TextureDirection        textureDirection;
};

struct sc_packet_remove_object {
    PacketSize              size;
    PacketType              type;
    ObjectIDType            removeID;
};

struct sc_packet_hit_object {
    PacketSize              size;
    PacketType              type;
    HpType                  hp;
    ObjectIDType            hitID;                       //������ Object ID
    TextureDirection        hitObjectTextureDirection;   //������ Object�� Texture ����
    ObjectIDType            attackID;                    //���ݴ��� Object ID
};

#pragma pack (pop)
#pragma endregion

#pragma region Client->Server
#define  CS_LOGIN		    21
#define  CS_MOVE_OBJECT	    22
#define  CS_ACTION		    23
#define  CS_HIT_OBJECT      24


#pragma pack(push ,1)
struct cs_packet_login {
    char	size;
    char	type;
    wchar_t player_id[MAX_ID_SIZE]; // DB�� �ִ� ID��ȸ��
    bool is_user;
};

struct cs_packet_move_object {
    PacketSize	            size;
    PacketType	            type;
    PositionType            x;
    PositionType            y;
    TextureDirection        textureDirection;
};

struct cs_packet_player_action {
    char size;
    char type;
    PLAYER_ACTION action_type;
};
#pragma pack (pop)


#pragma endregion


#pragma endregion












//
//
//enum MESSAGE_TYPE { MT_ATTACK_PLAYER = 1, MT_DESTROY_MONSTER, MT_ATTATK_MONSTER };
//
//enum REQUST_TYPE {
//    CS_QUEST = 1, CS_HEAL, CS_SHOP, CS_QUEST_REWARD,
//    SC_QUEST, SC_HEAL, SC_SHOP, SC_QUEST_CLEAR, SC_QUEST_REWARD,
//    REQUEST_TYPE_NULL
//};
//
//
//
////���Ŀ� ���� �ٲ����
////�ٲܽ� ������ �� �� �ִ� �κ� ����Ʈ
//constexpr int MAX_STR_LEN = 50;
//
//
////NPC ��ġ �ϵ��ڵ�
//constexpr int QUEST_NPC_X = 5;
//constexpr int QUEST_NPC_Y = 3;
//
//constexpr int HEAL_NPC_X = 11;
//constexpr int HEAL_NPC_Y = 2;
//
//constexpr int SHOP_NPC_X = 14;
//constexpr int SHOP_NPC_Y = 3;




//
//#define CS_CHAT			4
//#define CS_LOGOUT		5
//
//
//#define SC_POSITION			3
//#define SC_CHAT				4
//#define SC_STAT_CHANGE		5
//#define SC_REMOVE_OBJECT	6
//#define SC_MONSTER_INFO     8
//#define SC_CHAT_NOTICE      9
//#define SC_REQUEST_NPC       10
//#pragma pack(push ,1)
//struct sc_packet_login_fail {
//    char size;
//    char type;
//};
//struct sc_packet_chat {
//    char size;
//    char type;
//    int	id;
//    wchar_t	message[MAX_STR_LEN];
//};
//struct sc_packet_stat_change {
//    char size;
//    char type;
//    int	id;
//    int	HP, LEVEL, EXP;
//};
//
//struct sc_packet_monsterInfo {
//    char size;
//    char type;
//    int id;
//    int level;
//    int hp;
//    int x, y;
//    wchar_t name[MAX_STR_LEN];
//};
//
//struct sc_packet_chat_notice {
//    char size;
//    char type;
//    int monster_id;
//    MESSAGE_TYPE message_type;
//};
//struct sc_packet_request_npc {
//    char	size;
//    char	type;
//    REQUST_TYPE requst_type;
//    int quest_step, quest_killed_monster;
//};


//////////////////////////////////////
//////////Client------->Server////////
//////////////////////////////////////

//
//struct cs_packet_chat {
//    char	size;
//    char	type;
//    wchar_t	message[MAX_STR_LEN];
//};
//struct cs_packet_logout {
//    char	size;
//    char	type;
//};
//
//#pragma pack (pop)


