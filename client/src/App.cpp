
#include "WSListener.hpp"

#include <oatpp-websocket/Connector.hpp>
#include <oatpp-websocket/WebSocket.hpp>

#include <oatpp/network/tcp/client/ConnectionProvider.hpp>

#include <thread>

namespace {

  const char* TAG = "websocket-client";

  bool finished = false;

  void socketTask(const std::shared_ptr<oatpp::websocket::WebSocket>& websocket)
  {
    websocket->listen();
    OATPP_LOGd(TAG, "SOCKET CLOSED!!!");
    finished = true;
  }

} // namespace

void run()
{

  OATPP_LOGi(TAG, "Application Started");

  auto connectionProvider =
      oatpp::network::tcp::client::ConnectionProvider::createShared({"localhost", 3333});

  auto connector = oatpp::websocket::Connector::createShared(connectionProvider);

  auto connection = connector->connect("ws");

  OATPP_LOGi(TAG, "Connected");

  auto socket = oatpp::websocket::WebSocket::createShared(
      connection, true /* maskOutgoingMessages must be true for clients */);

  std::mutex socketWriteMutex;

  socket->setListener(std::make_shared<WSListener>(socketWriteMutex));

  std::thread thread(socketTask, socket);

  while(!finished) {
    {
      OATPP_LOGd(TAG, "sending message...");
      std::lock_guard<std::mutex> lock(socketWriteMutex);
      socket->sendOneFrameText("hello");
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  }

  thread.join();
}

int main()
{
  oatpp::Environment::init();
  run();
  oatpp::Environment::destroy();
  return 0;
}
