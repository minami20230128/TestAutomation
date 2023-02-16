#include <fstream>
#include <string>
#include <iostream>
#include <vector>
#include <stdio.h>

void read(std::string& filepath);
void write(std::string& filepath);
std::vector<std::string> conditionals;

int main()
{
    std::string readfilepath;
    std::cin >> readfilepath;
    std::string writefilepath;
    std::cin >> writefilepath;
    
    read(readfilepath);
    write(writefilepath);

    return 0;
}

void read(std::string& filepath)
{
    std::string line;
    std::ifstream file(filepath);

    while(std::getline(file, line))
    {
        const char* pLine;
        pLine = line.c_str();
        bool IsI = false;
        
        while(pLine != NULL)
        {
            int i = 0;
            printf("%d¥n", *pLine);
            if(pLine[i] == 'i')
            {
                IsI = true;
                pLine++;
                continue;
            }
            if(IsI && pLine[i] == 'f')
            {
                conditionals.push_back(line);
                break;
            }
            if(*pLine != char(0x09) && *pLine != char(0x20))
            {
                break;
            }
            pLine++;
            i++;
        }
        
    }
}

void write(std::string& filepath)
{
    std::ofstream file(filepath);
    for(auto&& conditional : conditionals)
    {
        file << conditional << std::endl;
    }
}