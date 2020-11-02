#pragma once

#include<chrono>
using namespace std::chrono;


enum EVENT_TYPE {
	EV_MOVE = 1, EV_RECV, EV_SEND, EV_ACCEPT, //지금까지 쓰고 있는 EV

	EV_PLAYER_ATTACK, EV_PLAYER_MOVE,
	EV_LOGIN_REQUEST, EV_LOGIN_OK, EV_LOGOUT, EV_LOGIN_FALSE,
	EV_PLAYER_MOVE_DETECT, EV_PLAYER_ATTACK_MONSER, EV_MONSER_ATTACK_PLAYER,
	EV_FOLLOWER_PLAYER, EV_FOLLOWER_PLAYER_DETECT, EV_HEAL, EV_REVIVE,
};

struct EVENT_ST {
	int obj_id; //어떤 객체인가
	int target_id; //타겟
	//BYTE dir;     //방향
	EVENT_TYPE type; //이벤트 타입

	high_resolution_clock::time_point start_time;
	constexpr bool operator<(const EVENT_ST& lhs)const {
		return start_time > lhs.start_time;
	}

	EVENT_ST(int obj_id, EVENT_TYPE et, high_resolution_clock::time_point time) {
		this->obj_id = obj_id;
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
