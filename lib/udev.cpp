////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2017-2025 Dimitry Ishenko
// Contact: dimitry (dot) ishenko (at) (gee) mail (dot) com
//
// Distributed under the GNU GPL license. See the LICENSE.md file for details.

////////////////////////////////////////////////////////////////////////////////
#include "udev++/error.hpp"
#include "udev++/udev.hpp"

#include <utility> // std::swap

namespace impl
{
#include <libudev.h>
}

////////////////////////////////////////////////////////////////////////////////
namespace udev
{

////////////////////////////////////////////////////////////////////////////////
udev::udev() : udev_{impl::udev_new()}
{
    if (!udev_) throw error{"udev::udev()"};
}

udev::~udev() { impl::udev_unref(udev_); }

udev::udev(const udev& rhs) noexcept : udev_{impl::udev_ref(rhs.udev_)} { }
udev::udev(udev&& rhs) noexcept : udev_{rhs.udev_} { rhs.udev_ = nullptr; }

udev& udev::operator=(udev rhs) noexcept
{
    std::swap(udev_, rhs.udev_);
    return (*this);
}

////////////////////////////////////////////////////////////////////////////////
udev udev::instance()
{
    static udev instance;
    return instance;
}

////////////////////////////////////////////////////////////////////////////////
}
