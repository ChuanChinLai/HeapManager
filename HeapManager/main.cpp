//
//  main.cpp
//  HeapManager
//
//  Created by CCLai on 2017/1/16.
//  Copyright © 2017年 CCLai. All rights reserved.
//

#include <iostream>
#include "HeapManager.hpp"

int main(int argc, const char * argv[])
{
    const size_t 		sizeHeap = 1024 * 1024;
    const unsigned int 	numDescriptors = 2048;
    
    void* pHeapMemory = malloc(sizeHeap);
    printf("Memory Address: %p\n", pHeapMemory);
    
    HeapManager* pHeapProxy = new HeapManager;
    HeapManager* pHeap = pHeapProxy->_create(pHeapMemory, sizeHeap, numDescriptors);
    
    pHeap->_display();

    return 0;
}
