//
//  BitArray.hpp
//  HeapManager
//
//  Created by CCLai on 2017/1/21.
//  Copyright © 2017年 CCLai. All rights reserved.
//

#ifndef BitArray_hpp
#define BitArray_hpp

#include <stdio.h>
#include <stdlib.h>
#include <cassert>

extern void BitArray_UnitTest(void);

class HeapManager;

class BitArray
{
public:
    
    static BitArray* _create(const size_t i_numBits, HeapManager* i_pAllocator);
    
    ~BitArray();
    
    void _display(void) const;
    
    void ClearAll(void);
    void SetAll(void);
    
    bool AreAllClear(void) const;
    bool AreAllSet(void) const;
    
    bool IsBitClear(const size_t i_bitNumber) const;
    bool IsBitSet(const size_t i_bitNumber) const;
    
    void ClearBit(const size_t i_bitNumber);
    void SetBit(const size_t i_bitNumber);
    
    bool GetFirstClearBit(size_t & o_bitNumber) const;
    bool GetFirstSetBit(size_t & o_bitNumber) const;
    
    bool operator[](size_t i_bitNumber) const;
    
    BitArray(const size_t i_NumBits, uint8_t* i_pBits): bitsPerBytes(8), m_NumBits(i_NumBits), m_pBits(i_pBits)
    {
        
        m_NumBytes = i_NumBits / bitsPerBytes + 1;
        
        if (i_NumBits % bitsPerBytes == 0)
            m_NumBytes--;
        
        assert(m_pBits);
        
        ClearAll();
    };
    
private:

    size_t m_NumBits;
    size_t m_NumBytes;
    uint8_t* m_pBits;
    uint8_t bitsPerBytes;
};

#endif /* BitArray_hpp */
