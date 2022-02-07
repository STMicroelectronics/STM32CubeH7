/**
******************************************************************************
* @file    audio_player/Src/songutilities.c
* @author  MCD Application Team
* @brief   This file includes mp3/wma tag extraction routines
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
#include "songutilities.h"

/** @addtogroup PLAYER_CODERS
* @{
*/

/** @defgroup SongUtilities
* @brief tag extraction modules
* @{
*/

/** @defgroup SongUtilities_Private_TypesDefinitions
* @{
*/
/* WMA typedefs --------------------------------------------------------------*/
/* ASF_CONTENT_DESCRIPTION_OBJEC */
typedef enum  {
  UnicodeString = 0,
  ByteArray,
  BOOL_,
  Dword,
  Qword,
  Word
} ValueDataType_TypeDef;

typedef struct
{
  uint16_t D_NameLength;
  uint8_t  *D_Name; /* variable size */
  uint16_t D_ValueDataType;
  uint16_t D_ValueLength;
  uint8_t * D_value; /* variable size */
}
ContentDescriptor_TypeDef;

#pragma pack(1)
typedef struct
{
  uint16_t CDC;
  ContentDescriptor_TypeDef CDescriptor;
}
ExtContentDescriptor_TypeDef;
#pragma pack()

/* Content Descriptor TypeDef */
typedef struct
{
  uint16_t TitleLength;
  uint16_t AuthorLength;
  uint16_t CprLength;
  uint16_t DescLength;
  uint16_t RatingLength;

}
ASFContentD_H;

typedef struct
{
  ASFContentD_H Header;
  uint8_t  *Title;       /* variable size */
  uint8_t  *Author;      /* variable size */
  uint8_t  *Copyright;   /* variable size */
  uint8_t  *Description; /* variable size */
  uint8_t  *Rating;      /* variable size */
}
ASFContentD_TypeDef;


typedef struct
{
  uint8_t objectGUID[16];   /* 128 bits */
  uint8_t objectSize[8];
}
t_ASFObjectHeader;

/* ASF Header TypeDef */
typedef struct
{
  t_ASFObjectHeader objH;
  uint8_t NumberOfHeaderObjs[4];
  uint8_t Reserved1;
  uint8_t Reserved2;
}
t_ASFHeader;


typedef struct
{
  uint32_t FirstObjOffset;
  uint32_t NextObjOffset;
}
t_ParserObj;

/* MP3 typedefs --------------------------------------------------------------*/

/* The TAG text encodings, keeps convention used in ID3v2 frames. */
typedef enum
{
  TEXT_ENCODING_ISO_8859_1 = 0x00,  /* ISO-8859-1, terminated with 0x00. */
  TEXT_ENCODING_UTF_16     = 0x01,  /* UTF-16 encoded Unicode with BOM, terminated with 0x00. */
  TEXT_ENCODING_UTF_16BE   = 0x02,  /* UTF-16BE encoded Unicode without BOM, terminated with 0x00. */
  TEXT_ENCODING_UTF_8      = 0x03,  /* UTF-8 encoded Unicode, terminated with 0x00. */
} t_TextEncoding;

/* Used in types */
#define XSONG_INFO_TAGS     (0x0F) /* worst case: 8 v2.4 tags + 7 v1.1 tags */
#define MAX_STRINGS_SIZE    (644)

typedef int32_t GRESULT;

typedef enum
{
  b_FALSE = 0,
  b_TRUE = 1
} t_bool;

typedef enum
{
  SONG_TITLE = 0x01,  /* song title (ID3, WMA) */
  ALBUM_TITLE, /* artist (ID3, WMA ) */
  ARTIST,      /* album title (ID3 ) */
  GENRE,       /* genre (ID3) */
  BIT_RATE,    /* bit rate */
  YEAR,
  COMMENT,
  TRACK_NO,    /* only for ID3 */
  CONTENT,
  COMPOSER
} t_SongTagType;

typedef struct
{
  int16_t          string_offset;  /* offset to pascal string */
  t_SongTagType    info_type;      /* type of song tag info */
  t_TextEncoding   enc_of_src;     /* encoding of the original tag string, stored tag string is UTF-8. */
  uint8_t          tag_version;
}
t_InfoTag;

typedef struct SongInfo
{
  FIL * file;
  t_InfoTag infos[XSONG_INFO_TAGS];
  uint8_t strings[MAX_STRINGS_SIZE];
  uint32_t ReadFileError;
  uint32_t start_offset;
  int16_t strtop;
  uint8_t n_infos;
}
t_SongInfos;

typedef enum
{
  ID3_V2_OLD_STYLE,
  ID3_V2_NEW_STYLE
} FRAME_NAMES_STYLE;

typedef enum
{
  ID3_NOT_PRESENT = 0,
  ID3_V1 = 1,
  ID3_V22 = 2,
  ID3_V23 = 4,
  ID3_V24 = 8
} ID3_VERSION;

typedef enum
{
  ID32_FIND_FRAME,
  ID32_READ_DATA,
  ID32_FRAME_SKIP,
  ID32_TERMINATE
} ID32_PROCESSING_ENUM;

/**
* @}
*/

/** @defgroup SongUtilities_Private_Defines
* @{
*/
/* ERROR CODES / FUNCTION RESULTS */
#define E_FAIL                                                  ((int8_t)-1)
#define S_OK                                                    ((int8_t)0)               /* success */
#define S_TRUE                                                  ((int8_t)1)
#define S_FALSE                                                 ((int8_t)0)

#define ID3V2_MAX_TEXT_SIZE                                     ((int32_t)100)

#define BUFFER_SIZE                                             ((uint32_t)0x100)
#define ID3_BUFFER_LENGTH                                       BUFFER_SIZE
#define ID3_TAG_V2                                              (0x494433) /* "ID3" representation in hex */
#define ID3_TAG_V2_VERSION_POSITION                             (0x03)
#define ID3_TAG_V2_FLAGS_POSITION                               (0x05)
#define ID3_TAG_V2_LENGTH_POSITION                              (0x06)
#define ID3_TAG_V2_HEADER_LENGTH                                (0x0A)  /* ID3 Tag Header length */

