#include "crow.h"
#include "db/database.h"
#include "routes/student_routes.h"

int main() {
    Database::initialize();

    crow::SimpleApp app;

    // Root health check
    CROW_ROUTE(app, "/")([]() {
        crow::json::wvalue res;
        res["message"] = "Student Management System API is running";
        return res;
    });

    // Register student routes
    registerStudentRoutes(app);

    app.port(18081).multithreaded().run();
}
