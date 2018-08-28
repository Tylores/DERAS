#ifndef AGGREGATOR_HPP_INCLUDED
#define AGGREGATOR_HPP_INCLUDED
#include "DistributedEnergyResource.hpp"

class Aggregator {
public:
    // constructor / destructor
    Aggregator ();
    virtual ~Aggregator ();

    void AddResource (std::map <std::string, std::string>& init);
    void RemoveResource (const std::string& path);
    void DisplayAll ();
    void DisplayFiltered ();
    void FilterResources (std::vector <std::string> args);
    void SortImportEnergy ();
    void SortExportEnergy ();

private:
    // aggregate
    std::vector <std::shared_ptr <DistributedEnergyResource>> resources_;
    std::vector <std::shared_ptr <DistributedEnergyResource>> sub_resources_;
};

#endif // AGGREGATOR_HPP_INCLUDED