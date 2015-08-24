/*************************************************************************
	> File Name: test.cpp
	> Author: cyp
	> Mail: caoyingpei12345@163.com 
	> Created Time: 2015年08月04日 星期二 17时01分20秒
 ************************************************************************/

#include <stdio.h>
#include <log4cpp/Category.hh>
#include <log4cpp/FileAppender.hh>
#include <log4cpp/OstreamAppender.hh>
#include <log4cpp/SimpleLayout.hh>

#define LOGFILE "./test.log"

int main() {
    /*Setting up Appender, layout and Category*/
    //输出文件位置 FileAppender RollingFileAppender
    log4cpp::Appender *appender = new log4cpp::FileAppender("FileAppender",LOGFILE);//第一个参数是appender的名字，第二个是log文件的名字
    log4cpp::Appender *ostreamappender = new log4cpp::OstreamAppender("FileAppender",LOGFILE);
    //输出方式 SimpleLayout BasicLayout PatternLayout
    //log4cpp::Layout *layout = new log4cpp::SimpleLayout();
    log4cpp::Layout *layout = new log4cpp::BasicLayout();
    log4cpp::Category& category = log4cpp::Category::getInstance("abc");
    //连接3者
    appender->setLayout(layout);
    category.setAppender(appender);
    //用于设置优先级
//    typedef enum {EMERG  = 0,
//    FATAL  = 0,
//            ALERT  = 100,
//            CRIT   = 200,
//            ERROR  = 300,
//            WARN   = 400,
//            NOTICE = 500,
//            INFO   = 600,
//            DEBUG  = 700,
//            NOTSET = 800
//} PriorityLevel;
    category.setPriority(log4cpp::Priority::DEBUG);

    /*The actual logging*/
    category.info("This is for tracing the flow");
    category.notice("This is to notify certain events");
    category.warn("This is to generate certain warnings");
}
