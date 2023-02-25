#include <OpenXLSX/OpenXLSX.hpp>

class Excel
{
    private:
    OpenXLSX::XLDocument document;
    OpenXLSX::XLWorksheet worksheet;

    public:
    void createFile(std::string filepath);
    void writeCell(std::string val, int ln, std::string col);
    void saveFile();
};

void Excel::createFile(std::string filepath)
{
    document.create(filepath);
    worksheet = document.workbook().worksheet("Sheet1");
}

void Excel::writeCell(std::string val, int ln, std::string col)
{
    std::string position = col + std::to_string(ln);
    worksheet.cell(position).value() = val;
}

void Excel::saveFile()
{
    document.save();
}

