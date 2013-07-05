#include "pocolog.h"


#ifdef POCOLOG

#include "Poco/Logger.h"
#include "Poco/PatternFormatter.h"
#include "Poco/FormattingChannel.h"
#include "Poco/ConsoleChannel.h"
#include "Poco/FileChannel.h"
#include "Poco/Message.h"

using Poco::Logger;
using Poco::PatternFormatter;
using Poco::FormattingChannel;
using Poco::ConsoleChannel;
using Poco::FileChannel;
using Poco::Message;

void initlogger() {
    FormattingChannel* pFCConsole = new FormattingChannel(new PatternFormatter("%Y-%m-%d %H:%M:%S.%c [%q] : %t"));
    pFCConsole->setChannel(new ConsoleChannel);
    pFCConsole->open();
    Logger::create("ConsoleLogger", pFCConsole, Message::PRIO_INFORMATION);


	FormattingChannel* pFCFile = new FormattingChannel(new PatternFormatter("%Y-%m-%d %H:%M:%S.%c [%q] : %t"));
	pFCFile->setChannel(new FileChannel("log.txt"));
	pFCFile->open();
	Logger::create("FileLogger", pFCFile, Message::PRIO_INFORMATION);
}

void log(const std::string& text){
    Logger::get("ConsoleLogger").information(text);
    Logger::get("FileLogger").information(text);
}

#endif //POCOLOG


#ifdef LOG4CPP

#include <iostream>

#include <log4cpp/Portability.hh>
#include <log4cpp/PropertyConfigurator.hh>
#include <log4cpp/Category.hh>

void initlogger() {
    try {
        std::string initFileName;
        initFileName = "./log4cpp.property";
        log4cpp::PropertyConfigurator::configure(initFileName);
    } catch(log4cpp::ConfigureFailure& f) {
        std::cout << "Configure Problem " << f.what() << std::endl;
        return;
    }
}

void log(const std::string& text){
    log4cpp::Category& root = log4cpp::Category::getRoot();
    root.info(text);
}


#endif //LOG4CPP


#if !defined(LOG4CPP) && !defined(POCOLOG)

#include <iostream>

void initlogger() { }

void log(const std::string& text){
    std::cout << "cout: " << text << std::endl;
}

#endif 


void log(const char *fmt, ... ) {
    va_list va;
    va_start(va, fmt);

    log( vfmt(fmt, va) );

    va_end(va);
}

void log(const QPointF pos){
    std::stringstream s;
    s << pos.x();
    s << ",";
    s << pos.y();
    log(s.str());
}

void log(const int text){
    std::stringstream s;
    std::string x;
    s << text;
    s >> x;
    log(x);
}

string vfmt(const char *fmt, va_list ap){

    string s;

    const char *next_c; // next character
    while ( *fmt != '\0' )
    {
        switch ( *fmt )
        {
            case '%':
                {
                    next_c = fmt + 1;
                    switch ( *next_c )
                    {
                        case '\0':
                            s += *fmt;
                            break;
                        case '%':
                            s += '%';
                            ++fmt;
                            break;
                        case 's':
                            s += va_arg(ap, char*); 
                            ++fmt;
                            break;
                        case 'd':
                            {
                                ostringstream oss;
                                oss << va_arg(ap, int);
                                s += oss.str();
                                ++fmt;
                                break;
                            }
                        case 'f':
                            {
                                ostringstream oss;
                                oss << va_arg(ap, double);
                                s += oss.str();
                                ++fmt;
                                break;
                            }
                        default:
                            //assert( !"format unsupported." );
                            cout << "format unsupported." << endl;
                            break;
                    }

                    break;
                }
            default:
                s += *fmt;
                break;
        }

        ++fmt;
    }

    return s;
}
