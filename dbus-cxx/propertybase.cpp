#include "propertybase.h"

namespace DBus {
  PropertyBase::PropertyBase(const std::string& name) : m_name(name) {

  }

  const std::string& PropertyBase::name() {
    return m_name;
  }
}
