#pragma once

#include <malloc.h>
#include <HeapManager.h>

namespace Engine
{
	namespace Memory
	{
		inline HeapManager::HeapManager() : m_pFreeMemoryList(nullptr), m_pFreeDescriptorList(nullptr), m_pOutstandingAllocationList(nullptr)
		{

		}

		inline HeapManager::~HeapManager()
		{
			_Destroy();

			if (m_pMemoryPool != NULL)
				_aligned_free(reinterpret_cast<void*>(m_pMemoryPool));
		}

		inline HeapManager* HeapManager::_Get()
		{
			return s_pHeapManager;
		}
	}
}