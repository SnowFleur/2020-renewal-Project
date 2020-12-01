#pragma once

enum class ControllerDirection {
    CONTROLLER_NON, CONTROLLER_LEFT, CONTROLLER_RIGHT,
    CONTROLLER_UP, CONTROLLER_DOWN, CONTROLLER_ATTACK
};


class Controller {
private:
    Controller() = default;
    static ControllerDirection direction_;
public:
    static void SetDirection(ControllerDirection direction) {
        direction_ = direction;
    }
    static ControllerDirection GetDirection() {
        return direction_;
    }
};


