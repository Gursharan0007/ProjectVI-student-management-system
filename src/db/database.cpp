#include "database.h"
#include <iostream>

sqlite3* Database::getConnection() {
    static sqlite3* db = nullptr;

    if (!db) {
        if (sqlite3_open("student.db", &db)) {
            std::cerr << "Cannot open database\n";
            return nullptr;
        }
    }
    return db;
}

void Database::initialize() {
    const char* studentTableSQL =
        "CREATE TABLE IF NOT EXISTS Student ("
        "studentID INTEGER PRIMARY KEY,"
        "firstName TEXT,"
        "lastName TEXT,"
        "email TEXT,"
        "age INTEGER,"
        "program TEXT"
        ");";

    char* errMsg = nullptr;
    sqlite3_exec(getConnection(), studentTableSQL, nullptr, nullptr, &errMsg);

    if (errMsg) {
        std::cerr << "SQL error: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    }
}
