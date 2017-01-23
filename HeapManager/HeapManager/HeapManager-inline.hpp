//
//  HeapManager-inline.hpp
//  HeapManager
//
//  Created by CCLai on 2017/1/21.
//  Copyright © 2017年 CCLai. All rights reserved.
//

#ifndef HeapManager_inline_h
#define HeapManager_inline_h

#include "HeapManager.hpp"

inline void* operator new(const size_t i_size)
{
    return HeapManager::pHeapManager->_alloc(i_size);
}

inline void operator delete(void * i_ptr)
{
    if (i_ptr)
    {
        HeapManager::pHeapManager->_free(i_ptr);
    }
}

#endif /* HeapManager_inline_h */
