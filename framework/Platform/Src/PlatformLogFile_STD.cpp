#include <iostream>

#include <Platform/Include/PlatformLogFile_STD.h>


namespace CommunicationPlatform
{
    PlatformFileImplementation::PlatformFileImplementation()
    {

    }

    PlatformFileImplementation::~PlatformFileImplementation()
    {
    }

    bool PlatformFileImplementation::Open(std::string& path)
    {
        this->m_FileWriteStream.open(path.c_str(),std::ios_base::out | std::ios_base::trunc);
        //this->m_FileWriteStream.open(path.c_str(),std::ios_base::out| std::ios_base::app);
        

        if (true == this->m_FileWriteStream.fail())
        {
            return false;
        }
        else
        {
            return true;
        }
    }

    void PlatformFileImplementation::Close()
    {
        if (true == this->m_FileWriteStream.is_open())
        {
            this->m_FileWriteStream.close();
        }
    }

    void PlatformFileImplementation::Write(const char* pStringToWrite, uint32_t lenOfStringToWrite)
    {
        this->m_FileWriteStream.write(pStringToWrite,lenOfStringToWrite);
    }

    /**< Get size of the file */
    uint64_t PlatformFileImplementation::Size()
    {
     	 
        std::streampos fileSize = this->m_FileWriteStream.tellp();

        return (uint64_t)fileSize;
    }

    /**< Rotate the file write cursor to the beginning of the file */
    void PlatformFileImplementation::RotateToFileBeginning()
    {
        this->m_FileWriteStream.seekp(0,std::ios_base::beg);
    }
}
