// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
// Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
// Переработано в соответсвии с C++11 (частично) Пановым А.А. 2021
// Битовое поле

#include <limits>
#include <iostream>
#include "tbitfield.h"


TBitField::TBitField(size_t len) :
    bitLen(len),
    memLen(0),
    pMem(nullptr)
{
    if (len != 0)
    {
        memLen = getIndex((len - 1)) + 1;
        pMem = new elType[memLen];
        for (size_t i = 0; i < memLen; ++i) pMem[i] = 0;
    }
}

TBitField::TBitField(const TBitField& bf) // конструктор копирования
{
    bitLen = bf.bitLen;
    memLen = bf.memLen;
    pMem = new elType[memLen];
    for (int i = 0; i < memLen; i++)
    {
        pMem[i] = bf.pMem[i];
    }
}

size_t TBitField::getIndex(const size_t n) const  // индекс в pМем для бита n
{
    if (n < 0 || n >= bitLen) throw n;
    return (n / (sizeof(elType) * 8));
}

elType TBitField::getMask(const size_t n) const // битовая маска для бита n
{
    if (n < 0 || n >= bitLen) throw n;

    return elType(1) << (n % (sizeof(elType) * 8));
}

// доступ к битам битового поля
size_t TBitField::getLength() const // получить длину (к-во битов)
{
    return bitLen;
}

size_t TBitField::getNumBytes() const // получить количество байт выделенной памяти
{
    return memLen * sizeof(elType);
}

void TBitField::setBit(const size_t n) // установить бит
{
    if (n < 0 || n >= bitLen) throw n;
    elType m = getMask(n);
    size_t i = getIndex(n);
    pMem[i] |= m;
}

void TBitField::clrBit(const size_t n) // очистить бит
{
    if (n < 0 || n >= bitLen) throw n;
    int i = getIndex(n);
    elType m = getMask(n);
    pMem[i] = pMem[i] & (~m);
}

bool TBitField::getBit(const size_t n) const // получить значение бита
{
    if (n >= bitLen) throw "Out of range";
    return pMem[getIndex(n)] & getMask(n);
}

// битовые операции
TBitField& TBitField::operator=(const TBitField& bf) // присваивание
{
    int i = 0;
    if (this != &bf)
    {
        delete[] pMem;
        bitLen = bf.bitLen;
        memLen = bf.memLen;
        pMem = new elType[memLen];
        for (i = 0; i < memLen; i++)
        {
            pMem[i] = bf.pMem[i];
        }
    }
    return (*this);
}

bool TBitField::operator==(const TBitField& bf) const // сравнение
{
    if (bitLen != bf.bitLen) return false;
    for (int i = 0; i < memLen; i++)
        if (pMem[i] != bf.pMem[i])
        {
           return false;
        }
    return true;
}


bool TBitField::operator!=(const TBitField& bf) const // сравнение
{
    return !(*this == bf);
}

TBitField TBitField::operator|(const TBitField& bf) // операция "или"
{
    int i;
    if (this == &bf)
    {
        TBitField res(*this);
        return res;
    }
    int len, flag = 0;
    if (bitLen < bf.bitLen)
    {
        len = bf.bitLen;
    }
    else
    {
        flag = 1;
        len = bitLen;
    }
    TBitField res(len);
    if (flag == 0)
    {
        for (i = 0; i < memLen; i++)
        {
            res.pMem[i] = pMem[i];
        }
        for (i = 0; i < bf.memLen; i++)
        {
            res.pMem[i] |= bf.pMem[i];
        }
    }
    else
    {
        for (i = 0; i < bf.memLen; i++)
        {
            res.pMem[i] = bf.pMem[i];
        }
        for (i = 0; i < memLen; i++)
        {
            res.pMem[i] |= pMem[i];
        }
    }
    return res;
}

TBitField TBitField::operator&(const TBitField& bf) // операция "и"
{
    int i;
    if (this == &bf)
    {
        TBitField res(*this);
        return res;
    }
    int len, flag = 0;
    if (bitLen < bf.bitLen)
    {
        len = bf.bitLen;
    }
    else
    {
        flag = 1;
        len = bitLen;
    }
    TBitField res(len);
    if (flag == 0)
    {
        for (i = 0; i < memLen; i++)
        {
            res.pMem[i] = pMem[i];
        }
        for (i = 0; i < bf.memLen; i++)
        {
            res.pMem[i] &= bf.pMem[i];
        }
    }
    else
    {
        for (i = 0; i < bf.memLen; i++)
        {
            res.pMem[i] = bf.pMem[i];
        }
        for (i = 0; i < memLen; i++)
        {
            res.pMem[i] &= pMem[i];
        }
    }
    return res;
}

TBitField TBitField::operator~(void) // отрицание
{
    int i;
    for (i = 0; i < bitLen; i++)
    {
        if (getBit(i) == 0)
        {
            setBit(i);
        }
        else
            clrBit(i);
    }
    return (*this);
}

TBitField::~TBitField()
{
    delete[] pMem;
}

// ввод/вывод
std::istream& operator>>(std::istream& istr, TBitField& bf) // ввод
{
    char x;
    int i;
    for (i = 0; i < bf.bitLen; i++)
    {
        istr >> x;
        if (x == '0')
        {
            bf.clrBit(i);
        }
        else
        {
            if (x == '1')
            {
                bf.setBit(i);
            }
            else
                return istr;
        }
    }
    return istr;
}


std::ostream& operator<<(std::ostream& ostr, const TBitField& bf) // вывод
{
    int i;
    for (i = 0; i < bf.bitLen; i++)
    {
        if (bf.getBit(i))
        {
            ostr << '1';
        }
        else
        {
            ostr << '0';
        }
    }
    return ostr;
}