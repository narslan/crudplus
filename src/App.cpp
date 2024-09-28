#include "./AppComponent.hpp"
#include "./controller/MyController.hpp"

#include <oatpp/network/Server.hpp>

#include <iostream>

void run() {

  /* Register Components in scope of run() method */
  AppComponent components;

  /* Get router component */
  OATPP_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>, router);

  /* Create MyController and add all of its endpoints to router */
  router->addController(std::make_shared<MyController>());

  /* Get connection handler component */
  OATPP_COMPONENT(std::shared_ptr<oatpp::network::ConnectionHandler>, connectionHandler, "http");

  /* Get connection provider component */
  OATPP_COMPONENT(std::shared_ptr<oatpp::network::ServerConnectionProvider>, connectionProvider);

  /* Create server which takes provided TCP connections and passes them to HTTP connection handler */
  oatpp::network::Server server(connectionProvider, connectionHandler);

  /* Priny info about server port */
  //OATPP_LOGi(
  //    "MyApp", "Server running on port='{}'", connectionProvider->getProperty("port").getData());
  OATPP_LOGi("MyApp", "Server running on port={}", 8000);
  /* Run server */
  server.run();
}

int main(int argc, const char* argv[]) {

  oatpp::Environment::init();

  run();

  oatpp::Environment::destroy();

  return 0;
}