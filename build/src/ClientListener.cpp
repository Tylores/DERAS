#include <iostream>


#include <alljoyn/ProxyBusObject.h>
#include <alljoyn/BusAttachment.h>
#include <alljoyn/Observer.h>

#include "include/ClientListener.hpp"

// (TS): this is the only way I could find to initialize a const char* array.
//       AllJoyn documentation states "NULL" for registering all properties, but
//       that didn't seem to work. 
const char* ClientListener::props_[] = {"rated_export_power",
                                        "rated_export_energy",
                                        "export_power",
                                        "export_energy",
                                        "export_ramp",
                                        "rated_import_power",
                                        "rated_import_energy",
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
    std::string path = proxy.GetPath();
    std::string service_name = proxy.GetServiceName();
    unsigned int session_id = proxy.GetSessionId();

    std::cout << "\n[LISTENER]\n";
    std::cout << "\tPath = " << path << '\n';
    std::cout << "\tService Name = " << service_name << '\n';
    std::cout << "\tSession ID = " << session_id << '\n';


    bus_->EnableConcurrentCallbacks();
    proxy.RegisterPropertiesChangedListener(
        client_interface_, props_, 11, *this, NULL
    );

    ajn::MsgArg values;
    proxy.GetAllProperties (client_interface_, values);

    std::map <std::string, unsigned int> init;
    init = ClientListener::MapProperties (values);

    vpp_->AddResource (init, path, service_name, session_id);
} // end ObjectDiscovered

// ObjectLost
// - the remote device is no longer available
// - RemoveResource from aggregator using path
void ClientListener::ObjectLost (ajn::ProxyBusObject& proxy) {
    std::string name = proxy.GetUniqueName();
    std::string path = proxy.GetPath();

    std::cout << "\n[LISTENER] : " << name << " connection lost\n";
    std::cout << "\tPath : " << path << " no longer exists\n";

    vpp_->RemoveResource (path);
} // end ObjectLost

// PropertiesChanged
// - callback to recieve property changed event from remote bus object
void ClientListener::PropertiesChanged (ajn::ProxyBusObject& obj,
                                        const char* interface_name,
                                        const ajn::MsgArg& changed,
                                        const ajn::MsgArg& invalidated,
                                        void* context) {
    std::map <std::string, unsigned int> init;
    init = ClientListener::MapProperties (changed);
    vpp_->UpdateResource (init, obj.GetPath ());
} // end PropertiesChanged

std::map <std::string, unsigned int> ClientListener::MapProperties (
    const ajn::MsgArg& properties) {
    std::map <std::string, unsigned int> init;
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
            init[name] = property;
        }
    }
    return init;
}
