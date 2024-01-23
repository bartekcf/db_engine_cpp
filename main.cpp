#include <iostream>
#include <fmt/ranges.h>
#include "include/Database.h"
#include "include/QueryParser.h"


// Kilka dodatkowych informacji oraz przykłady zaprezentowane poniżej znajdują sie w pliku readme.txt


//createTable() metoda tworząca tabele w bazie danych
//składnia CREATE TABLE nazwa_tabeli kolumna1,kolumna2,kolumna3 <------- oddzielamy przecinkiem bez spacji
//przykład polecania: CREATE TABLE dog id,name,age
//
//dropTable() metoda usuwająca tabele o podanej nazwie
//składnia DROP TABLE nazwa_tabeli
//przykład polecaniaL DROP TABLE dog
//
//insertInto() metoda dodajaca rekordy do tabeli
//składnia INSERT INTO nazwa_tabeli (pole_1, pole_2) VALUES (wartosc_1, wartosc_2)
//przykład INSERT INTO dog (id, name) VALUES (1, reks)
//
//delete() metoda usuwająca rekordy z tabeli
//składnia: DELETE FROM nazwa_tabeli WHERE pole = wartosc
//przykład polecenia: DELETE FROM dog WHERE id = 2
//uwagi: metoda ograniczona jest do jednego warunku WHERE tzn nie wspiera AND/OR
//
//alterTable() metoda aktualizująca strukture tabeli
//składnia ALTER TABLE nazwa_tabeli ADD nazwa_kolumny || ALTER TABLE nazwa_tabeli DROP COLUMN nazwa_kolumny
//przykład ALTER TABLE dog ADD breed || ALTER TABLE dog DROP COLUMN id
//
//updateTable() metoda aktualizująca dane w tabeli
//składnia UPDATE nazwa_tabeli SET nazwa_kolumny=wartosc WHERE parametr=wartosc
//przykład UPDATE dog SET name=azor WHERE name=reks
//uwagi: metoda ograniczona jest do porównywania tylko przy użyciu znaku "="
//
//select() metoda służy do pobierania rekordów
//składnia SELECT pola FROM nazwa_tabeli WHERE pole=wartosc ORDER BY nazwa_kolumny ASC/DESC LIMIT liczba
//przykład: SELECT * FROM dog WHERE age < 100 ORDER BY id DESC LIMIT 2


auto main() -> int {
    Database db;
    QueryParser parser;
    db.loadFromTxt("../files/database.txt");

    std::string line;
    while (std::getline(std::cin, line)) {
        if (line == "exit") {
            db.saveToTxt("../files/database.txt");
            fmt::println("Zaktualizowano bazę danych");
            break;
        }

        QueryParser::Query query = parser.parse(line);
        if (query.type == "CREATE") {
            db.createTable(query.tableName, query.columns);
        } else if (query.type == "DROP") {
            db.dropTable(query.tableName);
        } else if (query.type == "INSERT") {
            db.insertIntoTable(query.tableName, query.columns, query.values);
        } else if (query.type == "DELETE") {
            db.deleteFromTable(query.tableName, query.columns.front(), query.values.front());
        } else if (query.type == "ALTER") {
            db.alterTable(query.tableName, query.operation, query.columns.front());
        } else if (query.type == "UPDATE") {
            db.updateTable(query.tableName, query.columns.front(), query.values.front(), query.whereClause.column, query.whereClause.value);
        } else if (query.type == "SELECT") {
            db.selectFromTable(query);
        }
    }
}
