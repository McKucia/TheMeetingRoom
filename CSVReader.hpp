#include <string>
#include <vector>

using table = std::vector< std::vector<std::string> >;

class CSVReader 
{
public:
    table readAll(std::istream& stream, bool skipFirst = false)
    {
        if(skipFirst)
            std::getline(stream, line);

        while (!stream.eof()) 
        {
            std::vector<std::string> rowData = readNextRow(stream);
            data.push_back(rowData);
        };

        return data;
    }

private:
    std::string line;
    table data;

    std::vector<std::string> readNextRow(std::istream& stream)
    {
        std::getline(stream, line);
        std::vector<std::string> rowData;
        int pos = 0, tmp = 0;

        while((pos = line.find(',', pos)) != -1)
        {
            rowData.push_back(line.substr(tmp, pos - tmp));
            ++pos;
            tmp = pos;
        }

        std::string lastWord = line.substr(tmp, pos - tmp);

        if (!lastWord.empty() && lastWord[lastWord.size() - 1] == '\r')
            lastWord.erase(lastWord.size() - 1);

        rowData.push_back(lastWord);

        return rowData;
    }
};