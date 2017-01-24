//
//  FixedSizeAllocator.hpp
//  HeapManager
//
//  Created by CCLai on 2017/1/23.
//  Copyright © 2017年 CCLai. All rights reserved.
//

#ifndef FixedSizeAllocator_hpp
#define FixedSizeAllocator_hpp

#include <stdio.h>
#include <stdlib.h>

class BitArray;

class FixedSizeAllocator
{
    
public:
    
    FixedSizeAllocator() :  m_pMemoryPool(NULL),
                            m_alignedSize(8),
                            m_MemoryTotalSize(0),
                            m_BlockSize(0),
                            m_NumBlocks(0),
                            m_pBitArray(nullptr)
    {
        
    }
    
    // create a new Allocator
    bool _create(void* i_pMemoryPool, const size_t i_NumBlocks, const size_t i_BlockSize);
    
    // allocate a memory to user
    void* _alloc();
    
    // free a memory block
    bool _free(const void* i_pMemory);
    
    // display memory block
    void display() const;
    
    // clear memory block
    void _clear();
    
    // destroy the Allocator
    void _destroy();
    
    // get memory pool address
    void* _GetPoolAddress() const;
    
    // check if this allocator is available
    bool _IsAvailable() const;
    
    // return Block Size for User
    size_t _GetBlockSize() const;
    
    BitArray* _GetBitArray()
    {
        return m_pBitArray;
    }
    
private:
    
    // Pointer for Memory Pool
    uintptr_t m_pMemoryPool;
    
    // aligned size
    uint8_t m_alignedSize;
    
    // total size of memory
    size_t m_MemoryTotalSize;
    
    // size of memory block
    size_t m_BlockSize;
    
    // number of memory block
    size_t m_NumBlocks;
    
    // Pointer to a BitArray;
    BitArray* m_pBitArray;
};

#endif /* FixedSizeAllocator_hpp */
