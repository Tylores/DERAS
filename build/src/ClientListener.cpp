#include <iostream>


#include <alljoyn/ProxyBusObject.h>
#include <alljoyn/BusAttachment.h>
#include <alljoyn/Observer.h>

#include "include/ClientListener.hpp"

// (TS): this is the only way I could find to initialize a const char* array.
//       AllJoyn documentation states "NULL" for registering all properties, but
//       that didn't seem to work. 
const char* ClientListener::props_[] = {"export_power",
                                        "export_energy",
                                        "export_ramp",
                                        "import_power",
                                        "import_energy",
                                        "import_ramp",
                                        "idle_losses"};

ClientListener::ClientListener(
    ajn::BusAttachment* bus,
    ajn::Observer* obs,
    Aggregator* vpp,
    const char* client_name) : bus_(bus),
                               obs_(obs),
                               vpp_(vpp),
                               client_interface_(client_name){
} // end ClientListener

// ObjectDiscovered
// - a remote device has advertised the interface we are looking for.
// - GetAllProperties and AddResource to Aggregator
void ClientListener::ObjectDiscovered (ajn::ProxyBusObject& proxy) {
    std::string name = proxy.GetUniqueName();
    std::cout << "[LISTENER] : " << name << " has been discovered\n";

    bus_->EnableConcurrentCallbacks();
    proxy.RegisterPropertiesChangedListener(
        client_interface_, props_, 7, *this, NULL
    );
} // end ObjectDiscovered

// ObjectLost
// - the remote device is no longer available
// - RemoveResource from aggregator using path
void ClientListener::ObjectLost (ajn::ProxyBusObject& proxy) {
    const char* name = proxy.GetUniqueName().c_str();
    const char* path = proxy.GetPath().c_str();
    std::cout << "[LISTENER] : " << name << " connection lost\n";
    std::cout << "\tPath : " << path << " no longer exists\n";
} // end ObjectLost

// PropertiesChanged
// - callback to recieve property changed event from remote bus object
void ClientListener::PropertiesChanged (ajn::ProxyBusObject& obj,
                                        const char* interface_name,
                                        const ajn::MsgArg& changed,
                                        const ajn::MsgArg& invalidated,
                                        void* context) {
    std::map <std::string, std::string> init;
    init = ClientListener::MapProperties (changed);
    for (const auto& property : init) {
        std:: cout << property.first << '\t' << property.second << std::endl;
    }
} // end PropertiesChanged

std::map <std::string, std::string> ClientListener::MapProperties (
    const ajn::MsgArg& properties) {
    std::map <std::string, std::string> init;
    size_t nelem = 0;
    ajn::MsgArg* elems = NULL;
    QStatus status = properties.Get("a{sv}", &nelem, &elems);
    if (status == ER_OK) {
        const char* name;
        unsigned int property;
        ajn::MsgArg* val;
        for (size_t i = 0; i < nelem; i++) {
            status = elems[i].Get("{sv}", &name, &val);
            val->Get("u", &property);
            init[name] = std::to_string(property);
        }
    } 
    return init;
}
