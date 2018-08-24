#include <iostream>
#include <string>

#include <alljoyn/ProxyBusObject.h>
#include <alljoyn/BusAttachment.h>
#include <alljoyn/Observer.h>

#include "include/ClientListener.hpp"

const char* ClientListener::props[] = {"EMSName", "Time", "price"};

ClientListener::ClientListener(
    ajn::BusAttachment* bus,
    ajn::Observer* obs,
    const char* client_name) : bus_(bus),
                               obs_(obs),
                               client_interface_(client_name){
} // end ClientListener

// ObjectDiscovered
// - a remote device has advertised the interface we are looking for
void ClientListener::ObjectDiscovered (ajn::ProxyBusObject& proxy) {
    const char* name = proxy.GetUniqueName().c_str();
    std::cout << "[LISTENER] : " << name << " has been discovered\n";
    bus_->EnableConcurrentCallbacks();
    proxy.RegisterPropertiesChangedListener(
        client_interface_, props, 3, *this, NULL
    );
} // end ObjectDiscovered

// ObjectLost
// - the remote device is no longer available
void ClientListener::ObjectLost (ajn::ProxyBusObject& proxy) {
    const char* name = proxy.GetUniqueName().c_str();
    const char* path = proxy.GetPath().c_str();
    std::cout << "[LISTENER] : " << name << " connection lost\n";
    std::cout << "\tPath : " << " no longer exists\n";
} // end ObjectLost

// PropertiesChanged
// - callback to recieve property changed event from remote bus object
void ClientListener::PropertiesChanged (ajn::ProxyBusObject& obj,
                                        const char* interface_name,
                                        const ajn::MsgArg& changed,
                                        const ajn::MsgArg& invalidated,
                                        void* context) {
    size_t nelem = 0;
    ajn::MsgArg* elems = NULL;
    QStatus status = changed.Get("a{sv}", &nelem, &elems);
    if (status == ER_OK) {
        for (size_t i = 0; i < nelem; i++) {
            const char* name;
            ajn::MsgArg* val;
            status = elems[i].Get("{sv}", &name, &val);
            if (status == ER_OK) {
                if (!strcmp(name,"price")) {
                    status = val->Get("i", &price_);
                } else if (!strcmp(name,"Time")) {
                    status = val->Get("u", &time_);
                }
            } else {
                std::cout << "[LISTENER] : invalid property change!\n";
            }
        }
    }
} // end PropertiesChanged
