#include "propertybase.h"
#include "dbus-cxx-private.h"

namespace DBus {
  PropertyBase::PropertyBase(const std::string& name) : m_name(name) {
    SIMPLELOGGER_DEBUG( "dbus.MethodBase", "Creating new property with name " << name );
  }

  const std::string& PropertyBase::name() {
    return m_name;
  }
}
