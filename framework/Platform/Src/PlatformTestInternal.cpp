#include <Platform/Include/PlatformTestAPI.h>
#include <Platform/Include/PlatformTestInternal.h>
#include <Platform/Include/PlatformCommonClass.h>
#include <Platform/Include/PlatformCommonEnums.h>

namespace CommunicationPlatform
{
    extern MessageTriggeredTaskModule* gTestRunnerTaskModule = NULL;
    extern MessageTriggeredTaskModule* gTestHelperaskModule  = NULL;

    PlatformTestCaseRunnerModule* PlatformTestCaseRunnerModule::gTestCaseRunnerInstance = NULL;

    PlatformTestCaseRunnerModule::~PlatformTestCaseRunnerModule()
    {

    }

    PlatformTestCaseRunnerModule* PlatformTestCaseRunnerModule::GetInstance()
    {
        if (NULL == gTestCaseRunnerInstance)
        {
            gTestCaseRunnerInstance = new PlatformTestCaseRunnerModule();

            return gTestCaseRunnerInstance;
        }
        else
        {
            return gTestCaseRunnerInstance;
        }
    }

    ModuleRunner::RunnerInitResult PlatformTestCaseRunnerModule::InitRunner()
    {
        return RUNNER_INIT_SUCCESS;
    }

    void PlatformTestCaseRunnerModule::OnReceivedMessage(InterTaskMessage* pReceivedMessage)
    {
        switch(pReceivedMessage->m_MessageIdentity)
        {
        case PLATFORM_MESSAGE_START_RUN_TESTCASE_REQUEST:
            {
                StartRunTestCase((TestRunnerStartRunTestcaseRequestMsg*)(pReceivedMessage->m_MessageData.pMessageDataNew) );
            }
            break;
        default:
            break;
        }

    }

    void PlatformTestCaseRunnerModule::StartRunTestCase(TestRunnerStartRunTestcaseRequestMsg* pRunTestCaseReqMsg)
    {
        this->m_RunningTestCaseName = pRunTestCaseReqMsg->m_TestCaseName;

        this->m_HasTestCaseRunning = true;

        TestCaseEntryFunction pTestCaseEntry = (TestCaseEntryFunction)pRunTestCaseReqMsg->m_TestCaseFunctionPointer;

        if (NULL != pTestCaseEntry)
        {
            pTestCaseEntry();
        }

        PlatformTestCaseHelperModule* pTestCaseHelper = PlatformTestCaseHelperModule::GetInstance();

        this->m_HasTestCaseRunning = false;

        if (NULL !=  pTestCaseHelper)
        {
            pTestCaseHelper->m_ExpectedMessageID  = PLATFORM_MESSAGE_INVALID_MESSAGE_ID;
            pTestCaseHelper->m_HasMessageToExpect =  false;
            pTestCaseHelper->m_HasMessageToExpectEvent.set();
        }
    }


    PlatformTestCaseHelperModule::~PlatformTestCaseHelperModule()
    {

    }

    PlatformTestCaseHelperModule* PlatformTestCaseHelperModule::GetInstance()
    {
        if (NULL == gTestCaseHelperInstance)
        {
            gTestCaseHelperInstance = new PlatformTestCaseHelperModule();

            return gTestCaseHelperInstance;
        }
        else
        {
            return gTestCaseHelperInstance;
        }
    }

    ModuleRunner::RunnerInitResult PlatformTestCaseHelperModule::InitRunner()
    {
        return RUNNER_INIT_SUCCESS;
    }

    void PlatformTestCaseHelperModule::CleanExpectedMessageContent()
    {
        if (NULL != this->m_GottenExpectedMessageContent)
        {
            delete this->m_GottenExpectedMessageContent;
            this->m_GottenExpectedMessageContent = NULL;
        }
    }

