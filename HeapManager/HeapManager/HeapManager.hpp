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
#include <vector>


class BlockDescriptor;
class FixedSizeAllocator;

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

    // allocate a memory to user
    void* _alloc(const size_t i_Size, const size_t i_AlignedSize = 4);
    
    //free a memory block
    bool _free(const void* i_pMemory);
    
    //call garbage collection
    void _recycle();
    
    //Init function for create a HeapManager
    static HeapManager* _create(void *i_pMemoryPool, const size_t i_MemorySize, const size_t i_NumDescriptors);
    
    //Get the HeapManager;
    static HeapManager* _GetHeapManager()
    {
        return s_pHeapManager;
    }
    
    //search which FSA can be used for allocate memory
    FixedSizeAllocator* _search_Available_FSA(const size_t i_Size);
    
    //check if FSA can free the memory
    bool _free_FixedSizeAllocator(const void * i_pMemory);
    
    //pointer to FixedSizeAllocator
    FixedSizeAllocator* s_pFixedSizeAllocator;
    
private:
    
    HeapManager() : m_alignedSize(4),
                    m_pFreeMemoryList(nullptr),
                    m_pFreeDescriptorList(nullptr),
                    m_pOutstandingAllocationList(nullptr),
                    m_NumBlocks_FSA(100),
                    m_NumFSAs(5),
                    m_UnitSize_FSA(16)
    {
        
    }
    
    // Init the HeapManager
    HeapManager* _init(void *i_pMemoryPool, const size_t i_MemorySize, const size_t i_NumDescriptors);
    
    //static pointer to HeapManager
    static HeapManager* s_pHeapManager;
    
    // display memory lists
    void _display(const BlockDescriptor* i_pList) const;
    
    //Total Memory Size
    size_t m_MemoryTotalSize;
    
    //Block aligned Size
    uint8_t m_alignedSize;
    
    //Pointer for Memory Pool
    uintptr_t m_pMemoryPool;
    
    //memory lists
    BlockDescriptor* m_pFreeMemoryList;
    BlockDescriptor* m_pFreeDescriptorList;
    BlockDescriptor* m_pOutstandingAllocationList;
    
    //For FixedSizeAllocator:
    
    //init FSA
    void _init_FixedSizeAllocator(const void* i_pMemoryPool);
    
    //destroy FSA
    void _destroy_FixedSizeAllocator();

    //Total Memory Size for FSA
    size_t _Get_MemoryTotalSize_FSA();
    
    //number of FSA type
    size_t m_NumFSAs;
    
    //number of memory block in each FSA
    size_t m_NumBlocks_FSA;
    
    //Pointer for FSA Memory Pool
    uintptr_t m_pMemoryPool_FSA;
    
    //Block Unit Size
    //e.g, m_FSAUnitSize = 16, block memory will be 16, 32, 48, 64...
    const size_t m_UnitSize_FSA;
};

#endif /* HeapManager_hpp */

