CROW_ROUTE(app, "/api/students").methods("GET"_method)
([](){
    return crow::json::wvalue{{"status", "GET students"}};
});

CROW_ROUTE(app, "/api/students").methods("POST"_method)
([](const crow::request& req){
    return crow::json::wvalue{{"status", "POST student"}};
});

CROW_ROUTE(app, "/api/students/<int>").methods("PUT"_method)
([](int id){
    return crow::json::wvalue{{"status", "PUT student"}, {"id", id}};
});

CROW_ROUTE(app, "/api/students/<int>").methods("PATCH"_method)
([](int id){
    return crow::json::wvalue{{"status", "PATCH student"}, {"id", id}};
});

CROW_ROUTE(app, "/api/students/<int>").methods("DELETE"_method)
([](int id){
    return crow::json::wvalue{{"status", "DELETE student"}, {"id", id}};
});

CROW_ROUTE(app, "/api/students").methods("OPTIONS"_method)
([](){
    crow::response res;
    res.add_header("Allow", "GET, POST, PUT, PATCH, DELETE, OPTIONS");
    return res;
});
