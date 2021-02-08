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

	//WorkThread에서 전달해줄 구조체
	EVENT_ST		ev;

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


