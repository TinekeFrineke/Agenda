#pragma once

#include <fstream>
#include <string>

class Logger
{
public:
    static Logger& CreateInstance(const std::string& aFileName);
    static void DeleteInstance();

    static Logger& Instance();

    static Logger& endl(Logger& os)
    {
        os.m_LogStream << std::endl;
        return os;
    }

    Logger& operator<<(Logger& (__cdecl* _Pfn)(Logger&)) { // call Logger manipulator
        return _Pfn(*this);
    }

    template <typename T>
    Logger& operator<<(T& aT)
    {
        m_LogStream << aT;
        return *this;
    }

    template <typename T>
    Logger& operator<<(const T& aT)
    {
        m_LogStream << aT;
        return *this;
    }

private:
    Logger(const std::string& aFileName);

    std::ofstream m_LogStream;
};

