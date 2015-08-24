#include <Platform/Include/PlatformTestInternal.h>
#include <string>

namespace CommunicationPlatform
{
	void* FindTestCaseFunctionByName(const std::string& testSuiteName,const std::string& testCaseName)
    {
        if (testCaseName != "")
        {
            std::string testCaseFunction = "TestCaseFunction" + testCaseName;

			if (testSuiteName != "")
			{
				HMODULE theModuleHandle = GetModuleHandle(testSuiteName.c_str());

				if (NULL != theModuleHandle)
				{
					return GetProcAddress(theModuleHandle,testCaseFunction.c_str());
				}
				else
				{
					return NULL;
				}
				
			} 
			else
			{
				HMODULE theModuleHandle = GetModuleHandle(NULL);

				if (NULL != theModuleHandle)
				{
					return GetProcAddress(theModuleHandle,testCaseFunction.c_str());
				}
				else
				{
					return NULL;
				}
			}
            
        }
        else
        {
            return NULL;
        }
    }
}



