#include "HealthController.hpp"
#include <drogon/orm/Exception.h>

using namespace drogon;

Task<> HealthController::health(HttpRequestPtr req, std::function<void(const HttpResponsePtr &)> callback) {
	Json::Value body;
	
	try {
		co_await app().getDbClient()->execSqlCoro("SELECT 1");
		body["status"] = "ok";
		body["database"] = "up";
		callback(HttpResponse::newHttpJsonResponse(body));
	} catch (const orm::DrogonDbException &e) {
		LOG_ERROR << "Health cehck db failure: " << e.base().what();
		body["status"] = "degraded";
		body["database"] = "down";
		auto resp = HttpResponse::newHttpJsonResponse(body);
		resp->setStatusCode(k503ServiceUnavailable);
		callback(resp);
	}

	co_return;
}
