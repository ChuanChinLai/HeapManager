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

bool FixedSizeAllocator::_create(void* i_pMemoryPool, const size_t i_NumBlocks, const size_t i_BlockSize)
{
    assert(i_pMemoryPool);
    assert(i_BlockSize > 0);
    assert(i_NumBlocks > 0);
    
    //Init memory address
    m_pMemoryPool = reinterpret_cast<uintptr_t>(i_pMemoryPool);
    m_MemoryTotalSize = i_NumBlocks * i_BlockSize;
    
    //calculate number of memory block
    m_NumBlocks = i_NumBlocks;
    
    //calculate size of memory block
    m_BlockSize = (i_BlockSize + (m_alignedSize - 1) / m_alignedSize);
    assert(m_MemoryTotalSize >= m_BlockSize);
    
    
    //create a BitArray for FixedSizeAllocator
    m_pBitArray = BitArray::_create(m_NumBlocks, HeapManager::_GetHeapManager());
    assert(m_pBitArray);
    
    if (m_pBitArray == nullptr)
        return false;
    
    return true;
}


void* FixedSizeAllocator::_alloc()
{
    size_t GetFirstAvailableBit = 0;
    
    if (m_pBitArray->GetFirstClearBit(GetFirstAvailableBit))
    {
        // mark it in use because we're going to allocate it to user
        m_pBitArray->SetBit(GetFirstAvailableBit);
        
        //calculate MemoryBlock Address
        uintptr_t BlockAddress = m_pMemoryPool + (GetFirstAvailableBit * m_BlockSize);
        
        // calculate its address and return it to user
        printf("\nUsing FixedSizeAllocator:\n");
        printf("Memory Address: %lu, Size: %zu\n", BlockAddress, m_BlockSize);
        
        return reinterpret_cast<void *>(BlockAddress);
    }
    else
    {
        printf("\nFixedSizeAllocator is NOT available...\n");
        return nullptr;
    }
}

bool FixedSizeAllocator::_free(const void* i_pMemory)
{
    size_t BitPosition = 0;
    
    uintptr_t FreeMemoryAddress = reinterpret_cast<uintptr_t>(i_pMemory);
    uintptr_t BlockAddress = m_pMemoryPool;
    
    //looking for the memory address we want to free
    while (BlockAddress != FreeMemoryAddress)
    {
        BlockAddress += m_BlockSize;
        BitPosition++;

        if (BitPosition > m_NumBlocks)
            return false;
    }
    
    // mark it clear because we're going to free it
    printf("Using FixedSizeAllocator to free Memory: %lu\n", BlockAddress);
    m_pBitArray->ClearBit(BitPosition);
    return true;
}


void FixedSizeAllocator::display() const
{
    printf("MemoryPool Address: %lu, BlockSize: %zu\n", m_pMemoryPool, m_BlockSize);
    
    uintptr_t currentAddress = m_pMemoryPool;
    
    for (size_t i = 0; i < m_NumBlocks; i++)
    {
        if (m_pBitArray->IsBitClear(i) == true)
            printf("(O) Block Address: %lu\n", currentAddress);
        else
            printf("(X) Block Address: %lu\n", currentAddress);
        
        currentAddress += m_BlockSize;
    }
}


void FixedSizeAllocator::_clear()
{
    m_pBitArray->ClearAll();
}


void FixedSizeAllocator::_destroy()
{
    _clear();
    delete m_pBitArray;
}


void* FixedSizeAllocator::_GetPoolAddress() const
{
    return reinterpret_cast<void*>(m_pMemoryPool);
}


bool FixedSizeAllocator::_IsAvailable() const
{
    return !m_pBitArray->AreAllSet();
}


size_t FixedSizeAllocator::_GetBlockSize() const
{
    return m_BlockSize;
}
