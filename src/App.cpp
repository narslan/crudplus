
#include "AppComponent.hpp"
#include "controller/BestMoveController.hpp"
#include "controller/StaticController.hpp"
#include "controller/StatisticsController.hpp"

#include "controller/UserController.hpp"

#include "oatpp-swagger/Controller.hpp"

#include "oatpp/network/Server.hpp"

#include <iostream>

void run() {

  AppComponent components(
      const oatpp::base::CommandLineArguments& args); // Create scope Environment components

  /* Get router component */
  OATPP_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>, router);

  oatpp::web::server::api::Endpoints docEndpoints;

  docEndpoints.append(router->addController(UserController::createShared())->getEndpoints());
  docEndpoints.append(router->addController(BestMoveController::createShared())->getEndpoints());
  router->addController(oatpp::swagger::Controller::createShared(docEndpoints));
  router->addController(StaticController::createShared());
  router->addController(std::make_shared<StatisticsController>());

  /* Get connection handler component */
  OATPP_COMPONENT(std::shared_ptr<oatpp::network::ConnectionHandler>, connectionHandler, "http");

  /* Get connection provider component */
  OATPP_COMPONENT(std::shared_ptr<oatpp::network::ServerConnectionProvider>, connectionProvider);

  /* create server */
  oatpp::network::Server server(connectionProvider, connectionHandler);
  OATPP_COMPONENT(std::shared_ptr<oatpp::provider::Provider<oatpp::sqlite::Connection>>,
                  dbConnectionProvider);
  OATPP_LOGd("Server", "Running on port {}...", connectionProvider->getProperty("port").toString())

      std::thread serverThread([&server] { server.run(); });

  std::thread pingThread([] {
    OATPP_COMPONENT(std::shared_ptr<Lobby>, lobby);
    lobby->runPingLoop(std::chrono::seconds(30));
  });

  std::thread statThread([] {
    OATPP_COMPONENT(std::shared_ptr<Statistics>, statistics);
    statistics->runStatLoop();
  });
  OATPP_COMPONENT(oatpp::Object<ConfigDto>, appConfig);
  if(appConfig->useTLS) {
    OATPP_LOGi("chesskk",
               "clients are expected to connect at https://{}:{}/",
               appConfig->host,
               appConfig->port);
  }
  else {
    OATPP_LOGi("chesskk",
               "clients are expected to connect at http://{}:{}/",
               appConfig->host,
               appConfig->port);
  }

  OATPP_LOGi("chesskk", "canonical base URL={}", appConfig->getCanonicalBaseUrl());
  OATPP_LOGi("chesskk", "statistics URL={}", appConfig->getStatsUrl());

  serverThread.join();
  pingThread.join();
  statThread.join();
}

/**
 *  main
 */
int main(int argc, const char* argv[]) {

  oatpp::Environment::init();

  run();

  /* Print how many objects were created during app running, and what have left-probably leaked */
  /* Disable object counting for release builds using '-D OATPP_DISABLE_ENV_OBJECT_COUNTERS' flag for better performance */
  std::cout << "\nEnvironment:\n";
  std::cout << "objectsCount = " << oatpp::Environment::getObjectsCount() << "\n";
  std::cout << "objectsCreated = " << oatpp::Environment::getObjectsCreated() << "\n\n";

  oatpp::Environment::destroy();

  return 0;
}
