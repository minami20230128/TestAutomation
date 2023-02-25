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
bool findIf(std::string& line);
bool findElseIf(std::string& line);
void findElse(std::string& line);
std::vector<std::string> conditionals;
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
    writeHeader();
    writeExcel();

    return 0;
}

void read()
{
    std::string line;
    std::ifstream file(readfilepath);

    while(std::getline(file, line))
    {
        if(!findIf(line))
        {
            if(!findElseIf(line))
            {
                 findElse(line);
            }
        }
    }
}

void writeExcel()
{
    int row = DATAROW;

    for(auto&& conditional : conditionals)
    {
        if(conditional == "else")
        {
            excel->writeCell(conditional, row, "D");
            row += 1;
        }
        else
        {
            excel->writeCell(conditional, row, "D");
            excel->writeCell("true", row, "E");
            excel->writeCell("false", row + 1, "E");
            row += 2;
        }
        
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

bool findIf(std::string& line)
{
    const char* pLine = line.c_str();
    bool IsI = false;
    
    while(pLine != NULL)
    {
        printf("%d\n", *pLine);
        if(*pLine == 'i')
        {
            IsI = true;
            pLine++;
            continue;
        }
        if(IsI && *pLine == 'f')
        {
            conditionals.push_back(pLine - 1);
            return true;
        }
        if(*pLine != char(0x09) && *pLine != char(0x20))//タブかスペースなら探索を続ける
        {
            return false;
        }
        pLine++;
    }

    return false;
}

bool findElseIf(std::string& line)
{
    const char* pLine = line.c_str();
    bool IsI = false;
    
    while(pLine != NULL)
    {
        printf("%d\n", *pLine);

        if(*pLine != char(0x09) && *pLine != char(0x20))
        {
            break;
        }
        pLine++;
    }
    
    std::string str(pLine); 
    auto idx = str.find("else if");
    if(idx == 0)
    {
        conditionals.push_back(str.substr(idx, str.length() - idx));
        return true;
    }
    return false;
}

void findElse(std::string& line)
{
    const char* pLine = line.c_str();
    bool IsI = false;
    
    while(pLine != NULL)
    {
        printf("%d\n", *pLine);

        if(*pLine != char(0x09) && *pLine != char(0x20))
        {
            break;
        }
        pLine++;
    }

    std::string str(pLine); 
    auto idx = str.find("else");
    if(idx == 0)
    {
        conditionals.push_back(str);
    }
}

