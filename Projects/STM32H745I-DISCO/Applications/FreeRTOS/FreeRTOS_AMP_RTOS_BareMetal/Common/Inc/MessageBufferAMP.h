/*
 * FreeRTOS Kernel V10.0.0
 * Copyright (C) 2017 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software. If you wish to use our Amazon
 * FreeRTOS name, please do so in a fair use way that does not cause confusion.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * http://www.FreeRTOS.org
 * http://aws.amazon.com/freertos
 *
 * 1 tab == 4 spaces!
 */

#ifndef MESSAGE_BUFFER_AMP_H
#define MESSAGE_BUFFER_AMP_H
#include "stdint.h"
#include "FreeRTOS.h"
#include "message_buffer.h"
/* Enough four 4 8 byte strings, plus the additional 4 bytes per message
overhead of message buffers. */
#define mbaTASK_MESSAGE_BUFFER_SIZE ( 60 )

/* A block time of 0 simply means, don't block. */
#define mbaDONT_BLOCK       0
#if defined ( __ICCARM__ )
#pragma location = 0x38000000
MessageBufferHandle_t xCoreMessageBuffer;
/* The variable used to hold the stream buffer structure.*/
#pragma location = 0x38000004
StaticStreamBuffer_t xStreamBufferStruct;
/* Used to dimension the array used to hold the streams. */
/* Defines the memory that will actually hold the streams within the stream buffer.*/
#pragma location = 0x38000040
static uint8_t ucStorageBuffer[ mbaTASK_MESSAGE_BUFFER_SIZE ];
#elif defined ( __CC_ARM )
MessageBufferHandle_t xCoreMessageBuffer __attribute__((at(0x38000000)));
/* The variable used to hold the stream buffer structure.*/
StaticStreamBuffer_t xStreamBufferStruct __attribute__((at(0x38000004)));
/* Used to dimension the array used to hold the streams. */
/* Defines the memory that will actually hold the streams within the stream buffer.*/
static uint8_t ucStorageBuffer[ mbaTASK_MESSAGE_BUFFER_SIZE ]__attribute__((at(0x38000040)));
#elif defined ( __GNUC__ )
MessageBufferHandle_t xCoreMessageBuffer __attribute__((section(".xCoreMessageBuffer_section")));;
/* The variable used to hold the stream buffer structure.*/
StaticStreamBuffer_t xStreamBufferStruct __attribute__((section(".xStreamBufferStruct_section")));;
/* Used to dimension the array used to hold the streams. */
/* Defines the memory that will actually hold the streams within the stream buffer.*/
static uint8_t ucStorageBuffer[ mbaTASK_MESSAGE_BUFFER_SIZE ]__attribute__((section(".ucStorageBuffer_section")));
#endif

#endif /* MESSAGE_BUFFER_AMP_H */
