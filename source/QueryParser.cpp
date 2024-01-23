// QueryParser.cpp

#include <sstream>
#include <algorithm>
#include <iostream>
#include "../include/QueryParser.h"
#include "fmt/core.h"

QueryParser::QueryParser() {
}

QueryParser::~QueryParser() {
}

QueryParser::Query QueryParser::parse(const std::string &queryString) {
    std::istringstream iss(queryString);
    std::string word;
    Query query;

    if (iss >> word) {
        if (word == "CREATE") {
            query = parseCreateTable(iss);
        } else if (word == "DROP") {
            query = parseDropTable(iss);
        } else if (word == "INSERT") {
            query = parseInsertInto(iss);
        } else if (word == "DELETE"){
            query = parseDelete(iss);
        } else if (word == "ALTER") {
            query = parseAlterTable(iss);
        } else if (word == "UPDATE"){
            query = parseUpdateTable(iss);
        } else if (word == "SELECT"){
            query = parseSelectFromTable(iss);
        } else {
            std::cerr << "Blad skladni lub nie obslugiwane zapytanie";
        }
    }

    return query;
}

QueryParser::Query QueryParser::parseCreateTable(std::istringstream &iss) {
    std::string word;
    Query query;
    query.type = "CREATE";
    if (iss >> word && word == "TABLE") {
        if (iss >> query.tableName) {
            if (iss.peek() == ' ') {
                iss.ignore();
            }

            std::string columnsString;
            std::getline(iss, columnsString);

            columnsString.erase(
                    std::find_if(columnsString.rbegin(), columnsString.rend(), [](unsigned char ch) {
                        return !std::isspace(ch);
                    }).base(), columnsString.end()
            );

            std::istringstream columnsStream(columnsString);
            std::string columnName;
            while (std::getline(columnsStream, columnName, ',')) {
                columnName.erase(columnName.begin(),
                                 std::find_if(columnName.begin(), columnName.end(), [](unsigned char ch) {
                                     return !std::isspace(ch);
                                 }));
                query.columns.push_back(columnName);
            }
        }
    }
    return query;
}

QueryParser::Query QueryParser::parseDropTable(std::istringstream &iss) {
    std::string word;
    Query query;
    query.type = "DROP";
    if (iss >> word && word == "TABLE") {
        iss >> query.tableName;
    }
    return query;
}

QueryParser::Query QueryParser::parseInsertInto(std::istringstream &iss) {
    std::string word;
    Query query;

    if (iss >> word && word == "INTO") {
        if (iss >> query.tableName) {
            if (iss.peek() == ' ') {
                iss.ignore();
            }

            std::string columnsString;
            std::getline(iss, columnsString, '(');
            std::getline(iss, columnsString, ')');
            std::istringstream columnsStream(columnsString);
            std::string columnName;
            while (std::getline(columnsStream, columnName, ',')) {
                columnName.erase(columnName.begin(),
                                 std::find_if(columnName.begin(), columnName.end(), [](unsigned char ch) {
                                     return !std::isspace(ch);
                                 }));
                query.columns.push_back(columnName);
            }

            if (iss >> word && word == "VALUES") {
                if (iss.peek() == ' ') {
                    iss.ignore();
                }

                std::string valuesString;
                std::getline(iss, valuesString, '(');
                std::getline(iss, valuesString, ')');
                std::istringstream valuesStream(valuesString);
                std::string value;
                while (std::getline(valuesStream, value, ',')) {
                    value.erase(value.begin(), std::find_if(value.begin(), value.end(), [](unsigned char ch) {
                        return !std::isspace(ch);
                    }));
                    query.values.push_back(value);
                }

                query.type = "INSERT";
            }
        }
    }
    return query;
}

