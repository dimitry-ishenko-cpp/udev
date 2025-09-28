# udev++ – a udev Library for C++

The **udev++** library is a lightweight C++ wrapper for [libudev](https://www.freedesktop.org/software/systemd/man/libudev.html). It allows one to enumerate, monitor and introspect devices on the local system.

See the [Usage](#usage) section below if you are planning to develop applications with **udev++**.

## Installation

### Prerequisites

* libudev (provided by [systemd](https://www.freedesktop.org/wiki/Software/systemd/))
* [Asio](https://think-async.com/Asio/)

### Binary

Debian/Ubuntu/etc:

```shell
sudo add-apt-repository ppa:ppa-verse/public
sudo apt install libudev++
```

Install the development package, if you are planning to develop applications with **udev++**:
```shell
sudo apt install libudev++-dev
```

RaspberryPi:

```shell
sudo add-apt-repository -S deb https://ppa.launchpadcontent.net/ppa-verse/public/ubuntu jammy main
sudo apt install libudev++
```

Install the development package, if you are planning to develop applications with **udev++**:
```shell
sudo apt install libudev++-dev
```

### From source

Stable version (requires [CMake](https://cmake.org/) >= 3.16):

```shell
p=udev v=4.0.1
wget https://github.com/dimitry-ishenko-cpp/${p}/archive/v${v}.tar.gz
tar xzf v${v}.tar.gz
mkdir ${p}-${v}/build
cd ${p}-${v}/build
cmake ..
make
sudo make install
```

Latest master (requires [git](https://git-scm.com/) and [CMake](https://cmake.org/) >= 3.16):

```shell
p=udev
git clone --recursive https://github.com/dimitry-ishenko-cpp/${p}.git
mkdir ${p}/build
cd ${p}/build
cmake ..
make
sudo make install
```

## Usage

To use **udev++** in your application simply add:

```c++
#include <udev++.hpp>
```

to your file(s) and link with `-ludev++`. For [CMake](https://cmake.org/)-based projects use:

```cmake
find_package(udev++ REQUIRED)
...
target_link_libraries(marvin PRIVATE udev++::udev++)
```

to find the library and link with it.

**udev++** additionally exports `udev++::udev++_static` and `udev++::udev++_shared` targets, which you can use to explicitly link with the static and shared versions of the library respectively.

## Description

_TODO_

For now check the [example](./example/) directory for usage examples.

## Authors

* **Dimitry Ishenko** - dimitry (dot) ishenko (at) (gee) mail (dot) com

## License

This project is distributed under the GNU GPL license. See the
[LICENSE.md](LICENSE.md) file for details.
