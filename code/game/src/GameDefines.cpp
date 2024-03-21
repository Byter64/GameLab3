#include "GameDefines.h"
#include "Engine.h"

const std::filesystem::path Defines::path = Engine::Files::ASSETS / "Defines.txt";
std::map<std::string, std::string> Defines::defines;

void Defines::InitializeDefines()
{
    std::vector<std::string> rawFile = Engine::Files::ParseFile(path);

    std::vector<std::string> file;
    bool isComment = false;

    for(int i = 0; i < rawFile.size(); i++)
    {
        if(isComment == false)
        {
            unsigned long long pos = rawFile[i].find("/*");

            if(pos == std::string::npos)
                file.push_back(rawFile[i]);
            else if(pos == 0)
                isComment = true;
            else
            {
                isComment = true;
                file.push_back(rawFile[i].substr(0, pos));
            }
        }
        else
        {
            unsigned long long pos = rawFile[i].find("*/");

            if(pos == std::string::npos)
                continue;
            else if(pos == rawFile[i].size() - 2)
                isComment = false;
            else
            {
                isComment = false;
                file.push_back(rawFile[i].substr(pos + 2));
            }
        }
    }

    if (file.size() % 3 != 0)
    {
        std::cout << "Defines does not have an amount of strings % 3 == 0" << std::endl;
        Engine::EndGame();
    }

    for (int i = 0; i < file.size(); i += 3)
    {
        std::string key = file[i];
        std::transform(key.begin(), key.end(), key.begin(), tolower);
        defines[key] = file[i + 2];
    }
}

int Defines::Int(std::string key)
{
    std::transform(key.begin(), key.end(), key.begin(), tolower);
    return std::stoi(defines[key]);
}

std::string Defines::String(std::string key)
{
    std::transform(key.begin(), key.end(), key.begin(), tolower);
    return defines[key];
}

bool Defines::Bool(std::string key)
{
    std::transform(key.begin(), key.end(), key.begin(), tolower);
    std::string b = defines[key];
    std::transform(b.begin(), b.end(), b.begin(), tolower);
    if(b == "true" || b == "1")
        return true;
    if(b == "false" || b == "0")
        return false;

    std::cout << "For the key \"" << key << "\" a bool value (true, 1, false, 0) is expected but something else (or nothing) was given" << std::endl;
    exit(-1);
}

float Defines::Float(std::string key)
{
    std::transform(key.begin(), key.end(), key.begin(), tolower);
    return std::stof(defines[key]);
}
