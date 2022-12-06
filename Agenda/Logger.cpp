#include "stdafx.h"

#include "Logger.h"

#include <iostream>
#include <memory>

namespace
{
static std::unique_ptr<Logger> instance;
}

Logger::Logger(const std::string& aFileName)
    : m_LogStream(aFileName.c_str())
{
}

Logger& Logger::CreateInstance(const std::string& aFileName)
{
    instance.reset(new Logger(aFileName));
    return Instance();
}

void Logger::DeleteInstance()
{
    instance.reset();
}

Logger& Logger::Instance()
{
    return* instance;
}

