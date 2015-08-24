#ifndef PLATFORMMEMORYMANAGMENT_H
#define PLATFORMMEMORYMANAGMENT_H

#include <algorithm>

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

#include <Platform/Include/PlatformMutex.h>

namespace CommunicationPlatform
{
#ifdef _MSC_VER
#define ALIGNMENT_SETTING __declspec(align(4)) //The variable pointed to by the Addend parameter must be aligned on a 32-bit boundary; otherwise, this function will behave unpredictably on multiprocessor x86 systems and any non-x86 systems.
#else
#define ALIGNMENT_SETTING
#endif

    class ALIGNMENT_SETTING AtomicCounter
        /// This class implements a simple counter, which
        /// provides atomic operations that are safe to
        /// use in a multithreaded environment.
        ///
        /// Typical usage of AtomicCounter is for implementing
        /// reference counting and similar things.
        ///
        /// On some platforms, the implementation of AtomicCounter
        /// is based on atomic primitives specific to the platform
        /// (such as InterlockedIncrement, etc. on Windows), and
        /// thus very efficient. On platforms that do not support
        /// atomic primitives, operations are guarded by a Mutex.
        ///
        /// The following platforms currently have atomic
        /// primitives:
        ///   - Windows
        ///   - Mac OS X
    {
    public:
        typedef int ValueType; /// The underlying integer type.

        AtomicCounter();
        /// Creates a new AtomicCounter and initializes it to zero.

        explicit AtomicCounter(ValueType initialValue);
        /// Creates a new AtomicCounter and initializes it with
        /// the given value.

        AtomicCounter(const AtomicCounter& counter);
        /// Creates the counter by copying another one.

        ~AtomicCounter();
        /// Destroys the AtomicCounter.

        AtomicCounter& operator = (const AtomicCounter& counter);
        /// Assigns the value of another AtomicCounter.

        AtomicCounter& operator = (ValueType value);
        /// Assigns a value to the counter.

        operator ValueType () const;
        /// Returns the value of the counter.

        ValueType value() const;
        /// Returns the value of the counter.

        ValueType operator ++ (); // prefix
        /// Increments the counter and returns the result.

        ValueType operator ++ (int); // postfix
        /// Increments the counter and returns the previous value.

        ValueType operator -- (); // prefix
        /// Decrements the counter and returns the result.

        ValueType operator -- (int); // postfix
        /// Decrements the counter and returns the previous value.

        bool operator ! () const;
        /// Returns true if the counter is zero, false otherwise.

    private:
#ifdef _MSC_VER
        typedef volatile LONG ImplType; //The variable pointed to by the Addend parameter must be aligned on a 32-bit boundary; otherwise, this function will behave unpredictably on multiprocessor x86 systems and any non-x86 systems.
#else 
    #if defined(linux) || defined(__TI_COMPILER_VERSION__)
            // generic implementation based on Mutex
            struct ImplType
            {
                mutable Mutex mutex;
                volatile int      value;
            };
    #else
        #error Not supported Platform!
    #endif
#endif 
        ImplType _counter;
    };

#ifdef _MSC_VER
    //
    // Windows
    //
    inline AtomicCounter::operator AtomicCounter::ValueType () const
    {
        return _counter;
    }


    inline AtomicCounter::ValueType AtomicCounter::value() const
    {
        return _counter;
    }


    inline AtomicCounter::ValueType AtomicCounter::operator ++ () // prefix
    {
        return InterlockedIncrement(&_counter);
    }


    inline AtomicCounter::ValueType AtomicCounter::operator ++ (int) // postfix
    {
        ValueType result = InterlockedIncrement(&_counter);
        return --result;
    }


    inline AtomicCounter::ValueType AtomicCounter::operator -- () // prefix
    {
        return InterlockedDecrement(&_counter);
    }


    inline AtomicCounter::ValueType AtomicCounter::operator -- (int) // postfix
    {
        ValueType result = InterlockedDecrement(&_counter);
        return ++result;
    }


    inline bool AtomicCounter::operator ! () const
    {
        return _counter == 0;
    }
#else

#if defined(linux) || defined(__TI_COMPILER_VERSION__)
    //
    // Generic implementation based on Mutex
    //
    inline AtomicCounter::operator AtomicCounter::ValueType () const
    {
        ValueType result;
        {
            Mutex::ScopedMutex lock(_counter.mutex);
            result = _counter.value;
        }
        return result;
    }


