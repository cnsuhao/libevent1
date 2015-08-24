#ifndef PLATFORMASYNCLOGCHANNEL_H
#define PLATFORMASYNCLOGCHANNEL_H

#include <Platform/Include/PlatformLogChannel.h>
#include <Platform/Include/PlatformCommonClass.h>
#include <Platform/Include/PlatformModule.h>
#include <Platform/Include/InterTaskMessageBase.h>


namespace CommunicationPlatform
{
    class PlatformAsyncLogChannel: public PlatformLogChannel,public MessageTriggeredModuleRunner
    {
    public:
        PlatformAsyncLogChannel(uint32_t asyncTaskStackSize);
        
        virtual ~PlatformAsyncLogChannel();

        virtual RunnerInitResult InitRunner();

        virtual void OnReceivedMessage(InterTaskMessage* pReceivedMessage);

        virtual void PostReceivedMessage(InterTaskMessage* pReceivedMessage);

        virtual bool Open();

        void AddHookedChannel(PlatformLogChannel* pHookedChannelToAdd);
    protected:

        typedef std::list<PlatformLogChannel*> HookedChannelList;
        HookedChannelList m_HookedChannels;  /**< Hooked channel list, which do the real log output */

        uint32_t m_MaxPendingLogInQueue;     /**< The maximum allowed number of pending log in the message queue, when pending log number reaches this limit, following log will be discarded */

        uint32_t m_AsyncTaskStackSize;   

        MessageTriggeredTaskModule* m_LogModule; /**< Message triggered task module which holds the thread to print the log */

        Mutex m_HookedChannelProtectingMutex;

        virtual void logString(LoggingLevel logLevel, const char* stringToBeLogged, uint32_t lenOfStringToBeLoggedInByte);
        /// Logs the given string to the channel. Must be
        /// overridden by subclasses.

        virtual void logDumpBin(LoggingLevel logLevel, const char* stringToBeLogged, uint32_t lenOfStringToBeLoggedInByte, const void* pData, uint32_t lenOfDataInByte);
        /// Logs the given string and dump the bin buffer to the channel. Must be
        /// overridden by subclasses.

    };

    class AsyncLogChannelMessage: public InterModuleMessageBase
    {
    public:
        AsyncLogChannelMessage();
        ~AsyncLogChannelMessage();

        LoggingLevel m_LogLevel;
        char*        m_LogString;
        uint32_t     m_LenOfStringToBeLoggedInByte;

        void*        m_DumpedBinData;       /**< Only when there is dumped bin data, this pointer is not NULL, otherwise it is just NULL */
        uint32_t     m_LenOfDumpedBinData;  /**< When m_DumpedBinData is not NULL, set to length of the data pointed by m_DumpedBinData, otherwise it is 0 */
        InterModuleMessageBase* Clone(){return new AsyncLogChannelMessage(*this);}
    };

}

#endif