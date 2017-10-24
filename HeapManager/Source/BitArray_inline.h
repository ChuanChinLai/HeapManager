#pragma once

#include <BitArray.h>
#include <assert.h>

namespace Engine
{
	namespace Memory
	{
		inline void BitArray::_ClearAll(void)
		{
			ASSERT(m_pBitArray);

			for (size_t i = 0; i < m_NumBits; i++)
			{
				_ClearBit(i);
			}
		}

		inline void BitArray::_SetAll(void)
		{
			ASSERT(m_pBitArray);

			for (size_t i = 0; i < m_NumBytes; i++)
			{
				_SetBit(1);
			}
		}


		inline bool BitArray::_AreAllClear(void) const
		{
			ASSERT(m_pBitArray);

			for (size_t i = 0; i < m_NumBits; i++)
			{
				if (_IsBitSet(i))
				{
					return false;
				}
			}
			return true;
		}


		inline bool BitArray::_AreAllSet(void) const
		{
			ASSERT(m_pBitArray);

			for (size_t i = 0; i < m_NumBits; i++)
			{
				if (_IsBitClear(i))
				{
					return false;
				}
			}
			return true;
		}


		inline bool BitArray::_IsBitClear(const size_t i_bitNumber) const
		{
			ASSERT(m_pBitArray);

			const size_t SHIFT = BitsPerBytes * sizeof(uint8_t) - 1;
			const size_t index = i_bitNumber / BitsPerBytes;
			const uint8_t position = i_bitNumber % BitsPerBytes;
			const uint8_t MASK = 1 << SHIFT;
			uint8_t temp = m_pBitArray[index] << position;

			return (temp & MASK) == 0x00 ? true : false;
		}


		inline bool BitArray::_IsBitSet(const size_t i_bitNumber) const
		{
			ASSERT(m_pBitArray);

			const size_t SHIFT = BitsPerBytes * sizeof(uint8_t) - 1;
			const size_t index = i_bitNumber / BitsPerBytes;
			const size_t position = i_bitNumber % BitsPerBytes;
			const size_t MASK = 1 << SHIFT;
			uint8_t temp = m_pBitArray[index] << position;

			return (temp & MASK) == MASK ? true : false;
		}


		inline void BitArray::_ClearBit(const size_t i_bitNumber)
		{
			ASSERT(m_pBitArray != nullptr);
			const size_t SHIFT = BitsPerBytes * sizeof(uint8_t) - 1;
			const size_t index = i_bitNumber / BitsPerBytes;
			const size_t position = SHIFT - i_bitNumber % BitsPerBytes;
			const size_t MASK = 1 << position;

			m_pBitArray[index] = m_pBitArray[index] & ~MASK;
		}


		inline void BitArray::_SetBit(const size_t i_bitNumber)
		{
			ASSERT(m_pBitArray);
			const size_t SHIFT = BitsPerBytes * sizeof(uint8_t) - 1;
			const size_t index = i_bitNumber / BitsPerBytes;
			const size_t position = SHIFT - i_bitNumber % BitsPerBytes;
			const size_t MASK = 1 << position;

			m_pBitArray[index] = m_pBitArray[index] | MASK;
		}
	}
}