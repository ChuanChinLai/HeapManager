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
    FixedSizeAllocator* pFSA = HeapManager::_GetHeapManager()->_search_Available_FSA(i_size);
    
    if (pFSA)
        pReturn = pFSA->_alloc();
    
    if (pReturn == nullptr)
        pReturn = HeapManager::_GetHeapManager()->_alloc(i_size);
    
    return pReturn;
}

void operator delete(void * i_ptr)
{
    if (i_ptr)
    {
        if (HeapManager::_GetHeapManager()->_free_FixedSizeAllocator(i_ptr) == false)
        {
            HeapManager::_GetHeapManager()->_free(i_ptr);
        }
    }
}

void* operator new[](const size_t i_size)
{
    void* pReturn = nullptr;
    FixedSizeAllocator* pFSA = HeapManager::_GetHeapManager()->_search_Available_FSA(i_size);
    
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
        if (HeapManager::_GetHeapManager()->_free_FixedSizeAllocator(i_ptr) == false)
        {
            HeapManager::_GetHeapManager()->_free(i_ptr);
        }
    }
}
