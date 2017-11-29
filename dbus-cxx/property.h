
#include <sigc++/sigc++.h>

#include <iostream>
#include <string>
#include <map>
#include <stdexcept>

#include <dbus-cxx/forward_decls.h>
#include <dbus-cxx/objectpathhandler.h>
#include <dbus-cxx/dbus_signal.h>
#include <dbus-cxx/variant.h>

#include "connection.h"
#include "callmessage.h"
#include "returnmessage.h"
#include "propertybase.h"

#ifndef DBUSCXXPROPERTY_H
#define DBUSCXXPROPERTY_H

namespace DBus {

  template <class T>
  class Property : public PropertyBase {
  public:
        typedef DBusCxxPointer<Property> pointer;

        Property(const std::string& name) : PropertyBase(name) {

        }


        static pointer create(const std::string& name)
        { return pointer( new Property(name) ); }

        void set_getter_slot(sigc::slot0<T> slot) {
          m_slot_getter = slot;
        }
        void set_setter_slot(sigc::slot1<void, T> slot) {
          m_slot_setter = slot;
        }

        virtual HandlerResult handle_get( DBusCxxPointer<Connection> connection, CallMessage::const_pointer message ) {
          if(m_slot_getter == sigc::slot0<T>()) {
            return NOT_HANDLED;
          }

          try {
            Variant<T> value = m_slot_getter();
            ReturnMessage::pointer retmsg = message->create_reply();
            if ( not retmsg ) return NOT_HANDLED;

            *retmsg << value;

            connection->send(retmsg);
          }
          catch ( const std::exception &e ) {
            ErrorMessage::pointer errmsg = ErrorMessage::create( message, DBUS_ERROR_FAILED, e.what() );

            if ( not errmsg ) return NOT_HANDLED;

            connection->send(errmsg);
          }
          catch ( ... ) {
            std::ostringstream stream;
            stream << "DBus-cxx " << DBUS_CXX_PACKAGE_MAJOR_VERSION << "." <<
                 DBUS_CXX_PACKAGE_MINOR_VERSION << "." << DBUS_CXX_PACKAGE_MICRO_VERSION << " unknown error.";
            ErrorMessage::pointer errmsg = ErrorMessage::create( message, DBUS_ERROR_FAILED, stream.str() );

            if ( not errmsg ) return NOT_HANDLED;

            connection->send(errmsg);
          }

          return HANDLED;
        }

        virtual HandlerResult handle_set( DBusCxxPointer<Connection> connection, CallMessage::const_pointer message ) {
          if(m_slot_setter == sigc::slot1<void, T>()) {
            return NOT_HANDLED;
          }

          std::string interface_name;
          std::string property_name;
          Variant<T> value;
          try {
            Message::iterator i = message->begin();
            i >> interface_name >> property_name >> value;
          }
          catch ( ErrorInvalidTypecast& e ) {
              return NOT_HANDLED;
          }

          try {
            m_slot_setter(value.data);
            ReturnMessage::pointer retmsg = message->create_reply();

            if ( not retmsg ) return NOT_HANDLED;


            connection->send(retmsg);
          }
          catch ( const std::exception &e ) {
            ErrorMessage::pointer errmsg = ErrorMessage::create( message, DBUS_ERROR_FAILED, e.what() );

            if ( not errmsg ) return NOT_HANDLED;

            connection->send(errmsg);
          }
          catch ( ... ) {
            std::ostringstream stream;
            stream << "DBus-cxx " << DBUS_CXX_PACKAGE_MAJOR_VERSION << "." <<
                 DBUS_CXX_PACKAGE_MINOR_VERSION << "." << DBUS_CXX_PACKAGE_MICRO_VERSION << " unknown error.";
            ErrorMessage::pointer errmsg = ErrorMessage::create( message, DBUS_ERROR_FAILED, stream.str() );
            
            if ( not errmsg ) return NOT_HANDLED;

            connection->send(errmsg);
          }

          return HANDLED;
        }

  protected:
  private:
    sigc::slot0<T> m_slot_getter;
    sigc::slot1<void, T> m_slot_setter;


  };
}

#endif
