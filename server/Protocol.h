#pragma once

#include"DataType.h"


/*�����󿡼� ���� �߰� �� ������ �ٽ� �÷��� �����ϱ�*/
#pragma region NewDefineds

#define	WM_SOCKET				WM_USER + 1




/*
20.10.25
Client�� �ϳ��� Object���� ����
*/
namespace OBJECT_DEFINDS {
    constexpr int MAX_USER = 2500;
    constexpr int MAX_MONSER = 2500;
    constexpr int MAX_NPC = 5;

    enum TYPE {
        PLAYER = 1,
        MONSTER = 2,
    };
}


enum class PLAYER_ACTION {
    PA_ATTACK, PA_REQUEST_QUEST, PA_REQUEST_HEAL,
    PA_REQUEST_SHOP, PA_REQUEST_QUEST_CELAR,
};

// 0:Down, 1:Left, 2:Right, 3:Up
constexpr int CHARACTER_DOWN = 0;
constexpr int CHARACTER_LEFT = 1;
constexpr int CHARACTER_RIGHT = 2;
constexpr int CHARACTER_UP = 3;

/* 500������ User, 501���ʹ� AI */
constexpr int MAX_GAMEOBJECT = 5000;

constexpr int MAX_ID_SIZE = 10; // �� �ܼ�â �Է� �ϴ� ID



namespace MAP_DEFINDS {
    //MAP SIZE
    constexpr int WORLD_WIDTH = 300;
    constexpr int WORLD_HEIGHT = 300;
    constexpr int MAP_SIZE = 30;
    constexpr int CELL_SIZE = 5;
}



#pragma region serverinformation

#define SERVER_ENDPOINT  "127.0.0.1"
#define SERVER_PORT		 9000
#pragma endregion

#pragma region Server->Clinet
#define SC_LOGIN_OK			1
#define SC_LOGIN_FAIL		2
#define SC_ADD_OBJECT       3

struct sc_packet_add_object {
    PacketSize              size;
    PacketType              type;
    ObjectIDType            id;
    ObjectClass             objectClass; // 1: PLAYER,    2:ORC,  3:Dragon, ��..
    PositionType            x;
    PositionType            y;
    TextureDirection        textureDirection;
};

struct sc_packet_remove_object {
    char size;
    char type;
    int id;
};
#pragma endregion

#pragma region Client->Server
#define  CS_LOGIN		1
#define  CS_MOVE	    2
#define  CS_ACTION		3

struct cs_packet_login {
    char	size;
    char	type;
    wchar_t player_id[MAX_ID_SIZE]; // DB�� �ִ� ID��ȸ��
    bool is_user;
};

