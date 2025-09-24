////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2017-2021 Dimitry Ishenko
// Contact: dimitry (dot) ishenko (at) (gee) mail (dot) com
//
// Distributed under the GNU GPL license. See the LICENSE.md file for details.

////////////////////////////////////////////////////////////////////////////////
#ifndef UDEV_DEVICE_HPP
#define UDEV_DEVICE_HPP

////////////////////////////////////////////////////////////////////////////////
#include "udev.hpp"

#include <memory>
#include <string>

namespace impl
{

struct udev;
struct udev_device;
struct device_delete { void operator()(udev_device*); };

}

////////////////////////////////////////////////////////////////////////////////
namespace udev
{

using std::string;

enum action { added, removed, other };

////////////////////////////////////////////////////////////////////////////////
// Udev device.
//
// Provides access to udev device attributes. Device instances are returned by
// the enumerate and monitor classes.
//
class device
{
public:
    device() noexcept = default;

    device(const device&) = delete;
    device(device&& rhs) noexcept = default;

    device& operator=(const device&) = delete;
    device& operator=(device&& rhs) noexcept = default;

    bool is_valid() const noexcept { return !!dev_; }
    explicit operator bool() const noexcept { return is_valid(); }

    device parent() const noexcept;
    device parent(const string& subsystem, const string& devtype = { }) const noexcept;

    string subsystem() const noexcept;
    string   devtype() const noexcept;
    string   syspath() const noexcept;
    string   sysname() const noexcept;
    string    sysnum() const noexcept;
    string   devnode() const noexcept;

    string property(const string&) const noexcept;
    string driver() const noexcept;
    enum action action() const noexcept;
    string sysattr(const string&) const noexcept;
    bool has_tag(const string&) const noexcept;

private:
    udev udev_;
    std::unique_ptr<impl::udev_device, impl::device_delete> dev_;

    device(udev, const string&);
    device(udev, impl::udev_device* dev) noexcept;

    friend class enumerate;
    friend class monitor;
};

////////////////////////////////////////////////////////////////////////////////
}

////////////////////////////////////////////////////////////////////////////////
#endif
