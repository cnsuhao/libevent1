#include <string>

#include <Platform/Include/Linux/PlatformIPAddress_Linux.h>


namespace CommunicationPlatform
{
    PlatformIPAddressImplementation::PlatformIPAddressImplementation()
    {
        this->m_IPAddress.s_addr = htonl(INADDR_ANY);
    }

    PlatformIPAddressImplementation::~PlatformIPAddressImplementation()
    {

    }


    bool PlatformIPAddressImplementation::SetAddress(const std::string& ipAddress)
    {
		/* inet_pton()  returns  1  on success (network address was successfully converted).  
		   0 is returned if src does not contain  a  character  string  representing  a  valid  network address  in  the specified address family.  
		   If af does not contain a valid address family, -1 is returned and errno is set to EAFNOSUPPORT. */
         if (1 == inet_pton(AF_INET,ipAddress.c_str(),&(this->m_IPAddress.s_addr)))
         {
			 return true;
         }
		 else
		 {
			 return false;
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

		char tmpIPAddressString[INET_ADDRSTRLEN];

		/*  On  success,  inet_ntop() returns a non-NULL pointer to dst.  
		    NULL is returned if there was an error, with errno set to indicate the error.*/
		if (NULL != inet_ntop(AF_INET,&this->m_IPAddress,tmpIPAddressString,sizeof(tmpIPAddressString)))
		{
			formatedString.assign(tmpIPAddressString);
		} 
		
        return formatedString;
    }

    bool PlatformIPAddressImplementation::IsBroadcast() const
    {
        in_addr tmpBroadCastAddress;
		tmpBroadCastAddress.s_addr = htonl(INADDR_BROADCAST);

		if (this->m_IPAddress.s_addr == tmpBroadCastAddress.s_addr)
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