struct cs_packet_move {
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

#pragma endregion


#pragma endregion


















enum MESSAGE_TYPE { MT_ATTACK_PLAYER = 1, MT_DESTROY_MONSTER, MT_ATTATK_MONSTER };

enum REQUST_TYPE {
    CS_QUEST = 1, CS_HEAL, CS_SHOP, CS_QUEST_REWARD,
    SC_QUEST, SC_HEAL, SC_SHOP, SC_QUEST_CLEAR, SC_QUEST_REWARD,
    REQUEST_TYPE_NULL
};



//���Ŀ� ���� �ٲ����
//�ٲܽ� ������ �� �� �ִ� �κ� ����Ʈ
constexpr int MAX_STR_LEN = 50;


//NPC ��ġ �ϵ��ڵ�
constexpr int QUEST_NPC_X = 5;
constexpr int QUEST_NPC_Y = 3;

constexpr int HEAL_NPC_X = 11;
constexpr int HEAL_NPC_Y = 2;

constexpr int SHOP_NPC_X = 14;
constexpr int SHOP_NPC_Y = 3;



#define VIEW_RADIUS		10
#define BUF_SIZE		1024


#define CS_CHAT			4
#define CS_LOGOUT		5


#define SC_POSITION			3
#define SC_CHAT				4
#define SC_STAT_CHANGE		5
#define SC_REMOVE_OBJECT	6
#define SC_MONSTER_INFO     8
#define SC_CHAT_NOTICE      9
#define SC_REQUEST_NPC       10
#pragma pack(push ,1)

struct sc_packet_login_ok {
    char size;
    char type;
    int id;
    short	x, y;
    int	HP, LEVEL, EXP;
    int quest_step, quest_killed_monster;
};
struct sc_packet_login_fail {
    char size;
    char type;
};
struct sc_packet_chat {
    char size;
    char type;
    int	id;
    wchar_t	message[MAX_STR_LEN];
};
struct sc_packet_stat_change {
    char size;
    char type;
    int	id;
    int	HP, LEVEL, EXP;
};

struct sc_packet_monsterInfo {
    char size;
    char type;
    int id;
    int level;
    int hp;
    int x, y;
    wchar_t name[MAX_STR_LEN];
};
struct sc_packet_position {
    char size;
    char type;
    int id;
    short x, y;
    unsigned char renderCharacterDirection;
};
struct sc_packet_chat_notice {
    char size;
    char type;
    int monster_id;
    MESSAGE_TYPE message_type;
};
struct sc_packet_request_npc {
    char	size;
    char	type;
    REQUST_TYPE requst_type;
    int quest_step, quest_killed_monster;
};


//////////////////////////////////////
//////////Client------->Server////////
//////////////////////////////////////


struct cs_packet_chat {
    char	size;
    char	type;
    wchar_t	message[MAX_STR_LEN];
};
struct cs_packet_logout {
    char	size;
    char	type;
};

#pragma pack (pop)


/*
�μ��ΰ�� �׻� ��������.h �� ����� ���� (20.07.20)

ũ�� ���� �͵�
1. �� ��ġ
2. NPC ��ġ
3. ä�� â �� Notice â ����
4. ���� ��ġ


4. ���� global�� �ִ� directX������ ���� ��Ŭ��� �ϰ��ִµ� ���� ������ �ؾ��Ұ����� ����


20.07.26

3. NPC ������ ��ġ
4. NPC�� ��ȣ�ۿ�

6. ��� �ѹ� �غ���

7. Monster ���� Class�� �ʿ伺�� ����
7-1 �ƴϸ� Monster�� ����� Zombie, ��� Class�� �ʿ��غ���

8. Spawner �� �ʿ伺??


Guide Line

NPC ������ ��ġ �׸��� �� ��ġ ���� ������ Monster �������� Notice â

20.07.31

1. �浹�� ���� ������ ����
2. �浹�� ���� ��Ŷ����
// �ؾ��Ұ�
1. NPC ��ġ �� �� ����
2. �� ������ ũ�� ���� �� ī�޶� ����

20.08.02
0. �淮���� �����۵� Ȯ�� �� �����ս� ���� üũ( üũ �Ŀ� ���� �Լ� ����� ��!)
1. �ϴ� Player�� CGameObjectTexture���� ����Ǵ� ���� Ȯ�������� ������ Mosnter�� NPC�� ó���Ұ�
2. ������ �ִ� Texture�Լ��� �׳� ���δ°ɷ� �ϰ� Cycle�� ���� ������ �����ϵ��� ����
2-1 �׷���������Ʈ �ȿ� �ִ� ���� �����ͷ� �ٲٰ� Cycle��ŭ �����Ҵ��ϰ� Delete �ϴ��� SmartPointer ������
3. GenetrateSprite�� ������ �����غ��� ������ �̰� ����� ���� SetCharacterTexture�Լ��� �������ǰ� �����
4. GameObjectTexture �̸� �ٲٱ� Spanwer ���� �������� �ٲٱ�


// �ش系�� ��α� ���ε�
1. �淮���ϰ� Sector�� �̿��� �����ս� ���� �÷�����
1. �ϴ� �淮������ Ȯ���� �ε��ӵ��� �ſ� ����
1. 5õ���� ��ü�� ���� �� �� ��� ��ü�� ������ �� �ʿ�� ������
1. Sector�� �ִ� ��ü�� ������ �� �� �ֵ��� �غ���
1. ������ �Լ� ����� �˾Ƽ�

�ϴ� �淮���� �������ѵ� ������ ����

- ���� �۵��ϸ� �� �� �� ��ġ ����


20.08.13
2. �׷��� �ؼ� ä��â ����ȭ�ؼ� ����� ä�ü����� �������� ����
3. ä�ü��� ����� ĳ���� �����°� ����

// Last Update
1. ��û�ٳ�ͼ� �̹��� �ΰ� ���� ���غ��� ���İ� ����
2. �Է°����ϵ��� ����
3. BITMAP�� ��� �������� �����غ���(UI�� UI����) �����غ���..
4. ����ġ�� �Է³����� ĳ���� �Ӹ����� ������ �ϰ� ���ڼ� ���� ������ �ϸ� ���� ��������



20.10.25


��ó�� ���� AI�� �Ȱ��� �ൿ���״ϱ� �÷��̾� ���忡���� ������ �� AI�ϱ�
���� ������Ʈ�� �ھƹ�����





*/