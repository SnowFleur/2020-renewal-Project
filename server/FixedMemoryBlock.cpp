#include<algorithm>
#include<stdlib.h>
#include "FixedMemoryBlock.h"


CFixedMemoryBlock::CFixedMemoryBlock(size_t fixedMemorySize)
    :arenaSize_{ fixedMemorySize }, memoryhandle_{ nullptr }, blockSize_{ 0 } {
    memoryhandle_ = (char*)malloc(arenaSize_);
}

CFixedMemoryBlock::~CFixedMemoryBlock() {
    if (memoryhandle_ != nullptr) {
        free(memoryhandle_);
    }
}

void* CFixedMemoryBlock::Allocate(size_t size) {

    //메모리가 이미 할당됐다면 nullptr 반환
    if (Empty() == false) {
        return nullptr;
    }

    blockSize_ = std::max(size, sizeof(void*));
    size_t count = Capacity();

    char* p = reinterpret_cast<char*>(memoryhandle_);

    //Create Free List
    for (; count > 1; --count, p += size) {
        *reinterpret_cast<char**>(p) = p + size;
    }
    //Last nullptr
    *reinterpret_cast<char**>(p) = p + size;
    return memoryhandle_;
}

size_t CFixedMemoryBlock::BlockSize()const {
    return blockSize_;
}

size_t CFixedMemoryBlock::Capacity()const {
    return blockSize_ ? arenaSize_ / blockSize_ : 0;
}

void CFixedMemoryBlock::Clear() {
    blockSize_ = 0;
}

bool CFixedMemoryBlock::Empty()const {
    return blockSize_ ? false : true;
}