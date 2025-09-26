////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2017-2025 Dimitry Ishenko
// Contact: dimitry (dot) ishenko (at) (gee) mail (dot) com
//
// Distributed under the GNU GPL license. See the LICENSE.md file for details.

////////////////////////////////////////////////////////////////////////////////
#ifndef UDEV_ENUMERATE_HPP
#define UDEV_ENUMERATE_HPP

////////////////////////////////////////////////////////////////////////////////
#include "udev++/device.hpp"
#include "udev++/udev.hpp"

#include <memory>
#include <string_view>
#include <vector>

namespace impl
{
struct udev_enumerate;
struct enumerate_delete { void operator()(udev_enumerate*); };
}

////////////////////////////////////////////////////////////////////////////////
namespace udev
{

/**
 * @class udev::enumerate
 * @brief Enumerates udev devices.
 *
 * Call any of the `match_*()` functions to include or exclude specific devices.
 * Then, call the `get()` function to retrieve a list of matching devices.
 *
 * @sa udev::device
 */
class enumerate
{
public:
    ////////////////////
    enumerate();

    ////////////////////
    void match_subsystem(std::string_view);
    void nomatch_subsystem(std::string_view);

    void match_sysattr(std::string_view name, std::string_view value = {});
    void nomatch_sysattr(std::string_view name, std::string_view value = {});

    void match_property(std::string_view name, std::string_view value = {});

    void match_sysname(std::string_view);
    void match_tag(std::string_view);

    void match_parent(const device&);

    std::vector<device> get();

private:
    ////////////////////
    udev udev_;
    std::unique_ptr<impl::udev_enumerate, impl::enumerate_delete> enum_;
};

}

////////////////////////////////////////////////////////////////////////////////
#endif
