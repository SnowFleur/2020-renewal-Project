#include"Monster.h"
#include<iostream>


CMonster::CMonster(MonsterType type, const PositionType x, const PositionType y, const HpType hp,
    const LevelType level, const ExpType exp, const DamageType damage)
    :monsterType_(type), GameObject{ x,y,hp,level,exp,damage } {

    //Lua �ʱ�ȭ
    luaState_ = luaL_newstate(); //����ӽ��� ���� �������ش�.
    //ǥ�ض��̺귯���� �־��ش�.
    luaL_openlibs(luaState_); //����ӽſ� �־��ش�.
    //Lua �Լ� �ε�
    if (luaL_loadfile(luaState_, "Monster.lua") != 0) {
        lua_error(luaState_, "error luaL_loadfile: %s\n", lua_tostring(luaState_, -1));
    }

    //ó�� �ʱ�ȭ�� �� ������Ѵ�
    lua_pcall(luaState_, 0, 0, 0);

    lua_register(luaState_, "API_GetMonsterInfor", CMonster::API_GetMonsterInfor);


    //Json���� ���� ������ �о�� Lua�� ����
    lua_getglobal(luaState_, "SetMonsterInfor");
    lua_pushnumber(luaState_, 0); //ID
    lua_pushnumber(luaState_, x); //X
    lua_pushnumber(luaState_, y); //Y
    lua_pushstring(luaState_, "Orc"); //name
    lua_pushnumber(luaState_, type); //type
    lua_pushnumber(luaState_, level); //level
    lua_pushnumber(luaState_, hp); //hp
    lua_pushnumber(luaState_, damage); //damage
    lua_pushnumber(luaState_, exp); //damage

    if (0 != lua_pcall(luaState_, 9, 0, 0)) //�Ķ���� 1, ���� 0
        lua_error(luaState_, "error SetMonsterInfor: %s\n", lua_tostring(luaState_, -1));
    lua_pop(luaState_, 9);

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

void CMonster::MoveMonster() {
    //�ϴ��� ���� ���
    lua_getglobal(luaState_, "GetMonsterInfor");
    if (0 !=lua_pcall(luaState_, 0, 0, 0)) //�Ķ���� 1, ���� 0
        lua_error(luaState_, "error GetMonsterInfor: %s\n", lua_tostring(luaState_, -1));

    lua_pop(luaState_, 0);

}


//���߿� Lua�� �����ҵ�
int CMonster::API_SendMoveMonster(lua_State* L) {
    return 0;
}


int CMonster::API_GetMonsterInfor(lua_State* L) {

    ObjectIDType monsterID      = (int)lua_tonumber(L, -9);
    PositionType monsterX       = (int)lua_tonumber(L, -8);
    PositionType monsterY       = (int)lua_tonumber(L, -7);
    char* monsterName           = (char*)lua_tostring(L, -6);
    MonsterType monsterType     = (MonsterType)lua_tonumber(L, -5);
    LevelType monsterLevel      = (int)lua_tonumber(L, -4);
    HpType monsterHp            = (int)lua_tonumber(L, -3);
    DamageType monsterDamage    = (int)lua_tonumber(L, -2);
    ExpType monsterexp          = (int)lua_tonumber(L, -1);


    std::cout <<"Lua:"<< "ID: " << monsterID << " HP: " << (int)monsterHp
        << " level: " << (int)monsterLevel <<" EXP: " << (int)monsterexp
        << " Damage: " << (int)monsterDamage << "\n";

    return 0;
}
