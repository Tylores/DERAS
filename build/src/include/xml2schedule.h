#ifndef XML2SCHEDULE_H_INCLUDED
#define XML2SCHEDULE_H_INCLUDED


#include <iostream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/foreach.hpp>
#include <ctime>
#include <vector>
#include <string>
#include <stdio.h>
//#include <include/SetPoint.h>



namespace pt = boost::property_tree;

std::vector<SetPoint> xml2schedule(std::string xmlFile)
  {
/* Declaring some temporary data containers */

    /* Declaring an Initial Property Tree from a CAISO XML file*/
    pt::ptree propTree;  //declare a property tree
    pt::read_xml(xmlFile, propTree);  //reads an XML file into the prop tree.

    /* For storing the XML values for the uniquely identifying variables*/
    std::string scheduleType; // These identify a specific data set.
    std::string mridInterTie;
    std::string mridRegisteredInterTie;

    /* Fore recording the schedule data in to one data structure. */
    std::vector<SetPoint> schedule;  // Sets up a vector of my SetPoint class.
    SetPoint interval;  // Instantiates a SetPoint class.
    // This creates a vector of SetPoint classes, to store parsed and formatted
    // xml data.

    /* For storing the parsed time data */
    std::string timeDateStr; //For storing the initial start/end time&date Strs.
    char year[5] = {'\0','\0','\0','\0','\0'}; //For storing the year(0000-9999)
    char month[3] = {'\0','\0','\0'}; //For storing the month (1-12).
    char day[3] = {'\0','\0','\0'}; //For storing the day of the month (1-31).
    char hour[3] = {'\0','\0','\0'}; //For storing hour of the day (0-23).
    char minute[3] = {'\0','\0','\0'}; //For the minute of the hour (0-59).
    char second[3] = {'\0','\0','\0'}; //For the second of the minute (0-59).

    /* For formatting the parsed time data */
    int DLT = 1; // Is daylight savings time? 1=yes, 0=no.
    int PacTime = 0; // For correcting Pacific Time to get the right UTC time.
    time_t now = time(0);  // Gets the UTC time now.
    time_t UTC; //For storing my actual UTC times.
    tm *Time = localtime(&now);  //I initialize it to the local time to avoid a
                                 //segfault. Don't know why this occurs.




/* Information for calling a specific data set from. */
    //For some reason these three properties were needed to guarantee a distinct
    //data set. Some data sets can be identified with less information. All data
    //sets given in the test document required only two of these items.
    std::string mridInterTieGiven = "TIE";  // Specifies set 6 or 7.
    std::string mridRegisteredInterTieGiven = "REGISTERED_INTERTIE_ITIE_ETIE";
                                            //Specifies set 6 or 7.
    std::string scheduleTypeGiven = "FIRM"; // Specifies set 1, 2, 4, or 6.
    //End result of these settings is that set 6 will be specified.






/* This first loop counts through the data until I reach the data set I want. */
    int a = 0;  // Iteration counter.
    int b = 0;  // Iteration of data that I want to store (based on InterTie,
                //RegisteredInterTie, and scheduleType).

    BOOST_FOREACH( boost::property_tree::ptree::value_type const& node,
    propTree.get_child( "InterchangeScheduleData.MessagePayload") )
    // Similar to a loop - it goes through each set of nodes of prop tree data.
    {

        boost::property_tree::ptree subtree = node.second;
        // Creates a subtree, for accesssing lower ranking data.
        a++; // Counts my iteration of InterchangeSchedules.
             // The test file has 7 of these InterchangeSchedules.

        if (node.first == "InterchangeSchedule"){
            // Saves the uniquely identifying data for the Interchange Schedules
            mridInterTie = subtree.get<std::string>("InterTie.mrid");
            scheduleType = subtree.get<std::string>("scheduleType");
            mridRegisteredInterTie =
            subtree.get<std::string>("RegisteredInterTie.mrid");

            // Looks for a match with the desired unique data set.
            if ( (mridInterTie == mridInterTieGiven) &&
            (mridRegisteredInterTie == mridRegisteredInterTieGiven) &&
            (scheduleType == scheduleTypeGiven)){
                b = a; // If they match, set b = a.
            } // Closes the branch statement for matching the schedules.
        } // Closes the branch statement for indentifying the schedules.
    } // Closes the FOREACH.


/* 2nd loop counts to the right data set and records data into schedule vector*/
    a = 0;  // Resetting iteration variable. Having set b==a at the right point,
            // I now use a to count up to b.

    BOOST_FOREACH( boost::property_tree::ptree::value_type const& node,
    propTree.get_child( "InterchangeScheduleData.MessagePayload") )
    {
    // Repeat of the first FOREACH, except now I know the right data set.

        boost::property_tree::ptree subtree = node.second;
        a++; // I count through again, looking for the place where a == b.

        if (b == a){ //I only save data when the sets match: a == b.
            BOOST_FOREACH( boost::property_tree::ptree::value_type const& node,
            propTree.get_child(
            "InterchangeScheduleData.MessagePayload.InterchangeSchedule") )
                // Sorting through a deeper section of the property tree.
                {
                    boost::property_tree::ptree subsubtree = node.second;
                    // This is a deeper subtree.

                    if(node.first == "IrregularDateTimePoint"){
                    // My data is held under identical headings of this type.

                        /* Setting Value1 */

                        interval.value1 = std::stod(subsubtree.get<std::string>
                        ("value1")); // Saving as a double to interval, an
                                     // instantiation of my SetPoint class.

                        /* Setting the StartDateTime */

                            /* Pulling the string from the XML file */
                            timeDateStr =
                            subsubtree.get<std::string>("startDateTime");
                            /*Parsing string to get actual year,month,date,etc*/
                            year[0] = timeDateStr[0];
                            year[1] = timeDateStr[1];
                            year[2] = timeDateStr[2];
                            year[3] = timeDateStr[3];
                            month[0] = timeDateStr[5];
                            month[1] = timeDateStr[6];
                            day[0] = timeDateStr[8];
                            day[1] = timeDateStr[9];
                            hour[0] = timeDateStr[11];
                            hour[1] = timeDateStr[12];
                            minute[0] = timeDateStr[14];
                            minute[1] = timeDateStr[15];
                            second[0] = timeDateStr[17];
                            second[1] = timeDateStr[18];
                            /*Changing strings to times. Storing in tm struct.*/
                            Time->tm_year = atoi(year) - 1900;
                            //Stored as years after 1900
                            Time->tm_mon = atoi(month) - 1;
                            //Stored as month from 0 to 11, with 0=Jan
                            Time->tm_mday = atoi(day);
                            Time->tm_hour = atoi(hour);
                            Time->tm_min = atoi(minute);
                            Time->tm_sec = atoi(second);
                            Time->tm_isdst = DLT;
                            /* Converting data to correct UTC time. */
                            PacTime = (8 - Time->tm_isdst) * 3600; // This
                            // assumes the XML data was given in Pacific Time.
                            UTC = mktime(Time) + PacTime;

                        interval.startDateTime = (long int) UTC;
                        //Writing to interval, an instantiation of SetPoint Cls.

                        /* Setting the EndDateTime */

                            /* Pulling the string from the XML file */
                            timeDateStr =
                            subsubtree.get<std::string>("endDateTime");
                            /*Parsing string to get actual year,month,date,etc*/
                            year[0] = timeDateStr[0];
                            year[1] = timeDateStr[1];
                            year[2] = timeDateStr[2];
                            year[3] = timeDateStr[3];
                            month[0] = timeDateStr[5];
                            month[1] = timeDateStr[6];
                            day[0] = timeDateStr[8];
                            day[1] = timeDateStr[9];
                            hour[0] = timeDateStr[11];
                            hour[1] = timeDateStr[12];
                            minute[0] = timeDateStr[14];
                            minute[1] = timeDateStr[15];
                            second[0] = timeDateStr[17];
                            second[1] = timeDateStr[18];
                            /*Changing strings to times. Storing in tm struct.*/
                            Time->tm_year = atoi(year) - 1900;
                            //Stored as years after 1900
                            Time->tm_mon = atoi(month) - 1;
                            //Stored as month from 0 to 11, with 0=Jan
                            Time->tm_mday = atoi(day);
                            Time->tm_hour = atoi(hour);
                            Time->tm_min = atoi(minute);
                            Time->tm_sec = atoi(second);
                            Time->tm_isdst = DLT;
                            /* Converting data to correct UTC time. */
                            PacTime = (8 - Time->tm_isdst) * 3600;
                            //Assumes data was given in Pacific Time.
                            UTC = mktime(Time) + PacTime;

                        interval.endDateTime = (long int) UTC;
                        //Writing to interval, an instantiation of SetPoint Cls.

                        /* Populating schedule vector with Value1,Start & End */
                        schedule.push_back(interval);
                        // interval is an instantiation of SetPoint Class.

                    } // Closes my branch statement for storing data points.
                } // Closes my FOREACH for the subsubtree where b==a.
        } // Closes my branch statement for the right data set (b==a).
    } // Closes my FOREACH for the subtree which is counting for when b==a.


      return schedule;
  }


#endif // XML2SCHEDULE_H_INCLUDED
