#pragma once
#include <drogon/HttpController.h>

class ShortenController : public drogon::HttpController<ShortenController> {
public:
	METHOD_LIST_BEGIN
	ADD_METHOD_TO(ShortenController::shorten, "/api/shorten", drogon::Post, drogon::Options);
	ADD_METHOD_TO(ShortenController::stats, "/api/stats/{1}", drogon::Get);
	METHOD_LIST_END


	drogon::Task<> shorten(drogon::HttpRequestPtr req, 
			std::function<void(const drogon::HttpResponsePtr &)> callback);

	drogon::Task<> stats(drogon::HttpRequestPtr req,
		      std::function<void(const drogon::HttpResponsePtr &)> callback,
		      std::string code);
};
