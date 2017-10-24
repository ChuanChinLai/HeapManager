#include <HeapManager.h>
#include <MemoryManager.h>
#include <FixedSizeAllocator.h>
#include <Assert.h>
#include <algorithm>

namespace Engine
{
	namespace Memory
	{
		HeapManager*		pHeapManager = nullptr;
		FixedSizeAllocator* pFSAs[NumFSAs];

		bool _Init()
		{
			void * pDefaultHeapMemory = _aligned_malloc(SizeHeap, ALIGNMENT_DEFAULT);
			pHeapManager = HeapManager::_Create(pDefaultHeapMemory, SizeHeap, NumDescriptors);
			_InitFSA(pHeapManager);

			return true;
		}

		bool _Free()
		{
			_DestroyFSA();
			return true;
		}

		bool _InitFSA(HeapManager* i_pHeapManager)
		{
			ASSERT(i_pHeapManager != nullptr);

			for (int i = 0; i < NumFSAs; i++)
			{
				pFSAs[i] = FixedSizeAllocator::_Create(FSAInitDATA[i], i_pHeapManager);
			}

			return true;
		}

		void _DestroyFSA()
		{
			HeapManager* pHeap = HeapManager::_Get();

			for (int i = 0; i < NumFSAs; i++)
			{
				pFSAs[i]->_Destroy();
				pHeap->_Free(reinterpret_cast<void*>(pFSAs[i]));
			}
		}

		void* _AllocFSA(const size_t i_Size)
		{
			FixedSizeAllocator* pFSA = _GetAvailableFSA(i_Size);
			return pFSA != nullptr ? pFSA->_Alloc() : nullptr;
		}

		bool _FreeFSA(const void * i_pMemory)
		{
			if (i_pMemory == nullptr)
				return false;

			for (int i = 0; i < NumFSAs; i++)
			{
				if (pFSAs[i]->_Free(i_pMemory))
					return true;
			}

			return false;
		}

		FixedSizeAllocator* _GetAvailableFSA(const size_t i_Size)
		{
			for (int i = 0; i < NumFSAs; i++)
			{
				if (pFSAs[i] != nullptr && pFSAs[i]->_IsAvailable() && pFSAs[i]->_GetINFO().BlockSize >= i_Size)
				{
					return pFSAs[i];
				}
			}
			return nullptr;
		}

