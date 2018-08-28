#include <iostream>
#include <algorithm>
#include <memory>
#include <vector>
#include <string>
#include <map>

#include "include/Aggregator.hpp"


Aggregator::Aggregator () {

}
Aggregator::~Aggregator () {

}

void Aggregator::AddResource (std::map <std::string, unsigned int>& init,
							  const std::string& path) {
	std::shared_ptr <DistributedEnergyResource> 
		der (new DistributedEnergyResource (init, path));
	der->Print ();
	resources_.push_back (std::move (der));
}

void Aggregator::UpdateResource (std::map <std::string, unsigned int>& init,
								 const std::string& path) {
	for (const auto& resource : resources_) {
		if (resource->GetPath() == path) {
			resource->SetRatedExportEnergy (init["export_energy"]);
			resource->SetRatedExportPower (init["export_power"]);
			resource->SetExportRamp (init["export_ramp"]);
			resource->SetRatedImportEnergy (init["import_energy"]);
			resource->SetRatedImportPower (init["import_power"]);
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
}

void Aggregator::DisplayAll () {
    std::cout << "\nAll Resources:" << std::endl;
    SortImportEnergy();
    for (const auto &resource : resources_) {
        resource->Print ();
    }
}

void Aggregator::DisplayFiltered () {
    std::cout << "\nFiltered Resources:" << std::endl;
    SortImportEnergy();
    for (const auto &resource : sub_resources_) {
        resource->Print ();
    }
}

void Aggregator::FilterResources (std::vector <std::string> args) {
    sub_resources_.clear();
    for (const auto &resource : resources_) {
        bool found;
        for (unsigned int i = 0; i < args.size(); i++) {
            if (resource->GetPath().find(args[i])!= std::string::npos) {
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
    std::sort(resources_.begin(),resources_.end(),
        [](const std::shared_ptr <DistributedEnergyResource> lhs,
           const std::shared_ptr <DistributedEnergyResource> rhs) {
            return lhs->GetImportEnergy() > rhs->GetImportEnergy();
        }
    );
}

void Aggregator::SortExportEnergy () {
    std::sort(resources_.begin(),resources_.end(),
        [](const std::shared_ptr <DistributedEnergyResource> lhs,
           const std::shared_ptr <DistributedEnergyResource> rhs) {
            return lhs->GetExportEnergy() > rhs->GetExportEnergy();
        }
    );
}