/* id3v2.2 */
#define TXT_TAL							(0x54414C)    /* Album/Movie/Show title */
#define TXT_TT2							(0x545432)    /* Title/Song name/Content description */
#define TXT_TP1							(0x545031)    /* Lead artist(s)/Lead performer(s)/Soloist(s)/Performing group */
#define TXT_COM							(0x434F4D)    /* Comments */
#define TXT_TYE							(0x545945)    /* Year */
#define TXT_TCO							(0x54434F)    /* Content type */
#define TXT_TRK							(0x54524B)    /* Track number*/
#define TXT_PIC                                                 (0x504943)    /* Picture */
/* id3v2.3 or v2.4 */
#define TXT_TALB      (0x54414C42)    /* Album name */
#define TXT_TIT2      (0x54495432)    /* Song title */
#define TXT_TPE1      (0x54504531)    /* Artist name */
#define TXT_COMM      (0x434F4D4D)    /* Comments */
#define TXT_TYER      (0x54594552)    /* Year */
#define TXT_TCON      (0x54434F4E)    /* Content type */
#define TXT_APIC      (0x41504943)    /* Picture */
#define TXT_TRCK      (0x5452434B)    /* Track number */

/**
* @}
*/

/** @defgroup SongUtilities_Private_Macros
* @{
*/
/* WMA macro -----------------------------------------------------------------*/
#define FieldLenght                                        ((uint32_t)3 * sizeof(uint16_t))
#define DescriptorNameLength(pDescriptor)                  (*(uint16_t *)pDescriptor)
#define DescriptorName(pDescriptor)                        ((uint8_t *) ((uint8_t *)pDescriptor + sizeof(uint16_t)))
#define DescriptorLenght(pDescriptor,NameLength)           (*(uint16_t *)((uint8_t *)pDescriptor + ((NameLength + 2) * sizeof(uint8_t)) + sizeof(uint16_t) ))
#define DescriptorValue(pDescriptor,NameLength)            ((uint8_t *)pDescriptor + (NameLength ) + FieldLenght)
#define NextDescriptor(pDescriptor,NameLength,ValueLength) ((uint8_t *) ((uint8_t *)pDescriptor + ((NameLength ) + ValueLength ) + FieldLenght))
#define toupper(x)                                              ((x>='a'&& x<='z')? (x - 'a' + 'A'):x)
#define wmax(x, y)                                         (((uint32_t)(x) > (uint32_t)(y)) ? (x) : (y))
#define wmin(x,y)                                          (((uint32_t)(x) < (uint32_t)(y))? (x):(y))

/* MP3 macro -----------------------------------------------------------------*/
#define GetLength(pBuf,Offset)                             ((uint32_t)(pBuf[Offset + 4] << 24) |\
(pBuf[Offset + 5] << 16) |\
  (pBuf[Offset + 6] << 8)  |\
    (pBuf[Offset + 7]))

/**
* @}
*/

/** @defgroup SongUtilities_Private_Variables
* @{
*/
const uint8_t ASF_CONTENT_DESCRIPTION_OBJEC[] =
{
  0x33, 0x26, 0xb2, 0x75,
  0x8e, 0x66, 0xcf, 0x11,
  0xa6, 0xd9, 0x00, 0xaa,
  0x00, 0x62, 0xce, 0x6c
};

const uint8_t ASF_EXTENDED_CONTENT_DESCRIPTION_OBJECT[] =
{
  0x40, 0xA4, 0xD0, 0xD2,
  0x07, 0xe3, 0xd2, 0x11,
  0x97, 0xf0, 0x00, 0xa0,
  0xc9, 0x5e, 0xa8, 0x50
};

static uint32_t nbr = 0;
t_SongInfos  tmpInfo;

/**
* @}
*/

/** @defgroup SongUtilities_Private_FunctionPrototypes
* @{
*/
#ifdef __MP3_DECODER__
void SongUtilities_FillTags_struct(t_SongInfos *song_info, TAGS_TypeDef * pIDTAGS);
int32_t SongUtilities_ReadFile(t_SongInfos * song_info, uint8_t *buffer, uint32_t nb_bytes);
int32_t SongUtilities_SeekFile(t_SongInfos * song_info, int32_t position, uint8_t SeekMode);
int SongUtilities_write_utf8(t_SongInfos *pSongInfos, uint16_t w);
int SongUtilities_write_byte(t_SongInfos *pSongInfos, uint8_t b);
void StrCtrlLength(void* Dst, void* Src, uint32_t MaxLen);
#endif /* __MP3_DECODER__ */
uint8_t SongUtilities_IsGUID_eq(int32_t *_GUID1, int32_t* _GUID2);

#ifdef __WMA_DECODER__
void SongUtilities_WMAstrncpy(uint8_t *pDest, const uint8_t *pBuf, uint32_t length);
int8_t SongUtilities_WMAstrcmp(const uint8_t * string1, const uint8_t * string2);
void SongUtilities_parseContentDescriptor(uint8_t * pBuf, TAGS_TypeDef *pTAGS);
void SongUtilities_parseExtendedContentDescriptors(uint8_t *pBuf, TAGS_TypeDef *pTAGS );
#endif /* __WMA_DECODER__ */

/**
* @}
*/

/** @defgroup SongUtilities_Private_Functions
* @{
*/

/**
* @brief  Compare two GUID
* @param  _GUIDx : pointer to GUID.
* @retval 1: if equal 0: if not equal.
*/
uint8_t SongUtilities_IsGUID_eq(int32_t *_GUID1, int32_t* _GUID2)
{
  if (!(_GUID1[0] - _GUID2[0]) && !(_GUID1[1] - _GUID2[1]) && !(_GUID1[2] - _GUID2[2]) && !(_GUID1[3] - _GUID2[3]))
    return 1;
  return 0;
}

#ifdef __WMA_DECODER__
/**
* @brief  Copy n chars from header
* @param  pDest : destination buffer.
*         pBuf :  source buffer.
*         length: number of chars.
* @retval 1: if equal 0: if not equal.
*/
void SongUtilities_WMAstrncpy(uint8_t *pDest, const uint8_t *pBuf, uint32_t length)
{
  __IO uint32_t counter1 = 0, counter2 = 0;
  uint8_t tmp;
  for (counter2 = 0, counter1 = 0;counter1 <= length ;counter1 += 2, counter2++)
  {
    tmp = pBuf[counter1];
    pDest[counter2] = tmp;
  }
}

