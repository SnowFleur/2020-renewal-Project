#include"GameObject.h"
#include"InputComponent.h"

GameObject::~GameObject() {
    if (inputComponent_ != nullptr) {
        delete inputComponent_;
    }
}