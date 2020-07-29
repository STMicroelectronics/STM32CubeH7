#include <gui/video_player_screen/FileInput.hpp>

//Implementation of fileinput using FatFS functions
#ifndef SIMULATOR

#include "storage_app.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ctype.h"

namespace touchgfx
{
namespace fileinput
{
static FIL f1; /* The single File object */
static FIL f2; /* The single File object */

static bool f1open = false;
static bool f2open = false;

static char* stristr( char* str1, const char* str2 )
{
    char* p1 = str1 ;
    const char* p2 = str2 ;
    char* r = *p2 == 0 ? str1 : 0 ;

    while( *p1 != 0 && *p2 != 0 )
    {
        if( tolower( *p1 ) == tolower( *p2 ) )
        {
            if( r == 0 )
            {
                r = p1 ;
            }

            p2++ ;
        }
        else
        {
            p2 = str2 ;
            if( tolower( *p1 ) == tolower( *p2 ) )
            {
                r = p1 ;
                p2++ ;
            }
            else
            {
                r = 0 ;
            }
        }

        p1++ ;
    }

    return *p2 == 0 ? r : 0 ;
}

int Init(void)
{
  return STORAGE_GetStatus(/*USB_DISK_UNIT*/MSD_DISK_UNIT);
}

void deInit(void)
{
    closeFile((FIL*)&f1);
    closeFile((FIL*)&f2);
}

const char* getDrive(void)
{
  return STORAGE_GetDrive(/*USB_DISK_UNIT*/MSD_DISK_UNIT);
}

FileHdl openFile(const char* const filename)
{
    if(!STORAGE_GetStatus(/*USB_DISK_UNIT*/MSD_DISK_UNIT))
    {
        return 0;
    }

    if (f1open == false)
    {
        if (f_open(&f1, filename, FA_READ))
        {
            return 0;
        }
        else
        {
            f1open = true;
            return (FileHdl)&f1;
        }
    }
    else if (f2open == false)
    {
        if (f_open(&f2, filename, FA_READ))
        {
            return 0;
        }
        else
        {
            f2open = true;
            return (FileHdl)&f2;
        }
    }
    else
    {
        return 0;
    }
}

void closeFile(FileHdl hdl)
{
    if ((FIL*)hdl == &f1)
    {
        if(STORAGE_GetStatus(/*USB_DISK_UNIT*/MSD_DISK_UNIT))
        {
            f_sync(&f1);
            f_close(&f1);
        }
        f1open = false;
    }
    else if ((FIL*)hdl == &f2)
    {
        if(STORAGE_GetStatus(/*USB_DISK_UNIT*/MSD_DISK_UNIT))
        {
            f_sync(&f2);
            f_close(&f2);
        }
        f2open = false;
    }
}

int readFile(fileinput::FileHdl hdl, uint8_t* const buffer, uint32_t length)
{
    uint32_t r = 0;
    if(STORAGE_GetStatus(/*USB_DISK_UNIT*/MSD_DISK_UNIT))
    {
        f_read((FIL*)hdl, buffer, length, (unsigned int*)&r);
    }
    return r;
}

void seekFile(FileHdl hdl, uint32_t offset)
{
    if(STORAGE_GetStatus(/*USB_DISK_UNIT*/MSD_DISK_UNIT))
    {
        f_lseek((FIL*)hdl, offset);
    }
}

int getDirectoryList(const char* const dirname, const char* ext, DirEntry* dirlist, uint32_t length)
{
    FILINFO fno;
    DIR dir;

    if(!STORAGE_GetStatus(/*USB_DISK_UNIT*/MSD_DISK_UNIT))
    {
        return 0;
    }

    fno.fsize = _MAX_LFN;

    uint32_t n = 0;

    FRESULT res = f_opendir(&dir, dirname);
    if (res == FR_OK)
    {
        while (1)
        {
            res = f_readdir(&dir, &fno);

            if (res != FR_OK || fno.fname[0] == 0)
            {
                break;
            }
            if (fno.fname[0] == '.')
            {
                continue;
            }
            if (strcmp(fno.fname, "SYSTEM~1") == 0) //skip windows
            {
                continue;
            }
            if ((fno.fattrib & AM_DIR) != 0)
            {
                dirlist[n].isDirectory = true;
                strncpy(dirlist[n].name, fno.fname, sizeof(dirlist[n].name));
                n++;
            }
            else
            {
                if (stristr(fno.fname, ext))
                {
                    dirlist[n].isDirectory = false;
                    strncpy(dirlist[n].name, fno.fname, sizeof(dirlist[n].name));
                    n++;
                }
            }
        }
    }
    f_closedir(&dir);
    sortDirectoryList(dirlist, n);
    return n;
}

static int dir_sort(const DirEntry* v1, const DirEntry* v2);

static int dir_sort(const DirEntry* v1, const DirEntry* v2)
{
    if (v1->isDirectory && !v2->isDirectory)
    {
        return -1;
    }
    if (!v1->isDirectory && v2->isDirectory)
    {
        return 1;
    }
    return strcmp(v1->name, v2->name);
}

int sortDirectoryList(DirEntry* dirlist,
                      uint32_t length)
{
    qsort(dirlist, length, sizeof(DirEntry), (int (*)(const void*, const void*))dir_sort);
    return 1;
}

int getFileListR(const char* const dirname, const char* ext, DirEntry* filelist, uint32_t length)
{
    uint32_t accumulator = 0;
    int n = getFileListRAccumulator(dirname, ext, filelist, length, accumulator);
    sortDirectoryList(filelist, n);
    return n;
}

int getFileListRAccumulator(const char* const dirname, const char* ext, DirEntry* filelist, uint32_t length, uint32_t& current)
{
    FILINFO fno;
    DIR dir;

    if(!STORAGE_GetStatus(/*USB_DISK_UNIT*/MSD_DISK_UNIT))
    {
        return current;
    }

    FRESULT res = f_opendir(&dir, dirname);
    if (res == FR_OK)
    {
        while (1)
        {
            res = f_readdir(&dir, &fno);

            if (res != FR_OK || fno.fname[0] == 0)
            {
                break;
            }
            if (fno.fname[0] == '.')
            {
                continue;
            }
            if (strcmp(fno.fname, "SYSTEM~1") == 0) //skip windows
            {
                continue;
            }
            if ((fno.fattrib & AM_DIR) != 0)
            {
                char newdirname[300];
                snprintf(newdirname, 300, "%s/%s", dirname, fno.fname);
                getFileListRAccumulator(newdirname, ext, filelist, length, current);
            }
            else
            {
                if (stristr(fno.fname, ext))
                {
                    char path[300];
                    snprintf(path, 300, "%s/%s", dirname, fno.fname);
                    strncpy(filelist[current].name, path, 53);
                    filelist[current].isDirectory = false;
                    current++;
                }
            }
        }
    }
    f_closedir(&dir);
    return current;
}

} // namespace fileinput
} // namespace touchgfx

#endif
