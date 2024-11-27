/**
  ******************************************************************************
  * @file    FMAC/FMAC_FIR_DMAToIT/Src/main.c
  * @author  MCD Application Team
  * @brief   This sample code shows how to use FMAC HAL API (FMAC instance)
  *          to perform a FIR filter from DMA mode to IT mode.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
#include "stm32h735g_discovery.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
FMAC_HandleTypeDef hfmac;
DMA_HandleTypeDef hdma_fmac_preload;
DMA_HandleTypeDef hdma_fmac_write;

/* FMAC configuration structure */
FMAC_FilterConfigTypeDef sFmacConfig;

/* Array of filter coefficients B (feed-forward taps) in Q1.15 format */
static int16_t aFilterCoeffB[COEFF_VECTOR_B_SIZE] =
{
    2212,  8848, 13272,  8848,  2212
};

/* Array of input values in Q1.15 format (in four parts in order to write new data during the calculation) */
static int16_t aInputValues1[INPUT_ARRAY_1_SIZE] =
{
       0,  5276, -1548, 13844,     7, 17551,  5802, 16142, 14198, 12009,
   21624,  8678, 24576,  8672, 21611, 11990, 14172, 16111,  5765, 17510,
     -37, 13797, -1598,  5225,   -51, -5327,  1498,-13892,   -52,-17592,
   -5838,-16174,-14223,-12029,-21637, -8685,-24576, -8665,-21597,-11970,
  -14146,-16080, -5729,-17469,    82,-13749,  1647, -5174,   103,  5378,
   -1449, 13939,    96, 17632,  5874, 16205, 14249, 12048, 21650,  8691,
   24575,  8658, 21583, 11950, 14120, 16048,  5692, 17428,  -127, 13701,
   -1697,  5122,  -154, -5429,  1399,-13987,  -141,-17673, -5910,-16236,
  -14274,-12068,-21663, -8698,-24575, -8651,-21570,-11930,-14094,-16016,
   -5655,-17387,   171,-13654,  1747, -5071,   206,  5480, -1349, 14034,
};
static int16_t aInputValues2[INPUT_ARRAY_2_SIZE] =
{
     185, 17713,  5946, 16267, 14299, 12087, 21676,  8704, 24574,  8643,
   21556, 11909, 14067, 15984,  5618, 17346,  -216, 13606, -1797,  5020,
    -257, -5530,  1300,-14081,  -229,-17754, -5982,-16297,-14324,-12106,
  -21688, -8710,-24574, -8636,-21542,-11889,-14041,-15952, -5581,-17304,
     261,-13558,  1847, -4969,   309,  5581, -1250, 14128,   273, 17794,
    6018, 16328, 14349, 12124, 21701,  8715, 24573,  8628, 21527, 11868,
   14014, 15920,  5544, 17263,  -306, 13510, -1897,  4918,  -360, -5632,
    1201,-14176,  -317,-17834, -6053,-16358,-14374,-12143,-21713, -8721,
  -24571, -8620,-21513,-11847,-13988,-15888, -5507,-17221,   352,-13462,
    1947, -4867,   412,  5683, -1152, 14223,   361, 17874,  6089, 16389,
   14399, 12162, 21725,  8726, 24570,  8612, 21498, 11826, 13961, 15856,
    5470, 17180,  -397, 13414, -1997,  4816,  -463, -5734,  1102,-14270,
};
static int16_t aInputValues3[INPUT_ARRAY_3_SIZE] =
{
    -405,-17914, -6124,-16419,-14423,-12180,-21737, -8732,-24569, -8604,
  -21484,-11805,-13934,-15823, -5432,-17138,   442,-13366,  2047, -4764,
     515,  5785, -1053, 14317,   449, 17954,  6160, 16449, 14447, 12198,
   21749,  8737, 24567,  8596, 21469, 11784, 13907, 15790,  5395, 17096,
    -488, 13318, -2097,  4713,  -566, -5836,  1004,-14363,  -493,-17994,
   -6195,-16479,-14472,-12216,-21760, -8742,-24565, -8587,-21454,-11763,
  -13879,-15758, -5357,-17054,   533,-13269,  2147, -4662,   618,  5886,
    -955, 14410,   536, 18033,  6230, 16509, 14496, 12234, 21772,  8747,
   24563,  8579, 21438, 11741, 13852, 15725,  5319, 17012,  -579, 13221,
   -2198,  4611,  -669, -5937,   905,-14457,  -580,-18073, -6265,-16538,
  -14520,-12252,-21783, -8751,-24561, -8570,-21423,-11720,-13824,-15692,
};
static int16_t aInputValues4[INPUT_ARRAY_4_SIZE] =
{
   -5282,-16970,   624,-13173,  2248, -4559,   720,  5988,  -856, 14504,
     623, 18112,  6299, 16568, 14543, 12270, 21794,  8756, 24559,  8561,
   21408, 11698, 13797, 15659,  5244, 16928,  -670, 13124, -2298,  4508,
    -772, -6038,   807,-14550,  -667,-18152, -6334,-16597,-14567,-12287,
  -21805, -8760,-24557, -8552,-21392,-11676,-13769,-15625, -5205,-16886,
     716,-13076,  2348, -4457,   823,  6089,  -758, 14597,   710, 18191,
    6369, 16626, 14590, 12304, 21816,  8764, 24554,  8542, 21376, 11654,
   13741, 15592,  5167, 16843,  -761, 13027, -2399,  4405,  -875, -6140,
     709,-14643,  -753,-18230, -6403,-16656,-14614,-12321,-21827, -8768,
  -24551, -8533,-21360,-11632,-13713,-15559, -5129,-16801,   807,-12979,
    2449, -4354,   926,  6190,  -660, 14690,   796, 18269,  6437, 16685,
   14637, 12338, 21837,  8772, 24548,  8523, 21344, 11609, 13684, 15525,
    5090, 16758,  -853, 12930, -2500,  4303,  -977, -6241,   612,-14736,
    -839,-18308, -6472,-16713,-14660,-12355,-21847, -8776,-24545, -8514,
  -21328,-11587,-13656,-15491, -5052,-16715,   899,-12882,  2550, -4251,
    1029,  6291,  -563, 14782,   882, 18347,  6506, 16742, 14683, 12372,
   21858,  8779, 24542,  8504, 21311, 11564, 13628, 15457,  5013, 16673,
    -946, 12833, -2600,  4200, -1080, -6342,   514,-14828,  -925,-18386,
   -6540,-16771
};

