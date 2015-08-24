#include "stdio.h"
#include "tinyxml.h"
//______________________________________________________________________
// Read information from xml file.

// define xml file path, as follow , we use relative path,
// but you can use absolute path also.
struct CpmXmlConfig{
    char ip_addr[32];
    char data_port[32];
    char update_port[32];
};
int main()
{
        CpmXmlConfig xmlc;
        const char* filepath = "config.xml";
        TiXmlDocument doc(filepath);
        bool loadOkay = doc.LoadFile();
        // faile to load 'phonebookdata.xml'.
        if (!loadOkay) {
                  printf( "Could not load test file %s. Error='%s'. Exiting.\n", filepath,doc.ErrorDesc() );
                  exit( 1 );
        }

        // get dom root of 'phonebookdata.xml', here root should be 'phonebook'.
        TiXmlElement* root = doc.RootElement();

        printf("_______________________________________\n\n");
        printf("     contacted person information      \n\n");
        // trace every items below root.
        for( TiXmlNode*  item = root->FirstChild( "item" );
                       item;
                       item = item->NextSibling( "item" ) ) {
                  printf("_______________________________________\n");

                  // read ip_addr
                  TiXmlNode* child = item->FirstChild();
                  strcpy(xmlc.ip_addr ,child->ToElement()->GetText());
                  if (xmlc.ip_addr) {
                      printf("ip_addr:%s\n", xmlc.ip_addr );
                  } else {
                      printf("ip_addr:\n");
                  }

                  // read data_port.
                  child = item->IterateChildren(child);
                  strcpy(xmlc.data_port , child->ToElement()->GetText());
                  if (xmlc.data_port) {
                      printf("data_port:%s\n",xmlc.data_port);
                  } else {
                      printf("data_port:\n");
                  }


                  // read telephone no.
                  child = item->IterateChildren(child);
                  strcpy(xmlc.update_port, child->ToElement()->GetText());
                      if (xmlc.update_port) {
                      printf("tel:%s\n",xmlc.update_port);
                  } else {
                      printf("tel:\n");
                  }

                  printf("\n");

        }
        //______________________________________________________________________


        //______________________________________________________________________
        // Add information to xml file and save it.
        /*
        TiXmlElement* writeRoot = doc.RootElement();
        TiXmlNode* newNode = new TiXmlElement("item");

                 const TiXmlNode* name4NewNode = new TiXmlElement("name");
        newNode->InsertEndChild(*name4NewNode)->InsertEndChild(TiXmlText("pipi"));

        const TiXmlNode* addr4NewNode = new TiXmlElement("addr");
        newNode->InsertEndChild(*addr4NewNode)->InsertEndChild(TiXmlText("Shaanxi Xianyang"));

        const TiXmlNode* tel4NewNode = new TiXmlElement("tel");
        newNode->InsertEndChild(*tel4NewNode)->InsertEndChild(TiXmlText("02937310627"));

        const TiXmlNode* email4NewNode = new TiXmlElement("email");
        newNode->InsertEndChild(*email4NewNode)->InsertEndChild(TiXmlText("pipi@home.com"));

        writeRoot->InsertEndChild(*newNode);
        doc.SaveFile();  */
        return 0;
}
//__
