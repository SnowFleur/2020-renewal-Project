#pragma once
#pragma comment(lib, "lua53.lib")
#include"GameObject.h"

extern "C" {
#include"include/lua.h"
#include"include/lauxlib.h"
#include"include/lualib.h"
}

constexpr PositionType PRIMARY_MONSTER_X = 3;
constexpr PositionType PRIMARY_MONSTER_Y = 5;

enum MonsterType {
    ORC=1,ZOMBIE,MUMMY,BAT
};

class CMonsterInputComponent;
class CPlayer;

class CMonster :public GameObject {
private:
public:
    MonsterType             monsterType_; 
    lua_State*              luaState_;
    CMonsterInputComponent* inputcomponent_;
    TextureDirection        diretion_;

    CMonster(MonsterType type,const PositionType x,const PositionType y,const HpType hp,
        const LevelType level,const ExpType exp,const DamageType damage);

    CMonster() = delete;
    CMonster(const CMonster&) = delete;
    CMonster(CMonster&&) = delete;
    CMonster operator=(const CMonster&) = delete;
    CMonster operator=(CMonster&&) = delete;
    ~CMonster();

    //잠재적 오류가 보임(멀티스레드)
    void MoveMonster(CPlayer& player);


    void lua_error(lua_State* L, const char* fmt, ...);

    //Lua Function
    //int API_GET_X(lua_State*);
    //int API_GET_Y(lua_State*);
    //int API_add_timer(lua_State* L);

    static int API_SendMoveMonster(lua_State* L);
    static int API_GetMonsterInfor(lua_State* L);
};

