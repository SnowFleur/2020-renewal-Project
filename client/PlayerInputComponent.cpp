#include"PlayerInputComponent.h"
#include"PlayerObject.h"
#include"World.h"
#include"Controller.h"
#include"Locator.h"
#include"Network.h"
#include"Sector.h"

#include<iostream>

/*
클라이언트에서 이동 및 충돌처리를 하고 서버에 보내자
*/

/*
[2020-10-24]
일단 스위치에서 키가 눌르면 그 해당하는 디렉션을 보내게 했음 하나로 보낼 수 있도록 통합하면 좋을거 같고
밑에 충돌체크 부분이 매 프레임마다 확인을 하니까 수정 요망 (수정 후 삭제)
*/

void CPlayerInputComponent::Update(IGameObject& gameobject, CWorld& world) {

   if (Controller::GetDirection() == ControllerDirection::CONTROLLER_NON)return;

    unsigned short x{}, y{};
    char direction{};
    gameobject.GetPosition(x, y);

    switch (Controller::GetDirection()) {
    case ControllerDirection::CONTROLLER_DOWN: {

        direction = CHARACTER_DOWN;
        gameobject.SetRenderCharacterDirection(direction);
        if (world.GetSectorHandle()->Collision(&gameobject, COLLISION_TYPE::MOVE_DOWN) == false) {
            y += 1;
        }
        break;
    }
    case ControllerDirection::CONTROLLER_LEFT: {
        direction = CHARACTER_LEFT;
        gameobject.SetRenderCharacterDirection(direction);
        if (world.GetSectorHandle()->Collision(&gameobject, COLLISION_TYPE::MOVE_LEFT) == false) {
            x -= 1;
        }
        break;
    }
    case ControllerDirection::CONTROLLER_RIGHT: {
        direction = CHARACTER_RIGHT;
        gameobject.SetRenderCharacterDirection(direction);
        if (world.GetSectorHandle()->Collision(&gameobject, COLLISION_TYPE::MOVE_RIGHT) == false) {
            x += 1;
        }
        break;
    }
    case ControllerDirection::CONTROLLER_UP: {
        direction = CHARACTER_UP;
        gameobject.SetRenderCharacterDirection(direction);
        if (world.GetSectorHandle()->Collision(&gameobject, COLLISION_TYPE::MOVE_UP) == false) {
            y -= 1;
        }
        break;
    }
    case ControllerDirection::CONTROLLER_ATTACK: {
        world.GetSectorHandle()->Collision(&gameobject, COLLISION_TYPE::ATTACK);
        break;
    }
    default:
        break;
    }

    /*해당 타일에 이동이 가능할 때 에만 위치값 변경*/
    //20.10.05: 임시로 y값 최대를 15로 바꿈 나중에는 30으로 변경요망
    if ((x >= 0 && y >= 0) && (x < MAP_DEFINDS::SECTOR_SIZE && y < 15)) {
        if (world.GetTile(x, y)->MoveAble()) {
            //먼저 Sector에서 값을 바꾸자
            world.GetSectorHandle()->ChangeSector(&gameobject, x, y);
            gameobject.SetPosition(x, y);

            INetwork* handle = CLocator::GetNetwork();
            handle->SendPlayerPosition(x, y, direction);

            //플레이어 카메라 이동
            g_left_x = x - 5;
            g_top_y = y - 5;
        }
    }

    Controller::SetDirection(ControllerDirection::CONTROLLER_NON);
}

