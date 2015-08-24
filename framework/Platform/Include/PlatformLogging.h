#ifndef PLATFORMLOGGING_H
#define PLATFORMLOGGING_H

#include <string>
#include <map>
#include <Platform/Include/PlatformLogChannel.h>
#include <Platform/Include/PlatformMutex.h>

namespace CommunicationPlatform
{
    class LoggerInfo
    {
    public:
        std::string m_Name;              /**< Name of the Logger */
        LoggingLevel m_CurrentLogLevel;  /**< Current Level of this Logger */
    };

    /**
    *   
    */
    class Logger
    {
    public:

        class HeaderTextAppendHelper
        {
        public:
            HeaderTextAppendHelper(Logger& loggerToControl,const std::string& textToAppendToHeader)
            {
                this->m_LoggerToControl = &loggerToControl;

                this->m_LoggerToControl->SetHeaderAppendingText(textToAppendToHeader);
            }

            ~HeaderTextAppendHelper()
            {
                this->m_LoggerToControl->SetHeaderAppendingText("");
            }

        protected:
            Logger* m_LoggerToControl;
        private:
            HeaderTextAppendHelper();
            HeaderTextAppendHelper(const HeaderTextAppendHelper&);            
            HeaderTextAppendHelper& operator=(const HeaderTextAppendHelper&);
        };

        Logger();
        virtual ~Logger();

        const std::string& GetName() const;

        void SetName(std::string nameToBeSet);

        void SetChannel(PlatformLogChannel* pChannel);

        PlatformLogChannel* GetChannel() const;

        void SetLevel(LoggingLevel level);

        LoggingLevel GetLevel() const;

        void DoLog(LoggingLevel logLevel,const char* fileName, int lineNumber, const char* pFormatString, ...) const;

        void DumpData(LoggingLevel logLevel,const char* fileName, int lineNumber, const void* pBuffer, std::size_t bufferLength, const char* pFormatString, ...) const;

        bool WhetherLogLevelIsAllowed(LoggingLevel level) const;

        void SetHeaderAppendingText(const std::string& textToAppend); /**< Set text to be appended to header */

        static void GetLoggersInfo(std::list<LoggerInfo>& outputLoggersInfoList);

        static Logger* Get(const std::string& name = "");

        static Logger& root();

        static const std::string ROOT; /// The name of the root logger ("").
    protected:
        PlatformLogChannel*     m_AttachedChannel;          /**< Log output channel attached to this logger, the logger's log will output to this channel¡¡*/

        std::string  m_Name;                     /**< Name of this logger */

        std::string  m_ContentAppendToHeader;    /**< Content which is appended to the Header text */

        LoggingLevel m_CurrentLogLevel;          /**< Current Log level, any log request whose log level is greater than current log level is ignored */

        void GenerateLogHeader(std::string& outputHeader,LoggingLevel logLevel, const char* fileName, int lineNumber) const; /**< Generate log header for each line */

        void PrepareLocalLogBuffer(char* localLogBuffer, uint32_t localLogBufferLength, int& actualLogLength,LoggingLevel logLevel,const char* fileName, int lineNumber, const char* pFormatString,va_list args) const;

        static uint64_t gGlobalLogIndex;

        static Mutex m_GlobalLoggerMapMutex;     /**< Mutex to protext Global Logger map */

        typedef std::map<std::string,Logger*> LoggerMap;

        static LoggerMap m_LoggerManagementMap;  /**< Management map to store the global logger using logger name as the key */

        static Logger* FindLoggerByName(const std::string& loggerName);         /**< Find the logger by its name, return NULL if doesn't find the named logger */

        static Logger& FindParentByName(const std::string& loggerName);         /**< Find the parent logger of the logger specified by loggerName */

        static Logger* GetWithLockManagementMap(const std::string& loggerName); /**< Get the Logger without using lock to protect m_LoggerManagementMap */

        static void AddLoggerToManagementMap(Logger* loggerToBeAdded);    /**< Add the loggerToBeAdded to m_LoggerManagementMap */
    };

    /**
    *   Convenient printf-like logging macros 
    */
    #define Log_printf(logger, logLevel,formatString,...)\
                if ((logger!=NULL) && (logger)->WhetherLogLevelIsAllowed(logLevel)) (logger)->DoLog(logLevel,__THIS_FILE_NAME__ ,__LINE__,formatString,## __VA_ARGS__); else (void) 0

    #define Log_dumpBin(logger, logLevel, pDataBuffer, DataBufferLenInByte, formatString, ...)\
                if ((logger!=NULL) && (logger)->WhetherLogLevelIsAllowed(logLevel)) (logger)->DumpData(logLevel,__THIS_FILE_NAME__ ,__LINE__,pDataBuffer, DataBufferLenInByte, formatString,## __VA_ARGS__); else (void) 0
}


#endif

