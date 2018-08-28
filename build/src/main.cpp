/******************************************************************************
 *    Copyright (c) Open Connectivity Foundation (OCF), AllJoyn Open Source
 *    Project (AJOSP) Contributors and others.
 *
 *    SPDX-License-Identifier: Apache-2.0
 *
 *    All rights reserved. This program and the accompanying materials are
 *    made available under the terms of the Apache License, Version 2.0
 *    which accompanies this distribution, and is available at
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 *    Copyright (c) Open Connectivity Foundation and Contributors to AllSeen
 *    Alliance. All rights reserved.
 *
 *    Copyright (c) V2 Systems, LLC.  All rights reserved.
 *
 *    Permission to use, copy, modify, and/or distribute this software for
 *    any purpose with or without fee is hereby granted, provided that the
 *    above copyright notice and this permission notice appear in all
 *    copies.
 *
 *    THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL
 *    WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED
 *    WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE
 *    AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL
 *    DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR
 *    PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
 *    TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 *    PERFORMANCE OF THIS SOFTWARE.
******************************************************************************/

// INCLUDE
#include <iostream>     // cout, cin
#include <thread>       // thread, join
#include <chrono>       // now, duration
#include <map>
#include <string>
#include <vector>

#include "include/ts_utility.hpp"
#include "include/aj_utility.hpp"
#include "include/Aggregator.hpp"
#include "include/DistributedEnergyResource.hpp"
#include "include/ClientListener.hpp"
#include "include/SmartGridDevice.hpp"

using namespace std;
using namespace ajn;

bool done = false;  //signals program to stop

// Help
// - CLI interface description
static void Help () {
    cout << "\n\t[Help Menu]\n\n";
    cout << "> q            quit\n";
    cout << "> h            display help menu\n";
    cout << "> a            print all resources\n";
} // end Help

// Command Line Interface
// - method to allow user controls during program run-time
static bool CommandLineInterface (const string& input, Aggregator* vpp) {
    // check for program argument
    if (input == "") {
        return false;
    }
    char cmd = input[0];

    // deliminate input string to argument parameters
    vector <string> tokens;
    stringstream ss(input);
    string token;
    while (ss >> token) {
        tokens.push_back(token);
    }

    switch (cmd) {
        case 'q':
           return true;

        case 'a': {
            vpp->DisplayAll();
        }

        default: {
            Help();
            break;
        }
    }

    return false;
}  // end Command Line Interface

// Main
// ----
int main (int argc, char** argv) {
    cout << "\nStarting Program...\n";
    cout << "\n\tLoading parameters...\n";

    cout << "\n\tMapping configuration file...\n";
    tsu::config_map ini_map = tsu::MapConfigFile("../data/config.ini");

    cout << "\n\tStarting AllJoyn...\n";

    try {
        AllJoynInit();
    } catch (exception &e) {
        cout << "[ERROR]: " << e.what() << endl;
        return EXIT_FAILURE;
    }

    #ifdef ROUTER
        try {
            AllJoynRouterInit();
        } catch (exception &e) {
            cout << "[ERROR]: " << e.what() << endl;
            return EXIT_FAILURE;
        }
    #endif // ROUTER

    cout << "\n\t\tCreating message bus...\n";
    const char* app_name = ini_map["AllJoyn"]["app"].c_str();
    bool allow_remote = true;
    BusAttachment *bus_ptr = new BusAttachment(app_name, allow_remote);
    assert(bus_ptr != NULL);

    cout << "\n\t\tCreating about object...\n";
    AboutData about_data("en");
    AboutObj *about_ptr = new AboutObj(*bus_ptr);
    assert(about_ptr != NULL);

    cout << "\n\t\tEstablishing session port...\n";
    aj_utility::SessionPortListener SPL;
    ajn::SessionPort port = stoul (ini_map["AllJoyn"]["port"]);

    cout << "\n\t\tSetting up bus attachment...\n";
    QStatus status = aj_utility::SetupBusAttachment (ini_map,
                                                     port,
                                                     SPL,
                                                     bus_ptr,
                                                     &about_data);

    if (status != ER_OK) {
        delete about_ptr;
        delete bus_ptr;
        return EXIT_FAILURE;
    }

    cout << "\n\t\tCreating observer...\n";
    const char* client_name = ini_map["AllJoyn"]["client_interface"].c_str();
    Observer *obs_ptr = new Observer(*bus_ptr, &client_name, 1);

    cout << "\n\t\tCreating virtual power plant...\n";
    Aggregator *vpp_ptr = new Aggregator ();

    cout << "\n\t\tCreating listener...\n";
    ClientListener *listner_ptr = new ClientListener(bus_ptr,
                                                     obs_ptr,
                                                     vpp_ptr,
                                                     client_name);
    obs_ptr->RegisterListener(*listner_ptr);

    cout << "\n\t\tCreating bus object...\n";
    const char* server_name = ini_map["AllJoyn"]["server_interface"].c_str();
    const char* path = ini_map["AllJoyn"]["path"].c_str();
    SmartGridDevice *sgd_ptr = new SmartGridDevice(bus_ptr, 
                                                   server_name, 
                                                   path);

    cout << "\n\t\t\tRegistering bus object...\n";
    if (ER_OK != bus_ptr->RegisterBusObject(*sgd_ptr)){
        printf("\n\t\t\t[ERROR] failed registration...\n");
        delete &sgd_ptr;
        return EXIT_FAILURE;
    }
    about_ptr->Announce(port, about_data);

    cout << "\nProgram initialization complete...\n";

    Help ();
    string input;

    while (!done) {
        getline(cin, input);
        done = CommandLineInterface(input, vpp_ptr);
    }

    cout << "\nProgram shutting down...\n";
    cout << "\n\t Joining threads...\n";

    cout << "\n\t deleting pointers...\n";
    delete sgd_ptr;
    delete listner_ptr;
    delete vpp_ptr;
    delete obs_ptr;
    delete about_ptr;
    delete bus_ptr;

    cout << "\n\t Shutting down AllJoyn...\n";
    obs_ptr->UnregisterAllListeners ();

    #ifdef ROUTER
        AllJoynRouterShutdown ();
    #endif // ROUTER

    AllJoynShutdown ();

    return EXIT_SUCCESS;
} // end main
