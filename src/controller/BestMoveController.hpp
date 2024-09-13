
#ifndef BestmoveController_hpp
#define BestmoveController_hpp

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
};
#endif