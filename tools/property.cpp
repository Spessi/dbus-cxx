#include "property.h"
#include "parsed_object.h"


void Property::set_interface( Interface* iface ) {
  interface = iface;
}

void Property::set_type(std::string type) {
  if(type == "i")
    this->type = "int";
  else if(type == "b")
    this->type = "bool";
  else if(type == "s")
    this->type = "std::string";
}

void Property::set_name(std::string name) {
  this->name = name;
}

void Property::set_access(std::string access) {
  this->access = access;
}


std::string Property::cpp_adapter_creation()
{
  std::ostringstream sout;
  sout << "temp_property = this->create_property<";
  sout << this->type << ">(\"" << this->interface->name() << "\", ";
  sout << "\"" << this->name << "\", ";
  if(this->access == "read") {
    sout << "sigc::mem_fun(*this, &" << interface->node->name() << "Adapter::" << "get_" << stubname() << "), ";
    sout << "sigc::slot1<void, " << this->type << ">());";
  }
  else if(this->access == "write") {
    sout << "sigc::slot0<" << this->type << ">(), ";
    sout << "sigc::mem_fun(*this, " << interface->node->name() << "Adapter::" << "set_" << stubname() << "));";
  }
  else if(this->access == "readwrite") {
    sout << "sigc::mem_fun(*this, &" << interface->node->name() << "Adapter::" << "get_" << stubname() << "), ";
    sout << "sigc::mem_fun(*this, &" << interface->node->name() << "Adapter::" << "set_" << stubname() << "));";
  }

  return sout.str();
}

std::string Property::cpp_adapter_stub_getter()
{
  std::ostringstream sout;

  if(this->access == "read") {
    sout << this->type << " get_" << stubname() << "() { this->check_adaptee(); m_adaptee->get_" << this->name << "(); }";
  }
  else if(this->access == "readwrite") {
    sout << this->type << " get_" << stubname() << "() { this->check_adaptee(); m_adaptee->get_" << this->name << "(); }";
  }

  return sout.str();
}

std::string Property::cpp_adapter_stub_setter()
{
  std::ostringstream sout;

  if(this->access == "write") {
    sout << "void set_" << stubname() << "(" << this->type << " value) { this->check_adaptee(); return m_adaptee->set_" << this->name << "(value); }";
  }
  else if(this->access == "readwrite") {
    sout <<  "void set_" << stubname() << "(" << this->type << " value) { this->check_adaptee(); return m_adaptee->set_" << this->name << "(value); }";
  }

  return sout.str();
}
