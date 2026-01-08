#include "headers/base.h"

literal createEmptyLiteral()
{
  literal result;
  result.type = TYPE_UNDEFINED;
  result.ivalue = 0;
  result.fvalue = 0.0f;
  return result;
}

literal createIntegerLiteral(int ivalue)
{
  literal result;
  result.type = TYPE_INTEGER;
  result.ivalue = ivalue;
  return result;
}

literal createFloatLiteral(float fvalue)
{
  literal result;
  result.type = TYPE_FLOAT;
  result.fvalue = fvalue;
  return result;
}
