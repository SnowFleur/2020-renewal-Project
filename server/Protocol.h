#pragma once

#include"DataType.h"


/*리뉴얼에서 새로 추가 및 기존꺼 다시 올려서 정리하기*/
#pragma region NewDefineds

#define	WM_SOCKET				WM_USER + 1
#define BUF_SIZE		        1024

namespace OBJECT_DEFINDS {
    constexpr int   MAX_USER           = 2500;
    constexpr int   MAX_MONSER         = 400;
    constexpr int   MAX_NPC            = 5;
    constexpr int   MAX_GAMEOBJECT     = MAX_USER + MAX_MONSER + MAX_NPC;
    constexpr short NOT_USED_OBJID     = -9999;
    constexpr int   CHARACTER_DOWN     = 0;
    constexpr int   CHARACTER_LEFT     = 1;
    constexpr int   CHARACTER_RIGHT    = 2;
    constexpr int   CHARACTER_UP       = 3;
};


enum class PLAYER_ACTION {
    PA_ATTACK, PA_REQUEST_QUEST, PA_REQUEST_HEAL,
    PA_REQUEST_SHOP, PA_REQUEST_QUEST_CELAR,
};

constexpr int MAX_ID_SIZE = 10; // 그 콘솔창 입력 하는 ID

namespace MAP_DEFINDS {
    //MAP SIZE
    constexpr int WORLD_WIDTH       = 300;
    constexpr int WORLD_HEIGHT      = 300;
    constexpr int CELL_SIZE         = 15;
    constexpr int SECTOR_SIZE       = 30;                   //Navigation
    constexpr int SECTOR_X          = SECTOR_SIZE / 2;      //A* 범위 자르기
    constexpr int SECTOR_Y          = SECTOR_SIZE / 2;      //A* 범위 자르기
    constexpr int DIVISION_SECTOR   = 4;                    //Navigation
};

#pragma region serverInformation

#define SERVER_ENDPOINT          "127.0.0.1"
#define MAIN_SERVER_PORT		 9000
#define DB_SERVER_PORT           MAIN_SERVER_PORT+1
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
    ObjectIDType            hitID;                     //공격당한 Object ID
    TextureDirection        attackDirection;           //공격한   Object의 Texture 방향
    ObjectIDType            attackID;                  //공격한   Object ID
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
    wchar_t player_id[MAX_ID_SIZE]; // DB에 있는 ID조회용
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






#pragma region DB->Server
#define DS_DB_CONNECT_TO_SERVER 1

struct ds_packet_connectDB {
    PacketSize  size;
    PacketType  type;
};
#pragma endregion



#pragma region Server->DB

#pragma endregion





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


