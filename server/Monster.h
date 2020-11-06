#pragma once
#pragma comment(lib, "lua53.lib")
#include"GameObject.h"

#define		_NODISCARD			[[nodiscard]]

extern "C" {
#include"include/lua.h"
#include"include/lauxlib.h"
#include"include/lualib.h"
}

enum MonsterType {
    ORC=1,ZOMBIE,MUMMY,BAT
};


class CMonster :public GameObject {
private:
public:
    MonsterType    monsterType_; //몬스터타입?
    lua_State*     luaState_;

    CMonster(MonsterType type);
    CMonster() = delete;
    ~CMonster();

    void lua_error(lua_State* L, const char* fmt, ...);
    //Lua Function
    //int API_GET_X(lua_State*);
    //int API_GET_Y(lua_State*);
    //int API_add_timer(lua_State* L);

    static int API_SendMoveMonster(lua_State* L);
};