/* Array of calculated filtered data in Q1.15 format (two parts) */
static int16_t aCalculatedFilteredData1[OUTPUT_ARRAY_1_SIZE];
static int16_t aCalculatedFilteredData2[OUTPUT_ARRAY_2_SIZE];

/* Expected number of calculated samples for the used aCalculatedFilteredDataX */
uint16_t CurrentInputArraySize;

/* Expected number of calculated samples for the used aCalculatedFilteredDataX */
uint16_t ExpectedCalculatedFilteredDataSize;

/* Status of the calculation */
__IO uint32_t FilterConfigCallbackCount    = 0;
__IO uint32_t FilterPreloadCallbackCount   = 0;
__IO uint32_t HalfGetDataCallbackCount     = 0;
__IO uint32_t GetDataCallbackCount         = 0;
__IO uint32_t OutputDataReadyCallbackCount = 0;
__IO uint32_t ErrorCount                   = 0;

/* Array of reference filtered data for FIR "5 feed-forward taps, gain = 1" in Q1.15 format */
static const int16_t aRefFilteredData[ARRAY_SIZE] =
{
#if defined(CLIP_ENABLED)
  0x2370, 0x3498, 0x447e, 0x539a, 0x60ed, 0x6cdb, 0x76c6, 0x7ea8, 0x7fff, 0x7fff,
  0x7fff, 0x7fff, 0x7fff, 0x7e7e, 0x768f, 0x6c98, 0x609e, 0x5343, 0x441f, 0x3432,
  0x2306, 0x1185, 0xff92, 0xeda0, 0xdc24, 0xcb01, 0xbb21, 0xac0d, 0x9ec4, 0x92e1,
  0x8902, 0x812d, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x81ac, 0x89a7, 0x93aa,
  0x9fae, 0xad14, 0xbc40, 0xcc33, 0xdd63, 0xeee7, 0x00dc, 0x12cc, 0x2444, 0x3563,
  0x453c, 0x5448, 0x6188, 0x6d61, 0x7734, 0x7efb, 0x7fff, 0x7fff, 0x7fff, 0x7fff,
  0x7fff, 0x7e28, 0x7620, 0x6c11, 0x6002, 0x5293, 0x435f, 0x3366, 0x2230, 0x10aa,
  0xfeb5, 0xecc5, 0xdb50, 0xca36, 0xba63, 0xab5f, 0x9e2a, 0x925c, 0x8895, 0x80da,
  0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8201, 0x8a17, 0x9432, 0xa04c, 0xadc4,
  0xbcff, 0xccff, 0xde39, 0xefc3, 0x01b9, 0x13a7, 0x251a, 0x362f, 0x45fb, 0x54f6,
  0x6222, 0x6de5, 0x77a0, 0x7f4e, 0x7fff, 0x7fff, 0x7fff, 0x7fff, 0x7fff, 0x7dd1,
  0x75af, 0x6b88, 0x5f64, 0x51e3, 0x42a0, 0x329a, 0x215b, 0x0fcf, 0xfdd9, 0xebec,
  0xda7c, 0xc96c, 0xb9a6, 0xaab2, 0x9d90, 0x91d9, 0x882a, 0x8089, 0x8000, 0x8000,
  0x8000, 0x8000, 0x8000, 0x8259, 0x8a88, 0x94bb, 0xa0eb, 0xae75, 0xbdc0, 0xcdcc,
  0xdf0f, 0xf09e, 0x0295, 0x1481, 0x25ed, 0x36f8, 0x46b7, 0x55a3, 0x62bc, 0x6e68,
  0x780b, 0x7f9e, 0x7fff, 0x7fff, 0x7fff, 0x7fff, 0x7fff, 0x7d79, 0x753d, 0x6aff,
  0x5ec5, 0x5132, 0x41df, 0x31cc, 0x2085, 0x0ef4, 0xfcfc, 0xeb11, 0xd9a8, 0xc8a1,
  0xb8e9, 0xaa06, 0x9cf8, 0x9156, 0x87bf, 0x8039, 0x8000, 0x8000, 0x8000, 0x8000,
  0x8000, 0x82b2, 0x8afa, 0x9545, 0xa189, 0xaf27, 0xbe82, 0xce9a, 0xdfe5, 0xf179,
  0x0371, 0x155b, 0x26c1, 0x37c2, 0x4774, 0x564f, 0x6354, 0x6eeb, 0x7875, 0x7fef,
  0x7fff, 0x7fff, 0x7fff, 0x7fff, 0x7fff, 0x7d20, 0x74cb, 0x6a75, 0x5e26, 0x5080,
  0x411d, 0x30fe, 0x1faf, 0x0e19, 0xfc20, 0xea37, 0xd8d4, 0xc7d8, 0xb82d, 0xa95b,
  0x9c60, 0x90d4, 0x8756, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x830c,
  0x8b6e, 0x95d0, 0xa229, 0xafd9, 0xbf43, 0xcf67, 0xe0bc, 0xf254, 0x044e, 0x1635,
  0x2794, 0x388c, 0x4830, 0x56fa, 0x63ec, 0x6f6c, 0x78dd, 0x7fff, 0x7fff, 0x7fff,
  0x7fff, 0x7fff, 0x7fff, 0x7cc6, 0x7457, 0x69e9, 0x5d85, 0x4fcc, 0x405a, 0x3030,
  0x1ed8, 0x0d3d, 0xfb43, 0xe95e, 0xd802, 0xc70f, 0xb771, 0xa8b0, 0x9bc8, 0x9053,
  0x86ee, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8367, 0x8be3, 0x965c,
  0xa2ca, 0xb08d, 0xc006, 0xd036, 0xe193, 0xf32f, 0x052a, 0x170e, 0x2866, 0x3953,
  0x48ea, 0x57a4, 0x6482, 0x6fec, 0x7945, 0x7fff, 0x7fff, 0x7fff, 0x7fff, 0x7fff,
  0x7fff, 0x7c6a, 0x73e2, 0x695d, 0x5ce3, 0x4f18, 0x3f97, 0x2f61, 0x1e00, 0x0c62,
  0xfa67, 0xe884, 0xd72e, 0xc646, 0xb6b6, 0xa806, 0x9b33, 0x8fd4, 0x8687, 0x8000,
  0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x83c3, 0x8c58, 0x96e9, 0xa36c, 0xb140,
  0xc0c9, 0xd105, 0xe26a, 0xf40b, 0x0606, 0x17e7, 0x293a, 0x3a1c, 0x49a5, 0x584c,
  0x6516, 0x706a, 0x79ab, 0x7fff, 0x7fff, 0x7fff, 0x7fff, 0x7fff, 0x7fff, 0x7c0e,
  0x736c, 0x68d0, 0x5c42, 0x4e64, 0x3ed4, 0x2e92, 0x1d29, 0x0b86, 0xf98b, 0xe7ab,
  0xd65d, 0xc57f, 0xb5fd, 0xa75d, 0x9a9e, 0x8f56, 0x8622, 0x8000, 0x8000, 0x8000,
  0x8000, 0x8000, 0x8000, 0x8421, 0x8ccf, 0x9777, 0xa410, 0xb1f6, 0xc18d, 0xd1d4,
  0xe341, 0xf4e6, 0x06e2, 0x18c0, 0x2a0c, 0x3ae4, 0x4a5f, 0x58f6, 0x65ab, 0x70e7,
  0x7a0f, 0x7fff, 0x7fff, 0x7fff, 0x7fff, 0x7fff, 0x7fff, 0x7baf, 0x72f4, 0x6840,
  0x5b9e, 0x4daf, 0x3e10, 0x2dc2, 0x1c51, 0x0aa9, 0xf8ae, 0xe6d2, 0xd58a, 0xc4b8,
  0xb544, 0xa6b6, 0x9a0a, 0x8ed8, 0x85bd, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000,
  0x8000, 0x847f, 0x8d47, 0x9806, 0xa4b3, 0xb2ab, 0xc251, 0xd2a4, 0xe419, 0xf5c3,
  0x07bf, 0x1999, 0x2add, 0x3bab, 0x4b17, 0x599c, 0x663e, 0x7164, 0x7a73, 0x7fff,
  0x7fff, 0x7fff, 0x7fff, 0x7fff, 0x7fff, 0x7b4f, 0x727b, 0x67b0, 0x5af9, 0x4cf8,
  0x3d4b, 0x2cf2, 0x1b7a, 0x09cf, 0xf7d3, 0xe5fa, 0xd4ba, 0xc3f2, 0xb48c, 0xa60f,
  0x9978, 0x8e5d, 0x855b, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x84e0,
  0x8dc1, 0x9897, 0xa558, 0xb362, 0xc316, 0xd374, 0xe4f1, 0xf69f, 0x089b, 0x1a71,
  0x2bae, 0x3c70, 0x4bcf, 0x5a43, 0x66d1, 0x71e0, 0x7ad6, 0x7fff, 0x7fff, 0x7fff,
  0x7fff, 0x7fff, 0x7fff, 0x7aee, 0x7201, 0x6720, 0x5a54, 0x4c41, 0x3c85, 0x2c22,
  0x1aa2, 0x08f3, 0xf6f6, 0xe522, 0xd3e9, 0xc32b, 0xb3d4, 0xa568
#else
  0x2370, 0x3498, 0x447e, 0x539a, 0x60ed, 0x6cdb, 0x76c6, 0x7ea8, 0x8483, 0x87d3,
  0x892a, 0x87c5, 0x8467, 0x7e7e, 0x768f, 0x6c98, 0x609e, 0x5343, 0x441f, 0x3432,
  0x2306, 0x1185, 0xff92, 0xeda0, 0xdc24, 0xcb01, 0xbb21, 0xac0d, 0x9ec4, 0x92e1,
  0x8902, 0x812d, 0x7b60, 0x781d, 0x76d5, 0x7848, 0x7bb5, 0x81ac, 0x89a7, 0x93aa,
  0x9fae, 0xad14, 0xbc40, 0xcc33, 0xdd63, 0xeee7, 0x00dc, 0x12cc, 0x2444, 0x3563,
  0x453c, 0x5448, 0x6188, 0x6d61, 0x7734, 0x7efb, 0x84bb, 0x87ef, 0x8929, 0x87a7,
  0x842c, 0x7e28, 0x7620, 0x6c11, 0x6002, 0x5293, 0x435f, 0x3366, 0x2230, 0x10aa,
  0xfeb5, 0xecc5, 0xdb50, 0xca36, 0xba63, 0xab5f, 0x9e2a, 0x925c, 0x8895, 0x80da,
  0x7b28, 0x7802, 0x76d6, 0x7867, 0x7bef, 0x8201, 0x8a17, 0x9432, 0xa04c, 0xadc4,
  0xbcff, 0xccff, 0xde39, 0xefc3, 0x01b9, 0x13a7, 0x251a, 0x362f, 0x45fb, 0x54f6,
  0x6222, 0x6de5, 0x77a0, 0x7f4e, 0x84f2, 0x880a, 0x8927, 0x8788, 0x83f1, 0x7dd1,
  0x75af, 0x6b88, 0x5f64, 0x51e3, 0x42a0, 0x329a, 0x215b, 0x0fcf, 0xfdd9, 0xebec,
  0xda7c, 0xc96c, 0xb9a6, 0xaab2, 0x9d90, 0x91d9, 0x882a, 0x8089, 0x7af2, 0x77e8,
  0x76d9, 0x7886, 0x7c2b, 0x8259, 0x8a88, 0x94bb, 0xa0eb, 0xae75, 0xbdc0, 0xcdcc,
  0xdf0f, 0xf09e, 0x0295, 0x1481, 0x25ed, 0x36f8, 0x46b7, 0x55a3, 0x62bc, 0x6e68,
  0x780b, 0x7f9e, 0x8527, 0x8823, 0x8923, 0x8767, 0x83b4, 0x7d79, 0x753d, 0x6aff,
  0x5ec5, 0x5132, 0x41df, 0x31cc, 0x2085, 0x0ef4, 0xfcfc, 0xeb11, 0xd9a8, 0xc8a1,
  0xb8e9, 0xaa06, 0x9cf8, 0x9156, 0x87bf, 0x8039, 0x7abd, 0x77d1, 0x76df, 0x78a9,
  0x7c6a, 0x82b2, 0x8afa, 0x9545, 0xa189, 0xaf27, 0xbe82, 0xce9a, 0xdfe5, 0xf179,
  0x0371, 0x155b, 0x26c1, 0x37c2, 0x4774, 0x564f, 0x6354, 0x6eeb, 0x7875, 0x7fef,
  0x855c, 0x883a, 0x891d, 0x8745, 0x8375, 0x7d20, 0x74cb, 0x6a75, 0x5e26, 0x5080,
  0x411d, 0x30fe, 0x1faf, 0x0e19, 0xfc20, 0xea37, 0xd8d4, 0xc7d8, 0xb82d, 0xa95b,
  0x9c60, 0x90d4, 0x8756, 0x7fea, 0x7a8a, 0x77b9, 0x76e4, 0x78cb, 0x7ca8, 0x830c,
  0x8b6e, 0x95d0, 0xa229, 0xafd9, 0xbf43, 0xcf67, 0xe0bc, 0xf254, 0x044e, 0x1635,
  0x2794, 0x388c, 0x4830, 0x56fa, 0x63ec, 0x6f6c, 0x78dd, 0x803c, 0x858e, 0x8851,
  0x8918, 0x8723, 0x8337, 0x7cc6, 0x7457, 0x69e9, 0x5d85, 0x4fcc, 0x405a, 0x3030,
  0x1ed8, 0x0d3d, 0xfb43, 0xe95e, 0xd802, 0xc70f, 0xb771, 0xa8b0, 0x9bc8, 0x9053,
  0x86ee, 0x7f9c, 0x7a58, 0x77a4, 0x76ec, 0x78ef, 0x7ce8, 0x8367, 0x8be3, 0x965c,
  0xa2ca, 0xb08d, 0xc006, 0xd036, 0xe193, 0xf32f, 0x052a, 0x170e, 0x2866, 0x3953,
  0x48ea, 0x57a4, 0x6482, 0x6fec, 0x7945, 0x808a, 0x85c0, 0x8866, 0x890f, 0x86fd,
  0x82f5, 0x7c6a, 0x73e2, 0x695d, 0x5ce3, 0x4f18, 0x3f97, 0x2f61, 0x1e00, 0x0c62,
  0xfa67, 0xe884, 0xd72e, 0xc646, 0xb6b6, 0xa806, 0x9b33, 0x8fd4, 0x8687, 0x7f4f,
  0x7a27, 0x7790, 0x76f4, 0x7914, 0x7d2a, 0x83c3, 0x8c58, 0x96e9, 0xa36c, 0xb140,
  0xc0c9, 0xd105, 0xe26a, 0xf40b, 0x0606, 0x17e7, 0x293a, 0x3a1c, 0x49a5, 0x584c,
  0x6516, 0x706a, 0x79ab, 0x80d5, 0x85f0, 0x8879, 0x8906, 0x86d8, 0x82b4, 0x7c0e,
  0x736c, 0x68d0, 0x5c42, 0x4e64, 0x3ed4, 0x2e92, 0x1d29, 0x0b86, 0xf98b, 0xe7ab,
  0xd65d, 0xc57f, 0xb5fd, 0xa75d, 0x9a9e, 0x8f56, 0x8622, 0x7f05, 0x79f8, 0x777d,
  0x76fe, 0x793b, 0x7d6c, 0x8421, 0x8ccf, 0x9777, 0xa410, 0xb1f6, 0xc18d, 0xd1d4,
  0xe341, 0xf4e6, 0x06e2, 0x18c0, 0x2a0c, 0x3ae4, 0x4a5f, 0x58f6, 0x65ab, 0x70e7,
  0x7a0f, 0x811f, 0x861e, 0x888b, 0x88fb, 0x86b0, 0x8270, 0x7baf, 0x72f4, 0x6840,
  0x5b9e, 0x4daf, 0x3e10, 0x2dc2, 0x1c51, 0x0aa9, 0xf8ae, 0xe6d2, 0xd58a, 0xc4b8,
  0xb544, 0xa6b6, 0x9a0a, 0x8ed8, 0x85bd, 0x7ebb, 0x79ca, 0x776b, 0x770a, 0x7963,
  0x7db1, 0x847f, 0x8d47, 0x9806, 0xa4b3, 0xb2ab, 0xc251, 0xd2a4, 0xe419, 0xf5c3,
  0x07bf, 0x1999, 0x2add, 0x3bab, 0x4b17, 0x599c, 0x663e, 0x7164, 0x7a73, 0x8168,
  0x864b, 0x889b, 0x88ef, 0x8687, 0x822b, 0x7b4f, 0x727b, 0x67b0, 0x5af9, 0x4cf8,
  0x3d4b, 0x2cf2, 0x1b7a, 0x09cf, 0xf7d3, 0xe5fa, 0xd4ba, 0xc3f2, 0xb48c, 0xa60f,
  0x9978, 0x8e5d, 0x855b, 0x7e73, 0x799d, 0x775b, 0x7716, 0x798c, 0x7df6, 0x84e0,
  0x8dc1, 0x9897, 0xa558, 0xb362, 0xc316, 0xd374, 0xe4f1, 0xf69f, 0x089b, 0x1a71,
  0x2bae, 0x3c70, 0x4bcf, 0x5a43, 0x66d1, 0x71e0, 0x7ad6, 0x81b1, 0x8678, 0x88ab,
  0x88e2, 0x865d, 0x81e5, 0x7aee, 0x7201, 0x6720, 0x5a54, 0x4c41, 0x3c85, 0x2c22,
  0x1aa2, 0x08f3, 0xf6f6, 0xe522, 0xd3e9, 0xc32b, 0xb3d4, 0xa568
#endif /* CLIP_ENABLED */
};

