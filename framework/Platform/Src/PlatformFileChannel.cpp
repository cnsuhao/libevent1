#include <sstream>
#include <time.h>
#include <Platform/Include/PlatformFileChannel.h>
#include <Platform/Include/PlatformLogFormatter.h>
#include <Platform/Include/PlatformCommonMacros.h>

namespace CommunicationPlatform
{
    PlatformFileChannel::PlatformFileChannel()
        :m_FileToWriteLog()
    {

    }

    PlatformFileChannel::PlatformFileChannel(uint32_t fileSizeLimit)
        :m_FileToWriteLog(fileSizeLimit)
    {

    }

    PlatformFileChannel::~PlatformFileChannel()
    {

    }

    bool PlatformFileChannel::Open()
    {
        std::string logFilePath("./");

        time_t rawtime;
        struct tm * timeinfo;
        char buffer [256]="";

        bool getTimeSuccess = false;
        /* If the function could not retrieve the calendar time, it returns a -1 value.*/
        if (-1 != time ( &rawtime ))
        {
            timeinfo = localtime ( &rawtime );

            /* If the resulting C string fits in less than maxsize characters including the terminating null-character, the total number of characters copied to ptr (not including the terminating null-character) is returned.
               Otherwise, zero is returned and the contents of the array are indeterminate.*/
            if (0 != strftime(buffer,sizeof(buffer),"trace.%Y_%m%d_%H_%M_%S.txt",timeinfo))
            {
                getTimeSuccess = true;
                logFilePath.append(buffer);
            }
        }

        if (false == getTimeSuccess)
        {
            logFilePath.append("trace.txt");
        }

        return this->m_FileToWriteLog.Open(logFilePath);
    }

    void PlatformFileChannel::Close()
    {
        this->m_FileToWriteLog.Close();
    }

    void PlatformFileChannel::logString(LoggingLevel logLevel, const char* stringToBeLogged, uint32_t lenOfStringToBeLoggedInByte)
    {
        USE_UNFERENCED_PARAMETER(logLevel);

        std::ostringstream timeStampText;
        PlatformLogFormatter::GetCurrentTimeStamp(timeStampText);

        std::string timeStampString = timeStampText.str();

        this->m_FileToWriteLog.Write(timeStampString.c_str(),timeStampString.size());

        this->m_FileToWriteLog.Write(stringToBeLogged,lenOfStringToBeLoggedInByte-1);

        this->m_FileToWriteLog.Write("\r\n",2);
    }

    void PlatformFileChannel::logDumpBin(LoggingLevel logLevel, const char* stringToBeLogged, uint32_t lenOfStringToBeLoggedInByte, const void* pData, uint32_t lenOfDataInByte)
    {
        USE_UNFERENCED_PARAMETER(logLevel);

        std::ostringstream timeStampText;
        PlatformLogFormatter::GetCurrentTimeStamp(timeStampText);

        std::string timeStampString = timeStampText.str();

        this->m_FileToWriteLog.Write(timeStampString.c_str(),timeStampString.size());

        this->m_FileToWriteLog.Write(stringToBeLogged,lenOfStringToBeLoggedInByte-1);

        std::ostringstream binDataText;

        PlatformLogFormatter::FormatBinaryBufferToString(binDataText,pData,lenOfDataInByte);

        std::string formattedString = binDataText.str();

        this->m_FileToWriteLog.Write(formattedString.c_str(),formattedString.size());

        this->m_FileToWriteLog.Write("\r\n",2);
    }
}