    inline AtomicCounter::ValueType AtomicCounter::value() const
    {
        ValueType result;
        {
            Mutex::ScopedMutex lock(_counter.mutex);
            result = _counter.value;
        }
        return result;
    }


    inline AtomicCounter::ValueType AtomicCounter::operator ++ () // prefix
    {
        ValueType result;
        {
            Mutex::ScopedMutex lock(_counter.mutex);
            result = ++_counter.value;
        }
        return result;
    }


    inline AtomicCounter::ValueType AtomicCounter::operator ++ (int) // postfix
    {
        ValueType result;
        {
            Mutex::ScopedMutex lock(_counter.mutex);
            result = _counter.value++;
        }
        return result;
    }


    inline AtomicCounter::ValueType AtomicCounter::operator -- () // prefix
    {
        ValueType result;
        {
            Mutex::ScopedMutex lock(_counter.mutex);
            result = --_counter.value;
        }
        return result;
    }


    inline AtomicCounter::ValueType AtomicCounter::operator -- (int) // postfix
    {
        ValueType result;
        {
            Mutex::ScopedMutex lock(_counter.mutex);
            result = _counter.value--;
        }
        return result;
    }


    inline bool AtomicCounter::operator ! () const
    {
        bool result;
        {
            Mutex::ScopedMutex lock(_counter.mutex);
            result = _counter.value == 0;
        }
        return result;
    }
#else
    #error Not supported Platform!
#endif

#endif //_MSC_VER


class RefCountedObject
    /// A base class for objects that employ
    /// reference counting based garbage collection.
    ///
    /// Reference-counted objects inhibit construction
    /// by copying and assignment.
{
public:
    RefCountedObject();
    /// Creates the RefCountedObject.
    /// The initial reference count is one.

    void duplicate() const;
    /// Increments the object's reference count.

    void release() const;
    /// Decrements the object's reference count
    /// and deletes the object if the count
    /// reaches zero.

    int referenceCount() const;
    /// Returns the reference count.

protected:
    virtual ~RefCountedObject();
    /// Destroys the RefCountedObject.

private:
    RefCountedObject(const RefCountedObject&);
    RefCountedObject& operator = (const RefCountedObject&);

    mutable AtomicCounter _counter;
};


//
// inlines
//
inline int RefCountedObject::referenceCount() const
{
    return _counter.value();
}


inline void RefCountedObject::duplicate() const
{
    ++_counter;
}


inline void RefCountedObject::release() const
{
    if (--_counter == 0) delete this;
}

template <class C>
class AutoPtr
    /// AutoPtr is a "smart" pointer for classes implementing
    /// reference counting based garbage collection.
    /// To be usable with the AutoPtr template, a class must
    /// implement the following behaviour:
    /// A class must maintain a reference count.
    /// The constructors of the object initialize the reference
    /// count to one.
    /// The class must implement a public duplicate() method:
    ///     void duplicate();
    /// that increments the reference count by one.
    /// The class must implement a public release() method:
    ///     void release()
    /// that decrements the reference count by one, and,
    /// if the reference count reaches zero, deletes the
    /// object.
    ///
    /// AutoPtr works in the following way:
    /// If an AutoPtr is assigned an ordinary pointer to
    /// an object (via the constructor or the assignment operator),
    /// it takes ownership of the object and the object's reference 
    /// count remains unchanged.
    /// If the AutoPtr is assigned another AutoPtr, the
    /// object's reference count is incremented by one by
    /// calling duplicate() on its object.
    /// The destructor of AutoPtr calls release() on its
    /// object.
    /// AutoPtr supports dereferencing with both the ->
    /// and the * operator. An attempt to dereference a null
    /// AutoPtr results in a NullPointerException being thrown.
    /// AutoPtr also implements all relational operators.
    /// Note that AutoPtr allows casting of its encapsulated data types.
{
public:
    AutoPtr(): _ptr(0)
    {
    }

    AutoPtr(C* ptr): _ptr(ptr)
    {
    }

    AutoPtr(C* ptr, bool shared): _ptr(ptr)
    {
        if (shared && _ptr) _ptr->duplicate();
    }

    AutoPtr(const AutoPtr& ptr): _ptr(ptr._ptr)
    {
        if (_ptr) _ptr->duplicate();
    }

    template <class Other> 
    AutoPtr(const AutoPtr<Other>& ptr): _ptr(const_cast<Other*>(ptr.get()))
    {
        if (_ptr) _ptr->duplicate();
    }

    ~AutoPtr()
    {
        if (_ptr) _ptr->release();
    }

    AutoPtr& assign(C* ptr)
    {
        if (_ptr != ptr)
        {
            if (_ptr) _ptr->release();
            _ptr = ptr;
        }
        return *this;
    }

    AutoPtr& assign(C* ptr, bool shared)
    {
        if (_ptr != ptr)
        {
            if (_ptr) _ptr->release();
            _ptr = ptr;
            if (shared && _ptr) _ptr->duplicate();
        }
        return *this;
    }

    AutoPtr& assign(const AutoPtr& ptr)
    {
        if (&ptr != this)
        {
            if (_ptr) _ptr->release();
            _ptr = ptr._ptr;
            if (_ptr) _ptr->duplicate();
        }
        return *this;
    }

    template <class Other> 
    AutoPtr& assign(const AutoPtr<Other>& ptr)
    {
        if (ptr.get() != _ptr)
        {
            if (_ptr) _ptr->release();
            _ptr = const_cast<Other*>(ptr.get());
            if (_ptr) _ptr->duplicate();
        }
        return *this;
    }

    AutoPtr& operator = (C* ptr)
    {
        return assign(ptr);
    }

    AutoPtr& operator = (const AutoPtr& ptr)
    {
        return assign(ptr);
    }

    template <class Other> 
    AutoPtr& operator = (const AutoPtr<Other>& ptr)
    {
        return assign<Other>(ptr);
    }

    void swap(AutoPtr& ptr)
    {
        std::swap(_ptr, ptr._ptr);
    }

    template <class Other> 
    AutoPtr<Other> unsafeCast() const
        /// Casts the AutoPtr via a static cast to the given type.
        /// Example: (assume class Sub: public Super)
        ///    AutoPtr<Super> super(new Sub());
        ///    AutoPtr<Sub> sub = super.unsafeCast<Sub>();
        ///    poco_assert (sub.get());
    {
        Other* pOther = static_cast<Other*>(_ptr);
        return AutoPtr<Other>(pOther, true);
    }

    C* operator -> ()
    {
        return _ptr;
    }

    const C* operator -> () const
    {
        return _ptr;
    }

    C& operator * ()
    {
        return *_ptr;
    }

    const C& operator * () const
    {
        return *_ptr;
    }

    C* get()
    {
        return _ptr;
    }

    const C* get() const
    {
        return _ptr;
    }

    operator C* ()
    {
        return _ptr;
    }

    operator const C* () const
    {
        return _ptr;
    }

    bool operator ! () const
    {
        return _ptr == 0;
    }

    bool isNull() const
    {
        return _ptr == 0;
    }

    C* duplicate()
    {
        if (_ptr) _ptr->duplicate();
        return _ptr;
    }

    bool operator == (const AutoPtr& ptr) const
    {
        return _ptr == ptr._ptr;
    }

    bool operator == (const C* ptr) const
    {
        return _ptr == ptr;
    }

    bool operator == (C* ptr) const
    {
        return _ptr == ptr;
    }

    bool operator != (const AutoPtr& ptr) const
    {
        return _ptr != ptr._ptr;
    }

    bool operator != (const C* ptr) const
    {
        return _ptr != ptr;
    }

    bool operator != (C* ptr) const
    {
        return _ptr != ptr;
    }

    bool operator < (const AutoPtr& ptr) const
    {
        return _ptr < ptr._ptr;
    }

    bool operator < (const C* ptr) const
    {
        return _ptr < ptr;
    }

    bool operator < (C* ptr) const
    {
        return _ptr < ptr;
    }

    bool operator <= (const AutoPtr& ptr) const
    {
        return _ptr <= ptr._ptr;
    }

    bool operator <= (const C* ptr) const
    {
        return _ptr <= ptr;
    }

    bool operator <= (C* ptr) const
    {
        return _ptr <= ptr;
    }

    bool operator > (const AutoPtr& ptr) const
    {
        return _ptr > ptr._ptr;
    }

    bool operator > (const C* ptr) const
    {
        return _ptr > ptr;
    }

    bool operator > (C* ptr) const
    {
        return _ptr > ptr;
    }

    bool operator >= (const AutoPtr& ptr) const
    {
        return _ptr >= ptr._ptr;
    }

    bool operator >= (const C* ptr) const
    {
        return _ptr >= ptr;
    }

    bool operator >= (C* ptr) const
    {
        return _ptr >= ptr;
    }

private:
    C* _ptr;
};


template <class C>
inline void swap(AutoPtr<C>& p1, AutoPtr<C>& p2)
{
    p1.swap(p2);
}

}



#endif
