#include <Platform/Include/PlatformAsyncLogChannel.h>
#include <Platform/Include/PlatformCommonMacros.h>

namespace CommunicationPlatform
{
#define DEFAULT_MAX_PENDING_LOG_NUMBER (2000)
    PlatformAsyncLogChannel::PlatformAsyncLogChannel(uint32_t asyncTaskStackSize)
        :m_MaxPendingLogInQueue(DEFAULT_MAX_PENDING_LOG_NUMBER),
        m_AsyncTaskStackSize(asyncTaskStackSize),
        m_LogModule(NULL),
        m_HookedChannelProtectingMutex(),
        PlatformLogChannel(),
        MessageTriggeredModuleRunner("Platform Async Log Channel Runner")
    {
    
    }

    PlatformAsyncLogChannel::~PlatformAsyncLogChannel()
    {

    }

    ModuleRunner::RunnerInitResult PlatformAsyncLogChannel::InitRunner()
    {
        return RUNNER_INIT_SUCCESS;
    }

    void PlatformAsyncLogChannel::OnReceivedMessage(InterTaskMessage* pReceivedMessage)
    {
        Mutex::ScopedMutex hookedChannelProtectLock(m_HookedChannelProtectingMutex);

        HookedChannelList::iterator hookedChannelItor = this->m_HookedChannels.begin();

		AsyncLogChannelMessage* pLogChannelMessage = static_cast<AsyncLogChannelMessage*>(pReceivedMessage->m_MessageData.pMessageDataNew);

        for (;hookedChannelItor!=this->m_HookedChannels.end();++hookedChannelItor)
        {
            PlatformLogChannel*& currentHookedChannel = *hookedChannelItor;

            if (NULL == pLogChannelMessage->m_DumpedBinData)
            {
                currentHookedChannel->DoLogString(pLogChannelMessage->m_LogLevel,
                    pLogChannelMessage->m_LogString,
                    pLogChannelMessage->m_LenOfStringToBeLoggedInByte);
            } 
            else
            {
                currentHookedChannel->DoLogDumpBin(pLogChannelMessage->m_LogLevel,
                    pLogChannelMessage->m_LogString,
                    pLogChannelMessage->m_LenOfStringToBeLoggedInByte,
                    pLogChannelMessage->m_DumpedBinData,
                    pLogChannelMessage->m_LenOfDumpedBinData);
            }
        }

		free(pLogChannelMessage->m_LogString);

		if (NULL != pLogChannelMessage->m_DumpedBinData)
		{
			free(pLogChannelMessage->m_DumpedBinData);
		}
    }

    void PlatformAsyncLogChannel::PostReceivedMessage(InterTaskMessage* pReceivedMessage)
    {
        USE_UNFERENCED_PARAMETER(pReceivedMessage);
    }

    /**
    *   -# Start the message triggered module
    *   -# 
    */
    bool PlatformAsyncLogChannel::Open()
    {
        this->m_LogModule = new MessageTriggeredTaskModule(PLATFORM_INTERNAL_NODE_THIS_NODE,PLATFORM_INTERNAL_MODULE_ASYNC_LOG_CHANNEL,TASK_PRIORITY_LOW,m_AsyncTaskStackSize,"Platform Async Log Channel Module",this);

        if (NULL == this->m_LogModule)
        {
            return false;
        }

        ModuleCreateResult moduleStartResult = m_LogModule->Start();

        if (TASK_CREATE_RESULT_SUCCESS == moduleStartResult)
        {
            return true;
        } 
        else
        {
            delete m_LogModule;
            m_LogModule = NULL;

            return false;
        }
    }

    void PlatformAsyncLogChannel::AddHookedChannel(PlatformLogChannel* pHookedChannelToAdd)
    {
        Mutex::ScopedMutex hookedChannelProtectLock(m_HookedChannelProtectingMutex);

        this->m_HookedChannels.push_back(pHookedChannelToAdd);
    }


