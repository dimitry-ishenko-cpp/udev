#include <asio.hpp>
#include <iostream>
#include <udev++.hpp>

using namespace std::chrono_literals;

int main(int argc, char* argv[])
{
    asio::io_context io;

    udev::monitor monitor{io};
    monitor.match_device("block");
    monitor.enable();

    monitor.on_device_added([&](udev::device dev)
    {
        if (dev.devtype() == "partition" && dev.property("ID_BUS") == "usb")
        {
            std::cout << "USB drive plugged in"
                      << "\n  syspath: " << dev.syspath()
                      << "\n  devnode: " << dev.devnode()
                      << "\n  filesys: " << dev.property("ID_FS_TYPE")
                      << "\n  sysname: " << dev.sysname()
                      << "\n  sysnum : " << dev.sysnum ();
            std::cout << std::endl;
        }
    });

    monitor.on_device_removed([&](udev::device dev)
    {
        if (dev.devtype() == "partition" && dev.property("ID_BUS") == "usb")
        {
            std::cout << "USB drive unplugged, devnode: " << dev.devnode();
            std::cout << std::endl;
        }
    });

    while (io.run_one_for(30s));

    std::cout << "Nothing seems to be happening - exiting" << std::endl;
    return 0;
}
