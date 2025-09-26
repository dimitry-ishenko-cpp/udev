////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2017-2025 Dimitry Ishenko
// Contact: dimitry (dot) ishenko (at) (gee) mail (dot) com
//
// Distributed under the GNU GPL license. See the LICENSE.md file for details.

////////////////////////////////////////////////////////////////////////////////
#include "udev++/device.hpp"
#include "udev++/error.hpp"

namespace impl
{
#include <libudev.h>
void device_delete::operator()(udev_device* dev) { udev_device_unref(dev); }
}

////////////////////////////////////////////////////////////////////////////////
namespace
{

inline auto string(const char* s) noexcept
{
    return s ? std::string{s} : std::string{};
}

}

////////////////////////////////////////////////////////////////////////////////
namespace udev
{

////////////////////////////////////////////////////////////////////////////////
device::device(impl::udev* udev, std::string_view path) :
    dev_{impl::udev_device_new_from_syspath(udev, path.data())}
{
    if (!dev_) throw error{"device::device()"};
}

////////////////////////////////////////////////////////////////////////////////
device device::parent() const
{
    auto dev = impl::udev_device_get_parent(&*dev_);
    if (!dev) throw error{"device::parent()"};

    // udev_device_get_parent() returns reference whose lifetime is tied to the
    // child's lifetime, so we have to make a "deep copy"
    //
    auto udev = impl::udev_device_get_udev(&*dev_);
    auto path = impl::udev_device_get_syspath(dev);
    return device{udev, path};
}

device device::parent(std::string_view subsystem, std::string_view type) const
{
    auto dev = impl::udev_device_get_parent_with_subsystem_devtype(&*dev_,
        subsystem.data(), type.size() ? type.data() : nullptr
    );
    if (!dev) throw error{"device::parent()"};

    // udev_device_get_parent() returns reference whose lifetime is tied to the
    // child's lifetime, so we have to make a "deep copy"
    //
    auto udev = impl::udev_device_get_udev(&*dev_);
    auto path = impl::udev_device_get_syspath(dev);
    return device{udev, path};
}

////////////////////////////////////////////////////////////////////////////////
std::string device::subsystem() const noexcept
{
    return string(impl::udev_device_get_subsystem(&*dev_));
}

////////////////////////////////////////////////////////////////////////////////
std::string device::devtype() const noexcept
{
    return string(impl::udev_device_get_devtype(&*dev_));
}

////////////////////////////////////////////////////////////////////////////////
std::string device::syspath() const noexcept
{
    return string(impl::udev_device_get_syspath(&*dev_));
}

////////////////////////////////////////////////////////////////////////////////
std::string device::sysname() const noexcept
{
    return string(impl::udev_device_get_sysname(&*dev_));
}

////////////////////////////////////////////////////////////////////////////////
std::string device::sysnum() const noexcept
{
    return string(impl::udev_device_get_sysnum(&*dev_));
}

////////////////////////////////////////////////////////////////////////////////
std::string device::devnode() const noexcept
{
    return string(impl::udev_device_get_devnode(&*dev_));
}

////////////////////////////////////////////////////////////////////////////////
std::string device::property(std::string_view name) const noexcept
{
    return string(impl::udev_device_get_property_value(&*dev_, name.data()));
}

////////////////////////////////////////////////////////////////////////////////
std::string device::driver() const noexcept
{
    return string(impl::udev_device_get_driver(&*dev_));
}

////////////////////////////////////////////////////////////////////////////////
enum action device::action() const noexcept
{
    auto action = string(impl::udev_device_get_action(&*dev_));
    return action == "add" ? added : action == "remove" ? removed : other;
}

////////////////////////////////////////////////////////////////////////////////
std::string device::sysattr(std::string_view name) const noexcept
{
    return string(impl::udev_device_get_sysattr_value(&*dev_, name.data()));
}

////////////////////////////////////////////////////////////////////////////////
bool device::has_tag(std::string_view name) const noexcept
{
    return impl::udev_device_has_tag(&*dev_, name.data());
}

////////////////////////////////////////////////////////////////////////////////
}