    /**
    *   -# Send the log message to the destination module
    */
    void PlatformAsyncLogChannel::logString(LoggingLevel logLevel, const char* stringToBeLogged, uint32_t lenOfStringToBeLoggedInByte)
    {
        if (this->m_MessageQueue.size() >= m_MaxPendingLogInQueue)
        {
            return;
        }

		AsyncLogChannelMessage* pLogMessage = new(std::nothrow) AsyncLogChannelMessage();

        if (NULL == pLogMessage)
        {
            return;
        }

        pLogMessage->m_LogLevel = logLevel;
        pLogMessage->m_LogString = (char*)malloc(lenOfStringToBeLoggedInByte);

        if (NULL == pLogMessage->m_LogString)
        {
            delete pLogMessage;
            return;
        }

        memcpy(pLogMessage->m_LogString,stringToBeLogged,lenOfStringToBeLoggedInByte);

        pLogMessage->m_LenOfStringToBeLoggedInByte = lenOfStringToBeLoggedInByte;


        InterTaskMessage* pMessageToSend = new(std::nothrow) InterTaskMessage(PLATFORM_MESSAGE_ASYNC_LOG_MESSAGE_IND,
                                                                sizeof(AsyncLogChannelMessage),
                                                                PLATFORM_INTERNAL_NODE_THIS_NODE,
                                                                PLATFORM_INTERNAL_MODULE_ASYNC_LOG_CHANNEL,
                                                                InterTaskMessage::MESSAGE_DATA_MEMORY_TYPE_NEW);

        if (NULL == pMessageToSend)
        {
            free(pLogMessage->m_LogString);
            delete pLogMessage;
            return;
        }

        pMessageToSend->m_MessageData.pMessageDataNew = pLogMessage;

        this->m_MessageQueue.enqueueNotification(pMessageToSend);
    }

    /**
    *   -# Send the log message to the destination module
    */
    void PlatformAsyncLogChannel::logDumpBin(LoggingLevel logLevel, const char* stringToBeLogged, uint32_t lenOfStringToBeLoggedInByte, const void* pData, uint32_t lenOfDataInByte)
    {
        if (this->m_MessageQueue.size() >= m_MaxPendingLogInQueue)
        {
            return;
        }

        AsyncLogChannelMessage* pLogMessage = new(std::nothrow) AsyncLogChannelMessage();

        if (NULL == pLogMessage)
        {
            return;
        }

        pLogMessage->m_LogLevel = logLevel;
        pLogMessage->m_LogString = (char*)malloc(lenOfStringToBeLoggedInByte);

        if (NULL == pLogMessage->m_LogString)
        {
            delete pLogMessage;
            return;
        }

        memcpy(pLogMessage->m_LogString,stringToBeLogged,lenOfStringToBeLoggedInByte);

        pLogMessage->m_LenOfStringToBeLoggedInByte = lenOfStringToBeLoggedInByte;

        pLogMessage->m_DumpedBinData = malloc(lenOfDataInByte);

        if (NULL == pLogMessage->m_DumpedBinData)
        {
            free(pLogMessage->m_LogString);
            delete pLogMessage;
            return;
        }

        memcpy(pLogMessage->m_DumpedBinData,pData,lenOfDataInByte);

        pLogMessage->m_LenOfDumpedBinData = lenOfDataInByte;


        InterTaskMessage* pMessageToSend = new(std::nothrow) InterTaskMessage(PLATFORM_MESSAGE_ASYNC_LOG_MESSAGE_IND,
            sizeof(AsyncLogChannelMessage),
            PLATFORM_INTERNAL_NODE_THIS_NODE,
            PLATFORM_INTERNAL_MODULE_ASYNC_LOG_CHANNEL,
            InterTaskMessage::MESSAGE_DATA_MEMORY_TYPE_NEW);

        if (NULL == pMessageToSend)
        {
            free(pLogMessage->m_DumpedBinData);
            free(pLogMessage->m_LogString);
            delete pLogMessage;
            return;
        }

        pMessageToSend->m_MessageData.pMessageDataNew = pLogMessage;

        this->m_MessageQueue.enqueueNotification(pMessageToSend);
    
    }

    AsyncLogChannelMessage::AsyncLogChannelMessage()
        :m_LogLevel(LOGGING_LEVEL_NONE),
        m_LogString(NULL),
        m_LenOfStringToBeLoggedInByte(0),
        m_DumpedBinData(NULL),
        m_LenOfDumpedBinData(0)
    {
    
    }

    AsyncLogChannelMessage::~AsyncLogChannelMessage()
    {
    
    }
}