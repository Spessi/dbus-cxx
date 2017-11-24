
#include <sigc++/sigc++.h>

#include <string>
#include <map>
#include <stdexcept>

#include <dbus-cxx/forward_decls.h>
#include <dbus-cxx/objectpathhandler.h>
#include <dbus-cxx/dbus_signal.h>
#include <dbus-cxx/callmessage.h>
#include <dbus-cxx/variant.h>


#ifndef DBUSCXXPROPERTYBASE_H
#define DBUSCXXPROPERTYBASE_H

namespace DBus {

  class PropertyBase {
  public:
        PropertyBase(const std::string& name);
        typedef DBusCxxPointer<PropertyBase> pointer;

        const std::string& name();

        virtual HandlerResult handle_get( DBusCxxPointer<Connection> connection, CallMessage::const_pointer message ) = 0;
        virtual HandlerResult handle_set( DBusCxxPointer<Connection> connection, CallMessage::const_pointer message ) = 0;

  protected:
  private:
    std::string m_name;

  };
}

#endif
