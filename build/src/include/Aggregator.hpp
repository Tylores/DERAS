#ifndef AGGREGATOR_HPP_INCLUDED
#define AGGREGATOR_HPP_INCLUDED
#include "DistributedEnergyResource.hpp"
#include <alljoyn/BusAttachment.h>
#include <alljoyn/ProxyBusObject.h> 
#include <string>
#include <vector>

class Aggregator {
public:
    // constructor / destructor
    Aggregator (const tsu::config_map &init, ajn::BusAttachment *bus, unsigned int increment);
    virtual ~Aggregator ();

    // accessor / mutators
    void SetTargets (const std::vector <std::string> &args);
    unsigned int GetTotalExportEnergy ();
    unsigned int GetTotalExportPower ();
    unsigned int GetTotalImportEnergy ();
    unsigned int GetTotalImportPower ();

    // aggregator methods
    void AddResource (std::map <std::string, unsigned int>& init,
                      const std::string& path
    );
    void UpdateResource (std::map <std::string, unsigned int>& init,
                         const std::string& path
    );
    void RemoveResource (const std::string& path);
    void Loop (float delta_time);
    void Log ();
    void DisplayAllResources ();
    void DisplayTargetResources ();
    void DisplayTotals ();
    void ExportPower (unsigned int dispatch_power);
    void ImportPower (unsigned int dispatch_power);

private:
    // config map
    tsu::config_map config_;

    // alljoyn
    ajn::BusAttachment *bus_;

    // aggregate
    std::vector <std::shared_ptr <DistributedEnergyResource>> resources_;
    std::vector <std::shared_ptr <DistributedEnergyResource>> sub_resources_;

    // logging variables
    unsigned int last_log_;
    unsigned int log_inc_;  // will dicate the incraments between log events

    // dispatch variables
    // - these variables represent the filtered total resources based on target der
    std::vector <std::string> targets_;
    unsigned int total_export_energy_;
    unsigned int total_export_power_;
    unsigned int total_import_energy_;
    unsigned int total_import_power_;

    // control methods
    void FilterResources ();
    void SortImportEnergy ();
    void SortExportEnergy ();
};

#endif // AGGREGATOR_HPP_INCLUDED
