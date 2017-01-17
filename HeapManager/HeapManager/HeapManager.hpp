//
//  HeapManager.hpp
//  HeapManager
//
//  Created by CCLai on 2017/1/16.
//  Copyright © 2017年 CCLai. All rights reserved.
//

#ifndef HeapManager_hpp
#define HeapManager_hpp

#include <stdio.h>
#include <stdlib.h>

class BlockDescriptor;

class HeapManager
{
public:
    
    HeapManager() : m_alignment(4), pFreeMemoryList(nullptr), pFreeDescriptorList(nullptr), pOutstandingAllocationList(nullptr)
    {
        
    }
    
    // create a new Allocator
    HeapManager* _create(void *i_pMemory, const size_t i_sizeMemory, const size_t i_numDescriptors);
    
    // destroy the Allocator
    inline void _destroy() { free(reinterpret_cast<void*>(pMemory)); }
    
    // display memory lists
    void _display() const;
    
    // allocate a memory to user
    void* _allocate(const size_t i_size, const int i_guardbandSize = 4);
    
    //free a memory block
    bool _free(const void* i_pMemory);
    
    //call garbage collection
    void _garbageCollection();
    
private:
    
    size_t pMemorySize;
    uint8_t m_alignment;
    uintptr_t pMemory;
    
    //memory lists
    BlockDescriptor* pFreeMemoryList;
    BlockDescriptor* pFreeDescriptorList;
    BlockDescriptor* pOutstandingAllocationList;
};


#endif /* HeapManager_hpp */
