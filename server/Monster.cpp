#include"Monster.h"


CMonster::CMonster(MonsterType type, const PositionType x, const PositionType y, const HpType hp,
    const LevelType level, const ExpType exp, const DamageType damage)
    :monsterType_(type), GameObject{ x,y,hp,level,exp,damage } {

    //Lua 초기화
    luaState_ = luaL_newstate(); //가상머신을 만들어서 리턴해준다.
    //표준라이브러리를 넣어준다.
    luaL_openlibs(luaState_); //가상머신에 넣어준다.
    //Lua 함수 로드
    luaL_loadfile(luaState_, "Monster.lua");

    //Json에서 몬스터 정보를 읽어와 Lua에 저장
    lua_getglobal(luaState_, "SetMonsterInfor");

    lua_register(luaState_, "API_SendMoveMonster", CMonster::API_SendMoveMonster);

}
 
CMonster::~CMonster() {
    //Lua 가상머신 해제
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
