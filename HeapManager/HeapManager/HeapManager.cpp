//
//  HeapManager.cpp
//  HeapManager
//
//  Created by CCLai on 2017/1/16.
//  Copyright © 2017年 CCLai. All rights reserved.
//
#include <cassert>

#include "HeapManager.hpp"
#include "Block_Descriptor.hpp"


HeapManager* HeapManager::_create(void *i_pMemoryPool, const size_t i_MemorySize, const size_t i_NumDescriptors)
{
    //Setting Memory Info:
    m_MemorySize = i_MemorySize;
    m_pMemoryPool = reinterpret_cast<uintptr_t>(i_pMemoryPool);
    
    //Total Descriptors Size:
    size_t descriptorsTotalSize = i_NumDescriptors * sizeof(BlockDescriptor);
    assert(m_MemorySize > descriptorsTotalSize);
    
    
    //position of Pointer
    uintptr_t pCurrentAddress = m_pMemoryPool;
    uintptr_t pUsableMemory = m_pMemoryPool + descriptorsTotalSize;
    
    
    //Init Free Memory List
    m_pFreeMemoryList = reinterpret_cast<BlockDescriptor*>(pCurrentAddress);
    m_pFreeMemoryList->m_pBlockBase = pUsableMemory;
    m_pFreeMemoryList->m_BlockSize = m_MemorySize - descriptorsTotalSize;
    m_pFreeMemoryList->m_pNext = nullptr;
    
    
    //Init Free Descriptors List
    //-1: because 1 Descriptor has been used in pFreeMemoryList
    for (size_t i = 0; i < i_NumDescriptors - 1; i++)
    {
        //move to next descriptor:
        pCurrentAddress += (sizeof(BlockDescriptor));
        
        //Init a new descriptor:
        BlockDescriptor* new_Descriptor = reinterpret_cast<BlockDescriptor*>(pCurrentAddress);
        new_Descriptor->Init();
        
        //put the new descriptor to FreeDescriptorList:
        BlockDescriptor* this_Descriptor = m_pFreeDescriptorList;
        BlockDescriptor* prev_Descriptor = nullptr;
        
        if (this_Descriptor == nullptr)
        {
            m_pFreeDescriptorList = new_Descriptor;
        }
        else
        {
            while (this_Descriptor != nullptr)
            {
                prev_Descriptor = this_Descriptor;
                this_Descriptor = this_Descriptor->m_pNext;
            }
            prev_Descriptor->m_pNext = new_Descriptor;
        }
    }
    
    //Init Outstanding Allocation List
    m_pOutstandingAllocationList = nullptr;
    
    return this;
}

void HeapManager::_destroy()
{

}

void HeapManager::_display() const
{
    
    //display Memory Block Size:
    printf("Memory Pool Size: %zu\n", m_MemorySize);
    
    BlockDescriptor* this_node;

    //Display FreeMemoryList
    this_node = m_pFreeMemoryList;
    
    printf("\n##################\n");
    printf("#FreeMemoryList: #\n");
    printf("##################\n\n");
    
    if (this_node == nullptr)
    {
        printf("This List is NULL\n");
    }
    else
    {
        while (this_node != nullptr)
        {
            printf("Descriptor Position: %p, Block Size: %zu, Block Position %lu\n", this_node, this_node->m_BlockSize, this_node->m_pBlockBase);
            
            this_node = this_node->m_pNext;
        }
    }
    
    //Display FreeDescriptor list
    this_node = m_pFreeDescriptorList;
    
    printf("\n######################\n");
    printf("#FreeDescriptorList: #\n");
    printf("######################\n\n");
    
    if (this_node == nullptr)
    {
        printf("This List is NULL\n");
    }
    else
    {
        while (this_node != nullptr)
        {
            printf("Descriptor Position: %p, Block Size: %zu, Block Position %lu\n", this_node, this_node->m_BlockSize, this_node->m_pBlockBase);
            
            this_node = this_node->m_pNext;
        }
    }
    
    //Display OutstandingAllocationList
    this_node = m_pOutstandingAllocationList;
    
    printf("\n#############################\n");
    printf("#OutstandingAllocationList: #\n");
    printf("#############################\n\n");
    
    if (this_node == nullptr)
    {
        printf("This List is NULL\n");
    }
    else
    {
        while (this_node != nullptr)
        {
            printf("Descriptor Position: %p, Block Size: %zu, Block Position %lu\n", this_node, this_node->m_BlockSize, this_node->m_pBlockBase);
            
            this_node = this_node->m_pNext;
        }
    }
}


