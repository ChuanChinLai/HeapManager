#pragma once

#include <FixedSizeAllocator.h>

namespace Engine
{
	namespace Memory
	{
		class HeapManager;
		class FixedSizeAllocator;

		enum AlignmentType
		{
			ALIGNMENT_DEFAULT = 4,
			ALIGNMENT_8 = 8,
			ALIGNMENT_16 = 16,
			ALIGNMENT_32 = 32,
			ALIGNMENT_64 = 64
		};

		const unsigned int 	SizeHeap = 1024 * 1024;
		const unsigned int 	NumDescriptors = 2048;
		const unsigned int	NumFSAs = 4;

		const FSA_INFO FSAInitDATA[NumFSAs] = { FSA_INFO(ALIGNMENT_8,  48),
												FSA_INFO(ALIGNMENT_16, 48),
												FSA_INFO(ALIGNMENT_32, 48),
												FSA_INFO(ALIGNMENT_64, 48) };

		extern HeapManager*		   pHeapManager;
		extern FixedSizeAllocator* pFSAs[];

		FixedSizeAllocator* _GetAvailableFSA(const size_t i_Size);

		bool  _Init();
		bool  _Free();

		bool  _InitFSA(HeapManager* i_pHeapManager);
		void  _DestroyFSA();

		void* _AllocFSA(const size_t i_Size);
		bool  _FreeFSA(const void* i_pMemory);

		bool HeapManager_UnitTest();
	}
}


void* operator new(const size_t i_size);
void  operator delete(void * i_ptr);

void* operator new[](const size_t i_size);
void  operator delete[](void * i_ptr);