/* Auxiliary counter */
uint32_t Index;


/* Private function prototypes -----------------------------------------------*/
static void MPU_Config(void);
static void SystemClock_Config(void);
static void MX_DMA_Init(void);
static void MX_FMAC_Init(void);
static void CPU_CACHE_Enable(void);
/* Private user code ---------------------------------------------------------*/

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* Configure the MPU attributes */
  MPU_Config();

  /* Enable the CPU Cache */
  CPU_CACHE_Enable();

   /* STM32H7xx HAL library initialization:
       - Systick timer is configured by default as source of time base, but user
         can eventually implement his proper time base source (a general purpose
         timer for example or other time source), keeping in mind that Time base
         duration should be kept 1ms since PPP_TIMEOUT_VALUEs are defined and
         handled in milliseconds basis.
       - Set NVIC Group Priority to 4
       - Low Level Initialization
     */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();


  /* Configure the system clock */
  SystemClock_Config();

  /* Configure LED1 and LED2 */
  BSP_LED_Init(LED1);
  BSP_LED_Init(LED2);

  /* Initialize all configured peripherals */
  MX_DMA_Init();
  MX_FMAC_Init();

  /*## Configure the FMAC peripheral ###########################################*/
  sFmacConfig.InputBaseAddress  = INPUT_BUFFER_BASE;
  sFmacConfig.InputBufferSize   = INPUT_BUFFER_SIZE;
  sFmacConfig.InputThreshold    = INPUT_THRESHOLD;
  sFmacConfig.CoeffBaseAddress  = COEFFICIENT_BUFFER_BASE;
  sFmacConfig.CoeffBufferSize   = COEFFICIENT_BUFFER_SIZE;
  sFmacConfig.OutputBaseAddress = OUTPUT_BUFFER_BASE;
  sFmacConfig.OutputBufferSize  = OUTPUT_BUFFER_SIZE;
  sFmacConfig.OutputThreshold   = OUTPUT_THRESHOLD;
  sFmacConfig.pCoeffA           = NULL;
  sFmacConfig.CoeffASize        = 0;
  sFmacConfig.pCoeffB           = aFilterCoeffB;
  sFmacConfig.CoeffBSize        = COEFF_VECTOR_B_SIZE;
  sFmacConfig.Filter            = FMAC_FUNC_CONVO_FIR;
  sFmacConfig.InputAccess       = FMAC_BUFFER_ACCESS_DMA;
  sFmacConfig.OutputAccess      = FMAC_BUFFER_ACCESS_IT;
