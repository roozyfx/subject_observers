# Subject-Observers

A C++23 project exploring a templated implementation of classic **Observer design pattern** together with **[Protocol Buffers](https://protobuf.dev/)** for message serialization and **[ZeroMQ](https://zeromq.org/)** for transport.

The scenario is modeled as an agent, e.g. a robot or a human (`Player` class) being tracked by a set of sensors (`Sensor`). The design generalizes to any subject/observer relationship — a vehicle and its sensors, etc.

```
Player --notify--> Sensors --protobuf--> ZMQ PUB ---> SUB --> receiver
```

Everything is templated on the float type. Overkill for a demo, but I wanted the boilerplate for later.  

## Dependencies

- A C++23 compiler (tested with GCC 15 and Clang 21)
- [CMake](https://cmake.org/)
- [Ninja](https://ninja-build.org/)
- [Google Protobuf](https://protobuf.dev/)
- [ZeroMQ](https://zeromq.org/) (via [cppzmq](https://github.com/zeromq/cppzmq))
- [tomlplusplus](https://marzer.github.io/tomlplusplus/) (via CMake `FetchContent`)
- [GoogleTest](https://github.com/google/googletest) (via CMake `FetchContent`)

## Build
Included presets are gcc15, clang-21, macOS clang and Windows, Debug and Release. Debug builds get ASan/UBSan.
```bash
cmake --preset <preset>
cmake --build --preset <preset>
ctest --preset <preset>
```

## Run

Receiver first — PUB throws away anything nobody is listening to yet.
Upon successful build, there are two binaries together with the config file in `./build/<preset>/apps/subject_observers_simulation/`  
```bash
./receiver

# then, in another terminal  
./subject_observers_simulation
```

## Config

`configuration.toml`: field bounds, `top_speed`, `sample_frequency`, `measurement_noise`, `number_sensors`, `simulation_duration`.

## Threading Notes

`Simulation` owns its thread. `StartSimulation()` returns immediately, `Wait()` blocks and rethrows whatever the worker threw, `StopSimulation()` asks it to quit.  
The destructor joins *before* unregistering the sensors — a destructor body runs before its members are destroyed, so that join has to be explicit.  
