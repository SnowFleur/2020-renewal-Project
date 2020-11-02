
template<class MemoryBlock>
inline CMemoryPool<MemoryBlock>::CMemoryPool(size_t fixedMemorySize):
    freeBlockPtr_{ nullptr }, memoryBlockHandle_{ fixedMemorySize }, fixedBlockSize_{}{
}

template<class MemoryBlock>
inline void* CMemoryPool<MemoryBlock>::Allocate(size_t size) {
    // free_Ptr�� nullptr�̶��
    if (Empty()) {
        //Memory Block���� �޸𸮸� �޾ƿͼ� FreeBlock���� ĳ����
        freeBlockPtr_ = reinterpret_cast<freeBlock*>(memoryBlockHandle_.Allocate(size));
        //�䱸�� �޸� ũ�⸸ŭ ����
        fixedBlockSize_ = size;

        //������ freeBlockPtr�� nullptr �̸�
        //bad_alloc() ���� ����
        if (Empty()) {
            throw std::bad_alloc();
        }
    }
    //�䱸�ϴ� ������� blockSize�� �۴ٸ�
    //���⿡���� �� ū �޸𸮿䱸�� ���� ����ó���� �Ѵٴ� �͵� ����(���� �޸𸮺��̱� ����)
    //bad_alloc() ���� ����
    if (size > fixedBlockSize_) {
        throw std::bad_alloc();
    }

    //freeBlockPtr�� �޸𸮸� p�� �Ҵ��ϰ�
    //freeBlockPtr�� ���� �޸𸮷� �̵�
    auto p = freeBlockPtr_;
    freeBlockPtr_ = freeBlockPtr_->next_;
    return p;
}

template<class MemoryBlock>
inline bool CMemoryPool<MemoryBlock>::Empty()const {
    return freeBlockPtr_ ? false : true;
}

template<class MemoryBlock>
inline size_t CMemoryPool<MemoryBlock>::Capacity()const {
    //�޸𸮺� �ڵ��� �� ����
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