/**
* @brief  case unsensive string comparison.
* @param  string1: First input string.
* @param  string2: Second input string.
* @retval 0: if equal !0: if not equal.
*/
int8_t SongUtilities_WMAstrcmp(const uint8_t * string1, const uint8_t * string2)
{
  __IO int32_t index = 0;
  uint8_t tmpChar = 0x00;
  while ( (string1[index] != 0) && (string2[index] != 0))
  {
    tmpChar = toupper(string2[index]);
    if ((toupper(string1[index]) - tmpChar) != 0)
    {
      return string1[index] - tmpChar;
    }
    index++;
  }
  if (string1[index] != 0)
    return string1[index];
  else
    if (string2[index] != 0)
      return string2[index];
    else
      return 0;
}

/**
* @brief  extract title and author from content descriptor object.
* @param  pBuf: data buffer.
* @param  pTAGS: output struct.
* @retval None.
*/
void SongUtilities_parseContentDescriptor(uint8_t * pBuf, TAGS_TypeDef *pTAGS)
{
  ASFContentD_TypeDef * tmp = (ASFContentD_TypeDef*)pBuf;
  uint8_t * pointer = NULL;
  pointer = pBuf;
  pointer += sizeof(ASFContentD_H);

#ifdef USE_DYNAMIC_MEMORY /* use dynamic memory allocation */
  /* extract title */
  if (tmp->Header.TitleLength)
  {
    if (pTAGS->Title != NULL)
    {
      MEM_FREE(pTAGS->Title);
    }
    pTAGS->Title = (char *)MEM_MALLOC(tmp->Header.TitleLength);
    SongUtilities_WMAstrncpy((uint8_t*)pTAGS->Title, (uint8_t *) pointer , tmp->Header.TitleLength << 1);
    pointer += tmp->Header.TitleLength;
  }
  /* extract author */
  if (tmp->Header.AuthorLength)
  {
    if (pTAGS->Artist != NULL)
    {
      MEM_FREE(pTAGS->Artist);
    }
    pTAGS->Artist = (char *)MEM_MALLOC(tmp->Header.AuthorLength);
    SongUtilities_WMAstrncpy((uint8_t*)pTAGS->Artist, (uint8_t *) pointer , tmp->Header.AuthorLength << 1);
    pointer += tmp->Header.AuthorLength;
  }

#else /* Not USE_DYNAMIC_MEMORY */
  /* extract title */
  if (tmp->Header.TitleLength)
  {
    SongUtilities_WMAstrncpy((uint8_t*)pTAGS->Title, (uint8_t *) pointer , tmp->Header.TitleLength << 1);
    pointer += tmp->Header.TitleLength;
  }
  /* extract author */
  if (tmp->Header.AuthorLength)
  {
    SongUtilities_WMAstrncpy((uint8_t*)pTAGS->Artist, (uint8_t *) pointer , tmp->Header.AuthorLength << 1);
    pointer += tmp->Header.AuthorLength;
  }
#endif /* USE_DYNAMIC_MEMORY */

  /* can be extracted too */
  if (tmp->Header.CprLength)
  {
    pointer += tmp->Header.CprLength;
  }
  /* can be extracted too */
  if (tmp->Header.DescLength)
  {
    pointer += tmp->Header.DescLength;
  }
  /* can be extracted too */
  if (tmp->Header.RatingLength)
  {
    pointer += tmp->Header.RatingLength;
  }
}

/**
* @brief  extract song description from extended content descriptor object.
* @param  pBuf: data buffer.
* @param  pTAGS: output struct.
* @retval None.
*/
void SongUtilities_parseExtendedContentDescriptors(uint8_t *pBuf, TAGS_TypeDef *pTAGS )
{
  __IO uint32_t nb_descriptors;
  static uint8_t tmpBuf[256];
  uint16_t NameLength = 0, ValueLength = 0;

  uint8_t * pointer;

  /* retrieve number of descriptors */
  nb_descriptors = *(uint16_t *)pBuf;
  pointer = (uint8_t *) ((uint8_t *)pBuf + sizeof(uint16_t));

  while (nb_descriptors--)
  {
    /* retrieve Name field Length */
    NameLength = DescriptorNameLength(pointer);
    if (NameLength > 255)
      break;
    SongUtilities_WMAstrncpy(tmpBuf, (uint8_t *)DescriptorName(pointer), NameLength);
    /* retrieve Value field Length */
    ValueLength = DescriptorLenght(pointer, (NameLength) );
    if (ValueLength > 255)
      break;

#ifdef USE_DYNAMIC_MEMORY /* use dynamic memory allocation */
    /* check for title object */
    if (!SongUtilities_WMAstrcmp(tmpBuf, "WM/title"))
    {
      if (pTAGS->Title != NULL)
      {
        MEM_FREE(pTAGS->Title);
      }
      pTAGS->Title = MEM_MALLOC(ValueLength);
      SongUtilities_WMAstrncpy((uint8_t*)pTAGS->Title, (uint8_t *)DescriptorValue(pointer, (NameLength )), ValueLength);
    }

    /* check for author object */
    if (!SongUtilities_WMAstrcmp(tmpBuf, "WM/author"))
    {
      if (pTAGS->Artist != NULL)
      {
        MEM_FREE(pTAGS->Artist);
      }
      pTAGS->Artist = MEM_MALLOC(ValueLength);
      SongUtilities_WMAstrncpy((uint8_t*)pTAGS->Artist, (uint8_t *)DescriptorValue(pointer, (NameLength )), ValueLength);
    }

    /* check for album object */
    if (!SongUtilities_WMAstrcmp(tmpBuf, "WM/album") || !SongUtilities_WMAstrcmp(tmpBuf, "WM/albumtitle"))
    {
      if (pTAGS->Album != NULL)
      {
        MEM_FREE(pTAGS->Album);
      }
      pTAGS->Album = MEM_MALLOC(ValueLength);
      SongUtilities_WMAstrncpy((uint8_t*)pTAGS->Album, (uint8_t *)DescriptorValue(pointer, (NameLength )), ValueLength);
    }

    /* check for year object */
    if (!SongUtilities_WMAstrcmp(tmpBuf, "WM/year"))
    {
      if (pTAGS->Year != NULL)
      {
        MEM_FREE(pTAGS->Year);
      }
      pTAGS->Year = MEM_MALLOC(ValueLength);
      SongUtilities_WMAstrncpy((uint8_t*)pTAGS->Year, (uint8_t *)DescriptorValue(pointer, (NameLength )), ValueLength);
    }

    /* check for genre object */
    if (!SongUtilities_WMAstrcmp(tmpBuf, "WM/genre"))
    {
      if (pTAGS->Genre != NULL)
      {
        MEM_FREE(pTAGS->Genre);
      }
      pTAGS->Genre = MEM_MALLOC(ValueLength);
      SongUtilities_WMAstrncpy((uint8_t*)pTAGS->Genre, (uint8_t *)DescriptorValue(pointer, (NameLength )), ValueLength);
    }

#else /* Not USE_DYNAMIC_MEMORY */
    /* check for title object */
    if (!SongUtilities_WMAstrcmp(tmpBuf, (const uint8_t*)"WM/title") ||
        !SongUtilities_WMAstrcmp(tmpBuf, (const uint8_t*)"WM/OriginalAlbumTitle") ||
          !SongUtilities_WMAstrcmp(tmpBuf, (const uint8_t*)"WM/AlbumTitle"))
    {
      if (pTAGS->Title != NULL)
      {
        SongUtilities_WMAstrncpy((uint8_t*)pTAGS->Title, (uint8_t *)DescriptorValue(pointer, (NameLength )), wmin(ValueLength, MAX_TAG_STRING_LENGTH));
      }
    }

    /* check for author object */
    if (!SongUtilities_WMAstrcmp(tmpBuf, (const uint8_t*)"WM/author") ||
        !SongUtilities_WMAstrcmp(tmpBuf, (const uint8_t*)"WM/AlbumArtist") ||
          !SongUtilities_WMAstrcmp(tmpBuf, (const uint8_t*)"WM/OriginalAlbumArtist"))
    {
      if (pTAGS->Artist != NULL)
      {
        SongUtilities_WMAstrncpy((uint8_t*)pTAGS->Artist, (uint8_t *)DescriptorValue(pointer, (NameLength )), wmin(ValueLength, MAX_TAG_STRING_LENGTH));
      }
    }

    /* check for album object */
    if (!SongUtilities_WMAstrcmp(tmpBuf, (const uint8_t*)"WM/album") ||
        !SongUtilities_WMAstrcmp(tmpBuf, (const uint8_t*)"WM/albumtitle"))
    {
      if (pTAGS->Album != NULL)
      {
        SongUtilities_WMAstrncpy((uint8_t*)pTAGS->Album, (uint8_t *)DescriptorValue(pointer, (NameLength )), wmin(ValueLength, MAX_TAG_STRING_LENGTH));
      }
    }

    /* check for year object */
    if (!SongUtilities_WMAstrcmp(tmpBuf, (const uint8_t*)"WM/year"))
    {
      if (pTAGS->Year != NULL)
      {
        SongUtilities_WMAstrncpy((uint8_t*)pTAGS->Year, (uint8_t *)DescriptorValue(pointer, (NameLength )), wmin(ValueLength, MAX_TAG_STRING_LENGTH));
      }
    }

    /* check for genre object */
    if (!SongUtilities_WMAstrcmp(tmpBuf, (const uint8_t*)"WM/genre"))
    {
      if (pTAGS->Genre != NULL)
      {
        SongUtilities_WMAstrncpy((uint8_t*)pTAGS->Genre, (uint8_t *)DescriptorValue(pointer, (NameLength )), wmin(ValueLength, MAX_TAG_STRING_LENGTH));
      }
    }
#endif /* USE_DYNAMIC_MEMORY */

    pointer = NextDescriptor(pointer, NameLength, ValueLength);
  }
}

