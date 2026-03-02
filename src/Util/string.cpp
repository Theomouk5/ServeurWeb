#include "../../include/Util/string.hpp"
#include <string>
#include <vector>

namespace Util
{
    std::vector<std::string> splitLines(std::string msg)
    {
        std::vector<std::string> allLines;
        std::string line;

        for(auto c : msg)
        {
            if(c == '\n')
            {
                allLines.push_back(line);
                line = "";
            }
            {
                line.push_back(c);
            }
        }

        return allLines;
    }
}


