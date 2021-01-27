#pragma once

#include<chrono>
#include"DataType.h"
using namespace std::chrono;


enum EVENT_TYPE {
	EV_EXCUTE_MONSTER,

    //EV_PLAYER_ATTACK, EV_PLAYER_MOVE,
    //EV_LOGIN_REQUEST, EV_LOGIN_OK, EV_LOGOUT, EV_LOGIN_FALSE,
    //EV_PLAYER_MOVE_DETECT, EV_PLAYER_ATTACK_MONSER, EV_MONSER_ATTACK_PLAYER,
    //EV_FOLLOWER_PLAYER, EV_FOLLOWER_PLAYER_DETECT, EV_HEAL, EV_REVIVE,
};

struct EVENT_ST {
	ObjectIDType	obj_id;			//몬스터 객체(ID)
	ObjectIDType	target_id;		// 타겟 ID(플레이어)
	EVENT_TYPE		type;			// 이벤트 타입

	high_resolution_clock::time_point start_time;

	//20.11.09: 일단 살려두고 지금하는 방식이 실패라면 다시 롤백
	//SOCKET		socket;		//Network 함수에 전달하기 위한 SOCKET(플레이어) 

	constexpr bool operator<(const EVENT_ST& lhs)const {
		return start_time > lhs.start_time;
	}

	EVENT_ST() {}
	EVENT_ST(ObjectIDType obj_id, ObjectIDType target_id,
		EVENT_TYPE et, high_resolution_clock::time_point time) {
		this->obj_id = obj_id;
		this->target_id = target_id;
		this->type = et;
		this->start_time = time;
	}
};


struct EVENT_DB {
	int id; //어느 플레이어인가
	EVENT_TYPE type; //이벤트 타입
	wchar_t* db_id;
	int x, y;
	int hp, level, exp, quest_step, quest_killed_monster;
	bool login;
};

