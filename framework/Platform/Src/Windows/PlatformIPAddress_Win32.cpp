#include <string>

#include <Platform/Include/Windows/PlatformIPAddress_Win32.h>


namespace CommunicationPlatform
{
    PlatformIPAddressImplementation::PlatformIPAddressImplementation()
    {
        this->m_IPAddress.s_addr = INADDR_ANY;
    }

    PlatformIPAddressImplementation::~PlatformIPAddressImplementation()
    {

    }


    bool PlatformIPAddressImplementation::SetAddress(const std::string& ipAddress)
    {
        this->m_IPAddress.s_addr = inet_addr(ipAddress.c_str());

        /* If no error occurs, inet_addr returns an unsigned long value containing a suitable binary representation of the 
           Internet address given. If the string in the cp parameter does not contain a legitimate Internet address, for example 
           if a portion of an "a.b.c.d" address exceeds 255, then inet_addr returns the value INADDR_NONE. */
        if ((INADDR_NONE == this->m_IPAddress.s_addr) && (ipAddress!= "255.255.255.255") )
        {
            this->m_IPAddress.s_addr = INADDR_ANY;
            return false;
        } 
        else
        {
            return true;
        }
    }

    bool PlatformIPAddressImplementation::SetAddressImple(const in_addr& ipAddress)
    {
        this->m_IPAddress = ipAddress;

        return true;
    }


    std::string PlatformIPAddressImplementation::ToString() const
    {
        std::string formatedString;
        formatedString.append(this->m_IPAddress.S_un.S_un_b.s_b1,1);
        formatedString.append(this->m_IPAddress.S_un.S_un_b.s_b2,1);
        formatedString.append(this->m_IPAddress.S_un.S_un_b.s_b3,1);
        formatedString.append(this->m_IPAddress.S_un.S_un_b.s_b4,1);

        return formatedString;
    }

    bool PlatformIPAddressImplementation::IsBroadcast() const
    {
        if (this->m_IPAddress.s_addr == INADDR_BROADCAST)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    const void* PlatformIPAddressImplementation::AddressDataPointer() const
    {
        return &(this->m_IPAddress.s_addr);
    }

    uint32_t PlatformIPAddressImplementation::GetAddressLength()
    {
        return sizeof(in_addr);
    }
}