/**
 * @file File.cpp
 * @author DM8AT
 * @brief implement the file API
 * @version 0.1
 * @date 2025-09-30
 * 
 * @copyright Copyright (c) 2025
 * 
 */

//include the file API
#include "File.h"
//include threads to let windows sleep a short time to release the file handle
#include <thread>

File::~File()
{
    //if a file exists, close it
    if (m_file) {
        fclose(m_file); 
        m_file = nullptr;
    }
}

bool File::open(const std::filesystem::path& path, bool allowCreate)
{
    //if a file is open, close it
    close();

    //store the file size
    uint64_t fileSize = 0;
    //if the file exists, load its contents
    if (std::filesystem::is_regular_file(path))
    {
        //open the file
        m_file = fopen(path.string().c_str(), "rb");
        //check for success (this SHOULD NOT FAIL)
        if (!m_file) {return false;}

        //cache the file size
        fseek(m_file, 0, SEEK_END);
        fileSize = ftell(m_file);
        fseek(m_file, 0, SEEK_SET);
        //read the whole file
        m_contents.resize(fileSize);
        fread(m_contents.data(), sizeof(char), fileSize, m_file);
            
        //close the file again
        fclose(m_file);
        m_file = nullptr;
    }

    //open the file
    m_file = fopen(path.string().c_str(), (allowCreate) ? "wb+" : "rb+");
    //sanity check
    if (!m_file) {return false;}

    //store the file path
    m_path = path;
    //write back the old contents (if they exist)
    if (fileSize > 0) {fwrite(m_contents.data(), sizeof(char), m_contents.size(), m_file);fflush(m_file);}
    //store the last update time of the file
    m_lastUpdate = std::filesystem::last_write_time(m_path);

    //iterate over all decorators and call the open function if it exists
    String str = path.string();
    for (size_t i = 0; i < m_decorators.size(); ++i) {if (m_decorators[i].onOpen) {(*m_decorators[i].onOpen)(&str, m_decorators[i].userData);}}

    //iterate over all decorators and call the open function if it exists
    for (size_t i = 0; i < m_decorators.size(); ++i) {if (m_decorators[i].preprocessor) {(*m_decorators[i].preprocessor)(&m_contents, m_decorators[i].userData);}}

    //success
    return true;
}

void File::close()
{
    //check if a file is open
    if (!isOpen()) {return;}

    //save the contents
    save();
    
    //iterate over all decorators and call the open function if it exists
    String str = m_path.string();
    for (size_t i = 0; i < m_decorators.size(); ++i) {if (m_decorators[i].onClose) {(*m_decorators[i].onClose)(&str, m_decorators[i].userData);}}

    //then, close the filestream
    fclose(m_file);

    //reset all attributes
    m_file = nullptr;
    m_contents = "";
    m_lastUpdate = std::filesystem::file_time_type();
    m_path = "";
}

void File::save()
{
    //if no file is open, stop
    if (!isOpen()) {return;}

    //iterate over all decorators and call the open function if it exists
    for (size_t i = m_decorators.size(); i > 0; --i) {if (m_decorators[i-1].postprocessor) {(*m_decorators[i-1].postprocessor)(&m_contents, m_decorators[i-1].userData);}}

    //write all contents to the file
    fseek(m_file, 0, SEEK_SET);
    fwrite(m_contents.data(), sizeof(char), m_contents.size(), m_file);
    //make sure to flush the file
    fflush(m_file);
    //store the new update time
    m_lastUpdate = std::filesystem::last_write_time(m_path);
}

bool File::reload()
{
    //if no file is open, stop
    if (!isOpen()) {return false;}

    //close the old stream without saving
    fclose(m_file);
    m_file = nullptr;
    //let the file lock release
    std::this_thread::sleep_for(std::chrono::milliseconds(1));

    //just open the file and return if it was successful
    return open(m_path, false);
}