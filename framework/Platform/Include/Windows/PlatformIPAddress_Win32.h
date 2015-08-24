#ifndef PLATFORMIPADDRESS_WIN32_H
#define PLATFORMIPADDRESS_WIN32_H

#include <string>
#include <Platform/Include/Windows/WindowsSpecificHeaders.h>

namespace CommunicationPlatform
{
    class PlatformIPAddressImplementation
    {
    public:
        PlatformIPAddressImplementation();
        virtual ~PlatformIPAddressImplementation();


        bool SetAddress(const std::string& ipAddress);

        bool SetAddressImple(const in_addr& ipAddress);

        std::string ToString() const;

        bool IsBroadcast() const;

        const void* AddressDataPointer() const;

        uint32_t GetAddressLength();

    protected:
    private:
        in_addr m_IPAddress;
    };
}


#endif