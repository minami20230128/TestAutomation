#include "OpenXLSX.hpp"

class Excel
{
    private:
    OpenXLSX::XLDocument document;
    OpenXLSX::XLWorksheet worksheet;

    public:
    void createFile();
    void writeCell(std::string val, int ln, std::string col);
    void saveFile(std::string filepath);
};

void Excel::createFile()
{
    document.create("test.xlsx");
    worksheet = document.workbook().worksheet("Sheet1");
}

void Excel::writeCell(std::string val, int ln, std::string col)
{
    std::string position = col + std::to_string(ln);
    worksheet.cell(position).value() = val;
}

void Excel::saveFile(std::string filepath)
{
    document.saveAs(filepath);
}

