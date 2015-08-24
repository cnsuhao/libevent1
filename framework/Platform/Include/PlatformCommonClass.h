#ifndef PLATFORMCOMMONCLASS_H
#define PLATFORMCOMMONCLASS_H

#include <string>

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

#include <Platform/Include/InterTaskMessage.h>



namespace CommunicationPlatform
{
    class ModuleRunner
    {
    public:
        ModuleRunner(std::string moduleRunnerName):m_ModuleRunnerName(moduleRunnerName),m_NodeId(0){};
        virtual ~ModuleRunner(){};

        enum RunnerInitResult
        {
            RUNNER_INIT_SUCCESS = 0,
            RUNNER_INIT_FAIL    = 1
        };
        virtual RunnerInitResult InitRunner()=0;
        virtual uint32_t RunEntry()=0;
        void SetNodeId(uint32_t nodeId);
    protected:
        ModuleRunner();
        std::string m_ModuleRunnerName;
        uint32_t m_NodeId;
    };

    class MessageTriggeredModuleRunner: public ModuleRunner
    {
    public:
        MessageTriggeredModuleRunner(std::string moduleRunnerName,uint32_t moduleId):ModuleRunner(moduleRunnerName),m_ModuleId(moduleId){};
        MessageTriggeredModuleRunner(std::string moduleRunnerName):ModuleRunner(moduleRunnerName),m_ModuleId(0){};
        ~MessageTriggeredModuleRunner(){};

        virtual RunnerInitResult InitRunner()=0;
        virtual void OnReceivedMessage(InterTaskMessage* pReceivedMessage)=0;
        virtual void PostReceivedMessage(InterTaskMessage* pReceivedMessage);
        void SetModuleId(uint32_t moduleId);

        InterTaskMessageQueue         m_MessageQueue;

    protected:
        virtual uint32_t RunEntry(){return 0;}
        uint32_t m_ModuleId;
    };

	/**
	 * @brief	Interface for application module which process the received message.
	 */
	class IMessageProcessModule
	{
	public:
		virtual void OnReceivedMessage(uint32_t sourceNodeId, uint32_t sourceModuleId, uint32_t messageIdentity, void* pMessageData, uint32_t messageSizeInByte) = 0;
	};
}



#endif

