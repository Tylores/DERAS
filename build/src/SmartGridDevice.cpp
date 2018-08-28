#include <alljoyn/Status.h>
#include <alljoyn/BusObject.h>
#include <alljoyn/BusAttachment.h>


#include "include/SmartGridDevice.hpp"

// Constructor
// - initialize bus object interface and smart grid device properties
SmartGridDevice::SmartGridDevice (ajn::BusAttachment* bus,
                                  const char* name,
                                  const char* path) : ajn::BusObject(path),
                                                      bus_(bus),
                                                      interface_(name),
                                                      signal_(NULL) {
    const ajn::InterfaceDescription* interface = bus_->GetInterface(interface_);
    assert(interface != NULL);
    AddInterface(*interface, ANNOUNCED);

}