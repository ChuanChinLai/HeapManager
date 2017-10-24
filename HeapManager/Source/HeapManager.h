#pragma once

#include <stdint.h>
#include <vector>
#include <MemoryManager.h>

namespace Engine
{
	namespace Memory
	{
		class BlockDescriptor;
		class FixedSizeAllocator;

		class HeapManager
		{
		public:

			static HeapManager* _Create(void* i_pMemoryPool, const size_t i_PoolSize, const size_t i_NumDescriptors);
			static inline HeapManager* _Get();
			void  _Destroy();

			void* _Alloc(const size_t i_Size, const size_t i_AlignmentSize = ALIGNMENT_DEFAULT);
			bool  _Free(const void* i_pMemory);
			void  _Recycle();
			void  _Display() const;

		private:

			HeapManager();
			~HeapManager();
			HeapManager(const HeapManager&);
			HeapManager& operator = (const HeapManager&);


			HeapManager* _Init(void* i_pMemoryPool, const size_t i_PoolSize, const size_t i_NumDescriptors);
			void		 _Display(const BlockDescriptor* i_pList) const;

			size_t				m_SIZE;
			uintptr_t			m_pMemoryPool;

			BlockDescriptor*	m_pFreeMemoryList;
			BlockDescriptor*	m_pFreeDescriptorList;
			BlockDescriptor*	m_pOutstandingAllocationList;

			static HeapManager* s_pHeapManager;
		};
	}
}

#include <HeapManager_inline.h>