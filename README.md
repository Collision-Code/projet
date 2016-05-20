# COLLISION-CODE

University of Rouen (Normandie, France)
https://github.com/Collision-Code/projet

Software developped by :
	- Jérémie PANTIN
	- Clément POINSOT
	- Thomas CAPET, 
    - Anthony BRÉANT
	- Mohamed TAKHTOUKH

Collision-Code calculate collision cross sections according to the methods in
the MOBCAL software (Shvartsburg and Jarrold, 1996-1999) available from 
M. F. Jarrold's group at Indiana University
(http://www.indiana.edu/~nano/software/)

Contact us at : collisioncode@gmail.com

# Getting started :
-- Windows

-- Linux

## Only Console version is aviable now

Open a terminal and execute following commands :
```
sudo apt-get install build-essential
sudo apt-get install g++
sudo apt-get install qt5-default
sudo apt-get install qtbase5-dev-tolls
git clone https://github.com/Collision-Code/projet/
cd projet
make calc
```

`./Collision-Code` to execute software.

Execute `make` will make both versions (console and GUI) of Collision-Code.

`make calc` will make only console version and `make ihm` will make only GUI version. GUI version still in developpement.

If you compile at least the console version, just type `./Collision-Code --help` to get some help about usage.