#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <iomanip>
#include <vector>

#include <HeapManager.h>
#include <BlockDescriptor.h>
#include <FixedSizeAllocator.h>

#include <Assert.h>
#include <ToolKit.h>

Engine::Memory::HeapManager* Engine::Memory::HeapManager::s_pHeapManager = nullptr;

namespace Engine
{
	namespace Memory
	{
		HeapManager* HeapManager::_Create(void * i_pMemoryPool, const size_t i_PoolSize, const size_t i_NumDescriptors)
		{
			if (s_pHeapManager == nullptr)
			{
				void* MemoryForManager = malloc(sizeof(HeapManager));

				s_pHeapManager = new (MemoryForManager) HeapManager;

				s_pHeapManager->_Init(i_pMemoryPool, i_PoolSize, i_NumDescriptors);

				#if defined(_DEBUG)
					std::cout << std::endl;
					std::cout << "CREATE A NEW HEAP MANAGER:" << std::endl;
				#endif

			}
			return s_pHeapManager;
		}

		HeapManager* HeapManager::_Init(void * i_pMemoryPool, const size_t i_PoolSize, const size_t i_NumDescriptors)
		{
			ASSERT(i_PoolSize > 0 && i_NumDescriptors > 0);

			//Init Descriptors List:
			m_pFreeMemoryList = nullptr;
			m_pFreeDescriptorList = nullptr;
			m_pOutstandingAllocationList = nullptr;

			//Setting Memory Info:
			m_SIZE = i_PoolSize;
			m_pMemoryPool = reinterpret_cast<uintptr_t>(i_pMemoryPool);

			//Total Descriptors Size:
			size_t DescriptorsTotalSize = i_NumDescriptors * sizeof(BlockDescriptor);
			ASSERT(m_SIZE > DescriptorsTotalSize);


			//position of Pointer
			uintptr_t pCurrentAddress = m_pMemoryPool;
			uintptr_t pUsableMemory = m_pMemoryPool + DescriptorsTotalSize;


			//Init Free Memory List
			m_pFreeMemoryList = reinterpret_cast<BlockDescriptor*>(pCurrentAddress);
			m_pFreeMemoryList->m_pBlockAddress = pUsableMemory;
			m_pFreeMemoryList->m_BlockSize = m_SIZE - DescriptorsTotalSize;
			m_pFreeMemoryList->m_pNext = nullptr;

			//move to next descriptor:
			pCurrentAddress += sizeof(BlockDescriptor);

			//Init Free Descriptors List
			//-1: because 1 Descriptor has been used in pFreeMemoryList
			for (size_t i = 0; i < i_NumDescriptors - 1; i++)
			{
				//Init a new descriptor:
				BlockDescriptor* new_Descriptor = reinterpret_cast<BlockDescriptor*>(pCurrentAddress);
				new_Descriptor->Init();

				if (m_pFreeDescriptorList == nullptr)
				{
					m_pFreeDescriptorList = new_Descriptor;
				}
				else
				{
					//put the new descriptor to FreeDescriptorList:
					BlockDescriptor* this_Descriptor = m_pFreeDescriptorList;
					BlockDescriptor* prev_Descriptor = nullptr;

					while (this_Descriptor != nullptr)
					{
						prev_Descriptor = this_Descriptor;
						this_Descriptor = this_Descriptor->m_pNext;
					}
					prev_Descriptor->m_pNext = new_Descriptor;
				}

				//move to next descriptor:
				pCurrentAddress += sizeof(BlockDescriptor);
			}

			//Init Outstanding Allocation List
			m_pOutstandingAllocationList = nullptr;

			return this;
		}

		void  HeapManager::_Destroy()
		{
			if (s_pHeapManager != nullptr)
			{
				free(s_pHeapManager);
				s_pHeapManager = nullptr;
			}
		}