/**
* @brief  extract song Tags from wma file.
* @param  pTAGS: output struct.
* @param  file: file handler.
* @param  pBuf: temporary buffer for parsing.
* @retval None.
*/
void SongUtilities_WMATagParser( TAGS_TypeDef *pIDTAG, FIL * file, uint8_t * pBuf )
{
  t_ASFHeader ASF_header;
  t_ParserObj pParser;
  t_ASFObjectHeader tmp_ASFObjectHeader;
  uint32_t NB_headers = 0;

  if (file != NULL)
  {
    FILE_SEEK(file, 0);
    FILE_READ(file, &ASF_header, sizeof(t_ASFHeader), &nbr);

    NB_headers = *(uint32_t *)(ASF_header.NumberOfHeaderObjs);
    NB_headers--;
    pParser.FirstObjOffset = sizeof(t_ASFHeader);
    pParser.NextObjOffset = sizeof(t_ASFHeader);
    for (;NB_headers > 0;NB_headers--)
    {
      FILE_READ(file, &tmp_ASFObjectHeader, sizeof(t_ASFObjectHeader), &nbr);

      if (SongUtilities_IsGUID_eq((int32_t *)tmp_ASFObjectHeader.objectGUID, (int32_t*)ASF_CONTENT_DESCRIPTION_OBJEC))
      {
        FILE_READ(file, pBuf, *(uint32_t*)tmp_ASFObjectHeader.objectSize, &nbr);
        SongUtilities_parseContentDescriptor((uint8_t *)pBuf, pIDTAG);
      }

      if (SongUtilities_IsGUID_eq((int32_t *)tmp_ASFObjectHeader.objectGUID, (int32_t *)ASF_EXTENDED_CONTENT_DESCRIPTION_OBJECT))
      {
        FILE_READ(file, pBuf, *(uint32_t *)tmp_ASFObjectHeader.objectSize, &nbr);
        SongUtilities_parseExtendedContentDescriptors((uint8_t *)pBuf, pIDTAG);
      }

      pParser.NextObjOffset += *(uint32_t*)tmp_ASFObjectHeader.objectSize;
      FILE_SEEK(file, pParser.NextObjOffset);
    }
  }
}
#endif /* __WMA_DECODER__ */

#ifdef __MP3_DECODER__

/**
* @brief  Copy memory to memory.
* @param  dst: Pointer to the destination array.
* @param  src: Pointer to the source of data to be copied.
* @param  cnt: Number of bytes to copy.
* @retval : None.
*/
static void mem_cpy (void* dst, const void* src, int cnt)
{
  char *d = (char*)dst;
  const char *s = (const char *)src;
  while (cnt--) *d++ = *s++;
}

/**
* @brief  Fill memory.
* @param  ptr: Pointer to the block of memory to fill.
* @param  val: Value to be set.
* @param  cnt: Number of bytes to be set.
* @retval : None.
*/
static void mem_set (void* ptr, int val, int cnt)
{
  char *d = (char*)ptr;
  while (cnt--) *d++ = (char)val;
}

