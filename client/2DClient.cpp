// PROG14_1_16b.CPP - DirectInput keyboard demo


#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")


//삭제 예정(기존)
#include"global.h"


//새로 추가
#include"PlayerObject.h"
#include"PlayerGraphicsComponent.h"
#include"PlayerInputComponent.h"
#include"World.h"

#include"Controller.h"
#include"Socket.h"
#include"GameObject.h"
#include"Locator.h"
#include"ObjectTextureCreater.h"
//몬스터
#include"MonsterObject.h"
#include"MonsterGraphicsComponent.h"
#include"MonsterInputComponent.h"
//NPC
#include"NpcObject.h"
//Spawner
#include"Spawner.h"
//Sector
#include"Sector.h"
//UI
#include"PlayerUI.h"

#pragma region Player Camera Global Variable
// Extern Variable From gpdumb1.h
// 프레임워크에 정의된 변수라 그냥 사용하자
int g_left_x{}, g_top_y{};
#pragma endregion

//Test
#include<chrono>

// game console
int Game_Init(void* parms = NULL);
int Game_Shutdown(void* parms = NULL);
int Game_Main(void* parms = NULL);

// GLOBALS ////////////////////////////////////////////////

HWND main_window_handle = NULL; // save the window handle
HINSTANCE main_instance = NULL; // save the instance

HWND chat_window_handle = NULL; // save the window handle
HINSTANCE chat_instance = NULL; // save the instance

                                // demo globals
//BOB			npc[MAX_NPC];      // NPC Unit
//BOB         skelaton[MAX_USER];     // the other player skelaton


//새로 적용
#pragma region NEW Global Variable
//Player를 제외한 모든 오브젝트(Other Player,AI,NPC_통합
IGameObject* g_GameObjects[OBJECT_DEFINDS::MAX_GAMEOBJECT];
IGameObject* g_playerObject = nullptr;

CWorld* g_wordlHandle = new CWorld{ new CSector() };
#pragma endregion



LRESULT CALLBACK WindowProc(HWND hwnd,
    UINT msg,
    WPARAM wparam,
    LPARAM lparam)
{
    // this is the main message handler of the system
    PAINTSTRUCT	ps;		   // used in WM_PAINT
    HDC			hdc;	   // handle to a device context


    // what is the message 
    switch (msg) {
    case WM_KEYDOWN: {
        /*
        이동을 하고 -> Update -> Send  SendQueue를 만들어야겠지
        */
        switch (wparam) {
        case VK_RIGHT:
            Controller::SetDirection(ControllerDirection::CONTROLLER_RIGHT);
            break;
        case VK_LEFT:
            Controller::SetDirection(ControllerDirection::CONTROLLER_LEFT);
            break;
        case VK_UP:
            Controller::SetDirection(ControllerDirection::CONTROLLER_UP);
            break;
        case VK_DOWN:
            Controller::SetDirection(ControllerDirection::CONTROLLER_DOWN);
            break;
        case ATTACK_KEY:
            Controller::SetDirection(ControllerDirection::CONTROLLER_ATTACK);

            //switch (Interaction_npc(my_x, my_y)) {
            //case QUEST_NPC: {

            //	//Quest 요청인지 Quest 완료 요청인지 판단
            //	if (std::dynamic_pointer_cast<QuestNpc>(g_village_npc[QUEST_NPC])->GetType()==SC_QUEST_CLEAR) {

            //	SendEventPlayer(EVENT_PLAYER::EP_REQUEST_QUEST_CELAR);
            //	}
            //	else {
            //		SendEventPlayer(EVENT_PLAYER::EP_REQUEST_QUEST);
            //	}

            //	break;
            //}
            //case HEAL_NPC: {
            //	SendEventPlayer(EVENT_PLAYER::EP_REQUEST_HEAL);
            //	break;
            //}
            //case SHOP_NPC: {
            //	SendEventPlayer(EVENT_PLAYER::EP_REQUEST_SHOP);
            //	break;
            //}
            //default:
            //	SendEventPlayer(EVENT_PLAYER::EP_ATTACK);
            //	break;
            //}

            break;
        case VK_RETURN: {
            CPlayerUI::InputChat(wparam);
            break;
        }
        default:
            break;
        }
        break;
    }
    case WM_CREATE: {
        // do initialization stuff here
        return(0);
    } break;
    case WM_CHAR: {
        //Chating 프로그램은 잠시 멈추자
        //CPlayerUI::InputChat(wparam);
        break;
    }
    case WM_PAINT: {
        //hdc = BeginPaint(hwnd, &ps);

        /*for (int i = 0; i < 3; ++i) {
            g_village_npc[i]->SetWindow(hdc);
        }*/

        // end painting
        //EndPaint(hwnd, &ps);
        return(0);
    } break;


    case WM_DESTROY: {
        // kill the application			
        PostQuitMessage(0);
        return(0);
    } break;

    case WM_SOCKET: {
        INetwork* handle = CLocator::GetNetwork();
        if (WSAGETSELECTERROR(lparam)) {
            handle->CloseSocket();
            break;
        }
        switch (WSAGETSELECTEVENT(lparam)) {
        case FD_READ:
            handle->ReadPacket();
            break;
        case FD_CLOSE:
            handle->CloseSocket();
            break;
        }
        break;
    }

    default: {
        break;
    }

    } // end switch

      // process any messages that we didn't take care of 
    return (DefWindowProc(hwnd, msg, wparam, lparam));

} // end WinProc

  // WINMAIN ////////////////////////////////////////////////

