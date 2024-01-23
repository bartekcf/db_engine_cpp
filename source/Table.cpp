#include "../include/Table.h"

void Table::addRow(const Row& row) {
    rows.push_back(row);
}

void Table::removeRow(size_t rowIndex) {
    if (rowIndex < rows.size()) {
        rows.erase(rows.begin() + rowIndex);
    }
}