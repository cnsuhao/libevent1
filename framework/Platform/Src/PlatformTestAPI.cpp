#include <Platform/Include/PlatformTestAPI.h>
#include <Platform/Include/PlatformTestInternal.h>

using namespace CommunicationPlatform;

namespace CommunicationPlatform
{

    bool InitTestCaseRunner(uint32_t testCaseRunnerStackSize, uint32_t testCaseHelperStackSize)
    {
        PlatformTestCaseRunnerModule* pTestRunner = PlatformTestCaseRunnerModule::GetInstance();

        if (NULL == pTestRunner)
        {
            return false;
        }

        gTestRunnerTaskModule = new MessageTriggeredTaskModule(PLATFORM_INTERNAL_NODE_THIS_NODE,
            PLATFORM_INTERNAL_MODULE_TESTCASE_RUNNER,
            TASK_PRIORITY_HIGHEST,
            testCaseRunnerStackSize,
            "Test Runner",
            pTestRunner);

        if (NULL == gTestRunnerTaskModule)
        {
            return false;
        }

        PlatformTestCaseHelperModule* pTestHelper = PlatformTestCaseHelperModule::GetInstance();

        if (NULL == pTestHelper)
        {
            delete gTestRunnerTaskModule;
            gTestRunnerTaskModule = NULL;
            return false;
        }

        gTestHelperaskModule = new MessageTriggeredTaskModule(PLATFORM_INTERNAL_NODE_THIS_NODE,
            PLATFORM_INTERNAL_MODULE_TESTCASE_HELPER,
            TASK_PRIORITY_HIGHEST,
            testCaseHelperStackSize,
            "Test Helper",
            pTestHelper);

        if (NULL == gTestHelperaskModule)
        {
            delete gTestRunnerTaskModule;
            gTestRunnerTaskModule = NULL;
            return false;
        }


        ModuleCreateResult testRunnerStartResult = gTestRunnerTaskModule->Start();

        if (testRunnerStartResult != TASK_CREATE_RESULT_SUCCESS)
        {
            return false;
        }

        ModuleCreateResult testRunnerTestResult = gTestHelperaskModule->Start();

        if (testRunnerTestResult != TASK_CREATE_RESULT_SUCCESS)
        {
            return false;
        }

        return true;
    }

	void StopTestCaseRunner()
	{
		if (NULL != gTestRunnerTaskModule)
		{
			gTestRunnerTaskModule->StopProcessMessage();
			gTestRunnerTaskModule->Join();
		}

		if (NULL != gTestHelperaskModule)
		{
			gTestHelperaskModule->StopProcessMessage();
			gTestHelperaskModule->Join();
		}
	}

}