/**
* @brief  Extract needed Tag (for the application) information from internal parser buffer.
* @param  song_info: Pointer to internal parser buffer.
* @param  pIDTAGS: pointer to output tag struct.
* @retval : None.
*/
void SongUtilities_FillTags_struct(t_SongInfos *song_info, TAGS_TypeDef * pIDTAGS)
{
  int32_t counter = 0x00, string_length = 0x00;

  /* Check for a valid  pIDTAGS memory space */
  if ( pIDTAGS == NULL )
    return;

  /* Browse all found TAGS */
  for (counter = song_info->n_infos; counter >= 0; counter--)
  {
    /* Select needed Tag */
    switch (song_info->infos[counter].info_type)
    {
    case SONG_TITLE:
      /* First byte contain string length retrieve */
      string_length = *(uint8_t *)(song_info->strings + song_info->infos[counter].string_offset);

#ifdef USE_DYNAMIC_MEMORY /* use dynamic memory allocation */
      /* check for title object */
      if (pIDTAGS->Title != NULL)
      {
        MEM_FREE(pIDTAGS->Title);
      }
      pIDTAGS->Title = MEM_MALLOC((string_length + 1) * sizeof(uint8_t));
#endif /* USE_DYNAMIC_MEMORY */

      if (pIDTAGS->Title != NULL)
      {
        /* copy string (utf-8) */
        MEM_CPY(pIDTAGS->Title, (song_info->strings + song_info->infos[counter].string_offset + 1), wmin(string_length, MAX_TAG_STRING_LENGTH));
        pIDTAGS->Title[(wmin(string_length, MAX_TAG_STRING_LENGTH) )] = 0;
      }
      break;
    case ARTIST:
      /* First byte contain string length retrieve */
      string_length = *(uint8_t *)(song_info->strings + song_info->infos[counter].string_offset);

#ifdef USE_DYNAMIC_MEMORY /* use dynamic memory allocation */
      /* check for Artist object */
      if (pIDTAGS->Artist != NULL)
      {
        MEM_FREE(pIDTAGS->Artist);
      }
      pIDTAGS->Artist = MEM_MALLOC((string_length + 1) * sizeof(uint8_t));
#endif /* USE_DYNAMIC_MEMORY */

      if (pIDTAGS->Artist != NULL)
      {
        /* copy string (utf-8) */
        MEM_CPY(pIDTAGS->Artist, (song_info->strings + song_info->infos[counter].string_offset + 1), wmin(string_length, MAX_TAG_STRING_LENGTH) );
        pIDTAGS->Artist[(wmin(string_length, MAX_TAG_STRING_LENGTH) )] = 0;
      }
      break;
    case ALBUM_TITLE:
      /* First byte contain string length retrieve */
      string_length = *(uint8_t *)(song_info->strings + song_info->infos[counter].string_offset);

#ifdef USE_DYNAMIC_MEMORY /* use dynamic memory allocation */
      /* check for Album object */
      if (pIDTAGS->Album != NULL)
      {
        MEM_FREE(pIDTAGS->Album);
      }
      pIDTAGS->Album = MEM_MALLOC((string_length + 1) * sizeof(uint8_t));
#endif /* USE_DYNAMIC_MEMORY */

      if (pIDTAGS->Album != NULL)
      {
        /* copy string (utf-8) */
        MEM_CPY(pIDTAGS->Album, (song_info->strings + song_info->infos[counter].string_offset + 1), wmin(string_length, MAX_TAG_STRING_LENGTH) );
        pIDTAGS->Album[(wmin(string_length, MAX_TAG_STRING_LENGTH) )] = 0;
      }
      break;
    case GENRE:
      /* First byte contain string length retrieve */
      string_length = *(uint8_t *)(song_info->strings + song_info->infos[counter].string_offset);

#ifdef USE_DYNAMIC_MEMORY /* use dynamic memory allocation */
      /* check for Genre object */
      if (pIDTAGS->Genre != NULL)
      {
        MEM_FREE(pIDTAGS->Genre);
      }
      pIDTAGS->Genre = MEM_MALLOC((string_length + 1) * sizeof(uint8_t));
#endif /* USE_DYNAMIC_MEMORY */

      if (pIDTAGS->Genre != NULL)
      {
        /* copy string (utf-8) */
        MEM_CPY(pIDTAGS->Genre, (song_info->strings + song_info->infos[counter].string_offset + 1), wmin(string_length, MAX_TAG_STRING_LENGTH));
        pIDTAGS->Genre[(wmin(string_length, MAX_TAG_STRING_LENGTH) )] = 0;
      }
      break;
    case YEAR:
      /* First byte contain string length retrieve */
      string_length = *(uint8_t *)(song_info->strings + song_info->infos[counter].string_offset);

#ifdef USE_DYNAMIC_MEMORY /* use dynamic memory allocation */
      /* check for Year object */
      if (pIDTAGS->Year != NULL)
      {
        MEM_FREE(pIDTAGS->Year);
      }
      pIDTAGS->Year = MEM_MALLOC((string_length + 1) * sizeof(uint8_t));
#endif /* USE_DYNAMIC_MEMORY */

      if (pIDTAGS->Year != NULL)
      {
        /* copy string (utf-8) */
        MEM_CPY(pIDTAGS->Year, (song_info->strings + song_info->infos[counter].string_offset + 1), wmin(string_length, MAX_TAG_STRING_LENGTH));
        pIDTAGS->Year[(wmin(string_length, MAX_TAG_STRING_LENGTH) )] = 0;
      }
      break;
    default:
      break;
    }
    string_length = 0x00;
  }
}

