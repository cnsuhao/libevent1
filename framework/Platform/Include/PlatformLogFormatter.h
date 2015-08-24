#ifndef PLATFORMLOGFORMATTER_H
#define PLATFORMLOGFORMATTER_H

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

#include <sstream>

namespace CommunicationPlatform
{
    class PlatformLogFormatter
    {
    public:
        static void FormatBinaryBufferToString(std::ostringstream& outputStream, const void* pData, uint32_t lenOfDataInByte);

        static void SetMaxBufferSizeToConvertToString(uint32_t maxValue);

        static uint32_t GetMaxBufferSizeToConvertToString();

        static void GetCurrentTimeStamp(std::ostringstream& outputStream);

    protected:
        static uint32_t gMaxBufferSizeToConvertToString;
    };

}



#endif