    void PlatformTestCaseHelperModule::OnReceivedMessage(InterTaskMessage* pReceivedMessage)
    {
        PlatformTestCaseRunnerModule* pTestCaseRunner = PlatformTestCaseRunnerModule::GetInstance();

        if (NULL == pTestCaseRunner)
        {
            return;
        }

        if ((true == this->m_DependentOnTestRunner) && (false == pTestCaseRunner->m_HasTestCaseRunning) )
        {
            return;
        }

        if (false == this->m_HasMessageToExpect)
        {
            this->m_HasMessageToExpectEvent.wait();
        }
        
        PlatformInterModuleMessageCopyIndMsg* pCopiedInterModuleMessage = (PlatformInterModuleMessageCopyIndMsg*)pReceivedMessage->m_MessageData.pMessageDataNew;

        if (NULL == pCopiedInterModuleMessage)
        {
            return;
        }

        if (this-> m_ExpectedMessageID == pCopiedInterModuleMessage->m_CopiedMessage->m_MessageIdentity )
        {
            CleanExpectedMessageContent();

            this->m_GottenExpectedMessageContent = pCopiedInterModuleMessage->m_CopiedMessage;

            this->m_HasMessageToExpect = false;

            this->m_MessageExpectSuccessEvent.set();

            return;
        }
        else
        {
            return;
        }
    }

    PlatformTestCaseHelperModule* PlatformTestCaseHelperModule::gTestCaseHelperInstance = NULL;

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
	TestCaseStartResult StartRunTestCase(const std::string& testCaseName, std::string& runningTestCase, const std::string& testSuiteName)
    {
        PlatformTestCaseRunnerModule* pTestRunner = PlatformTestCaseRunnerModule::GetInstance();

        if (NULL == pTestRunner)
        {
            return TEST_CASE_START_RESULT_FAIL_TEST_RUNNER_NOT_AVAILABLE;
        }

        if (true == pTestRunner->m_HasTestCaseRunning)
        {
            runningTestCase = pTestRunner->m_RunningTestCaseName;

            return TEST_CASE_START_RESULT_FAIL_ANOTHER_CASE_IS_RUNNING;
        }

        void* pTestCaseFunctionAddress = FindTestCaseFunctionByName(testSuiteName,testCaseName);
        if (NULL == pTestCaseFunctionAddress)
        {
            return TEST_CASE_START_RESULT_FAIL_CASE_CANNOT_BE_FOUND;
        }

        NodeMessageRouter* pMessageRounter = NodeMessageRouter::GetMessageRouter();

        if (NULL == pMessageRounter)
        {
            return TEST_CASE_START_RESULT_FAIL_OTHER_REASON;
        }

        ModuleDestinationInfo* pTestRunnerModule = pMessageRounter->GetDestination(PLATFORM_INTERNAL_NODE_THIS_NODE,PLATFORM_INTERNAL_MODULE_TESTCASE_RUNNER);

        if (NULL == pTestRunnerModule)
        {
            return TEST_CASE_START_RESULT_FAIL_OTHER_REASON;
        }

        TestRunnerStartRunTestcaseRequestMsg* pStartRunTestCaseReqMsg = new TestRunnerStartRunTestcaseRequestMsg();

        if (NULL == pStartRunTestCaseReqMsg)
        {
            return TEST_CASE_START_RESULT_FAIL_OTHER_REASON;
        }

        pStartRunTestCaseReqMsg->m_TestCaseFunctionPointer = pTestCaseFunctionAddress;
        pStartRunTestCaseReqMsg->m_TestCaseName            = testCaseName;

        pTestRunnerModule->SendMessageNew(PLATFORM_INTERNAL_NODE_THIS_NODE,PLATFORM_INTERNAL_MODULE_MESSAGE_ROUTER,
                                          PLATFORM_MESSAGE_START_RUN_TESTCASE_REQUEST,
                                          pStartRunTestCaseReqMsg,sizeof(TestRunnerStartRunTestcaseRequestMsg));

        return TEST_CASE_START_RESULT_SUCCESS;
    }


    /**
     * @fn  void WaitForMessageArrive(uint32_t expectedMessageId, void* expectedMessageBody, uint32_t& expectedMessageBodyLength);
     *
     * @brief   Wait for a expected message to arrived at the destination module
     *
     * @param [in]      expectedMessageId   Message Id for the expected InterTaskMessage
     * @param [out]     expectedMessageBody If non-null, the expected message body.
     * @param [out]     expectedMessageBodyLength   Length of the expected message body.
     */
    void WaitForMessageArrive(uint32_t expectedMessageId, void*& expectedMessageBody, uint32_t& expectedMessageBodyLength)
    {
        PlatformTestCaseHelperModule* pTestCaseHelper = PlatformTestCaseHelperModule::GetInstance();

        if (NULL == pTestCaseHelper)
        {
            return;
        }

        pTestCaseHelper->m_ExpectedMessageID  = expectedMessageId;

        pTestCaseHelper->m_HasMessageToExpect = true;

        pTestCaseHelper->m_HasMessageToExpectEvent.set();

        pTestCaseHelper->m_MessageExpectSuccessEvent.wait();

        switch (pTestCaseHelper->m_GottenExpectedMessageContent->m_MessageMemoryType)
        {
        case InterTaskMessage::MESSAGE_DATA_MEMORY_TYPE_MALLOC:
            {
                expectedMessageBody = pTestCaseHelper->m_GottenExpectedMessageContent->m_MessageData.pMessageDataMalloc;
            }
        	break;
        case InterTaskMessage::MESSAGE_DATA_MEMORY_TYPE_NEW:
            {
                expectedMessageBody = pTestCaseHelper->m_GottenExpectedMessageContent->m_MessageData.pMessageDataNew;
            }
            break;
        default:
            {
                expectedMessageBody = NULL;
            }
            break;
        }

        expectedMessageBodyLength = pTestCaseHelper->m_GottenExpectedMessageContent->m_MessageSizeInByte;
    }

