#pragma once

#include"WinSocketHeader.h"
#include"EventType.h"

#define MAX_BUFFER        1024
#define VIEW_SIZE		  7
#define VIEW_NPC	      2


struct OverEx {
public:
	WSAOVERLAPPED	over_;
	SOCKET			socket_;
	WSABUF			dataBuffer_;
	char			messageBuffer_[MAX_BUFFER];
	EVENT_TYPE		ev_;
	BYTE			dir_; // ĳ���� ����
	int				target_player_; //� �÷��̾ �̵��� �ߴ°�.

	OverEx() {
		ZeroMemory(&over_, sizeof(over_));
		ZeroMemory(&messageBuffer_, MAX_BUFFER);
		dataBuffer_.buf = messageBuffer_;
		dataBuffer_.len = MAX_BUFFER;
	}

};

/*
�̰� ����?
*/
struct OVER_EX_PLAYER {
	WSAOVERLAPPED  over;
	EVENT_TYPE		   ev;
	int target_player; //� �÷��̾ �̵��� �ߴ°�.
};