#if defined(CLIP_ENABLED)
  sFmacConfig.Clip              = FMAC_CLIP_ENABLED;
#else
  sFmacConfig.Clip              = FMAC_CLIP_DISABLED;
#endif
  sFmacConfig.P                 = COEFF_VECTOR_B_SIZE;
  sFmacConfig.Q                 = FILTER_PARAM_Q_NOT_USED;
  sFmacConfig.R                 = GAIN;

  if (HAL_FMAC_FilterConfig_DMA(&hfmac, &sFmacConfig) != HAL_OK)
  {
    /* Configuration Error */
    Error_Handler();
  }

  while(FilterConfigCallbackCount == 0);

  /*## Preload the input and output buffers ####################################*/
  if (HAL_FMAC_FilterPreload_DMA(&hfmac, aInputValues1, INPUT_ARRAY_1_SIZE, NULL, 0) != HAL_OK)
  {
    /* Configuration Error */
    Error_Handler();
  }

  while(FilterPreloadCallbackCount == 0);

  /*  Before starting a new process, you need to check the current state of the peripheral;
      if it's busy you need to wait for the end of current transfer before starting the calculation.
      For simplicity reasons, this example is just waiting till the end of the
      process, but the application may perform other tasks while the transfer is ongoing. */
  while (HAL_FMAC_GetState(&hfmac) != HAL_FMAC_STATE_READY)
  {
  }

  /*## Start calculation of FIR filter in DMA/IT mode ##########################*/
  ExpectedCalculatedFilteredDataSize = OUTPUT_ARRAY_1_SIZE;
  if (HAL_FMAC_FilterStart(&hfmac, aCalculatedFilteredData1, &ExpectedCalculatedFilteredDataSize) != HAL_OK)
  {
    /* Processing Error */
    Error_Handler();
  }

  /*## Append data to start the DMA process after the preloaded data handling ##*/
  CurrentInputArraySize = INPUT_ARRAY_2_SIZE;
  if (HAL_FMAC_AppendFilterData(&hfmac,
                                aInputValues2,
                                &CurrentInputArraySize) != HAL_OK)
  {
    ErrorCount++;
  }

  /*## Wait for the end of the handling (no new data written) ##################*/
  /*  For simplicity reasons, this example is just waiting till the end of the
      calculation, but the application may perform other tasks while the operation
      is ongoing. */
  while((HalfGetDataCallbackCount < GET_DATA_CALLBACK_COUNT) ||
        (GetDataCallbackCount < GET_DATA_CALLBACK_COUNT) ||
        (OutputDataReadyCallbackCount < DATA_RDY_CALLBACK_COUNT))
  {
    if(ErrorCount != 0)
    {
      /* Processing Error */
      Error_Handler();
    }
  }

  /*## Stop the calculation of FIR filter in polling/DMA mode ##################*/
  if (HAL_FMAC_FilterStop(&hfmac) != HAL_OK)
  {
    /* Processing Error */
    Error_Handler();
  }

  /*## Check the final error status ############################################*/
  if(ErrorCount != 0)
  {
    /* Processing Error */
    Error_Handler();
  }

  /*## Compare FMAC results to the reference values ############################*/
  for (Index = 0; Index < OUTPUT_ARRAY_1_SIZE; Index++)
  {
    if (aCalculatedFilteredData1[Index]  != aRefFilteredData[Index])
    {
      /* Processing Error */
      Error_Handler();
    }
  }
  for (Index = 0; Index < OUTPUT_ARRAY_2_SIZE; Index++)
  {
    if (aCalculatedFilteredData2[Index]  != aRefFilteredData[OUTPUT_ARRAY_1_SIZE + Index])
    {
      /* Processing Error */
      Error_Handler();
    }
  }

  /* There is no error in the output values: Turn LED1 on */
  BSP_LED_On(LED1);


  /* Infinite loop */
  while (1)
  {
  }
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow :
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 520000000 (CPU Clock)
  *            HCLK(Hz)                       = 260000000 (AXI and AHBs Clock)
  *            AHB Prescaler                  = 2
  *            D1 APB3 Prescaler              = 2 (APB3 Clock  130MHz)
  *            D2 APB1 Prescaler              = 2 (APB1 Clock  130MHz)
  *            D2 APB2 Prescaler              = 2 (APB2 Clock  130MHz)
  *            D3 APB4 Prescaler              = 2 (APB4 Clock  130MHz)
  *            HSE Frequency(Hz)              = 25000000
  *            PLL_M                          = 5
  *            PLL_N                          = 104
  *            PLL_P                          = 1
  *            PLL_Q                          = 4
  *            PLL_R                          = 2
  *            VDD(V)                         = 3.3
  *            Flash Latency(WS)              = 3
  * @param  None
  * @retval None
  */