    TestModeGatewayDestination::TestModeGatewayDestination(uint32_t nodeId, uint32_t moduleId,ModuleDestinationInfo* destination)
        :ModuleDestinationInfo(nodeId,moduleId),
        m_ProxiedDestination(destination),
        m_CurrentWorkMode(GATEWAY_DESTINATION_REAL_DEST_WORK_AND_COPY_MESSAGE_TO_TESTRUNNER)
    {

    }

    TestModeGatewayDestination::~TestModeGatewayDestination()
    {

    }

   /** 
    *    @brief  For intra-node, inter-module communication only, 
    *    @param [in] pMessage           The message pointer, should be allocated using C++ new operator,  Message content pMessage is not copied and be directly sent to the destination, 
    *    @param [in] MessageSizeInByte  How much data the pMessage is pointed to.
    */
    MessageSendResult TestModeGatewayDestination::SendMessageNew(uint32_t sourceNodeId, uint32_t sourceModuleId, uint32_t messageIdentity,InterModuleMessageBase* pMessage, uint32_t MessageSizeInByte)
    {
        switch (this->m_CurrentWorkMode)
        {
        case GATEWAY_DESTINATION_REAL_DEST_WORK_AND_COPY_MESSAGE_TO_TESTRUNNER:
            {
                MessageSendResult sendResult = m_ProxiedDestination->SendMessageNew(sourceNodeId,sourceModuleId,messageIdentity,pMessage,MessageSizeInByte);
                
                if (sendResult == MESSAGE_SEND_SUCCESS)
                {
                    SendMessageNewToTestHelper(sourceNodeId,sourceModuleId,messageIdentity,pMessage,MessageSizeInByte);
                }

                return sendResult;
            }
        	break;
        case GATEWAY_DESTINATION_REAL_DEST_WORK_NOT_COPY_MESSAGE_TO_TESTRUNNER:
            {
                return m_ProxiedDestination->SendMessageNew(sourceNodeId,sourceModuleId,messageIdentity,pMessage,MessageSizeInByte);
            }
            break;
        case GATEWAY_DESTINATION_REAL_DEST_NOT_WORK_AND_SEND_MESSAGE_TO_TESTRUNNER:
            {
                SendMessageNewToTestHelper(sourceNodeId,sourceModuleId,messageIdentity,pMessage,MessageSizeInByte);
            }
            break;
        case GATEWAY_DESTINATION_REAL_DEST_NOT_WORK_AND_NOT_MESSAGE_TO_TESTRUNNER:
        default:
            break;
        }

        return MESSAGE_SEND_SUCCESS;
    }