QueryParser::Query QueryParser::parseDelete(std::istringstream &iss) {
    std::string word;
    Query query;
    query.type = "DELETE";

    if (iss >> word && word == "FROM") {
        if (iss >> query.tableName) {
            if (iss.peek() == ' ') {
                iss.ignore();
            }

            if (iss >> word && word == "WHERE") {
                std::string condition;
                std::getline(iss, condition);

                condition.erase(
                        std::find_if(condition.rbegin(), condition.rend(), [](unsigned char ch) {
                            return !std::isspace(ch);
                        }).base(), condition.end()
                );

                std::string columnName, value;
                auto pos = condition.find('=');
                if (pos != std::string::npos) {
                    columnName = condition.substr(0, pos);
                    value = condition.substr(pos + 1);

                    columnName.erase(columnName.begin(), std::find_if(columnName.begin(), columnName.end(), [](unsigned char ch) {
                        return !std::isspace(ch);
                    }));
                    value.erase(value.begin(), std::find_if(value.begin(), value.end(), [](unsigned char ch) {
                        return !std::isspace(ch);
                    }));

                    query.columns.push_back(columnName);
                    query.values.push_back(value);
                }
            }
        }
    }
    return query;
}

QueryParser::Query QueryParser::parseAlterTable(std::istringstream &iss) {
    std::string word;
    Query query;
    query.type = "ALTER";

    if (iss >> word && word == "TABLE") {
        if (iss >> query.tableName) {
            if (iss.peek() == ' ') {
                iss.ignore();
            }

            if (iss >> word) {
                if (word == "ADD") {
                    query.operation = "ADD";

                    std::string columnName;
                    if (iss >> columnName) {
                        query.columns.push_back(columnName);
                    }
                } else if (word == "DROP") {
                    if (iss >> word && word == "COLUMN") {
                        query.operation = "DROP COLUMN";

                        std::string columnName;
                        if (iss >> columnName) {
                            query.columns.push_back(columnName);
                        }
                    }
                }
            }
        }
    }

    return query;
}

QueryParser::Query QueryParser::parseUpdateTable(std::istringstream &iss) {
    std::string word;
    Query query;
    query.type = "UPDATE";

    if (iss >> query.tableName) {
        if (iss >> word && word == "SET") {
            std::string columnSet;
            std::getline(iss, columnSet, '=');

            std::string valueSet;
            if (iss >> valueSet) {
                columnSet.erase(columnSet.begin(), std::find_if(columnSet.begin(), columnSet.end(), [](unsigned char ch) {
                    return !std::isspace(ch);
                }));
                valueSet.erase(valueSet.begin(), std::find_if(valueSet.begin(), valueSet.end(), [](unsigned char ch) {
                    return !std::isspace(ch);
                }));

                query.columns.push_back(columnSet);
                query.values.push_back(valueSet);

                if (iss >> word && word == "WHERE") {
                    std::string columnWhere;
                    std::getline(iss, columnWhere, '=');

                    std::string valueWhere;
                    if (iss >> valueWhere) {
                        columnWhere.erase(columnWhere.begin(), std::find_if(columnWhere.begin(), columnWhere.end(), [](unsigned char ch) {
                            return !std::isspace(ch);
                        }));
                        valueWhere.erase(valueWhere.begin(), std::find_if(valueWhere.begin(), valueWhere.end(), [](unsigned char ch) {
                            return !std::isspace(ch);
                        }));

                        query.whereClause.column = columnWhere;
                        query.whereClause.value = valueWhere;
                    }
                }
            }
        }
    }

    return query;
}

QueryParser::Query QueryParser::parseSelectFromTable(std::istringstream &iss) {
    Query query;
    query.type = "SELECT";

    std::string segment;
    iss >> segment;
    if (segment == "*") {
        query.select.allColumns = true;
    } else {
        std::stringstream ss(segment);
        std::string item;
        while (std::getline(ss, item, ',')) {
            query.select.columns.push_back(item);
        }
    }

    iss >> segment;
    iss >> query.tableName;

    while (iss >> segment) {
        if (segment == "WHERE") {
            WhereCondition condition;
            iss >> condition.column >> condition.operator_ >> condition.value;
            query.select.whereConditions.push_back(condition);
        } else if (segment == "ORDER") {
            iss >> segment;
            iss >> query.select.orderBy;
        } else if (segment == "LIMIT") {
            iss >> query.select.limit;
        }
    }

    return query;
}