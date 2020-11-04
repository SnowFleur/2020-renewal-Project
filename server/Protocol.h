#pragma once

#include"DataType.h"


/*리뉴얼에서 새로 추가 및 기존꺼 다시 올려서 정리하기*/
#pragma region NewDefineds

#define	WM_SOCKET				WM_USER + 1
#define VIEW_RADIUS		        10
#define BUF_SIZE		        1024

namespace OBJECT_DEFINDS {
    //Clinet에서는 하나의 객체로 하기 때문에 2500씩 나눔
    constexpr int MAX_USER          = 2500;
    constexpr int MAX_MONSER        = 400;
    constexpr int MAX_NPC           = 5;
    constexpr int MAX_GAMEOBJECT    = MAX_USER + MAX_MONSER + MAX_NPC;

    enum TYPE {
        OTHER_PLAYER = 1,
        MONSTER,
    };
};


enum class PLAYER_ACTION {
    PA_ATTACK, PA_REQUEST_QUEST, PA_REQUEST_HEAL,
    PA_REQUEST_SHOP, PA_REQUEST_QUEST_CELAR,
};



constexpr int MAX_ID_SIZE = 10; // 그 콘솔창 입력 하는 ID



namespace MAP_DEFINDS {
    //MAP SIZE
    constexpr int WORLD_WIDTH = 300;
    constexpr int WORLD_HEIGHT = 300;
    constexpr int MAP_SIZE = 30;
    constexpr int CELL_SIZE = 5;
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
    ObjectClass             objectClass; // 1: PLAYER,    2:ORC,  3:Dragon, …..
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
    char size;
    char type;
    int id;
};
#pragma pack (pop)
#pragma endregion

#pragma region Client->Server
#define  CS_LOGIN		    21
#define  CS_MOVE_OBJECT	    22
#define  CS_ACTION		    23

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
////초후에 갯수 바꿔야함
////바꿀시 문제가 될 수 있는 부분 퀘스트
//constexpr int MAX_STR_LEN = 50;
//
//
////NPC 위치 하드코딩
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


/*
인수인계는 항상 프로토콜.h 에 쓰기로 하자 (20.07.20)

크게 남은 것들
1. 맵 배치
2. NPC 배치
3. 채팅 창 및 Notice 창 복구
4. 몬스터 배치


4. 현재 global에 있는 directX파일을 전부 인클루드 하고있는데 따로 빼던지 해야할것으로 보임


20.07.26

3. NPC 생성과 배치
4. NPC와 상호작용

6. 통신 한번 해보기

7. Monster 하위 Class의 필요성이 보임
7-1 아니면 Monster를 지우고 Zombie, 등등 Class가 필요해보임

8. Spawner 의 필요성??


Guide Line

NPC 생성과 배치 그리고 맵 배치 부터 끝내고 Monster 마지막은 Notice 창

20.07.31

1. NPC 배치 및 맵 구성
2. 맵 사이즈 크기 증가 및 카메라 조정

20.08.02
0. 경량패턴 정상작동 확인 및 퍼포먼스 차이 체크( 체크 후에 기존 함수 지울것 꼭!)
1. 일단 Player는 CGameObjectTexture에서 실행되는 것을 확인했으니 나머지 Mosnter와 NPC를 처리할것
2. 기존에 있던 Texture함수는 그냥 납두는걸로 하고 Cycle에 따른 변경이 가능하도록 유도
2-1 그래픽컴포넌트 안에 있는 것을 포인터로 바꾸고 Cycle만큼 동적할당하고 Delete 하던지 SmartPointer 쓰던지
3. GenetrateSprite는 삭제는 생각해보고 지우자 이걸 지우면 기존 SetCharacterTexture함수의 존재의의가 사라짐
4. GameObjectTexture 이름 바꾸기 Spanwer 같은 느낌으로 바꾸기


// 해당내용 블로그 업로드
1. 경량패턴과 Sector를 이용한 퍼포먼스 차이 올려보자
1. 일단 경량패턴은 확실히 로딩속도가 매우 빠름
1. 5천개의 객체가 있을 떄 이 모든 객체를 렌더링 할 필요는 없으니
1. Sector에 있는 객체만 렌더링 할 수 있도록 해보자
1. 끝나면 함수 지우던 알아서

일단 경량패턴 적용은한듯 과거의 내가

- 정상 작동하면 그 때 맵 배치 시작


20.08.13
2. 그렇게 해서 채팅창 최적화해서 만들고 채팅서버를 제작하자 이제
3. 채팅서버 만들고 캐릭터 나오는거 보고

// Last Update
1. 시청다녀와서 이미지 두개 좀더 비교해보고 알파값 조절
2. 입력가능하도록 조정
3. BITMAP을 어디서 관리할지 생각해보자(UI는 UI에서) 생각해보자..
4. 엔터치면 입력내용을 캐릭터 머리위에 나오게 하고 글자수 제한 때리고 하면 이제 서버시작



2020. 11. 02

AI 처리

1. Timer Thread PQCS 이용 등 기반 틀 작성
2. Lua Scrpit 삽입 및 작동 확인
3. AI 작동 확인
4. A* 길 찾기 코드 및 지형데이터(Navigation Mesh)삽입



5. Send Memory Pool 이상 없어보임 대신 AI 넣고 동시테스트 달리면 Lock의 필요성 보임

*/