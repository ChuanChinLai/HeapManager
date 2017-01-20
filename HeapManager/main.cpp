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

int main(int argc, const char * argv[])
{
    const size_t 		sizeHeap = 1024 * 1024;
    const unsigned int 	numDescriptors = 2048;
    
    void* pHeapMemory = malloc(sizeHeap);
    printf("Memory Address: %p\n", pHeapMemory);
    
    HeapManager* pHeapProxy = new HeapManager;
    HeapManager* pHeap = pHeapProxy->_create(pHeapMemory, sizeHeap, numDescriptors);
    
    
    printf("Memory Address: %d  %p\n", sizeof(BlockDescriptor), pHeap);
    
    
    void* m1 = pHeap->_alloc(12);
    void* m2 = pHeap->_alloc(1);
    void* m3 = pHeap->_alloc(5);
    void* m4 = pHeap->_alloc(34);
    
//    pHeap->_display();
    
    
    
    
    return 0;
}
