#ifndef PLATFORMFILECHANNEL_H
#define PLATFORMFILECHANNEL_H

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

#include <Platform/Include/PlatformLogChannel.h>
#include <Platform/Include/PlatformLogFile.h>

namespace CommunicationPlatform
{
    class PlatformFileChannel: public PlatformLogChannel
    {
    public:
        PlatformFileChannel();
        PlatformFileChannel(uint32_t fileSizeLimit);
        ~PlatformFileChannel();

        bool Open();

        void Close();
    protected:
        virtual void logString(LoggingLevel logLevel, const char* stringToBeLogged, uint32_t lenOfStringToBeLoggedInByte);

        virtual void logDumpBin(LoggingLevel logLevel, const char* stringToBeLogged, uint32_t lenOfStringToBeLoggedInByte, const void* pData, uint32_t lenOfDataInByte);

        PlatformLogFile m_FileToWriteLog;
    };
}


#endif