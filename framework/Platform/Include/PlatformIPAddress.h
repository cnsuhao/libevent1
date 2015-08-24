#ifndef PLATFORMIPADDRESS_H
#define PLATFORMIPADDRESS_H

#ifdef _MSC_VER
    #include <Platform/Include/Windows/PlatformIPAddress_Win32.h>
#else
    #ifdef linux
        #include <Platform/Include/Linux/PlatformIPAddress_Linux.h>
    #else
        #ifdef __TI_COMPILER_VERSION__
            #include <Platform/Include/SysBios/PlatformIPAddress_SYSBIOS_NDK.h>
        #else
            #error Not supported Platform!
        #endif
    #endif    
#endif

namespace CommunicationPlatform
{
    class PlatformIPAddress: public PlatformIPAddressImplementation
    {
    public:
        PlatformIPAddress();
        virtual ~PlatformIPAddress();

        bool SetAddress(const std::string& ipAddress);

        std::string ToString() const;

        bool IsBroadcast() const;

        bool operator == (const PlatformIPAddress& addr);	

        bool operator != (const PlatformIPAddress& addr);	

        const void* AddressDataPointer() const;

    protected:
    };
}


#endif