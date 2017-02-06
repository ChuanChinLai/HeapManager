//
//  HeapManager.cpp
//  HeapManager
//
//  Created by CCLai on 2017/1/16.
//  Copyright © 2017年 CCLai. All rights reserved.
//
#include <cassert>

#include "Block_Descriptor.hpp"
#include "HeapManager.hpp"
#include "FixedSizeAllocator.hpp"

#include "ToolKit.hpp"


HeapManager* HeapManager::s_pHeapManager = nullptr;

HeapManager* HeapManager::_create(void *i_pMemoryPool, const size_t i_MemorySize, const size_t i_NumDescriptors)
{
    if (s_pHeapManager == nullptr)
    {
        void* MemoryForManager = malloc(sizeof(HeapManager));

        s_pHeapManager = new(MemoryForManager) HeapManager;
        s_pHeapManager->_init(i_pMemoryPool, i_MemorySize, i_NumDescriptors);
        
        printf("\nCreate a new HeapManager\n");
    }
    else
    {
        printf("\nHeapManager already exists\n");
    }
    
    return s_pHeapManager;
}


HeapManager* HeapManager::_init(void *i_pMemoryPool, const size_t i_MemorySize, const size_t i_NumDescriptors)
{
    
    assert(i_MemorySize > 0);
    assert(i_NumDescriptors > 0);
    
    //Init Descriptors List:
    m_pFreeMemoryList = nullptr;
    m_pFreeDescriptorList = nullptr;
    m_pOutstandingAllocationList = nullptr;
    
    //Setting Memory Info:
    m_MemoryTotalSize = i_MemorySize;
    m_pMemoryPool = reinterpret_cast<uintptr_t>(i_pMemoryPool);
    
    //Total Descriptors Size:
    size_t descriptorsTotalSize = i_NumDescriptors * sizeof(BlockDescriptor);
    assert(m_MemoryTotalSize > descriptorsTotalSize);
    
    
    //position of Pointer
    uintptr_t pCurrentAddress = m_pMemoryPool;
    uintptr_t pUsableMemory = m_pMemoryPool + descriptorsTotalSize;
    
    
    //Init Free Memory List
    m_pFreeMemoryList = reinterpret_cast<BlockDescriptor*>(pCurrentAddress);
    m_pFreeMemoryList->m_pBlockAddress = pUsableMemory;
    m_pFreeMemoryList->m_BlockSize = m_MemoryTotalSize - descriptorsTotalSize;
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

    void* pMemoryToFSA = _alloc(_Get_MemoryTotalSize_FSA());
    _init_FixedSizeAllocator(pMemoryToFSA);
    
    return this;
}

void HeapManager::_destroy()
{
    _destroy_FixedSizeAllocator();
    free(s_pHeapManager);
}

