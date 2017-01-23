//
//  HeapManager_Operator.hpp
//  HeapManager
//
//  Created by CCLai on 2017/1/23.
//  Copyright © 2017年 CCLai. All rights reserved.
//

#ifndef HeapManager_Operator_hpp
#define HeapManager_Operator_hpp

#include <stdio.h>
#include <stdlib.h>

extern void* operator new(const size_t i_size);
extern void operator delete(void* i_ptr);

extern void* operator new[](const size_t i_size);
extern void operator delete[](void* i_ptr);

extern void* operator new(const size_t i_size, uint8_t i_align);
extern void operator delete(void* i_ptr, uint8_t i_align);




#endif /* HeapManager_Operator_hpp */
