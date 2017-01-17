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
    // insert code here...
    
    void* a1 = malloc(1024);
    printf("Memory Address: %p\n", a1);
    
    HeapManager* pHeapProxy = new HeapManager;
    
    HeapManager* pHeap = pHeapProxy->_create(a1, 1024, 2);
    
    pHeap->_display();


    std::cout << "Hello, World!\n";
    return 0;
}
