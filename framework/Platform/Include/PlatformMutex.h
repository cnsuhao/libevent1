#ifndef PLATFORMMUTEX_H
#define PLATFORMMUTEX_H

#ifdef _MSC_VER
    #include <Platform/Include/Windows/PlatformMutex_Win32.h>
#else
    #ifdef linux
        #include <Platform/Include/Linux/PlatformMutex_Linux.h>
    #else
        #ifdef __TI_COMPILER_VERSION__
            #include <Platform/Include/SysBios/PlatformMutex_SysBios.h>
        #else
            #error Not supported Platform!
        #endif
    #endif    
#endif

namespace CommunicationPlatform
{
    template <class M>
    class ScopedLock

        /// A class that simplifies thread synchronization
        /// with a mutex.
        /// The constructor accepts a Mutex and locks it.
        /// The destructor unlocks the mutex.
    {
    public:
        ScopedLock(M& mutex): m_Mutex(mutex)
        {
            m_Mutex.Lock();
        }

        ~ScopedLock()
        {
            m_Mutex.Unlock();
        }

    private:
        M& m_Mutex;

        ScopedLock();
        ScopedLock(const ScopedLock&);
        ScopedLock& operator = (const ScopedLock&);
    };

    class Mutex: private MutexImplementation
    {
    public:
        Mutex();

        virtual ~Mutex();

        typedef ScopedLock<Mutex> ScopedMutex;

        void Lock()
        {
            MutexImplementation::Lock();
        }

		bool TryLock()
		{
			return MutexImplementation::TryLock();
		}

        void Unlock()
        {
            MutexImplementation::Unlock();
        }

    protected:
    };
}


#endif
