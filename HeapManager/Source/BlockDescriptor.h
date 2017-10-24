#pragma once

#include<stdlib.h>
#include<iostream>
#include<stdio.h>
#include<iomanip>

namespace Engine
{
	namespace Memory
	{
		class BlockDescriptor
		{
		public:

			//Constructor
			BlockDescriptor() : m_pBlockAddress(NULL), m_BlockSize(0), m_pNext(nullptr)
			{

			}

			//Init function
			inline void Init()
			{
				m_pBlockAddress = NULL;
				m_BlockSize = 0;
				m_pNext = nullptr;
			}

			inline void _Display() const
			{
				std::cout << std::setw(20) << this;
				std::cout << std::setw(20) << m_pBlockAddress;
				std::cout << std::setw(20) << m_BlockSize;
			}

			uintptr_t		 m_pBlockAddress;
			size_t			 m_BlockSize;
			BlockDescriptor* m_pNext;
		};
	}
}