static void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  HAL_StatusTypeDef ret = HAL_OK;

  /* The voltage scaling allows optimizing the power consumption when the device is
     clocked below the maximum system frequency, to update the voltage scaling value
     regarding system frequency refer to product datasheet.  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE0);

  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}

  /* Enable HSE Oscillator and activate PLL with HSE as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSIState = RCC_HSI_OFF;
  RCC_OscInitStruct.CSIState = RCC_CSI_OFF;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;

  RCC_OscInitStruct.PLL.PLLM = 5;
  RCC_OscInitStruct.PLL.PLLN = 104;
  RCC_OscInitStruct.PLL.PLLFRACN = 0;
  RCC_OscInitStruct.PLL.PLLP = 1;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLQ = 4;

  RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_2;
  ret = HAL_RCC_OscConfig(&RCC_OscInitStruct);
  if(ret != HAL_OK)
  {
    while(1) {};
  }

/* Select PLL as system clock source and configure  bus clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_D1PCLK1 | RCC_CLOCKTYPE_PCLK1 | \
                                 RCC_CLOCKTYPE_PCLK2  | RCC_CLOCKTYPE_D3PCLK1);

  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;
  RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;
  ret = HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3);
  if(ret != HAL_OK)
  {
    while(1) {};
  }

}

/**
  * @brief FMAC Initialization Function
  * @param None
  * @retval None
  */
