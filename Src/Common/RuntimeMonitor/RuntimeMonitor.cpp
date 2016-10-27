#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <algorithm>

#include <windows.h>

#include "RuntimeMonitor.h"

namespace runtime
{

Monitor::Monitor()
{}

Monitor::~Monitor()
{}

Monitor& Monitor::Instance()
{
    static Monitor monitor;
    return monitor;
}

void Monitor::Note(const MonitorUnit& unit)
{
    const StorageKeyType& key = std::make_pair(unit.GetFile(), unit.GetFuncName());

    auto it = _storage.find(key);

    if(it == _storage.end())
    {
        const StorageValueType val(1, unit.GetDuration());
        
        _storage.insert(std::make_pair(key, val));
    }
    else
    {
        ++it->second.first; // Increment count;
        it->second.second += unit.GetDuration();
    }
}

bool Monitor::storageSorter(const Monitor::StatStorageType::value_type& first, const Monitor::StatStorageType::value_type& second)
{
    return first.second.second > second.second.second;
}

void Monitor::Report(std::ostream& oss) const
{
    // Report sorted according to time spend.    
    std::vector< std::pair<StorageKeyType, StorageValueType> > sortedVec(_storage.begin(), _storage.end());
    std::sort(sortedVec.begin(), sortedVec.end(), &Monitor::storageSorter);

    auto it = sortedVec.begin();
    const auto itEnd = sortedVec.end();
    for(; it != itEnd; ++it)
    {
        oss << it->first.second.c_str() << " Called [" << it->second.first << " times]," << " duration summary [" << it->second.second << " microseconds]. "
            << "Average call [" << (it->second.second / it->second.first) << " microseconds]" << std::endl;
    }
}

void Monitor::LogReport() const
{
    std::ostringstream oss;
    Report(oss);
    std::cout << "RuntimeMonitor: " << oss.str();
}

void Monitor::Reset()
{
    _storage.clear();
}

MonitorUnit::MonitorUnit(const char* file, const char* functionName)
    : _file(file)
    , _functionName(functionName)
{
    LARGE_INTEGER start;
    QueryPerformanceCounter(&start);

    _start = static_cast<double>(start.QuadPart);
}

MonitorUnit::~MonitorUnit()
{
    static const double freq  = GetFreq();

    LARGE_INTEGER end;
    QueryPerformanceCounter(&end); // end
    _duration = (end.QuadPart - _start) / freq;

    Monitor::Instance().Note(*this);
}

double GetFreq()
{
    LARGE_INTEGER li;
    if(!QueryPerformanceFrequency(&li))
    {
        return 0.0; // Error
    }
    return double(li.QuadPart)/1000000.0; // microseconds
}

}
