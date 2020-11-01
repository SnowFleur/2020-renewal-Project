//// 서버는 64비트를 사용한다.
//
//
//#include"global.h"
//wchar_t* ConverCtoWC(char* str)
//{
//	//wchar_t형 변수 선언
//	wchar_t* pStr;
//	//멀티 바이트 크기 계산 길이 반환
//	int strSize = MultiByteToWideChar(CP_ACP, 0, str, -1, NULL, NULL);
//	//wchar_t 메모리 할당
//	pStr = new WCHAR[strSize];
//	//형 변환
//	MultiByteToWideChar(CP_ACP, 0, str, strlen(str) + 1, pStr, strSize);
//	return pStr;
//}
//
///******************************
//packet functios
//********************************/
//void send_packet(int client, void *packet) {
//	char *p = reinterpret_cast<char *> (packet);
//	OVER_EX *ov = new OVER_EX;
//
//	ov->dataBuffer.len = p[0];
//	ov->dataBuffer.buf = ov->messageBuffer;
//	ov->ev = EV_SEND;
//
//	// 보낼 데이터로 채우기
//	memcpy(ov->messageBuffer, p, p[0]);
//	ZeroMemory(&ov->over, sizeof(ov->over));
//
//	
//
//
//	if (client >= MAX_USER)
//		return;
//
//	int error = WSASend(clients[client].socket, &ov->dataBuffer, 1, 0, 0, &ov->over, NULL);
//	if (0 != error) {
//		if (WSAGetLastError() != WSA_IO_PENDING) {
//
//			std::cout << "오류 클라이언트 id" << client;
//
//			printf("Error - Fail WSAsend(error_code : %d)\n", WSAGetLastError());
//		}
//	}
//	else {
//		/*	cout << "Non Overlapped Send \n";
//			while (true);*/
//	}
//}
//void send_stat_change_packet(int id, int stat_chagne_id) {
//	sc_packet_stat_change packet;
//	packet.size = sizeof(packet);
//	packet.HP = clients[id].m_hp;
//	packet.LEVEL = clients[id].m_level;
//	packet.EXP = clients[id].m_exp;
//	packet.id = stat_chagne_id;
//	packet.type = SC_STAT_CHANGE;
//
//	send_packet(id, &packet);
//}
//void send_position_pakcet(int client, int player) {
//	sc_packet_position packet;
//
//
//	packet.id = player;
//	packet.size = sizeof(packet);
//	packet.type = SC_POSITION;
//	packet.x = clients[player].m_x;
//	packet.y = clients[player].m_y;
//	packet.character_shift = clients[player].m_character_shift;
//
//	send_packet(client, &packet);
//}
//void send_remove_player_packet(int client, int id) {
//	sc_packet_remove_object packet;
//
//	packet.id = id;
//	packet.size = sizeof(packet);
//	packet.type = SC_REMOVE_OBJECT;
//
//
//	send_packet(client, &packet);
//}
//void send_login_ok_packet(int new_id) {
//	sc_packet_login_ok packet;
//
//	packet.id = new_id;
//	packet.size = sizeof(packet);
//	packet.type = SC_LOGIN_OK;
//	packet.x = clients[new_id].m_x;
//	packet.y = clients[new_id].m_y;
//	packet.HP = clients[new_id].m_hp;
//	packet.LEVEL = clients[new_id].m_level;
//	packet.EXP = clients[new_id].m_exp;
//	packet.quest_step = clients[new_id].m_quest_step;
//	packet.quest_killed_monster = clients[new_id].m_quest_killed_monster;
//
//	send_packet(new_id, &packet);
//}
////void send_login_false_packet(int new_id) {
////	sc_packet_login_false packet;
////	packet.id = new_id;
////	packet.size = sizeof(packet);
////	packet.type = SC_LOGIN_FALSE;
////	send_packet(new_id, &packet);
////}
//void send_put_player_packet(int client, int new_id) {
//	sc_packet_add_object packet;
//
//	packet.id = new_id;
//	packet.size = sizeof(packet);
//	packet.type = SC_ADD_OBJECT;
//	packet.x = clients[new_id].m_x;
//	packet.y = clients[new_id].m_y;
//	send_packet(client, &packet);
//
//}
//void send_chat_packet(int client, int from_id, wchar_t* mess) {
//	sc_packet_chat packet;
//
//	packet.id = from_id;
//	packet.size = sizeof(packet);
//	packet.type = SC_CHAT;
//	wcscpy(packet.message, mess);
//	send_packet(client, &packet);
//}
//void send_npc(int client, int npc_id) {
//	sc_packet_position packet;
//	packet.id = npc_id;
//	packet.size = sizeof(packet);
//	packet.character_shift = clients[npc_id].m_character_shift;
//	packet.type = SC_POSITION;
//	packet.x = clients[npc_id].m_x;
//	packet.y = clients[npc_id].m_y;
//
//
//	send_packet(client, &packet);
//
//}
//void send_monsterInfo_packet(int id, int player_id, int level, int hp, wchar_t* name) {
//	sc_packet_monsterInfo packet;
//
//	packet.size = sizeof(packet);
//	packet.type = SC_MONSTER_INFO;
//	packet.id = id;
//	packet.level = level;
//	packet.hp = hp;
//	packet.x = clients[id].m_x;
//	packet.y = clients[id].m_y;
//
//	wcscpy(packet.name, name);
//	send_packet(player_id, &packet);
//}
//void send_request_npc(int id, REQUST_TYPE type) {
//	sc_packet_request_npc packet;
//	packet.size = sizeof(packet);
//	packet.type = SC_REQUEST_NPC;
//	packet.requst_type = type;
//	packet.quest_killed_monster = clients[id].m_quest_killed_monster;
//	packet.quest_step = clients[id].m_quest_step;
//	send_packet(id, &packet);
//}
///******************************
//npc functios
//********************************/
//void RandNPCMove(int npc_id) {
//	enum SHIFT {
//		AI_RIGHT = 0, AI_LEFT, AI_UP, AI_DOWN
//	};
//
//	int shift = rand() % 4;
//
//	int x = clients[npc_id].m_x;
//	int y = clients[npc_id].m_y;
//
//	//이동하기전에 old리스트를 만든다.
//	std::unordered_set <int> old_vl;
//	for (int i = 0; i < MAX_USER; i++) {
//		if (false == clients[i].in_used) continue;
//		if (false == GetNearObject(npc_id, i)) continue;
//		old_vl.insert(i);
//	}
//
//	switch (shift) {
//	case AI_RIGHT:
//		if ((500 <= npc_id && npc_id < 550) && x < 50)++x;
//		else if ((550 <= npc_id && npc_id < 600) && x < 50) ++x;
//		else if ((600 <= npc_id && npc_id < 650) && x < 20)++x;
//		clients[npc_id].m_character_shift = CHARACTER_RIGHT;
//		break;
//	case AI_LEFT:
//		if ((500 <= npc_id && npc_id < 550) && x > 20) --x;
//		else if ((550 <= npc_id && npc_id < 600) && x > 25) --x;
//		else if ((600 <= npc_id && npc_id < 650) && x > 0) --x;
//		clients[npc_id].m_character_shift = CHARACTER_LEFT;
//		break;
//	case AI_UP:
//		if ((500 <= npc_id && npc_id < 550) && y > 0) --y;
//		else if ((550 <= npc_id && npc_id < 600) && y > 25) --y;
//		else if ((600 <= npc_id && npc_id < 650) && y > 50) --y;
//		clients[npc_id].m_character_shift = CHARACTER_UP;
//		break;
//	case AI_DOWN:
//		if ((500 <= npc_id && npc_id < 550) && y < 20) ++y;
//		else if ((550 <= npc_id && npc_id < 600) && y < 50) ++y;
//		else if ((600 <= npc_id && npc_id < 650) && y < 50) ++y;
//		clients[npc_id].m_character_shift = CHARACTER_DOWN;
//		break;
//
//	default:
//		std::cout << "not defined AI shift " << std::endl;
//		break;
//	}
//
//
//	clients[npc_id].m_x = x;
//	clients[npc_id].m_y = y;
//
//	std::unordered_set <int> nearList;
//
//	//모든 유저와 현재 npc와의 NearList를 만든다.
//	for (int i = 0; i < MAX_USER; ++i) {
//		if (clients[i].in_used == false)continue;
//		if (GetNearObject(npc_id, i) == false)continue;
//
//		nearList.insert(i);
//	}
//
//
//	//예전 list
//	for (auto user : old_vl) {
//
//		//User가 이동전 리스트와 이동후 리스트에 존재한다..
//		if (nearList.count(user) != 0) {
//
//			//현재 User의 viewlist에 npc_id가 있는지 확인
//			//있으면 npc정보를 해당 User에게 보낸다.
//			clients[user].m_lock.lock();
//			if (clients[user].viewlist.count(npc_id)) {
//				clients[user].m_lock.unlock();
//				send_npc(user, npc_id);
//			}
//
//			//현재 User의 viewIst에 npc_id가 없다.
//			//없기 때문에 npc의 정보를 User에게 보낸다.
//			else {
//				clients[user].viewlist.insert(npc_id);
//				clients[user].m_lock.unlock();
//				send_put_player_packet(user, npc_id);
//			}
//
//		}
//		//User가 이동 후 리스트에 없다.
//		else {
//
//			//해당 user에 지금 npc_id가 있으면
//			//지운다. 
//			//가까이 있는게 아니라 멀어진거이기 때문
//			clients[user].m_lock.lock();
//			if (clients[user].viewlist.count(npc_id) == 1) {
//				clients[user].viewlist.erase(npc_id);
//				clients[user].m_lock.unlock();
//				send_remove_player_packet(user, npc_id);
//			}
//			else {
//				clients[user].m_lock.unlock();
//			}
//
//		}
//
//	}
//
//	//이동 후 리스트에 있는 유저와 NPC
//	for (auto user : nearList) {
//		//이동 전에 리스트 정보에는 해당 npc_id가 없다.
//		if (old_vl.count(npc_id) == 0) {
//
//			//이동 후 리스트에도 현재 NPC의 정보가 없다.
//
//			clients[user].m_lock.lock();
//			if (clients[user].viewlist.count(npc_id) == 0) {
//
//				clients[user].viewlist.insert(npc_id);
//				clients[user].m_lock.unlock();
//				send_put_player_packet(user, npc_id);
//
//			}
//
//			else {
//				clients[user].m_lock.unlock();
//				send_npc(user, npc_id);
//			}
//		}
//
//
//	}
//
//
//}
//bool is_Sleeping(int id) {
//	return clients[id].is_sleeping;
//}
//void WakeupNPC(int id, int player_id) {
//
//	if (is_npc(id) == true) {
//
//		OVER_EX* ex_over = new OVER_EX;
//		ex_over->ev = EV_PLAYER_MOVE_DETECT;
//		ex_over->target_player = player_id;
//		PostQueuedCompletionStatus(g_iocp, 1, id, &ex_over->over);
//
//	}
//
//}
//
//void WakeupMoveNPC(int id, int player_id) {
//
//	if (is_npc(id) == true) {
//
//		if (GetNearObject_npc(id, player_id) == true && clients[id].m_is_npc_move == false) {
//			OVER_EX* ex_over = new OVER_EX;
//			ex_over->ev = EV_FOLLOWER_PLAYER_DETECT;
//			ex_over->target_player = player_id;
//			PostQueuedCompletionStatus(g_iocp, 1, id, &ex_over->over);
//		}
//
//	}
//
//}
//
//
//bool is_npc(int id) {
//	if ((id >= MAX_USER) && (id < MAX_USER + NUM_NPC)) return true;
//	else return false;
//}
//void InitNPC() {
//	//미라 초기화 500~550
//	//서식지는 x: 20~50  y:0~20
//	for (int i = 0; i < 50; ++i) {
//		int npc_id = i + MAX_USER;
//
//		clients[npc_id].in_used = true;
//		clients[npc_id].m_x = rand() % 30 + 20;
//		clients[npc_id].m_y = rand() % 20;
//		clients[npc_id].is_sleeping = true;
//
//		add_timer(npc_id, EV_MOVE, 0, high_resolution_clock::now() + 1s);
//
//		lua_State*L = clients[npc_id].L;
//		lua_getglobal(L, "Set_AI_infor");
//		lua_pushnumber(L, npc_id); //d
//		lua_pushnumber(L, clients[npc_id].m_x);
//		lua_pushnumber(L, clients[npc_id].m_y);
//		lua_pushstring(L, "Mummy "); //이름
//		lua_pushnumber(L, 1);  //type
//		lua_pushnumber(L, 3);  //level
//		lua_pushnumber(L, 100);  //hp
//		if (0 != lua_pcall(L, 7, 0, 0)) //파라미터 1, 리턴 0
//			lua_error(L, "error Init NPC: %s\n", lua_tostring(L, -1));
//		//함수 등록
//		lua_register(L, "API_GET_X", API_GET_X);
//		lua_register(L, "API_GET_Y", API_GET_Y);
//		lua_register(L, "API_SendMessage", API_SendMessage);
//		lua_register(L, "API_WakeupNPC", API_WakeupNPC);
//		lua_register(L, "API_SleepNPC", API_SleepNPC);
//		lua_register(L, "API_add_timer", API_add_timer);
//		lua_register(L, "API_SendMonsterInfo", API_SendMonsterInfo);
//		lua_register(L, "API_SendDamageMonster", API_SendDamageMonster);
//		lua_register(L, "API_Requst_Revive", API_Requst_Revive);
//		lua_register(L, "API_MonsterAttack", API_MonsterAttack);
//		lua_register(L, "API_FollowerPlayer", API_FollowerPlayer);
//		lua_register(L, "API_GetNearPlayer", API_GetNearPlayer);
//
//
//
//	}
//
//	//박쥐 초기화 550~600
//	//서식지는 x: 25~50  y:25~50
//	for (int i = 50; i < 100; ++i) {
//		int npc_id = i + MAX_USER;
//		clients[npc_id].in_used = true;
//		clients[npc_id].m_x = rand() % 25 + 25;
//		clients[npc_id].m_y = rand() % 25 + 25;
//		clients[npc_id].is_sleeping = true;
//		add_timer(npc_id, EV_MOVE, 0, high_resolution_clock::now() + 1s);
//
//		lua_State*L = clients[npc_id].L;
//		lua_getglobal(L, "Set_AI_infor");
//		lua_pushnumber(L, npc_id);
//		lua_pushnumber(L, clients[npc_id].m_x);
//		lua_pushnumber(L, clients[npc_id].m_y);
//		lua_pushstring(L, "Bat ");
//		lua_pushnumber(L, 1);
//		lua_pushnumber(L, 3);
//		lua_pushnumber(L, 100);
//		if (0 != lua_pcall(L, 7, 0, 0)) //파라미터 1, 리턴 0
//			lua_error(L, "error Init NPC: %s\n", lua_tostring(L, -1));
//
//		//함수 등록
//		lua_register(L, "API_GET_X", API_GET_X);
//		lua_register(L, "API_GET_Y", API_GET_Y);
//		lua_register(L, "API_SendMessage", API_SendMessage);
//		lua_register(L, "API_WakeupNPC", API_WakeupNPC);
//		lua_register(L, "API_SleepNPC", API_SleepNPC);
//		lua_register(L, "API_add_timer", API_add_timer);
//		lua_register(L, "API_SendMonsterInfo", API_SendMonsterInfo);
//		lua_register(L, "API_SendDamageMonster", API_SendDamageMonster);
//		lua_register(L, "API_Requst_Revive", API_Requst_Revive);
//		lua_register(L, "API_MonsterAttack", API_MonsterAttack);
//		lua_register(L, "API_FollowerPlayer", API_FollowerPlayer);
//		lua_register(L, "API_GetNearPlayer", API_GetNearPlayer);
//	}
//	//좀비 초기화 600~650
//	//서식지는 x: 0~20  y:30~50
//	for (int i = 100; i < 150; ++i) {
//		int npc_id = i + MAX_USER;
//
//		clients[npc_id].in_used = true;
//		clients[npc_id].m_x = rand() % 20;
//		clients[npc_id].m_y = rand() % 20 + 30;
//		clients[npc_id].is_sleeping = true;
//
//		add_timer(npc_id, EV_MOVE, 0, high_resolution_clock::now() + 1s);
//
//		lua_State*L = clients[npc_id].L;
//		lua_getglobal(L, "Set_AI_infor");
//		lua_pushnumber(L, npc_id);
//		lua_pushnumber(L, clients[npc_id].m_x);
//		lua_pushnumber(L, clients[npc_id].m_y);
//		lua_pushstring(L, "Zombie ");
//		lua_pushnumber(L, 1);
//		lua_pushnumber(L, 3);
//		lua_pushnumber(L, 100);
//		if (0 != lua_pcall(L, 7, 0, 0)) //파라미터 1, 리턴 0
//			lua_error(L, "error Init NPC: %s\n", lua_tostring(L, -1));
//
//		//함수 등록
//		lua_register(L, "API_GET_X", API_GET_X);
//		lua_register(L, "API_GET_Y", API_GET_Y);
//		lua_register(L, "API_SendMessage", API_SendMessage);
//		lua_register(L, "API_WakeupNPC", API_WakeupNPC);
//		lua_register(L, "API_SleepNPC", API_SleepNPC);
//		lua_register(L, "API_add_timer", API_add_timer);
//		lua_register(L, "API_SendMonsterInfo", API_SendMonsterInfo);
//		lua_register(L, "API_SendDamageMonster", API_SendDamageMonster);
//		lua_register(L, "API_Requst_Revive", API_Requst_Revive);
//		lua_register(L, "API_MonsterAttack", API_MonsterAttack);
//		lua_register(L, "API_FollowerPlayer", API_FollowerPlayer);
//		lua_register(L, "API_GetNearPlayer", API_GetNearPlayer);
//	}
//
//	std::cout << "몬스터 가상머신 초기화 완료" << std::endl;
//
//}
//void ReviveMonster(int monster_id) {
//
//
//	//미라
//	if (500 < monster_id &&monster_id < 550) {
//		/*
//		HP 90으로 나옴
//		*/
//
//		/*clients[monster_id].m_x = rand() % 30 + 20;
//		clients[monster_id].m_y = rand() % 20;
//*/
//		clients[monster_id].m_x = rand() % 10 + 20;
//		clients[monster_id].m_y = 0;
//		lua_State*L = clients[monster_id].L;
//
//
//		clients[monster_id].lua_lock.lock();
//		lua_getglobal(L, "Set_AI_infor");
//		lua_pushnumber(L, monster_id);
//		lua_pushnumber(L, clients[monster_id].m_x);
//		lua_pushnumber(L, clients[monster_id].m_y);
//		lua_pushstring(L, "Mummy "); //이름
//		lua_pushnumber(L, 1);
//		lua_pushnumber(L, 3);
//		lua_pushnumber(L, 100);
//		if (0 != lua_pcall(L, 7, 0, 0)) //파라미터 1, 리턴 0
//			lua_error(L, "error REVIVE MONSER Mummy: %s\n", lua_tostring(L, -1));
//
//		lua_pop(L, 8);
//		clients[monster_id].lua_lock.unlock();
//
//		clients[monster_id].m_lock.lock();
//		if (clients[monster_id].in_used == false) {
//			clients[monster_id].in_used = true;
//			clients[monster_id].m_lock.unlock();
//		}
//		else {
//			clients[monster_id].m_lock.unlock();
//		}
//
//	}
//	//박쥐
//	else if (550 < monster_id &&monster_id < 600) {
//		clients[monster_id].m_x = rand() % 25 + 25;
//		clients[monster_id].m_y = rand() % 25 + 25;
//
//		lua_State*L = clients[monster_id].L;
//
//		clients[monster_id].lua_lock.lock();
//		lua_getglobal(L, "Set_AI_infor");
//		lua_pushnumber(L, monster_id);
//		lua_pushnumber(L, clients[monster_id].m_x);
//		lua_pushnumber(L, clients[monster_id].m_y);
//		lua_pushstring(L, "Bat ");
//		lua_pushnumber(L, 1);
//		lua_pushnumber(L, 3);
//		lua_pushnumber(L, 100);
//		if (0 != lua_pcall(L, 7, 0, 0)) //파라미터 1, 리턴 0
//			lua_error(L, "error REVIVE MONSER Bat: %s\n", lua_tostring(L, -1));
//
//		lua_pop(L, 8);
//		clients[monster_id].lua_lock.unlock();
//
//		clients[monster_id].m_lock.lock();
//		if (clients[monster_id].in_used == false) {
//			clients[monster_id].in_used = true;
//			clients[monster_id].m_lock.unlock();
//		}
//		else
//			clients[monster_id].m_lock.unlock();
//	}
//	//좀비
//	else if (600 < monster_id &&monster_id < 650) {
//		clients[monster_id].m_x = rand() % 20;
//		clients[monster_id].m_y = rand() % 20 + 30;
//
//		clients[monster_id].lua_lock.lock();
//		lua_State*L = clients[monster_id].L;
//		lua_getglobal(L, "Set_AI_infor");
//		lua_pushnumber(L, monster_id);
//		lua_pushnumber(L, clients[monster_id].m_x);
//		lua_pushnumber(L, clients[monster_id].m_y);
//		lua_pushstring(L, "Zombie ");
//		lua_pushnumber(L, 1);
//		lua_pushnumber(L, 3);
//		lua_pushnumber(L, 100);
//		if (0 != lua_pcall(L, 7, 0, 0)) //파라미터 1, 리턴 0
//			lua_error(L, "error REVIVE MONSER zombie: %s\n", lua_tostring(L, -1));
//
//		lua_pop(L, 8);
//		clients[monster_id].lua_lock.unlock();
//
//		clients[monster_id].m_lock.lock();
//		if (clients[monster_id].in_used == false) {
//			clients[monster_id].in_used = true;
//			clients[monster_id].m_lock.unlock();
//		}
//		else
//			clients[monster_id].m_lock.unlock();
//	}
//
//
//
//	//플레이어의 근처에 있다면 put을 보낸다.
//	for (int i = 0; i < MAX_USER; ++i) {
//		if (clients[i].in_used == false)continue;
//		if (GetNearObject(i, monster_id) == false)continue;
//
//		clients[i].m_lock.lock();
//		if (clients[i].viewlist.count(monster_id) == 0) {
//			clients[i].viewlist.insert(monster_id);
//			clients[i].m_lock.unlock();
//		}
//		else
//			clients[i].m_lock.unlock();
//
//		OVER_EX* over_ex = new OVER_EX;
//		over_ex->ev = EV_PLAYER_MOVE_DETECT;
//		over_ex->target_player = i;
//		PostQueuedCompletionStatus(g_iocp, 1, monster_id, &over_ex->over);
//
//	}
//
//}
///******************************
//Lua functios
//********************************/
//int API_GET_X(lua_State* L) {
//	int obj_Id = (int)lua_tonumber(L, -1);
//	lua_pop(L, 2);
//	int x = clients[obj_Id].m_x;
//	lua_pushnumber(L, x);
//	return 1;
//
//}
//int API_GET_Y(lua_State* L) {
//	int obj_Id = (int)lua_tonumber(L, -1);
//	lua_pop(L, 2);
//	int y = clients[obj_Id].m_y;
//	lua_pushnumber(L, y);
//	return 1;
//}
//int API_SendMessage(lua_State* L) {
//	int client_id = (int)lua_tonumber(L, -3);
//	int from_id = (int)lua_tonumber(L, -2);
//
//	//루아 소스코드를 바꿔서 한글도 가능하게 만든다. 어렵다
//	char* mess = (char*)lua_tostring(L, -1);
//	wchar_t wmess[MAX_STR_LEN];
//	lua_pop(L, 4);
//
//	size_t wlen, len;
//	len = strnlen_s(mess, MAX_STR_LEN);
//	//크기,받는놈,바뀔길이,바뀔아스키코드
//	mbstowcs_s(&wlen, wmess, len, mess, _TRUNCATE);
//
//	send_chat_packet(client_id, from_id, wmess);
//	return 0;
//}
//int API_WakeupNPC(lua_State* L) {
//	int ai_id = (int)lua_tonumber(L, -1);
//	int target_id = (int)lua_tonumber(L, -2);
//	lua_pop(L, 3);
//	if (is_Sleeping(ai_id) == true) {
//		clients[ai_id].is_sleeping = false;
//		add_timer(ai_id, EV_MOVE, target_id, high_resolution_clock::now() + 1s);
//	}
//	return 0;
//}
//int API_SleepNPC(lua_State* L) {
//	int ai_id = (int)lua_tonumber(L, -1);
//	if (is_Sleeping(ai_id) == false) {
//
//		clients[ai_id].is_sleeping = true;
//	}
//
//	return 0;
//}
//
//int API_GetNearPlayer(lua_State* L) {
//	int player_id = (int)lua_tonumber(L, -2);
//	int monster_id = (int)lua_tonumber(L, -1);
//	lua_pop(L, 3);
//
//	if (GetNearObject_npc(player_id, monster_id)) {
//		lua_pushnumber(L, 1);
//	}
//
//	return 1;
//}
//
//int API_add_timer(lua_State* L) {
//	int target_id = (int)lua_tonumber(L, -2);
//	int obj_id = (int)lua_tonumber(L, -1);
//	lua_pop(L, 3);
//
//
//	EVENT_ST et;
//	et.obj_id = obj_id;
//	et.target_id = target_id;
//	et.type = EV_MOVE;
//	et.start_time = high_resolution_clock::now() + 1s;
//
//	timer_l.lock();
//	g_timer_queue.emplace(et);
//	timer_l.unlock();
//
//
//	return 0;
//}
//int API_SendMonsterInfo(lua_State* L) {
//
//	int player_id = (int)lua_tonumber(L, -6);
//	int monster_id = (int)lua_tonumber(L, -5);
//	int monster_type = (int)lua_tonumber(L, -4);
//	int monster_level = (int)lua_tonumber(L, -3);
//	int monster_hp = (int)lua_tonumber(L, -2);
//
//
//	//루아 소스코드를 바꿔서 한글도 가능하게 만든다. 어렵다
//	char* mess = (char*)lua_tostring(L, -1);
//	wchar_t wmess[MAX_STR_LEN];
//	lua_pop(L, 7);
//	size_t wlen, len;
//	len = strnlen_s(mess, MAX_STR_LEN);
//
//	//크기,받는놈,바뀔길이,바뀔아스키코드
//	mbstowcs_s(&wlen, wmess, len, mess, _TRUNCATE);
//	send_monsterInfo_packet(monster_id, player_id, monster_level, monster_hp, wmess);
//	return 0;
//
//}
//int API_SendDamageMonster(lua_State* L) {  //몬스터가 공격받음
//
//	int player_id = (int)lua_tonumber(L, -6);
//	int monster_id = (int)lua_tonumber(L, -5);
//	int monster_type = (int)lua_tonumber(L, -4);
//	int monster_level = (int)lua_tonumber(L, -3);
//	int monster_hp = (int)lua_tonumber(L, -2);
//
//	//루아 소스코드를 바꿔서 한글도 가능하게 만든다. 어렵다
//	char* mess = (char*)lua_tostring(L, -1);
//	wchar_t wmess[MAX_STR_LEN];
//	lua_pop(L, 7);
//	size_t wlen, len;
//	len = strnlen_s(mess, MAX_STR_LEN);
//
//	//크기,받는놈,바뀔길이,바뀔아스키코드
//	mbstowcs_s(&wlen, wmess, len, mess, _TRUNCATE);
//	Process_chat_notice(player_id, monster_id, monster_level, MESSAGE_TYPE::MT_ATTACK_PLAYER);
//
//	for (int i = 0; i < MAX_USER; ++i) {
//		if (clients[i].in_used == false)continue;
//		if (GetNearObject(i, player_id) == false)continue;
//		send_monsterInfo_packet(monster_id, i, monster_level, monster_hp, wmess);
//	}
//
//	return 0;
//
//}
//int API_Requst_Revive(lua_State* L) {  //몬스터가 죽었음
//
//	int monster_id = (int)lua_tonumber(L, -1); //죽은 몬스터
//	int player_id = (int)lua_tonumber(L, -2); //보낼 플레이어
//	int monster_level = (int)lua_tonumber(L, -3); //보낼 플레이어
//	lua_pop(L, 4);
//
//	EVENT_ST et;
//	et.obj_id = monster_id;
//	et.type = EV_REVIVE;
//	et.start_time = high_resolution_clock::now() + 5s;
//
//
//	//in user_false; -> 안그려짐  Lock 해야하지 않나 싶음
//	clients[monster_id].m_lock.lock();
//	if (clients[monster_id].in_used == true) {
//		clients[monster_id].in_used = false;
//		clients[monster_id].m_lock.unlock();
//		timer_l.lock();
//		g_timer_queue.emplace(et);
//		timer_l.unlock();
//
//
//		Process_chat_notice(player_id, monster_id, monster_level, MESSAGE_TYPE::MT_DESTROY_MONSTER);
//
//		for (int i = 0; i < MAX_USER; ++i) {
//			if (clients[i].in_used == false)continue;
//			if (GetNearObject(i, monster_id) == false)continue;
//
//			clients[i].m_lock.lock();
//			if (clients[i].viewlist.count(monster_id)==1) {
//				clients[i].viewlist.erase(monster_id);
//				clients[i].m_lock.unlock();
//
//			send_remove_player_packet(i, monster_id);
//			}
//			else
//				clients[i].m_lock.unlock();
//
//		}
//
//	}
//	else
//		clients[monster_id].m_lock.unlock();
//	return 0;
//}
//int API_MonsterAttack(lua_State* L) {
//	int player_id = (int)lua_tonumber(L, -2); //죽은 몬스터
//	int monster_id = (int)lua_tonumber(L, -1);
//	lua_pop(L, 3);
//
//
//	EVENT_ST et;
//	et.obj_id = monster_id;
//	et.target_id = player_id;
//	et.type = EV_MONSER_ATTACK_PLAYER;
//	et.start_time = high_resolution_clock::now() + 1s;
//
//
//	timer_l.lock();
//	g_timer_queue.emplace(et);
//	timer_l.unlock();
//
//	return 0;
//}
//
//int API_FollowerPlayer(lua_State* L) {
//	int player_id = (int)lua_tonumber(L, -3);
//	int monster_id = (int)lua_tonumber(L, -2);
//	int dir = (int)lua_tonumber(L, -1);
//	lua_pop(L, 4);
//
//	EVENT_ST et;
//	et.obj_id = monster_id;
//	et.target_id = player_id;
//	et.dir = (BYTE)dir;
//
//	et.type = EV_FOLLOWER_PLAYER;
//	et.start_time = high_resolution_clock::now() + 1s;
//	clients[monster_id].m_is_npc_move = true;
//
//
//	timer_l.lock();
//	g_timer_queue.emplace(et);
//	timer_l.unlock();
//
//
//	return 0;
//}
//
///******************************
//DB functios
//********************************/
//void DB_Dis_error(SQLHANDLE hHandle, SQLSMALLINT hType, RETCODE RetCode) {
//	SQLSMALLINT iRec = 0;
//	SQLINTEGER  iError;
//	WCHAR       wszMessage[1000];
//	WCHAR       wszState[SQL_SQLSTATE_SIZE + 1];
//	if (RetCode == SQL_INVALID_HANDLE) {
//		fwprintf(stderr, L"Invalid handle!\n");
//		return;
//	}
//	while
//		(SQLGetDiagRec(hType, hHandle, ++iRec, wszState, &iError, wszMessage, (SQLSMALLINT)(sizeof(wszMessage) / sizeof(WCHAR)), (SQLSMALLINT *)NULL) == SQL_SUCCESS) {
//		// Hide data truncated.. 
//		if (wcsncmp(wszState, L"01004", 5)) {
//			fwprintf(stderr, L"[%5.5s] %s (%d)\n", wszState, wszMessage, iError);
//		}
//	}
//}
///******************************
//Thread functios
//********************************/
//void Timer_thread() {
//
//	while (1) {
//		//쓰레드 하나가 CPU하나를 잡아먹는것을 방지
//		std::this_thread::sleep_for(10ms);
//
//		//다음 이벤트와의 딜레이 차이 방지를 위한
//		//이중 반복문
//		while (1) {
//			timer_l.lock();
//			if (g_timer_queue.empty()) {
//				timer_l.unlock();
//				break;
//			}
//
//			EVENT_ST ev = g_timer_queue.top();
//			if (ev.start_time > high_resolution_clock::now()) {
//				timer_l.unlock();
//				break;
//			}
//			g_timer_queue.pop();
//			timer_l.unlock();
//
//			OVER_EX* over_ex = new OVER_EX;
//			over_ex->target_player = ev.target_id;
//
//			if (ev.type == EV_MOVE) {
//				over_ex->ev = EV_MOVE;
//			}
//
//			else if (ev.type == EV_REVIVE) {
//				over_ex->ev = EV_REVIVE;
//			}
//			else if (ev.type == EV_PLAYER_MOVE) {
//				over_ex->dir = ev.dir;
//				over_ex->ev = EV_PLAYER_MOVE;
//
//			}
//			else if (ev.type == EV_PLAYER_ATTACK) {
//				over_ex->ev = EV_PLAYER_ATTACK;
//			}
//			else if (ev.type == EV_MONSER_ATTACK_PLAYER) {
//				over_ex->ev = EV_MONSER_ATTACK_PLAYER;
//			}
//			else if (ev.type == EV_FOLLOWER_PLAYER) {
//				over_ex->dir = ev.dir;
//				over_ex->ev = EV_FOLLOWER_PLAYER;
//			}
//			else {
//				std::cout << "error TimerThread" << std::endl;
//				while (1);
//			}
//
//			// over 초기화를 해줘야 하느냐?
//			// 각각의 값이 가지는 값은 검색을 안한다.
//			// obj_id의 ID는 등록을 한적이 없다. ->검색을 안한다.
//
//			PostQueuedCompletionStatus(g_iocp, 1, ev.obj_id, &over_ex->over);
//		}
//	}
//
//}
//
//void worker_thread() {
//	while (true) {
//		DWORD io_byte;
//		ULONGLONG l_key;
//		OVER_EX *over_ex;
//
//		// h_event는 나중에 어떻게 사용할지 모르기 때문에 사용하면 안된다.
//		// overlapped 구조체를 확장 하여 사용한다. 그렇기 때문에 type cast를 해줘야 한다.
//		int is_error = GetQueuedCompletionStatus(g_iocp, &io_byte, &l_key, reinterpret_cast<LPWSAOVERLAPPED*>(&over_ex), INFINITE);
//
//		int key = static_cast<int>(l_key);
//
//		if (0 == is_error) {
//
//			//접속이 완료된 플레이어만 로그아웃 처리를  하여 DB에 이상한값 갱신 방지
//			if (clients[key].in_used == true) {
//				EVENT_DB db;
//				db.id = key; //플레이어 인덱스
//				db.type = EV_LOGOUT; //로그아웃 이벤트
//				db.db_id = clients[key].db_id; //db에 접속할 id;
//				db.x = clients[key].m_x;  //접속은 끊은 플레이어 위치
//				db.y = clients[key].m_y;   //접속은 끊은 플레이어 위치
//				db.hp = clients[key].m_hp;
//				db.exp = clients[key].m_exp;
//				db.level = clients[key].m_level;
//				db.quest_step = clients[key].m_quest_step;
//				db.quest_killed_monster = clients[key].m_quest_killed_monster;
//
//				db_l.lock();
//				g_db_queue.push(db);
//				db_l.unlock();
//			}
//
//
//			// 끊김을 처리
//			int error_no = WSAGetLastError();
//			if (64 == error_no) {
//				disconnect_client(key); // 64번 클라에서 끊을 경우
//				continue;
//			}
//			if (0 == error_no) {
//				disconnect_client(key);
//				continue;
//			}
//			error_display("GQCS : ", error_no);
//
//
//		}
//		if (0 == io_byte) {
//
//		}
//
//		if (over_ex->ev == EV_RECV) {
//			// Recv 처리
//			int rest = io_byte;
//			char *ptr = over_ex->messageBuffer;
//			char packet_size = 0;
//
//			if (0 < clients[key].prev_size) packet_size = clients[key].packet_buffer[0];
//
//			// 패킷 조립
//			while (0 < rest) {
//				// 이전 기록이 없다면 패킷 사이즈를 ptr의 맨 처음으로 알 수 있다.
//				if (0 == packet_size) packet_size = ptr[0];
//				int required = packet_size - clients[key].prev_size;
//				if (required <= rest) {
//					// 그냥 패킷 버퍼를 담을 경우 덮어쓸 위험이 있기 때문에
//					memcpy(clients[key].packet_buffer + clients[key].prev_size, ptr, required);
//
//					// 패킷 내의 데이터 처리[여기 까지]
//					process_packet(key, clients[key].packet_buffer);
//
//					rest -= required;
//					ptr += required;
//
//					packet_size = 0;
//					clients[key].prev_size = 0;
//				}
//				else {
//					//모자랄 경우
//					memcpy(clients[key].packet_buffer + clients[key].prev_size, ptr, rest);
//					rest = 0;
//					clients[key].prev_size += rest;
//
//					//delete 부분 못함
//				}
//			}
//			do_recv(key);
//		}
//		else if (over_ex->ev == EV_SEND) {
//			delete over_ex;
//
//		}
//		else if (over_ex->ev == EV_MOVE) {
//
//			EVENT_ST ev;
//			ev.obj_id = key;
//			ev.target_id = over_ex->target_player;
//			ev.start_time = high_resolution_clock::now();
//			ev.type = EV_MOVE;
//			Process_event(ev);
//
//			delete over_ex;
//		}
//		else if (over_ex->ev == EV_PLAYER_MOVE_DETECT) {
//
//			//누가 움직였나 받아온다.
//			//멀티쓰레드에서 동시에 작동을 하는데 루아 가상머신이 버티질 못한다.
//			//루아는 멀티쓰레드를 생각해서 만들어진게 아니다.
//			//여러 쓰레드에서 getglobal하고 하고 하면 꼬인다
//
//			if (clients[key].in_used == true) {
//				lua_State* L = clients[key].L;
//				int player_id = over_ex->target_player;
//
//				clients[key].lua_lock.lock();
//				lua_getglobal(L, "event_my_status");
//				lua_pushnumber(L, player_id);
//				if (0 != lua_pcall(L, 1, 0, 0))
//					lua_error(L, "error EV_PLAYER_MOVE_DETECT: %s\n", lua_tostring(L, -1));
//
//				lua_pop(L, -2);
//				clients[key].lua_lock.unlock();
//			}
//
//			delete over_ex;
//
//		}
//		else if (over_ex->ev == EV_PLAYER_ATTACK_MONSER) {
//
//			lua_State* L = clients[key].L;
//			int player_id = over_ex->target_player;  //공격한 플레이어
//
//			clients[key].m_lock.lock();
//			if (clients[key].in_used == true) {
//				clients[key].m_lock.unlock();
//
//				clients[key].lua_lock.lock();
//				lua_getglobal(L, "event_attak_to_player");
//				lua_pushnumber(L, player_id);
//				if (0 != lua_pcall(L, 1, 0, 0))
//					lua_error(L, "error EV_PLAYER_MONSER: %s\n", lua_tostring(L, -1));
//				lua_pop(L, -2);
//				clients[key].lua_lock.unlock();
//
//			}
//			else
//				clients[key].m_lock.unlock();
//
//			delete over_ex;
//		}
//		else if (over_ex->ev == EV_REVIVE) {
//
//			EVENT_ST ev;
//			ev.obj_id = key;
//			ev.type = EV_REVIVE;
//			Process_event(ev);
//
//			delete over_ex;
//		}
//		else if (over_ex->ev == EV_PLAYER_MOVE) {
//
//			EVENT_ST ev;
//			ev.obj_id = key;
//			ev.dir = over_ex->dir;
//			ev.type = EV_PLAYER_MOVE;
//			Process_event(ev);
//
//			delete over_ex;
//		}
//		else if (over_ex->ev == EV_PLAYER_ATTACK) {
//
//			EVENT_ST ev;
//			ev.obj_id = key;
//			ev.dir = over_ex->dir;
//			ev.type = EV_PLAYER_ATTACK;
//			Process_event(ev);
//
//			delete over_ex;
//		}
//		else if (over_ex->ev == EV_LOGIN_OK) {
//
//
//			int new_id = key;
//
//			send_login_ok_packet(new_id);
//			send_put_player_packet(new_id, new_id);
//			//나의 접속을 현재 사람들에게 알림
//			for (int i = 0; i < MAX_USER; ++i) {
//
//				//사용중인것만 보내기
//				if (clients[i].in_used == false) continue;
//
//				// 나에게 두번 보내는 것을 방지
//				if (i == new_id) continue;
//
//				if (GetNearObject(i, new_id) == false)continue;
//
//				clients[i].m_lock.lock();
//				clients[i].viewlist.insert(new_id);
//				clients[i].m_lock.unlock();
//				send_put_player_packet(i, new_id);
//			}
//
//			// 이미 접속해 있는 플레이어 정보를 받는다.
//			for (int i = 0; i < MAX_USER; ++i) {
//
//				//사용중인것만 보내기
//				if (clients[i].in_used == false) continue;
//
//				// 나에게 두번 보내는 것을 방지
//				if (i == new_id) continue;
//
//				//가까이에 있는 유저
//				if (GetNearObject(i, new_id) == false)continue;
//
//				clients[new_id].m_lock.lock();
//				clients[new_id].viewlist.insert(i);
//				clients[new_id].m_lock.unlock();
//
//				send_put_player_packet(new_id, i);
//			}
//
//			// NCP정보를 보낸다
//			for (int i = 0; i < NUM_NPC; ++i) {
//
//				int npc_id = i + MAX_USER;
//
//				//가까이 있는 NPC가 있는가?
//				if (GetNearObject(new_id, npc_id) == false)continue;
//
//				//가까이 있는 NPC를 깨운다.
//				WakeupNPC(npc_id, new_id);
//				WakeupMoveNPC(npc_id, new_id);
//
//				clients[new_id].m_lock.lock();
//				clients[new_id].viewlist.insert(npc_id);
//				clients[new_id].m_lock.unlock();
//				send_put_player_packet(new_id, npc_id);
//			}
//
//			delete over_ex;
//		}
//		else if (over_ex->ev == EV_LOGIN_FALSE) {
//
//			sc_packet_login_fail packet;
//			packet.size = sizeof(packet);
//			packet.type = SC_LOGIN_FAIL;
//			send_packet(key, &packet);
//			delete over_ex;
//		}
//		else if (over_ex->ev == EV_MONSER_ATTACK_PLAYER) {
//
//			EVENT_ST ev;
//			ev.obj_id = key;
//			ev.target_id = over_ex->target_player;
//			ev.start_time = high_resolution_clock::now();
//			ev.type = EV_MONSER_ATTACK_PLAYER;
//			Process_event(ev);
//
//			delete over_ex;
//		}
//		else if (over_ex->ev == EV_FOLLOWER_PLAYER) {
//
//			EVENT_ST ev;
//			ev.obj_id = key;
//			ev.target_id = over_ex->target_player;
//			ev.dir = over_ex->dir;
//			ev.start_time = high_resolution_clock::now();
//			ev.type = EV_FOLLOWER_PLAYER;
//			Process_event(ev);
//
//			delete over_ex;
//		}
//		else if (over_ex->ev == EV_FOLLOWER_PLAYER_DETECT) {
//			if (clients[key].in_used == true) {
//				lua_State* L = clients[key].L;
//				int player_id = over_ex->target_player;
//
//				clients[key].lua_lock.lock();
//				lua_getglobal(L, "event_follwer_to_player");
//				lua_pushnumber(L, player_id);
//				if (0 != lua_pcall(L, 1, 0, 0))
//					lua_error(L, "error EV_FOLLOWER_PLAYER_DETECT: %s\n", lua_tostring(L, -1));
//
//				lua_pop(L, -2);
//				clients[key].lua_lock.unlock();
//			}
//
//			delete over_ex;
//		}
//
//		else {
//			std::cout << " worker Thread Unknow Event" << std::endl;
//			while (1);
//		}
//
//	}
//}
//void DB_thread() {
//	setlocale(LC_ALL, "korean");
//	std::wcout.imbue(std::locale("korean"));
//	SQLHENV henv;
//	SQLHDBC hdbc;
//	SQLHSTMT hstmt = 0;
//	SQLRETURN retcode;
//
//	SQLWCHAR user_id[10];
//	SQLINTEGER user_position_x;
//	SQLINTEGER user_position_y;
//	SQLINTEGER user_level;
//	SQLINTEGER user_exp;
//	SQLINTEGER user_hp;
//	SQLINTEGER user_quest_step;
//	SQLINTEGER user_quest_killed_Monster;
//	BOOL user_login;
//
//	SQLLEN cb_id, cb_position_x, cb_position_y, cb_user_level, cb_user_exp, cb_user_hp,
//		cb_user_quest_step, db_user_quest_killedMonster, cb_user_login;
//
//	// Allocate environment handle  
//	retcode = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &henv);
//
//	// Set the ODBC version environment attribute  
//	if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
//		retcode = SQLSetEnvAttr(henv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER*)SQL_OV_ODBC3, 0);
//
//		// Allocate connection handle  
//		if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
//			retcode = SQLAllocHandle(SQL_HANDLE_DBC, henv, &hdbc);
//
//			// Set login timeout to 5 seconds  
//			if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
//				SQLSetConnectAttr(hdbc, SQL_LOGIN_TIMEOUT, (SQLPOINTER)5, 0);
//				// Connect to data source  
//				retcode = SQLConnect(hdbc, (SQLWCHAR*)L"my_GameServer", SQL_NTS, (SQLWCHAR*)NULL, 0, NULL, 0);
//				std::cout << "Connect DB Server" << std::endl;
//
//				if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
//					while (1) {
//						std::this_thread::sleep_for(10ms);
//						db_l.lock();
//						if (g_db_queue.empty()) {
//							db_l.unlock();
//							continue;
//						}
//						db_l.unlock();
//
//
//						retcode = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);
//						wchar_t buf[100];
//						SQLWCHAR* sql = nullptr;
//
//						db_l.lock();
//						auto t = g_db_queue.front();
//						db_l.unlock();
//						switch (t.type) {
//						case EV_LOGIN_REQUEST: {
//							//wsprintf(buf, L"SELECT* FROM user_table WHERE user_name = '%s'", t.db_id);
//							wsprintf(buf, L"EXEC Select_user_name '%s'", t.db_id);
//
//							sql = buf;
//
//							retcode = SQLExecDirect(hstmt, sql, SQL_NTS);
//
//							if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
//								// Bind columns 1, 2, and 3  
//								retcode = SQLBindCol(hstmt, 1, SQL_UNICODE, user_id, 10, &cb_id);
//								retcode = SQLBindCol(hstmt, 2, SQL_INTEGER, &user_position_x, 100, &cb_position_x);
//								retcode = SQLBindCol(hstmt, 3, SQL_INTEGER, &user_position_y, 100, &cb_position_y);
//								retcode = SQLBindCol(hstmt, 4, SQL_INTEGER, &user_level, 100, &cb_user_level);
//								retcode = SQLBindCol(hstmt, 5, SQL_INTEGER, &user_exp, 100, &cb_user_exp);
//								retcode = SQLBindCol(hstmt, 6, SQL_INTEGER, &user_hp, 100, &cb_user_hp);
//								retcode = SQLBindCol(hstmt, 7, SQL_INTEGER, &user_quest_step, 100, &cb_user_quest_step);
//								retcode = SQLBindCol(hstmt, 8, SQL_INTEGER, &user_quest_killed_Monster, 100, &db_user_quest_killedMonster);
//								retcode = SQLBindCol(hstmt, 9, SQL_INTEGER, &user_login, 100, &cb_user_login);
//
//
//
//
//								retcode = SQLFetch(hstmt);
//								// Fetch and print each row of data. On an error, display a message and exit.  
//								if (retcode == SQL_ERROR || retcode == SQL_SUCCESS_WITH_INFO)
//									DB_Dis_error(hstmt, SQL_HANDLE_STMT, retcode);
//
//								if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
//									//	wprintf(L"%S %d %d \n", user_id, user_position_x, user_position_y);
//
//									OVER_EX* over_ex = new OVER_EX;
//
//									if (user_login == FALSE) {
//
//										if (clients[t.id].in_used == false) {
//											clients[t.id].in_used = true;
//											clients[t.id].m_x = user_position_x;
//											clients[t.id].m_y = user_position_y;
//											clients[t.id].m_level = user_level;
//											clients[t.id].db_id = user_id;
//											clients[t.id].m_exp = user_exp;
//											clients[t.id].m_hp = user_hp;
//											clients[t.id].m_quest_step = user_quest_step;
//											clients[t.id].m_quest_killed_monster = user_quest_killed_Monster;
//											clients[t.id].m_login = user_login = TRUE;
//
//
//												retcode = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);
//												wsprintf(buf, L"EXEC Update_user_login '%s',%d", t.db_id, user_login);
//												sql = buf;
//												retcode = SQLExecDirect(hstmt, sql, SQL_NTS);
//
//											over_ex->ev = EV_LOGIN_OK;
//											PostQueuedCompletionStatus(g_iocp, 1, t.id, &over_ex->over);
//
//										}
//
//									}
//
//									else if (user_login == TRUE) {
//										std::cout << "DB: 이미 사용중인 ID" << std::endl;
//										over_ex->ev = EV_LOGIN_FALSE;
//										PostQueuedCompletionStatus(g_iocp, 1, t.id, &over_ex->over);
//									}
//
//									db_l.lock();
//									g_db_queue.pop();
//									db_l.unlock();
//
//								}
//								else {
//									DB_Dis_error(hstmt, SQL_HANDLE_STMT, retcode);
//
//									std::cout << "없는 아이디" << std::endl;
//
//									OVER_EX* over_ex = new OVER_EX;
//									over_ex->ev = EV_LOGIN_FALSE;
//									PostQueuedCompletionStatus(g_iocp, 1, t.id, &over_ex->over);
//									db_l.lock();
//									g_db_queue.pop();
//									db_l.unlock();
//								}
//							}
//
//							break;
//						}
//						case EV_LOGOUT: {
//							std::cout << "플레이어 로그아웃" << std::endl;
//
//							//	wsprintf(buf, L"UPDATE user_data SET user_po_x=%d,user_po_y=%d WHERE user_id='%s'", t.x, t.y, t.db_id);
//							wsprintf(buf, L"EXEC Update_user_data '%s',%d,%d,%d,%d,%d,%d,%d,%d",
//								t.db_id, t.x, t.y, t.level, t.exp, t.hp, t.quest_step, t.quest_killed_monster, FALSE);
//							sql = buf;
//							retcode = SQLExecDirect(hstmt, sql, SQL_NTS);
//
//
//							db_l.lock();
//							g_db_queue.pop();
//							db_l.unlock();
//
//							break;
//						}
//						default:
//							std::cout << "un difeind DB event" << std::endl;
//							while (1);
//							break;
//						}
//
//						// Process data  
//						if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {}
//						else {
//							DB_Dis_error(hstmt, SQL_HANDLE_STMT, retcode);
//						}
//
//						SQLCancel(hstmt);
//						SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
//
//					}  //while
//				} //DB연결 성고하면
//				SQLDisconnect(hdbc);
//			}
//			SQLFreeHandle(SQL_HANDLE_DBC, hdbc);
//		}
//		SQLFreeHandle(SQL_HANDLE_ENV, henv);
//	}
//
//}  //end
//
///******************************
//Process functios
//********************************/
//void error_display(const char *message, int error_no) {
//	WCHAR *IpMsgBuf;
//	FormatMessage(
//		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
//		NULL, error_no,
//		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
//		(LPTSTR)&IpMsgBuf, 0, NULL);
//
//	std::cout << message;
//	std::wcout << L"에러 [" << error_no << "] L" << IpMsgBuf << std::endl;
//	while (true);
//	LocalFree(IpMsgBuf);
//}
//
//
//void Process_move(EVENT_ST& ev) {
//	int id = ev.obj_id;
//	int x = clients[id].m_x;
//	int y = clients[id].m_y;
//	switch (ev.dir) {
//	case CHARACTER_UP: {
//		if (y > 0)
//			y--;
//		clients[id].m_is_move = false;
//		clients[id].m_character_shift = CHARACTER_UP;
//		break;
//	}
//	case CHARACTER_DOWN: {
//		if (y < (WORLD_HEIGHT - 1))
//			y++;
//		clients[id].m_is_move = false;
//		clients[id].m_character_shift = CHARACTER_DOWN;
//		break;
//	}
//	case CHARACTER_RIGHT: {
//		if (x < (WORLD_WIDTH - 1))
//			x++;
//		clients[id].m_is_move = false;
//		clients[id].m_character_shift = CHARACTER_RIGHT;
//		break;
//	}
//	case CHARACTER_LEFT: {
//		if (x > 0)
//			x--;
//		clients[id].m_is_move = false;
//		clients[id].m_character_shift = CHARACTER_LEFT;
//		break;
//	}
//	default:
//		break;
//	}
//
//	clients[id].m_lock.lock();
//	std::unordered_set <int> old_view = clients[id].viewlist;
//	clients[id].m_lock.unlock();
//
//	clients[id].m_x = x;
//	clients[id].m_y = y;
//	std::unordered_set <int> nearList;
//
//
//	//내 주변에 있는 유저 정보 push
//	for (int i = 0; i < MAX_USER; ++i) {
//		if ((clients[i].in_used) && (GetNearObject(id, i)) && (i != id))
//			nearList.insert(i);
//	}
//
//	//내 주변에 있는 NPC 정보 push
//	for (int i = 0; i < NUM_NPC; ++i) {
//		int npc_id = i + MAX_USER;
//		if (clients[npc_id].in_used == false)continue;
//		if (GetNearObject(id, npc_id) == false)continue;
//		nearList.insert(npc_id);
//	}
//
//	//Near의 모든 객체에 대해
//	for (auto iter : nearList) {
//		// 이동한 본인viewList에 NearList의 상대정보가 있으면
//		if (old_view.count(iter)) {
//
//
//			WakeupMoveNPC(iter, id);
//
//			//사람인경우실행
//			//if (iter > MAX_USER)continue;
//			if (is_npc(iter) == true)continue;
//
//			clients[iter].m_lock.lock();
//			//상대 viewList에 내가 있으면
//			if (clients[iter].viewlist.count(id)) {
//				//이동한 플레이어의 정보를 시야안에 있는 플레이어에게 전송
//				clients[iter].m_lock.unlock();
//				send_position_pakcet(iter, id);
//			}
//
//			//상대 viewList에 내가 없으면
//			else {
//				//viewList에 등록
//				clients[iter].viewlist.insert(id);
//				clients[iter].m_lock.unlock();
//
//				//이동한 플레이어의 정보를 시야안에 있는 플레이어에게 전송
//				send_put_player_packet(iter, id);
//			}
//
//		}
//
//		// 이동한 본인 viewList에 NearList의 상대정보가 없으면 (새로 시야에 들어온 유저 혹은 NPC)
//		else {
//
//			//내 viewList에 상대정보 등록
//			clients[id].m_lock.lock();
//			clients[id].viewlist.insert(iter);
//			clients[id].m_lock.unlock();
//
//
//			//상대정보를 전송
//			send_put_player_packet(id, iter);
//
//			//새로 시야에 들어온 NPC를 깨운다.
//			WakeupNPC(iter, id);
//
//			if (iter > MAX_USER)continue;
//			//기존에 상대의 viewList에 내가 있으면
//			//이동
//
//
//			clients[iter].m_lock.lock();
//			if (clients[iter].viewlist.count(id)) {
//				clients[iter].m_lock.unlock();
//				send_position_pakcet(iter, id);
//			}
//			//기존에 상대의 viewList에 내가 없으면
//			//내 정보 전송
//			else {
//				clients[iter].viewlist.insert(id);
//				clients[iter].m_lock.unlock();
//				send_put_player_packet(iter, id);
//			}
//		}
//	}
//
//	//ViewList에 있는 모든 객채에 대해
//	for (auto iter : old_view) {      //시야에서 사라짐
//		if (0 != nearList.count(iter))
//			continue;
//
//
//		clients[id].m_lock.lock();
//		clients[id].viewlist.erase(iter);
//		clients[id].m_lock.unlock();
//		send_remove_player_packet(id, iter);
//
//		//사람인가?
//		//if (iter > MAX_USER)continue;
//		if (is_npc(iter) == true)continue;
//
//
//
//		clients[iter].m_lock.lock();
//		if (0 != clients[iter].viewlist.count(id)) {
//			clients[iter].viewlist.erase(id);
//			clients[iter].m_lock.unlock();
//
//			send_remove_player_packet(iter, id);
//		}
//	}
//	send_position_pakcet(id, id);
//
//
//	//for (auto monster : nearList) {
//
//	//	if (is_npc(monster) == false)continue;
//	//	if (GetNearObject(monster, id) == false)continue;
//
//
//	//	OVER_EX* ex_over = new OVER_EX;
//	//	ex_over->ev = EV_FOLLOWER_PLAYER_DETECT;
//	//	ex_over->target_player = id;
//	//	PostQueuedCompletionStatus(g_iocp, 1, monster, &ex_over->over);
//
//	//}
//
//
//
//}
//void Process_attack(EVENT_ST& ev) {
//
//	int id = ev.obj_id;
//	int x = clients[id].m_x;
//	int y = clients[id].m_y;
//	int left = x - 1, right = x + 1, up = y - 1, down = y + 1;
//	clients[id].m_is_attack = false;
//	clients[id].m_lock.lock();
//	std::unordered_set<int>near_list = clients[id].viewlist;
//	clients[id].m_lock.unlock();
//
//	//시야에 있는 몬스터만 체크
//
//	for (auto monster : near_list) {
//		//사람은 패스
//		if (is_npc(monster) == false)continue;
//		if (clients[monster].in_used == false)continue;
//
//		if ((clients[monster].m_x == left || clients[monster].m_x == right) && clients[monster].m_y == y) {
//			OVER_EX* over_ex = new OVER_EX;
//			over_ex->ev = EV_PLAYER_ATTACK_MONSER;
//			over_ex->target_player = id;  //공격한 플레이어
//
//
//			PostQueuedCompletionStatus(g_iocp, 1, monster, &over_ex->over);
//		}
//
//		else if ((clients[monster].m_y == down || clients[monster].m_y == up) && clients[monster].m_x == x) {
//			OVER_EX* over_ex = new OVER_EX;
//			over_ex->ev = EV_PLAYER_ATTACK_MONSER;
//			over_ex->target_player = id;  //공격한 플레이어
//
//			PostQueuedCompletionStatus(g_iocp, 1, monster, &over_ex->over);
//		}
//	}
//}
//void Process_chat_notice(int player_id, int monster_id, int monster_level, MESSAGE_TYPE type) {
//
//	sc_packet_chat_notice packet;
//	packet.size = sizeof(packet);
//	packet.type = SC_CHAT_NOTICE;
//	packet.monster_id = monster_id;
//	switch (type) {
//	case MT_ATTACK_PLAYER: {  //플레이어->몬스터 공격
//		packet.message_type = MT_ATTACK_PLAYER;
//		break;
//	}
//	case MT_DESTROY_MONSTER: { //플레이어 ->몬스터 죽임
//		packet.message_type = MT_DESTROY_MONSTER;
//
//		clients[player_id].m_lock.lock();
//		if (clients[player_id].viewlist.count(monster_id)) {
//			clients[player_id].viewlist.erase(monster_id);
//			clients[player_id].m_lock.unlock();
//		}
//		else
//			clients[player_id].m_lock.unlock();
//
//		int step = clients[player_id].m_quest_step;
//		bool is_quest = clients[player_id].m_is_quest;
//
//
//		//미라 퀘스트
//		if (step == 1 && 500 < monster_id && monster_id < 550 && is_quest) {
//			clients[player_id].m_stat_lock.lock();
//			clients[player_id].m_exp += (monster_level * 5);
//			clients[player_id].m_quest_killed_monster++;
//			clients[player_id].m_stat_lock.unlock();
//			send_request_npc(player_id, SC_QUEST);
//			//3마리를 잡으면
//			if (clients[player_id].m_quest_killed_monster >= 3) {
//
//				clients[player_id].m_exp += 50;
//				clients[player_id].m_quest_killed_monster = 0;
//				clients[player_id].m_is_quest = false;
//				send_request_npc(player_id, SC_QUEST_CLEAR);
//			}
//
//		}
//		//박쥐 퀘스트
//		else if (step == 2 && 550 < monster_id && monster_id < 600 && is_quest) {
//			clients[player_id].m_stat_lock.lock();
//			clients[player_id].m_exp += (monster_level * 5);
//			clients[player_id].m_quest_killed_monster++;
//			clients[player_id].m_stat_lock.unlock();
//			send_request_npc(player_id, SC_QUEST);
//			//4마리를 잡으면
//			clients[player_id].m_stat_lock.lock();
//			if (clients[player_id].m_quest_killed_monster >= 4) {
//				clients[player_id].m_stat_lock.unlock();
//
//				clients[player_id].m_exp += 70;
//				clients[player_id].m_quest_killed_monster = 0;
//				clients[player_id].m_is_quest = false;
//				send_request_npc(player_id, SC_QUEST_CLEAR);
//			}
//			else
//				clients[player_id].m_stat_lock.unlock();
//
//
//		}
//		//좀비 퀘스트
//		else if (step == 3 && 600 < monster_id && monster_id < 650 && is_quest) {
//			clients[player_id].m_stat_lock.lock();
//			clients[player_id].m_exp += (monster_level * 5);
//			clients[player_id].m_quest_killed_monster++;
//			clients[player_id].m_stat_lock.unlock();
//			send_request_npc(player_id, SC_QUEST);
//
//			//5마리를 잡으면
//			clients[player_id].m_stat_lock.lock();
//			if (clients[player_id].m_quest_killed_monster >= 5) {
//				clients[player_id].m_stat_lock.unlock();
//				clients[player_id].m_quest_killed_monster = 0;
//				clients[player_id].m_is_quest = false;
//				send_request_npc(player_id, SC_QUEST_CLEAR);
//			}
//			else
//				clients[player_id].m_stat_lock.unlock();
//
//
//		}
//		//퀘스트 안받은 상태
//		else {
//			clients[player_id].m_stat_lock.lock();
//			clients[player_id].m_exp += (monster_level * 5);
//			clients[player_id].m_stat_lock.unlock();
//
//		}
//
//		Process_player_level(player_id);
//
//		for (int i = 0; i < MAX_USER; ++i) {
//			if (clients[i].in_used == false)continue;
//			if (GetNearObject(player_id, i) == false)continue;
//			send_stat_change_packet(i, player_id);
//		}
//		break;
//	}
//	case MT_ATTATK_MONSTER: { //몬스터 -> 플레이어 공격
//		packet.message_type = MT_ATTATK_MONSTER;
//		break;
//	}
//	default:
//		std::cout << "not difend event" << std::endl;
//		while (1);
//		break;
//	}
//	send_packet(player_id, &packet);
//}
//void Process_Login(int id, char* packet) {
//	cs_packet_login* login_packet = reinterpret_cast<cs_packet_login*>(packet);
//
//
//	if (login_packet->is_user == true) {
//		EVENT_DB db;
//		db.type = EV_LOGIN_REQUEST;
//		db.id = id;
//		db.db_id = login_packet->player_id;
//
//		db_l.lock();
//		g_db_queue.push(db);
//		db_l.unlock();
//	}
//	else if (login_packet->is_user == false) {
//
//		if (login_packet->player_id[0] == 'D') {
//			clients[id].m_x = rand() % 100;
//			clients[id].m_y = rand() % 60;
//
//		}
//		OVER_EX* over_ex = new OVER_EX;
//		clients[id].in_used = true;
//		over_ex->ev = EV_LOGIN_OK;
//		PostQueuedCompletionStatus(g_iocp, 1, id, &over_ex->over);
//	
//	}
//
//
//	else { std::cout << "이상한값" << std::endl; }
//
//}
//void process_packet(int id, char *packet) {
//	switch (packet[1]) {
//	case CS_MOVE: {
//
//		if (clients[id].in_used == true) {
//			cs_packet_move* move_packet = reinterpret_cast<cs_packet_move*>(packet);
//
//			if (clients[id].m_is_move == false) {
//				clients[id].m_is_move = true;
//				EVENT_ST et;
//				et.obj_id = id;
//				et.dir = move_packet->direction;
//				et.start_time = high_resolution_clock::now();
//				et.type = EV_PLAYER_MOVE;
//				add_timer(et);
//			}
//		}
//		//Process_move(id,packet);
//		break;
//	}
//
//	case CS_ATTACK: {
//		if (clients[id].in_used == true) {
//			if (clients[id].m_is_attack == false) {
//				clients[id].m_is_attack = true;
//				EVENT_ST et;
//				et.obj_id = id;
//				et.start_time = high_resolution_clock::now() + 1s;
//				et.type = EV_PLAYER_ATTACK;
//				add_timer(et);
//			}
//		}
//
//		//Process_attack(id, packet);
//		break;
//	}
//	case CS_LOGIN:
//		Process_Login(id, packet);
//		break;
//	case CS_LOGOUT:
//		break;
//	case CS_REQULST_NPC: {
//		Process_request_npc(id, packet);
//		break; }
//
//	default:
//		std::wcout << "정의 되지 않은 패킷 도착 오류 \n";
//		while (true);
//		break;
//	}
//
//
//}
//void disconnect_client(int id) {
//
//	for (int i = 0; i < MAX_USER; ++i) {
//		if (false == clients[i].in_used) continue;
//		if (i == id) continue; // i가 나자신이면 보낼 필요가 없다.
//
//		clients[i].m_lock.lock();
//		if (0 != clients[i].viewlist.count(id)) {
//			clients[i].m_lock.unlock();
//			send_remove_player_packet(i, id);
//		}
//		else
//			clients[i].m_lock.unlock();
//
//	}
//
//	clients[id].m_lock.lock();
//	clients[id].viewlist.clear();
//	clients[id].in_used = false;
//	clients[id].m_lock.unlock();
//
//	closesocket(clients[id].socket);
//
//}
//bool GetNearObject(int a, int b) {
//
//	if (VIEW_SIZE < abs(clients[a].m_x - clients[b].m_x) || VIEW_SIZE < abs(clients[a].m_y - clients[b].m_y))
//		return false;
//	
//	return true;
//}
//bool GetNearObject_npc(int a, int b) {
//
//	if (VIEW_NPC < abs(clients[a].m_x - clients[b].m_x) || VIEW_NPC < abs(clients[a].m_y - clients[b].m_y))
//		return false;
//
//	return true;
//}
//
//void add_timer(int obj_id, EVENT_TYPE et, int target, high_resolution_clock::time_point start_time) {
//
//
//	timer_l.lock();
//	g_timer_queue.emplace(EVENT_ST{ obj_id,et,start_time });
//	timer_l.unlock();
//}
//void add_timer(EVENT_ST& et) {
//	timer_l.lock();
//	g_timer_queue.emplace(et);
//	timer_l.unlock();
//}
//
//
//void Process_event(EVENT_ST& ev) {
//
//	switch (ev.type) {
//	case EV_MOVE: {
//
//		bool player_is_near = false;
//
//		for (int i = 0; i < MAX_USER; ++i) {
//
//			if (clients[i].in_used == false) {
//				continue;
//			}
//
//			if (GetNearObject(i, ev.obj_id) == false)continue;
//
//			player_is_near = true;
//			break;
//		}
//		if (player_is_near) {
//			RandNPCMove(ev.obj_id);
//
//			WakeupNPC(ev.obj_id, ev.obj_id);
//
//
//			//add_timer(ev.obj_id, EV_MOVE, 0, high_resolution_clock::now() + 1s);
//		}
//		else {
//			clients[ev.obj_id].is_sleeping = true;
//		}
//		break;
//	}
//	case EV_MONSER_ATTACK_PLAYER: {
//		//방향 //플레이어 공격
//
//		auto px = clients[ev.target_id].m_x;
//		auto py = clients[ev.target_id].m_y;
//
//		auto mx = clients[ev.obj_id].m_x;
//		auto my = clients[ev.obj_id].m_y;
//		int player_id = ev.target_id;
//		int monster_id = ev.obj_id;
//
//
//			//몬스터 공격력
//			clients[player_id].m_lock.lock();
//			clients[player_id].m_hp -= 5;
//			clients[player_id].m_lock.unlock();
//
//			clients[player_id].m_lock.lock();
//			if (clients[player_id].m_hp <= 0) {
//				clients[player_id].m_hp = 100;
//				clients[player_id].m_x = 0;
//				clients[player_id].m_y = 0;
//				clients[player_id].m_exp /= 2;
//				clients[player_id].m_lock.unlock();
//
//				clients[player_id].m_lock.lock();
//				clients[player_id].viewlist.clear();
//				clients[player_id].m_lock.unlock();
//
//				send_position_pakcet(player_id, player_id);
//				send_stat_change_packet(player_id, player_id);
//			}
//			else {
//				clients[player_id].m_lock.unlock();
//				send_stat_change_packet(player_id, player_id);
//			}
//
//			//몬스터 기준으로 플레이어 왼쪽
//			if ((mx - px) == 1) {
//				clients[monster_id].m_character_shift = CHARACTER_LEFT;
//
//			}
//
//			//몬스터 기준으로 플레이어 오른쪽
//			else if ((mx - px) == -1) {
//				clients[monster_id].m_character_shift = CHARACTER_RIGHT;
//			}
//
//			//몬스터 기준으로 플레이어 위
//			else  if ((my - py) == 1) {
//				clients[monster_id].m_character_shift = CHARACTER_UP;
//			}
//
//			//몬스터 기준으로 플레이어 아래
//			else  if ((my - py) == -1) {
//				clients[monster_id].m_character_shift = CHARACTER_DOWN;
//
//			}
//			for (int i = 0; i < MAX_USER; ++i) {
//				if (clients[i].in_used == false)continue;
//				if (GetNearObject(i, player_id) == false)continue;
//				send_npc(i, monster_id);
//			}
//
//
//		break;
//	}
//	case EV_HEAL: {
//		break;
//	}
//	case EV_PLAYER_MOVE: {
//		Process_move(ev);
//		break;
//	}
//	case EV_PLAYER_ATTACK: {
//		Process_attack(ev);
//		break;
//	}
//	case EV_REVIVE: {
//
//		ReviveMonster(ev.obj_id);
//		break;
//	}
//	case EV_FOLLOWER_PLAYER: {
//		Process_follwer_player(ev);
//
//		if (GetNearObject_npc(ev.obj_id, ev.target_id) == true) {
//			OVER_EX* over_ex = new OVER_EX;
//			over_ex->ev = EV_FOLLOWER_PLAYER_DETECT;
//			over_ex->dir = ev.dir;
//			over_ex->target_player = ev.target_id;
//			PostQueuedCompletionStatus(g_iocp, 1, ev.obj_id, &over_ex->over);
//		}
//		else {
//			clients[ev.obj_id].m_is_npc_move = false;
//
//		}
//		break;
//	}
//
//	default:
//		std::cout << " Process Event Unknown Event Eror" << std::endl;
//		while (1);
//		break;
//	}
//
//
//}
//void Process_request_npc(int id, char* packet) {
//	int request_id = id;
//	cs_packet_request_npc* requst_packet = reinterpret_cast<cs_packet_request_npc*>(packet);
//
//	switch (requst_packet->requst_type) {
//	case CS_QUEST: {
//		send_request_npc(id, SC_QUEST);
//
//		if (clients[id].m_is_quest == false)
//			clients[id].m_is_quest = true;
//		break;
//	}
//	case CS_QUEST_REWARD: {
//
//		int step = clients[id].m_quest_step;
//
//		//퀘스트 받으면서 사냥을 할 일이 없을거같으니 lock처리를 안한다.
//		if (step == 1) {
//			clients[id].m_exp += 50;
//			clients[id].m_quest_step++;
//			Process_player_level(id);
//		}
//		else if (step == 2) {
//			clients[id].m_exp += 50;
//			clients[id].m_quest_step++;
//			Process_player_level(id);
//		}
//		else if (step == 3) {
//			clients[id].m_exp += 50;
//			Process_player_level(id);
//		}
//		send_stat_change_packet(id, id);
//
//		break;
//	}
//	case CS_HEAL: {
//		send_request_npc(id, SC_HEAL);
//		clients[id].m_hp = 100;
//		break;
//	}
//	case CS_SHOP: {
//		send_request_npc(id, SC_SHOP);
//		break;
//	}
//	default:
//		std::cout << "not defined requst_npc" << std::endl;
//		while (1);
//		break;
//	}
//
//}
//void Process_player_level(int player_id) {
//
//
//	// 1레벨 ->100 //2레벨 200-> 3레벨 400
//	if (clients[player_id].m_level == 1 && clients[player_id].m_exp >= 100) {
//		clients[player_id].m_stat_lock.lock();
//		clients[player_id].m_level++;
//		clients[player_id].m_exp %= 100;
//		clients[player_id].m_stat_lock.unlock();
//	}
//	//2->3
//	else if (clients[player_id].m_level == 2 && clients[player_id].m_exp >= 200) {
//		clients[player_id].m_stat_lock.lock();
//		clients[player_id].m_level++;
//		clients[player_id].m_exp %= 200;
//		clients[player_id].m_stat_lock.unlock();
//	}
//	//3---->
//	else if (clients[player_id].m_level == 3 && clients[player_id].m_exp >= 400) {
//		clients[player_id].m_stat_lock.lock();
//		clients[player_id].m_level++;
//		clients[player_id].m_exp %= 400;
//		clients[player_id].m_stat_lock.unlock();
//	}
//
//
//}
//void Process_follwer_player(EVENT_ST& ev) {
//	int npc_id = ev.obj_id;
//	int player_id = ev.target_id;
//	int x = clients[ev.obj_id].m_x;
//	int y = clients[ev.obj_id].m_y;
//	int dir = (int)ev.dir;
//
//	//이동하기전에 old리스트를 만든다.
//	std::unordered_set <int> old_vl;
//	for (int i = 0; i < MAX_USER; i++) {
//		if (false == clients[i].in_used) continue;
//		if (false == GetNearObject(npc_id, i)) continue;
//		old_vl.insert(i);
//	}
//
//	switch (dir) {
//	case CHARACTER_UP: {
//
//		if (clients[npc_id].m_y != (clients[player_id].m_y + 1)) {
//			if ((500 <= npc_id && npc_id < 550) && y > 0) --y;
//			else if ((550 <= npc_id && npc_id < 600) && y > 25) --y;
//			else if ((600 <= npc_id && npc_id < 650) && y > 50) --y;
//		}
//		clients[npc_id].m_character_shift = CHARACTER_UP;
//		break; }
//	case CHARACTER_DOWN: {
//		if (clients[npc_id].m_y != (clients[player_id].m_y - 1)) {
//
//			if ((500 <= npc_id && npc_id < 550) && y < 20) ++y;
//			else if ((550 <= npc_id && npc_id < 600) && y < 50) ++y;
//			else if ((600 <= npc_id && npc_id < 650) && y < 50) ++y;
//		}
//		clients[npc_id].m_character_shift = CHARACTER_DOWN;
//		break; }
//	case CHARACTER_LEFT: {
//		if (clients[npc_id].m_x != (clients[player_id].m_x + 1) || (clients[npc_id].m_y != clients[player_id].m_y)) {
//			if ((500 <= npc_id && npc_id < 550) && x > 20) --x;
//			else if ((550 <= npc_id && npc_id < 600) && x > 25) --x;
//			else if ((600 <= npc_id && npc_id < 650) && x > 0) --x;
//		}
//		clients[npc_id].m_character_shift = CHARACTER_LEFT;
//		break; }
//	case CHARACTER_RIGHT: {
//		if (clients[npc_id].m_x != (clients[player_id].m_x - 1) || (clients[npc_id].m_y != clients[player_id].m_y)) {
//
//			if ((500 <= npc_id && npc_id < 550) && x < 50)++x;
//			else if ((550 <= npc_id && npc_id < 600) && x < 50) ++x;
//			else if ((600 <= npc_id && npc_id < 650) && x < 20)++x;
//			clients[npc_id].m_character_shift = CHARACTER_RIGHT;
//		}
//		break; }
//	default:
//		std::cout << "error dir" << std::endl;
//
//		break;
//	}
//
//
//	clients[npc_id].m_x = x;
//	clients[npc_id].m_y = y;
//
//	std::unordered_set <int> nearList;
//
//	//모든 유저와 현재 npc와의 NearList를 만든다.
//	for (int i = 0; i < MAX_USER; ++i) {
//		if (clients[i].in_used == false)continue;
//		if (GetNearObject(npc_id, i) == false)continue;
//
//		nearList.insert(i);
//	}
//
//
//	//예전 list
//	for (auto user : old_vl) {
//
//		//User가 이동전 리스트와 이동후 리스트에 존재한다..
//		if (nearList.count(user) != 0) {
//
//			//현재 User의 viewlist에 npc_id가 있는지 확인
//			//있으면 npc정보를 해당 User에게 보낸다.
//			clients[user].m_lock.lock();
//			if (clients[user].viewlist.count(npc_id)) {
//				clients[user].m_lock.unlock();
//				send_npc(user, npc_id);
//			}
//
//			//현재 User의 viewIst에 npc_id가 없다.
//			//없기 때문에 npc의 정보를 User에게 보낸다.
//			else {
//				clients[user].viewlist.insert(npc_id);
//				clients[user].m_lock.unlock();
//				send_put_player_packet(user, npc_id);
//			}
//
//		}
//		//User가 이동 후 리스트에 없다.
//		else {
//
//			//해당 user에 지금 npc_id가 있으면
//			//지운다. 
//			//가까이 있는게 아니라 멀어진거이기 때문
//			clients[user].m_lock.lock();
//			if (clients[user].viewlist.count(npc_id) == 1) {
//				clients[user].viewlist.erase(npc_id);
//				clients[user].m_lock.unlock();
//				send_remove_player_packet(user, npc_id);
//			}
//			else {
//				clients[user].m_lock.unlock();
//			}
//
//		}
//
//	}
//
//	//이동 후 리스트에 있는 유저와 NPC
//	for (auto user : nearList) {
//		//이동 전에 리스트 정보에는 해당 npc_id가 없다.
//		if (old_vl.count(npc_id) == 0) {
//
//			//이동 후 리스트에도 현재 NPC의 정보가 없다.
//
//			clients[user].m_lock.lock();
//			if (clients[user].viewlist.count(npc_id) == 0) {
//
//				clients[user].viewlist.insert(npc_id);
//				clients[user].m_lock.unlock();
//				send_put_player_packet(user, npc_id);
//
//			}
//
//			else {
//				clients[user].m_lock.unlock();
//				send_npc(user, npc_id);
//			}
//		}
//
//
//	}
//
//
//
//}