#pragma once

#include"WinSocketHeader.h"
#include"EventType.h"

#define MAX_BUFFER        1024
#define VIEW_SIZE		  12
#define VIEW_NPC	      2


struct OverEx {
public:
	WSAOVERLAPPED	over_;
	SOCKET			socket_;	//accept
	WSABUF			dataBuffer_;
	char			messageBuffer_[MAX_BUFFER];
	EVENT_TYPE		ev_;
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


