#ifndef PLATFORMEVENT_H
#define PLATFORMEVENT_H

#ifdef _MSC_VER
    #include <Platform/Include/Windows/PlatformEvent_Win32.h>
#else
    #ifdef linux
        #include <Platform/Include/Linux/PlatformEvent_Linux.h>
    #else
        #ifdef __TI_COMPILER_VERSION__
            #include <Platform/Include/SysBios/PlatformEvent_SysBios.h>
        #else
            #error Not supported Platform!
        #endif
    #endif    
#endif

namespace CommunicationPlatform
{
    class Event: protected EventImplementation
        /// An Event is a synchronization object that
        /// allows one thread to signal one or more
        /// other threads that a certain event
        /// has happened.
        /// Usually, one thread signals an event,
        /// while one or more other threads wait
        /// for an event to become signalled.
    {
    public:
        Event(bool autoReset = true);
        /// Creates the event. If autoReset is true,
        /// the event is automatically reset after
        /// a wait() successfully returns.

        ~Event();
        /// Destroys the event.

        void set();
        /// Signals the event. If autoReset is true,
        /// only one thread waiting for the event 
        /// can resume execution.
        /// If autoReset is false, all waiting threads
        /// can resume execution.

        bool wait();
        /// Waits for the event to become signalled.

        bool wait(long milliseconds);
        /// Waits for the event to become signalled.
        /// Throws a TimeoutException if the event
        /// does not become signalled within the specified
        /// time interval.

        bool tryWait(long milliseconds);
        /// Waits for the event to become signalled.
        /// Returns true if the event
        /// became signalled within the specified
        /// time interval, false otherwise.

        void reset();
        /// Resets the event to unsignalled state.

    private:
        Event(const Event&);
        Event& operator = (const Event&);
    };


    //
    // inlines
    //
    inline void Event::set()
    {
        setImpl();
    }


    inline bool Event::wait()
    {
        return waitImpl();
    }


    inline bool Event::wait(long milliseconds)
    {
        return waitImpl(milliseconds);
    }


    inline bool Event::tryWait(long milliseconds)
    {
        return waitImpl(milliseconds);
    }


    inline void Event::reset()
    {
        resetImpl();
    }

}



#endif
