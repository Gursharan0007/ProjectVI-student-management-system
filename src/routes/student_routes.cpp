#include "student_routes.h"
#include "../db/database.h"
#include <sqlite3.h>

// Register all Student Management routes
void registerStudentRoutes(crow::SimpleApp& app) {

    // ---------------------------------------------------
    // POST /api/students  → Create Student
    // ---------------------------------------------------
    CROW_ROUTE(app, "/api/students").methods("POST"_method)
    ([](const crow::request& req) {
        auto body = crow::json::load(req.body);
        if (!body)
            return crow::response(400, "Invalid JSON");

        sqlite3* db = Database::getConnection();

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
            return crow::response(400, "Failed to create student");
        }

        sqlite3_finalize(stmt);
        return crow::response(201, "Student created successfully");
    });

    // ---------------------------------------------------
    // GET /api/students  → Get All Students
    // ---------------------------------------------------
    CROW_ROUTE(app, "/api/students").methods("GET"_method)
    ([]() {
        sqlite3* db = Database::getConnection();
        sqlite3_stmt* stmt;

        crow::json::wvalue result;
        int index = 0;

        sqlite3_prepare_v2(db, "SELECT * FROM Student;", -1, &stmt, nullptr);

        while (sqlite3_step(stmt) == SQLITE_ROW) {
            result[index]["studentID"] = sqlite3_column_int(stmt, 0);
            result[index]["firstName"] = (const char*)sqlite3_column_text(stmt, 1);
            result[index]["lastName"]  = (const char*)sqlite3_column_text(stmt, 2);
            result[index]["email"]     = (const char*)sqlite3_column_text(stmt, 3);
            result[index]["age"]       = sqlite3_column_int(stmt, 4);
            result[index]["program"]   = (const char*)sqlite3_column_text(stmt, 5);
            index++;
        }

        sqlite3_finalize(stmt);
        return crow::response(result);
    });

    // ---------------------------------------------------
    // PUT /api/students/{studentID}  → Full Update
    // ---------------------------------------------------
    CROW_ROUTE(app, "/api/students/<int>").methods("PUT"_method)
    ([](const crow::request& req, int studentID) {
        auto body = crow::json::load(req.body);
        if (!body)
            return crow::response(400, "Invalid JSON");

        sqlite3* db = Database::getConnection();

        std::string firstName = body["firstName"].s();
        std::string lastName  = body["lastName"].s();
        std::string email     = body["email"].s();
        std::string program   = body["program"].s();

        std::string sql =
            "UPDATE Student SET firstName=?, lastName=?, email=?, age=?, program=? "
            "WHERE studentID=?;";

        sqlite3_stmt* stmt;
        sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);

        sqlite3_bind_text(stmt, 1, firstName.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 2, lastName.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 3, email.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_int(stmt, 4, body["age"].i());
        sqlite3_bind_text(stmt, 5, program.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_int(stmt, 6, studentID);

        if (sqlite3_step(stmt) != SQLITE_DONE) {
            sqlite3_finalize(stmt);
            return crow::response(400, "Failed to update student");
        }

        sqlite3_finalize(stmt);
        return crow::response(200, "Student updated successfully");
    });

    // ---------------------------------------------------
    // PATCH /api/students/{studentID}  → Partial Update
    // ---------------------------------------------------
    CROW_ROUTE(app, "/api/students/<int>").methods("PATCH"_method)
    ([](const crow::request& req, int studentID) {
        auto body = crow::json::load(req.body);
        if (!body)
            return crow::response(400, "Invalid JSON");

        sqlite3* db = Database::getConnection();
        sqlite3_stmt* stmt;

        if (body.has("email")) {
            std::string email = body["email"].s();
            std::string sql = "UPDATE Student SET email=? WHERE studentID=?;";
            sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
            sqlite3_bind_text(stmt, 1, email.c_str(), -1, SQLITE_TRANSIENT);
            sqlite3_bind_int(stmt, 2, studentID);
            sqlite3_step(stmt);
            sqlite3_finalize(stmt);
        }

        if (body.has("program")) {
            std::string program = body["program"].s();
            std::string sql = "UPDATE Student SET program=? WHERE studentID=?;";
            sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
            sqlite3_bind_text(stmt, 1, program.c_str(), -1, SQLITE_TRANSIENT);
            sqlite3_bind_int(stmt, 2, studentID);
            sqlite3_step(stmt);
            sqlite3_finalize(stmt);
        }

        return crow::response(200, "Student partially updated");
    });

    // ---------------------------------------------------
    // DELETE /api/students/{studentID}
    // ---------------------------------------------------
    CROW_ROUTE(app, "/api/students/<int>").methods("DELETE"_method)
    ([](int studentID) {
        sqlite3* db = Database::getConnection();
        sqlite3_stmt* stmt;

        std::string sql = "DELETE FROM Student WHERE studentID=?;";
        sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
        sqlite3_bind_int(stmt, 1, studentID);

        if (sqlite3_step(stmt) != SQLITE_DONE) {
            sqlite3_finalize(stmt);
            return crow::response(400, "Failed to delete student");
        }

        sqlite3_finalize(stmt);
        return crow::response(200, "Student deleted successfully");
    });
}
