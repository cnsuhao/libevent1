#ifndef PLATFORMLOGCHANNEL_H
#define PLATFORMLOGCHANNEL_H

#include <list>
#include <Platform/Include/PlatformCommonEnums.h>

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


namespace CommunicationPlatform
{
    class ModuleDestinationInfo;

    class PlatformLogChannel
        /// The base class for all Channel classes.
    {
    public:
        PlatformLogChannel();
        /// Creates the channel and initializes

        virtual ~PlatformLogChannel();


        virtual bool Open();
        /// Does whatever is necessary to open the channel. 
        /// The default implementation does nothing.

        virtual void Close();
        /// Does whatever is necessary to close the channel.
        /// The default implementation does nothing.

        void AddChainingChannel(PlatformLogChannel* pChainedChannel);
        ///Add a channel to the chaining channels of this channel 
        
        void DoLogString(LoggingLevel logLevel, const char* stringToBeLogged, uint32_t lenOfStringToBeLoggedInByte) ; 
        /// Logs the given string to the channel.
        
        void DoLogDumpBin(LoggingLevel logLevel, const char* stringToBeLogged, uint32_t lenOfStringToBeLoggedInByte, const void* pData, uint32_t lenOfDataInByte);
        /// Logs the given string and dump the bin buffer to the channel.
    protected:
        virtual void logString(LoggingLevel logLevel, const char* stringToBeLogged, uint32_t lenOfStringToBeLoggedInByte) = 0 ;
        /// Logs the given string to the channel. Must be
        /// overridden by subclasses.

        virtual void logDumpBin(LoggingLevel logLevel, const char* stringToBeLogged, uint32_t lenOfStringToBeLoggedInByte, const void* pData, uint32_t lenOfDataInByte) = 0;
        /// Logs the given string and dump the bin buffer to the channel. Must be
        /// overridden by subclasses.

        std::list<PlatformLogChannel*> m_ChainingChannels;
        /// Chaining channels will continue logging the string or dumped bin after this channel has finished logging

        void LogStringToChainingChannels(LoggingLevel logLevel, const char* stringToBeLogged, uint32_t lenOfStringToBeLoggedInByte);
        /// Log string to the chaining channels in m_ChainingChannels

        void LogDumpBinToChainingChannels(LoggingLevel logLevel, const char* stringToBeLogged, uint32_t lenOfStringToBeLoggedInByte, const void* pData, uint32_t lenOfDataInByte);
        /// Log dump bin to the chaining channels in m_ChainingChannels

    private:
        PlatformLogChannel(const PlatformLogChannel&);
        PlatformLogChannel& operator = (const PlatformLogChannel&);
    };

}



#endif

