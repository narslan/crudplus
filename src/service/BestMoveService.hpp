#ifndef CRUD_BESTMOVESERVICE_HPP
#define CRUD_BESTMOVESERVICE_HPP

#include <oatpp/web/protocol/http/Http.hpp>
#include <string>
class BestMoveService
{
  private:
  typedef oatpp::web::protocol::http::Status Status;

  public:
  std::string createGame();
};
#endif