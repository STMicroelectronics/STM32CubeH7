#include <widgets/FileInput.hpp>

//Implementation of fileinput using FatFS functions
#ifndef SIMULATOR

#include "storage.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cctype>

static char* stristr( const char* str1, const char* str2 )
{
    const char* p1 = str1 ;
    const char* p2 = str2 ;
    const char* r = *p2 == 0 ? str1 : 0 ;

    while( *p1 != 0 && *p2 != 0 )
    {
        if( tolower( (unsigned char)*p1 ) == tolower( (unsigned char)*p2 ) )
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
            if( r != 0 )
            {
                p1 = r + 1 ;
            }

            if( tolower( (unsigned char)*p1 ) == tolower( (unsigned char)*p2 ) )
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

    return *p2 == 0 ? (char*)r : 0 ;
}

namespace touchgfx
{
namespace fileinput
{
static FIL f1; /* The single File object */
static FIL f2; /* The single File object */

static char full_path[_MAX_LFN + 2];
static char parent_dir[_MAX_LFN + 2];
static int level = -1;
static int num_files = 0;

static bool f1open = false;
static bool f2open = false;

void deInit(void)
{
    f_close(&f1);
    f1open = false;
    f_close(&f2);
    f2open = false;
}

const char* getDrive(void)
{
  if(Storage_GetStatus(MSD_DISK_UNIT))
    return Storage_GetDrive(MSD_DISK_UNIT);

  /* We always try to get at least a default storage Drive */
  return Storage_GetDrive(USB_DISK_UNIT);
}

FileHdl openFile(const char* const filename)
{
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
        f_close(&f1);
        f1open = false;
    }
    else if ((FIL*)hdl == &f2)
    {
        f_close(&f2);
        f2open = false;
    }
}

int readFile(fileinput::FileHdl hdl, uint8_t* const buffer, uint32_t length)
{
    uint32_t r;
    f_read((FIL*)hdl, buffer, length, (unsigned int*)&r);
    return r;
}

void seekFile(FileHdl hdl, uint32_t offset)
{
    f_lseek((FIL*)hdl, offset);
}

int getDirectoryList(const char* const dirname, const char* ext, DirEntry* dirlist, uint32_t length)
{
    FILINFO fno;
    DIR dir;

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
                memset(full_path, '\0', _MAX_LFN);

                level++;
                if (level <= 0)
                {
                  memset(parent_dir, '\0', _MAX_LFN);
                  strcat(parent_dir, dirname);
                }

                int i = strlen(parent_dir);

                strcpy(full_path, parent_dir);
                strcat(full_path, "/");
                strcat(full_path, fno.fname);

                 sprintf(parent_dir, full_path);

                /*
                 * search media files in the subdirectory
                 */
                getDirectoryList(full_path, ext, dirlist, length - n);
                level--;
                parent_dir[i] = 0;
            }
            else
            {
                if (stristr(fno.fname, ext))
                {
                    dirlist[num_files].isDirectory = false;
                    if (level > 0)
                    {
                      strncat(dirlist[num_files].name, full_path, strlen(full_path));
                    }
                    else
                    {
                      strncat(dirlist[num_files].name, parent_dir, strlen(parent_dir));
                    }
                    strcat(dirlist[num_files].name, "/");
                    strncat(dirlist[num_files].name, fno.fname, sizeof(dirlist[num_files].name) - strlen(full_path) - 1);
                    num_files++;
                    n++;
                }
            }
        }
    }

    f_closedir(&dir);

    if (level == -1)
      sortDirectoryList(dirlist, num_files);

    return (level == -1) ? num_files : n;
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
                char newdirname[_MAX_LFN + 2];
                snprintf(newdirname, _MAX_LFN + 2 , "%s/%s", dirname, fno.fname);
                getFileListRAccumulator(newdirname, ext, filelist, length, current);
            }
            else
            {
                if (stristr(fno.fname, ext))
                {
                    char path[_MAX_LFN + 2];
                    snprintf(path, _MAX_LFN + 2 , "%s/%s", dirname, fno.fname);
                    strncpy(filelist[current].name, path, 50);
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
