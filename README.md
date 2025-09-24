# udev++ – a Udev Library for C++

The **udev++** library is a lightweight C++ wrapper for [libudev](https://www.freedesktop.org/software/systemd/man/libudev.html). It allows one to enumerate, monitor and introspect devices on the local system.

See the [Usage](#usage) section below if you are planning to develop applications with **udev++**.

## Installation

### Prerequisites

* libudev (provided by [systemd](https://www.freedesktop.org/wiki/Software/systemd/))

### Binary

Debian/Ubuntu/etc:

```shell
sudo add-apt-repository ppa:ppa-verse/termin
sudo apt install libudev++
```

Install the development package, if you are planning to develop applications with **udev++**:
```shell
sudo apt install libudev++-dev
```

RaspberryPi:

```shell
sudo add-apt-repository -S deb https://ppa.launchpadcontent.net/ppa-verse/termin/ubuntu jammy main
sudo apt install libudev++
```

Install the development package, if you are planning to develop applications with **udev++**:
```shell
sudo apt install libudev++-dev
```

### From source

Stable version (requires [CMake](https://cmake.org/) >= 3.16):

```shell
p=udev v=1.0
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

to find the library and link with it. **udev++** additionally exports `udev++::udev++_static` and `udev++::udev++_shared` targets, which you can use to explicitly link with the static and shared versions of the library respectively.

## Description

TODO

Following are a few examples demonstrating its capabilities:

Example #1:

```c++
auto ctx = udev::udev::instance();

udev::enumerate enumerate{ ctx };
enumerate.match_subsystem("block");

auto devices = enumerate.get();
for(auto const& device : devices)
{
    using namespace std;

    if(device.type() == "partition" && device.property("ID_BUS") == "ata")
    {
        cout << "Found hard disk" << endl
             << "Path: " << device.syspath() << endl
             << "Node: " << device.devnode() << endl
             << "  FS: " << device.property("ID_FS_TYPE") << endl
             << "Name: " << device.sysname() << endl
             << "   #: " << device.sysnum () << endl;
        cout << endl;
    }
}
```

Example #2:
```c++
auto ctx = udev::udev::instance();

udev::monitor monitor{ ctx };
monitor.match_device("block");

for(;;)
{
    using namespace std::chrono_literals;
    using namespace std;

    if(auto device = monitor.try_get_for(30s))
    {
        switch(device.action())
        {
        case udev::added:
            if(device.type() == "partition" && device.property("ID_BUS") == "usb")
            {
                cout << "USB drive plugged in" << endl
                     << "Path: " << device.syspath() << endl
                     << "Node: " << device.devnode() << endl
                     << "  FS: " << device.property("ID_FS_TYPE") << endl
                     << "Name: " << device.sysname() << endl
                     << "   #: " << device.sysnum () << endl;
                cout << endl;
            }
            break;

        case udev::removed:
            if(device.type() == "partition" && device.property("ID_BUS") == "usb")
            {
                cout << "USB drive " << device.devnode() << " unplugged" << endl;
                cout << endl;
            }
            break;
        }
    }
    else
    {
        cout << "Nothing seems to be happening" << endl;
        break;
    }
}
```

## Authors

* **Dimitry Ishenko** - dimitry (dot) ishenko (at) (gee) mail (dot) com

## License

This project is distributed under the GNU GPL license. See the
[LICENSE.md](LICENSE.md) file for details.
