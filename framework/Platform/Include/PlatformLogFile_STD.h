#ifndef PLATFORMLOGFILE_STD_H
#define PLATFORMLOGFILE_STD_H

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

#include <string>
#include <fstream>

namespace CommunicationPlatform
{
    class PlatformFileImplementation
    {
    public:
        PlatformFileImplementation();
        virtual ~PlatformFileImplementation();

        bool Open(std::string& path);

        void Close();

        void Write(const char* pStringToWrite, uint32_t lenOfStringToWrite);

        uint64_t Size();  /**< Get size of the file */

    protected:
        void RotateToFileBeginning();   /**< Rotate the file write cursor to the beginning of the file */

        std::ofstream m_FileWriteStream;  /**< ostream to write the file */
    };
}


#endif