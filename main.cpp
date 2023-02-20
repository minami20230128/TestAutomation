#include <fstream>
#include <string>
#include <iostream>
#include <vector>
#include <stdio.h>
#include "excel.hpp"
#include <memory>

void read(std::string& filepath);
void write(std::string& filepath);
void writeExcel(std::string& filepath);
void findIf(std::string& line);
bool findElseIf(std::string& line);
void findElse(std::string& line);
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
        findIf(line);
        if(!findElseIf(line))
        {
            findElse(line);
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

void writeExcel(std::string& filepath)
{
    auto excel = std::make_unique<Excel>();
    excel->createFile();
    excel->writeCell("hello", 1, "A");
}

void findIf(std::string& line)
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
            conditionals.push_back(pLine - 1);
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

bool findElseIf(std::string& line)
{
    auto idx = line.find("else if");
    if(idx != std::string::npos)
    {
        conditionals.push_back(line.substr(idx, line.length() - idx));
        return true;
    }
    return false;
}

void findElse(std::string& line)
{
    auto idx = line.find("else");
    if(idx != std::string::npos)
    {
        conditionals.push_back(line.substr(idx, line.length() - idx));
    }
}