		void* HeapManager::_Alloc(const size_t i_Size, const size_t i_AlignmentSize)
		{
			//Calculate the memory size:
			ASSERT(i_Size > 0);
			size_t size = (i_Size % i_AlignmentSize == 0) ? i_Size : (i_Size / i_AlignmentSize + 1) * i_AlignmentSize;

			//we can use a Memory Block in pFreeMemoryList for allocating

			BlockDescriptor* this_Descriptor = m_pFreeMemoryList;
			BlockDescriptor* prev_Descriptor = nullptr;

			while (this_Descriptor != nullptr)
			{
				if (this_Descriptor->m_BlockSize < size || this_Descriptor->m_BlockSize > 2 * size)
				{
					prev_Descriptor = this_Descriptor;
					this_Descriptor = this_Descriptor->m_pNext;
				}
				else
				{
					//It is the first node:
					if (this_Descriptor == m_pFreeMemoryList)
					{
						m_pFreeMemoryList = m_pFreeMemoryList->m_pNext;
					}
					else
					{
						prev_Descriptor->m_pNext = this_Descriptor->m_pNext;
					}

					this_Descriptor->m_pNext = m_pOutstandingAllocationList;
					m_pOutstandingAllocationList = this_Descriptor;


#if defined(_DEBUG)
					this_Descriptor->_Display();
					std::cout << " - ALLOC MEMORY WITH REGULAR ALLOCATOR" << std::endl;
#endif;

					return reinterpret_cast<void *>(this_Descriptor->m_pBlockAddress);
				}
			}

			//Or we have to cut a memory block from Biggest Memory Pool for user
			//Last Descriptor in my FreeDescriptorList always has Biggest Size.

			if (m_pFreeDescriptorList == nullptr)
			{
				std::cout << "BLOCK DESCRIPTOR NOT ENOUGH FOR USE" << std::endl;
				return nullptr;
			}

			if (size > prev_Descriptor->m_BlockSize)
			{
				std::cout << "HEAP MEMORY NOT ENOUGH FOR USE" << std::endl;
				return nullptr;
			}

			//pick a new Descriptor from FreeDescriptorList:
			BlockDescriptor* newDescriptor = m_pFreeDescriptorList;
			newDescriptor->m_BlockSize = size;
			newDescriptor->m_pBlockAddress = prev_Descriptor->m_pBlockAddress;
			m_pFreeDescriptorList = newDescriptor->m_pNext;

			//add the Descriptor into OutstandingAllocationList:
			newDescriptor->m_pNext = m_pOutstandingAllocationList;
			m_pOutstandingAllocationList = newDescriptor;

			//Re-calculate the BIG memory pool's address and size:
			prev_Descriptor->m_BlockSize -= size;
			prev_Descriptor->m_pBlockAddress += size;

#if defined(_DEBUG)

			newDescriptor->_Display();
			std::cout << " - ALLOC MEMORY WITH REGULAR ALLOCATOR" << std::endl;
#endif;

			return reinterpret_cast<void*>(newDescriptor->m_pBlockAddress);
		}


		bool  HeapManager::_Free(const void * i_pMemory)
		{
			BlockDescriptor* this_Descriptor = m_pOutstandingAllocationList;
			BlockDescriptor* prev_Descriptor = nullptr;

			while (this_Descriptor != nullptr && this_Descriptor->m_pBlockAddress != reinterpret_cast<uintptr_t>(i_pMemory))
			{
				//move to next descriptor
				prev_Descriptor = this_Descriptor;
				this_Descriptor = this_Descriptor->m_pNext;
			}

			if (this_Descriptor == nullptr)
			{
				//Cannot find any node
				return false;
			}
			else if (this_Descriptor == m_pOutstandingAllocationList)
			{
				//It is the first node 
				m_pOutstandingAllocationList = m_pOutstandingAllocationList->m_pNext;
			}
			else
			{
				prev_Descriptor->m_pNext = this_Descriptor->m_pNext;
			}

			//Insert the descriptor from OutstandingAllocationList into pFreeMemoryList
			//We have to keep the pFreeMemoryList in sorted order (memory size)

			//If FreeMemoryList is null
			if (m_pFreeMemoryList == nullptr)
			{
				m_pFreeMemoryList = this_Descriptor;
				this_Descriptor->m_pNext = nullptr;
			}
			//If the descriptor can be put in the first position of pFreeMemoryList
			else if (m_pFreeMemoryList->m_BlockSize >= this_Descriptor->m_BlockSize)
			{
				this_Descriptor->m_pNext = m_pFreeMemoryList;
				m_pFreeMemoryList = this_Descriptor;
			}
			else
			{
				BlockDescriptor* pThisDES_FML = m_pFreeMemoryList;
				BlockDescriptor* pPrevDES_FML = nullptr;

				while (pThisDES_FML != nullptr && pThisDES_FML->m_BlockSize < this_Descriptor->m_BlockSize)
				{
					pPrevDES_FML = pThisDES_FML;
					pThisDES_FML = pThisDES_FML->m_pNext;
				}

				pPrevDES_FML->m_pNext = this_Descriptor;
				this_Descriptor->m_pNext = pThisDES_FML;
			}

			#if defined(_DEBUG)
			this_Descriptor->_Display();
			std::cout << " -  FREE MEMORY WITH REGULAR ALLOCATOR" << std::endl;
			#endif;

			return true;
		}

