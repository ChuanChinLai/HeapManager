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
    BlockDescriptor() : m_pBlockBase(NULL), m_BlockSize(0), m_pNext(nullptr)
    {

    }
    
    //Init function
    inline void Init()
    {
        m_pBlockBase = NULL;
        m_BlockSize = 0;
        m_pNext = nullptr;
    }
    
    //Pointer be returned to user
    uintptr_t m_pBlockBase;
    
    //Block Size
    size_t m_BlockSize;
    
    //Pointer points to next Block
    BlockDescriptor *m_pNext;
};

#endif /* Block_Descriptor_hpp */
