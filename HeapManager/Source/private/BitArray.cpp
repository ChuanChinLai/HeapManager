#include <iostream>

#include <BitArray.h>
#include <HeapManager.h>
#include <Assert.h>

uint8_t const Engine::Memory::BitArray::BitsPerBytes = 8;

namespace Engine
{
	namespace Memory
	{
		BitArray::BitArray(const size_t i_NumBits, uint8_t* i_pBitArray) : m_NumBits(i_NumBits), m_pBitArray(i_pBitArray)
		{
			//calculate number of bytes
			m_NumBytes = (i_NumBits + (BitsPerBytes - 1) / BitsPerBytes);

			ASSERT(m_pBitArray);
			ASSERT(m_NumBytes > 0);

			//clear all bits;
			_ClearAll();
		};

		BitArray::~BitArray()
		{
			delete (m_pBitArray - sizeof(BitArray));
		}

		BitArray* BitArray::_Create(const size_t i_NumBits, HeapManager* i_pHeapManager)
		{
			ASSERT(i_pHeapManager);
			size_t NumBytes = (i_NumBits + (BitsPerBytes - 1) / BitsPerBytes);

			//Total Memory = BitArray memory itself + extra memory where pBitArray points to;
			size_t TotalMemory = sizeof(BitArray) + NumBytes * sizeof(uint8_t);

			//allocate a memory block for BitArray
			uint8_t* pMemory = reinterpret_cast<uint8_t*>(i_pHeapManager->_Alloc(TotalMemory));

			ASSERT(pMemory);

			//pointer to BitArray
			uint8_t* pBitArray = reinterpret_cast<uint8_t*>(pMemory + sizeof(BitArray));

			//using placement new to create a BitArray
			return new (pMemory) BitArray(i_NumBits, pBitArray);
		}

		bool BitArray::_GetFirstClearBit(size_t & o_bitNumber) const
		{
			ASSERT(m_pBitArray);

			const size_t SHIFT = BitsPerBytes * sizeof(uint8_t) - 1;
			const uint8_t MASK = 1 << SHIFT;
			size_t index = 0;

			while ((m_pBitArray[index] == 0xFF) && (index < m_NumBytes))
			{
				index++;
				if (index == m_NumBytes)
					return false;
			}


			uint8_t temp = m_pBitArray[index];

			for (size_t i = 0; i < BitsPerBytes; i++)
			{
				if ((temp & MASK) == 0x00)
				{
					o_bitNumber = index * BitsPerBytes + i;
					break;
				}
				else
				{
					temp <<= 1;
				}
			}

			if (o_bitNumber < m_NumBits)
				return true;
			else
				return false;
		}


		bool BitArray::_GetFirstSetBit(size_t & o_bitNumber) const
		{
			ASSERT(m_pBitArray);
			const size_t SHIFT = BitsPerBytes * sizeof(uint8_t) - 1;
			const uint8_t MASK = 1 << SHIFT;
			size_t index = 0;

			while ((m_pBitArray[index] == 0x00) && (index < m_NumBytes))
			{
				index++;
				if (index == m_NumBytes)
				{
					return false;
				}
			}

			uint8_t temp = m_pBitArray[index];

			for (size_t i = 0; i < BitsPerBytes; i++)
			{
				if ((temp & MASK) == MASK)
				{
					o_bitNumber = index * BitsPerBytes + i;
					break;
				}
				else
				{
					temp <<= 1;
				}
			}

			if (o_bitNumber < m_NumBits)
				return true;
			else
				return false;
		}


		bool BitArray::operator[](size_t i_bitNumber) const
		{
			ASSERT(m_pBitArray);
			ASSERT(i_bitNumber < m_NumBits);

			return _IsBitSet(i_bitNumber);
		}

	}
}