    MessageSendResult TestModeGatewayDestination::SendMessageNewToTestHelper(uint32_t sourceNodeId, uint32_t sourceModuleId, uint32_t messageIdentity,InterModuleMessageBase* pMessage, uint32_t MessageSizeInByte)
    {

		PlatformInterModuleMessageCopyIndMsg* pMessageCopyIndMsg = new(std::nothrow) PlatformInterModuleMessageCopyIndMsg();

        if (NULL == pMessageCopyIndMsg)
        {
            return MESSAGE_SEND_FAIL_INSUFFICIENT_MEMORY;
        }

        InterTaskMessage* pCopiedMessage = NULL;

        if (pMessage != NULL)
        {
            pCopiedMessage = new(std::nothrow) InterTaskMessage(messageIdentity,MessageSizeInByte,sourceNodeId,sourceModuleId,InterTaskMessage::MESSAGE_DATA_MEMORY_TYPE_NEW);
        } 
        else
        {
            pCopiedMessage = new(std::nothrow) InterTaskMessage(messageIdentity,MessageSizeInByte,sourceNodeId,sourceModuleId,InterTaskMessage::MESSAGE_DATA_MEMORY_TYPE_NONE);
        }

        if (NULL == pCopiedMessage)
        {
            return MESSAGE_SEND_FAIL_INSUFFICIENT_MEMORY;
        }

        if (NULL != pMessage)
        {
            pCopiedMessage->m_MessageData.pMessageDataNew = pMessage->Clone();
        }

        pMessageCopyIndMsg->m_CopiedMessage = pCopiedMessage;

        InterTaskMessage* pActualSendedMessage = NULL;

        pActualSendedMessage = new(std::nothrow) InterTaskMessage(messageIdentity,MessageSizeInByte,sourceNodeId,sourceModuleId,InterTaskMessage::MESSAGE_DATA_MEMORY_TYPE_NEW);

        if (NULL == pActualSendedMessage)
        {
            return MESSAGE_SEND_FAIL_INSUFFICIENT_MEMORY;
        }

        pActualSendedMessage->m_MessageData.pMessageDataNew = pMessageCopyIndMsg;

        PlatformTestCaseHelperModule* pTestCaseHelper = PlatformTestCaseHelperModule::GetInstance();

        if (NULL == pTestCaseHelper)
        {
            return MESSAGE_SEND_FAIL_INSUFFICIENT_MEMORY;
        }

        pTestCaseHelper->m_MessageQueue.enqueueNotification(pActualSendedMessage);

        return MESSAGE_SEND_SUCCESS;
    }


   /** 
    *    @brief  Copy and send message to any module on any node,  Message content pMessage is copied and sent to the destination 
    *    @param [in] pMessage           The message pointer 
    *    @param [in] MessageSizeInByte  How much data the pMessage is pointed to.
    */
    MessageSendResult TestModeGatewayDestination::CopyAndSendMessage(uint32_t sourceNodeId, uint32_t sourceModuleId, uint32_t messageIdentity,void* pMessage, uint32_t MessageSizeInByte)
    {
        switch (this->m_CurrentWorkMode)
        {
        case GATEWAY_DESTINATION_REAL_DEST_WORK_AND_COPY_MESSAGE_TO_TESTRUNNER:
            {
                MessageSendResult sendResult = m_ProxiedDestination->CopyAndSendMessage(sourceNodeId,sourceModuleId,messageIdentity,pMessage,MessageSizeInByte);

                if (sendResult == MESSAGE_SEND_SUCCESS)
                {
                    CopyAndSendMessageToTestHelper(sourceNodeId,sourceModuleId,messageIdentity,pMessage,MessageSizeInByte);
                }

                return sendResult;
            }
            break;
        case GATEWAY_DESTINATION_REAL_DEST_WORK_NOT_COPY_MESSAGE_TO_TESTRUNNER:
            {
                return m_ProxiedDestination->CopyAndSendMessage(sourceNodeId,sourceModuleId,messageIdentity,pMessage,MessageSizeInByte);
            }
            break;
        case GATEWAY_DESTINATION_REAL_DEST_NOT_WORK_AND_SEND_MESSAGE_TO_TESTRUNNER:
            {
                CopyAndSendMessageToTestHelper(sourceNodeId,sourceModuleId,messageIdentity,pMessage,MessageSizeInByte);
            }
            break;
        case GATEWAY_DESTINATION_REAL_DEST_NOT_WORK_AND_NOT_MESSAGE_TO_TESTRUNNER:
        default:
            break;
        }

        return MESSAGE_SEND_SUCCESS;
    }

