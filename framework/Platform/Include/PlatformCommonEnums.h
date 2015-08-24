#ifndef PLATFORMCOMMONENUMS_H
#define PLATFORMCOMMONENUMS_H

#include <Platform/Include/PlatformInternalMessageEnum.h>

namespace CommunicationPlatform
{
    enum LoggingLevel
    {
        LOGGING_LEVEL_NONE        = 0,
        LOGGING_LEVEL_FATAL       = 1,
        LOGGING_LEVEL_CRITICAL    = 2,
        LOGGING_LEVEL_ERROR       = 3,
        LOGGING_LEVEL_WARNING     = 4,
        LOGGING_LEVEL_NOTICE      = 5,
        LOGGING_LEVEL_INFORMATION = 6,
        LOGGING_LEVEL_DEBUG       = 7,
        LOGGING_LEVEL_TRACE       = 8
    };

    enum ModulePriority
    {
        TASK_PRIORITY_HIGHEST       = 0,
        TASK_PRIORITY_HIGH          = 1,
        TASK_PRIORITY_MIDDLE_HIGH   = 2,
        TASK_PRIORITY_MIDDLE        = 3,
        TASK_PRIORITY_MIDDLE_LOW    = 4,
        TASK_PRIORITY_LOW           = 5,
        TASK_PRIORITY_LOWEST        = 6
    };

    enum ModuleCreateResult
    {
        TASK_CREATE_RESULT_SUCCESS                  = 0,
        TASK_CREATE_RESULT_FAIL_BEGIN_THREAD_ERROR  = 1,
        TASK_CREATE_RESULT_FAIL_SET_PRIORITY_ERROR  = 2,
        TASK_CREATE_RESULT_FAIL_RESUME_THREAD_ERROR = 3,
        TASK_CREATE_RESULT_FAIL_RUNNER_INIT_FAIL    = 4,
        TASK_CREATE_RESULT_FAIL_OTHER_REASON        = 5
    };

    enum MessageSendResult
    {
        MESSAGE_SEND_SUCCESS                         = 0,
        MESSAGE_SEND_FAIL_UNSUPPORTED_SEND_METHOD    = 1, //This destination does not support sending message in this way
        MESSAGE_SEND_FAIL_INSUFFICIENT_MEMORY        = 2, //This destination does not support sending message in this way
        MESSAGE_SEND_FAIL_ROUTER_NOT_AVAILABLE       = 3, //The message router is not available
        MESSAGE_SEND_FAIL_DESTINATION_NOT_FOUND      = 4, //The message router can not find the destination indicated by destNodeId and destModuleId
        MESSAGE_SEND_FAIL_OTHER_REASON               = 5
    };


    enum PlatformInternalModule
    {
        PLATFORM_INTERNAL_MODULE_TIMER             = 0,   /**< module for running the timer task */
        PLATFORM_INTERNAL_MODULE_MESSAGE_ROUTER    = 1,   /**< module for controlling routing of inter-module message*/
        PLATFORM_INTERNAL_MODULE_ASYNC_LOG_CHANNEL = 2,   /**< module for receiving formatted log messge and write to the hooked channel(s) */
        PLATFORM_INTERNAL_MODULE_TESTCASE_RUNNER   = 3,   /**< module for running each test case */
        PLATFORM_INTERNAL_MODULE_TESTCASE_HELPER   = 4,   /**< module for collecting inter-module message and filter what is wanted for the running testcases */

        PLATFORM_INTERNAL_MODULE_MAX               = 128
    };

    enum PlatformInternalNodes
    {
        PLATFORM_INTERNAL_NODE_THIS_NODE = 0,
        PLATFORM_INTERNAL_NODE_MAX       = 100
    };
}



#endif
