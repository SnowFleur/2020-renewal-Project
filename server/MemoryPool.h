#pragma once

template<class MemoryBlock>
class CMemoryPool {
private:
    struct freeBlock {
        freeBlock* next_;
    };
    freeBlock*      freeBlockPtr_;
    MemoryBlock     memoryBlockHandle_;
    size_t          fixedBlockSize_;
public:
    CMemoryPool(size_t fixedMemorySize);
    ~CMemoryPool() = default;

    CMemoryPool(const CMemoryPool&) = delete;
    CMemoryPool(CMemoryPool&&) = delete;
    CMemoryPool& operator=(const CMemoryPool&) = delete;
    CMemoryPool& operator=(CMemoryPool&&) = delete;

    void* Allocate(size_t size);
    bool            Empty()const;
    size_t          Capacity()const;
    void            Clear();
    void            Deallocate(void*);
};

#include"MemoryPool.inl"
