#include <iostream>
#include "Xml.h"

void LoadXML()
{
    CXML xml;
    std::string result;
    xml.ParseXmlFile("ServerConfig.xml");
    TiXmlElement * pParentElem, *pElement;
    //////////////////////////////////////////////////////////////////////////
    std::cout<<"测试一：顺序遍历查找:"<<std::endl;
    pParentElem = xml.GetRootElement();
    std::cout<<"ServerName:"<<xml.GetChildElementValue(pParentElem,"ServerName").c_str()<<std::endl;
    std::cout<<"ServerIP:"<<xml.GetChildElementValue(pParentElem,"ServerIP").c_str()<<std::endl;
    std::cout<<"ListenPortID:"<<xml.GetChildElementValue(pParentElem,"ListenPortID").c_str()<<std::endl;
    result = xml.GetChildElementAttributeValue(pParentElem,"PortsList","Num");
    std::cout<<"PortsList Num:"<<result<<std::endl;
    pParentElem = xml.GetChildElement(pParentElem,"PortsList");
    pElement = xml.GetFirstChildElement(pParentElem);
    while(pElement)
    {
        std::cout<<"================================================================="<<std::endl;
        std::cout<<"Port:"<<std::endl;
        std::cout<<"PortID:"<<xml.GetChildElementValue(pElement,"PortID").c_str()<<std::endl;
        std::cout<<"MaxConnection:"<<xml.GetChildElementValue(pElement,"MaxConnection").c_str()<<std::endl;
        pElement = xml.GetNextChildElement(pElement);
    }
    //////////////////////////////////////////////////////////////////////////
    std::cout<<"测试二：递归遍历查找:"<<std::endl;
    pElement = xml.FindFirstElement("Port");
    while(pElement)
    {
        std::cout<<"================================================================="<<std::endl;
        std::cout<<"Port:"<<std::endl;
        std::cout<<"PortID:"<<xml.GetChildElementValue(pElement,"PortID").c_str()<<std::endl;
        std::cout<<"MaxConnection:"<<xml.GetChildElementValue(pElement,"MaxConnection").c_str()<<std::endl;
        pElement = xml.GetNextChildElement(pElement);
    }
    //////////////////////////////////////////////////////////////////////////
    std::cout<<"测试三：替换:"<<std::endl;
    pElement = xml.FindFirstElement("ServerName");
    xml.ReplaceElementValue(pElement,"新服务器");
    std::cout<<"ServerName:"<<xml.GetElementValue(pElement).c_str()<<std::endl;
    pElement = xml.FindFirstElement("PortsList");
    xml.ReplaceElementAttribute(pElement,"Num","10");
    std::cout<<"PortsList Num:"<<xml.GetElementAttributeValue(pElement,"Num").c_str()<<std::endl;
    xml.SaveFile("ServerConfig.xml");
}

void CreateXML()
{
    CXML xml;
    xml.AddXmlDeclaration();
    TiXmlElement* root = xml.AddXmlRootElement("ServerInfo");
    xml.AddXmlComment(root,"服务器配置信息");
    TiXmlElement * pElement = xml.AddXmlChildElement(root,"ServerName");
    xml.AddElementValue(pElement,"旧服务器");
    pElement = xml.AddXmlChildElement(root,"ServerIP");
    xml.AddElementValue(pElement,"192.168.14.28");
    pElement = xml.AddXmlChildElement(root,"ListenPortID");
    xml.AddElementValue(pElement,"8080");
    pElement = xml.AddXmlChildElement(root,"PortsList");
    xml.AddXmlAttribute(pElement,"Num","5");
    root = pElement;
    char cPortID[10];
    for (int i = 0; i < 5; i++)
    {
        ::memset(cPortID,'\0',10);
        ::sprintf(cPortID,"%d",20000 + i + 1);
        pElement = xml.AddXmlChildElement(root,"Port");
        xml.AddXmlChildElement(pElement,"PortID",cPortID);
        ::sprintf(cPortID,"%d",60);
        xml.AddXmlChildElement(pElement,"MaxConnection",cPortID);
    }
    xml.SaveFile("ServerConfig.xml");
}

int main(int argc, char * argv[])
{
    CreateXML();
    LoadXML();
//    getchar();
    return 0;
}