int WINAPI WinMain(HINSTANCE hinstance,
    HINSTANCE hprevinstance,
    LPSTR lpcmdline,
    int ncmdshow)
{
    // this is the winmain function

    WNDCLASS winclass;	// this will hold the class we create
    HWND	 hwnd;		// generic window handle
    MSG		 msg;		// generic message


    // first fill in the window class stucture
    winclass.style = CS_DBLCLKS | CS_OWNDC |
        CS_HREDRAW | CS_VREDRAW;
    winclass.lpfnWndProc = WindowProc;
    winclass.cbClsExtra = 0;
    winclass.cbWndExtra = 0;
    winclass.hInstance = hinstance;
    winclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    winclass.hCursor = LoadCursor(NULL, IDC_ARROW);
    winclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    winclass.lpszMenuName = NULL;
    winclass.lpszClassName = WINDOW_CLASS_NAME;


    // register the window class
    if (!RegisterClass(&winclass))
        return(0);

    // create the window, note the use of WS_POPUP
    if (!(hwnd = CreateWindow(WINDOW_CLASS_NAME, // class
        L"Simple 2DMMORPG",	 // title
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        0, 0,	   // x,y
        WINDOW_WIDTH,  // width
        WINDOW_HEIGHT, // height
        NULL,	   // handle to parent 
        NULL,	   // handle to menu
        hinstance,// instance
        NULL)))	// creation parms
        return(0);

    // save the window handle and instance in a global
    //이거 지우지 말것
    main_window_handle = hwnd;
    main_instance = hinstance;

#pragma region SocketInit
    //중재자 패턴에 Socket값 Init
    CLocator::Provide(new CSocket((SOCKET_TYPE::TCP),
        SERVER_ENDPOINT, SERVER_PORT, hwnd));

    //Input ID
  /*  std::wstring id;
    std::cout << "ID:";
    std::wcin >> id;
    CLocator::GetNetwork()->SendLogin(id.c_str(), id.size());
*/

// perform all game console specific initialization
    Game_Init();

#pragma endregion
    // enter main event loop
    while (1)
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            // test if this is a quit
            if (msg.message == WM_QUIT)
                break;

            // translate any accelerator keys
            TranslateMessage(&msg);

            // send the message to the window proc
            DispatchMessage(&msg);
        } // end if

          // main game processing goes here
        Game_Main();

    } // end while

      // shutdown game and release all resources
    Game_Shutdown();

    // return to Windows like this
    return(msg.wParam);

} // end WinMain

  ///////////////////////////////////////////////////////////

  // WINX GAME PROGRAMMING CONSOLE FUNCTIONS ////////////////

