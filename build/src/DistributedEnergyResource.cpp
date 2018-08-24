#include "include/DistributedEnergyResource.hpp"

DistributedEnergyResource::DistributedEnergyResource (
    std::map <std::string, std::string> init) :
    rated_export_power_(stoul(init["ExportPower"])),
    rated_export_energy_(stoul(init["ExportEnergy"])),
    export_ramp_(stoul(init["ExportRamp"])),
    rated_import_power_(stoul(init["ImportPower"])),
    rated_import_energy_(stoul(init["ImportEnergy"])),
    import_ramp_(stoul(init["ImportRamp"])),
    idle_losses_(stoul(init["IdleLosses"])),
    export_power_(0),
    export_energy_(rated_export_energy_),
    import_power_(0),
    import_energy_(0),
    export_watts_(0),
    import_watts_(0),
    delta_time_(0) {
    //ctor
}

DistributedEnergyResource::~DistributedEnergyResource () {
    //dtor
}

// Set Export Watts
// - set the export control property used in the control loop
void DistributedEnergyResource::SetExportWatts (unsigned int power) {
    import_watts_ = 0;
    import_power_ = 0;
    if (power > rated_export_power_) {
        power = rated_export_power_;
    }
    export_watts_ = power;
}  // end Set Export Watts

// Set Rated Export Power
// - set the watt value available to export to the grid
void DistributedEnergyResource::SetRatedExportPower (unsigned int power) {
    rated_export_power_ = power;
}  // end Rated Export Power

// Set Rated Export Energy
// - set the watt-hour value available to export to the grid
void DistributedEnergyResource::SetRatedExportEnergy (unsigned int energy) {
    rated_export_energy_ = energy;
}  // end Set Export Energy

// Set Export Ramp
// - set the watt per second value available to export to the grid
void DistributedEnergyResource::SetExportRamp (unsigned int ramp) {
    export_ramp_ = ramp;
}  // end Set Export Ramp

// Set Import Watts
// - set the import control property used in the control loop
void DistributedEnergyResource::SetImportWatts (unsigned int power) {
    export_watts_ = 0;
    export_power_ = 0;
    if (power > rated_import_power_) {
        power = rated_import_power_;
    }
    import_watts_ = power;
}  // end Set Import Watts

// Set Rated Import Power
// - set the watt value available to import from the grid
void DistributedEnergyResource::SetRatedImportPower (unsigned int power) {
    rated_import_power_ = power;
}  // end Set Rated Import Power

// Set Rated Import Energy
// - set the watt-hour value available to import from the grid
void DistributedEnergyResource::SetRatedImportEnergy (unsigned int energy) {
    rated_import_energy_ = energy;
}  // end Set Import Energy

// Set Import Ramp
// - set the watt per second value available to import from the grid
void DistributedEnergyResource::SetImportRamp (unsigned int ramp) {
    import_ramp_ = ramp;
}  // end Set Import Ramp

// Set Idle Losses
// - set the watt-hours per hour loss when idle
void DistributedEnergyResource::SetIdleLosses (unsigned int losses) {
    idle_losses_ = losses;
}  // end Set Idle Losses

// Get Rated Export Power
// - get the watt value available to export to the grid
unsigned int DistributedEnergyResource::GetRatedExportPower () {
    return rated_export_power_;
}  // end Get Rated Export Power

// Get Export Power
// - get the watt value available to export to the grid
unsigned int DistributedEnergyResource::GetExportPower () {
    unsigned int power = export_power_;
    return power;
}  // end Get Export Power

// Get Export Energy
// - get the watt-hour value available to export to the grid
unsigned int DistributedEnergyResource::GetExportEnergy () {
    unsigned int energy = export_energy_;
    return energy;
}  // end Get Export Energy

// Get Export Ramp
// - get the watt per second value available to export to the grid
unsigned int DistributedEnergyResource::GetExportRamp () {
    return export_ramp_;
}  // end Get Export Ramp

// Get Rated Import Power
// - get the watt value available to import from the grid
unsigned int DistributedEnergyResource::GetRatedImportPower () {
    return rated_import_power_;
}  // end Rated Import Power

// Get Import Power
// - get the watt value available to import from the grid
unsigned int DistributedEnergyResource::GetImportPower () {
    unsigned int power = import_power_;
    return power;
}  // end Get Import Power

// Get Import Energy
// - get the watt-hour value available to import from the grid
unsigned int DistributedEnergyResource::GetImportEnergy () {
    unsigned int energy = import_energy_;
    return energy;
}  // end Get Import Energy

// Get Import Ramp
// - get the watt per second value available to import from the grid
unsigned int DistributedEnergyResource::GetImportRamp () {
    return import_ramp_;
}  // end Get Import Ramp

// Get Idle Losses
// - get the watt-hours per hour loss when idle
unsigned int DistributedEnergyResource::GetIdleLosses () {
    return idle_losses_;
}  // end Get Idle Losses

// Import Power
// - called by control loop if import power is set
// - assume loss is factored into import power
void DistributedEnergyResource::ImportPower () {
    float seconds = delta_time_ / 1000;
    float watts = import_ramp_ * seconds;

    // regulate import power
    if (import_power_ + watts < import_watts_) {
        import_power_ += watts;
    } else {
        import_power_ = import_watts_;
    }

    // regulate energy
    float hours = seconds / (60*60);
    if (import_energy_ - import_power_ > 0) {
        // area under the linear function
        import_energy_ -= (import_power_*hours + watts*hours/2);
        export_energy_ = rated_export_energy_ - import_energy_;
    } else {
        import_power_ = 0;
        import_energy_ = 0;
        export_energy_ = rated_export_energy_;
    }
}  // end Import Power

// Export Power
// - called by control loop if export power is set
// - assume loss is factored into export power
void DistributedEnergyResource::ExportPower () {
    float seconds = delta_time_ / 1000;
    float watts = export_ramp_ * seconds;

    // regulate import power
    if (export_power_ + watts < export_watts_) {
        export_power_ += watts;
    } else {
        export_power_ = export_watts_;
    }

    // regulate energy
    float hours = seconds / (60*60);
    if (export_energy_ - export_power_ > 0) {
        // area under the linear function
        export_energy_ -= (export_power_*hours + watts*hours/2);
        import_energy_ = rated_import_energy_ - export_energy_;
    } else {
        export_power_ = 0;
        export_energy_ = 0;
        import_energy_ = rated_import_energy_;
    }
}  // end Export Power

// Idle Loss
// - update energy available based on energy lost
void DistributedEnergyResource::IdleLoss () {
    float seconds = delta_time_ / 1000;
    float hours = seconds / (60*60);
    float energy_loss = idle_losses_ * hours;

    if (import_energy_ + energy_loss < rated_import_energy_) {
        import_energy_ += energy_loss;
    }

    if (export_energy_ - energy_loss > 0) {
        export_energy_ -= energy_loss;
    }
}  // end Idle Loss

// Control
// - check state of import / export power properties from main loop on a timer
void DistributedEnergyResource::Loop (float delta_time) {
    delta_time_ = delta_time;

    if (import_watts_ > 0) {
        DistributedEnergyResource::ImportPower ();
    } else if (export_watts_ > 0) {
        DistributedEnergyResource::ExportPower ();
    } else {
        IdleLoss ();
    }
}  // end Control