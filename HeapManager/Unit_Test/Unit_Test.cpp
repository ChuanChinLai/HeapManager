//
//  Unit_Test.cpp
//  HeapManager
//
//  Created by CCLai on 2017/1/16.
//  Copyright © 2017年 CCLai. All rights reserved.
//

#include "Unit_Test.hpp"
#include "HeapManager.hpp"
#include "BitArray.hpp"

#include <cassert>
#include <string.h>
#include <sys/malloc.h>
#include <assert.h>
#include <algorithm>
#include <vector>
#define _
#define __SHOW_FREE_BLOCKS
#define __SHOW_ALLOCATIONS

bool HeapManager_UnitTest()
{
//    using namespace HeapManagerProxy;
    const size_t 		sizeHeap = 1024 * 1024;
    const unsigned int 	numDescriptors = 8;
    // Allocate memory for my test heap.
    void * pHeapMemory = malloc(sizeHeap);
    assert( pHeapMemory );
    // Create a heap manager for my test heap.
    
    
    HeapManager* pHeapProxy = HeapManager::_init(pHeapMemory, sizeHeap, numDescriptors);

    assert(pHeapProxy);
    if( pHeapProxy == NULL )
        return false;
#ifdef TEST_SINGLE_LARGE_ALLOCATION
    // This is a test I wrote to check to see if using the whole block if it was almost consumed by
    // an allocation worked. Also helped test my ShowFreeBlocks() and ShowOutstandingAllocations().
    {
        ShowFreeBlocks( s_pHeapManager );
        size_t largestBeforeAlloc = GetLargestFreeBlock( s_pHeapManager );
        void * pPtr = alloc( s_pHeapManager, largestBeforeAlloc - HeapManager::s_MinumumToLeave );
        if( pPtr )
        {
#ifdef __SHOW_FREE_BLOCKS
            ShowFreeBlocks( s_pHeapManager );
            printf( "\n" );
#endif // __SHOW_FREE_BLOCKS
#ifdef __SHOW_ALLOCATIONS
            ShowOutstandingAllocations( s_pHeapManager );
            printf( "\n" );
#endif // __SHOW_ALLOCATIONS
            size_t largestAfterAlloc = GetLargestFreeBlock( s_pHeapManager );
            free( s_pHeapManager, pPtr );
#ifdef __SHOW_FREE_BLOCKS
            ShowFreeBlocks( s_pHeapManager );
#endif // __SHOW_FREE_BLOCKS
#ifdef __SHOW_ALLOCATIONS
            ShowOutstandingAllocations( s_pHeapManager );
#endif // __SHOW_ALLOCATIONS
            printf( "\n" );
            Collect( s_pHeapManager );
#ifdef __SHOW_FREE_BLOCKS
            ShowFreeBlocks( s_pHeapManager );
#endif // __SHOW_FREE_BLOCKS
#ifdef __SHOW_ALLOCATIONS
            ShowOutstandingAllocations( s_pHeapManager );
#endif // __SHOW_ALLOCATIONS
            printf( "\n" );
            size_t largestAfterCollect = GetLargestFreeBlock( s_pHeapManager );
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
        const size_t		maxTestAllocationSize = 1024;
        size_t			sizeAlloc = 1 + ( rand() & ( maxTestAllocationSize - 1 ) );
        
#ifdef SUPPORT_ALIGNMENT
        const unsigned int	alignments[] = { 4, 8, 16, 32, 64 };
        unsigned int	index = rand() % (sizeof( alignments ) / sizeof( alignments[0] ));
        unsigned int	alignment = alignments[index];
        void * pPtr = alloc( s_pHeapManager, sizeAlloc, alignment );
        assert( (reinterpret_cast<uintptr_t>(pPtr) & (alignment - 1)) == 0 );
#else
        void * pPtr = pHeapProxy->_alloc(sizeAlloc);
        
        
#endif // SUPPORT_ALIGNMENT
        if( pPtr == NULL )
        {
            pHeapProxy->_recycle();
#ifdef SUPPORT_ALIGNMENT
            pPtr = alloc( s_pHeapManager, sizeAlloc, alignment );
#else
            pPtr = pHeapProxy->_alloc(sizeAlloc);
#endif // SUPPORT_ALIGNMENT
            if( pPtr == NULL )
            {
                done = true;
                break;
            }
        }
        
        printf("vector: %p\n", pPtr);
        AllocatedAddresses.push_back( pPtr );
        numAllocs++;
        
        const unsigned int freeAboutEvery = 10;
        const unsigned int garbageCollectAboutEvery = 40;
        
        if( !AllocatedAddresses.empty() && ((rand() % freeAboutEvery) == 0) )
        {
            void * pPtr = AllocatedAddresses.back();
            AllocatedAddresses.pop_back();
            bool success = pHeapProxy->_free(pPtr);
            assert( success );
            numFrees++;
        }
        
        if( (rand() % garbageCollectAboutEvery) == 0 )
        {
            pHeapProxy->_recycle();
    
            numCollects++;
        }
    } while( 1 );
#ifdef __SHOW_FREE_BLOCKS
    printf( "After exhausting allocations:\n" );
//    ShowFreeBlocks( s_pHeapManager );
    pHeapProxy->_display();
#endif // __SHOW_FREE_BLOCKS
#ifdef __SHOW_ALLOCATIONS
    pHeapProxy->_display();
#endif // __SHOW_ALLOCATIONS
    printf( "\n" );
    // now free those blocks in a random order
    if( !AllocatedAddresses.empty() )
    {
        // randomize the addresses
        std::random_shuffle( AllocatedAddresses.begin(), AllocatedAddresses.end() );
        
        printf("Size: %lu\n", AllocatedAddresses.size());
        
        for (std::vector<void*>::iterator i = AllocatedAddresses.begin(); i != AllocatedAddresses.end(); i++)
        {
            printf("%p\n", *i);
        }
        
        
        
        // return them back to the heap manager
        while( !AllocatedAddresses.empty() )
        {
            void * pPtr = AllocatedAddresses.back();
            AllocatedAddresses.pop_back();
            
            /*
            bool success = Contains( s_pHeapManager, pPtr );
            assert( success );
            
            success = IsAllocated( s_pHeapManager, pPtr );
            assert( success );
            */
            
            bool success = pHeapProxy->_free(pPtr);
            assert( success );
        }
#ifdef __SHOW_FREE_BLOCKS
        printf( "After freeing allocations:\n" );
//        ShowFreeBlocks( s_pHeapManager );
        pHeapProxy->_display();
#endif // __SHOW_FREE_BLOCKS
#ifdef __SHOW_ALLOCATIONS
//        ShowOutstandingAllocations( s_pHeapManager );
        pHeapProxy->_display();
#endif // __SHOW_ALLOCATIONS
        // do garbage collection
        
        pHeapProxy->_recycle();
        
//        Collect( s_pHeapManager );
        // our heap should be one single block, all the memory it started with
#ifdef __SHOW_FREE_BLOCKS
        
        printf( "After garbage collection:\n" );
//        ShowFreeBlocks( s_pHeapManager );
        pHeapProxy->_display();
#endif // __SHOW_FREE_BLOCKS
#ifdef __SHOW_ALLOCATIONS
        pHeapProxy->_display();
//        ShowOutstandingAllocations( s_pHeapManager );
#endif // __SHOW_ALLOCATIONS
        
        printf( "\n" );		// do a large test allocation to see if garbage collection worked
        void * pPtr = pHeapProxy->_alloc(sizeHeap / 2);
        assert( pPtr );
        if( pPtr )
        {
            bool success = pHeapProxy->_free( pPtr );
            assert( success );
        }
    }
    
//    pHeapProxy->_destroy();
//    pHeapProxy = NULL;
    
    free( pHeapMemory );
    // we succeeded
    
    return true;
}

void BitArray_UnitTest(void)
{
    const size_t bitCount = 1000;
    BitArray* pMyArray = BitArray::_create(bitCount, HeapManager::_GetHeapManager());
    
    pMyArray->SetBit(20);
    size_t firstSetBit = 0;
    size_t firstClearBit = 0;
    bool foundSetBit = pMyArray->GetFirstSetBit(firstSetBit);
    assert(foundSetBit && (firstSetBit == 20));
    pMyArray->ClearBit(20);
    foundSetBit = pMyArray->GetFirstSetBit(firstSetBit);
    assert(foundSetBit == false);
    
    for (unsigned int i = 0; i < bitCount; i++)
    {
        assert(pMyArray->IsBitClear(i) == true);
        assert(pMyArray->IsBitSet(i) == false);
        size_t bit = 0;
        pMyArray->GetFirstClearBit(bit);
        assert(bit == i);
        pMyArray->SetBit(i);
        assert(pMyArray->IsBitClear(i) == false);
        assert(pMyArray->IsBitSet(i) == true);
        bool success = pMyArray->GetFirstClearBit(bit);
        assert(((i < (bitCount - 1)) && success && (bit == (i + 1))) || ((i == (bitCount - 1)) && !success));
    }
    pMyArray->SetAll();
    assert(pMyArray->GetFirstClearBit(firstClearBit) == false);
    pMyArray->ClearAll();
    assert(pMyArray->GetFirstSetBit(firstSetBit) == false);
    printf("Pass my Unit_Test for BitArray!!!\n");
    
    delete pMyArray;
    pMyArray = nullptr;
}
