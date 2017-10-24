#pragma once

#include <cmath>

namespace Engine
{
    namespace Math
    {
        const float PI = static_cast<float>(std::acos(-1));
        
        inline bool IsNaN(const float i_value)
        {
            volatile float value = i_value;
            return value != value;
        }
        
        inline float toRad(const float i_degree)
        {
            return PI * i_degree / 180.0f;
        }
    }

	namespace Memory
	{
		class BlockDescriptor;

		void Swap(BlockDescriptor *descriptor_1, BlockDescriptor *descriptor_2);
		void Sort(BlockDescriptor *DescriptorList);
	}
}
