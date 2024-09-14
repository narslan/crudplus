#ifndef CRUD_BESTMOVESERVICE_HPP
#define CRUD_BESTMOVESERVICE_HPP

#include "dto/BestMoveDto.hpp"
#include "dto/StatusDto.hpp"
#include <oatpp/web/protocol/http/Http.hpp>
class BestMoveService
{
  private:
  typedef oatpp::web::protocol::http::Status Status;

  public:
  oatpp::Object<BestMoveDto> createGame();
};
#endif