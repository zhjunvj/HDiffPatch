//adler_roll.h
//support adler32,adler64,roll,FAST_BASE,roll_combine,etc.
//  adler32 support roll by uint8,uint16
//  adler64 support roll by uint8,uint16,uint32
//https://github.com/madler/zlib/blob/master/adler32.c not find roll
//
/*
 The MIT License (MIT)
 Copyright (c) 2017 HouSisong
 
 Permission is hereby granted, free of charge, to any person
 obtaining a copy of this software and associated documentation
 files (the "Software"), to deal in the Software without
 restriction, including without limitation the rights to use,
 copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the
 Software is furnished to do so, subject to the following
 conditions:
 
 The above copyright notice and this permission notice shall be
 included in all copies of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef adler_roll_h
#define adler_roll_h

#ifdef __cplusplus
extern "C" {
#endif
    
#ifndef adler_data_t
#   define adler_data_t unsigned char
#endif
    
#define _IS_USE_ADLER_FAST_BASE //WARNING: result non-standard, optimize roll_step speed
#define _IS_NEED_ADLER64
#if defined(__LP64__) || defined(_WIN64) || ( __WORDSIZE==64 ) || (UINT_MAX > 0xffffffff )
#   define _IS_DEFAULT_ADLER64
#endif
    
#ifdef _MSC_VER
#   if (_MSC_VER < 1300)
    typedef signed int        int32_t;
    typedef unsigned int      uint32_t;
#   else
    typedef signed __int32    int32_t;
    typedef unsigned __int32  uint32_t;
#   endif
#   ifdef _IS_NEED_ADLER64
    typedef signed   __int64  int64_t;
    typedef unsigned __int64  uint64_t;
#   endif
#   define __adler_inline _inline
#else
#   include <stdint.h> //for int32_t uint32_t int64_t uint64_t
#   define __adler_inline inline
#endif
#include <stdlib.h> //for size_t
    

#ifdef _IS_DEFAULT_ADLER64
#   ifndef _IS_NEED_ADLER64
#       define _IS_NEED_ADLER64
#   endif
#   define adler_uint_t         uint64_t
#   define adler_append         adler64_append
#   define adler_start          adler64_start
#   define adler_roll           adler64_roll
#   define adler_combine        adler64_combine
#else
#   define adler_uint_t         uint32_t
#   define adler_append         adler32_append
#   define adler_start          adler32_start
#   define adler_roll           adler32_roll
#   define adler_combine        adler32_combine
#endif
    
#define ADLER_INITIAL 1 //must 0 or 1
    
#ifdef _IS_USE_ADLER_FAST_BASE
#   define  __private_adler_roll_fast(uint_t,half_bit,\
                                      adler,blockSize,out_data,in_data){ \
        uint_t sum=adler>>half_bit; \
        adler= adler + in_data - out_data; \
        sum  = sum + adler - ADLER_INITIAL - (uint_t)blockSize*out_data;\
        return (adler&(((uint_t)1<<half_bit)-1)) | (sum<<half_bit); \
    }
#endif


uint32_t adler32_append(uint32_t adler,const adler_data_t* pdata,size_t n);

#define  adler32_start(pdata,n) adler32_append(ADLER_INITIAL,pdata,n)
#ifdef _IS_USE_ADLER_FAST_BASE
__adler_inline static
uint32_t adler32_roll(uint32_t adler,size_t blockSize,adler_data_t out_data,adler_data_t in_data)
                __private_adler_roll_fast(uint32_t,16,adler,blockSize,out_data,in_data)
#else
uint32_t adler32_roll(uint32_t adler,size_t blockSize,adler_data_t out_data,adler_data_t in_data);
#endif
    
uint32_t adler32_combine(uint32_t adler_left,uint32_t adler_right,size_t len_right);


#ifdef _IS_NEED_ADLER64
    
uint64_t adler64_append(uint64_t adler,const adler_data_t* pdata,size_t n);

#define  adler64_start(pdata,n) adler64_append(ADLER_INITIAL,pdata,n)
#ifdef _IS_USE_ADLER_FAST_BASE
__adler_inline static
uint64_t adler64_roll(uint64_t adler,uint64_t blockSize,adler_data_t out_data,adler_data_t in_data)
                __private_adler_roll_fast(uint64_t,32,adler,blockSize,out_data,in_data)
#else
uint64_t adler64_roll(uint64_t adler,uint64_t blockSize,adler_data_t out_data,adler_data_t in_data);
#endif
    
uint64_t adler64_combine(uint64_t adler_left,uint64_t adler_right,uint64_t len_right);

#endif //_IS_NEED_ADLER64
    
#ifdef __cplusplus
}
#endif
#endif
