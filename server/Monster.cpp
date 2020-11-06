#include"Monster.h"


int Test(lua_State*);

CMonster::CMonster(MonsterType type) :monsterType_(type) {

    //Lua �ʱ�ȭ
    luaState_ = luaL_newstate(); //����ӽ��� ���� �������ش�.
    //ǥ�ض��̺귯���� �־��ش�.
    luaL_openlibs(luaState_); //����ӽſ� �־��ش�.

    luaL_loadfile(luaState_, "Monster.lua");
    lua_getglobal(luaState_, "SetMonsterInfor");

    lua_register(luaState_, "API_SendMoveMonster", CMonster::API_SendMoveMonster);

}
 
CMonster::~CMonster() {
    //Lua ����ӽ� ����
    lua_close(luaState_);
}

void CMonster::lua_error(lua_State* L, const char* fmt, ...) {
    va_list argp;
    va_start(argp, fmt);
    vfprintf(stderr, fmt, argp);
    va_end(argp);
    lua_close(L);
    system("pause");
    exit(EXIT_FAILURE);

}

int CMonster::API_SendMoveMonster(lua_State* L) {



    return 1;

}
