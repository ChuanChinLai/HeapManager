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
#include "BitArray.hpp"
#include "ToolKit.hpp"
#include "Unit_Test.hpp"

int main(int argc, const char * argv[])
{

    HeapManager_UnitTest();
    BitArray_UnitTest();
    
    HeapManager* pHeap = HeapManager::_GetHeapManager();
    
    pHeap->_recycle();
    pHeap->_display();

    const size_t 		sizeHeap = 1024 * 1024;
    const unsigned int 	numDescriptors = 8;
    
    void* pHeapMemory = malloc(sizeHeap);
    printf("Memory Address: %p\n", pHeapMemory);

//    HeapManager::_init(pHeapMemory, sizeHeap, numDescriptors);
    HeapManager* q = HeapManager::_init(pHeapMemory, 1000, numDescriptors);
    
    q->_display();
    
    void* m1 = new int[10];

    return 0;
}
