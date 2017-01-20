//
//  Unit_Test.cpp
//  HeapManager
//
//  Created by CCLai on 2017/1/16.
//  Copyright © 2017年 CCLai. All rights reserved.
//

#include "Unit_Test.hpp"
#include "HeapManager.hpp"
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
    const unsigned int 	numDescriptors = 2048;
    // Allocate memory for my test heap.
    void * pHeapMemory = malloc(sizeHeap);
    assert( pHeapMemory );
    // Create a heap manager for my test heap.
    
    
    HeapManager* pHeapProxy = new HeapManager;
    HeapManager *pHeapManager = pHeapProxy->_create(pHeapMemory, sizeHeap, numDescriptors);
    
    assert( pHeapManager );
    if( pHeapManager == NULL )
        return false;
#ifdef TEST_SINGLE_LARGE_ALLOCATION
    // This is a test I wrote to check to see if using the whole block if it was almost consumed by
    // an allocation worked. Also helped test my ShowFreeBlocks() and ShowOutstandingAllocations().
    {
        ShowFreeBlocks( pHeapManager );
        size_t largestBeforeAlloc = GetLargestFreeBlock( pHeapManager );
        void * pPtr = alloc( pHeapManager, largestBeforeAlloc - HeapManager::s_MinumumToLeave );
        if( pPtr )
        {
#ifdef __SHOW_FREE_BLOCKS
            ShowFreeBlocks( pHeapManager );
            printf( "\n" );
#endif // __SHOW_FREE_BLOCKS
#ifdef __SHOW_ALLOCATIONS
            ShowOutstandingAllocations( pHeapManager );
            printf( "\n" );
#endif // __SHOW_ALLOCATIONS
            size_t largestAfterAlloc = GetLargestFreeBlock( pHeapManager );
            free( pHeapManager, pPtr );
#ifdef __SHOW_FREE_BLOCKS
            ShowFreeBlocks( pHeapManager );
#endif // __SHOW_FREE_BLOCKS
#ifdef __SHOW_ALLOCATIONS
            ShowOutstandingAllocations( pHeapManager );
#endif // __SHOW_ALLOCATIONS
            printf( "\n" );
            Collect( pHeapManager );
#ifdef __SHOW_FREE_BLOCKS
            ShowFreeBlocks( pHeapManager );
#endif // __SHOW_FREE_BLOCKS
#ifdef __SHOW_ALLOCATIONS
            ShowOutstandingAllocations( pHeapManager );
#endif // __SHOW_ALLOCATIONS
            printf( "\n" );
            size_t largestAfterCollect = GetLargestFreeBlock( pHeapManager );
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
        void * pPtr = alloc( pHeapManager, sizeAlloc, alignment );
        assert( (reinterpret_cast<uintptr_t>(pPtr) & (alignment - 1)) == 0 );
#else
        void * pPtr = pHeapManager->_alloc(sizeAlloc);
#endif // SUPPORT_ALIGNMENT
        if( pPtr == NULL )
        {
            pHeapManager->_recycle();
#ifdef SUPPORT_ALIGNMENT
            pPtr = alloc( pHeapManager, sizeAlloc, alignment );
#else
            pPtr = pHeapManager->_alloc(sizeAlloc);
#endif // SUPPORT_ALIGNMENT
            if( pPtr == NULL )
            {
                done = true;
                break;
            }
        }
        AllocatedAddresses.push_back( pPtr );
        numAllocs++;
        const unsigned int freeAboutEvery = 10;
        const unsigned int garbageCollectAboutEvery = 40;
        if( !AllocatedAddresses.empty() && ((rand() % freeAboutEvery) == 0) )
        {
            void * pPtr = AllocatedAddresses.back();
            AllocatedAddresses.pop_back();
            bool success = pHeapManager->_free(pPtr);
            assert( success );
            numFrees++;
        }
        if( (rand() % garbageCollectAboutEvery) == 0 )
        {
            pHeapManager->_recycle();
    
            numCollects++;
        }
    } while( 1 );
#ifdef __SHOW_FREE_BLOCKS
    printf( "After exhausting allocations:\n" );
//    ShowFreeBlocks( pHeapManager );
    pHeapManager->_display();
#endif // __SHOW_FREE_BLOCKS
#ifdef __SHOW_ALLOCATIONS
    pHeapManager->_display();
#endif // __SHOW_ALLOCATIONS
    printf( "\n" );
    // now free those blocks in a random order
    if( !AllocatedAddresses.empty() )
    {
        // randomize the addresses
        std::random_shuffle( AllocatedAddresses.begin(), AllocatedAddresses.end() );
        // return them back to the heap manager
        while( !AllocatedAddresses.empty() )
        {
            void * pPtr = AllocatedAddresses.back();
            AllocatedAddresses.pop_back();
            
            /*
            bool success = Contains( pHeapManager, pPtr );
            assert( success );
            
            success = IsAllocated( pHeapManager, pPtr );
            assert( success );
            */
            
            bool success = pHeapManager->_free(pPtr);
            assert( success );
        }
#ifdef __SHOW_FREE_BLOCKS
        printf( "After freeing allocations:\n" );
//        ShowFreeBlocks( pHeapManager );
        pHeapManager->_display();
#endif // __SHOW_FREE_BLOCKS
#ifdef __SHOW_ALLOCATIONS
//        ShowOutstandingAllocations( pHeapManager );
        pHeapManager->_display();
#endif // __SHOW_ALLOCATIONS
        // do garbage collection
        
        pHeapManager->_recycle();
        
//        Collect( pHeapManager );
        // our heap should be one single block, all the memory it started with
#ifdef __SHOW_FREE_BLOCKS
        
        printf( "After garbage collection:\n" );
//        ShowFreeBlocks( pHeapManager );
        pHeapManager->_display();
#endif // __SHOW_FREE_BLOCKS
#ifdef __SHOW_ALLOCATIONS
        pHeapManager->_display();
//        ShowOutstandingAllocations( pHeapManager );
#endif // __SHOW_ALLOCATIONS
        
        printf( "\n" );		// do a large test allocation to see if garbage collection worked
        void * pPtr = pHeapManager->_alloc(sizeHeap / 2);
        assert( pPtr );
        if( pPtr )
        {
            bool success = pHeapManager->_free( pPtr );
            assert( success );
        }
    }
    pHeapManager->_destroy();
    
    pHeapManager = NULL;
    
    free( pHeapMemory );
    // we succeeded
    
    return true;
}