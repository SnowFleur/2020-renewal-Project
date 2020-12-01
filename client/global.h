#pragma once

#define WIN32_LEAN_AND_MEAN  
#define INITGUID

#define IMAGE_SIZE 64


#define MAX(a,b)	((a)>(b))?(a):(b)
#define	MIN(a,b)	((a)<(b))?(a):(b)

// defines for windows 
#define WINDOW_CLASS_NAME L"WINXCLASS"  // class name

#define WINDOW_WIDTH    680+65   // size of window
#define WINDOW_HEIGHT   730+65



#define NUMBER_SIZE 12

//타일 사이즈
//#define TILE_WIDTH 65
#define TILE_WIDTH 34
#define UNIT_TEXTURE  0

//일단 합치자

#include<string>
#include <d3d9.h>     // directX includes
#include "d3dx9tex.h"     // directX includes
#include"gpdumb1.h"


#include"..\server\Protocol.h"



namespace CLIENT_OBJECT_INDEX {
    constexpr int START_AI = OBJECT_DEFINDS::MAX_USER;
}

#define ATTACK_KEY 0x41



