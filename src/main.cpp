#include "crow.h"
#include "db/database.h"
#include <sqlite3.h>

int main() {
    // Initialize database and tables
    Database::initialize();

    crow::SimpleApp app;

    // ---------------------------------------------------
    // Root Route (Health Check)
    // ---------------------------------------------------
    CROW_ROUTE(app, "/")([]() {
        crow::json::wvalue res;
        res["message"] = "Student Management System API is running";
        return res;
    });

    // ---------------------------------------------------
    // POST /api/students (Create Student)
    // ---------------------------------------------------
CROW_ROUTE(app, "/api/students").methods("POST"_method)
([](const crow::request& req) {
    auto body = crow::json::load(req.body);
    if (!body)
        return crow::response(400, "Invalid JSON");

    sqlite3* db = Database::getConnection();
    if (!db)
        return crow::response(500, "Database error");

    // Convert Crow JSON strings to std::string
    std::string firstName = body["firstName"].s();
    std::string lastName  = body["lastName"].s();
    std::string email     = body["email"].s();
    std::string program   = body["program"].s();

    std::string sql = "INSERT INTO Student VALUES (?, ?, ?, ?, ?, ?);";
    sqlite3_stmt* stmt;

    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);

    sqlite3_bind_int(stmt, 1, body["studentID"].i());
    sqlite3_bind_text(stmt, 2, firstName.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, lastName.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 4, email.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 5, body["age"].i());
    sqlite3_bind_text(stmt, 6, program.c_str(), -1, SQLITE_TRANSIENT);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        sqlite3_finalize(stmt);
        return crow::response(400, "Insert failed");
    }

    sqlite3_finalize(stmt);
    return crow::response(201, "Student created");
});


    // ---------------------------------------------------
    // GET /api/students (Get All Students)
    // ---------------------------------------------------
    CROW_ROUTE(app, "/api/students").methods("GET"_method)
    ([]() {
        sqlite3* db = Database::getConnection();
        if (!db) {
            return crow::response(500, "Database error");
        }

        sqlite3_stmt* stmt;
        crow::json::wvalue result;
        int index = 0;

        sqlite3_prepare_v2(db, "SELECT * FROM Student;", -1, &stmt, nullptr);

        while (sqlite3_step(stmt) == SQLITE_ROW) {
            result[index]["studentID"] = sqlite3_column_int(stmt, 0);
            result[index]["firstName"] = (const char*)sqlite3_column_text(stmt, 1);
            result[index]["lastName"] = (const char*)sqlite3_column_text(stmt, 2);
            result[index]["email"] = (const char*)sqlite3_column_text(stmt, 3);
            result[index]["age"] = sqlite3_column_int(stmt, 4);
            result[index]["program"] = (const char*)sqlite3_column_text(stmt, 5);
            index++;
        }

        sqlite3_finalize(stmt);
        return crow::response(result);
    });

    // ---------------------------------------------------
    // Start Server
    // ---------------------------------------------------
    app.port(18081).multithreaded().run();
}