		bool HeapManager_UnitTest()
		{
			const size_t 		sizeHeap = 1024 * 1024;
			const unsigned int 	numDescriptors = 2048;

			Engine::Memory::HeapManager* pHeap = Engine::Memory::HeapManager::_Get();


#ifdef TEST_SINGLE_LARGE_ALLOCATION
			// This is a test I wrote to check to see if using the whole block if it was almost consumed by 
			// an allocation worked. Also helped test my ShowFreeBlocks() and ShowOutstandingAllocations().
			{
				ShowFreeBlocks(pHeapManager);

				size_t largestBeforeAlloc = GetLargestFreeBlock(pHeapManager);
				void * pPtr = alloc(pHeapManager, largestBeforeAlloc - HeapManager::s_MinumumToLeave);

				if (pPtr)
				{
					ShowFreeBlocks(pHeapManager);
					printf("\n");
#ifdef __TRACK_ALLOCATIONS
					ShowOutstandingAllocations(pHeapManager);
#endif // __TRACK_ALLOCATIONS
					printf("\n");

					size_t largestAfterAlloc = GgetLargestFreeBlock(pHeapManager);
					free(pHeapManager, pPtr);

					ShowFreeBlocks(pHeapManager);
#ifdef __TRACK_ALLOCATIONS
					ShowOutstandingAllocations(pHeapManager);
#endif // __TRACK_ALLOCATIONS
					printf("\n");

					Collect(pHeapManager);

					ShowFreeBlocks(pHeapManager);
#ifdef __TRACK_ALLOCATIONS
					ShowOutstandingAllocations(pHeapManager);
#endif // __TRACK_ALLOCATIONS
					printf("\n");

					size_t largestAfterCollect = GetLargestFreeBlock(pHeapManager);
				}
			}
#endif

			std::vector<void *> AllocatedAddresses;

			long	numAllocs = 0;
			long	numFrees = 0;
			long	numCollects = 0;

			bool	done = false;

			// allocate memory of random sizes up to 1024 bytes from the heap manager
			// until it runs out of memory
			do
			{
				const size_t		maxTestAllocationSize = 64;

				const unsigned int	alignments[] = { 4, 8, 16, 32, 64 };

				unsigned int	index = rand() % (sizeof(alignments) / sizeof(alignments[0]));

				unsigned int	alignment = alignments[index];

				size_t			sizeAlloc = 1 + (rand() & (maxTestAllocationSize - 1));


				void* pPtr = nullptr;

				pPtr = _AllocFSA(sizeAlloc);

				if (pPtr == nullptr)
					pPtr = pHeap->_Alloc(sizeAlloc);


				if (pPtr == nullptr)
				{
					pHeap->_Recycle();

					pPtr = _AllocFSA(sizeAlloc);

					if (pPtr == nullptr)
					{
						pPtr = pHeap->_Alloc(sizeAlloc);
					}

					if (pPtr == NULL)
					{
						done = true;
						break;
					}
				}


				AllocatedAddresses.push_back(pPtr);

				numAllocs++;

				const unsigned int freeAboutEvery = 10;
				const unsigned int garbageCollectAboutEvery = 40;

				if (!AllocatedAddresses.empty() && ((rand() % freeAboutEvery) == 0))
				{
					void * pPtr = AllocatedAddresses.back();
					AllocatedAddresses.pop_back();

					bool success = false;

					if (_FreeFSA(pPtr) == true)
					{
						success = true;
					}
					else
					{
						success = pHeap->_Free(pPtr);
					}

					ASSERT(success);

					numFrees++;
				}

				if ((rand() % garbageCollectAboutEvery) == 0)
				{
					pHeap->_Recycle();

					numCollects++;
				}

			} while (1);

			printf("After exhausting allocations:\n");
			printf("\n");

			// now free those blocks in a random order
			if (!AllocatedAddresses.empty())
			{
				// randomize the addresses
				std::random_shuffle(AllocatedAddresses.begin(), AllocatedAddresses.end());

				// return them back to the heap manager
				while (!AllocatedAddresses.empty())
				{
					void * pPtr = AllocatedAddresses.back();
					AllocatedAddresses.pop_back();

					bool success = _FreeFSA(pPtr);

					if(!success)
						success = pHeap->_Free(pPtr);

//					assert(success, "Could not free memory");
				}

				printf("After freeing allocations:\n");

				// do garbage collection
				pHeap->_Recycle();
				// our heap should be one single block, all the memory it started with

				printf("After garbage collection:\n");

				printf("\n");		// do a large test allocation to see if garbage collection worked
				void * pPtr = pHeap->_Alloc(sizeHeap / 2);

				if (pPtr)
				{
					pHeap->_Free(pPtr);
				}
			}

			pHeap->_Display();

			pHeap->_Recycle();

			// we succeeded
			return true;
		}
	}
}


void * operator new(const size_t i_size)
{
	void* pMemory = Engine::Memory::_AllocFSA(i_size);
	return (pMemory != nullptr) ? pMemory : Engine::Memory::pHeapManager->_Alloc(i_size);
}

void operator delete(void * i_ptr)
{
	if (Engine::Memory::_FreeFSA(i_ptr) == false)
		Engine::Memory::pHeapManager->_Free(i_ptr);
}

void * operator new[](const size_t i_size)
{
	void* pMemory = Engine::Memory::_AllocFSA(i_size);
	return (pMemory != nullptr) ? pMemory : Engine::Memory::pHeapManager->_Alloc(i_size);
}

void operator delete[](void * i_ptr)
{
	if (Engine::Memory::_FreeFSA(i_ptr) == false)
		Engine::Memory::pHeapManager->_Free(i_ptr);
}
