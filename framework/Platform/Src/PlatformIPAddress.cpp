#include <string>
#include <Platform/Include/PlatformIPAddress.h>


namespace CommunicationPlatform
{
    PlatformIPAddress::PlatformIPAddress()
        :PlatformIPAddressImplementation()
    {

    }

    PlatformIPAddress::~PlatformIPAddress()
    {

    }

    bool PlatformIPAddress::SetAddress(const std::string& ipAddress)
    {
        return PlatformIPAddressImplementation::SetAddress(ipAddress);
    }

    std::string PlatformIPAddress::ToString() const
    {
        return PlatformIPAddressImplementation::ToString();
    }

    bool PlatformIPAddress::IsBroadcast() const
    {
        return PlatformIPAddressImplementation::IsBroadcast();
    }

    bool PlatformIPAddress::operator == (const PlatformIPAddress& addr)
    {
        if (0 == memcmp(AddressDataPointer(),addr.AddressDataPointer(),GetAddressLength()))
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    bool PlatformIPAddress::operator != (const PlatformIPAddress& addr)
    {
        return !(*this == addr);
    }

    const void* PlatformIPAddress::AddressDataPointer() const
    {
        return PlatformIPAddressImplementation::AddressDataPointer();
    }

}