#pragma once

#include <stdio.h>
#include <BitArray.h>
#include <FixedSizeAllocator.h>

namespace Engine
{
	namespace Memory
	{
		inline void FixedSizeAllocator::_Clear()
		{
			m_pStates->_ClearAll();
		}

		inline void FixedSizeAllocator::_Destroy()
		{
			_Clear();
			delete m_pStates;
		}

		inline bool FixedSizeAllocator::_IsAvailable() const
		{
			return !m_pStates->_AreAllSet();
		}

		inline const FSA_INFO & FixedSizeAllocator::_GetINFO() const
		{
			return m_INFO;
		}
	}
}