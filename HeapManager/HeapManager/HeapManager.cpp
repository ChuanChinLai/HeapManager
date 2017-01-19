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
            printf("Descriptor Address: %p, Memory Address: %lu, BlockSize: %zu\n", this_node, this_node->m_pBlockBase, this_node->m_BlockSize);
            
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
    
    
//  assert there is enough free memory available:
//  assert(m_pFreeMemoryList);
    
    if (m_pFreeMemoryList == nullptr)
    {
        printf("Not enough free memory is available...");
        return nullptr;
    }
    
    
//  assert there is enough free descriptor available:
//  assert(m_pFreeDescriptorList);
    
    BlockDescriptor* this_Descriptor = m_pFreeMemoryList;
    BlockDescriptor* prev_Descriptor = nullptr;
    
    while (this_Descriptor != nullptr)
    {
        if(this_Descriptor->m_BlockSize < i_Size)
        {
            prev_Descriptor = this_Descriptor;
            this_Descriptor = this_Descriptor->m_pNext;
        }
        else
        {
            
            
            
            
            
            if (prev_Descriptor == nullptr)
            {
                if (m_pFreeDescriptorList == nullptr)
                {
                    printf("Not enough free memory is available...");
                    return nullptr;
                }
                
                //pick a new Descriptor from FreeDescriptorList:
                BlockDescriptor* newDescriptor = m_pFreeDescriptorList;
                newDescriptor->m_BlockSize = size;
                newDescriptor->m_pBlockBase = this_Descriptor->m_pBlockBase;
                m_pFreeDescriptorList = newDescriptor->m_pNext;
                
                //add the Descriptor into OutstandingAllocationList:
                newDescriptor->m_pNext = m_pOutstandingAllocationList;
                m_pOutstandingAllocationList = newDescriptor;
                
                //Recalculate the BIG memory pool's address and size:
                uintptr_t new_position = this_Descriptor->m_pBlockBase;
                this_Descriptor->m_pBlockBase = new_position + size;
                this_Descriptor->m_BlockSize -= size;
                
                printf("Allocate Memory, Address: %lu, Size: %zu\n", newDescriptor->m_pBlockBase, newDescriptor->m_BlockSize);
                
                return reinterpret_cast<void*>(newDescriptor->m_pBlockBase);
            }

        }
    }
    
    
    
    
/*===========================================================*/
    
    
    BlockDescriptor* this_Descriptor = m_pFreeMemoryList;
    BlockDescriptor* prev_Descriptor = nullptr;
    
    //To see if there is a free descriptor for allocation
    while (this_Descriptor != nullptr)
    {
        if (size >= this_Descriptor->m_BlockSize)
        {
            prev_Descriptor = this_Descriptor;
            this_Descriptor = this_Descriptor->m_pNext;
        }
        else
        {
            //If we can find a node in free memory descriptor can be used in allocation
            if (this_Descriptor->m_pNext != nullptr)
            {
                if (prev_Descriptor == nullptr)
                {
                    m_pFreeMemoryList = this_Descriptor->m_pNext;
                }
                else
                {
                    prev_Descriptor->m_pNext = this_Descriptor->m_pNext;
                }
                this_Descriptor->m_pNext = m_pOutstandingAllocationList;
                m_pOutstandingAllocationList = this_Descriptor;
                //add X for guardbands
#if defined(_DEBUG)
                char *front = reinterpret_cast<char*>(this_Descriptor->m_pBlockBase);
                char *end = reinterpret_cast<char*>(this_Descriptor->m_pBlockBase) + size - i_guardbandSize;
                for (size_t i = 0; i < i_guardbandSize; i++)
                {
                    front[i] = 'X';
                    end[i] = 'X';
                }
                this_Descriptor->m_pBlockBase = reinterpret_cast<uintptr_t>(front + i_guardbandSize);
                printf("Allocate Memory, Position: %p, Size: %zu\n", this_Descriptor->m_pBlockBase, this_Descriptor->m_sizeBlock);
                return reinterpret_cast<void *>(this_Descriptor->m_pBlockBase);
#else
                printf("Allocate Memory, Position: %lu, Size: %zu\n", this_Descriptor->m_pBlockBase, this_Descriptor->m_BlockSize);
                return reinterpret_cast<void *>(this_Descriptor->m_pBlockBase);
#endif
            }
            //use the BIG memory pool
            else
            {
                //pick a descriptor and point to the big pool
                BlockDescriptor* selectedBlock = m_pFreeDescriptorList;
                if (selectedBlock == nullptr)
                {
                    printf("Not enough Descriptor");
                    return nullptr;
                }
                else
                {
                    selectedBlock->m_BlockSize = size;
                    selectedBlock->m_pBlockBase = this_Descriptor->m_pBlockBase;
                }
#if defined(_DEBUG)
                //for guardbands
                char* front = reinterpret_cast<char*>(selectedBlock->m_pBlockBase);
                char* end = reinterpret_cast<char*>(selectedBlock->m_pBlockBase) + size - i_guardbandSize;
                for (size_t i = 0; i < i_guardbandSize; i++)
                {
                    front[i] = 'X';
                    end[i] = 'X';
                }
                selectedBlock->m_pBlockBase = reinterpret_cast<uintptr_t>(front + i_guardbandSize);
#endif
                m_pFreeDescriptorList = selectedBlock->m_pNext;
                selectedBlock->m_pNext = m_pOutstandingAllocationList;
                m_pOutstandingAllocationList = selectedBlock;
                //Re-define the BIG free pool position and size
                uintptr_t new_position = this_Descriptor->m_pBlockBase;
                this_Descriptor->m_pBlockBase = new_position + size;
                this_Descriptor->m_BlockSize -= size;
                printf("Allocate Memory, Position: %p, Size: %zu\n", selectedBlock->m_pBlockBase, selectedBlock->m_BlockSize);
                return reinterpret_cast<void*>(selectedBlock->m_pBlockBase);
            }
        }
    }
    printf("No enough Memory\n");
    
    return nullptr;
}


bool HeapManager::_free(const void* i_pMemory)
{
    return false;
}


void HeapManager::_recycle()
{
    
}