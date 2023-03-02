#include <fstream>
#include <string>
#include <iostream>
#include <vector>
#include <stdio.h>
#include "excel.hpp"
#include <memory>

#define HEADERROW 4
#define DATAROW 5

void read();
void writeExcel();
void writeHeader();
void writeModule();
bool findConditional(std::string& line, const char* target);
void findMethod(std::string& line);
std::vector<std::pair<std::string, std::vector<std::string>>> methods;
std::unique_ptr<Excel> excel;
std::string readfilepath;
std::string writefilepath;

int main()
{
    printf("%s", "input filepath to read\n");
    std::cin >> readfilepath;
    printf("%s", "input filepath to write\n");
    std::cin >> writefilepath;
    excel = std::make_unique<Excel>();
    excel->createFile(writefilepath);
    read();
    auto itr = methods.begin();
    while(itr != methods.end())
    {
        std::cout << itr->first << std::endl;
        itr++;
    }
    writeHeader();
    //writeExcel();

    return 0;
}

void read()
{
    std::string line;
    std::ifstream file(readfilepath);

    while(std::getline(file, line))
    {
        if(!findConditional(line, "if"))
        {
            if(!findConditional(line, "else if"))
            {
                findConditional(line, "else");
            }
        }
        findMethod(line);
    }
}

void writeExcel()
{
    auto itr = methods.begin();
    int row = DATAROW;
    while(itr != methods.end())
    {   
        excel->writeCell(itr->first, row, "C");
        for(auto&& conditional : itr->second)
        {
            std::cout << itr->first << ":" << conditional << std::endl;
            excel->writeCell(itr->first, row, "C");
            row++;
        }
        itr++;
    }

    int idx = readfilepath.rfind("/");
    std::string modulename = readfilepath.substr(idx + 1);
    excel->writeCell(modulename, DATAROW, "B");
    excel->saveFile();
}

void writeHeader()
{
    excel->writeCell("モジュール名", HEADERROW, "B");
    excel->writeCell("メソッド名", HEADERROW, "C");
    excel->writeCell("条件", HEADERROW, "D");
    excel->writeCell("条件の成否", HEADERROW, "E");
    excel->writeCell("テスト結果", HEADERROW, "F");
    excel->writeCell("備考", HEADERROW, "G");
    excel->writeCell("担当者", HEADERROW, "H");
    excel->writeCell("実施日", HEADERROW, "I");
}

void findMethod(std::string& line)
{
    const char* pLine = line.c_str();

    while(pLine != NULL)
    {
        if(*pLine != char(0x09) && *pLine != char(0x20))
        {
            break;
        }
        pLine++;
    }

    std::string str(pLine);
    if(str.find("class") != std::string::npos)
    {
        return;
    }

    if(str.find("if") != std::string::npos)
    {
        return;
    }

    if(str.find("while") != std::string::npos)
    {
        return;
    }

    if(str.find("for") != std::string::npos)
    {
        return;
    }

    if(str.find(char(0x20)) == -1)
    {
        return;
    }

    if(str.rfind(")") != str.length() - 1)
    {
        return;
    }

    std::vector<std::string> second;
    methods.push_back(std::make_pair(str, second));
    std::cout << str << std::endl;
}

bool findConditional(std::string& line, const char* target)
{
    const char* pLine = line.c_str();
    
    while(pLine != NULL)
    {
        if(*pLine != char(0x09) && *pLine != char(0x20))
        {
            break;
        }
        pLine++;
    }

    std::string str(pLine); 
    auto idx = str.find(target);
    if(idx == 0)
    {
        methods[methods.size()].second.push_back(str.substr(idx, str.length() - idx));
        std::cout << methods[methods.size()].second[methods.size()] << std::endl;
        return true;
    }

    return false;
}