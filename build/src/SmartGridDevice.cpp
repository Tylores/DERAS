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

    const ajn::BusObject::MethodEntry methods[] = {{
            interface->GetMember("ImportPower"), 
            static_cast <ajn::BusObject::MessageReceiver::MethodHandler>
            (&SmartGridDevice::ImportPowerHandler)
        }, {
            interface->GetMember("ExportPower"), 
            static_cast <ajn::BusObject::MessageReceiver::MethodHandler>
            (&SmartGridDevice::ExportPowerHandler)
        },

    };

    size_t count = sizeof (methods) / sizeof (methods[0]);
    QStatus status = AddMethodHandlers (methods, count);
    if (ER_OK != status) {
        throw status;
    }
}

// Import Power Handler
// - called by remote consumer and sends the watt value for import
void SmartGridDevice::ImportPowerHandler (
        const ajn::InterfaceDescription::Member* member,
        ajn::Message& message) {
    (void)member;
    der_->SetImportWatts (message->GetArg(0)->v_uint32);
}  // end Import Power Handler

// Export Power Handler
// - called by remote consumer and sends the watt value for export
void SmartGridDevice::ExportPowerHandler (
        const ajn::InterfaceDescription::Member* member,
        ajn::Message& message) {
    (void)member;
    der_->SetExportWatts (message->GetArg(0)->v_uint32);
}  // end Export Power Handler

// Get
// - this method will be called by remote devices looking to get this devices
// - properties
QStatus SmartGridDevice::Get (const char* interface,
                              const char* property,
                              ajn::MsgArg& value) {
    QStatus status;
    if (strcmp(interface, interface_)) {
        return ER_FAIL;
    }

    if (!strcmp(property,"import_power")) {
        status = value.Set("u", der_->GetRatedImportPower ());
        return status;
    } else if (!strcmp(property,"import_energy")) {
        status = value.Set("u", der_->GetImportEnergy ());
        return status;
    } else if (!strcmp(property,"import_ramp")) {
        status = value.Set("u", der_->GetImportRamp ());
        return status;
    }else if (!strcmp(property,"export_power")) {
        status = value.Set("u", der_->GetRatedExportPower ());
        return status;
    } else if (!strcmp(property,"export_energy")) {
        status = value.Set("u", der_->GetExportEnergy ());
        return status;
    } else if (!strcmp(property,"export_ramp")) {
        status = value.Set("u", der_->GetExportRamp ());
        return status;
    } else if (!strcmp(property,"idle_losses")) {
        status = value.Set("u", der_->GetIdleLosses ());
        return status;
    } else {
        return ER_FAIL;
    }
} // end Get
