#include <Platform/Include/Windows/PlatformWindowsConsoleChannel.h>
#include <Platform/Include/PlatformLogFormatter.h>

namespace CommunicationPlatform
{
    PlatformWindowsConsoleChannel::PlatformWindowsConsoleChannel()
        :PlatformLogChannel(),
         m_isFile(false),
         m_HandleOfConsole(INVALID_HANDLE_VALUE)
    {
        this->m_HandleOfConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        // check whether the console has been redirected
        DWORD mode;	
        this->m_isFile = (GetConsoleMode(m_HandleOfConsole, &mode) == 0);
    }

    PlatformWindowsConsoleChannel::~PlatformWindowsConsoleChannel()
    {
    
    }

    void PlatformWindowsConsoleChannel::logString(LoggingLevel logLevel, const char* stringToBeLogged, uint32_t lenOfStringToBeLoggedInByte)
    {
        SetConsoleColor(logLevel);

        WriteDataToConsole(stringToBeLogged,lenOfStringToBeLoggedInByte-1);

        WriteDataToConsole("\r\n",2);
    }

    void PlatformWindowsConsoleChannel::logDumpBin(LoggingLevel logLevel, const char* stringToBeLogged, uint32_t lenOfStringToBeLoggedInByte, const void* pData, uint32_t lenOfDataInByte)
    {
        SetConsoleColor(logLevel);

        WriteDataToConsole(stringToBeLogged,lenOfStringToBeLoggedInByte-1);

        std::ostringstream binDataText;

        PlatformLogFormatter::FormatBinaryBufferToString(binDataText,pData,lenOfDataInByte);

        std::string formattedString = binDataText.str();
        WriteDataToConsole(formattedString.data(),formattedString.size());
    
        WriteDataToConsole("\r\n",2);
    }

    void PlatformWindowsConsoleChannel::SetConsoleColor(LoggingLevel logLevel)
    {
        switch(logLevel)
        {
        case LOGGING_LEVEL_FATAL:
        case LOGGING_LEVEL_CRITICAL:
            {
                SetConsoleTextAttribute(m_HandleOfConsole,FOREGROUND_RED|FOREGROUND_INTENSITY);
            }
            break;
        case LOGGING_LEVEL_ERROR:
            {
                SetConsoleTextAttribute(m_HandleOfConsole,FOREGROUND_RED| FOREGROUND_BLUE|FOREGROUND_INTENSITY);
            }
            break;
        case LOGGING_LEVEL_WARNING:
            {
                SetConsoleTextAttribute(m_HandleOfConsole,FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_INTENSITY);
            }
            break;
        case LOGGING_LEVEL_NOTICE:
            {
                SetConsoleTextAttribute(m_HandleOfConsole,FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_INTENSITY);
            }
            break;
        case LOGGING_LEVEL_INFORMATION:
            {
                SetConsoleTextAttribute(m_HandleOfConsole,FOREGROUND_GREEN|FOREGROUND_INTENSITY);
            }
            break;
        case LOGGING_LEVEL_DEBUG:
            {
                SetConsoleTextAttribute(m_HandleOfConsole,FOREGROUND_BLUE|FOREGROUND_INTENSITY);
            }
            break;
        case LOGGING_LEVEL_TRACE:
        default:
            {
                SetConsoleTextAttribute(m_HandleOfConsole,FOREGROUND_RED|FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_INTENSITY);
            }
            break;
        }
    }

    void PlatformWindowsConsoleChannel::WriteDataToConsole(const char* pDataToWrite, uint32_t lengthOfDataToWrite)
    {
        DWORD written;
        WriteFile(this->m_HandleOfConsole, pDataToWrite, lengthOfDataToWrite, &written, NULL);	
    }

}