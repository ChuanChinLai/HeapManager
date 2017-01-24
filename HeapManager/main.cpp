//
//  main.cpp
//  HeapManager
//
//  Created by CCLai on 2017/1/16.
//  Copyright © 2017年 CCLai. All rights reserved.
//

#include <algorithm>
#include <iostream>

#include "HeapManager.hpp"
#include "FixedSizeAllocator.hpp"
#include "BitArray.hpp"
#include "ToolKit.hpp"
#include "Unit_Test.hpp"

int main(int argc, const char * argv[])
{
    //    using namespace HeapManagerProxy;
    const size_t 		sizeHeap = 1024 * 1024;
    const unsigned int 	numDescriptors = 16;
    // Allocate memory for my test heap.
    void * pHeapMemory = malloc(sizeHeap);
    assert( pHeapMemory );
    
    HeapManager* pHeap = HeapManager::_init(pHeapMemory, sizeHeap, numDescriptors);
    // Create a heap manager for my test heap.
    
    BitArray_UnitTest();
    
    pHeap->_display();
    
    HeapManager_UnitTest();

    

//    pHeap->_recycle();
//    pHeap->_display();
    

//    HeapManager::_init(pHeapMemory, sizeHeap, numDescriptors);

    return 0;
}
