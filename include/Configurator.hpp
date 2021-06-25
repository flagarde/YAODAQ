#pragma once

#include "Module.hpp"
#include "soci/session.h"

#include <string>

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