int Game_Init(void* parms) {

    // set up screen dimensions
    screen_width = WINDOW_WIDTH;
    screen_height = WINDOW_HEIGHT;
    screen_bpp = 32;

    // initialize directdraw
    DD_Init(screen_width, screen_height, screen_bpp);

    /*
    World 객체 초기화
    */
    g_wordlHandle->GenerateTerrain();

    /*
    Player 객체 초기화
    */
    {
        auto texture = CObjectTextureCreater::SetGameObjectTextre(L"resource/player.PNG", 770, 70, 4);
        g_playerObject = new CPlayerObject(new CPlayerInputComponent, new CPlayerGraphicsComponent);
        g_playerObject->SetCharacterTexture(texture, 4);

    }
    /*
    Other Player 객체 초기화
    */
    {
        auto texture = CObjectTextureCreater::SetGameObjectTextre(L"resource/OtherPlayer.PNG", 770, 70, 4);
        //내 입장에서 다른사람도 결국 AI이기 때문에 MonsterComponent를 넣는다. (나중에 다른 Texture삽입)
        for (int i = 0; i < CLIENT_OBJECT_INDEX::START_AI; ++i) {
            g_GameObjects[i] = new CPlayerObject(new CMonsterInputComponent, new CPlayerGraphicsComponent);
            g_GameObjects[i]->SetCharacterTexture(texture, 4);
        }
    }


    /*
    Montser 객체 초기화
    추후 종류별로 나누자 패턴하나 본거같은데..
    */
    {
        auto texture = CObjectTextureCreater::SetGameObjectTextre(L"resource/monster1.PNG", 770, 70, 4);
        //MAX_GAME_OBJECT-3
        for (int i = CLIENT_OBJECT_INDEX::START_AI; i < OBJECT_DEFINDS::MAX_GAMEOBJECT; ++i) {
            g_GameObjects[i] = new CMonsterObject(new CMonsterInputComponent, new CMonsterGraphicsComponent);
            g_GameObjects[i]->SetCharacterTexture(texture, 4);
        }
    }

    /*
    Initliazation 순서 신경쓰자
    */

#pragma region Initlization NPC
    ////Set Heal NPC 
    //g_GameObjects[OBJECT_DEFINDS::MAX_GAMEOBJECT - 3] = new CNpcObject(new CMonsterInputComponent, new CMonsterGraphicsComponent);
    //g_GameObjects[OBJECT_DEFINDS::MAX_GAMEOBJECT - 3]->SetCharacterTexture(L"resource/npc1.PNG", 770, 70);
    //dynamic_cast<CNpcObject*>(g_GameObjects[OBJECT_DEFINDS::MAX_GAMEOBJECT - 3])->SetNpcType(NPC_TYPE::HEAL_NPC);
    //
    ////Set Quest NPC 
    //g_GameObjects[OBJECT_DEFINDS::MAX_GAMEOBJECT - 2] = new CNpcObject(new CMonsterInputComponent, new CMonsterGraphicsComponent);
    //g_GameObjects[OBJECT_DEFINDS::MAX_GAMEOBJECT - 2]->SetCharacterTexture(L"resource/npc3.PNG", 770, 70);
    //dynamic_cast<CNpcObject*>(g_GameObjects[OBJECT_DEFINDS::MAX_GAMEOBJECT - 2])->SetNpcType(NPC_TYPE::QUEST_NPC);
    //
    ////Set SHOP_NPC NPC 
    //g_GameObjects[OBJECT_DEFINDS::MAX_GAMEOBJECT - 1] = new CNpcObject(new CMonsterInputComponent, new CMonsterGraphicsComponent);
    //g_GameObjects[OBJECT_DEFINDS::MAX_GAMEOBJECT - 1]->SetCharacterTexture(L"resource/npc2.PNG", 770, 70);
    //dynamic_cast<CNpcObject*>(g_GameObjects[OBJECT_DEFINDS::MAX_GAMEOBJECT - 1])->SetNpcType(NPC_TYPE::SHOP_NPC);
#pragma endregion

    //CSpawner* zombieSpawner = new CSpawnerFor<CMonsterObject>();

    // return success
    return(1);

} // end Game_Init

  ///////////////////////////////////////////////////////////

int Game_Shutdown(void* parms)
{
    // this function is where you shutdown your game and
    // release all resources that you allocated

    // kill the reactor
 /*   Destroy_Bitmap32(&black_tile);
    Destroy_Bitmap32(&white_tile);*/

    // kill skelaton
    //for (int i = 0; i < MAX_USER; ++i) Destroy_BOB32(&skelaton[i]);

    //for (int i = 0; i < MAX_NPC; ++i)
    //	Destroy_BOB32(&npc[i]);

    // shutdonw directdraw
    DD_Shutdown();

#pragma region Delete My Object

    //delete[] g_GameObjects;
    //Network 나중에 지우기

#pragma endregion

    // return success
    return(1);
} // end Game_Shutdown

  ///////////////////////////////////////////////////////////

