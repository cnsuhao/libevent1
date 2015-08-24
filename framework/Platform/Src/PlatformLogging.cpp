#include <sstream>
#include <Platform/Include/PlatformLogging.h>
#include <Platform/Include/PlatformEnvironmentWatcher.h>
#include <Platform/Include/PlatformInternalLogger.h>
#include <Platform/Include/debug.h>
#ifdef cdebug
#include <stdio.h>
#endif
namespace CommunicationPlatform
{
	Logger* pPlatformInternalLogger = NULL;

    uint64_t Logger::gGlobalLogIndex = 0;

    Mutex Logger::m_GlobalLoggerMapMutex;     /**< Mutex to protext Global Logger map */

    Logger::LoggerMap Logger::m_LoggerManagementMap;

    const std::string Logger::ROOT = "";


    Logger::Logger()
        :m_AttachedChannel(NULL),
        m_CurrentLogLevel(LOGGING_LEVEL_NONE)
    {
    
    }

    Logger::~Logger()
    {
    
    }

    void Logger::SetChannel(PlatformLogChannel* pChannel)
    {
        this->m_AttachedChannel = pChannel;
    }

    PlatformLogChannel* Logger::GetChannel() const
    {
        return this->m_AttachedChannel;
    }

    const std::string& Logger::GetName() const
    {
        return this->m_Name;
    }

    void Logger::SetName(std::string nameToBeSet)
    {
        this->m_Name = nameToBeSet;
    }


    void Logger::SetLevel(LoggingLevel level)
    {
        this->m_CurrentLogLevel = level;
    }

    LoggingLevel Logger::GetLevel() const
    {
        return this->m_CurrentLogLevel;
    }

#define LOCAL_LOG_BUFFER_LEN (1024)
    void Logger::DoLog(LoggingLevel logLevel,const char* fileName, int lineNumber, const char* pFormatString, ...) const
    {
        #ifdef cdebug
        //printf("debug[1]: enter dolog!\r\n");
        #endif
        char localLogBuffer[LOCAL_LOG_BUFFER_LEN]="";
        int actualStringLen = 0;

        va_list args;
        va_start(args, pFormatString);
        PrepareLocalLogBuffer(localLogBuffer,LOCAL_LOG_BUFFER_LEN,actualStringLen,logLevel,fileName,lineNumber,pFormatString,args);
        va_end(args);

        /* Output the log into the Channel */
        if (this->m_AttachedChannel != NULL)
        {
            #ifdef cdebug
        	//printf("debug[2]: this->m_AttachedChannel != NULL\r\n");
        	#endif
            this->m_AttachedChannel->DoLogString(logLevel,localLogBuffer,actualStringLen);
        }
    }

    void Logger::DumpData(LoggingLevel logLevel,const char* fileName, int lineNumber, const void* pBuffer, std::size_t bufferLength, const char* pFormatString, ...) const
    {
        char localLogBuffer[LOCAL_LOG_BUFFER_LEN]="";
        int actualStringLen = 0;

        va_list args;
        va_start(args, pFormatString);
        PrepareLocalLogBuffer(localLogBuffer,LOCAL_LOG_BUFFER_LEN,actualStringLen,logLevel,fileName,lineNumber,pFormatString,args);
        va_end(args);

        /* Output the log into the Channel */
        if (this->m_AttachedChannel != NULL)
        {
            this->m_AttachedChannel->DoLogDumpBin(logLevel,localLogBuffer,actualStringLen,pBuffer, bufferLength);
        }
    }

    void Logger::PrepareLocalLogBuffer(char* localLogBuffer, uint32_t localLogBufferLength, int& actualLogLength,LoggingLevel logLevel,const char* fileName, int lineNumber, const char* pFormatString,va_list args) const
    {
        /* Generate Header text of the log */
        std::string headerString;
        GenerateLogHeader(headerString,logLevel,fileName,lineNumber);

        /* Copy Header text into the local log buffer */
        strncpy(localLogBuffer,headerString.c_str(),localLogBufferLength);

        int remainingBufferSpace = 0;
        size_t headerLength         = headerString.length();

        if (headerLength<localLogBufferLength)
        {
            remainingBufferSpace = localLogBufferLength - headerLength;

            actualLogLength = headerLength;
        }
        else
        {
            remainingBufferSpace = 0;
            localLogBuffer[localLogBufferLength-1] = '\0';

            actualLogLength = localLogBufferLength;
        }

        if (remainingBufferSpace>0)
        {
            /* Assemble the customer log text into the local buffer */
            int vsnprintfWrittenLen = (unsigned int)vsnprintf((localLogBuffer+(localLogBufferLength-remainingBufferSpace)),remainingBufferSpace,pFormatString,args);

            if ( (vsnprintfWrittenLen>=remainingBufferSpace) || (-1 == vsnprintfWrittenLen))
            {
                actualLogLength = localLogBufferLength;
                localLogBuffer[localLogBufferLength-1] = '\0';
            }
            else
            {
                actualLogLength      += vsnprintfWrittenLen+1;
                remainingBufferSpace -= (vsnprintfWrittenLen+1);
                localLogBuffer[actualLogLength] = '\0';
            }

        }
    }


