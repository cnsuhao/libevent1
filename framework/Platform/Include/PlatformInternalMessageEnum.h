#ifndef PLATFORMINTERNALMESSAGEENUM_H
#define PLATFORMINTERNALMESSAGEENUM_H

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

#undef DECL_EVENT
#define DECL_PLATFORM_INTERNAL_MESSAGE(x) x

namespace CommunicationPlatform
{
	enum PlatformSpecificMessageId
	{
		#include <Platform/Include/PlatformInternalMessageDeclare.h>
	};

	extern const char *PlatformSpecificMessageIdNames[];

	const char* GetPlatformSpecificMessageIdName(uint32_t platformSepcificMessageId);
}

#endif