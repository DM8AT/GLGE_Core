/**
 * @file File.h
 * @author DM8AT
 * @brief define the API to interface with a file class to load, save and manipulate files
 * @version 0.1
 * @date 2025-09-29
 * 
 * @copyright Copyright (c) 2025
 * 
 */

//header guard
#ifndef _GLGE_CORE_FILE_
#define _GLGE_CORE_FILE_

//include strings
#include "../../GLGE_BG/CBinding/String.h"
//include the file decorator system
#include "FileDecorator.h"

//check for C++ to create a class
#if __cplusplus
//include the default io library for C (i'm using the C file interface)
#include <cstdlib>
#include <cstdio>

//include the C++ filesystem
#include <filesystem>
//include vectors for resizable arrays
#include <vector>

/**
 * @brief store a file to handle
 */
class File
{
public:

    /**
     * @brief Construct a new File
     * 
     * No file will be opend
     */
    File() = default;

    /**
     * @brief Construct a new File interface
     * 
     * @param path the path to the file to open / read from
     * @param allowCreate true : if the file does not exist, it will be created (if possible) | false : if the file does not exist, it won't be created. 
     */
    File(const std::filesystem::path& path, bool allowCreate, const std::vector<FileDecorator>& decorators = {})
     : m_decorators(decorators)
    {open(path, allowCreate);}

    /**
     * @brief Destroy the File and close it if it is open
     */
    ~File();

    /**
     * @brief open a file (close the current one if one is open)
     * 
     * @param path the path to the file to open
     * @param allowCreate true : the file will be created if it does not currently exist
     * @return true : the file was opend successfully
     * @return false : failed to open the file
     */
    bool open(const std::filesystem::path& path, bool allowCreate);

    /**
     * @brief close any file that is currently open
     */
    void close();

    /**
     * @brief save the current contents to the open file
     * 
     * @warning if no file is open, this does nothing
     */
    void save();

    /**
     * @brief re-load the contents of any currently opend file
     * 
     * All changes made to the contents will be reverted. 
     * The contents MAY NOT MATCH the last read contents if the actual file contents were changed. 
     * 
     * @warning if no file is open, this does nothing
     * 
     * @return true : the file changed contents
     * @return false : the file did not change contents
     */
    bool reload();

    /**
     * @brief Get the amount of bound File Decorators
     * 
     * @return size_t the amount of bound file decorators
     */
    inline size_t getFileDecoratorCount() const noexcept {return m_decorators.size();}

    /**
     * @brief Get the All Decorators 
     * 
     * @return FileDecorator* 
     */
    inline FileDecorator* getAllDecorators() noexcept {return m_decorators.data();}

    /**
     * @brief Get a specific File Decorator
     * 
     * @param index the index of the file decorator to quarry
     * @return FileDecorator& a reference to the file decorator
     */
    inline FileDecorator& getFileDecorator(size_t index) {return m_decorators[index];}

    /**
     * @brief add a new file decorator to the file class
     * 
     * @param decorator the new file decorator to add to the file
     */
    inline void addFileDecorator(const FileDecorator& decorator) noexcept {m_decorators.push_back(decorator);}

    /**
     * @brief remove a specific file decorator from the file
     * 
     * @param index the index of the file decorator to remove
     */
    inline void removeFileDecorator(size_t index) noexcept {m_decorators.erase(m_decorators.begin() + index);}

    /**
     * @brief check if the file is open
     * 
     * @return true : the file is open
     * @return false : the file is not open
     */
    inline bool isOpen() const noexcept {return (bool)m_file;}

    /**
     * @brief Get the C File handle
     * 
     * @return FILE* a pointer to the C file handle
     */
    inline FILE* getFile() const noexcept {return m_file;}

    /**
     * @brief Get the size of the file in bytes. Returns 0 if no file is open. 
     * 
     * @return uint64_t the size of the file in bytes
     */
    inline uint64_t getSize() const noexcept {return m_contents.size();}

    /**
     * @brief Get a reference to a string that contains the file's contents
     * 
     * @return String& a reference to the contents of the file
     */
    inline String& getContents() noexcept {return m_contents;}

    /**
     * @brief Get the Path to the file currently open
     * 
     * @return const std::filesystem::path& the path to the current file. If no file is currently open, it returns an empty path. 
     */
    inline const std::filesystem::path& getPath() const noexcept {return m_path;}

    /**
     * @brief get if the file was updated by some program
     * 
     * @return true : the file's contents were updated
     * @return false : no content update occurred
     */
    inline bool didContentsChange() const noexcept {return (m_file) ? (m_lastUpdate < std::filesystem::last_write_time(m_path)) : false;}

protected:

    /**
     * @brief store the C file handle
     */
    FILE* m_file = NULL;
    /**
     * @brief store the path to the actual file
     */
    std::filesystem::path m_path;
    /**
     * @brief get the time the file was updated last
     */
    std::filesystem::file_time_type m_lastUpdate;
    /**
     * @brief store the whole file contents
     */
    String m_contents;
    /**
     * @brief store a list of all decorators to apply to the file
     */
    std::vector<FileDecorator> m_decorators;

};

#endif

#endif