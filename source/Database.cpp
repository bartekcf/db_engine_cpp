// Database.cpp

#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <iomanip>
#include "../include/Database.h"
#include "fmt/core.h"
#include "../include/QueryParser.h"

void Database::createTable(const std::string &name, const std::vector<std::string> &columns) {
    if (tables.find(name) != tables.end()) {
        fmt::println("Tabela o nazwie {} istnieje juz w bazie danych!", name);
        return;
    }
    tables.emplace(name, Table(name, columns));
    fmt::println("Dodano tabele {}", name);

}


void Database::dropTable(const std::string &name) {

    if (tables.find(name) == tables.end()) {
        fmt::println("Tabela {} nie istnieje w bazie danych", name);
        return;
    }

    tables.erase(name);
    fmt::println("Tabela {} zostala usnieta", name);

}

void Database::insertIntoTable(const std::string &tableName, const std::vector<std::string> &columnNames,
                               const std::vector<std::string> &values) {
    auto tableIter = tables.find(tableName);
    if (tableIter != tables.end()) {
        Table &table = tableIter->second;

        if (columnNames.size() != values.size()) {
            std::cerr << "Liczba kolumn nie odpowiada liczbie wartości." << '\n';
            return;
        }

        Row row;
        for (const auto &column: table.columns) {
            auto it = std::find(columnNames.begin(), columnNames.end(), column);
            if (it != columnNames.end()) {
                size_t index = std::distance(columnNames.begin(), it);
                row[column] = values[index];
            } else {
                row[column] = "";
            }
        }

        table.addRow(row);
        fmt::print("Dodano dane do tabeli {}\n", tableName);
    } else {
        std::cerr << "Tabela " << tableName << " nie istnieje." << '\n';
    }
}

void Database::saveToTxt(const std::string &filename) {
    std::ofstream file(filename);
    if (!file) {
        std::cerr << "Nie można otworzyć pliku do zapisu: " << filename << '\n';
        return;
    }

    for (const auto &[tableName, table]: tables) {
        file << tableName << "\n";
        // Najpierw zapisz nagłówki kolumn
        for (const auto &column: table.columns) {
            file << column << ";";
        }
        file << "\n";
        for (const auto &row: table.rows) {
            for (const auto &column: table.columns) {
                file << row.at(column) << ";";
            }
            file << "\n";
        }
        file << "\n";
    }
}

void Database::loadFromTxt(const std::string &filename) {
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Nie można otworzyć pliku do odczytu: " << filename << '\n';
        return;
    }

    std::string tableName;
    while (std::getline(file, tableName)) {
        if (tableName.empty()) continue;

        std::string header;
        std::getline(file, header);
        std::istringstream headerStream(header);
        std::string columnName;
        std::vector<std::string> columns;
        while (std::getline(headerStream, columnName, ';')) {
            columns.push_back(columnName);
        }

        std::string rowLine;
        std::vector<Row> rows;
        while (std::getline(file, rowLine) && !rowLine.empty()) {
            std::istringstream rowStream(rowLine);
            std::string cell;
            Row row;
            size_t columnIndex = 0;
            while (std::getline(rowStream, cell, ';')) {
                row[columns[columnIndex++]] = cell;
            }
            rows.push_back(row);
        }

        Table table(tableName, columns);
        for (const auto &row: rows) {
            table.addRow(row);
        }
        tables[tableName] = table;
    }
}

void Database::deleteFromTable(const std::string &tableName, const std::string &columnName, const std::string &value) {
    auto tableIter = tables.find(tableName);
    if (tableIter != tables.end()) {
        Table &table = tableIter->second;

        std::string trimmedColumnName = columnName;
        trimmedColumnName.erase(trimmedColumnName.begin(),
                                std::find_if(trimmedColumnName.begin(), trimmedColumnName.end(), [](unsigned char ch) {
                                    return !std::isspace(ch);
                                }));
        trimmedColumnName.erase(
                std::find_if(trimmedColumnName.rbegin(), trimmedColumnName.rend(), [](unsigned char ch) {
                    return !std::isspace(ch);
                }).base(), trimmedColumnName.end());

        auto colIt = std::find(table.columns.begin(), table.columns.end(), trimmedColumnName);
        if (colIt == table.columns.end()) {
            fmt::print("Kolumna {} nie istnieje w tabeli {}\n", trimmedColumnName, tableName);
            return;
        }

        auto &rows = table.rows;
        rows.erase(std::remove_if(rows.begin(), rows.end(),
                                  [&trimmedColumnName, &value](const Row &row) {
                                      return row.at(trimmedColumnName) == value;
                                  }),
                   rows.end());

        fmt::print("Usunięto wiersze z tabeli {}\n", tableName);
    } else {
        fmt::print("Tabela {} nie istnieje.\n", tableName);
    }
}