    MessageSendResult TestModeGatewayDestination::CopyAndSendMessageToTestHelper(uint32_t sourceNodeId, uint32_t sourceModuleId, uint32_t messageIdentity,void* pMessage, uint32_t MessageSizeInByte)
    {
        PlatformInterModuleMessageCopyIndMsg* pMessageCopyIndMsg = new(std::nothrow) PlatformInterModuleMessageCopyIndMsg();

        if (NULL == pMessageCopyIndMsg)
        {
            return MESSAGE_SEND_FAIL_INSUFFICIENT_MEMORY;
        }

        InterTaskMessage* pMessageToCopy = NULL;

        if (   (NULL != pMessage)
            &&(0    != MessageSizeInByte )
            )
        {
            pMessageToCopy = new(std::nothrow) InterTaskMessage(messageIdentity,MessageSizeInByte,sourceNodeId,sourceModuleId,InterTaskMessage::MESSAGE_DATA_MEMORY_TYPE_MALLOC);
        } 
        else
        {
            pMessageToCopy = new(std::nothrow) InterTaskMessage(messageIdentity,MessageSizeInByte,sourceNodeId,sourceModuleId,InterTaskMessage::MESSAGE_DATA_MEMORY_TYPE_NONE);
        }

        if (NULL == pMessageToCopy)
        {
            return MESSAGE_SEND_FAIL_INSUFFICIENT_MEMORY;
        }

        if (   (NULL != pMessage)
            &&(0    != MessageSizeInByte )
            )
        {
            pMessageToCopy->m_MessageData.pMessageDataMalloc = malloc(MessageSizeInByte);
            if (NULL == pMessageToCopy->m_MessageData.pMessageDataMalloc)
            {
                delete pMessageToCopy;
                return MESSAGE_SEND_FAIL_INSUFFICIENT_MEMORY;
            }

            memcpy(pMessageToCopy->m_MessageData.pMessageDataMalloc,pMessage,MessageSizeInByte);
        }

        pMessageCopyIndMsg->m_CopiedMessage = pMessageToCopy;

        InterTaskMessage* pActualSendedMessage = NULL;

        pActualSendedMessage = new(std::nothrow) InterTaskMessage(messageIdentity,MessageSizeInByte,sourceNodeId,sourceModuleId,InterTaskMessage::MESSAGE_DATA_MEMORY_TYPE_NEW);

        if (NULL == pActualSendedMessage)
        {
            return MESSAGE_SEND_FAIL_INSUFFICIENT_MEMORY;
        }

        pActualSendedMessage->m_MessageData.pMessageDataNew = pMessageCopyIndMsg;

        PlatformTestCaseHelperModule* pTestCaseHelper = PlatformTestCaseHelperModule::GetInstance();

        if (NULL == pTestCaseHelper)
        {
            free(pMessageToCopy->m_MessageData.pMessageDataMalloc);
            delete pMessageToCopy;
            return MESSAGE_SEND_FAIL_INSUFFICIENT_MEMORY;
        }

        pTestCaseHelper->m_MessageQueue.enqueueNotification(pActualSendedMessage);

        return MESSAGE_SEND_SUCCESS;
    }


   /** 
    *    @brief  Send message to any module on any node,  pMessage should be allocated using C malloc operator,message content is not copied for intra-node and copied for inter-node communication 
    *    @param [in] pMessage           The message pointer 
    *    @param [in] MessageSizeInByte  How much data the pMessage is pointed to.
    */
    MessageSendResult TestModeGatewayDestination::SendMessageMalloc(uint32_t sourceNodeId, uint32_t sourceModuleId, uint32_t messageIdentity,void* pMessage, uint32_t MessageSizeInByte)
    {
        switch (this->m_CurrentWorkMode)
        {
        case GATEWAY_DESTINATION_REAL_DEST_WORK_AND_COPY_MESSAGE_TO_TESTRUNNER:
            {
                MessageSendResult sendResult = m_ProxiedDestination->SendMessageMalloc(sourceNodeId,sourceModuleId,messageIdentity,pMessage,MessageSizeInByte);

                if (sendResult == MESSAGE_SEND_SUCCESS)
                {
                    CopyAndSendMessageToTestHelper(sourceNodeId,sourceModuleId,messageIdentity,pMessage,MessageSizeInByte);
                }

                return sendResult;
            }
            break;
        case GATEWAY_DESTINATION_REAL_DEST_WORK_NOT_COPY_MESSAGE_TO_TESTRUNNER:
            {
                return m_ProxiedDestination->SendMessageMalloc(sourceNodeId,sourceModuleId,messageIdentity,pMessage,MessageSizeInByte);
            }
            break;
        case GATEWAY_DESTINATION_REAL_DEST_NOT_WORK_AND_SEND_MESSAGE_TO_TESTRUNNER:
            {
                CopyAndSendMessageToTestHelper(sourceNodeId,sourceModuleId,messageIdentity,pMessage,MessageSizeInByte);
            }
            break;
        case GATEWAY_DESTINATION_REAL_DEST_NOT_WORK_AND_NOT_MESSAGE_TO_TESTRUNNER:
        default:
            break;
        }

        return MESSAGE_SEND_SUCCESS;
    }
}