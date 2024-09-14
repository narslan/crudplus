#include "BestMoveService.hpp"
#include "UCI.hpp"
#include <iostream>

oatpp::Object<BestMoveDto> BestMoveService::createGame()
{
  uciadapter::UCI u("/usr/bin/stockfish");

  //std::string buf = u.GetBuffer();

  u.position("2k2r2/6R1/8/8/8/6Q1/4K3/8 w - - 0 1");
  u.go(uciadapter::Go()); // Launch go with no arguments
  u.SyncAfter(2); // Wait 2s and fetch data from engine
  auto blines = u.GetLines();

  std::string buf;

  for(auto& [key, value] : blines) {
    std::cout << key << '-' << value.cpuload << std::endl;
    buf = buf + value.pv[0];
  }

  u.quit();

  auto x = BestMoveDto::createShared();
  x->id = buf;
  return x;
};