#pragma once

#include<chrono>
using namespace std::chrono;


enum EVENT_TYPE {
	EV_MOVE = 1, EV_RECV, EV_SEND, EV_ACCEPT, //���ݱ��� ���� �ִ� EV

	EV_PLAYER_ATTACK, EV_PLAYER_MOVE,
	EV_LOGIN_REQUEST, EV_LOGIN_OK, EV_LOGOUT, EV_LOGIN_FALSE,
	EV_PLAYER_MOVE_DETECT, EV_PLAYER_ATTACK_MONSER, EV_MONSER_ATTACK_PLAYER,
	EV_FOLLOWER_PLAYER, EV_FOLLOWER_PLAYER_DETECT, EV_HEAL, EV_REVIVE,
};

struct EVENT_ST {
	int obj_id; //� ��ü�ΰ�
	int target_id; //Ÿ��
	//BYTE dir;     //����
	EVENT_TYPE type; //�̺�Ʈ Ÿ��

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
	int id; //��� �÷��̾��ΰ�
	EVENT_TYPE type; //�̺�Ʈ Ÿ��
	wchar_t* db_id;
	int x, y;
	int hp, level, exp, quest_step, quest_killed_monster;
	bool login;
};
