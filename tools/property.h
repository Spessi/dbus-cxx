
#ifndef PROPERTY_H
#define PROPERTY_H

#include <string>
#include <map>

#include "arg.h"

class Interface;

/**
 * Represents a DBus property.
 */
class Property
{
public:
  void set_interface( Interface* iface );
  void set_type(std::string type);
  void set_name(std::string name);
  void set_access(std::string access);

  std::string stubname() { return name + "_adapter_stub"; }

  std::string cpp_adapter_creation();
  std::string cpp_adapter_stub_getter();
  std::string cpp_adapter_stub_setter();

private:
  Interface* interface;
  std::string type;
  std::string name;
  std::string access;
};


#endif
