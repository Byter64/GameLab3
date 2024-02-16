#include "FileSystem.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <cctype>

//Wieso?????
#define GET_STRING(x) #x
#define GET_STRING2(x) GET_STRING(x)

namespace Engine::Files
{
    static const std::filesystem::path ASSETS = GET_STRING2(_ASSET_ROOT);

    /// Parses a file and separates the text by whitespaces
    /// \param path
    /// \return
    std::vector<std::string> ParseFile(std::filesystem::path path)
    {
        std::vector<std::string> result;

        std::ifstream file(path);
        if(!file)
        {
            throw std::runtime_error("path " + path.string() + " could not be accessed");
        }

        std::ostringstream stringStream;
        stringStream << file.rdbuf();
        std::string text = stringStream.str();

        int start = 0;
        for(int pos = 0; pos < text.size(); pos++)
        {
            if(std::isspace(text[pos]))
            {
                if(pos - start > 0)
                {
                    result.push_back(text.substr(start, pos - start));
                }

                start = pos + 1; //pos is whitespace, so start should be after the whitespace
            }
        }

        if(text.size() - start > 0)
        {
            result.push_back(text.substr(start, text.size() - start));
        }

        return result;
    }
}