static void MX_FMAC_Init(void)
{

  hfmac.Instance = FMAC;
  if (HAL_FMAC_Init(&hfmac) != HAL_OK)
  {
    Error_Handler();
  }

}

/** 
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void) 
{
  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Stream0_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream0_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream0_IRQn);
  /* DMA1_Stream1_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream1_IRQn);

}
 

/**
  * @brief FMAC filter configuration callback
  * @par hfmac: FMAC HAL handle
  * @retval None
  */
void HAL_FMAC_FilterConfigCallback(FMAC_HandleTypeDef *hfmac)
{
  FilterConfigCallbackCount++;;
}

/**
  * @brief FMAC filter preload callback
  * @par hfmac: FMAC HAL handle
  * @retval None
  */
void HAL_FMAC_FilterPreloadCallback(FMAC_HandleTypeDef *hfmac)
{
  FilterPreloadCallbackCount++;;
}

/**
  * @brief FMAC half get data callback
  * @par hfmac: FMAC HAL handle
  * @retval None
  */
void HAL_FMAC_HalfGetDataCallback(FMAC_HandleTypeDef *hfmac)
{
  HalfGetDataCallbackCount++;
}

/**
  * @brief FMAC get data callback
  * @par hfmac: FMAC HAL handle
  * @retval None
  */
