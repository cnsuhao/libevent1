#include <Platform/Include/PlatformMemoryManagment.h>
#include <Platform/Include/PlatformMutex.h>

namespace CommunicationPlatform
{
    RefCountedObject::RefCountedObject(): _counter(1)
    {
    }


    RefCountedObject::~RefCountedObject()
    {
    }

#ifdef _MSC_VER
    //
    // Windows
    //
    AtomicCounter::AtomicCounter():
_counter(0)
{
}


AtomicCounter::AtomicCounter(AtomicCounter::ValueType initialValue):
_counter(initialValue)
{
}


AtomicCounter::AtomicCounter(const AtomicCounter& counter):
_counter(counter.value())
{
}


AtomicCounter::~AtomicCounter()
{
}


AtomicCounter& AtomicCounter::operator = (const AtomicCounter& counter)
{
    InterlockedExchange(&_counter, counter.value());
    return *this;
}


AtomicCounter& AtomicCounter::operator = (AtomicCounter::ValueType value)
{
    InterlockedExchange(&_counter, value);
    return *this;
}
#else
    #ifdef linux
        AtomicCounter::AtomicCounter()
        {
            _counter.value = 0;
        }


        AtomicCounter::AtomicCounter(AtomicCounter::ValueType initialValue)
        {
            _counter.value = initialValue;
        }


        AtomicCounter::AtomicCounter(const AtomicCounter& counter)
        {
            _counter.value = counter.value();
        }


        AtomicCounter::~AtomicCounter()
        {
        }


        AtomicCounter& AtomicCounter::operator = (const AtomicCounter& counter)
        {
            Mutex::ScopedMutex lock(_counter.mutex);
            _counter.value = counter.value();
            return *this;
        }


        AtomicCounter& AtomicCounter::operator = (AtomicCounter::ValueType value)
        {
            Mutex::ScopedMutex lock(_counter.mutex);
            _counter.value = value;
            return *this;
        }
    #else
    #endif
#endif

}
