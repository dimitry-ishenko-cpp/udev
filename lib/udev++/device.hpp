////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2017-2025 Dimitry Ishenko
// Contact: dimitry (dot) ishenko (at) (gee) mail (dot) com
//
// Distributed under the GNU GPL license. See the LICENSE.md file for details.

////////////////////////////////////////////////////////////////////////////////
#ifndef UDEV_DEVICE_HPP
#define UDEV_DEVICE_HPP

////////////////////////////////////////////////////////////////////////////////
#include <memory>
#include <string>
#include <string_view>

namespace impl
{

struct udev;
struct udev_device;
struct device_deleter { void operator()(udev_device*); };

}

////////////////////////////////////////////////////////////////////////////////
namespace udev
{

enum action { added, removed, other };

/**
 * @class udev::device
 * @brief Represents a udev device.
 *
 * Enables access to udev device properties. Instances are obtained from
 * `enumerate` and `monitor` classes.
 *
 * @sa udev::enumerate, udev::monitor
 */
class device
{
public:
    ////////////////////
    device() noexcept = default;

    ////////////////////
    bool is_valid() const noexcept { return !!dev_; }
    explicit operator bool() const noexcept { return is_valid(); }

    device parent() const;
    device parent(std::string_view subsystem, std::string_view devtype = {}) const;

    std::string subsystem() const noexcept;
    std::string   devtype() const noexcept;
    std::string   syspath() const noexcept;
    std::string   sysname() const noexcept;
    std::string    sysnum() const noexcept;
    std::string   devnode() const noexcept;
    std::string  property(std::string_view) const noexcept;
    std::string    driver() const noexcept;
    enum action    action() const noexcept;
    std::string sysattr(std::string_view) const noexcept;
    bool has_tag(std::string_view) const noexcept;

private:
    ////////////////////
    std::unique_ptr<impl::udev_device, impl::device_deleter> dev_;

    explicit device(impl::udev_device* dev) noexcept : dev_{dev} { }
    device(impl::udev*, std::string_view);

    friend class enumerate;
    friend class monitor;
};

}

////////////////////////////////////////////////////////////////////////////////
#endif
