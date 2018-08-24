#ifndef DISTRIBUTEDENERGYRESOURCE_H
#define DISTRIBUTEDENERGYRESOURCE_H

#include <string>
#include <map>

class DistributedEnergyResource {
    public:
        // constructor / destructor
        DistributedEnergyResource (std::map <std::string, std::string> init);
        virtual ~DistributedEnergyResource ();
        void Loop (float delta_time);


    public:
        // set export methods        
        void SetExportWatts (unsigned int power);
        void SetRatedExportPower (unsigned int watts);
        void SetRatedExportEnergy (unsigned int watt_hours);
        void SetExportRamp (unsigned int watts_per_second);

        // set import methods
        void SetImportWatts (unsigned int power);
        void SetRatedImportPower (unsigned int watts);
        void SetRatedImportEnergy (unsigned int watt_hours);
        void SetImportRamp (unsigned int watts_per_second);

        // set idle methods
        void SetIdleLosses (unsigned int energy_per_hour);
        

    public:
        // get export methods
        unsigned int GetRatedExportPower ();
        unsigned int GetExportPower ();
        unsigned int GetExportEnergy ();
        unsigned int GetExportRamp ();

        // get import methods
        unsigned int GetRatedImportPower ();
        unsigned int GetImportPower ();
        unsigned int GetImportEnergy ();
        unsigned int GetImportRamp ();
        
        // get idle methods
        unsigned int GetIdleLosses ();

    private:
        // controls
        void ImportPower ();
        void ExportPower ();
        void IdleLoss ();

    private:
        // rated export properties
        unsigned int rated_export_power_;       // (W) to grid
        unsigned int rated_export_energy_;      // (Wh)
        unsigned int export_ramp_;              // (W s^-1)

        // rated import properties
        unsigned int rated_import_power_;       // (W) from grid
        unsigned int rated_import_energy_;      // (Wh)
        unsigned int import_ramp_;              // (W s^-1)
        
        // rated idle properties
        unsigned int idle_losses_;              // (Wh h^-1)

    private:
        // dynamic properties
        float export_power_;
        float export_energy_;
        float import_power_;
        float import_energy_;     

    private:
        // control properties
        unsigned int export_watts_;
        unsigned int import_watts_;
        float delta_time_;  // milliseconds
};

#endif // DISTRIBUTEDENERGYRESOURCE_H
