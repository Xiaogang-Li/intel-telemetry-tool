#ifndef __PERF_TOOL_UTIL_H__
#define __PERF_TOOL_UTIL_H__

#include <streambuf>
#include <sstream>
#include <vector>

class Util
{
public:
    template <typename ReturnType>
    static ReturnType GetDataFromRaw(const std::string& rawString,
                              uint8_t skipCount = 0)
    {
        std::string temp = "";
        ReturnType  data = 0;
        uint8_t     loop = 0;
    
        std::stringstream stream(rawString);
    
        for (loop == 0; loop < skipCount; loop++)
        {
            stream >> temp;
        }
    
        stream >> data;
        return data;
    }
                              
    static std::string GetSringDataFromRaw(const std::string& rawString)
    {
        std::string            data = ""; 
        std::string::size_type pos  = 0;
        std::string::size_type pos2 = 0;
    
        pos  = rawString.find(':');
        pos2 = rawString.find_last_of(' ');
	if (pos2 > pos)
	{
	    data = rawString.substr(pos+2, pos2);
	}
	else
	{
	    data = rawString.substr(pos+2);
	}
    
        return data;
    }
    
    static bool IsInclude(const std::string& rawString, const std::string& prefix)
    {
        std::string::size_type pos = 0;
    
        pos = rawString.find(prefix);
    
        if (pos != std::string::npos)
        {
            return true;
        }
    
        return false;
    }
    
    static inline uint32_t ByteToKB(uint32_t sizeInByte) 
    {
        return (sizeInByte / 1024);
    }

    template <typename ReturnType>
    static ReturnType Convert(const std::string& rawString)
    {
	ReturnType  data;
	std::stringstream stream(rawString);
	stream >> data;

	return data;
    }
};

#endif
