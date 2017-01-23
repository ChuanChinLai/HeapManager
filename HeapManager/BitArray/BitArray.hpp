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

class HeapManager;

class BitArray
{
public:
    
    ~BitArray();
    
    static BitArray* _create(const size_t i_NumBits, HeapManager* i_pHeapManager);
    
    //Support Function for BitArray:
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
    
private:
    
    BitArray(const size_t i_NumBits, uint8_t* i_pBitArray);
    
    //number of Bits
    size_t m_NumBits;
    
    //number of Bytes
    size_t m_NumBytes;
    
    //Pointer to BitArray
    uint8_t* m_pBitArray;
    
    //BitsPerBytes = 8;
    static const uint8_t BitsPerBytes;
};

#endif /* BitArray_hpp */
