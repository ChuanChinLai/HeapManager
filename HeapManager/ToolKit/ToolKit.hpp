//
//  ToolKit.hpp
//  HeapManager
//
//  Created by CCLai on 2017/1/17.
//  Copyright © 2017年 CCLai. All rights reserved.
//

#ifndef ToolKit_hpp
#define ToolKit_hpp

#include <stdio.h>
class BlockDescriptor;

namespace ToolKit
{
    void swap(BlockDescriptor* descriptor_1, BlockDescriptor* descriptor_2);
    void sorting(BlockDescriptor* DescriptorList);
}


#endif /* ToolKit_hpp */
