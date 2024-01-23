// Database.h

#ifndef DATABASE_H
#define DATABASE_H

#include "Table.h"
#include "QueryParser.h"
#include <unordered_map>

using Row = std::unordered_map<std::string, std::string>;
using TableAlias = std::vector<Row>;

class Database {
    std::unordered_map<std::string, Table> tables;

public:
    //operacje
    void createTable(const std::string& name, const std::vector<std::string>& columns);
    void dropTable(const std::string& name);
    void insertIntoTable(const std::string& tableName, const std::vector<std::string>& columns, const std::vector<std::string>& values);
    void deleteFromTable(const std::string &tableName, const std::string &columnName, const std::string &value);
    void alterTable(const std::string &tableName, const std::string &operation, const std::string &columnName);
    void updateTable(const std::string &tableName, const std::string &columnName, const std::string &newValue,
                     const std::string &whereColumn, const std::string &whereValue);
    void selectFromTable(const QueryParser::Query &query);

    //ładowanie i zapisywanie danych
    void saveToTxt(const std::string &filename);
    void loadFromTxt(const std::string &filename);
};

#endif // DATABASE_H
