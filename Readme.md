## Observer Pattern + Protobuf + 0MQ
This project is an exercise on Observer pattern, google's protobuf and 0MQ:
The subject in the observer pattern, here called Player -with football in mind- generates 
data according to a proto file. 
The observers, called Sensors (for example think of cameras) sample the generated data.
Obviously this can easily extend to anything else, e.g. a car and its sensors, etc. 

Almost everything is templated which may seem redundant for such a simple setup. 
But the goal was to have fun and create boiler plates for my other projects.

To set the configuration values, update configuration.toml in the root before building, 
or the copied file in your build directory.

### ================================================================

Upon successful building, there are different apps available:
1) subject_observers_simulation and receiver binaries in [build_directory/]apps/subject_observers_simulation : 
The first one generates the data and the other one simply receives the data stream and displays it on standard character output (std:cout)

TODO 
2) A Docker version of the above 

TODO 
3) Generator which writes to a file.

### Dependencies
- C++23
- cmake
- Google Protobuf (https://protobuf.dev/)
- 0MQ (https://zeromq.org/)
- Toml++ (https://marzer.github.io/tomlplusplus/)
+ Google Test (https://github.com/google/googletest)

