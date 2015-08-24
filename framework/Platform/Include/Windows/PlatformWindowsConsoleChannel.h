#ifndef PLATFORMWINDOWSCONSOLECHANNEL_H
#define PLATFORMWINDOWSCONSOLECHANNEL_H

#include <Platform/Include/Windows/WindowsSpecificHeaders.h>
#include <Platform/Include/PlatformLogChannel.h>

namespace CommunicationPlatform
{
    class PlatformWindowsConsoleChannel: public PlatformLogChannel
    {
    public:
        PlatformWindowsConsoleChannel();

        virtual ~PlatformWindowsConsoleChannel();
    protected:
        virtual void logString(LoggingLevel logLevel, const char* stringToBeLogged, uint32_t lenOfStringToBeLoggedInByte);
        /// Logs the given string to the channel. Must be
        /// overridden by subclasses.

        virtual void logDumpBin(LoggingLevel logLevel, const char* stringToBeLogged, uint32_t lenOfStringToBeLoggedInByte, const void* pData, uint32_t lenOfDataInByte);
        /// Logs the given string and dump the bin buffer to the channel. Must be
        /// overridden by subclasses.
    private:
        PlatformWindowsConsoleChannel(const PlatformWindowsConsoleChannel&){};
        PlatformWindowsConsoleChannel& operator = (const PlatformWindowsConsoleChannel&){};

        void SetConsoleColor(LoggingLevel logLevel);

        void WriteDataToConsole(const char* pDataToWrite, uint32_t lengthOfDataToWrite);

        HANDLE m_HandleOfConsole;  
        bool   m_isFile;
    };
}


#endif