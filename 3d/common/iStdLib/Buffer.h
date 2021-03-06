/*===================== begin_copyright_notice ==================================

Copyright (c) 2017 Intel Corporation

Permission is hereby granted, free of charge, to any person obtaining a
copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be included
in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.


======================= end_copyright_notice ==================================*/
#pragma once
#include "LinearAllocator.h"
#include "MemCopy.h"

namespace iSTD
{

/*****************************************************************************\

Class:
    CBuffer

Description:
    Allocates and manages a system memory buffer

\*****************************************************************************/
template<class CAllocatorType>
class CBuffer : public CLinearAllocator<CAllocatorType>
{
public:

    CBuffer( void );
    virtual ~CBuffer( void );

    bool    Allocate( const DWORD allocSize, const DWORD alignSize );
    void    Deallocate( void );

    DWORD   GetBlockSize( void ) const;
    void*   GetLinearAddress( void ) const;

protected:

    BYTE*   m_pAllocAddress;    // Address of allocation pointer
};

/*****************************************************************************\

Function:
    CBuffer Constructor

Description:
    Initializes internal data

Input:
    none

Output:
    none

\*****************************************************************************/
template<class CAllocatorType>
inline CBuffer<CAllocatorType>::CBuffer( void )
    : CLinearAllocator<CAllocatorType>( NULL, 0 )
{
    m_pAllocAddress = NULL;
}

/*****************************************************************************\

Function:
    CBuffer Destructor

Description:
    Deletes internal data

Input:
    none

Output:
    none

\*****************************************************************************/
template<class CAllocatorType>
inline CBuffer<CAllocatorType>::~CBuffer( void )
{
    Deallocate();
}

/*****************************************************************************\

Function:
    CBuffer::Allocate

Description:
    Allocates memory for the buffer

Input:
    const DWORD allocSize - size in bytes
    const DWORD alignSize - alignment in bytes

Output:
    bool - success or fail

\*****************************************************************************/
template<class CAllocatorType>
inline bool CBuffer<CAllocatorType>::Allocate(
    const DWORD allocSize,
    const DWORD alignSize )
{
    Deallocate();

    const DWORD alignedAllocSize = allocSize + alignSize;

    if( alignedAllocSize )
    {
        m_pAllocAddress = (BYTE*)CAllocatorType::Allocate( alignedAllocSize );

        if( m_pAllocAddress )
        {
            const DWORD offset = ( alignSize )
                ? GetAlignmentOffset( m_pAllocAddress, alignSize )
                : 0;

            this->m_pBaseAddress = this->m_pAllocAddress + offset;
            this->m_Size = allocSize;

            SafeMemSet( this->m_pBaseAddress, 0, this->m_Size );
        }
        else
        {
            ASSERT(0);
            this->m_Size = 0;
        }
    }
    else
    {
        ASSERT(0);
        this->m_Size = 0;
    }

    this->m_SizeUsed = 0;
    this->m_SizeReserved = 0;

    return ( this->m_Size ) ? true : false;
}

/*****************************************************************************\

Function:
    CBuffer::Deallocate

Description:
    Deallocates memory for the buffer

Input:
    none

Output:
    none

\*****************************************************************************/
template<class CAllocatorType>
inline void CBuffer<CAllocatorType>::Deallocate( void )
{
    CAllocatorType::Deallocate( m_pAllocAddress );
    m_pAllocAddress = NULL;

    this->m_pBaseAddress = NULL;
    this->m_Size = 0;
    this->m_SizeUsed = 0;
    this->m_SizeReserved = 0;
}

/*****************************************************************************\

Function:
    CBuffer::GetBlockSize

Description:
    Gets the total size of the buffer

Input:
    none

Output:
    DWORD - size in bytes

\*****************************************************************************/
template<class CAllocatorType>
inline DWORD CBuffer<CAllocatorType>::GetBlockSize( void ) const
{
    return this->m_Size;
}

/*****************************************************************************\

Function:
    CBuffer::GetLinearAddress

Description:
    Gets the base address of the buffer

Input:
    void

Output:
    void* - linear address

\*****************************************************************************/
template<class CAllocatorType>
inline void* CBuffer<CAllocatorType>::GetLinearAddress( void ) const
{
    return (void*)(this->m_pBaseAddress);
}

} // iSTD
