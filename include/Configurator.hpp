#pragma once

#include <string>

#include "Module.hpp"
#include "soci/session.h"

namespace yaodaq
{

class Configurator: public Module
{
public:
  Configurator(const std::string&);

private:
  soci::session m_session;
};

}
