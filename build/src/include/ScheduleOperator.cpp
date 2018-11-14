#include <iostream>
#include <string>
#include <vector>
//#include "..\schedule_operator.h"
#include "tsu.h"
//#include "tsu.h"
//#include "electric_water_heater.h"
#include <string>
#include "Aggregator.hpp"
class ScheduleOperator {
public:

ScheduleOperator (std::string input, Aggregator *VPP) : file_name_(input), ptr_(VPP), index_(0){
	schedule_ = tsu::FileToMatrix(file_name_, ',', 3);

}

~ScheduleOperator () {};


	Aggregator *ptr_;
	tsu::string_matrix schedule_;

//	void Idle ();
//	void Export (std::string amount);
//	void Import (std::string amount);
//	void Loop ();
void Idle () {
	ptr_->ImportPower(0);
};

void Export (std::string amount){
    int watts = std::stoi(amount);
	ptr_->ExportPower(watts);
};

void Import (std::string amount){
	int watts = std::stoi(amount);
	ptr_->ImportPower(watts);
};

void Loop () {

    unsigned int time;
    time = tsu::GetUTC();  //get the UTC time to compare with the schedule time.
    unsigned int days = 21;
    unsigned int adj = 29203200 - (86400 * days) + 3600*10; //Corrects to the right day, where days is days after Oct 24th.

    int cnt=0;
    int check=0;
    char act[2];
    //unsigned int ext=0;


    time = tsu::GetUTC();
    time = time + adj;  //Only for testing with a fake schedule. This will
                            //be removed when working with real time schedules.
    //std::cout << time << std::endl;

    for (int i=0 ; i < schedule_.size() ; i++){
                cnt = i;
                check = stoul(schedule_[i][0]);
            //std::cout << check << std::endl;
            //std::cout << time << std::endl;

                if (check == time){
                    std::cout << "check = time, success!!" << std::endl;
                    act[0]=schedule_[i][1][1];


                    break;
                    }

                if (i == schedule_.size()){

                    std::cout << "Error. Time not found in Schedule." << std::endl;
                    break;

                }

            }

            //Parses the command by picking out the second letter.
            // d for idle, x for export, m for import
            std::cout << act[0] << std::endl;
            switch(act[0]){
                case 'd' :
                    printf("Idle\n");
                    Idle();
                    break;
                case 'x' :
                    printf("Export %d Watts\n", stoul(schedule_[cnt][2]));
                    Export(schedule_[cnt][2]);
                    break;
                case 'm' :
                    printf("Import %d Watts\n", stoul(schedule_[cnt][2]));
                    Import(schedule_[cnt][2]);
                    break;
                default :
                    printf("Error. No grid command identified.");
            }



};

private:
	std::string file_name_;
	unsigned int index_;
};


/*
class Operator
{
public:

//Declaring Member Variables.

    // This operator is operating on an instantiation of a DER Class
    // Currently the only class I have is the electric water heater class.
    // I could add additional pointers to DER assets in the future.
    ElectricWaterHeater *ptr;


    std::string fname;  // Name of the schduling csv file.
    //I envision this file as having the following columns.
    // 0) Integer Time in UTC.
    // 1) Action as a string, with three possibilities, Import, Export, and Idle.
    // 2) Integer Value for power. Could be a phyical unit, like kWm W, mW, ergs, or
        //could be a dimensionless fraction of max output, like percent or permyriad.


    tsu::string_matrix schedule; //This is one of Tylor's utilities. It declares a
                                 //string matrix, which is a vector of vectors.
                                 //the file name will be used to access the file that
                                 //creates this vector of vectors.

    // Time in UTC pulled from the UTC function.
    unsigned int time;

    // Testing variables. These are only for using my test csv schedule with fixed dates.
    //Using the test csv requires daily updates.
    unsigned int days = 0;  // Days after October 24th.
    unsigned int adj = 29203200 - (86400 * days); //Corrects to the right day, where days is days after Oct 24th.


    //Iterating variable. I declare this here, because I use it outside my for loop.
    int i=0;



    //This is the constructor I created. It includes the pointer to an electric water heater.
    //Theoretically this could be generalized to any DER asset, but creating a new similar constructor
    //that uses a different DER asset parameter.
    Operator(std::string input, ElectricWaterHeater *ewh) : fname(input), ptr(ewh){  //Constructor which has filename parameter.
                                                 //Automatically converts file to Tylor's
                                                 // Matrix data structure, a vector of vectors.

            std::cout << fname << std::endl;

    tsu::string_matrix data = tsu::FileToMatrix(fname, ',' , 3);
                std::cout << fname << std::endl;

    schedule = data;
    }


    void Idle(){
    printf("Idling.\n");
    //ptr->Idle(amount);

    };

    void Export(std::string amount){
    //printf("Exporting %d Watts\n", amount);
    //ptr->Export(amount);
    printf("Error. Electric water heater cannot export. Report to HR to be fired.\n");
    };

    void Import(std::string amount){
    //printf("Importing %s Watts\n", amount);
    cout << "Importing " << amount << " watts" << endl;
    int imp = std::stoi(amount);
    ptr->SetImportWatts(imp);

    };


    void Loop(){
            int cnt=0;
            int check=0;
            char act[2];
            int dummy=0;
            unsigned int ext=1;
        while(1){
            cnt = 0;
            check = 0;
            act[0]='A';
            act[1]='\0';

            time = tsu::GetUTC();

            time = time + adj;  //Only for testing with a fake schedule. This will
                            //be removed when working with real time schedules.
            std::cout << time << std::endl;

            for (i=0 ; i < schedule.size() ; i++){

                check = stoul(schedule[i][0]);
                cnt = i;
            //std::cout << cnt << std::endl;
            //std::cout << time << std::endl;

                if (check == time){
                    //std::cout << "cnt = time, success!!" << std::endl;
                    //std::cout << schedule[i][0] << std::endl;
                    //std::cout << schedule[i][1] << std::endl;
                    //std::cout << schedule[i][2] << std::endl;
                    //std::cout << schedule[i][1][2] << std::endl;
                    act[0]=schedule[i][1][1];


                    break;
                    }

                if (i == schedule.size()){

                    std::cout << "Error. Time not found in Schedule." << std::endl;
                    break;

                }

            }
            //std::cout << cnt << std::endl;

            //Parses the command by picking out the second letter.
            // d for idle, x for export, m for import
            //schedule[cnt][1].copy(act,1,1);
            //std::cout << stoul(schedule[cnt][2]) << std::endl;
            //ext = stoul(schedule[cnt][2]);
            //std::cout << ext << std::endl;

            //printf("%s\n",act);
            //printf("%c\n",act[0]);



            std::cout << act[0] << endl;
            switch(act[0]){
                case 'd' :
                    printf("Idle\n");
                    Idle();
                    break;
                case 'x' :
                    printf("Export %d Watts\n", ext);
                    Export(schedule[i][2]);
                    break;
                case 'm' :
                    printf("Import %d Watts\n", ext);
                    Import(schedule[i][2]);
                    break;
                default :
                    printf("Error. No grid command identified.");
            }
            //std::cout << schedule[cnt][2] << std::endl;
            //dummy++;
            //if (dummy > 3000){
            //break;
            //}
        }
    };


private:

  };


#endif // OPERATOR_H_INCLUDED
*/
