#ifndef PLATFORMTESTAPI_H
#define PLATFORMTESTAPI_H

/**
*   -# To Write a testcase simply using  TESTCASE_DEFINE macro to declare a testcase,
*      e.g.:  
*      TESTCASE_DEFINE(TestMac001)
*      {
*           //Write the testcase code here...
*      }
*   -# At the program Initialization stage, InitTestCaseRunner should be called with appropriate arguments
*   -# To start a testcase, simply call StartRunTestCase function
*/

#ifdef _MSC_VER
    #include <Platform/Include/Windows/WindowsSpecificHeaders.h>
    #include <Platform/Include/Windows/PlatformTestAPI_Win32.h>
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

#include <string>

#include <Platform/Include/PlatformAPI.h>
#include <Platform/Include/PlatformCommonEnums.h>
#include <Platform/Include/MessageRouting.h>

#define TESTCASE_DECLARE(TestCaseName) TESTCASE_DECLARE_IMPLE_(TestCaseName)
#define TESTCASE_DEFINE(TestCaseName)  TESTCASE_DEFINE_IMPLE_(TestCaseName)
namespace CommunicationPlatform
{
    bool InitTestCaseRunner(uint32_t testCaseRunnerStackSize, uint32_t testCaseHelperStackSize);
	void StopTestCaseRunner();

    enum TestCaseStartResult
    {
        TEST_CASE_START_RESULT_SUCCESS                         = 0,
        TEST_CASE_START_RESULT_FAIL_ANOTHER_CASE_IS_RUNNING    = 1,
        TEST_CASE_START_RESULT_FAIL_CASE_CANNOT_BE_FOUND       = 2,
        TEST_CASE_START_RESULT_FAIL_TEST_RUNNER_NOT_AVAILABLE  = 3,
        TEST_CASE_START_RESULT_FAIL_OTHER_REASON               = 4
    };
    /**
     * @fn  bool StartRunTestCase(std::string testCaseName, std::string& runningTestCase);
     *
     * @brief   Starts to run a testcase
     *
     * @param [in]      testCaseName    name of the test case to run
     * @param [out]     if function returns false, output which testcase is running
     *
     * @return  true if the testcase specified by testCaseName is successfully started, false if another test case is already running
     */
    TestCaseStartResult StartRunTestCase(const std::string& testCaseName, std::string& runningTestCase, const std::string& testSuiteName);

    /**
     * @fn  void WaitForMessageArrive(uint32_t expectedMessageId, void* expectedMessageBody, uint32_t& expectedMessageBodyLength);
     *
     * @brief   Wait for a expected message to arrived at the destination module
     *
     * @param [in]      expectedMessageId   Message Id for the expected InterTaskMessage
     * @param [out]     expectedMessageBody If non-null, the expected message body.
     * @param [out]     expectedMessageBodyLength   Length of the expected message body.
     *
     */
    void WaitForMessageArrive(uint32_t expectedMessageId, void*& expectedMessageBody, uint32_t& expectedMessageBodyLength);

    class TestRunnerStartRunTestcaseRequestMsg;
    class InterTaskMessage;

    class PlatformTestCaseRunnerModule:public MessageTriggeredModuleRunner
    {
    public:
        virtual ~PlatformTestCaseRunnerModule();
        bool        m_HasTestCaseRunning;   /**< whether there is any testcase running */
        std::string m_RunningTestCaseName;

        static PlatformTestCaseRunnerModule* GetInstance();

        RunnerInitResult InitRunner();

        void OnReceivedMessage(InterTaskMessage* pReceivedMessage);

    private:
        PlatformTestCaseRunnerModule():MessageTriggeredModuleRunner("Platform Test Runner"),m_HasTestCaseRunning(false),m_RunningTestCaseName(""){};

        static PlatformTestCaseRunnerModule* gTestCaseRunnerInstance;

        void StartRunTestCase(TestRunnerStartRunTestcaseRequestMsg* pRunTestCaseReqMsg);

    };


    class PlatformTestCaseHelperModule:public MessageTriggeredModuleRunner
    {
    public:
        virtual ~PlatformTestCaseHelperModule();
        static PlatformTestCaseHelperModule* GetInstance();

        RunnerInitResult InitRunner();

