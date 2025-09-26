#include <iostream>
#include <udev++.hpp>

using namespace std;

int main(int argc, char* argv[])
{
    udev::enumerate enumerate;
    enumerate.match_subsystem("block");

    for(auto&& device : enumerate.get())
    {
        using namespace std;

        if(device.devtype() == "partition")
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

    return 0;
}
