#ifndef PLATFORMTIMER_H
#define PLATFORMTIMER_H

#include <list>

#ifdef _MSC_VER
    #include <Platform/Include/Windows/WaitableTimingObject_Win32.h>
#else
    #ifdef linux
        #include <Platform/Include/Linux/WaitableTimingObject_Linux.h>
    #else
        #ifdef __TI_COMPILER_VERSION__
            #include <Platform/Include/SysBios/WaitableTimingObject_SysBios.h>
        #else
            //#error Unsupported Platform!
        #endif
    #endif
#endif


#include <Platform/Include/PlatformCommonClass.h>
#include <Platform/Include/PlatformMutex.h>

namespace CommunicationPlatform
{
    class BasicTaskModule;
    class MessageNotifiedTimer;
    class TimerModuleRunner;

    class WaitableTimingObject: public WaitableTimingObjectImplementation
    {
    public:
        WaitableTimingObject();
        virtual ~WaitableTimingObject();
        bool Initialize()
        {
            return WaitableTimingObjectImplementation::Initialize();
        }

        bool Wait(int64_t waitPeriodInMicroSeconds)
        {
            return WaitableTimingObjectImplementation::Wait(waitPeriodInMicroSeconds);
        }
    
    };

    /**
    *   Timing Item information in the timer thread's timing queue 
    */
    class TimingItemInfo
    {
    public:
        TimingItemInfo(int64_t PeriodToTimeoutInMicroSecond,uint32_t DestinationNodeId,uint32_t DestinationModuleId,uint32_t TimerId,void* m_TimeOutData = NULL);
        ~TimingItemInfo();

        int64_t                m_PeriodToTimeoutInMicroSecond; /**< How much microseconds for this timing item to timeout */
        uint32_t               m_DestinationNodeId;            /**< Node Id to send the timeout message */
        uint32_t               m_DestinationModuleId;          /**< Module Id to send the timeout message */
        uint32_t               m_TimerId;                      /**< Timer Id in the destination Node and Module */
        void*                  m_TimeOutData;                  /**< Data to be send to destination Node and Module */
        MessageNotifiedTimer*  m_TimerObjectRef;               /**< Reference for the Timer object */                       
		    uint32_t               m_TimerInstanceId;              /**< Timer instance id */
    private:
        TimingItemInfo(){};
    };

    enum TimerPeriodUnit
    {
        TIMER_PERIOD_UNIT_MINUTES     = 0,
        TIMER_PERIOD_UNIT_SECOND      = 1,
        TIMER_PERIOD_UNIT_MILLISECOND = 2,
        TIMER_PERIOD_UNIT_MICROSECOND = 3
    };

    typedef std::list<TimingItemInfo> TimerRunnerQueue;

    /**
    *   When This type of timer timeouts, a message will be sent to the module's message queue
    */
    class MessageNotifiedTimer
    {
    public:
        MessageNotifiedTimer(TimerPeriodUnit timerUnit, int64_t periodToTimeout,uint32_t DestinationNodeId,uint32_t DestinationModuleId,uint32_t TimerId);
        virtual ~MessageNotifiedTimer(){};

        void PrepareStartTimer();

        void ResetTimerPeriod(TimerPeriodUnit timerUnit, int64_t periodToTimeout);
        int64_t GetTimerPeriodInMicroSecond(){return this->m_PeriodToTimeoutInMicroSecond;}
        TimerPeriodUnit GetTimerPeriodUnit() {return this->m_TimerPeriodUnit;}
        TimingItemInfo& GetTimingItemInfos() {return this->m_TimingItemInfo;}
        bool m_IsRunning;  /**< Whether this timer is running */
    	bool IsRunning() const { return m_IsRunning; }
	    uint32_t IncreaseTimerInstanceId(){return ++m_TimerInstanceId;}
		uint32_t GetTimerInstanceId(){return this->m_TimerInstanceId;}

        TimerModuleRunner* GetTimerRunner(){return this->m_TimerQueueRunner;}
        void SetTimerRunner(TimerModuleRunner* timerModuleRunner){this->m_TimerQueueRunner = timerModuleRunner;}

        TimerRunnerQueue::iterator& GetTimerQueueIterator(){return this->m_CurrentIteratorInTimerQueue;}
        void SetTimerQueueIterator(TimerRunnerQueue::iterator timerQueueIterator){this->m_CurrentIteratorInTimerQueue = timerQueueIterator;}
    protected:
        int64_t                    m_PeriodToTimeoutInMicroSecond; /**< How much microseconds for this timing item to timeout */
        TimerPeriodUnit            m_TimerPeriodUnit;
        TimingItemInfo             m_TimingItemInfo;
        TimerModuleRunner*         m_TimerQueueRunner;             /**< Timer queue runner for this timer */
        TimerRunnerQueue::iterator m_CurrentIteratorInTimerQueue;  /**< Current timer queue iterator in m_TimerQueueRunner */
		uint32_t                   m_TimerInstanceId;              /**< Timer instance identity used to prevent false-timeout */
    };
    
    class TimerModuleRunner:public ModuleRunner
    {
    public:
        static bool InitTimerMechanisms();

        static bool StartTimer(MessageNotifiedTimer& timerToStart,void* timerSpecificData = NULL);
        static bool StopTimer(MessageNotifiedTimer& timerToStop);
    private:
        static TimerModuleRunner* m_MilliSecondTimerQueue;  /**< Minimal Period is half of a millisecond */
        static BasicTaskModule*   m_MilliSecondTimerTask;   

        static TimerModuleRunner* m_SecondTimerQueue;       /**< Minimal Period is half of a second */
        static BasicTaskModule*   m_SecondTimerTask;   

        static TimerModuleRunner* m_MinuteTimerQueue;       /**< Minimal Period is half of a minute */
        static BasicTaskModule*   m_MinuteTimerTask;   

    public:
        TimerModuleRunner(std::string moduleName,int64_t minialTimingTickPeriodInMicroSecond);
        ~TimerModuleRunner();
        RunnerInitResult InitRunner();
        uint32_t RunEntry();
        bool InsertTimingItem(TimingItemInfo& timingItemToInsert);
        bool RemoveTimingItem(TimerRunnerQueue::iterator& timingItemItor);
        bool KeepWorking;
    protected:
        TimerModuleRunner();
        TimerRunnerQueue m_TimingItemsQueue;                     /**< Queue to store the timing items */
        Mutex                     m_TimingItemsQueueProtectLock;          /**< Used to protect and lock m_TimingItemsQueue */
        WaitableTimingObject      m_TimingTick;                           /**< Used to count the minial timing tick of this timer module */
        int64_t                   m_MinialTimingTickPeriodInMicroSecond;  /**< Minial timing period of this timer module */

        void OnItemTimeout(TimingItemInfo theTimeoutTimerInfo);
    };

    struct PlatformTimerTimeoutIndMsg: public InterModuleMessageBase
    {
        uint32_t     m_TimerId;
		    uint32_t     m_TimerInstanceId;      /**< Timer instance identity used to prevent false-timeout */
		    void*        pTimerTimoutData;
        InterModuleMessageBase* Clone(){return new PlatformTimerTimeoutIndMsg(*this);};
    };

#define Timer_Start(timerToStart) TimerModuleRunner::StartTimer(timerToStart) 
#define Timer_Stop(timerToStop) TimerModuleRunner::StopTimer(timerToStop)
}

#endif
