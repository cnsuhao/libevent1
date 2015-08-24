#include <Platform/Include/SysBios/SysBiosSpecificHeaders.h>


UInt  ConvertMicroSecondsToSystemTick(int64_t microSeconds)
{
	int64_t clockTickPeriod = Clock_tickPeriod;
    if (microSeconds<=clockTickPeriod)
    {
        return 1;
    } 
    else
    {
    	UInt sleepPeriod = microSeconds/clockTickPeriod;
        return sleepPeriod;
    }

}
