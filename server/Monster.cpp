#include"Monster.h"


CMonster::CMonster(MonsterType type, const PositionType x, const PositionType y, const HpType hp,
    const LevelType level, const ExpType exp, const DamageType damage)
    :monsterType_(type), GameObject{ x,y,hp,level,exp,damage } {

    //Lua �ʱ�ȭ
    luaState_ = luaL_newstate(); //����ӽ��� ���� �������ش�.
    //ǥ�ض��̺귯���� �־��ش�.
    luaL_openlibs(luaState_); //����ӽſ� �־��ش�.
    //Lua �Լ� �ε�
    luaL_loadfile(luaState_, "Monster.lua");

    //Json���� ���� ������ �о�� Lua�� ����
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
