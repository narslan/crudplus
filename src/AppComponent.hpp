
#ifndef AppComponent_hpp
#define AppComponent_hpp

#include "DatabaseComponent.hpp"
#include "SwaggerComponent.hpp"

#include "ErrorHandler.hpp"
#include "ws/Lobby.hpp"

#include <oatpp/web/mime/ContentMappers.hpp>
#include <oatpp/web/server/HttpConnectionHandler.hpp>
#include <oatpp/web/server/HttpRouter.hpp>

#include <oatpp/network/tcp/server/ConnectionProvider.hpp>

#include <oatpp/json/ObjectMapper.hpp>

#include <oatpp/web/server/AsyncHttpConnectionHandler.hpp>

#include <oatpp/macro/component.hpp>
/**
 *  Class which creates and holds Application components and registers components in oatpp::base::Environment
 *  Order of components initialization is from top to bottom
 */
class AppComponent
{
  public:
  /**
   *  Swagger component
   */
  SwaggerComponent swaggerComponent;

  /**
   * Database component
   */
  DatabaseComponent databaseComponent;

  /**
   * Create Async Executor
   */
  OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::async::Executor>, executor)
  ([] {
    return std::make_shared<oatpp::async::Executor>(
        4 /* Data-Processing threads */, 1 /* I/O threads */, 1 /* Timer threads */
    );
  }());

  /**
   * Create ObjectMapper component to serialize/deserialize DTOs in Controller's API
   */
  OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::web::mime::ContentMappers>, apiContentMappers)
  ([] {
    auto json = std::make_shared<oatpp::json::ObjectMapper>();
    json->serializerConfig().json.useBeautifier = true;

    auto mappers = std::make_shared<oatpp::web::mime::ContentMappers>();
    mappers->putMapper(json);

    return mappers;
  }());

  /**
   *  Create ConnectionProvider component which listens on the port
   */
  OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::network::ServerConnectionProvider>,
                         serverConnectionProvider)
  ([] {
    return oatpp::network::tcp::server::ConnectionProvider::createShared(
        {"0.0.0.0", 8000, oatpp::network::Address::IP_4});
  }());

  /**
   *  Create Router component
   */
  OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>, httpRouter)
  ([] { return oatpp::web::server::HttpRouter::createShared(); }());

  /**
   *  Create ConnectionHandler component which uses Router component to route requests
   */
  OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::network::ConnectionHandler>,
                         serverConnectionHandler)
  ([] {
    OATPP_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>,
                    router); // get Router component
    OATPP_COMPONENT(std::shared_ptr<oatpp::web::mime::ContentMappers>,
                    contentMappers); // get ContentMappers component

    auto connectionHandler = oatpp::web::server::HttpConnectionHandler::createShared(router);
    connectionHandler->setErrorHandler(std::make_shared<ErrorHandler>(contentMappers));
    return connectionHandler;
  }());

  /**
   *  Create websocket connection handler
   */
  OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::network::ConnectionHandler>,
                         websocketConnectionHandler)
  ("websocket", [] {
    OATPP_COMPONENT(std::shared_ptr<oatpp::async::Executor>, executor);
    auto connectionHandler = oatpp::websocket::AsyncConnectionHandler::createShared(executor);
    connectionHandler->setSocketInstanceListener(std::make_shared<Lobby>());
    return connectionHandler;
  }());
};

#endif /* AppComponent_hpp */
