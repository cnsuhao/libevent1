#include "log4cpp/Category.hh"
#include "log4cpp/PropertyConfigurator.hh"
struct LOG_global
{
    log4cpp::Category *LogRoot;
    log4cpp::Category *LogSub1 ;
    log4cpp::Category *LogHeart;
    log4cpp::Category *LogTxt ;
    log4cpp::Category *LogThread0;
    log4cpp::Category *LogThread1;
    log4cpp::Category *LogThread2;
    log4cpp::Category *LogThread3;
    log4cpp::Category *LogThread4;
    log4cpp::Category *LogThread5;
    log4cpp::Category *LogThread6;
    log4cpp::Category *LogThread7;
    log4cpp::Category *LogThread8;
    log4cpp::Category *LogThread9;

};
int log_init(LOG_global *m_logo){
    // 1 读取解析配置文件
    // 读取出错, 完全可以忽略，可以定义一个缺省策略或者使用系统缺省策略
    // BasicLayout输出所有优先级日志到ConsoleAppender
    try
    {
        log4cpp::PropertyConfigurator::configure("./log4cpp.conf");
    }
    catch (log4cpp::ConfigureFailure& f)
    {
        std::cout << "Configure Problem " << f.what() << std::endl;
        return -1;
    }
    log4cpp::Category& root = log4cpp::Category::getRoot();
    log4cpp::Category& sub1 = log4cpp::Category::getInstance(std::string("sub1"));
    log4cpp::Category& LogHeartBeat = log4cpp::Category::getInstance(std::string("LogHeartBeat"));
    log4cpp::Category& LogText = log4cpp::Category::getInstance(std::string("LogText"));
    log4cpp::Category& thread0 = log4cpp::Category::getInstance(std::string("thread0"));
    log4cpp::Category& thread1 = log4cpp::Category::getInstance(std::string("thread1"));
    log4cpp::Category& thread2 = log4cpp::Category::getInstance(std::string("thread2"));
    log4cpp::Category& thread3 = log4cpp::Category::getInstance(std::string("thread3"));
    log4cpp::Category& thread4 = log4cpp::Category::getInstance(std::string("thread4"));
    log4cpp::Category& thread5 = log4cpp::Category::getInstance(std::string("thread5"));
    log4cpp::Category& thread6 = log4cpp::Category::getInstance(std::string("thread6"));
    log4cpp::Category& thread7 = log4cpp::Category::getInstance(std::string("thread7"));
    log4cpp::Category& thread8 = log4cpp::Category::getInstance(std::string("thread8"));
    log4cpp::Category& thread9 = log4cpp::Category::getInstance(std::string("thread9"));
    m_logo->LogRoot=&root;
    m_logo->LogSub1=&sub1;
    m_logo->LogHeart=&LogHeartBeat;
    m_logo->LogTxt=&LogText ;
    m_logo->LogThread0=&thread0;
    m_logo->LogThread1=&thread1;
    m_logo->LogThread2=&thread2;
    m_logo->LogThread3=&thread3;
    m_logo->LogThread4=&thread4;
    m_logo->LogThread5=&thread5;
    m_logo->LogThread6=&thread6;
    m_logo->LogThread7=&thread7;
    m_logo->LogThread8=&thread8;
    m_logo->LogThread9=&thread9;
    return 0;
}
int main()
{
	LOG_global m_LOG_global;
	log_init(&m_LOG_global);
	int a=1;
	m_LOG_global.LogRoot->fatal("root's log");

	//    sub1 has appender A1 and rootappender. since the additivity property is set true by default
	m_LOG_global.LogSub1->info("sub1's log");

	//    sub2 has appender A2 appender. since the additivity property is set to false
	m_LOG_global.LogHeart->alert("LogHeartBeat's log");
	m_LOG_global.LogHeart->alert("LogHeartBeat's log");
	m_LOG_global.LogHeart->alert("LogHeartBeat's log");
	m_LOG_global.LogHeart->alert("LogHeartBeat's log");
	m_LOG_global.LogHeart->alert("LogHeartBeat's log");
	m_LOG_global.LogHeart->alert("LogHeartBeat's log");
	m_LOG_global.LogHeart->alert("LogHeartBeat's log");
	m_LOG_global.LogHeart->alert("LogHeartBeat's log");
	m_LOG_global.LogHeart->alert("LogHeartBeat's log");

	//    sub3 only has A3 appender. since the additivity property is set to false
	m_LOG_global.LogTxt->debug("LogTxt's log");
	m_LOG_global.LogTxt->alert("LogTxt's log");

	//    sub4 can not be found in the config file, so the root category's appender and layout are used
	m_LOG_global.LogThread0->debug("xxx log");
	m_LOG_global.LogThread1->warn("xxx log");
	m_LOG_global.LogThread2->warn("xxx log");
	m_LOG_global.LogThread3->warn("xxx log");
	m_LOG_global.LogThread4->warn("xxx log");
	m_LOG_global.LogThread5->warn("xxx log");
	m_LOG_global.LogThread6->warn("xxx log");
	m_LOG_global.LogThread7->warn("xxx log");
	m_LOG_global.LogThread8->warn("xxx log");
	m_LOG_global.LogThread9->warn("xxx log");
	return 0;
}
