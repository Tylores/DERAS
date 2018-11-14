#include <iostream>
#include <algorithm>
#include <memory>
#include <vector>
#include <string>
#include <map>

#include "include/Aggregator.hpp"
#include "include/logger.h"

#include <alljoyn/ProxyBusObject.h>
#include <alljoyn/Status.h>

Aggregator::Aggregator (tsu::config_map &init) :
	config_(init),
	last_log_(0),
	log_inc_(stoul(init["Logger"]["increment"])),
	total_export_energy_(0),
	total_export_power_(0),
	total_import_energy_(0),
	total_import_power_(0) {
}  // end constructor

Aggregator::~Aggregator () {
}  // end destructor

void Aggregator::SetTargets (const std::vector <std::string> &targets) {
	targets_ = targets;
	Aggregator::FilterResources ();
}  // end Set Targets

unsigned int Aggregator::GetTotalExportEnergy () {
    return total_export_energy_;
}  // end Get Total Export Energy

unsigned int Aggregator::GetTotalExportPower () {
    return total_export_power_;
}  // end Get Total Export Power

unsigned int Aggregator::GetTotalImportEnergy () {
    return total_import_energy_;

}  // end Get Total Import Energy
unsigned int Aggregator::GetTotalImportPower () {
    return total_import_power_;
}  // end Get Total Import Power

void Aggregator::AddResource (
	std::map <std::string, unsigned int>& init,
	ajn::ProxyBusObject &proxy) {
	std::string interface = config_["AllJoyn"]["client_interface"];
	std::shared_ptr <DistributedEnergyResource> 
		der (new DistributedEnergyResource (init, proxy, interface));
	resources_.push_back (std::move (der));
	Aggregator::FilterResources ();
}

void Aggregator::UpdateResource (std::map <std::string, unsigned int>& init,
				 				 const std::string& path) {
	for (auto& resource : resources_) {
		if (resource->GetPath() == path) {
			resource->SetRatedExportEnergy (init["rated_export_energy"]);
			resource->SetRatedExportPower (init["rated_export_power"]);
			resource->SetExportEnergy (init["export_energy"]);
			resource->SetExportPower (init["export_power"]);
			resource->SetExportRamp (init["export_ramp"]);
			resource->SetRatedImportEnergy (init["rated_import_energy"]);
			resource->SetRatedImportPower (init["rated_import_power"]);
			resource->SetImportEnergy (init["import_energy"]);
			resource->SetImportPower (init["import_power"]);
			resource->SetImportRamp (init["import_ramp"]);
			resource->SetIdleLosses (init["idle_losses"]);
		} else {
			std::cout << "Resource not found!" << std::endl;
		}
	}
}

void Aggregator::RemoveResource (const std::string& path) {
    // (TS): unlike a normal for loop, this loops iterator must be
    //       incremented only if the element is not deleted or you get
    //       memory leaks.
    for (auto it = resources_.begin(); it != resources_.end();) {
        if ((*it)->GetPath ().find(path) != std::string::npos) {
            it = resources_.erase (it);
        } else {
            it++;
        }
    }
    Aggregator::FilterResources ();
}

void Aggregator::Loop (float delta_time) {
    // update all resources
    for (auto &resource : resources_) {
		resource->Loop (delta_time);
    }

    // log resources based on elapsed time
    unsigned int seconds = tsu::GetSeconds ();
    if (seconds != last_log_ && seconds % log_inc_ == 0) {
		Aggregator::Log ();
		last_log_ = seconds;
    }
}

void Aggregator::Log () {
    for (const auto &resource : resources_) {
	Logger("DATA")
		<< resource->GetPath () << '\t'
		<< resource->GetExportRamp () << '\t'
		<< resource->GetRatedExportPower () << '\t'
		<< resource->GetRatedExportEnergy () << '\t'
		<< resource->GetExportPower () << '\t'
		<< resource->GetExportEnergy () << '\t'
		<< resource->GetImportRamp () << '\t'
		<< resource->GetRatedImportPower () << '\t'
		<< resource->GetRatedImportEnergy () << '\t'
		<< resource->GetImportPower () << '\t'
		<< resource->GetImportEnergy ();
    }
}

