#pragma once

#include <stdint.h>

namespace Engine
{
	namespace Memory
	{
		class HeapManager;
		class BitArray
		{
		public:

			~BitArray();

			static BitArray* _Create(const size_t i_NumBits, HeapManager* i_pHeapManager);

			//Support Function for BitArray:
			inline void _ClearAll(void);
			inline void _SetAll(void);

			inline bool _AreAllClear(void) const;
			inline bool _AreAllSet(void) const;

			inline bool _IsBitClear(const size_t i_bitNumber) const;
			inline bool _IsBitSet(const size_t i_bitNumber) const;

			inline void _ClearBit(const size_t i_bitNumber);
			inline void _SetBit(const size_t i_bitNumber);

			bool _GetFirstClearBit(size_t & o_bitNumber) const;
			bool _GetFirstSetBit(size_t & o_bitNumber) const;

			bool operator[](size_t i_bitNumber) const;

		private:

			BitArray(const size_t i_NumBits, uint8_t* i_pBitArray);

			size_t	 m_NumBits;
			size_t	 m_NumBytes;
			uint8_t* m_pBitArray;

			//BitsPerBytes = 8;
			static const uint8_t BitsPerBytes;
		};
	}
}

#include <BitArray_inline.h>