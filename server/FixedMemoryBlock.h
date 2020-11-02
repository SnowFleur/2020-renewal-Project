#pragma once
class CFixedMemoryBlock {
private:
    void* memoryhandle_;
    size_t  arenaSize_;
    size_t  blockSize_;
public:
    CFixedMemoryBlock(size_t fixedMemorySize);
    ~CFixedMemoryBlock();
    CFixedMemoryBlock(const CFixedMemoryBlock&) = delete;
    CFixedMemoryBlock(CFixedMemoryBlock&&) = delete;
    CFixedMemoryBlock& operator=(const CFixedMemoryBlock&) = delete;
    CFixedMemoryBlock& operator=(CFixedMemoryBlock&&) = delete;

    void* Allocate(size_t);
    size_t		BlockSize() const;
    size_t		Capacity() const;
    void		Clear();
    bool		Empty() const;
};