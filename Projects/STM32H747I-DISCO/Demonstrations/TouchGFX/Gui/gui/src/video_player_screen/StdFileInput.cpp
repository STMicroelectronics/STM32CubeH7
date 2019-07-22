

#include <gui/video_player_screen/FileInput.hpp>

//Implementation of fileinput using standard libc functions
#ifdef SIMULATOR
#include <windows.h>
#include <stdio.h>

#ifdef _MSC_VER
#define snprintf _snprintf
#endif

namespace touchgfx
{
namespace fileinput
{

void deInit(void)
{

}

const char* getDrive(void)
{
#ifdef _MSC_VER
    return "../../sdcard";
#else
    return "sdcard";
#endif
}

FileHdl openFile(const char* const filename)
{
    return fopen(filename, "rb");
}

void closeFile(FileHdl hdl)
{
    fclose((FILE*)hdl);
}

int readFile(fileinput::FileHdl hdl, uint8_t* const buffer, uint32_t length)
{
    uint32_t r = fread(buffer, 1, length, (FILE*)hdl);
    return (int)r;
}

void seekFile(FileHdl hdl, uint32_t offset)
{
    fseek((FILE*)hdl, offset, SEEK_SET);
}

int getDirectoryList(const char* const dirname, const char* ext, DirEntry* dirlist, uint32_t length)
{
    char pattern[53];
    snprintf(pattern, 53, "%s\\*", dirname);
    WIN32_FIND_DATA findFileData;
    HANDLE hFind = FindFirstFile(pattern, &findFileData);

    if (hFind == INVALID_HANDLE_VALUE)
    {
        return 0;
    }
    else
    {
        uint32_t n = 0;
        do
        {
            if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            {
                if (strcmp(findFileData.cFileName, ".") && strcmp(findFileData.cFileName, ".."))
                {
                    dirlist[n].isDirectory = true;
                    strncpy(dirlist[n].name, findFileData.cFileName, sizeof(dirlist[n].name));
                    n++;
                }
            }
            else
            {
                if (strstr(findFileData.cFileName, ext))
                {
                    dirlist[n].isDirectory = false;
                    strncpy(dirlist[n].name, findFileData.cFileName, sizeof(dirlist[n].name));
                    n++;
                }
            }
        }
        while ((FindNextFile(hFind, &findFileData) != 0) && (n < length));
        FindClose(hFind);
        sortDirectoryList(dirlist, n);
        return n;
    }
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
    char pattern[53];
    snprintf(pattern, 53, "%s\\*", dirname);
    WIN32_FIND_DATA findFileData;
    HANDLE hFind = FindFirstFile(pattern, &findFileData);

    if (hFind == INVALID_HANDLE_VALUE)
    {
        return 0;
    }
    else
    {
        //uint32_t n = 0;
        do
        {
            if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            {
                if (strcmp(findFileData.cFileName, ".") && strcmp(findFileData.cFileName, ".."))
                {
                    char newdirname[53];
#ifdef _MSC_VER
                    _snprintf_s(newdirname, 53, "%s\\%s", dirname, findFileData.cFileName);
#else
                    snprintf(newdirname, 53, "%s\\%s", dirname, findFileData.cFileName);
#endif
                    getFileListRAccumulator(newdirname, ext, filelist, length, current);
                    //filelist[n].isDirectory = true;
                    //strncpy(filelist[n].name, findFileData.cFileName, sizeof(filelist[n].name));
                    //n++;
                }
            }
            else
            {
                if (strstr(findFileData.cFileName, ext))
                {
                    filelist[current].isDirectory = false;
                    char path[53];
                    snprintf(path, 53, "%s\\%s", dirname, findFileData.cFileName);
                    strncpy(filelist[current].name, path, 53);
                    current++;
                }
            }
        }
        while ((FindNextFile(hFind, &findFileData) != 0) && (current < length));
        FindClose(hFind);

        return current;
    }
}


} // namespace fileinput
} // namespace touchgfx

#endif
