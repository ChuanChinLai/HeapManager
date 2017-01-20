//
//  Block_Descriptor.hpp
//  HeapManager
//
//  Created by CCLai on 2017/1/16.
//  Copyright © 2017年 CCLai. All rights reserved.
//

#ifndef Block_Descriptor_hpp
#define Block_Descriptor_hpp

#include <stdio.h>
#include <stdlib.h>

class BlockDescriptor
{
public:

    //Constructor
    BlockDescriptor() : m_pBlockAddress(NULL), m_BlockSize(0), m_pNext(nullptr)
    {

    }
    
    //Init function
    inline void Init()
    {
        m_pBlockAddress = NULL;
        m_BlockSize = 0;
        m_pNext = nullptr;
    }
    
    inline void _display() const
    {
        printf("Memory Address: %zu\n", m_pBlockAddress);
        printf("Memory    Size: %zu\n", m_BlockSize);
    }
    
    //Pointer be returned to user
    uintptr_t m_pBlockAddress;
    
    //Block Size
    size_t m_BlockSize;
    
    //Pointer points to next Block
    BlockDescriptor *m_pNext;
};

#endif /* Block_Descriptor_hpp */
