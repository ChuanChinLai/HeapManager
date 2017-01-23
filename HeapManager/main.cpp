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
    
    HeapManager* pHeap = HeapManager::pHeapManager;
    
    pHeap->_recycle();
    pHeap->_display();
    
    /*
    const size_t 		sizeHeap = 1024 * 1024;
    const unsigned int 	numDescriptors = 8;
    
    void* pHeapMemory = malloc(sizeHeap);
    printf("Memory Address: %p\n", pHeapMemory);
    
    HeapManager::_init(pHeapMemory, sizeHeap, numDescriptors);
    HeapManager* q = HeapManager::_init(pHeapMemory, 1, numDescriptors);
    
    printf("??: %p\n", q);
    */
  
//    void* m1 = new int;

    return 0;
}
