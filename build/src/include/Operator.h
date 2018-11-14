#ifndef OPERATOR_H_INCLUDED
#define OPERATOR_H_INCLUDED
#include <iostream>
#include <string>
#include <vector>
#include "tsu.h"

class Operator
{
public:

//Declaring Member Variables.

    // This operator is operating on an instantiation of a DER Class
    // Currently the only class I have is the electric water heater class.
    // I could add additional pointers to DER assets in the future.
    Aggregator *ptr;


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
    unsigned int adj = 29203200 - (86400 * 16) + (3600*(-6)); //Corrects to the right day, where days is days after Oct 24th.
                                                    // Corrects to right hour by multiples of 1 and -1.

    //Iterating variable. I declare this here, because I use it outside my for loop.
    int i=0;



    //This is the constructor I created. It includes the pointer to an electric water heater.
    //Theoretically this could be generalized to any DER asset, but creating a new similar constructor
    //that uses a different DER asset parameter.
    Operator(std::string input, Aggregator *DERAS) : fname(input), ptr(DERAS){  //Constructor which has filename parameter.
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
    //std::cout << "Exporting" << std::endl;
    //std::cout << amount << std::endl;
    //std::cout << "Watts" << std::endl;
    int expo = std::stoi(amount);
    ptr->ExportPower(expo);
    };

    void Import(std::string amount){
    //printf("Importing %s Watts\n", amount);
    //std::cout << "Importing" << std::endl;
    //std::cout << amount << std::endl;
    //std::cout << "Watts" << std::endl;
    int imp = std::stoi(amount);
    ptr->ImportPower(imp);

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
            std::cout << act[0] << std::endl;
            switch(act[0]){
                case 'd' :
                    //printf("Idle\n");
                    Idle();
                    break;
                case 'x' :
                    //printf("Export %d Watts\n", ext);
                    Export(schedule[i][2]);
                    break;
                case 'm' :
                    //printf("Import %d Watts\n", ext);
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

