#include <Platform/Include/PlatformLogChannel.h>
#include <Platform/Include/debug.h>

namespace CommunicationPlatform
{
    PlatformLogChannel::PlatformLogChannel()
    {
    
    }

    /// Does whatever is necessary to open the channel. 
    /// The default implementation does nothing.
    bool PlatformLogChannel::Open()
    {
        return true;
    }
    
    /// Does whatever is necessary to close the channel.
    /// The default implementation does nothing.
    void PlatformLogChannel::Close()
    {

    }

    PlatformLogChannel::~PlatformLogChannel()
    {

    }


    ///Add a channel to the chaining channels of this channel 
    void PlatformLogChannel::AddChainingChannel(PlatformLogChannel* pChainedChannel)
    {
        this->m_ChainingChannels.push_back(pChainedChannel);
    }

    /// Logs the given string to the channel.
    void PlatformLogChannel::DoLogString(LoggingLevel logLevel, const char* stringToBeLogged, uint32_t lenOfStringToBeLoggedInByte) 
    {
        /* First log string to this channel */
        logString(logLevel,stringToBeLogged,lenOfStringToBeLoggedInByte);

        /* Log the string to the chaining channels */
        LogStringToChainingChannels(logLevel,stringToBeLogged,lenOfStringToBeLoggedInByte);
    }

    /// Log string to the chaining channels in m_ChainingChannels
    void PlatformLogChannel::LogStringToChainingChannels(LoggingLevel logLevel, const char* stringToBeLogged, uint32_t lenOfStringToBeLoggedInByte)
    {
        std::list<PlatformLogChannel*>::const_iterator chainChannelItor = this->m_ChainingChannels.begin();

        for (;chainChannelItor!=this->m_ChainingChannels.end();++chainChannelItor)
        {
            PlatformLogChannel* const& currentChannel = *chainChannelItor;

            currentChannel->DoLogString(logLevel,stringToBeLogged,lenOfStringToBeLoggedInByte);
        }
    }

    /// Logs the given string and dump the bin buffer to the channel.
    void PlatformLogChannel::DoLogDumpBin(LoggingLevel logLevel, const char* stringToBeLogged, uint32_t lenOfStringToBeLoggedInByte, const void* pData, uint32_t lenOfDataInByte)
    {
        /* First log the dumped bin in this channel */
        logDumpBin(logLevel,stringToBeLogged,lenOfStringToBeLoggedInByte,pData,lenOfDataInByte);

        /* Log the dumped bing to the chaining channels */
        LogDumpBinToChainingChannels(logLevel,stringToBeLogged,lenOfStringToBeLoggedInByte,pData,lenOfDataInByte);
    }

    /// Log dump bin to the chaining channels in m_ChainingChannels
    void PlatformLogChannel::LogDumpBinToChainingChannels(LoggingLevel logLevel, const char* stringToBeLogged, uint32_t lenOfStringToBeLoggedInByte, const void* pData, uint32_t lenOfDataInByte)
    {
        std::list<PlatformLogChannel*>::iterator chainChannelItor = this->m_ChainingChannels.begin();

        for (;chainChannelItor!=this->m_ChainingChannels.end();++chainChannelItor)
        {
            PlatformLogChannel*& currentChannel = *chainChannelItor;

            currentChannel->DoLogDumpBin(logLevel,stringToBeLogged,lenOfStringToBeLoggedInByte,pData,lenOfDataInByte);
        }
    }
    

    
}
