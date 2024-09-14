
#ifndef BestmoveController_hpp
#define BestmoveController_hpp

#include "dto/BestMoveDto.hpp"
#include "dto/StatusDto.hpp"
#include "service/BestMoveService.hpp"

#include "oatpp/web/mime/ContentMappers.hpp"
#include "oatpp/web/server/api/ApiController.hpp"

#include "oatpp/macro/codegen.hpp"
#include <oatpp/macro/component.hpp>
#include OATPP_CODEGEN_BEGIN(ApiController) //<- Begin Codegen
/**
 * BestMove REST controller.
 */
class BestMoveController : public oatpp::web::server::api::ApiController
{
  public:
  BestMoveController(OATPP_COMPONENT(std::shared_ptr<oatpp::web::mime::ContentMappers>,
                                     apiContentMappers))
      : oatpp::web::server::api::ApiController(apiContentMappers)
  { }

  private:
  BestMoveService m_bestMoveService; // Create best move service.
  public:
  static std::shared_ptr<BestMoveController>
  createShared(OATPP_COMPONENT(std::shared_ptr<oatpp::web::mime::ContentMappers>,
                               apiContentMappers) // Inject ContentMappers
  )
  {
    return std::make_shared<BestMoveController>(apiContentMappers);
  }

  ENDPOINT_INFO(createGame)
  {
    info->summary = "Get one User by userId";

    info->addResponse<Object<BestMoveDto>>(Status::CODE_200, "application/json");
    info->addResponse<Object<StatusDto>>(Status::CODE_404, "application/json");
    info->addResponse<Object<StatusDto>>(Status::CODE_500, "application/json");
  }
  ENDPOINT("GET", "moves", getUserById, PATH(Int32, userId))
  {
    return createDtoResponse(Status::CODE_200, m_bestMoveService.createGame());
  }
};
#endif