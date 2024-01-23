BAZA DANYCH autor s27612

Baza danych oparta jest na zapytan z języka MySQL

Informacje ogólne:
Baza danych jest case sensitvie tzn. polecenia muszą być pisanę wielkimi literami np. create nie zadziała trzeba napisać CREATE
Nie jest wymagany żadny znak na końcu lini wciśnięcie klawisza ENTER automatycznie powoduje wysłanie.
Podczas wykonywania operacji na bazie stan jej jest przechowyany w pamięci operacyjnej dane zapisują sie do pliku podczas wyjśćia z konsoli poleceniem "exit"
Baza przy każdym uruchomieniu jest zasilana wcześniej zapisanymi danymi.


Wspierane polecenia, składnia oraz ich ograniczenia:

createTable() metoda tworząca tabele w bazie danych
składnia CREATE TABLE nazwa_tabeli kolumna1,kolumna2,kolumna3 <------- oddzielamy przecinkiem bez spacji 
przykład polecania: CREATE TABLE dog id,name,age

dropTable() metoda usuwająca tabele o podanej nazwie
składnia DROP TABLE nazwa_tabeli
przykład polecaniaL DROP TABLE dog

insertInto() metoda dodajaca rekordy do tabeli
składnia INSERT INTO nazwa_tabeli (pole_1, pole_2) VALUES (wartosc_1, wartosc_2)
przykład INSERT INTO dog (id, name) VALUES (1, reks)

delete() metoda usuwająca rekordy z tabeli 
składnia: DELETE FROM nazwa_tabeli WHERE pole = wartosc
przykład polecenia: DELETE FROM dog WHERE id = 2
uwagi: metoda ograniczona jest do jednego warunku WHERE tzn nie wspiera AND/OR

alterTable() metoda aktualizująca strukture tabeli
składnia ALTER TABLE nazwa_tabeli ADD nazwa_kolumny || ALTER TABLE nazwa_tabeli DROP COLUMN nazwa_kolumny
przykład ALTER TABLE dog ADD breed || ALTER TABLE dog DROP COLUMN id

updateTable() metoda aktualizująca dane w tabeli
składnia UPDATE nazwa_tabeli SET nazwa_kolumny=wartosc WHERE parametr=wartosc
przykład UPDATE dog SET name=azor WHERE name=reks
uwagi: metoda ograniczona jest do porównywania tylko przy użyciu znaku "="

select() metoda służy do pobierania rekordów
składnia SELECT pola FROM nazwa_tabeli WHERE pole=wartosc ORDER BY nazwa_kolumny ASC/DESC LIMIT 

Brakujące elementy:
Niestety typy danych nie są wspierane w bazie danych.
Brak walidacji uniklaności