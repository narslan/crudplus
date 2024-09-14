#include "BestMoveService.hpp"
#include "UCI.hpp"
#include <string>

std::string BestMoveService::createGame()
{
  uciadapter::UCI u("/usr/bin/stockfish");

  std::string buf = u.GetBuffer();
  u.stop();
  return buf;
};