/**
* @brief  found ID3 Tag version 2 - extract info.
* @param  buffer: temporary buffer.
* @param  pIDTAGS: pointer to output tag struct.
* @param  file: file handler.
* @retval 0: if success, <0: if error.
*/
int8_t SongUtilities_MP3TagParser(uint8_t * buffer,  TAGS_TypeDef * pIDTAGS , FIL * file)
{
  /* found ID3 Tag version 2 - extract info */
  int32_t               p = 0, n = 0, cnt = 0, chcnt = 0, index = 0, cnt0 = 0;
  int32_t               comment = 0;
  uint16_t              word = 0;  /* pointer inside ID3 tag */
  ID32_PROCESSING_ENUM  processing_switch = ID32_FIND_FRAME;  /* general functionality switch (find frame, parse info from frame) */
  uint32_t              frID = 0, frflags = 0, id_be = 0;
  uint32_t              pos = 0, id32rev = 0, id32flags = 0, id_data = 0, id_frtop = 0;
  uint32_t              load_pos = 0, cmp_enc = 0, gr_iden = 0;
  static uint32_t       v2_length = 0;
  uint32_t              identifier = 0, utf8_conv = 0;
  t_TextEncoding        id32_encoding = TEXT_ENCODING_UTF_8;
  GRESULT               res = 0;
  t_SongInfos           *song_info = NULL;

  /* check if we have a valid file pointer. */
  if ( file == NULL )
    return E_FAIL;

  /* Set access pointer value. */
  song_info = &tmpInfo;
  /* Clear memory space */
  MEM_SET(song_info, 0, sizeof(t_SongInfos));
  /* Set file pointer. */
  song_info->file = file;


  /* get length of id3 tag version 2: 7 lsb bits valid on each byte */
  /* Test ID3 tag ver 2 */
  if ( SongUtilities_ReadFile(song_info, buffer, 10) < 10 )
  {
    SongUtilities_SeekFile (song_info, 0, 0);
    return song_info->ReadFileError;
  }
  else
  {
    identifier = (buffer[0] << 16) | (buffer[1] << 8) | (buffer[2]);
    SongUtilities_SeekFile (song_info, 0, 0);
    if ( ID3_TAG_V2 != identifier )
    {
      return S_FALSE;
    }
  }
  v2_length = ((buffer[ID3_TAG_V2_LENGTH_POSITION] & 0x7f) << 21)
    | ((buffer[ID3_TAG_V2_LENGTH_POSITION+1] & 0x7f) << 14)
      | ((buffer[ID3_TAG_V2_LENGTH_POSITION+2] & 0x7f) << 7)
        | (buffer[ID3_TAG_V2_LENGTH_POSITION+3] & 0x7f) ;

  if ( (v2_length + ID3_TAG_V2_HEADER_LENGTH ) < ID3_BUFFER_LENGTH )
    res = SongUtilities_ReadFile(song_info, buffer, v2_length + ID3_TAG_V2_HEADER_LENGTH);
  else
    res = SongUtilities_ReadFile(song_info, buffer, ID3_BUFFER_LENGTH);

  load_pos = 0; /* what is currently in buffer beginning */

  if (!res)
    return song_info->ReadFileError;

  v2_length += ID3_TAG_V2_HEADER_LENGTH;
  song_info->start_offset = v2_length;

  id32rev = buffer[ID3_TAG_V2_VERSION_POSITION];
  if ((id32rev > 4) || (id32rev < 2))
    return E_FAIL;

  id32flags = buffer[ID3_TAG_V2_FLAGS_POSITION];
  pos = ID3_TAG_V2_HEADER_LENGTH;
  p = id32flags & 0x4f;
  if (p)
  {
    if (p&0xf)
      return E_FAIL;

    /* extended header present */
    switch (id32rev)
    {
    case 3:
      pos = buffer[13] + 14;
      break;
    case 4:
      pos = 10 + (buffer[11] << 14) + (buffer[12] << 7) + buffer[13];
      break;
    default:
      return E_FAIL;
    }
  }

  while (1)
  {
    switch (processing_switch)
    {
    case ID32_FIND_FRAME:
      /* evaluate frame header size */
      if (id32rev == 2)
        p = 6;
      else
        p = 10;

      if (((pos + p) >= v2_length) || (song_info->strtop >= MAX_STRINGS_SIZE))
      {
        processing_switch = ID32_TERMINATE;
        break;
      }

      if ((pos + p) > (load_pos + ID3_BUFFER_LENGTH))
      {
        n = pos & (~3);   /*  new load_pos */
        p = wmin(n + ID3_BUFFER_LENGTH, v2_length);
        cnt = p - n;
        if (cnt <= 0)
        {
          processing_switch = ID32_TERMINATE;
          break;
        }
        /* how many bytes should be prepared in buffer */
        if ((n - load_pos) < ID3_BUFFER_LENGTH) /* check if data follow continuously */
        {
          p = ID3_BUFFER_LENGTH - (n - load_pos);  /* count of present bytes to keep */
          MEM_CPY(buffer, buffer + (n - load_pos), p );
          cnt -= p  ;
        }
        else
        {
          /* not continuous jump ( frame is big ) */
          p = 0;  /* no saved bytes */
          SongUtilities_SeekFile(song_info, n, 0);

        }
        res = SongUtilities_ReadFile(song_info, buffer + p, cnt); /* fill rest of buffer */

        load_pos = n;
        if (!res)
          return song_info->ReadFileError;
      }

      /* set string leading byte address */
      /* to indicate that no characters were added so far */
      song_info->strings[song_info->strtop] = 0 ;
      song_info->infos[song_info->n_infos].string_offset = song_info->strtop;

      frID = (buffer[pos - load_pos] << 16)
        + (buffer[pos - load_pos + 1] << 8)
          + buffer[pos - load_pos + 2];
      if (id32rev != 2)
        frID = (frID << 8) + buffer[pos - load_pos + 3];
      frflags = (buffer[pos - load_pos + 8] << 8) + buffer[pos-load_pos + 9];

      processing_switch = ID32_READ_DATA;

      switch (id32rev)
      {
      case 2:
        /* Obsolete revesion */
        id_frtop = (buffer[pos - load_pos + 3] << 16)
          + (buffer[pos - load_pos + 4] << 8)
            + buffer[pos - load_pos + 5];
        if(!id_frtop)
        {
          processing_switch = ID32_TERMINATE;
          break;

        }
        id_data = pos+6;
        id_frtop += id_data;
        switch(frID)	// revision 2
        {
        case TXT_TAL:
          song_info->infos[song_info->n_infos].info_type = ALBUM_TITLE;
          break;
        case TXT_TT2:
          song_info->infos[song_info->n_infos].info_type = SONG_TITLE;
          break;
        case TXT_TP1:
          song_info->infos[song_info->n_infos].info_type = ARTIST;
          break;
        case TXT_COM:
          if(comment)
            processing_switch = ID32_FRAME_SKIP;
          else
            song_info->infos[song_info->n_infos].info_type = COMMENT;
          break;
        case TXT_TYE:
          song_info->infos[song_info->n_infos].info_type = YEAR;
          break;
        case TXT_TCO:
          song_info->infos[song_info->n_infos].info_type = GENRE;
          break;
        case TXT_TRK:
          song_info->infos[song_info->n_infos].info_type = TRACK_NO;
          break;
          /* Picture Tag */
        case TXT_PIC:
          /* we stop processing tags */
          processing_switch = ID32_TERMINATE;
          /* Picture Tag End*/
          break;
        default:
          processing_switch = ID32_FRAME_SKIP;
          break;
        }
        break;
      case 3:
      case 4:
        if (id32rev == 3)
          id_frtop = (buffer[pos - load_pos + 4] << 24)
            + (buffer[pos - load_pos + 5] << 16)
              + (buffer[pos - load_pos + 6] << 8)
                + buffer[pos - load_pos + 7];
        else
          id_frtop = (buffer[pos - load_pos + 4] << 21)
            + (buffer[pos - load_pos + 5] << 14)
              + (buffer[pos - load_pos + 6] << 7)
                + buffer[pos - load_pos + 7];
        if (!id_frtop)
        {
          processing_switch = ID32_TERMINATE;
          break;

        }
        id_data = pos + 10;
        id_frtop += id_data;
        frflags = (buffer[pos - load_pos + 8] << 8) + buffer[pos - load_pos + 9];
        switch (frID)
        {
        case TXT_TALB:
          song_info->infos[song_info->n_infos].info_type = ALBUM_TITLE;
          break;
        case TXT_TIT2:
          song_info->infos[song_info->n_infos].info_type = SONG_TITLE;
          break;
        case TXT_TPE1:
          song_info->infos[song_info->n_infos].info_type = ARTIST;
          break;
        case TXT_COMM:
          if (comment)
            processing_switch = ID32_FRAME_SKIP;
          else
            song_info->infos[song_info->n_infos].info_type = COMMENT;
          break;
        case TXT_TYER:
          song_info->infos[song_info->n_infos].info_type = YEAR;
          break;
        case TXT_TCON:
          song_info->infos[song_info->n_infos].info_type = GENRE;
          break;
        case TXT_TRCK:
          song_info->infos[song_info->n_infos].info_type = TRACK_NO;
          break;
        case TXT_APIC:
          /* Picture tag */
          /* Stop processing if picture tag is found. */
          processing_switch = ID32_TERMINATE;
          break;
        default:
          processing_switch = ID32_FRAME_SKIP;
          break;
        }
        break;
      default:
        return E_FAIL;
      }

      break;

    case ID32_READ_DATA:
      if ((song_info->n_infos >= XSONG_INFO_TAGS) || (song_info->strtop >= MAX_STRINGS_SIZE - 2))
      {
        processing_switch = ID32_TERMINATE;
        break;
      }

      if (id_frtop > v2_length)
      {
        processing_switch = ID32_TERMINATE;
        break;
      }

      if (id_frtop > (load_pos + ID3_BUFFER_LENGTH))
      {
        n = pos & (~3);   /* new load_pos */
        p = wmin(n + ID3_BUFFER_LENGTH, v2_length);
        cnt = p - n;
        if (cnt <= 0)
        {
          processing_switch = ID32_TERMINATE;
          break;
        }
        /* how many bytes should be prepared in buffer */
        if ((n - load_pos) < ID3_BUFFER_LENGTH)
        {
          /* check if data follow continuously */
          /* count of present bytes to keep */
          p = ID3_BUFFER_LENGTH - (n - load_pos);
          MEM_CPY(buffer, buffer + (n - load_pos), p );
          cnt -= p  ;
        }
        else
        {
          /* not continuous jump ( frame is big ) */
          p = 0;  /* no saved bytes */
          SongUtilities_SeekFile(song_info, n, 0);
        }
        /* fill rest of buffer */
        res = SongUtilities_ReadFile(song_info, buffer + p, cnt);
        load_pos = n;
        if (!res)
          return song_info->ReadFileError;
      }

      /* frame data should be in buffer, at least full buffer */
      if (id32rev != 2)
      {
        if (id32rev == 3)
        {
          cmp_enc = 128 + 64;
          gr_iden = 0x20;
        }
        else
        {
          cmp_enc = 8 + 4;
          gr_iden = 0x40;
          /* check length indicator */
          if (frflags & 1)
            id_data += 4;
        }
        /* test encryption & compression */
        if (frflags & cmp_enc)
        {
          processing_switch = ID32_FRAME_SKIP;
          goto id32_skipfr;
        }
        /* test group identity */
        if (frflags & gr_iden)
        {
          id_data += 1;
        }
      }
      /* start parsing string  */
      id32_encoding = (t_TextEncoding)buffer[id_data - load_pos];
      id_data++;

      /* All ID3v2 tags are stored as UTF-8 Pascal strings, keep track of the
      ** original encoding type used in the file. */
      song_info->infos[song_info->n_infos].enc_of_src = id32_encoding;
      song_info->infos[song_info->n_infos].tag_version = 2;
      /* ignore frame when contains only encoding byte */
      if (id_data >= id_frtop)
      {
        processing_switch = ID32_FRAME_SKIP;
        goto id32_skipfr;
      }

      switch (id32_encoding)
      {
        /* ISO-8859-1, terminated with 0x00. */
      case TEXT_ENCODING_ISO_8859_1:
        utf8_conv = 1;

      asc_loop:
        chcnt = cnt0 = 0;
        p = 1;

        if (song_info->infos[song_info->n_infos].info_type != COMMENT)
          goto id3_unic_skip2;

        for (index = 0;index < 3;index++)
        {
          if (!buffer[id_data-load_pos+index])
            cnt0++;

          if ((buffer[id_data-load_pos+index] > 96)
              && (buffer[id_data-load_pos+index] < 123)
                && ((id_data - load_pos + index) < ID3_BUFFER_LENGTH)
                  && (id_data + index < id_frtop ))
          {
            chcnt++;
          }
        }

        if (((cnt0 == 3) || (chcnt == 3)) && (!buffer[id_data-load_pos+index]))
        {
          /* skip language descriptor */
          id_data += 4;
        }
        chcnt = 0;

      id3_unic_skip2:

        while (p && ((id_data - load_pos) < ID3_BUFFER_LENGTH) && (id_data < id_frtop))
        {
          word = buffer[id_data - load_pos];
          if (utf8_conv)
          {
            p = SongUtilities_write_utf8(song_info, word);
          }
          else
          {
            /* it is already UTF-8 byte */
            p = SongUtilities_write_byte(song_info, word);
          }

          chcnt += p;

          id_data++;
        }

        if (chcnt > 0)
        {
          /* first check COMM presence before ptr is increased */
          if (song_info->infos[song_info->n_infos].info_type == COMMENT)
            comment = 1;
          song_info->n_infos++;
        }

        processing_switch = ID32_FRAME_SKIP; /* continue with next frame */

        break;

      case TEXT_ENCODING_UTF_16:   /* UTF-16 encoded Unicode with BOM, terminated with 0x00. */
      case TEXT_ENCODING_UTF_16BE: /* UTF-16BE encoded Unicode without BOM, terminated with 0x00. */
        id_be = 1;
        chcnt = cnt0 = 0;
        p = 1;

        if (song_info->infos[song_info->n_infos].info_type != COMMENT)
          goto id3_unic_skip;

        for (index = 0;index < 3;index++)
        {
          if (!buffer[id_data-load_pos+index])
            cnt0++;

          if ((buffer[id_data-load_pos+index] > 96)
              && (buffer[id_data-load_pos+index] < 123)
                && ((id_data - load_pos + index) < ID3_BUFFER_LENGTH)
                  && (id_data + index < id_frtop ))
          {
            chcnt++;
          }
        }

        if ((chcnt == 3) || (cnt0 == 3))
        {
          chcnt = 0;
          id_data += 5;  /* skip language descriptor */
        }
        else
        {
          chcnt = 0;
        }

      id3_unic_skip:

        while (p && ((id_data - load_pos) < ID3_BUFFER_LENGTH) && (id_data < id_frtop))
        {
          word = (buffer[id_data-load_pos] << 8) + buffer[id_data-load_pos+1];
          switch (word)
          {
          case 0xFEFF:
            id_be = 1;
            id_data += 2;
            break;
          case 0xFFFE:
            id_be = 0;
            id_data += 2;
            break;
          default:
            if (!id_be) /* format word to BE */
            {
              word = ((word & 0xff) << 8) + ((word & 0xff00) >> 8);
            }

            if ((id32rev == 4) && ((word&0xFC00) == 0xD800))
            {
              id_data += 4 ;  /* skip surrogate pair */
            }
            else
            {
              p = SongUtilities_write_utf8(song_info, word);
              chcnt += p;
              id_data += 2;
            }
          }
        }

        if (chcnt > 0)
        {
          /* first check COMM presence before ptr is increased */
          if (song_info->infos[song_info->n_infos].info_type == COMMENT)
            comment = 1;
          song_info->n_infos++;
        }
        processing_switch = ID32_FRAME_SKIP; /* continue with next frame */

        break;

      case TEXT_ENCODING_UTF_8:  /* UTF-8 encoded Unicode, terminated with 0x00. */
        utf8_conv = 0;
        goto asc_loop;

      default:
        processing_switch = ID32_FRAME_SKIP;
        goto id32_skipfr;
      }
    id32_skipfr:
      break;

    case ID32_FRAME_SKIP:
      pos = id_frtop;
      if (pos < v2_length)
        processing_switch = ID32_FIND_FRAME;
      else
        processing_switch = ID32_TERMINATE;

      break;

    case ID32_TERMINATE:
      SongUtilities_FillTags_struct(song_info, pIDTAGS);
      return S_OK;

    default:
      return E_FAIL;
    }
  }
}

