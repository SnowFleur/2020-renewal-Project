#pragma once

#include"WinSocketHeader.h"
#include"EventType.h"

#define MAX_BUFFER        1024
#define VIEW_NPC	      2

enum class IO_EVENT {
	IO_ACCEPT,
	IO_RECV,
	IO_SEND,
	IO_MONSTER_EVENT,
};

struct OverEx {
public:
	WSAOVERLAPPED	over_;
	SOCKET			socket_;	//accept
	WSABUF			dataBuffer_;
	char			messageBuffer_[MAX_BUFFER];
	IO_EVENT		ioEvent_;

	//필요한가?
	ObjectIDType	target_player_; //어떤 플레이어가 이동을 했는가.

	OverEx() {
		ZeroMemory(&over_, sizeof(over_));
		ZeroMemory(&messageBuffer_, MAX_BUFFER);
		dataBuffer_.buf = messageBuffer_;
		dataBuffer_.len = MAX_BUFFER;
	}

	void InitOverEx() {
		ZeroMemory(&over_, sizeof(over_));
		ZeroMemory(&messageBuffer_, MAX_BUFFER);
		dataBuffer_.buf = messageBuffer_;
		dataBuffer_.len = MAX_BUFFER;
	}
};


