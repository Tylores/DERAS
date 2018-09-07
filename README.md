# DERAS
Distributed Energy Resource Aggregation System

## Dependancies
### Linux Environment
``` console
sudo apt-get update
sudo apt-get upgrade
sudo apt-get install git build-essential gcc g++ cmake make xsltproc scons doxygen graphviz libgtk2.0-dev libssl-dev libxml2-dev libcap-dev
mkdir ~/dev ~/src
```
### AllJoyn
First clone the AllJoyn repository into your /src folder. 
``` console
cd ~/src
git clone https://github.com/alljoyn/core-alljoyn
cd core-alljoyn
```

#### x86 and x86_64 processors
``` console
scons BINDINGS=cpp WS=off DOCS=html
```

#### Test AllJoyn Installation
Note: the CPU variable is dependant on your system. 
``` console
export CPU=x86_64
export AJ_LIB=~/src/core-alljoyn/build/linux/$CPU/debug/dist/cpp/lib
export LD_LIBRARY_PATH=$AJ_LIB:$LD_LIBRARY_PATH

cd $AJ_LIB
cd ../bin/samples
./AboutService
```
If the BusAttachment/AboutObj succeed, then the installation should be fine.

## Install
``` console
cd ~/dev
git clone https://github.com/Tylores/DERAS
```

### Setup
1. Open /DERAS/tools/build-run.sh
2. Modify "CPU" to reflect the system you are working on
3. Ensure "AJ_ROOT" is correct

``` console
cd ~/dev/DCS/tools
./build-run.sh
```
## Use
The program can be controlled two ways:
1. TODO: Operator class that will execute commands based on input file.
2. The Command Line Interface (CLI).

```
        [Help]
> q             quit
> h             help
> i <watts>     import power
> e <watts>     export power
> p             print properties
```

## Class UML

<p align="center">
  <img src="DERAS_Class_UML.png" alt="Class UML">
</p>
