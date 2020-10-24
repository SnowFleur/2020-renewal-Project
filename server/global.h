//#define _CRT_SECURE_NO_WARNINGS
//#define UNICODE
//#pragma once
//#pragma comment(lib, "lua53.lib")
//
//#include <iostream>
//#include <vector>
//#include <thread>
//#include <queue>
//#include <sqlext.h>  
//#include "Protocol.h"
//#include<locale.h>
//
//extern "C" {
//#include"include/lua.h"
//#include"include/lauxlib.h"
//#include"include/lualib.h"
//}
//
//
///*
//분활은 나중에...
//*/
//class MONSTERINFO {
//public:
//	//DB
//	int m_hp,m_x, m_y, m_level, m_exp, m_quest_step, m_quest_killed_monster;
//	int m_character_shift;
//	wchar_t* db_id = nullptr;
//	BOOL m_login=FALSE;
//	BOOL m_is_quest = false;
//	bool in_used; // 사용하고 있는 지를 확인
//
//	bool is_sleeping;
//	std::mutex lua_lock;
//	lua_State* L;
//	bool m_is_move = false;
//	bool m_is_npc_move = false;
//	bool m_is_attack = false;
//};
//
//
//class SOCKETINFO : public MONSTERINFO {
//
//public:
//	char packet_buffer[MAX_BUFFER]; // 패킷 처리시 찌꺼기 처리 용 버퍼
//	int prev_size;  // 패킷 버퍼에 몇 사이즈가 들어있는 지를 확인
//	SOCKETINFO() {
//		in_used = false;
//		ZeroMemory(&over_ex.over, sizeof(over_ex.over));
//		over_ex.dataBuffer.len = MAX_BUFFER;
//		over_ex.dataBuffer.buf = over_ex.messageBuffer;
//		over_ex.ev = EV_RECV;
//
//		//Lua 초기화
//		L = luaL_newstate(); //가상머신을 만들어서 리턴해준다.
//		//표준라이브러리를 넣어준다.
//		luaL_openlibs(L); //가상머신에 넣어준다.
//
//		luaL_loadfile(L, "monster_ai.lua");
//		lua_pcall(L, 0, 0, 0);
//
//	}
//};
//
///*
//Lua API Funcitons
//*/
//int API_GET_X(lua_State*);
//int API_GET_Y(lua_State*);
//int API_SendMessage(lua_State*);
//int API_WakeupNPC(lua_State* L);
//int API_SleepNPC(lua_State* L);
//int API_add_timer(lua_State* L);
//int API_SendMonsterInfo(lua_State* L);
//int API_SendDamageMonster(lua_State* L);
//int API_Requst_Revive(lua_State* L);
//int API_MonsterAttack(lua_State* L);
//int API_GetNearPlayer(lua_State* L);
//int API_FollowerPlayer(lua_State* L);
//void lua_error(lua_State *L, const char *fmt, ...) {
//
//	va_list argp;
//	va_start(argp, fmt);
//	vfprintf(stderr, fmt, argp);
//	va_end(argp);
//	lua_close(L);
//	system("pause");
//	exit(EXIT_FAILURE);
//
//}
//
///*
//send functions
//*/
//void send_packet(int client, void *packet);
//void send_position_pakcet(int client, int player);
//void send_remove_player_packet(int client, int id);
//void send_login_ok_packet(int new_id);
//void send_monsterInfo_packet(int id, int player_id, int level, int hp, wchar_t* name);
//void send_put_player_packet(int client, int new_id);
//void send_npc(int,int);
//void send_request_npc(int id,REQUST_TYPE type);
//void send_stat_change_packet(int id,int stat_change_id);
//void send_chat_packet(int, int,wchar_t*);
////void send_login_false_packet(int new_id);
//
///*
//recv Process functions
//*/
//void process_packet(int id, char *packet);
//void Process_event(EVENT_ST&);
//void Process_move(EVENT_ST&);
//void Process_attack(EVENT_ST&);
//void Process_follwer_player(EVENT_ST&);
//void Process_request_npc(int id,char* packet);
//void Process_Login(int id, char* packet);
//void Process_chat_notice(int player_id,int monster_id,int monster_level, MESSAGE_TYPE type);
//void Process_player_level(int player_id);
///*
//Thread functions
//*/
//void worker_thread();
//void Timer_thread();
//int do_accept();
////void DB_thread();
//
///*
//npc functions
//*/
//void InitNPC();
//void ReviveMonster(int id);
//bool is_npc(int id);
//bool is_Sleeping(int id);
//void RandNPCMove(int );
//void WakeupNPC(int id,int player_id);
//void WakeupMoveNPC(int id,int player_id);
///*
//
//*/
//
//void disconnect_client(int id);
//bool GetNearObject(int a, int b);
//bool GetNearObject_npc(int a, int b);
//
//void error_display(const char *message, int error_no);
//void add_timer(int obj_id, EVENT_TYPE et,int target,high_resolution_clock::time_point start_time);
//void add_timer(EVENT_ST&);
//void add_moteTImer(EVENT_ST&);
////void DB_Dis_error(SQLHANDLE hHandle, SQLSMALLINT hType, RETCODE RetCode);
//
///*전역변수*/
//HANDLE g_iocp;
//SOCKETINFO clients[MAX_USER + NUM_NPC];
//
//std::mutex timer_l;
//std::priority_queue<EVENT_ST>g_timer_queue;
//
//std::mutex db_l;
//std::queue<EVENT_DB>g_db_queue;
//
