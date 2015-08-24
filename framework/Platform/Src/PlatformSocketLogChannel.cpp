#include <sstream>
#include <Platform/Include/PlatformLogFormatter.h>
#include <Platform/Include/PlatformSocketLogChannel.h>


namespace CommunicationPlatform
{
    PlatformUDPSocketChannel::PlatformUDPSocketChannel(PlatformUDPSocket* m_LogWriteUDPSocket)
        :m_LogWriteUDPSocket(m_LogWriteUDPSocket)
    {
    }

    PlatformUDPSocketChannel::~PlatformUDPSocketChannel()
    {

    }


    bool PlatformUDPSocketChannel::Open()
    {
        return true;
    }

    void PlatformUDPSocketChannel::Close()
    {
        this->m_LogWriteUDPSocket = NULL;
    }

    void PlatformUDPSocketChannel::logString(LoggingLevel logLevel, const char* stringToBeLogged, uint32_t lenOfStringToBeLoggedInByte)
    {
        uint32_t bufferWriteIndex = UDP_LOG_PACKET_LEVEL_INDEX;

        m_LogPacketAssembleBuffer[bufferWriteIndex] = (uint8_t)logLevel;
        bufferWriteIndex+=UDP_LOG_PACKET_LEVEL_LENGTH;

        memcpy(m_LogPacketAssembleBuffer+bufferWriteIndex,stringToBeLogged,lenOfStringToBeLoggedInByte);

        bufferWriteIndex+=lenOfStringToBeLoggedInByte;

        this->m_LogWriteUDPSocket->SendBytes(m_LogPacketAssembleBuffer,bufferWriteIndex);
    }


    void PlatformUDPSocketChannel::logDumpBin(LoggingLevel logLevel, const char* stringToBeLogged, uint32_t lenOfStringToBeLoggedInByte, const void* pData, uint32_t lenOfDataInByte)
    {
        uint32_t bufferWriteIndex = UDP_LOG_PACKET_LEVEL_INDEX;

        m_LogPacketAssembleBuffer[bufferWriteIndex] = (uint8_t)logLevel;
        bufferWriteIndex+=UDP_LOG_PACKET_LEVEL_LENGTH;

        memcpy(m_LogPacketAssembleBuffer+bufferWriteIndex,stringToBeLogged,lenOfStringToBeLoggedInByte);

        bufferWriteIndex+=lenOfStringToBeLoggedInByte;

        std::ostringstream binDataText;
        PlatformLogFormatter::FormatBinaryBufferToString(binDataText,pData,lenOfDataInByte);

        std::string formattedString = binDataText.str();

        memcpy(m_LogPacketAssembleBuffer+bufferWriteIndex,formattedString.data(),formattedString.size());
        bufferWriteIndex+=formattedString.size();

        this->m_LogWriteUDPSocket->SendBytes(m_LogPacketAssembleBuffer,bufferWriteIndex);
    }

    PlatformUDPSocketChannelEndPoint::PlatformUDPSocketChannelEndPoint(std::string moduleName)
        :ModuleRunner(moduleName),
        m_DataReceiveSocket(),
        m_ListeningPort(0),
        m_TargetChannel(NULL)
    {

    }

    PlatformUDPSocketChannelEndPoint::~PlatformUDPSocketChannelEndPoint()
    {

    }

    ModuleRunner::RunnerInitResult PlatformUDPSocketChannelEndPoint::InitRunner()
    {
        PlatformIPAddress listingAddress;
        listingAddress.SetAddress("0.0.0.0");

        if (false == this->m_DataReceiveSocket.InitServerSide(listingAddress,this->m_ListeningPort,true))
        {
            return RUNNER_INIT_FAIL;
        }

        return RUNNER_INIT_SUCCESS;
    }

    uint32_t PlatformUDPSocketChannelEndPoint::RunEntry()
    {
        uint32_t receivedDataLength = 0;

        PlatformIPAddress clientIPAddress;

        for (;;)
        {
            bool receiveResult = this->m_DataReceiveSocket.ReceiveBytes(m_ReceivingBuffer,sizeof(m_ReceivingBuffer),receivedDataLength,clientIPAddress);

            if (false == receiveResult)
            {
                continue;
            }

            if (NULL != this->m_TargetChannel)
            {
                this->m_TargetChannel->DoLogString((LoggingLevel)m_ReceivingBuffer[UDP_LOG_PACKET_LEVEL_INDEX],
                                                   m_ReceivingBuffer+UDP_LOG_PACKET_CONTENT_INDEX,
                                                   (receivedDataLength-UDP_LOG_PACKET_LEVEL_LENGTH));
            }
        }
    }

}