		void  HeapManager::_Recycle()
		{
			//If FreeMemoryList is NULL or there is only one MemoryBlock: Do nothing...
			if (m_pFreeMemoryList == nullptr || m_pFreeMemoryList->m_pNext == nullptr)
			{
#if defined(_DEBUG)
				std::cout << std::endl;
				std::cout << std::setw(60) << "GARBAGE COLLECTION UNNECESSARY"; 
				std::cout << std::endl;
#endif
				return;
			}

			//Pick a TARGET MemoryBlock for Garbage Collection:
			BlockDescriptor* pTarget = m_pFreeMemoryList;

			while (pTarget != nullptr)
			{
				bool doCollection = false;

				BlockDescriptor* pPrev = nullptr;
				BlockDescriptor* pThis = m_pFreeMemoryList;

				while (pThis != nullptr)
				{
					//If TARGET can be merged with a MemoryBlock...
					if (pTarget->m_pBlockAddress + pTarget->m_BlockSize == pThis->m_pBlockAddress)
					{
						doCollection = true;

						//Re-calculate TARGET's MemorySize
						pTarget->m_BlockSize += pThis->m_BlockSize;

						//Re-connect descriptors in pFreeMemoryList
						if(pThis == m_pFreeMemoryList)
							m_pFreeMemoryList = pThis->m_pNext;
						else
							pPrev->m_pNext = pThis->m_pNext;

						//Free and put the descriptor merged with TARGET in FreeDescriptorList
						pThis->Init();
						pThis->m_pNext = m_pFreeDescriptorList;
						m_pFreeDescriptorList = pThis;

						//Go to next potential Descriptor
						pThis = pThis->m_pNext;
					}
					else
					{
						//Go to next potential Descriptor
						pPrev = pThis;
						pThis = pThis->m_pNext;
					}
				}

				//If TARGET can not be merged with others, go to next TARGET
				if (doCollection == false)
				{
					pTarget = pTarget->m_pNext;
				}
			}
#if defined(_DEBUG)
			std::cout << std::endl;
			std::cout << std::setw(60) << "GARBAGE COLLECTION SUCCESS" << std::endl;
			std::cout << std::endl;
#endif
			Sort(m_pFreeMemoryList);
		}

		void  HeapManager::_Display() const
		{
			//Display FreeMemoryList
			// "##########" : 10
			std::cout << std::endl;
			std::cout << std::setw(30) << "##############################";
			std::cout << std::setw(30) << "#       FreeMemoryList       #";
			std::cout << std::setw(30) << "##############################";
			std::cout << std::endl << std::endl;

			_Display(m_pFreeMemoryList);

			//Display FreeDescriptor list

			std::cout << std::endl;
			std::cout << std::setw(30) << "##############################";
			std::cout << std::setw(30) << "#     FreeDescriptorList     #";
			std::cout << std::setw(30) << "##############################";
			std::cout << std::endl << std::endl;

			_Display(m_pFreeDescriptorList);

			//Display OutstandingAllocationList

			std::cout << std::endl;
			std::cout << std::setw(30) << "##############################";
			std::cout << std::setw(30) << "#  OutstandingAllocationList #";
			std::cout << std::setw(30) << "##############################";
			std::cout << std::endl << std::endl;

			_Display(m_pOutstandingAllocationList);
		}

		void  HeapManager::_Display(const BlockDescriptor * i_pList) const
		{
			if (i_pList == nullptr)
			{
				std::cout << std::setw(20) << "NULL" << std::endl;
			}
			else
			{
				BlockDescriptor* this_node = const_cast<BlockDescriptor*>(i_pList);
				std::cout << std::setw(20) << "BLOCK DESCRIPTOR"; 
				std::cout << std::setw(20) << "MEMORY ADDRESS"; 
				std::cout << std::setw(20) << "SIZE";
				std::cout << std::endl << std::endl;

				while (this_node != nullptr)
				{
					std::cout << std::setw(20) << this_node; 
					std::cout << std::setw(20) << this_node->m_pBlockAddress;
					std::cout << std::setw(20) << this_node->m_BlockSize;
					std::cout << std::endl;

					this_node = this_node->m_pNext;
				}
			}
		}
	}
}
