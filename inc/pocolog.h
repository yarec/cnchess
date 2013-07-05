#ifndef LOG_H
#define LOG_H

#include <string>
#include <sstream>
#include <QPointF>
#include <QString>

#include <cstdarg>

using namespace std;

void initlogger();
void log(const std::string& text);
void log(const char *fmt, ... );
void log(const int text);
void log(const QPointF pos);
string vfmt(const char *fmt, va_list va);

#endif // LOG_H

