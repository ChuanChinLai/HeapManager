//
//  BitArray.cpp
//  HeapManager
//
//  Created by CCLai on 2017/1/21.
//  Copyright © 2017年 CCLai. All rights reserved.
//
#include <iostream>
#include "BitArray.hpp"
#include "HeapManager.hpp"

uint8_t const BitArray::BitsPerBytes = 8;

BitArray::BitArray(const size_t i_NumBits, uint8_t* i_pBitArray): m_NumBits(i_NumBits), m_pBitArray(i_pBitArray)
{
    //calculate number of bytes
    m_NumBytes = (i_NumBits + (BitsPerBytes - 1) / BitsPerBytes);
    
    assert(m_pBitArray);
    assert(m_NumBytes > 0);
    
    //clear all bits;
    ClearAll();
};

BitArray::~BitArray()
{
    delete m_pBitArray;
}

BitArray* BitArray::_create(const size_t i_NumBits, HeapManager* i_pHeapManager)
{
    assert(i_pHeapManager);
    size_t NumBytes = (i_NumBits + (BitsPerBytes - 1) / BitsPerBytes);
    
    //Total Memory = BitArray memory + Bytes memory;
    size_t TotalMemory = NumBytes * sizeof(uint8_t) + sizeof(BitArray);
    
    //allocate a memory block for BitArray
    uint8_t* pMemory = reinterpret_cast<uint8_t*>(i_pHeapManager->_alloc(TotalMemory));
    
    assert(pMemory);
    
    //pointer to BitArray
    uint8_t* pBitArray = reinterpret_cast<uint8_t*>(pMemory + sizeof(BitArray));
    
    //using placement new to create a BitArray
    return new(pMemory) BitArray(i_NumBits, pBitArray);
}

void BitArray::ClearAll(void)
{
    assert(m_pBitArray);
    
    for (size_t i = 0; i < m_NumBits; i++)
    {
        ClearBit(i);
    }
}


void BitArray::SetAll(void)
{
    assert(m_pBitArray);
    
    for (size_t i = 0; i < m_NumBytes; i++)
    {
        SetBit(1);
    }
}


bool BitArray::AreAllClear(void) const
{
    assert(m_pBitArray);
    
    for (size_t i = 0; i < m_NumBits; i++)
    {
        if (IsBitSet(i))
        {
            return false;
        }
    }
    return true;
}


bool BitArray::AreAllSet(void) const
{
    assert(m_pBitArray);
    
    for (size_t i = 0; i < m_NumBits; i++)
    {
        if (IsBitClear(i))
        {
            return false;
        }
    }
    return true;
}


bool BitArray::IsBitClear(const size_t i_bitNumber) const
{
    assert(m_pBitArray);
    
    const size_t SHIFT = BitsPerBytes * sizeof(uint8_t) - 1;
    const size_t index = i_bitNumber / BitsPerBytes;
    const uint8_t position = i_bitNumber % BitsPerBytes;
    const uint8_t MASK = 1 << SHIFT;
    uint8_t temp = m_pBitArray[index] << position;
    
    if ((temp & MASK) == 0x00)
        return true;
    else
        return false;
}


bool BitArray::IsBitSet(const size_t i_bitNumber) const
{
    assert(m_pBitArray);
    
    const size_t SHIFT = BitsPerBytes * sizeof(uint8_t) - 1;
    const size_t index = i_bitNumber / BitsPerBytes;
    const uint8_t position = i_bitNumber % BitsPerBytes;
    const uint8_t MASK = 1 << SHIFT;
    uint8_t temp = m_pBitArray[index] << position;
    
    if ((temp & MASK) == MASK)
        return true;
    else
        return false;
}


void BitArray::ClearBit(const size_t i_bitNumber)
{
    assert(m_pBitArray);
    const size_t SHIFT = BitsPerBytes * sizeof(uint8_t) - 1;
    const size_t index = i_bitNumber / BitsPerBytes;
    const uint8_t position = SHIFT - i_bitNumber % BitsPerBytes;
    const uint8_t MASK = 1 << position;
    
    m_pBitArray[index] = m_pBitArray[index] & ~MASK;
}


void BitArray::SetBit(const size_t i_bitNumber)
{
    assert(m_pBitArray);
    const size_t SHIFT = BitsPerBytes * sizeof(uint8_t) - 1;
    const size_t index = i_bitNumber / BitsPerBytes;
    const uint8_t position = SHIFT - i_bitNumber % BitsPerBytes;
    const uint8_t MASK = 1 << position;
    
    m_pBitArray[index] = m_pBitArray[index] | MASK;
}


bool BitArray::GetFirstClearBit(size_t & o_bitNumber) const
{
    assert(m_pBitArray);
    
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


bool BitArray::GetFirstSetBit(size_t & o_bitNumber) const
{
    assert(m_pBitArray);
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
    assert(m_pBitArray);
    assert(i_bitNumber < m_NumBits);
    
    return IsBitSet(i_bitNumber);
}