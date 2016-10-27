#ifndef __RUNTIME_MONITOR_H__
#define __RUNTIME_MONITOR_H__

#include <map>
#include <sstream>

// Enables disable runtime monitor utility 
#define RT_ENABLE_MONITOR

// Runtime monitor macros
#ifdef RT_ENABLE_MONITOR
#define RT_MONITOR  runtime::MonitorUnit  _rmu_(__FILE__, __FUNCTION__)
#define RT_MONITOR_LOG_REPORT runtime::Monitor::Instance().LogReport()
#define RT_MONITOR_RESET runtime::Monitor::Instance().Reset()
#else
#define RT_MONITOR
#define RT_MONITOR_LOG_REPORT
#define RT_MONITOR_RESET
#endif

namespace runtime
{

// Monitoring unit - do not use directly. Use MONITOR macro above.
class MonitorUnit
{
public:
    MonitorUnit(const char* file, const char* functionName);
    ~MonitorUnit();

    const std::string& GetFile() const { return _file; }
    const std::string& GetFuncName() const { return _functionName; }
    int GetDuration() const { return (int)_duration; }

private:
    static double _freq;
    
    std::string _file;
    std::string _functionName;

    double _start;
    double _duration;
};

// Class for runtime stats monitoring - not thread safe.
class Monitor
{
    friend MonitorUnit;

private: // types
    typedef std::pair<std::string /*file*/, std::string /*func name*/> StorageKeyType; 
    typedef std::pair<int/*call count*/, int/*call overall duration*/> StorageValueType;

    typedef std::map<StorageKeyType, StorageValueType> StatStorageType;

public:
    Monitor();
    ~Monitor();

    static Monitor& Instance();

    // Get the report out into ostream.
    void Report(std::ostream& oss) const;

    // Logs the report into log file. 
    void LogReport() const;

    // Resets collected stats.
    void Reset();

private:
    void Note(const MonitorUnit& unit);

    // Sorter helper function.
    static bool storageSorter(const StatStorageType::value_type& first, const StatStorageType::value_type& second);
    
private:
    StatStorageType _storage;
};

// Utility function returns frequency used in tick calculation.
double GetFreq();

}

#endif//__RUNTIME_MONITOR_H__