void HAL_FMAC_GetDataCallback(FMAC_HandleTypeDef *hfmac)
{
  GetDataCallbackCount++;

  if (GetDataCallbackCount == 1)
  {
    /* The preloaded data (1) and the appended data (2) have been handled,
       write the following input values into FMAC (3 over 4) */
    CurrentInputArraySize = INPUT_ARRAY_3_SIZE;
    if (HAL_FMAC_AppendFilterData(hfmac,
                                  aInputValues3,
                                  &CurrentInputArraySize) != HAL_OK)
    {
      ErrorCount++;
    }
  }
  else if (GetDataCallbackCount == 2)
  {
    /* Write the following input values into FMAC (4 over 4) */
    CurrentInputArraySize = INPUT_ARRAY_4_SIZE;
    if (HAL_FMAC_AppendFilterData(hfmac,
                                  aInputValues4,
                                  &CurrentInputArraySize) != HAL_OK)
    {
      ErrorCount++;
    }
  }
  else
  {
    /* No more data to write */
  }
}

/**
  * @brief FMAC output data ready callback
  * @par hfmac: FMAC HAL handle
  * @retval None
  */
void HAL_FMAC_OutputDataReadyCallback(FMAC_HandleTypeDef *hfmac)
{
  OutputDataReadyCallbackCount++;

  if (OutputDataReadyCallbackCount == 1)
  {
    ExpectedCalculatedFilteredDataSize = OUTPUT_ARRAY_2_SIZE;
    if (HAL_FMAC_ConfigFilterOutputBuffer(hfmac,
                                          aCalculatedFilteredData2,
                                          &ExpectedCalculatedFilteredDataSize) != HAL_OK)
    {
      ErrorCount++;
    }
  }
  else
  {
    /* No more data will be read, disable the FMAC read interrupt */
    __HAL_FMAC_DISABLE_IT(hfmac, FMAC_IT_RIEN);
  }
}

