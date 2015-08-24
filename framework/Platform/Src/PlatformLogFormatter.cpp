#include <time.h>

#include <ios>
#include <iomanip>

#include <Platform/Include/PlatformLogFormatter.h>

namespace CommunicationPlatform
{
#define INITIAL_MAX_BUFFER_SIZE_TO_CONVERT_STRING (4096)
    uint32_t PlatformLogFormatter::gMaxBufferSizeToConvertToString = INITIAL_MAX_BUFFER_SIZE_TO_CONVERT_STRING;

    void PlatformLogFormatter::FormatBinaryBufferToString(std::ostringstream& outputStream, const void* pData, uint32_t lenOfDataInByte)
    {
        uint8_t* pDataByte = (uint8_t*)pData;
        uint32_t dataLenToOutput = 0;

        if (lenOfDataInByte>gMaxBufferSizeToConvertToString)
        {
            dataLenToOutput = gMaxBufferSizeToConvertToString;
        }
        else
        {
            dataLenToOutput = lenOfDataInByte;
        }

        for (uint32_t pDataIndex=0;pDataIndex<dataLenToOutput;++pDataIndex)
        {
            outputStream<<std::setw(2)<<std::setfill('0')<<std::hex<<(unsigned int)pDataByte[pDataIndex]<<" ";
        }
    }

    void PlatformLogFormatter::SetMaxBufferSizeToConvertToString(uint32_t maxValue)
    {
        gMaxBufferSizeToConvertToString = maxValue;
    }

    uint32_t PlatformLogFormatter::GetMaxBufferSizeToConvertToString()
    {
        return gMaxBufferSizeToConvertToString;
    }

    void PlatformLogFormatter::GetCurrentTimeStamp(std::ostringstream& outputStream)
    {
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
            if (0 != strftime(buffer,sizeof(buffer),"[%Y-%m-%d %H:%M:%S.",timeinfo))
            {
                getTimeSuccess = true;
            }
        }

        if (true == getTimeSuccess)
        {
            int tInClock = clock();
            double tInMilliSeconds = ((double)tInClock/CLOCKS_PER_SEC)*1000.0;
            int tInMilliSecondsToOutput = (int)tInMilliSeconds%1000;

            outputStream<<buffer<< tInMilliSecondsToOutput<<"] ";
        }
    }

}