void Database::alterTable(const std::string &tableName, const std::string &operation, const std::string &columnName) {
    auto tableIter = tables.find(tableName);
    if (tableIter != tables.end()) {
        Table &table = tableIter->second;

        if (operation == "ADD") {
            if (std::find(table.columns.begin(), table.columns.end(), columnName) == table.columns.end()) {
                table.columns.push_back(columnName);
                for (auto &row: table.rows) {
                    row[columnName] = "";
                }
                fmt::print("Dodano kolumnę {} do tabeli {}\n", columnName, tableName);
            } else {
                fmt::print("Kolumna {} już istnieje w tabeli {}\n", columnName, tableName);
            }
        } else if (operation == "DROP COLUMN") {
            auto colIt = std::find(table.columns.begin(), table.columns.end(), columnName);
            if (colIt != table.columns.end()) {
                table.columns.erase(colIt);
                for (auto &row: table.rows) {
                    row.erase(columnName);
                }
                fmt::print("Usunięto kolumnę {} z tabeli {}\n", columnName, tableName);
            } else {
                fmt::print("Kolumna {} nie istnieje w tabeli {}\n", columnName, tableName);
            }
        }
    } else {
        fmt::print("Tabela {} nie istnieje.\n", tableName);
    }
}

void Database::updateTable(const std::string &tableName, const std::string &columnName, const std::string &newValue,
                           const std::string &whereColumn, const std::string &whereValue) {
    auto tableIter = tables.find(tableName);
    if (tableIter != tables.end()) {
        Table &table = tableIter->second;

        if (std::find(table.columns.begin(), table.columns.end(), columnName) == table.columns.end() ||
            std::find(table.columns.begin(), table.columns.end(), whereColumn) == table.columns.end()) {
            fmt::print("Niektóre kolumny nie istnieją w tabeli {}\n", tableName);
            return;
        }

        for (auto &row: table.rows) {
            if (row.at(whereColumn) == whereValue) {
                row[columnName] = newValue;
            }
        }

        fmt::print("Zaktualizowano dane w tabeli {}\n", tableName);
    } else {
        fmt::print("Tabela {} nie istnieje.\n", tableName);
    }
}

void Database::selectFromTable(const QueryParser::Query &query) {
    auto tableIter = tables.find(query.tableName);
    if (tableIter == tables.end()) {
        fmt::print("Tabela {} nie istnieje.\n", query.tableName);
        return;
    }

    const Table &table = tableIter->second;
    std::vector<Row> selectedRows;

    for (const auto &row: table.rows) {
        bool match = true;
        for (const auto &condition: query.select.whereConditions) {
            const std::string &cellValue = row.at(condition.column);
            if (condition.operator_ == "=" && cellValue != condition.value) {
                match = false;
            } else if (condition.operator_ == "!=" && cellValue == condition.value) {
                match = false;
            } else if (condition.operator_ == ">" && !(cellValue < condition.value)) {
                match = false;
            } else if (condition.operator_ == "<" && !(cellValue > condition.value)) {
                match = false;
            }

            if (!match) {
                break;
            }
        }
        if (match) {
            selectedRows.push_back(row);
        }
    }

    if (!query.select.orderBy.empty()) {
        std::sort(selectedRows.begin(), selectedRows.end(),
                  [&query](const Row &a, const Row &b) {
                      return a.at(query.select.orderBy) < b.at(query.select.orderBy);
                  });
    }

    if (query.select.limit > 0 && selectedRows.size() > static_cast<size_t>(query.select.limit)) {
        selectedRows.resize(query.select.limit);
    }

    for (const auto &col: table.columns) {
        fmt::print("{}\t", col);
    }
    fmt::print("\n");

    for (const auto &row: selectedRows) {
        for (const auto &col: table.columns) {
            fmt::print("{}\t", row.at(col));
        }
        fmt::print("\n");
    }
}
