#include <fstream>
#include <string>
#include <iostream>
#include <vector>
#include <stdio.h>
#include "excel.hpp"
#include <memory>
#include <map>

#define HEADERROW 4
#define DATAROW 5

void read();
void writeExcel();
void writeHeader();
void writeModule();
void writeMethod();
bool findIf(std::string& line, int linenumber);
bool findElseIf(std::string& line, int linenumber);
void findElse(std::string& line, int linenumber);
void findMethod(std::string& line, int linenumber);
std::map<int, std::string> conditionals;
std::map<int, std::string> methods;
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
    auto itr = conditionals.begin();
    while(itr != conditionals.end())
    {
        std::cout << itr->first << ":" << itr->second << std::endl;
        itr++;
    }

    auto methodItr = methods.begin();
    while(methodItr != methods.end())
    {
        std::cout << methodItr->first << ":" << methodItr->second << std::endl;
        methodItr++;
    }
    writeHeader();
    //writeMethod();
    writeExcel();

    return 0;
}

void read()
{
    std::string line;
    std::ifstream file(readfilepath);
    int linenumber = 1;

    while(std::getline(file, line))
    {
        if(!findIf(line, linenumber))
        {
            if(!findElseIf(line, linenumber))
            {
                 findElse(line, linenumber);
            }
        }
        findMethod(line, linenumber);
        linenumber++;
    }
}

void writeExcel()
{
    auto itr = conditionals.begin();
    auto methodItr = methods.begin();
    int row = DATAROW;
    excel->writeCell(methodItr->second, row, "C");

    // while(itr != conditionals.end() && methodItr != methods.end())
    // {
    //     excel->writeCell(methodItr->second, row, "C");
    //     if(methodItr != methods.end()) methodItr++;

    //     if(itr->first < methodItr->first)
    //     {
    //         excel->writeCell(itr->second, row, "D");
    //         itr++;
    //     }
    //     row++;
    // }

    int idx = readfilepath.rfind("/");
    std::string modulename = readfilepath.substr(idx + 1);
    excel->writeCell(modulename, DATAROW, "B");
    excel->saveFile();
}

void writeMethod()
{
    int row = DATAROW;

    auto itr = conditionals.begin();
    auto methodItr = methods.begin();
    while(itr != conditionals.end())
    {
        if(itr->first >= methodItr->first)//条件式の行数がメソッドの行数より大きかったら
        {
            excel->writeCell(itr->second, DATAROW, "C");
            methodItr++;
        }
        itr++;
    }
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

void findMethod(std::string& line, int linenumber)
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

    if(str.find(char(0x20)) == -1)
    {
        return;
    }

    if(str.rfind(")") != str.length() - 1)
    {
        return;
    }

    methods[linenumber] = str;
}

bool findIf(std::string& line, int linenumber)
{
    const char* pLine = line.c_str();
    bool IsI = false;
    
    while(pLine != NULL)
    {
        if(*pLine == 'i')
        {
            IsI = true;
            pLine++;
            continue;
        }
        if(IsI && *pLine == 'f')
        {
            conditionals[linenumber] = pLine - 1;
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

bool findElseIf(std::string& line, int linenumber)
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
    auto idx = str.find("else if");
    if(idx == 0)
    {
        conditionals[linenumber] = str.substr(idx, str.length() - idx);
        return true;
    }
    return false;
}

void findElse(std::string& line, int linenumber)
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
    auto idx = str.find("else");
    if(idx == 0)
    {
        conditionals[linenumber] = str;
    }
}