void HeapManager::_display() const
{
    //display Memory Block Size:
    printf("Memory Pool Size: %zu\n", m_MemoryTotalSize);
    
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

void* HeapManager::_alloc(const size_t i_Size, const size_t i_AlignedSize)
{
    //Calculate the memory size:
    assert(i_Size > 0);
    
    size_t size = 0;
    
    if (i_Size % m_alignedSize == 0)
        size = i_Size;
    else
        size = (i_Size / m_alignedSize + 1) * m_alignedSize;

    //Check whether the first descriptor can be used for allocating and it is not the BIGGEST one:
    if(m_pFreeMemoryList->m_pNext != nullptr && m_pFreeMemoryList->m_BlockSize >= size)
    {
        BlockDescriptor* this_Descriptor = m_pFreeMemoryList;
        
        m_pFreeMemoryList = m_pFreeMemoryList->m_pNext;
        this_Descriptor->m_pNext = m_pOutstandingAllocationList;
        m_pOutstandingAllocationList = this_Descriptor;
    
        printf("\nAllocate a Memory Block:\n");
        this_Descriptor->_display();
        
        return reinterpret_cast<void *>(this_Descriptor->m_pBlockAddress);
    }
    //Or we can use a good MemoryBlock in pFreeMemoryList for allocating
    else
    {

        BlockDescriptor* this_Descriptor = m_pFreeMemoryList->m_pNext;
        BlockDescriptor* prev_Descriptor = m_pFreeMemoryList;
        
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
            printf("Block Descriptor is Not enough for use...\n");
            return nullptr;
        }
        if(size > prev_Descriptor->m_BlockSize)
        {
            printf("Memory is Not enough for use...\n");
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
        
        //Re-calculate the BIG memory pool's address and size:
        prev_Descriptor->m_BlockSize -= size;
        prev_Descriptor->m_pBlockAddress = prev_Descriptor->m_pBlockAddress + size;
        
        printf("\nAllocate a Memory Block:\n");
        newDescriptor->_display();
        
        return reinterpret_cast<void*>(newDescriptor->m_pBlockAddress);
    }
}


bool HeapManager::_free(const void* i_pMemory)
{
    if (m_pOutstandingAllocationList == nullptr)
        return false;
    
    BlockDescriptor* this_Descriptor = m_pOutstandingAllocationList;
    BlockDescriptor* prev_Descriptor = nullptr;
    
    //Looking for whether the MemoryAddress is in the OutstandingAllocationList
    while (true)
    {
        if (this_Descriptor->m_pBlockAddress == reinterpret_cast<uintptr_t>(i_pMemory))
        {
            //If the MemoryAddress is at the first Descriptor of OutstandingAllocationList
            if (prev_Descriptor == nullptr)
                m_pOutstandingAllocationList = m_pOutstandingAllocationList->m_pNext;
            else
                prev_Descriptor->m_pNext = this_Descriptor->m_pNext;
            
            break;
        }
        
        //move to next descriptor
        prev_Descriptor = this_Descriptor;
        this_Descriptor = this_Descriptor->m_pNext;
        
        //If we cannot find the MemoryAddress in OutstandingAllocationList
        if (this_Descriptor == nullptr)
            return false;
    }
    
    //Insert the descriptor from OutstandingAllocationList into pFreeMemoryList
    //We have to keep the pFreeMemoryList in sorted order (memory size)
    
    if (m_pFreeMemoryList == nullptr)
    {
        m_pFreeMemoryList = this_Descriptor;
        this_Descriptor->m_pNext = nullptr;
        return true;
    }
    
    //If the descriptor can be put in the first position of pFreeMemoryList
    if (m_pFreeMemoryList->m_BlockSize >= this_Descriptor->m_BlockSize)
    {
        this_Descriptor->m_pNext = m_pFreeMemoryList;
        m_pFreeMemoryList = this_Descriptor;
        return true;
    }
    else
    {
        BlockDescriptor* pThisDES_FML = m_pFreeMemoryList;
        BlockDescriptor* pPrevDES_FML = nullptr;
        
        while (pThisDES_FML != nullptr)
        {
            if (pThisDES_FML->m_BlockSize >= this_Descriptor->m_BlockSize)
            {
                pPrevDES_FML->m_pNext = this_Descriptor;
                this_Descriptor->m_pNext = pThisDES_FML;
                return true;
            }
            else
            {
                pPrevDES_FML = pThisDES_FML;
                pThisDES_FML = pThisDES_FML->m_pNext;
            }
        }
        
        pPrevDES_FML->m_pNext = this_Descriptor;
        this_Descriptor->m_pNext = nullptr;
        return true;
    }
    
}


void HeapManager::_recycle()
{
    //If FreeMemoryList is NULL or there is only one MemoryBlock: Do nothing...
    if(m_pFreeMemoryList == nullptr || m_pFreeMemoryList->m_pNext == nullptr)
    {
        printf("\nGarbage Collection is NOT necessary\n");
        return;
    }
    
    //Pick a TARGET MemoryBlock for Garbage Collection:
    BlockDescriptor* pTarget = m_pFreeMemoryList;
    
    while (pTarget != nullptr)
    {
        bool doCollection = false;
        
        BlockDescriptor* pThisDES = m_pFreeMemoryList;
        BlockDescriptor* pPrevDES = nullptr;
        BlockDescriptor* pNextDES = m_pFreeMemoryList->m_pNext;
        
        //Looking for a MemoryBlock that can be merged with TARGET
        while (pThisDES != nullptr)
        {
            //If TARGET can be merged with a MemoryBlock...
            if (pTarget->m_pBlockAddress + pTarget->m_BlockSize == pThisDES->m_pBlockAddress)
            {
                //Re-calculate TARGET's MemorySize
                pTarget->m_BlockSize += pThisDES->m_BlockSize;
                
                //Re-connect descriptors in pFreeMemoryList
                if (pPrevDES == nullptr)
                    m_pFreeMemoryList = pNextDES;
                else
                    pPrevDES->m_pNext = pNextDES;
                
                //Free and put the descriptor merged with TARGET in FreeDescriptorList
                pThisDES->Init();
                pThisDES->m_pNext = m_pFreeDescriptorList;
                m_pFreeDescriptorList = pThisDES;


                doCollection = true;
                printf("Collection Successfully\n");
                
                //Go to next potential Descriptor
                pThisDES = pNextDES;
                pNextDES = (pThisDES == nullptr) ? nullptr : pNextDES->m_pNext;
            }
            else
            {
                //Go to next potential Descriptor
                pPrevDES = pThisDES;
                pThisDES = pNextDES;
                pNextDES = (pThisDES == nullptr) ? nullptr : pNextDES->m_pNext;
            }
        }
        
        //If TARGET can not be merged with others, go to next TARGET
        if (doCollection == false)
        {
            pTarget = pTarget->m_pNext;
        }
    }
    
    ToolKit::sorting(m_pFreeMemoryList);
}

