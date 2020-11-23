#include"LogCollector.h"

template<class MemoryBlock>
inline CMemoryPool<MemoryBlock>::CMemoryPool(size_t fixedMemorySize):
    freeBlockPtr_{ nullptr }, memoryBlockHandle_{ fixedMemorySize }, fixedBlockSize_{}{
}

template<class MemoryBlock>
inline void* CMemoryPool<MemoryBlock>::Allocate(size_t size) {
    // free_Ptr이 nullptr이라면
    if (Empty()) {
        //Memory Block에서 메모리를 받아와서 FreeBlock으로 캐스팅
        freeBlockPtr_ = reinterpret_cast<freeBlock*>(memoryBlockHandle_.Allocate(size));
        //요구한 메모리 크기만큼 저장
        fixedBlockSize_ = size;

        //아직도 freeBlockPtr이 nullptr 이면
        //bad_alloc() 예외 전달
        if (Empty()) {
            CLogCollector::GetInstance()->PrintLog("Memory Pool Is Empty");
        }
    }

    //요구하는 사이즈보다 blockSize가 작다면
    //여기에서는 더 큰 메모리요구가 오면 예외처리를 한다는 것도 존재(고정 메모리블럭이기 때문)
    //bad_alloc() 예외 전달
    if (size > fixedBlockSize_) {
        CLogCollector::GetInstance()->PrintLog("Request Memory Bigger than Fixed Memory");
    }

    //freeBlockPtr의 메모리를 p에 할당하고
    //freeBlockPtr은 다음 메모리로 이동
    if (freeBlockPtr_ == nullptr) {
        CLogCollector::GetInstance()->PrintLog("Memory Pool Is Empty");
    }
    else {
        auto p = freeBlockPtr_;
        freeBlockPtr_ = freeBlockPtr_->next_;
        return p;
    }
}

template<class MemoryBlock>
inline bool CMemoryPool<MemoryBlock>::Empty()const {
    return freeBlockPtr_ == nullptr;
}

template<class MemoryBlock>
inline size_t CMemoryPool<MemoryBlock>::Capacity()const {
    //메모리블럭 핸들의 값 전달
    return memoryBlockHandle_.Capacity();
}

template<class MemoryBlock>
inline void CMemoryPool<MemoryBlock>::Clear() {
    freeBlockPtr_ = nullptr;
    memoryBlockHandle_.clear();
}

template<class MemoryBlock>
inline void CMemoryPool<MemoryBlock>::Deallocate(void* ptr) {
    if (ptr != nullptr) {
        auto freePtr = reinterpret_cast<freeBlock*>(ptr);
        freePtr->next_ = freeBlockPtr_;
        freeBlockPtr_ = freePtr;
    }
}
