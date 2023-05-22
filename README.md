# MuhSat-1 - A Student-Developed CubSat Flight Software

Experimental C++17 based CubeSat flight control software.
Currently runing on linux, but planned o adapt to FreeRTOS in the near future.

* Ground station and Sat segments
* CubeSat with subsystems
  - OBC
  - EPS
  - COMS
  - Payloads plugins
* uses [CSP](https://en.wikipedia.org/wiki/Cubesat_Space_Protocol) protocol for communication between CubSat and ground station segment and between the CubeSat subsystem.
 

## How to build

* first time you need to prepare all dependencies
  - sqlite3 lib
  - libcsp sub module
  
  - the following will handle this
```
./prep-depends.sh
```

* TBD
