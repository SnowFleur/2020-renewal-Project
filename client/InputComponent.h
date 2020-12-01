#pragma once

__interface IGameObject;
class CWorld;


constexpr int CHARACTER_DOWN    = 0;
constexpr int CHARACTER_LEFT    = 1;
constexpr int CHARACTER_RIGHT   = 2;
constexpr int CHARACTER_UP      = 3;


class CInputComponent{
private:
public:
    virtual ~CInputComponent() {};
    virtual void Update(IGameObject&, CWorld&) = 0;
};

