//
//  FixedSizeAllocator.cpp
//  HeapManager
//
//  Created by CCLai on 2017/1/23.
//  Copyright © 2017年 CCLai. All rights reserved.
//

#include "FixedSizeAllocator.hpp"
#include "HeapManager.hpp"
#include "BitArray.hpp"
#include <cassert>

bool FixedSizeAllocator::_create(void* i_pMemoryPool, const size_t i_MemorySize, const size_t i_BlockSize)
{
    assert(i_pMemoryPool);
    assert(i_BlockSize > 0);

    //Init memory address
    m_pMemoryPool = reinterpret_cast<uintptr_t>(i_pMemoryPool);
    m_MemoryTotalSize = i_MemorySize;
    
    //calculate size of memory block
    m_BlockSize = (i_BlockSize + (m_alignedSize - 1) / m_alignedSize);
    assert(m_MemoryTotalSize >= m_BlockSize);
    
    //calculate number of memory block
    m_NumBlocks = m_MemoryTotalSize / m_BlockSize;
    assert(m_NumBlocks > 0);
    
    //create a BitArray for FixedSizeAllocator
    m_pBitArray = BitArray::_create(m_NumBlocks, HeapManager::_GetHeapManager());
    assert(m_pBitArray);
    
    if (m_pBitArray == nullptr)
        return false;
    
    return true;
}
