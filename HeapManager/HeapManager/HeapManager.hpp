//
//  HeapManager.hpp
//  HeapManager
//
//  Created by CCLai on 2017/1/16.
//  Copyright © 2017年 CCLai. All rights reserved.
//

#pragma once
//#ifndef HeapManager_hpp
//#define HeapManager_hpp

#include <stdio.h>
#include <stdlib.h>

class BlockDescriptor;

class HeapManager
{
public:
    
    ~HeapManager()
    {
        
    }

    // destroy the Allocator
    void _destroy();
    
    // display memory lists
    void _display() const;
    void _display(const BlockDescriptor* i_pList) const;
    
    // allocate a memory to user
    void* _alloc(const size_t i_Size);
    
    //free a memory block
    bool _free(const void* i_pMemory);
    
    //call garbage collection
    void _recycle();
    
    static HeapManager* pHeapManager;
    
    static HeapManager* _init(void *i_pMemoryPool, const size_t i_MemorySize, const size_t i_NumDescriptors);
    
private:
    
    HeapManager() : m_alignedSize(4),
    m_pFreeMemoryList(nullptr),
    m_pFreeDescriptorList(nullptr),
    m_pOutstandingAllocationList(nullptr)
    {
        
    }
    
    // create a new Allocator
    HeapManager* _create(void *i_pMemoryPool, const size_t i_MemorySize, const size_t i_NumDescriptors);
    
    //Total Memory Size
    size_t m_MemorySize;
    
    //Block aligned Size
    uint8_t m_alignedSize;
    
    //Pointer for Memory Pool
    uintptr_t m_pMemoryPool;
    
    //memory lists
    BlockDescriptor* m_pFreeMemoryList;
    BlockDescriptor* m_pFreeDescriptorList;
    BlockDescriptor* m_pOutstandingAllocationList;
};

//#endif /* HeapManager_hpp */

#include "HeapManager-inline.hpp"