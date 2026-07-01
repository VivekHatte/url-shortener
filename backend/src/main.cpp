#include <drogon/drogon.h>

#include <cstdlib>
#include <string>

using namespace drogon;

namespace {
std::string env(const char *key, const std::string &fallback) {
	const char* value{std::getenv(key)};
	return value ? std::string(value) : fallback;

}
}

int main() {
	const auto httpPort = static_cast<uint16_t>(std::stoi(env(("PORT"), "8080")));
	const auto dbHost = env("DB_HOST", "local_host");
	const auto dbPort = static_cast<unsigned short>(std::stoi(env("DB_PORT", "5432")));
	const auto dbName = env("DB_NAME", "urlshortener");
	const auto dbUser = env("DB_USER", "postgres");
	const auto dbPassword = env("DB_PASSWORD", "postgres");	// TODO: change to aws secret

	app().addListener("0.0.0.0", httpPort);

	orm::PostgresConfig dbConfig;
	dbConfig.host = dbHost;
	dbConfig.port = dbPort;
	dbConfig.databaseName = dbName;
	dbConfig.username = dbUser;
	dbConfig.password = dbPassword;
	dbConfig.connectionNumber = 4;
	dbConfig.name = "default";
	dbConfig.isFast = false;
	dbConfig.characterSet = "";
	dbConfig.timeout = -1.0;
	dbConfig.autoBatch = false;
	app().addDbClient(dbConfig);

	app().registerPostHandlingAdvice([](const HttpRequestPtr &, const HttpResponsePtr &resp) {
		resp->addHeader("Access-Control-Allow-Origin", "*");
		resp->addHeader("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
		resp->addHeader("Access-Control-Allow-Headers", "Content-Type");
	});

	app().setLogLevel(trantor::Logger::kInfo);
	LOG_INFO << "Url Shortener backend listening on port" << httpPort;

	app().run();
	return 0;
}