    /**
    *  Generate log header for each line 
    */
    void Logger::GenerateLogHeader(std::string& outputHeader, LoggingLevel logLevel, const char* fileName, int lineNumber) const
    {
        /* Output log Index and tick */
        std::ostringstream logIndexAndTickStream;
        logIndexAndTickStream<<"["<<gGlobalLogIndex++<<"]"<<"["<<PlatformEnvironmentWatcher::GetTick()<<"]";

        outputHeader.append(logIndexAndTickStream.str());

        /* Output the Logger's name */
        outputHeader.append("["+ this->m_Name + "]");

        /* Output the Log's Level */
        switch (logLevel)
        {
        case LOGGING_LEVEL_FATAL:       {outputHeader.append("[FATAL]"); }
        	break;
        case LOGGING_LEVEL_CRITICAL:    {outputHeader.append("[CRITICAL]"); }
            break;
        case LOGGING_LEVEL_ERROR:       {outputHeader.append("[ERROR]"); }
            break;
        case LOGGING_LEVEL_WARNING:     {outputHeader.append("[WARNING]"); }
            break;
        case LOGGING_LEVEL_NOTICE:      {outputHeader.append("[NOTICE]"); }
            break;
        case LOGGING_LEVEL_INFORMATION: {outputHeader.append("[INFORMATION]"); }
            break;
        case LOGGING_LEVEL_DEBUG:       {outputHeader.append("[DEBUG]"); }
            break;
        case LOGGING_LEVEL_TRACE:       {outputHeader.append("[TRACE]"); }
            break;
        default:
            break;
        }
        
        /* Output Filename and line number */
        std::ostringstream lineNumberStream;
        lineNumberStream<<lineNumber;

        outputHeader.append("["+ std::string(fileName)+ "("+lineNumberStream.str()+")]");

        /* Append customized Log Header */
        outputHeader.append(this->m_ContentAppendToHeader);

        outputHeader.append(": ");
    
    }

    bool Logger::WhetherLogLevelIsAllowed(LoggingLevel level) const
    {
        if ((level<=this->m_CurrentLogLevel) && (level != LOGGING_LEVEL_NONE))
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    /**
    *  Set text to be appended to header 
    */
    void Logger::SetHeaderAppendingText(const std::string& textToAppend)
    {
        this->m_ContentAppendToHeader = textToAppend;
    }

    void Logger::GetLoggersInfo(std::list<LoggerInfo>& outputLoggersInfoList)
    {
        Mutex::ScopedMutex mapProtectLock(m_GlobalLoggerMapMutex);

        for (LoggerMap::const_iterator loggerItor = m_LoggerManagementMap.begin();loggerItor!=m_LoggerManagementMap.end();++loggerItor)
        {
            Logger* const & currentLogger = loggerItor->second;

            LoggerInfo infoOfCurrentLogger;
            infoOfCurrentLogger.m_Name            = currentLogger->GetName();
            infoOfCurrentLogger.m_CurrentLogLevel = currentLogger->GetLevel();

            outputLoggersInfoList.push_back(infoOfCurrentLogger);
        }
    }


    Logger* Logger::Get(const std::string& name)
    {
        Mutex::ScopedMutex mapProtectLock(m_GlobalLoggerMapMutex);

        return GetWithLockManagementMap(name);
    }

    /**
    *   Get the Logger without using lock to protect m_LoggerManagementMap 
    */
    Logger* Logger::GetWithLockManagementMap(const std::string& loggerName)
    {
        Logger* loggerToBeGotten = FindLoggerByName(loggerName);

        if (NULL == loggerToBeGotten)
        {
            if (loggerName == ROOT)
            {
                loggerToBeGotten = new Logger();
                if (loggerToBeGotten != NULL)
                {
                    loggerToBeGotten->SetName(loggerName);
                    loggerToBeGotten->SetChannel(NULL);
                    loggerToBeGotten->SetLevel(LOGGING_LEVEL_INFORMATION);
                }
            }
            else
            {
                Logger& parentLogger = FindParentByName(loggerName);

                loggerToBeGotten = new Logger();
                if (loggerToBeGotten != NULL)
                {
                    loggerToBeGotten->SetName(loggerName);
                    loggerToBeGotten->SetChannel(parentLogger.GetChannel());
                    loggerToBeGotten->SetLevel(parentLogger.GetLevel());
                }
            }
            AddLoggerToManagementMap(loggerToBeGotten);
        }

        return loggerToBeGotten;
    }


    /**
    *  Find the logger by its name, return NULL if doesn't find the named logger 
    */
    Logger* Logger::FindLoggerByName(const std::string& loggerName)
    {
        LoggerMap::iterator foundLogger = m_LoggerManagementMap.find(loggerName);

        /* Found the logger */
        if (foundLogger != m_LoggerManagementMap.end())
        {
            return foundLogger->second;
        }
        else
        {
            return NULL;
        }
    }

    /**
    *  Find the parent logger of the logger specified by loggerName 
    */
    Logger& Logger::FindParentByName(const std::string& loggerName)
    {
        std::string::size_type pos = loggerName.rfind('.');
        if (pos != std::string::npos)
        {
            std::string parentLoggerName = loggerName.substr(0, pos);
            Logger* pParent = FindLoggerByName(parentLoggerName);
            if (pParent != NULL)
            {
                return *pParent;
            }
            else
            {
                return FindParentByName(parentLoggerName);
            }
        }
        else
        {
            return *GetWithLockManagementMap(ROOT);
        }
    }

    /**
    *  Add the loggerToBeAdded to m_LoggerManagementMap 
    */
    void Logger::AddLoggerToManagementMap(Logger* loggerToBeAdded)
    {
        m_LoggerManagementMap.insert(LoggerMap::value_type(loggerToBeAdded->GetName(),loggerToBeAdded));
    }

    Logger& Logger::root()
    {
        return *Get(ROOT);
    }



}
