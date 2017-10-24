#include <ToolKit.h>
#include <BlockDescriptor.h>

namespace Engine
{
	namespace Memory
	{
		void Swap(BlockDescriptor * descriptor_1, BlockDescriptor * descriptor_2)
		{
			uintptr_t tmp_address = descriptor_1->m_pBlockAddress;
			size_t tmp_size = descriptor_1->m_BlockSize;

			descriptor_1->m_pBlockAddress = descriptor_2->m_pBlockAddress;
			descriptor_1->m_BlockSize = descriptor_2->m_BlockSize;

			descriptor_2->m_pBlockAddress = tmp_address;
			descriptor_2->m_BlockSize = tmp_size;
		}

		void Sort(BlockDescriptor * DescriptorList)
		{
			if (DescriptorList == nullptr)
				return;

			BlockDescriptor* this_descriptor = DescriptorList;

			while (this_descriptor != nullptr)
			{
				BlockDescriptor* next_descriptor = this_descriptor->m_pNext;

				while (next_descriptor != nullptr)
				{
					if (this_descriptor->m_BlockSize >= next_descriptor->m_BlockSize)
					{
						Swap(this_descriptor, next_descriptor);
					}
					next_descriptor = next_descriptor->m_pNext;
				}
				this_descriptor = this_descriptor->m_pNext;
			}
		}

	}
}