#endif /* __MP3_DECODER__ */

/**
* @brief  Read a buffer from file.
* @param  pSongInfos: pointer to t_SongInfos struct.
* @param  buffer: Destination buffer.
* @param  nb_bytes: number of bytes.
* @retval number of bytes.
*/
int32_t SongUtilities_ReadFile(t_SongInfos * song_info, uint8_t *buffer, uint32_t nb_bytes)
{
  FILE_READ(song_info->file, buffer, nb_bytes, (void *)(uint32_t)&nbr);

  return nbr;
}

/**
* @brief  Seek to new position in file.
* @param  pSongInfos: pointer to t_SongInfos struct.
* @param  position: position in byte.
* @param  SeekMode: Seek mode.
* @retval 0: if success, error code else.
*/
int32_t SongUtilities_SeekFile(t_SongInfos * song_info, int32_t position, uint8_t SeekMode)
{
  return FILE_SEEK(song_info->file, position);
}

/**
* @brief  Write a byte to string.
* @param  pSongInfos: pointer to t_SongInfos struct.
* @param  b: byte.
* @retval 1: if success, 0: if not success.
*/
int SongUtilities_write_byte(t_SongInfos *pSongInfos, uint8_t b)
{
  if (pSongInfos->strtop >= MAX_STRINGS_SIZE)        /* to avoid strings array overflow */
    return 0;

  if (pSongInfos->strings[pSongInfos->infos[pSongInfos->n_infos].string_offset] >= ID3V2_MAX_TEXT_SIZE)   /* check current length */
    return 0;

  if (pSongInfos->strings[pSongInfos->infos[pSongInfos->n_infos].string_offset] == 0)   /* pointer shift at beginning */
    pSongInfos->strtop++;

  *(pSongInfos->strings + pSongInfos->strtop) = b;

  pSongInfos->strtop++;
  (pSongInfos->strings[pSongInfos->infos[pSongInfos->n_infos].string_offset])++;

  return 1;
}