        void OnReceivedMessage(InterTaskMessage* pReceivedMessage);

        uint32_t m_ExpectedMessageID; 

        bool m_HasMessageToExpect; 

        Event m_HasMessageToExpectEvent;
        Event m_MessageExpectSuccessEvent;


        InterTaskMessage* m_GottenExpectedMessageContent;
		bool m_DependentOnTestRunner;

    private:
        PlatformTestCaseHelperModule():MessageTriggeredModuleRunner("Platform Test Helper"),
            m_ExpectedMessageID(PLATFORM_MESSAGE_INVALID_MESSAGE_ID),
            m_HasMessageToExpect(false),
            m_HasMessageToExpectEvent(true),
            m_MessageExpectSuccessEvent(true),
            m_GottenExpectedMessageContent(),
			m_DependentOnTestRunner(false)
        {

        };

        void CleanExpectedMessageContent();

        static PlatformTestCaseHelperModule* gTestCaseHelperInstance;
    };

    class MessageTriggeredModuleRunner;
    class InterModuleMessageBase;
    class ModuleDestinationInfo;

    class TestModeGatewayDestination: public ModuleDestinationInfo
    {
    public:
        enum WorkMode
        {
            GATEWAY_DESTINATION_REAL_DEST_WORK_AND_COPY_MESSAGE_TO_TESTRUNNER      =0,
            GATEWAY_DESTINATION_REAL_DEST_WORK_NOT_COPY_MESSAGE_TO_TESTRUNNER      =1,
            GATEWAY_DESTINATION_REAL_DEST_NOT_WORK_AND_SEND_MESSAGE_TO_TESTRUNNER  =2,
            GATEWAY_DESTINATION_REAL_DEST_NOT_WORK_AND_NOT_MESSAGE_TO_TESTRUNNER   =3
        };

        TestModeGatewayDestination(uint32_t nodeId, uint32_t moduleId,ModuleDestinationInfo* destination);
        ~TestModeGatewayDestination();
    protected:
       /** 
        *    @brief  For intra-node, inter-module communication only, 
        *    @param [in] pMessage           The message pointer, should be allocated using C++ new operator,  Message content pMessage is not copied and be directly sent to the destination, 
        *    @param [in] MessageSizeInByte  How much data the pMessage is pointed to.
        */
        virtual MessageSendResult SendMessageNew(uint32_t sourceNodeId, uint32_t sourceModuleId, uint32_t messageIdentity,InterModuleMessageBase* pMessage, uint32_t MessageSizeInByte);

        MessageSendResult SendMessageNewToTestHelper(uint32_t sourceNodeId, uint32_t sourceModuleId, uint32_t messageIdentity,InterModuleMessageBase* pMessage, uint32_t MessageSizeInByte);

       /** 
        *    @brief  Copy and send message to any module on any node,  Message content pMessage is copied and sent to the destination 
        *    @param [in] pMessage           The message pointer 
        *    @param [in] MessageSizeInByte  How much data the pMessage is pointed to.
        */
        virtual MessageSendResult CopyAndSendMessage(uint32_t sourceNodeId, uint32_t sourceModuleId, uint32_t messageIdentity,void* pMessage, uint32_t MessageSizeInByte);

        MessageSendResult CopyAndSendMessageToTestHelper(uint32_t sourceNodeId, uint32_t sourceModuleId, uint32_t messageIdentity,void* pMessage, uint32_t MessageSizeInByte);

       /** 
        *    @brief  Send message to any module on any node,  pMessage should be allocated using C malloc operator,message content is not copied for intra-node and copied for inter-node communication 
        *    @param [in] pMessage           The message pointer 
        *    @param [in] MessageSizeInByte  How much data the pMessage is pointed to.
        */
        virtual MessageSendResult SendMessageMalloc(uint32_t sourceNodeId, uint32_t sourceModuleId, uint32_t messageIdentity,void* pMessage, uint32_t MessageSizeInByte);         /**< pMessage should be allocated using C malloc operator, Message content pMessage is not copied and be directly sent to the destination,  */

        WorkMode               m_CurrentWorkMode;
    private:
        TestModeGatewayDestination();

        ModuleDestinationInfo* m_ProxiedDestination;
    };
}


#endif