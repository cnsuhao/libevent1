#pragma once
#pragma once
#include<string> 
#include <iostream>
#include "tinyxml.h" 
using namespace std; 
class CXML
{
public:
    CXML(void);
    ~CXML(void); 
    bool ParseXmlFile(const char* xmlFile); 
    void Clear(); 
    void SaveFile(const char* file); 
    //////////////////////////////////////////////////////////////////////////
    TiXmlElement * GetRootElement();
    TiXmlElement * GetChildElement(TiXmlElement * pParentElement,const char * title);
    TiXmlElement * GetFirstChildElement(TiXmlElement * pParentElement); 
    TiXmlElement * GetNextChildElement(TiXmlElement * pElement); 
    //////////////////////////////////////////////////////////////////////////
    TiXmlElement * FindFirstElement(const char * title);//递归查找
    //////////////////////////////////////////////////////////////////////////
    std::string GetElementValue(TiXmlElement * pElement);
    std::string GetElementAttributeValue(TiXmlElement* Element,const char* AttributeName); 
    std::string GetChildElementValue(TiXmlElement * pParentElement,const char * title);
    std::string GetChildElementAttributeValue(TiXmlElement * pParentElement,const char * title,const char* AttributeName); 
    //////////////////////////////////////////////////////////////////////////
    TiXmlElement* AddXmlRootElement(const char* title);
    TiXmlElement* AddXmlChildElement(TiXmlElement* pPareElement,const char* title,const char * value = "\0"); 
    void AddXmlAttribute(TiXmlElement* pElement,const char* name,const char* value); 
    void AddXmlDeclaration(const char* vesion="1.0",const char* encoding="UTF-8",const char* standalone="");
    void AddElementValue(TiXmlElement * pElement,const char* value); 
    void AddXmlComment(TiXmlElement* pElement,const char* Comment); 
    //////////////////////////////////////////////////////////////////////////
    bool ReplaceElementValue(TiXmlElement * pElement,const char * newValue);
    bool ReplaceElementAttribute(TiXmlElement* pElement,const char * name,const char * newValue);
    //////////////////////////////////////////////////////////////////////////
private:
    TiXmlElement * FindFirstElement(TiXmlElement* pcrElement,const char * title);//递归查找

private: 
    TiXmlDocument m_xml; 
};
