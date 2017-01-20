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
    
    assert(i_MemorySize > 0);
    assert(i_NumDescriptors > 0);
    
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
    m_pFreeMemoryList->m_pBlockAddress = pUsableMemory;
    m_pFreeMemoryList->m_BlockSize = m_MemorySize - descriptorsTotalSize;
    m_pFreeMemoryList->m_pNext = nullptr;
    
    //move to next descriptor:
    pCurrentAddress += sizeof(BlockDescriptor);
    
    //Init Free Descriptors List
    //-1: because 1 Descriptor has been used in pFreeMemoryList
    for (size_t i = 0; i < i_NumDescriptors - 1; i++)
    {
        //Init a new descriptor:
        BlockDescriptor* new_Descriptor = reinterpret_cast<BlockDescriptor*>(pCurrentAddress);
        new_Descriptor->Init();
        
        if (m_pFreeDescriptorList == nullptr)
        {
            m_pFreeDescriptorList = new_Descriptor;
        }
        else
        {
            //put the new descriptor to FreeDescriptorList:
            BlockDescriptor* this_Descriptor = m_pFreeDescriptorList;
            BlockDescriptor* prev_Descriptor = nullptr;
            
            while (this_Descriptor != nullptr)
            {
                prev_Descriptor = this_Descriptor;
                this_Descriptor = this_Descriptor->m_pNext;
            }
            prev_Descriptor->m_pNext = new_Descriptor;
        }
        
        //move to next descriptor:
        pCurrentAddress += sizeof(BlockDescriptor);
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
    
    //Display FreeMemoryList
    printf("\n##################\n");
    printf("#FreeMemoryList: #\n");
    printf("##################\n\n");
    _display(m_pFreeMemoryList);

    
    //Display FreeDescriptor list
    printf("\n######################\n");
    printf("#FreeDescriptorList: #\n");
    printf("######################\n\n");
    _display(m_pFreeDescriptorList);
    
    
    //Display OutstandingAllocationList
    printf("\n#############################\n");
    printf("#OutstandingAllocationList: #\n");
    printf("#############################\n\n");
    _display(m_pOutstandingAllocationList);
    
}

void HeapManager::_display(const BlockDescriptor* i_pList) const
{
    if (i_pList == nullptr)
    {
        printf("This List is NULL\n");
    }
    else
    {
        BlockDescriptor* this_node = const_cast<BlockDescriptor*>(i_pList);
        
        while (this_node != nullptr)
        {
            printf("Descriptor Address: %p, Memory Address: %lu, BlockSize: %zu\n", this_node, this_node->m_pBlockAddress, this_node->m_BlockSize);
            
            this_node = this_node->m_pNext;
        }
    }
}

void* HeapManager::_alloc(const size_t i_Size)
{

    //Calculate the memory size:
    assert(i_Size > 0);
    
    size_t size = 0;
    
    if (i_Size % m_alignedSize == 0)
        size = i_Size;
    else
        size = (i_Size / m_alignedSize + 1) * m_alignedSize;

    
    //If there is only a descriptor in pFreeMemoryList
    if (m_pFreeMemoryList->m_pNext == nullptr)
    {
        //pick a new Descriptor from FreeDescriptorList:
        BlockDescriptor* newDescriptor = m_pFreeDescriptorList;
        newDescriptor->m_BlockSize = size;
        newDescriptor->m_pBlockAddress = m_pFreeMemoryList->m_pBlockAddress;
        m_pFreeDescriptorList = newDescriptor->m_pNext;
        
        //add the Descriptor into OutstandingAllocationList:
        newDescriptor->m_pNext = m_pOutstandingAllocationList;
        m_pOutstandingAllocationList = newDescriptor;
        
        //Recalculate the BIG memory pool's address and size:
        m_pFreeMemoryList->m_BlockSize -= size;
        m_pFreeMemoryList->m_pBlockAddress = m_pFreeMemoryList->m_pBlockAddress + size;
        
        printf("\nAllocate a Memory Block:\n");
        newDescriptor->_display();
        
        return reinterpret_cast<void*>(newDescriptor->m_pBlockAddress);
    }

    //If first descriptor can be used for allocating:
    if(m_pFreeMemoryList->m_BlockSize >= size)
    {
        BlockDescriptor* this_Descriptor = m_pFreeMemoryList;
        
        m_pFreeMemoryList = m_pFreeMemoryList->m_pNext;
        this_Descriptor->m_pNext = m_pOutstandingAllocationList;
        m_pOutstandingAllocationList = this_Descriptor;
    
        printf("\nAllocate a Memory Block:\n");
        this_Descriptor->_display();
        
        return reinterpret_cast<void *>(this_Descriptor->m_pBlockAddress);
    }
    //Or we can look for a good descriptor
    else
    {
        BlockDescriptor* this_Descriptor = m_pFreeMemoryList;
        BlockDescriptor* prev_Descriptor = nullptr;
        
        while (this_Descriptor != nullptr)
        {
            if (this_Descriptor->m_BlockSize < size || this_Descriptor->m_BlockSize > 2 * size)
            {
                prev_Descriptor = this_Descriptor;
                this_Descriptor = this_Descriptor->m_pNext;
            }
            else
            {
                prev_Descriptor->m_pNext = this_Descriptor->m_pNext;
                this_Descriptor->m_pNext = m_pOutstandingAllocationList;
                m_pOutstandingAllocationList = this_Descriptor;
                
                printf("\nAllocate a Memory Block:\n");
                this_Descriptor->_display();
                
                return reinterpret_cast<void *>(this_Descriptor->m_pBlockAddress);
            }
        }
        
        //Or finally we have to cut a memory block from Biggest Memory Pool for user
        //Last Descriptor in my FreeDescriptorList always has Biggest Size.
        if (m_pFreeDescriptorList == nullptr)
        {
            printf("Not enough Descriptor for use...");
            return nullptr;
        }
        
        //pick a new Descriptor from FreeDescriptorList:
        BlockDescriptor* newDescriptor = m_pFreeDescriptorList;
        newDescriptor->m_BlockSize = size;
        newDescriptor->m_pBlockAddress = prev_Descriptor->m_pBlockAddress;
        m_pFreeDescriptorList = newDescriptor->m_pNext;
        
        //add the Descriptor into OutstandingAllocationList:
        newDescriptor->m_pNext = m_pOutstandingAllocationList;
        m_pOutstandingAllocationList = newDescriptor;
        
        //Recalculate the BIG memory pool's address and size:
        prev_Descriptor->m_BlockSize -= size;
        prev_Descriptor->m_pBlockAddress = prev_Descriptor->m_pBlockAddress + size;
        
        printf("\nAllocate a Memory Block:\n");
        newDescriptor->_display();
        
        return reinterpret_cast<void*>(newDescriptor->m_pBlockAddress);
    }
}


bool HeapManager::_free(const void* i_pMemory)
{
    return false;
}


void HeapManager::_recycle()
{
    
}