/**
  * @brief FMAC error callback
  * @par hfmac: FMAC HAL handle
  * @retval None
  */
void HAL_FMAC_ErrorCallback(FMAC_HandleTypeDef *hfmac)
{
  ErrorCount++;
}

/**
  * @brief  CPU L1-Cache enable.
  * @param  None
  * @retval None
  */
static void CPU_CACHE_Enable(void)
{
  /* Enable I-Cache */
  SCB_EnableICache();

  /* Enable D-Cache */
  SCB_EnableDCache();
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* User can add his own implementation to report the HAL error return state */
  while(1)
  {
    /* LED2 is blinking */
    BSP_LED_Toggle(LED2);
    HAL_Delay(500);
  }
}


/**
  * @brief  Configure the MPU attributes
  * @param  None
  * @retval None
  */
static void MPU_Config(void)
{
  MPU_Region_InitTypeDef MPU_InitStruct;

  /* Disable the MPU */
  HAL_MPU_Disable();

  /* Configure the MPU as Strongly ordered for not defined regions */
  MPU_InitStruct.Enable = MPU_REGION_ENABLE;
  MPU_InitStruct.BaseAddress = 0x00;
  MPU_InitStruct.Size = MPU_REGION_SIZE_4GB;
  MPU_InitStruct.AccessPermission = MPU_REGION_NO_ACCESS;
  MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;
  MPU_InitStruct.IsCacheable = MPU_ACCESS_NOT_CACHEABLE;
  MPU_InitStruct.IsShareable = MPU_ACCESS_SHAREABLE;
  MPU_InitStruct.Number = MPU_REGION_NUMBER0;
  MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
  MPU_InitStruct.SubRegionDisable = 0x87;
  MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_DISABLE;

  HAL_MPU_ConfigRegion(&MPU_InitStruct);

  /* Enable the MPU */
  HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
}
#endif /* USE_FULL_ASSERT */

