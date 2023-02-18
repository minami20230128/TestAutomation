#include <OpenXLSX.hpp>

class Excel
{
    private:
    OpenXLSX::XLDocument document;
    OpenXLSX::XLWorksheet worksheet;

    void createFile();
    void createWorksheet();
    void writeCell(std::string val, int col, int ln);
};

void Excel::createFile()
{
    document.create("test.xlsx");
}

void Excel::createWorksheet()
{
    worksheet = document.workbook().worksheet("Sheet1");
}

void Excel::writeCell(std::string val, int col, int ln)
{
    std::string position = std::to_string(col) + std::to_string(ln);
    worksheet.cell(position).value() = val;
}