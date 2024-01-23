// QueryParser.h

#ifndef QUERY_PARSER_H
#define QUERY_PARSER_H

#include <string>
#include <vector>

class QueryParser {
public:
    struct WhereClause {
        std::string column;
        std::string value;
    };

    struct WhereCondition {
        std::string condition;
        std::string column;
        std::string operator_;
        std::string value;
    };

    struct Select {
        bool allColumns = false;
        std::vector<std::string> columns;
        std::vector<WhereCondition> whereConditions;
        std::string orderBy;
        int limit = -1;
    };

    struct Query {
        std::string type;
        std::string tableName;
        std::string operation;
        WhereClause whereClause;
        std::vector<std::string> columns;
        std::vector<std::string> values;
        Select select;
    };

    QueryParser();
    ~QueryParser();

    Query parse(const std::string& queryString);

private:
    Query parseCreateTable(std::istringstream& iss);
    Query parseDropTable(std::istringstream& iss);
    Query parseInsertInto(std::istringstream& iss);
    Query parseDelete(std::istringstream& iss);
    Query parseAlterTable(std::istringstream &iss);
    Query parseUpdateTable(std::istringstream &iss);
    Query parseSelectFromTable(std::istringstream &iss);
};

#endif // QUERY_PARSER_H
