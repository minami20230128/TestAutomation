#include <fstream>
#include <string>
#include <iostream>
#include <vector>
#include <stdio.h>
#include "excel.hpp"
#include "pipe.hpp"
#include <memory>
#include <unistd.h>

#define HASHROW 2
#define HEADERROW 4
#define DATAROW 5

void read();
void writeExcel();
void writeHeader();
void writeModule();
void getGitHash();
bool isExistFile(std::string path);
bool findConditional(std::string& line, const char* target);
void findMethod(std::string& line);

std::vector<std::pair<std::string, std::vector<std::string>>> methods;
std::unique_ptr<Excel> excel;
std::string readfilepath;
std::string writefilepath;
std::string hash;

int main()
{
    printf("%s", "input filepath to read\n");
    std::cin >> readfilepath;
    printf("%s", "input filepath to write\n");
    std::cin >> writefilepath;
    excel = std::make_unique<Excel>();
    if(!isExistFile(writefilepath))
    {
        excel->createFile(writefilepath);
    }
    else
    {
        excel->openFile(writefilepath);
    }
    
    read();
    getGitHash();
    writeHeader();
    writeExcel();

    return 0;
}

bool isExistFile(std::string path)
{
    auto pipe = std::make_unique<Pipe>();
    int idx = readfilepath.rfind("/");
    std::string directry = readfilepath.substr(0, idx);
    std::cout << directry << std::endl;
    chdir(directry.c_str());
    auto files = pipe->executeCommand("ls");
    std::cout << files << std::endl;
    if(files.find(path) != std::string::npos)
    {
        return true;
    }

    return false;
}

void read()
{
    std::string line;
    std::ifstream file(readfilepath);

    while(std::getline(file, line))
    {
        if(findConditional(line, "if"))
        {
            continue;
        }

        if(findConditional(line, "else if"))
        {
            continue;
        }

        if(findConditional(line, "else"))
        {
            continue;
        }

        if(findConditional(line, "while"))
        {
            continue;
        }

        if(findConditional(line, "try"))
        {
            continue;
        }

        if(findConditional(line, "catch"))
        {
            continue;
        }

        if(findConditional(line, "else"))
        {
            continue;
        }

        if(findConditional(line, "finally"))
        {
            continue;
        }

        if(findConditional(line, "switch"))
        {
            continue;
        }

        if(findConditional(line, "case"))
        {
            continue;
        }

        if(findConditional(line, "default"))
        {
            continue;
        }

        findMethod(line);
    }
}

void writeExcel()
{
    int row = DATAROW;
    for(auto&& method : methods)
    {   
        std::cout << row << ":" << method.first << std::endl;
        excel->writeCell(method.first, row, "C");
        if(!method.second.empty())
        {
            for(auto&& conditional : method.second)
            {
                std::cout << row << ":" << conditional << std::endl;
                excel->writeCell(conditional, row, "D");
                if(conditional.find("if") != std::string::npos)
                {
                    excel->writeCell("true", row, "E");
                    row++;
                    excel->writeCell("false", row, "E");
                }
                row++;
            }
            row++;
        }
    }

    int idx = readfilepath.rfind("/");
    std::string modulename = readfilepath.substr(idx + 1);
    excel->writeCell(modulename, DATAROW, "B");

    excel->writeCell("git hash:" + hash, HASHROW, "B");

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

    std::vector<std::string> second{};
    methods.push_back(std::make_pair(str, second));
    std::cout << methods.size() << std::endl;
    std::cout << methods[methods.size()-1].first << std::endl;
}

bool findConditional(std::string& line, const char* target)
{
    const char* pLine = line.c_str();
    
    while(pLine != NULL)
    {
        if(*pLine != char(0x09) && *pLine != char(0x20))//スペースとタブを切り詰める
        {
            break;
        }
        pLine++;
    }

    std::string str(pLine);
    int idx;
    if((idx = str.find("//")) != std::string::npos)//コメントを削除
    {
        str = readfilepath.substr(0, idx);
    }

    if((idx = str.find(target)) == 0)//必ず行頭にあるときのみ
    {
        methods[methods.size() - 1].second.push_back(str.substr(idx, str.length() - idx));
        std::cout << methods[methods.size()-1].second[methods[methods.size()-1].second.size()-1] << std::endl;
        return true;
    }

    return false;
}

void getGitHash()
{
    auto pipe = std::make_unique<Pipe>();
    int idx = readfilepath.rfind("/");
    std::string directry = readfilepath.substr(0, idx);
    std::cout << directry << std::endl;
    chdir(directry.c_str());
    hash = pipe->executeCommand("git show --format='%H' --no-patch");
    std::cout << hash << std::endl;
}