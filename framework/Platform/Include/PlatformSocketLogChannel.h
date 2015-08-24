#ifndef PLATFORMSOCKETLOGCHANNEL_H
#define PLATFORMSOCKETLOGCHANNEL_H

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

#include <Platform/Include/PlatformLogChannel.h>
#include <Platform/Include/PlatformUDPSocket.h>
#include <Platform/Include/PlatformCommonClass.h>

namespace CommunicationPlatform
{
#define UDP_LOG_PACKET_LEVEL_INDEX (0)
#define UDP_LOG_PACKET_LEVEL_LENGTH (1)
#define UDP_LOG_PACKET_CONTENT_INDEX (1)

    class PlatformUDPSocketChannel: public PlatformLogChannel
    {
    public:
        PlatformUDPSocketChannel(PlatformUDPSocket* m_LogWriteUDPSocket);
        ~PlatformUDPSocketChannel();

        bool Open();

        void Close();
    protected:
        virtual void logString(LoggingLevel logLevel, const char* stringToBeLogged, uint32_t lenOfStringToBeLoggedInByte);

        virtual void logDumpBin(LoggingLevel logLevel, const char* stringToBeLogged, uint32_t lenOfStringToBeLoggedInByte, const void* pData, uint32_t lenOfDataInByte);

        PlatformUDPSocket* m_LogWriteUDPSocket;

#define PLATFORM_UDP_SOCKET_CHANNEL_PACKET_ASSEMBLE_BUFFER_SIZE 65535
        uint8_t m_LogPacketAssembleBuffer[65535];
    };

    class PlatformUDPSocketChannelEndPoint: public ModuleRunner
    {
    public:
        PlatformUDPSocketChannelEndPoint(std::string moduleName);
        ~PlatformUDPSocketChannelEndPoint();
        RunnerInitResult InitRunner();
        uint32_t RunEntry();

        uint16_t m_ListeningPort;

        PlatformLogChannel* m_TargetChannel;
    protected:
        PlatformUDPSocket m_DataReceiveSocket;
#define PLATFORM_UDPSOCKET_CHANNELENDPOINT_BUFFER_SIZE (65535)
        char m_ReceivingBuffer[PLATFORM_UDPSOCKET_CHANNELENDPOINT_BUFFER_SIZE];
    }; 
}

#endif