int Game_Main(void* parms) {

    // this is the workhorse of your game it will be called
    // continuously in real-time this is like main() in C
    // all the calls for you game go here!
    // check of user is trying to exit
    //if (KEY_DOWN(VK_ESCAPE) || KEY_DOWN(VK_SPACE))
    //	PostMessage(main_window_handle, WM_DESTROY, 0, 0);

    /*if (KEY_DOWN(VK_ESCAPE))
        PostMessage(main_window_handle, WM_DESTROY, 0, 0);
*/

// start the timing clock
    Start_Clock();

    // clear the drawing surface
    DD_Fill_Surface(D3DCOLOR_ARGB(255, 0, 0, 0));

    // get player input
    g_pd3dDevice->BeginScene();

    g_pSprite->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE);
    g_wordlHandle->Draw();
    g_pSprite->End();

    g_pSprite->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE);

    /*새로 추가*/
    /* Update GameObject*/

    g_playerObject->Update(*g_wordlHandle);

    // 20.10.25 최적화 해야함 너무 느림 천개이상 넣으면 
    // 우선순위는 뒤로 미루기
    // 
    for (int i = 0; i < OBJECT_DEFINDS::MAX_GAMEOBJECT; ++i) {
        if (g_GameObjects[i]->GetUsed() == false)continue;
        g_GameObjects[i]->Update(*g_wordlHandle);
    }

    g_pSprite->End();
    g_pd3dDevice->EndScene();

    // flip the surfaces
    DD_Flip();

    // sync to 3o fps
    //Wait_Clock(30);

    // return success
    return(1);

} // end Game_Main

  //////////////////////////////////////////////////////////

//
//NPC_TYPE Interaction_npc(int x, int y) {
//
//	//int rx, ry;
//	//int result_x, result_y;
//	//for (int i = 0; i < 3; ++i) {
//	//	g_village_npc[i]->GetPosition(rx, ry);
//
//	//	result_x = rx - x;
//	//	result_y = ry - y;
//
//	//	if (result_x == -1 && y == ry) {
//	//		return g_village_npc[i]->GetNpcType();
//	//	}
//	//	if (result_x == 1 && y == ry) {
//
//	//		return g_village_npc[i]->GetNpcType();
//	//	}
//	//	if (rx == x && result_y == -1) {
//	//		return g_village_npc[i]->GetNpcType();
//	//	}
//	//	if (rx == x && result_y == 1) {
//	//		return g_village_npc[i]->GetNpcType();
//	//	}
//
//	//}
//	return NULL_NPC;
//}