/**
* @brief  Convert a charactere to utf-8 and write it to string.
* @param  pSongInfos: pointer to t_SongInfos struct.
* @param  w: wide char.
* @retval 1: if success, 0: if not success.
*/
int SongUtilities_write_utf8(t_SongInfos *pSongInfos, uint16_t w)
{
  if (w <= 0x7f)
    return SongUtilities_write_byte(pSongInfos, w);
  if (w <= 0x7ff)
  {
    if (SongUtilities_write_byte(pSongInfos, 0xC0 | (w >> 6)))
    {
      return SongUtilities_write_byte(pSongInfos, 0x80 | (w & 0x3F));
    }
    return 0;
  }

  if (SongUtilities_write_byte(pSongInfos, 0xE0 | (w >> 12)))
  {
    if (SongUtilities_write_byte(pSongInfos, 0x80 | (0x3f & (w >> 6))))
    {
      return SongUtilities_write_byte(pSongInfos, 0x80 | (w & 0x3F));
    }
  }
  return 0;
}

/**
* @brief  Truncate a string if it is longer than a maximum length.
* @param  Dst: pointer to the resulting string
* @param  Src: pointer to the source string
* @param  MaxLen: Maximum allowed length
* @retval pointer to 'TSpiritMP3Info' struct.
*/
void StrCtrlLength(void* Dst, void* Src, uint32_t MaxLen)
{
  if(strlen((char*)Src) > MaxLen)
  {
    if (Src == Dst)
    {
      ((char*)Src)[MaxLen-1] = '\0';
    }
    else
    {
      strncpy(Dst, Src, MaxLen);
    }
  }
}

/**
* @}
*/

/**
* @}
*/

/**
* @}
*/
