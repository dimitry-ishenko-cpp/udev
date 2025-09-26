////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2017-2025 Dimitry Ishenko
// Contact: dimitry (dot) ishenko (at) (gee) mail (dot) com
//
// Distributed under the GNU GPL license. See the LICENSE.md file for details.

////////////////////////////////////////////////////////////////////////////////
#include "udev++/enumerate.hpp"
#include "udev++/error.hpp"

namespace impl
{
#include <libudev.h>
void enumerate_deleter::operator()(udev_enumerate* en) { udev_enumerate_unref(en); }
}

////////////////////////////////////////////////////////////////////////////////
namespace
{

inline auto charp(std::string_view s) noexcept
{
    return s.size() ? s.data() : nullptr;
}

}

////////////////////////////////////////////////////////////////////////////////
namespace udev
{

////////////////////////////////////////////////////////////////////////////////
enumerate::enumerate() : udev_{udev::instance()},
    enum_{impl::udev_enumerate_new(&*udev_.udev_)}
{
    if (!enum_) throw error{"enumerate::enumerate()"};
}

////////////////////////////////////////////////////////////////////////////////
void enumerate::match_subsystem(std::string_view subsystem)
{
    auto code = impl::udev_enumerate_add_match_subsystem(&*enum_, subsystem.data());
    if (code) throw error{code, "enumerate::match_subsystem()"};
}

void enumerate::nomatch_subsystem(std::string_view subsystem)
{
    auto code = impl::udev_enumerate_add_nomatch_subsystem(&*enum_, subsystem.data());
    if (code) throw error{code, "enumerate::nomatch_subsystem()"};
}

////////////////////////////////////////////////////////////////////////////////
void enumerate::match_sysattr(std::string_view name, std::string_view value)
{
    auto code = impl::udev_enumerate_add_match_sysattr(&*enum_, name.data(), charp(value));
    if (code) throw error{code, "enumerate::match_sysattr()"};
}

void enumerate::nomatch_sysattr(std::string_view name, std::string_view value)
{
    auto code = impl::udev_enumerate_add_nomatch_sysattr(&*enum_, name.data(), charp(value));
    if (code) throw error{code, "enumerate::nomatch_sysattr()"};
}

////////////////////////////////////////////////////////////////////////////////
void enumerate::match_property(std::string_view name, std::string_view value)
{
    auto code = impl::udev_enumerate_add_match_property(&*enum_, name.data(), charp(value));
    if (code) throw error{code, "enumerate::match_property()"};
}

////////////////////////////////////////////////////////////////////////////////
void enumerate::match_sysname(std::string_view name)
{
    auto code = impl::udev_enumerate_add_match_sysname(&*enum_, name.data());
    if (code) throw error{code, "enumerate::match_sysname()"};
}

////////////////////////////////////////////////////////////////////////////////
void enumerate::match_tag(std::string_view name)
{
    auto code = impl::udev_enumerate_add_match_tag(&*enum_, name.data());
    if (code) throw error{code, "enumerate::match_tag()"};
}

////////////////////////////////////////////////////////////////////////////////
void enumerate::match_parent(const device& dev)
{
    auto code = impl::udev_enumerate_add_match_parent(&*enum_, &*dev.dev_);
    if (code) throw error{code, "enumerate::match_parent()"};
}

////////////////////////////////////////////////////////////////////////////////
std::vector<device> enumerate::get()
{
    std::vector<device> devices;

    auto code = impl::udev_enumerate_scan_devices(&*enum_);
    if (code) throw error{code, "enumerate::get()"};

    impl::udev_list_entry* e;
    udev_list_entry_foreach(e, impl::udev_enumerate_get_list_entry(&*enum_))
    {
        auto path = impl::udev_list_entry_get_name(e);
        if (path) devices.push_back(device{&*udev_.udev_, path});
    }

    return devices;
}

////////////////////////////////////////////////////////////////////////////////
}
