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
#include "Block_Descriptor.hpp"
#include "ToolKit.hpp"
#include "Unit_Test.hpp"

int main(int argc, const char * argv[])
{
    HeapManager_UnitTest();
    
    /*
    const size_t 		sizeHeap = 1024 * 1024;
    const unsigned int 	numDescriptors = 8;
    
    void* pHeapMemory = malloc(sizeHeap);
    printf("Memory Address: %p\n", pHeapMemory);
    
    HeapManager* pHeapProxy = new HeapManager;
    HeapManager* pHeap = pHeapProxy->_create(pHeapMemory, sizeHeap, numDescriptors);
    
    
    printf("Memory Address: %d  %p\n", sizeof(BlockDescriptor), pHeap);
    
    void* m1 = pHeap->_alloc(12);

    pHeap->_display();
    
    pHeap->_free(m1);

    
    
    pHeap->_display();
    pHeap->_recycle();
    pHeap->_display();
    */
    return 0;
}
