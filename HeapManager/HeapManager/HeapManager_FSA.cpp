//
//  HeapManager_FSA.cpp
//  HeapManager
//
//  Created by CCLai on 2017/1/23.
//  Copyright © 2017年 CCLai. All rights reserved.
//

#include <stdio.h>
#include <cassert>
#include <iostream>
#include "HeapManager.hpp"
#include "FixedSizeAllocator.hpp"

void HeapManager::_init_FixedSizeAllocator(const void* i_pMemoryPool)
{
    //Used Memory Size
    size_t UsedMemorySize = 0;
    
    //set up the memory address for memory pool of FSA
    uintptr_t MemoryAddress = reinterpret_cast<uintptr_t>(i_pMemoryPool);
    m_pMemoryPool_FSA = MemoryAddress;
    
    if (m_pFixedSizeAllocator == nullptr)
    {
        m_pFixedSizeAllocator = new FixedSizeAllocator[m_NumFSAs];
        
        for (size_t i = 0; i < m_NumFSAs; i++)
        {
            FixedSizeAllocator newAllocator;
            
            newAllocator._create(reinterpret_cast<void*>(MemoryAddress), m_NumBlocks_FSA, m_UnitSize_FSA * (i + 1));
            
            m_pFixedSizeAllocator[i] = newAllocator;
            
            UsedMemorySize += m_UnitSize_FSA * (i + 1) * m_NumBlocks_FSA;
            assert(_Get_MemoryTotalSize_FSA() >= UsedMemorySize);
            
            MemoryAddress += m_UnitSize_FSA * (i + 1) * m_NumBlocks_FSA;
        }
    }
}

void HeapManager::_destroy_FixedSizeAllocator()
{
    if (m_pFixedSizeAllocator != nullptr)
    {
        for (size_t i = 0; i < m_NumFSAs; i++)
        {
            m_pFixedSizeAllocator[i]._destroy();
        }
        
        delete[] m_pFixedSizeAllocator;
        m_pFixedSizeAllocator = nullptr;
        
        _free(reinterpret_cast<void*>(m_pMemoryPool_FSA));
    }
}


bool HeapManager::_free_FixedSizeAllocator(const void * i_pMemory)
{
    if (m_pFixedSizeAllocator == nullptr || m_pFixedSizeAllocator == i_pMemory)
        return false;
    
    bool success = false;
    
    for (size_t i = 0; i < m_NumFSAs ; i++)
    {
        success = m_pFixedSizeAllocator[i]._free(i_pMemory);
            
        if (success)
        {
            return true;
        }
    }
    return false;
}


FixedSizeAllocator* HeapManager::_search_Available_FSA(const size_t i_Size)
{
    if(m_pFixedSizeAllocator == nullptr)
        return nullptr;
    
    for (size_t i = 0; i < m_NumFSAs; i++)
    {
        if (i_Size < m_pFixedSizeAllocator[i]._GetBlockSize() && m_pFixedSizeAllocator[i]._IsAvailable())
        {
                return &m_pFixedSizeAllocator[i];
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
