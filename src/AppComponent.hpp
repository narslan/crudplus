
#ifndef AppComponent_hpp
#define AppComponent_hpp

#include "DatabaseComponent.hpp"
#include "SwaggerComponent.hpp"

#include "ErrorHandler.hpp"
#include "ws/Lobby.hpp"

#include <oatpp/web/mime/ContentMappers.hpp>
#include <oatpp/web/server/HttpConnectionHandler.hpp>

#include <oatpp/network/tcp/server/ConnectionProvider.hpp>

#include <oatpp/json/ObjectMapper.hpp>

#include <oatpp/web/server/AsyncHttpConnectionHandler.hpp>
#include <oatpp/web/server/HttpRouter.hpp>

#include "utils/Statistics.hpp"
#include <oatpp/base/CommandLineArguments.hpp>
#include <oatpp/macro/component.hpp>
/**
 *  Class which creates and holds Application components and registers components in oatpp::base::Environment
 *  Order of components initialization is from top to bottom
 */
class AppComponent {

  private:
  oatpp::base::CommandLineArguments m_cmdArgs;

  public:
  AppComponent(const oatpp::base::CommandLineArguments& cmdArgs)
      : m_cmdArgs(cmdArgs) { }

  public:
  /**
   * Create config component
   */
  OATPP_CREATE_COMPONENT(oatpp::Object<ConfigDto>, appConfig)
  ([this] {
    auto config = ConfigDto::createShared();

    config->host = std::getenv("EXTERNAL_ADDRESS");
    if(!config->host) {
      config->host = m_cmdArgs.getNamedArgumentValue("--host", "localhost");
    }

    const char* portText = std::getenv("EXTERNAL_PORT");
    if(!portText) {
      portText = m_cmdArgs.getNamedArgumentValue("--port", "8443");
    }

    bool success;
    auto port = oatpp::utils::Conversion::strToUInt32(portText, success);
    if(!success || port > 65535) {
      throw std::runtime_error("Invalid port!");
    }
    config->port = (v_uint16)port;

    // config->tlsPrivateKeyPath = std::getenv("TLS_FILE_PRIVATE_KEY");
    // if(!config->tlsPrivateKeyPath) {
    //   config->tlsPrivateKeyPath = m_cmdArgs.getNamedArgumentValue("--tls-key", "" CERT_PEM_PATH);
    // }

    // config->tlsCertificateChainPath = std::getenv("TLS_FILE_CERT_CHAIN");
    // if(!config->tlsCertificateChainPath) {
    //   config->tlsCertificateChainPath =
    //       m_cmdArgs.getNamedArgumentValue("--tls-chain", "" CERT_CRT_PATH);
    // }

    config->statisticsUrl = std::getenv("URL_STATS_PATH");
    if(!config->statisticsUrl) {
      config->statisticsUrl = m_cmdArgs.getNamedArgumentValue("--url-stats", "admin/stats.json");
    }

    return config;
  }());
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
   *  Create ObjectMapper component to serialize/deserialize DTOs in Contoller's API
   */
  OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::data::mapping::ObjectMapper>, apiObjectMapper)
  ([] {
    auto mapper = std::make_shared<oatpp::json::ObjectMapper>();
    mapper->serializerConfig().mapper.includeNullFields = false;
    return mapper;
  }());

  /**
   *  Create statistics object
   */
  OATPP_CREATE_COMPONENT(std::shared_ptr<Statistics>, statistics)
  ([] { return std::make_shared<Statistics>(); }());
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
