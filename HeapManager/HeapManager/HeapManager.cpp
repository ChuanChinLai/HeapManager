//
//  HeapManager.cpp
//  HeapManager
//
//  Created by CCLai on 2017/1/16.
//  Copyright © 2017年 CCLai. All rights reserved.
//

#include "HeapManager.hpp"

HeapManager* HeapManager::_create(void *i_pMemoryPool, const size_t i_MemorySize, const size_t i_NumDescriptors)
{
    m_pMemoryPool = reinterpret_cast<uintptr_t>(i_pMemoryPool);
    return this;
}

void HeapManager::_destroy()
{

}

void HeapManager::_display() const
{
    printf("Memory Address: %lu\n", m_pMemoryPool);
}