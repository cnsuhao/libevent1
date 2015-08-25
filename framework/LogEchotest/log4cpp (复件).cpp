#include "log4cpp/Category.hh"
#include "log4cpp/PropertyConfigurator.hh"

int main(int argc, char* argv[]) {
    // 1 读取解析配置文件
    // 读取出错, 完全可以忽略，可以定义一个缺省策略或者使用系统缺省策略
    // BasicLayout输出所有优先级日志到ConsoleAppender
//    try
//    {
//        log4cpp::PropertyConfigurator::configure("./log4cpp.conf");
//    }
//    catch (log4cpp::ConfigureFailure& f)
//    {
//        std::cout << "Configure Problem " << f.what() << std::endl;
//        return -1;
//    }
/*        typedef enum {EMERG  = 0,
        FATAL  = 0,
                ALERT  = 100,
                CRIT   = 200,
                ERROR  = 300,
                WARN   = 400,
                NOTICE = 500,
                INFO   = 600,
                DEBUG  = 700,
                NOTSET = 800
    } PriorityLevel;*/

    //2    实例化category对象
    //    这些对象即使配置文件没有定义也可以使用，不过其属性继承其父category
    //    通常使用引用可能不太方便，可以使用指针，以后做指针使用
    log4cpp::Category* root = &log4cpp::Category::getRoot();
    log4cpp::Category* sub1 = &log4cpp::Category::getInstance(std::string("sub1"));
    log4cpp::Category* LogHeartBeat = &log4cpp::Category::getInstance(std::string("LogHeartBeat"));
    log4cpp::Category* LogText = &log4cpp::Category::getInstance(std::string("LogText"));
    log4cpp::Category* thread0 = &log4cpp::Category::getInstance(std::string("thread0"));
    log4cpp::Category* thread1 = &log4cpp::Category::getInstance(std::string("thread1"));
    log4cpp::Category* thread2 = &log4cpp::Category::getInstance(std::string("thread2"));
    log4cpp::Category* thread3 = &log4cpp::Category::getInstance(std::string("thread3"));
    log4cpp::Category* thread4 = &log4cpp::Category::getInstance(std::string("thread4"));
    log4cpp::Category* thread5 = &log4cpp::Category::getInstance(std::string("thread5"));
    log4cpp::Category* thread6 = &log4cpp::Category::getInstance(std::string("thread6"));
    log4cpp::Category* thread7 = &log4cpp::Category::getInstance(std::string("thread7"));
    log4cpp::Category* thread8 = &log4cpp::Category::getInstance(std::string("thread8"));
    log4cpp::Category* thread9 = &log4cpp::Category::getInstance(std::string("thread9"));

    //    正常使用这些category对象进行日志处理。
    int a=1;
    root->fatal("root's log");

    //    sub1 has appender A1 and rootappender. since the additivity property is set true by default
    sub1->info("sub1's log");

    //    sub2 has appender A2 appender. since the additivity property is set to false
    LogHeartBeat->alert("LogHeartBeat's log");
    LogHeartBeat->alert("LogHeartBeat's log");
    LogHeartBeat->alert("LogHeartBeat's log");
    LogHeartBeat->alert("LogHeartBeat's log");
    LogHeartBeat->alert("LogHeartBeat's log");
    LogHeartBeat->alert("LogHeartBeat's log");
    LogHeartBeat->alert("LogHeartBeat's log");
    LogHeartBeat->alert("LogHeartBeat's log");
    LogHeartBeat->alert("LogHeartBeat's log");

    //    sub3 only has A3 appender. since the additivity property is set to false
    LogText->debug("sub3's log");
    LogText->alert("sub3's log");

    //    sub4 can not be found in the config file, so the root category's appender and layout are used
    thread0->warn("xxx log");
    thread1->warn("xxx log");
    thread2->warn("xxx log");
    thread3->warn("xxx log");
    thread4->warn("xxx log");
    thread5->warn("xxx log");
    thread6->warn("xxx log");
    thread7->warn("xxx log");
    thread8->warn("xxx log");
    thread9->warn("xxx log");


    return 0;
}
