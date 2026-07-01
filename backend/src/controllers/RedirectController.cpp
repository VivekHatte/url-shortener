#include "RedirectController.hpp"
#include <drogon/orm/Exception.h>

using namespace drogon;

Task<> RedirectController::redirect(HttpRequestPtr req, std::function<void(const HttpResponsePtr &)> callback, std::string code) {
	auto db{app().getDbClient()};

	try {
		auto rows = co_await db->execSqlCoro("SELECT long_url FROM urls WHERE code = $1", code);

		if(rows.empty()) {
			auto resp{HttpResponse::newHttpResponse()};
			resp->setStatusCode(k404NotFound);
			resp->setBody("Short link not found");
			callback(resp);
			co_return;
		}

		const auto longUrl = rows[0]["long_url"].as<std::string>();

		// add click count for analytics
		co_await db->execSqlCoro("UPDATE urls SET clicks = clicks + 1 WHERE code = $1", code);

		auto resp = HttpResponse::newRedirectionResponse(longUrl, k302Found);
		callback(resp);
		
	} catch (const orm::DrogonDbException &e) {
		LOG_ERROR << "redirect failed: " << e.base().what();

		auto resp = HttpResponse::newHttpResponse();
		resp->setStatusCode(k500InternalServerError);
		resp->setBody("Server error");
		callback(resp);
	}
	co_return;
}
