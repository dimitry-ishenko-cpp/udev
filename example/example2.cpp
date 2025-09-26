#include <iostream>
#include <udev++.hpp>

using namespace std;
using namespace std::chrono_literals;

int main(int argc, char* argv[])
{
    udev::monitor monitor;
    monitor.match_device("block");

    for(;;)
    {
        if(auto device = monitor.try_get_for(30s))
        {
            switch(device.action())
            {
            case udev::added:
                if(device.devtype() == "partition" && device.property("ID_BUS") == "usb")
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
                if(device.devtype() == "partition" && device.property("ID_BUS") == "usb")
                {
                    cout << "USB drive " << device.devnode() << " unplugged" << endl;
                    cout << endl;
                }
                break;

            default:;
            }
        }
        else
        {
            cout << "Nothing seems to be happening" << endl;
            break;
        }
    }

    return 0;
}
