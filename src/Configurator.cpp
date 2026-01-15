#include "Configurator.hpp"

#include <string>

//#include "soci/soci-config.h"
#ifdef SOCI_HAVE_DB2
  #include "soci/db2/soci-db2.h"
#endif
#ifdef SOCI_HAVE_MYSQL
  #include "soci/mysql/soci-mysql.h"
#endif
#ifdef SOCI_HAVE_ORACLE
  #include "soci/oracle/soci-oracle.h"
#endif
#ifdef SOCI_HAVE_POSTGRESQL
  #include "soci/postgresql/soci-postgresql.h"
#endif
#ifdef SOCI_HAVE_SQLITE3
  #include "soci/sqlite3/soci-sqlite3.h"
#endif
#ifdef SOCI_HAVE_ODBC
  #include "soci/odbc/soci-odbc.h"
#endif
#ifdef SOCI_HAVE_FIREBIRD
  #include "soci/firebird/soci-firebird.h"
#endif
#ifdef SOCI_HAVE_EMPTY
  #include "soci/empty/soci-empty.h"
#endif

namespace yaodaq
{

Configurator::Configurator(const std::string& name) : Module(name,"Configurator",CLASS::Configurator)
{
  //m_session.open(soci::mysql, "");
}

}
