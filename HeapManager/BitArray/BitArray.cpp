//
//  BitArray.cpp
//  HeapManager
//
//  Created by CCLai on 2017/1/21.
//  Copyright © 2017年 CCLai. All rights reserved.
//

#include "BitArray.hpp"
#include "HeapManager.hpp"

BitArray* BitArray::_create(const size_t i_numBits, HeapManager* i_pAllocator)
{
    /*
    static const size_t bitsPerBytes = 8;
    const size_t bitsPerElement = bitsPerBytes * sizeof(uint8_t);
    size_t numElements = (i_numBits + (bitsPerElement - 1) / bitsPerElement);
    size_t totalMemory = sizeof(BitArray) + numElements * sizeof(uint8_t);
    uint8_t * pMemory = reinterpret_cast<uint8_t *>(i_pAllocator->_alloc(totalMemory));
    assert(pMemory);
    uint8_t * pBitMemory = reinterpret_cast<uint8_t *>(pMemory + sizeof(BitArray));
    */
    return nullptr;
//    return new(pMemory) BitArray(i_numBits, pBitMemory);
}

BitArray::~BitArray()
{
    delete[] m_pBits;
}

void BitArray::_display(void) const
{
    /*
    assert(m_pBits);
    
    printf("BitArray Size: %d\n", m_NumBytes);
    
    for (size_t i = 0; i < m_NumBytes; i++)
        
    {
        printf("Index: %d, ", i);
        displayBits(m_pBits[i]);
    }
    */
}


void BitArray::ClearAll(void)
{
    assert(m_pBits);
    
    for (size_t i = 0; i < m_NumBytes; i++)
    {
        m_pBits[i] = 0x00;
    }
}


void BitArray::SetAll(void)
{
    assert(m_pBits);
    
    for (size_t i = 0; i < m_NumBytes; i++)
    {
        m_pBits[i] = 0xFF;
    }
}


bool BitArray::AreAllClear(void) const
{
    assert(m_pBits);
    
    for (size_t i = 0; i < m_NumBytes; i++)
    {
        if (m_pBits[i] != 0x00)
        {
            return false;
        }
    }
    return true;
}


bool BitArray::AreAllSet(void) const
{
    assert(m_pBits);
    
    for (size_t i = 0; i < m_NumBytes; i++)
    {
        if (m_pBits[i] != 0xFF)
        {
            return false;
        }
    }
    return true;
}


bool BitArray::IsBitClear(const size_t i_bitNumber) const
{
    assert(m_pBits);
    
    const size_t SHIFT = bitsPerBytes * sizeof(uint8_t) - 1;
    const size_t index = i_bitNumber / bitsPerBytes;
    const uint8_t position = i_bitNumber % bitsPerBytes;
    const uint8_t MASK = 1 << SHIFT;
    uint8_t temp = m_pBits[index] << position;
    
    if ((temp & MASK) == 0x00)
        return true;
    else
        return false;
}


bool BitArray::IsBitSet(const size_t i_bitNumber) const
{
    assert(m_pBits);
    
    const size_t SHIFT = bitsPerBytes * sizeof(uint8_t) - 1;
    const size_t index = i_bitNumber / bitsPerBytes;
    const uint8_t position = i_bitNumber % bitsPerBytes;
    const uint8_t MASK = 1 << SHIFT;
    uint8_t temp = m_pBits[index] << position;
    
    if ((temp & MASK) == MASK)
        return true;
    else
        return false;
}


void BitArray::ClearBit(const size_t i_bitNumber)
{
    assert(m_pBits);
    const size_t SHIFT = bitsPerBytes * sizeof(uint8_t) - 1;
    const size_t index = i_bitNumber / bitsPerBytes;
    const uint8_t position = SHIFT - i_bitNumber % bitsPerBytes;
    const uint8_t MASK = 1 << position;
    
    m_pBits[index] = m_pBits[index] & ~MASK;
}


void BitArray::SetBit(const size_t i_bitNumber)
{
    assert(m_pBits);
    const size_t SHIFT = bitsPerBytes * sizeof(uint8_t) - 1;
    const size_t index = i_bitNumber / bitsPerBytes;
    const uint8_t position = SHIFT - i_bitNumber % bitsPerBytes;
    const uint8_t MASK = 1 << position;
    
    m_pBits[index] = m_pBits[index] | MASK;
}


bool BitArray::GetFirstClearBit(size_t & o_bitNumber) const
{
    assert(m_pBits);
    
    const size_t SHIFT = bitsPerBytes * sizeof(uint8_t) - 1;
    const uint8_t MASK = 1 << SHIFT;
    size_t index = 0;
    
    while ((m_pBits[index] == 0xFF) && (index < m_NumBytes))
    {
        index++;
        if (index == m_NumBytes)
            return false;
    }
    
    
    uint8_t temp = m_pBits[index];
    
    for (size_t i = 0; i < bitsPerBytes; i++)
    {
        if ((temp & MASK) == 0x00)
        {
            o_bitNumber = index * bitsPerBytes + i;
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
    assert(m_pBits);
    const size_t SHIFT = bitsPerBytes * sizeof(uint8_t) - 1;
    const uint8_t MASK = 1 << SHIFT;
    size_t index = 0;
    
    while ((m_pBits[index] == 0x00) && (index < m_NumBytes))
    {
        index++;
        if (index == m_NumBytes)
        {
            return false;
        }
    }
    
    uint8_t temp = m_pBits[index];
    
    for (size_t i = 0; i < bitsPerBytes; i++)
    {
        if ((temp & MASK) == MASK)
        {
            o_bitNumber = index * bitsPerBytes + i;
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
    assert(m_pBits);
    assert(i_bitNumber < m_NumBits);
    
    return IsBitSet(i_bitNumber);
}

void displayBits(uint8_t i_value)
{
    const int SHIFT = 8 * sizeof(uint8_t) - 1;
    const uint8_t MASK = 1 << SHIFT;
    printf("Value: %d = ", i_value);
    for (size_t i = 0; i <= SHIFT; ++i)
    {
//        std::cout << (i_value & MASK ? '1' : '0');
        i_value <<= 1;
    }
    printf("\n");
}

/*
void BitArray_UnitTest(void)
{
    HeapManager* pHeapManager = HeapManager::pHeapManager;
    
    const size_t bitCount = 1000;
    void* memory = malloc(1024);
    
    BitArray* pMyArray = new BitArray(bitCount, reinterpret_cast<uint8_t*>(memory));
    
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
    
    
    delete pHeapManager;
}
*/