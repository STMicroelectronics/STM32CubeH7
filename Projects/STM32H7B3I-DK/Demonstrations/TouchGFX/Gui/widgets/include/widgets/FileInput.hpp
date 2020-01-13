#ifndef FILE_INPUT_HPP
#define FILE_INPUT_HPP

#include <touchgfx/hal/Types.hpp>

namespace touchgfx
{
namespace fileinput
{
//type used for file handles
typedef void* FileHdl;

//Directory entry. Name and type.
struct DirEntry
{
    char name[50];
    bool isDirectory;
};

int Init(void);
void deInit(void);

/**
 * @fn const char* getDrive(void);
 *
 * @brief Get the Storage Drive Logical Name.
 *
 *        Open a file for reading in binary mode.
 *
 * @param None.
 *
 * @return Logical Drive Name.
 */
const char* getDrive(void);

/**
 * @fn FileHdl openFile(const char* const filename);
 *
 * @brief Open a file for reading.
 *
 *        Open a file for reading in binary mode.
 *
 * @param filename Name of file.
 *
 * @return Handle to file.
 */
FileHdl openFile(const char* const filename);

/**
 * @fn void closeFile(FileHdl hdl);
 *
 * @brief Close an open file.
 *
 *        Close an open file.
 *
 * @param hdl    Handle to file.
 */
void closeFile(FileHdl hdl);

/**
 * @fn int readFile(FileHdl hdl, uint8_t* const buffer, uint32_t length);
 *
 * @brief Read data from a file
 *
 *        Read data from an open file.
 *
 * @param hdl    Handle to file.
 * @param buffer The buffer to data into.
 * @param length The number of bytes to read.
 *
 * @return Actual number of bytes read.
 */
int readFile(FileHdl hdl, uint8_t* const buffer, uint32_t length);

/**
 * @fn void seekFile(FileHdl hdl, uint32_t offset);
 *
 * @brief Seek in a file
 *
 *        Seek in an open file. The offset is from the beginning
 *        of the file.
 *
 * @param hdl    Handle to file.
 * @param offset The offset to seek to.
 */
void seekFile(FileHdl hdl, uint32_t offset);

/**
 * @fn int getDirectoryList(const char* const dirname, const char* ext, DirEntry* dirlist, uint32_t length);
 *
 * @brief List files in a directory.
 *
 *        List directories and files in a directory. Only listing
 *        files matching the extension. The entries are put into
 *        the dirlist array. Search is done from current
 *        directory. The file extension does not apply to
 *        directories.
 *
 * @param dirname Name of directory to list.
 * @param ext     File extension to match.
 * @param dirlist Array of DirEntry to hold result.
 * @param length  Length of dirlist array.
 *
 * @return Number of entries found.
 */
int getDirectoryList(const char* const dirname,
                     const char* ext,
                     DirEntry* dirlist,
                     uint32_t length);


//sort the dirlist
int sortDirectoryList(DirEntry* dirlist,
                      uint32_t length);

// helper accumulator
int getFileListRAccumulator(const char* const dirname,
                            const char* ext,
                            DirEntry* filelist,
                            uint32_t length,
                            uint32_t& current);


/**
 * @fn int getFileListR(const char* const dirname, const char* ext, DirEntry* filelist, uint32_t length);
 *
 * @brief List files in a directory recursively
 *
 *        List files in a directory recursively. Only listing
 *        files matching the extension. The entries are put into
 *        the dirlist array. Search is done from current
 *        directory.
 *
 * @param dirname Name of directory to list.
 * @param ext     File extension to match.
 * @param dirlist Array of DirEntry to hold result.
 * @param length  Length of dirlist array.
 *
 * @return Number of entries found.
 */
int getFileListR(const char* const dirname,
                 const char* ext,
                 DirEntry* filelist,
                 uint32_t length);



#ifdef SIMULATOR
const char* const DirectorySeparator = "\\";
#else
const char* const DirectorySeparator = "/";
#endif

}
}

#endif
