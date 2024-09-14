#ifndef BestMoveDto_hpp
#define BestMoveDto_hpp

#include "oatpp/Types.hpp"
#include "oatpp/macro/codegen.hpp"

#include OATPP_CODEGEN_BEGIN(DTO)

class BestMoveDto : public oatpp::DTO
{

  DTO_INIT(BestMoveDto, DTO)

  DTO_FIELD(String, id);
};

#include OATPP_CODEGEN_END(DTO)

#endif