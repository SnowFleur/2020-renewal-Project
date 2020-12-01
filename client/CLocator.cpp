#include "Locator.h"
#include"Network.h"


INetwork* CLocator::netWorkHandle_ = nullptr;

INetwork* CLocator::GetNetwork() {
    return netWorkHandle_;
}

void CLocator::Provide(INetwork* network) {
    if (netWorkHandle_ == nullptr) {
        netWorkHandle_ = network;
    }
}