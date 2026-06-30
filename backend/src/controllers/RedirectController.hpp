#pragma once
#include <drogon/HttpController.h>

class RedirectController : public drogon::HttpController<RedirectController> {
public:
	METHOD_LIST_BEGIN
	ADD_METHOD_TO(RedirectController::redirect, "/{1}", drogon::Get);
	METHOD_LIST_END

	drogon::Task<> redirect(drogon::HttpRequestPtr req, std::function<void(const drogon::HttpResponsePtr &)> callback, 
			 std::string code);

};
