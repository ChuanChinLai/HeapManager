//
//  HeapManager_Operator.cpp
//  HeapManager
//
//  Created by CCLai on 2017/1/23.
//  Copyright © 2017年 CCLai. All rights reserved.
//

#include "HeapManager_Operator.hpp"
#include "HeapManager.hpp"
#include "FixedSizeAllocator.hpp"

void* operator new(const size_t i_size)
{
    void* pReturn = nullptr;
    FixedSizeAllocator* pFSA = HeapManager::_search_FixedSizeAllocator(i_size);
    
    if (pFSA)
        pReturn = pFSA->_alloc();
    
    if (pReturn == nullptr)
        pReturn = HeapManager::_GetHeapManager()->_alloc(i_size);
    
    return pReturn;
    
    
//    return HeapManager::_GetHeapManager()->_alloc(i_size);
}

void operator delete(void * i_ptr)
{
    if (i_ptr)
    {
        HeapManager::_GetHeapManager()->_free(i_ptr);
    }
}

void* operator new[](const size_t i_size)
{
    void* pReturn = nullptr;
    FixedSizeAllocator* pFSA = HeapManager::_search_FixedSizeAllocator(i_size);
    
    if (pFSA)
        pReturn = pFSA->_alloc();
        
    if (pReturn == nullptr)
        pReturn = HeapManager::_GetHeapManager()->_alloc(i_size);
            
    return pReturn;
}

void operator delete[](void * i_ptr)
{
    if (i_ptr)
    {
        HeapManager::_GetHeapManager()->_free(i_ptr);
    }
}

void * operator new(const size_t i_size, uint8_t i_align)
{
    switch (i_align)
    {
        case 16:
        case 32:
            return HeapManager::_GetHeapManager()->_alloc(i_size, i_align);
        default:
            return HeapManager::_GetHeapManager()->_alloc(i_size);
    }
}

void operator delete(void * i_ptr, uint8_t i_align)
{
    if (i_ptr)
    {
        HeapManager::_GetHeapManager()->_free(i_ptr);
    }
}
