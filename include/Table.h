#ifndef TABLE_H
#define TABLE_H

#include <string>
#include <vector>
#include <unordered_map>

using Row = std::unordered_map<std::string, std::string>;

class Table {
public:
    Table() = default;
    std::string name;
    std::vector<std::string> columns;
    std::vector<Row> rows;

    Table(const std::string& name, const std::vector<std::string>& columns)
            : name(name), columns(columns) {}


    void addRow(const Row& row);
    void removeRow(size_t rowIndex);
};

#endif // TABLE_H
