#include <Platform/Include/PlatformTestAPI.h>
#include <Platform/Include/PlatformAPI.h>
#include <Platform/Include/InterTaskMessageBase.h>

namespace CommunicationPlatform
{
    typedef void (*TestCaseEntryFunction)(void); 

    void* FindTestCaseFunctionByName(const std::string& testSuiteName,const std::string& testCaseName);

    /**
     *  PLATFORM_MESSAGE_START_RUN_TESTCASE_REQUEST Message Definition
     */
    class TestRunnerStartRunTestcaseRequestMsg: public InterModuleMessageBase
    {
    public:
        TestRunnerStartRunTestcaseRequestMsg():m_TestCaseFunctionPointer(NULL),m_TestCaseName(""){};
        ~TestRunnerStartRunTestcaseRequestMsg(){};

        InterModuleMessageBase* Clone(){return new TestRunnerStartRunTestcaseRequestMsg();}

        void*       m_TestCaseFunctionPointer;
        std::string m_TestCaseName;
    };

    /**
     *  PLATFORM_MESSAGE_INTER_MODULE_MESSAGE_COPY_IND Message Definition
     */
    class PlatformInterModuleMessageCopyIndMsg: public InterModuleMessageBase
    {
    public:
        PlatformInterModuleMessageCopyIndMsg():m_CopiedMessage(NULL){};
        ~PlatformInterModuleMessageCopyIndMsg(){};

        InterModuleMessageBase* Clone(){return new PlatformInterModuleMessageCopyIndMsg();}

        InterTaskMessage* m_CopiedMessage;
    };

    extern MessageTriggeredTaskModule* gTestRunnerTaskModule;
    extern MessageTriggeredTaskModule* gTestHelperaskModule;
}