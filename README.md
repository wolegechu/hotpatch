# HotPatch

## Introduction

Computer progroms are like humans in a way which is extensible to do almost everything. But human can learn and react continuously while softwares need to restart after adding features and applying patches. In some cases, we can apply the hot patches which are different from the static code patches without restarting the processes.

This project helps to patch and upgrade your programs with the following features.

[*] Set and get the [gflags](https://github.com/gflags/gflags) on the fly.
[*] Set the logging properties of [glogs](https://github.com/google/glog) on the fly.
[*] Set and print any registered variables on the fly.
[*] Load and unload the dynamic libraries on the fly.
[*] Upgrade and rollback the implementation of registered functions on the fly.
[*] Full support for Linux, MacOS and Windows.


## Installation

Build the project from scratch.

```
mkdir ./build/
cd ./build/
cmake ..
make
```

Integrated with your C++ programs.

```
cmake -DCMAKE_INSTALL_PREFIX=/foo/bar/ ..
make
make install
```

## Quickstart

We can run the pre-built binary with the preload library to change the properties at any time.

```
# For Linux
LD_PRELOAD="./preload/libpreload_hotpatch.so" ./examples/prebuilt_server

# For Mac
DYLD_INSERT_LIBRARIES=./preload/libpreload_hotpatch.dylib ./examples/prebuilt_server
```

Use the Python client or command `nc` to set and get the log level on the fly.

```
hotpatch -p $PID gflags set minloglevel 2

hotpatch -p $PID gflags get minloglevel
```

We can register the variables and functions in your C++ program, take [simple_server](./examples/simple_server.cpp) for example.

```
// Use Hotpatch
hotpatch::InitHotpatchServer();

// Register variable
std::string user_name = "myname";
hotpatch::RegisterVariable("user_name", &user_name);
int age = 10;
hotpatch::RegisterVariable("age", &age);

// Register function
hotpatch::RegisterFunction("add_func", reinterpret_cast<void*>(add_func));
```

Then we can get and set the values in any time while the programing is running. It is usable for debuging or changing the logic of your programs without re-compiling and restarting the processes.

```
hotpatch -p $PID var get string user_name
hotpatch -p $PID var set string user_name new_name

hotpatch -p $PID var get int age
hotpatch -p $PID var set int age 20
```

Loading the new libraries allows developers to inject the new implementation of the functions on the fly and rollback is supported as well.

```
hotpatch -p $PID lib load add_func_patch1 ./examples/libadd_func_patch1.dylib

hotpatch -p $PID func upgrade add_func_patch1 add_func

hotpatch -p $PID func rollback add_func
```

## Client

### Python Client

The command `hotpatch` is the socket client in Python which can be installed easily.

```
pip install hotpatch
```

Get and set the `gflags` or `glogs` properties.

```
hotpatch -p $PID gflags list

hotpatch -p $PID gflags get minloglevel

hotpatch -p $PID gflags set minloglevel 2
```

Get and set the registered variables.

```
hotpatch -p $PID var list

hotpatch -p $PID var get string user_name

hotpatch -p $PID var set string user_name new_name
```

Load and unload the dynamic libraries.

```
hotpatch -p $PID lib list

hotpatch -p $PID lib load add_func_patch1 ./examples/libadd_func_patch1.dylib

hotpatch -p $PID lib unload add_func_patch1
```

Upgrade and rollback the function implementation.

```
hotpatch -p $PID func upgrade add_func_patch1 add_func

hotpatch -p $PID func rollback add_func
```

### Shell Client

`nc` is the built-in tool in most operating systems which can access the unix socket as well.

Get and set the `gflags` or `glogs` properties.

```
echo "gflags list" | nc -U /tmp/$PID.socket

echo "gflags get minloglevel" | nc -U /tmp/$PID.socket

echo "gflags set minloglevel 2" | nc -U /tmp/$PID.socket
```

Get and set the registered variables.

```
echo "var list" | nc -U /tmp/$PID.socket

echo "var get string user_name" | nc -U /tmp/$PID.socket

echo "var set string user_name new_name" | nc -U /tmp/$PID.socket
```

Load and unload the dynamic libraries.

```
echo "lib list" | nc -U /tmp/$PID.socket

echo "lib load add_func_patch1 ./examples/libadd_func_patch1.dylib" | nc -U /tmp/$PID.socket

echo "lib unload add_func_patch1" | nc -U /tmp/$PID.socket
```

Upgrade and rollback the function implementation.

```
echo "func upgrade add_func_patch1 add_func" | nc -U /tmp/$PID.socket

echo "func rollback add_func" | nc -U /tmp/$PID.socket
```
