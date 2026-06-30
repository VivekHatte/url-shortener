#include "ShortenController.hpp"
#include "../utils/Base62.hpp"
#include <drogon/orm/Exception.h>

using namespace drogon;

namespace {

std::string baseUrlFromRequest(const HttpREquestPtr & req){
	std::string scheme = req->getHeader("x-forwarded-proto");
	if(scheme.empty()) {
		scheme = "http";
	}
	std::string host{ req->getHeader("host") };
	if(host.empty()) {
		host = "localhost:8080";
	}
	return scheme + "://" + host;
}

HttpResponsePtr jsonError(HttpStatusCode code, const std::string &message) {
	Json::Value body;
	body["error"] = message;
	auto resp = HttpResponse::newHttpJsonResponse(body);
	resp->setStatusCode(code);
	return resp;
}

bool looksLikeUrl(const std::string &url) {
	return url.rfind("http://", 0) == 0 || url.rfind("https://", 0) == 0;
}
}

Task<> ShortenController::shorten(HttpRequestPtr req, std::function<void(const HttpResponsePtr &)> callback) {

	if(req->method() == Options) {
		auto resp = HttpResponse::newHttpResponse();
		callback(resp);
		co_return;
	}

	auto json = req->getJsonObject();
	if(!json || !json->isMember("url") || !(*json)["url"].isString()) {
		callback(jsonError(k400BadRequest, "Body must be JSON with a 'url' field"));
		co_return;
	}

	const std::string longUrl{ (*json)["url"].asString() };
	if (!looksLikeUrl(longUrl)) {
		callback(jsonError(k400BadRequest, "url must start with http:// or https://"));
		co_return;
	}

	const std::string alias{ json->get("alias", "").asString() };

	auto db{ app().getDbClient() };

	try {
		std::string code;

		if(!alias.empty()) {
			try {
				co_await db->execSqlCoro("INSERT INTO urls (code, long_url) VALUES ($1, $2)", alias, longUrl);
				code = alias;
			} catch (const orm::DrogonDbException &){
				callback(jsonError(k409Conflict, "That alias is already taken"));
				co_return;
			}
		} else {
			auto inserted{co_await db->execSqlCoro("INSERT INTO urls (long_url) VALUES ($1) RETURNING id", longUrl)};
			const auto id{inserted[0]["id"].as<uint64_t>()};
			code = util::toBase62(id);
			co_await db->execSqlCoro("UPDATE urls SET code = $1 WHERE id = $2", code, id);
		}

		Json::Value body;
		body["code"] = code;
		body["long_url"] = longUrl;
		body["short_url"] = baseUrlFromRequest(req) + "/" + code;
		auto resp{HttpResponse::newHttpJsonResponse(body)};
		resp->setStatusCode(k201Created);
		callback(resp);
	} catch(const orm::DrogonDbException &e) {
		LOG_ERROR << "shorten failed" " << e.base().what();
		callback(jsonError(k500InternalServerError, "database error"));
	}
	co_return;
}

Task<> ShortenController::stats(HttpRequestPtr req, std::function<void(const HttpResponsePtr &)> callback, std::string code) {
	auto db = app().getDbClient();
	try {
		auto rows = co_await db->execSqlCoro("SELECT code, long_url, clicks, created_at FROM urls WHERE code = $1", code);
		if(rows.empty()) {
			callback(jsonError(k404NotFound, "No such code"));
			co_return;
		}

		const auto &row  = rows[0];
		Json::Value body;
		body["code"] = row["code"].as<std::string>();
		body["long_url"] = row["long_url"].as<std::string>();
		body["clicks"] = row["clicks"].as<int64_t>();
		body["created_at"] = row["created_at"].as<std::string>();
		callback(HttpResponse::newHttpJsonResponse(body));
	} catch (cost orm::DrogonDbException &e) {
		LOG_ERROR << "stats failed: " << e.base().what();
		callback(jsonError(k500InternalServerError,  "Database Error"));
	}
	co_return;
}
