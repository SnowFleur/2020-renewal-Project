#include"Monster.h"
#include"MonsterInputComponent.h"
#include"Protocol.h"

CMonster::CMonster(MonsterType type, const PositionType x, const PositionType y, const HpType hp,
    const LevelType level, const ExpType exp, const DamageType damage)
    :monsterType_(type), inputcomponent_{ nullptr },diretion_{ OBJECT_DEFINDS ::CHARACTER_DOWN},
    GameObject{ PRIMARY_MONSTER_X,PRIMARY_MONSTER_Y,hp,level,exp,damage }{

    inputcomponent_ = new CMonsterInputComponent();

    
    //Lua 초기화
    //luaState_ = luaL_newstate(); //가상머신을 만들어서 리턴해준다.
    ////표준라이브러리를 넣어준다.
    //luaL_openlibs(luaState_); //가상머신에 넣어준다.
    ////Lua 함수 로드
    //if (luaL_loadfile(luaState_, "Monster.lua") != 0) {
    //    lua_error(luaState_, "error luaL_loadfile: %s\n", lua_tostring(luaState_, -1));
    //}
    //
    ////lua 스크립트 실행
    //lua_pcall(luaState_, 0, 0, 0);

    ////루아 함수 등록
    //lua_register(luaState_, "API_GetMonsterInfor", CMonster::API_GetMonsterInfor);
    //lua_register(luaState_, "API_SendMoveMonster", CMonster::API_SendMoveMonster);

    ////Json에서 몬스터 정보를 읽어와 Lua에 저장
    //lua_getglobal(luaState_, "SetMonsterInfor");
    //lua_pushinteger(luaState_, 0); //ID
    //lua_pushinteger(luaState_, x); //X
    //lua_pushinteger(luaState_, y); //Y
    //lua_pushstring(luaState_, "Orc"); //name
    //lua_pushinteger(luaState_, type); //type
    //lua_pushinteger(luaState_, level); //level
    //lua_pushinteger(luaState_, hp); //hp
    //lua_pushinteger(luaState_, damage); //damage
    //lua_pushinteger(luaState_, exp); //damage

    //if (0 != lua_pcall(luaState_, 9, 0, 0)) //파라미터 1, 리턴 0
    //    lua_error(luaState_, "error SetMonsterInfor: %s\n", lua_tostring(luaState_, -1));
}


CMonster::~CMonster() {

    if (inputcomponent_ != nullptr) {
        delete inputcomponent_;
    }
    //Lua 가상머신 해제
    lua_close(luaState_);
}

void CMonster::MoveMonster(CPlayer& player) {
    
    //2020.11.12: 여기에 들어간 플레이어 정보가 A*를 진행하는 플레이어
    inputcomponent_->State(*this,player);


    //lua_getglobal(luaState_, "GetMonsterInfor"); //스택에 함수 푸시
    //if (0 !=lua_pcall(luaState_, 0, 0, 0)) //함수 실행
    //    lua_error(luaState_, "error GetMonsterInfor: %s\n", lua_tostring(luaState_, -1));
}



#pragma region Temp
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
    return 0;
}
int CMonster::API_GetMonsterInfor(lua_State* L) {

    ObjectIDType monsterID      = lua_tointeger(L, -9);
    PositionType monsterX       = lua_tointeger(L, -8);
    PositionType monsterY       = lua_tointeger(L, -7);
    char* monsterName           = (char*)lua_tostring(L, -6);
    MonsterType monsterType     = (MonsterType)lua_tointeger(L, -5);
    LevelType monsterLevel      = lua_tointeger(L, -4);
    HpType monsterHp            = lua_tointeger(L, -3);
    DamageType monsterDamage    = lua_tointeger(L, -2);
    ExpType monsterexp          = lua_tointeger(L, -1);
    return 0;
}
#pragma endregion









