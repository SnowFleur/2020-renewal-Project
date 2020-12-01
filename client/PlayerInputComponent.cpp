#include"PlayerInputComponent.h"
#include"PlayerObject.h"
#include"World.h"
#include"Controller.h"
#include"Locator.h"
#include"Network.h"
#include"Sector.h"

#include<iostream>

/*
Ŭ���̾�Ʈ���� �̵� �� �浹ó���� �ϰ� ������ ������
*/

/*
[2020-10-24]
�ϴ� ����ġ���� Ű�� ������ �� �ش��ϴ� �𷺼��� ������ ���� �ϳ��� ���� �� �ֵ��� �����ϸ� ������ ����
�ؿ� �浹üũ �κ��� �� �����Ӹ��� Ȯ���� �ϴϱ� ���� ��� (���� �� ����)
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

    /*�ش� Ÿ�Ͽ� �̵��� ������ �� ���� ��ġ�� ����*/
    //20.10.05: �ӽ÷� y�� �ִ븦 15�� �ٲ� ���߿��� 30���� ������
    if ((x >= 0 && y >= 0) && (x < MAP_DEFINDS::SECTOR_SIZE && y < 15)) {
        if (world.GetTile(x, y)->MoveAble()) {
            //���� Sector���� ���� �ٲ���
            world.GetSectorHandle()->ChangeSector(&gameobject, x, y);
            gameobject.SetPosition(x, y);

            INetwork* handle = CLocator::GetNetwork();
            handle->SendPlayerPosition(x, y, direction);

            //�÷��̾� ī�޶� �̵�
            g_left_x = x - 5;
            g_top_y = y - 5;
        }
    }

    Controller::SetDirection(ControllerDirection::CONTROLLER_NON);
}