#pragma region ProcessPacketEvent
//지금은 이 방법밖에 생각이 안남
void ProcessPacketEvent(char* ptr){
    switch (ptr[1]) {
    case SC_LOGIN_OK: {
        sc_packet_login_ok* packet =
            reinterpret_cast<sc_packet_login_ok*>(ptr);
        g_playerObject->InitGameObject(packet);
        g_left_x = packet->x - 5;
        g_top_y = packet->y - 5;

        g_wordlHandle->GetSectorHandle()->ChangeSector(g_playerObject, packet->x, packet->y);
        break;
    }
    case SC_LOGIN_FAIL: {
        break;
    }
    case SC_ADD_OBJECT: {
        sc_packet_add_object* packet = reinterpret_cast<sc_packet_add_object*>(ptr);
        ObjectIDType id = packet->addID;

        switch (packet->objectClass) {
        case OBJECT_DEFINDS::OTHER_PLAYER: {
            g_GameObjects[id]->SetPosition(packet->x, packet->y);
            g_GameObjects[id]->SetVisibility(VISIBILITY_OPTION::VISIABLE);
            g_GameObjects[id]->SetUsed(true);
            g_wordlHandle->GetSectorHandle()->Add(g_GameObjects[id]);
            break;
        }
        case OBJECT_DEFINDS::MONSTER: {
            std::cout << "Add Monster\n";
            //id += CLIENT_OBJECT_INDEX::START_AI;
            g_GameObjects[id]->SetPosition(packet->x, packet->y);
            g_GameObjects[id]->SetVisibility(VISIBILITY_OPTION::VISIABLE);
            g_GameObjects[id]->SetUsed(true);
            g_wordlHandle->GetSectorHandle()->Add(g_GameObjects[id]);
            break;
        }
        default:
            printf("Unknown SC_ADD_OBJECT type [%d]\n", ptr[1]);
            break;
        }
        break;
    }
    case SC_MOVE_OBJECT: {
        sc_packet_move_object* my_packet = reinterpret_cast<sc_packet_move_object*>(ptr);
        int id = my_packet->movedID;


        switch (my_packet->objectClass) {
        case OBJECT_DEFINDS::OTHER_PLAYER: {
            g_wordlHandle->GetSectorHandle()->ChangeSector(g_GameObjects[id], my_packet->x, my_packet->y);
            g_GameObjects[id]->SetPosition(my_packet->x, my_packet->y);
            g_GameObjects[id]->SetRenderCharacterDirection(my_packet->textureDirection);
            break;
        }
        case OBJECT_DEFINDS::MONSTER: {
            //id += CLIENT_OBJECT_INDEX::START_AI;
            g_wordlHandle->GetSectorHandle()->ChangeSector(g_GameObjects[id], my_packet->x, my_packet->y);
            g_GameObjects[id]->SetPosition(my_packet->x, my_packet->y);
            g_GameObjects[id]->SetRenderCharacterDirection(my_packet->textureDirection);
            break;
        }
        default:
            printf("Unknown SC_MOVE_OBJECT type [%d]\n", ptr[1]);
            break;
        }
        break;
    }
    case SC_REMOVE_OBJECT: {

        /*
        이거는 메모리를 지우는게 아니라
        렌더를 지우는거임
        */
        sc_packet_remove_object* my_packet = reinterpret_cast<sc_packet_remove_object*>(ptr);
        int id = my_packet->removeID;

        switch (my_packet->objectClass) {
        case OBJECT_DEFINDS::OTHER_PLAYER: {
            g_GameObjects[id]->SetVisibility(VISIBILITY_OPTION::INVISIABLE);
            g_GameObjects[id]->SetUsed(false);
            g_wordlHandle->GetSectorHandle()->Remove(g_GameObjects[id]);
            break;
        }
        case OBJECT_DEFINDS::MONSTER: {
            std::cout << "Remove Monster: " << (int)id << "\n";

            //id += CLIENT_OBJECT_INDEX::START_AI;
            g_GameObjects[id]->SetVisibility(VISIBILITY_OPTION::INVISIABLE);
            g_GameObjects[id]->SetUsed(false);
            g_wordlHandle->GetSectorHandle()->Remove(g_GameObjects[id]);
            break;
        }
        default:
            printf("Unknown SC_REMOVE_OBJECT type [%d]\n", ptr[1]);
            break;
        }
        break;
    }
                         /*여기부터는 아직 안함*/
#pragma region Temp
                       
    //case SC_MONSTER_INFO: {
    //    sc_packet_monsterInfo* packet =
    //        reinterpret_cast<sc_packet_monsterInfo*>(ptr);
    //    int id = packet->id;
    //    //g_monster[id-500]->SetMonsterInfor(packet);


    //    break;
    //}
    //case SC_CHAT_NOTICE: {
    //    sc_packet_chat_notice* packet =
    //        reinterpret_cast<sc_packet_chat_notice*>(ptr);

    //    //g_screenManager->InsertChat(packet);


    //    break;
    //}
                      //case SC_REQUEST_NPC: {
                      //    sc_packet_request_npc* my_packet = reinterpret_cast<sc_packet_request_npc*>(ptr);

                      //    /*switch (my_packet->requst_type){
                      //    case SC_QUEST:
                      //    case SC_QUEST_CLEAR:{
                      //        g_village_npc[QUEST_NPC]->RecvRequest(my_packet);
                      //        break;
                      //    }
                      //    case SC_HEAL: {
                      //        g_player->ProcessHeal();
                      //        g_village_npc[HEAL_NPC]->RecvRequest(my_packet);
                      //        break;
                      //    }
                      //    case SC_SHOP: {
                      //        g_village_npc[SHOP_NPC]->RecvRequest(my_packet);
                      //        break;
                      //    }
                      //    default:
                      //        std::cout << "not defined requst_type" << std::endl;
                      //        break;
                      //    }
                      //    break;*/
                      //}
                      //case SC_STAT_CHANGE: {
                      //    sc_packet_stat_change* my_packet = reinterpret_cast<sc_packet_stat_change*>(ptr);


                      //    //if (my_packet->id == g_myid) {
                      //    //	g_player->SetStatChange(my_packet);
                      //    //}
                      //    //else {
                      //    //	g_OtherPlayer[my_packet->id]->SetStatChange(my_packet);
                      //    //}

                      //    break;
                      //}
#pragma endregion
    default:
        printf("Unknown PACKET type [%d] Size[%d]\n", ptr[1], ptr[2]);

    }
}
#pragma endregion
