#ifndef OPERATOR_H_INCLUDED
#define OPERATOR_H_INCLUDED
#include <iostream>
#include <string>
#include <vector>
//#include "UtilityK.hpp"
#include "tsu.h"

class Operator
{
public:
//    File *csvp;
    std::string fname;  // Name of the schduling csv file.
    //I envision this file as having the following.
    // 1) Integer Time in UTC.
    // 2) Action as a string, with three possibilities, Import, Export, and Idle.
    // 3) Integer Value for power. Could be a phyical unit, like kWm W, mW, ergs, or
        //could be a dimensionless fraction of max output, like percent or permyriad.
    tsu::string_matrix schedule;
    //std::string time;
    unsigned int time; // Time in UTC pulled from the UTC function.

    unsigned int adj = 31017600; //Corrects to the right day.
                                 //Using the test csv, this requires daily updates.


    Operator(std::string input) : fname(input){  //Constructor which has filename parameter.
                                                 //Automatically converts file to Tylor's
                                                 // Matrix data structure, a vector of vectors.
    tsu::string_matrix data = tsu::FileToMatrix(fname, ',' , 3);
/**
        for (const auto &row: shedule){

            for (const auto &col: row){
                std::cout << col << '\t';
            }
            //std::cout << std::endl;
            std::cout << '\n';
        }
**/
    schedule = data;
    }


    void Idle(){
    printf("Idling\n");


    };

    void Export(){
    printf("Exporting\n");


    };

    void Import(){
    printf("Importing\n");


    };


    void Loop(){
            int cnt=0;
            int check=0;
            char act[2];
            int dummy=0;
            unsigned int ext=1;
        //while(1){
            cnt = 0;
            check = 0;
            act[0]='A';
            act[1]='\0';

            time = tsu::GetUTC();

            time = time + adj;  //Only for testing with a fake schedule. This will
                            //be removed when working with real time schedules.

            for (int i=0 ; i < schedule.size() ; i++){

                check = stoul(schedule[i][0]);
                cnt = i;
            //std::cout << cnt << std::endl;
            //std::cout << time << std::endl;

                if (check == time){
                    //std::cout << "cnt = time, success!!" << std::endl;

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




            switch(act[0]){
                case 'd' :
                    printf("Idle\n");
                    Idle();
                    break;
                case 'x' :
                    printf("Export %d Watts\n", ext);
                    Export();
                    break;
                case 'm' :
                    printf("Import %d Watts\n", ext);
                    Import();
                    break;
                default :
                    printf("Error. No grid command identified.");
            }
            //std::cout << schedule[cnt][2] << std::endl;
            dummy++;
            if (dummy > 3000){
            //break;
            }
        //}
    };


private:
  // member properties
//    std::vector<int> timeUTC;
//    std::vector<char> action;
//    std::vector<>
  };


#endif // OPERATOR_H_INCLUDED
    /* //Constructor which determines data length.
    std::cout << fname << std::endl;
    const char* C = fname.c_str();              //Converts to a const char string for parsing.
        //printf("\n..\n..\n..\n %s \n %d \n..\n..\n..\n", str, &str);
        FILE *csvp;                             //Creates a pointer to the file.
        csvp = fopen(C, "r");                   //Uses that pointer open my csv file.
            if (!csvp){                         //In case of error in loading, print the error.
                printf("Error loading taction.csv.\n");
            }

        int counter= 0;                 //Counts the dataLength.
        int x = 3;
        for(;;){                           //Run infinitely until EOF, then break.
            fscanf(csvp, "%*[^\n]\n", NULL);     // Skips to the next line.
            x = feof(csvp);
            //printf("\n\%d",x);
            counter++;                   // Iterate the dataLength variable.
            if (x){
                printf("\n End of file reached.");
                printf("\n\%d",counter);
                break;                      // Break loop when EOF.
                }
            }
        fclose(csvp);                       // Close the csv file.

        dataLength = counter;    //This looks silly. I tried using
                                 //dataLength instead of counter, and it
                                 //didn't work. That's why I used this.
    };
    */
    //static const int DATALENGTH = dataLength;
    //int timeUTC[DATALENGTH];
    //char action[DATALENGTH];
    //float amount[DATALENGTH];


    //Operator(){};
    //Operator(char input1[]) : fname(input1){};
  // member functions
  // functions, include Import, Export, and Idle.
/*
  int GetDataLength(std::string fname)
  {
      std::cout << fname << std::endl;
        const char* C = fname.c_str();
        //printf("\n..\n..\n..\n %s \n %d \n..\n..\n..\n", str, &str);
        FILE *csvp;                                 //Creates a pointer to the file.
        csvp = fopen(C, "r");    //Uses that pointer open my csv file.
            if (!csvp){                             //In case of error in loading, stop the program.
                printf("Error loading taction.csv.\n");
                return 0;
            }

        int counter= 0;
        int x = 3;
        for(;;){
            fscanf(csvp, "%*[^\n]\n", NULL);     // Skips to the next line.
            x = feof(csvp);
            //printf("\n\%d",x);
            counter++;
            if (x){
                printf("\n End of file reached.");
                printf("\n\%d",counter);
                break;
                }
            }
        fclose(csvp);
        return counter;
    }

    std::vector<int> GetTime(std::string fname, int counter){
        int time[counter];
        const char* C = fname.c_str();
        FILE *csvp;             //Creates a pointer to the file.
        //std::vector dummy[0]=0;
        //std::vector dummy[1]=1;

        csvp = fopen(C, "r");    //Uses that pointer open my csv file.
            if (!csvp){                             //In case of error in loading, stop the program.
                printf("Error loading taction.csv.\n");
                std::vector<int> dummmy;
                return  dummy;
                }

        fclose(csvp);
        return 0;
    }
*/
/*
        int time[counter];
        char action[counter];

printf("\n Test\n.");
        //FILE *csvp2;
        csvp = fopen(fname, "r");    //Uses that pointer open my csv file.
            if (!csvp){                             //In case of error in loading, stop the program.
                printf("Error loading taction.csv.\n");
                return 0;
                }





    //Reading the data lines from CSV.
        for(int i = 0; i < counter ; i++){
            fscanf(csvp, "%d", &time[i]);       //Saves the time.
            fgetc(csvp);                        //Eats the comma.
            fgetc(csvp);                        //Eats the first letter.
            action[i] = fgetc(csvp);            //Saves the second letter.

            fscanf(csvp, "%*[^\n]\n", NULL);         // Skips to the next line.

            if (feof(csvp)){
                printf("\n End of file reached.");
                }
            }

        printf("\n %d    %c \n %d     %c\n %d    %c \n %d    %c \n %d    %c \n %d    %c \n %d   %c \n %d   %c ", time[10000], action[10000], time[20000],action[20000], time[30000],action[30000], time[40000],action[40000], time[50000],action[50000],  time[60000],action[60000],  time[70000],action[70000], time[80000],action[80000]);
     fclose(csvp);

//Close the .csv file.
        //fclose(csvp);

    //Input Portion Complete
        printf("\n Data finished parsing.\n");
*/
