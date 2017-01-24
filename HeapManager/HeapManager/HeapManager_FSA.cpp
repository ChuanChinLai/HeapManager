//
//  HeapManager_FSA.cpp
//  HeapManager
//
//  Created by CCLai on 2017/1/23.
//  Copyright © 2017年 CCLai. All rights reserved.
//

#include <stdio.h>
#include <cassert>
#include "HeapManager.hpp"
#include "FixedSizeAllocator.hpp"

void HeapManager::_init_FixedSizeAllocator(const void* i_pMemoryPool)
{
    //Used Memory Size
    size_t UsedMemorySize = 0;
    
    uintptr_t MemoryAddress = reinterpret_cast<uintptr_t>(i_pMemoryPool);
    
    if (s_pFixedSizeAllocator == nullptr)
    {
        s_pFixedSizeAllocator = new FixedSizeAllocator[m_NumFSAs];
        
        for (size_t i = 0; i < m_NumFSAs; i++)
        {
            FixedSizeAllocator newAllocator;
            newAllocator._create(reinterpret_cast<void*>(MemoryAddress), m_NumBlocks_FSA, m_UnitSize_FSA * (i + 1));
            
            s_pFixedSizeAllocator[i] = newAllocator;
            
            UsedMemorySize += m_UnitSize_FSA * (i + 1) * m_NumBlocks_FSA;
            assert(_Get_MemoryTotalSize_FSA() >= UsedMemorySize);
            
            MemoryAddress += m_UnitSize_FSA * (i + 1) * m_NumBlocks_FSA;
        }
    }
}

void HeapManager::_destroy_FixedSizeAllocator()
{
    if (s_pFixedSizeAllocator != nullptr)
    {
        
        for (size_t i = 0; i < m_NumFSAs; i++)
        {
            printf("MISS?:  %p\n", s_pFixedSizeAllocator[i]._GetBitArray());
        }
        
        void* tmp = s_pFixedSizeAllocator[0]._GetPoolAddress();
        
        printf("%p, %p\n", tmp, s_pFixedSizeAllocator);
        
        for (size_t i = 0; i < m_NumFSAs; i++)
        {
            s_pFixedSizeAllocator[i]._destroy();
        }
        
        delete[] s_pFixedSizeAllocator;
        s_pFixedSizeAllocator = nullptr;
        
        delete tmp;
    }
}


bool HeapManager::_free_FixedSizeAllocator(const void * i_pMemory)
{
    if (s_pFixedSizeAllocator == nullptr)
        return false;
    
    bool success = false;
    
    printf("POOL POSITION: %p\n",  s_pFixedSizeAllocator[0]._GetPoolAddress());
    
    for (size_t i = 0; i < m_NumFSAs ; i++)
    {
        if (s_pFixedSizeAllocator[0]._GetPoolAddress() != i_pMemory)
        {
            success = s_pFixedSizeAllocator[i]._free(i_pMemory);
            
            if (success)
            {
                return true;
            }
        }
    }
    return false;
}


FixedSizeAllocator* HeapManager::_search_FixedSizeAllocator(const size_t i_Size)
{
    if(s_pFixedSizeAllocator == nullptr)
        return nullptr;

    
    for (size_t i = 0; i < m_NumFSAs; i++)
    {
        
        bool c1 = i_Size < s_pFixedSizeAllocator[i]._GetBlockSize();
        bool c2 = s_pFixedSizeAllocator[i]._IsAvailable();
        printf("%d, %d ISIZE: %zu\n", c1, c2, i_Size);
        
        if (c1 && c2)
        {
                return &s_pFixedSizeAllocator[i];
        }
    }

    return nullptr;
}


size_t HeapManager::_Get_MemoryTotalSize_FSA()
{
    size_t UsedMemorySize = 0;
    
    for (size_t i = 0; i < m_NumFSAs; i++)
    {
        UsedMemorySize += m_UnitSize_FSA * (i + 1) * m_NumBlocks_FSA;
    }

    return UsedMemorySize;
}
