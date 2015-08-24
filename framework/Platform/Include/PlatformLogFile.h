#ifndef PLATFORMFILE_H
#define PLATFORMFILE_H

#ifdef _MSC_VER
    #include <Platform/Include/Windows/WindowsSpecificHeaders.h>
#else
    #ifdef linux
        #include <Platform/Include/Linux/LinuxSpecificHeaders.h>
    #else
        #ifdef __TI_COMPILER_VERSION__
            #include <Platform/Include/SysBios/SysBiosSpecificHeaders.h>
        #else
            #error Not supported Platform!
        #endif
    #endif    
#endif

#include <Platform/Include/PlatformLogFile_STD.h>

namespace CommunicationPlatform
{
    class PlatformLogFile: public PlatformFileImplementation
    {
    public:
        PlatformLogFile();  /**< Constructor without file size limit */
        PlatformLogFile(uint32_t fileSizeLimit); /**< Constructor with file size limit */

        virtual ~PlatformLogFile();

        bool Open(std::string& path);

        void Close();

        void Write(const char* pStringToWrite, uint32_t lenOfStringToWrite);

        uint64_t Size();  /**< Get size of the file */

    protected:
        bool     m_WhetherHasFileSizeLimit;
        uint32_t m_FileSizeLimit;

        void RotateToFileBeginning();   /**< Rotate the file write cursor to the beginning of the file */
    };
}

#endif