void Aggregator::DisplayAllResources () {
    std::cout << "\nAll Resources:" << std::endl;
    SortImportEnergy();
    for (const auto &resource : resources_) {
        resource->Print ();
    }
}

void Aggregator::DisplayTargetResources () {
    std::cout << "\nTarget Resources:" << std::endl;
    SortImportEnergy();
    for (const auto &resource : sub_resources_) {
        resource->Print ();
    }
}

void Aggregator::UpdateTotals () {
	total_export_energy_ = 0;
	total_export_power_ = 0;
	total_import_energy_ = 0;
	total_import_power_ = 0;
    for (const auto &resource : sub_resources_) {
		total_export_energy_ += resource->GetExportEnergy ();
		total_export_power_ += resource->GetExportPower ();
		total_import_energy_ += resource->GetImportEnergy ();
		total_import_power_ += resource->GetImportPower ();    	
    }
}

void Aggregator::DisplayTotals () {
	Aggregator::UpdateTotals ();
    std::cout << "\nAggregated Properties:!"
	<< "\n\tTotal Export Energy = " << total_export_energy_
	<< "\n\tTotal Export Power = " << total_export_power_
	<< "\n\tTotal Import Energy = " << total_import_energy_
	<< "\n\tTotal Import Power = " << total_import_power_ << std::endl;
}

void Aggregator::FilterResources () {
    sub_resources_.clear();
    for (const auto &resource : resources_) {
        bool found;
        for (unsigned int i = 0; i < targets_.size(); i++) {
            if (resource->GetPath().find(targets_[i])!= std::string::npos) {
                found = true;
            } else {
                found = false;
                break;
            }
        }
        if (found) {
            sub_resources_.push_back(resource);
        }
    }
}

void Aggregator::SortImportEnergy () {
    std::sort(sub_resources_.begin(),sub_resources_.end(),
        [](const std::shared_ptr <DistributedEnergyResource> lhs,
           const std::shared_ptr <DistributedEnergyResource> rhs) {
            return lhs->GetImportEnergy() > rhs->GetImportEnergy();
        }
    );
}

void Aggregator::SortExportEnergy () {
    std::sort(sub_resources_.begin(),sub_resources_.end(),
        [](const std::shared_ptr <DistributedEnergyResource> lhs,
           const std::shared_ptr <DistributedEnergyResource> rhs) {
            return lhs->GetExportEnergy() > rhs->GetExportEnergy();
        }
    );
}

// Export Power
// - loop through target resources and send export signal based on greatest
// - export energy available. The signal sets both the "digital twin" and the 
// - remote devices control watts;
// - (TS): note we could also check to see if it is already dispatched to reduce 
// - 	   data transfer.
// - (TS): depending on the service we may need to check to see if the resource
// - 	   can support the dispatch power for the full period.
void Aggregator::ExportPower (unsigned int dispatch_power) {
    Aggregator::SortExportEnergy ();

    unsigned int power = 0;
    for (auto &resource : sub_resources_) {
		if (dispatch_power > 0) {
		    // Digital Twin
		    power = resource->GetRatedExportPower ();
		    resource->SetExportWatts (power);

		    // AllJoyn Method Call
		    resource->RemoteExportPower (power);

		    // subtract resources power from dispatch power
		    if (dispatch_power > power) {
		    	dispatch_power -= power;
		    } else {
		   		dispatch_power = 0;
		    }
		} else {
		    break;
		}
    }
}  // end Export Power

// Import Power
void Aggregator::ImportPower (unsigned int dispatch_power) {
    Aggregator::SortImportEnergy ();

    unsigned int power = 0;
    for (auto &resource : sub_resources_) {
		if (dispatch_power > 0) {
		    // Digital Twin
		    power = resource->GetRatedImportPower ();
		    resource->SetImportWatts (power);

		    // AllJoyn Method Call
		    resource->RemoteImportPower (power);

		    // subtract resources power from dispatch power
		    if (dispatch_power > power) {
		    	dispatch_power -= power;
		    } else {
		   		dispatch_power = 0;
		    }
		} else {
		    break;
		}
    }
}  // end Import Power
