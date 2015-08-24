#include <Platform/Include/PlatformLogFile.h>


namespace CommunicationPlatform
{
    /**< Constructor without file size limit */
    PlatformLogFile::PlatformLogFile()
        :PlatformFileImplementation(),
        m_WhetherHasFileSizeLimit(false),
        m_FileSizeLimit(0)
    {

    }

    /**< Constructor with file size limit */
    PlatformLogFile::PlatformLogFile(uint32_t fileSizeLimit)
        :PlatformFileImplementation(),
        m_WhetherHasFileSizeLimit(true),
        m_FileSizeLimit(fileSizeLimit)
    {

    }

    PlatformLogFile::~PlatformLogFile()
    {

    }

    bool PlatformLogFile::Open(std::string& path)
    {
        return PlatformFileImplementation::Open(path);
    }

    void PlatformLogFile::Close()
    {
        PlatformFileImplementation::Close();
    }

    void PlatformLogFile::Write(const char* pStringToWrite, uint32_t lenOfStringToWrite)
    {
        if ( (true == this->m_WhetherHasFileSizeLimit) && (Size()>=m_FileSizeLimit) )
        {
            RotateToFileBeginning();
        }

        PlatformFileImplementation::Write(pStringToWrite,lenOfStringToWrite);

    }

    /**< Get size of the file */
    uint64_t PlatformLogFile::Size()
    {
        return PlatformFileImplementation::Size();
    }

    /**  
    *  Rotate the file write cursor to the beginning of the file 
    */
    void PlatformLogFile::RotateToFileBeginning()
    {
        PlatformFileImplementation::RotateToFileBeginning();
    }
}