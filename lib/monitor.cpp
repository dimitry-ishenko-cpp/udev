////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2017-2025 Dimitry Ishenko
// Contact: dimitry (dot) ishenko (at) (gee) mail (dot) com
//
// Distributed under the GNU GPL license. See the LICENSE.md file for details.

////////////////////////////////////////////////////////////////////////////////
#include "udev++/error.hpp"
#include "udev++/monitor.hpp"

#include <poll.h>

namespace impl
{
#include <libudev.h>
void monitor_deleter::operator()(udev_monitor* mon) { udev_monitor_unref(mon); }
}

////////////////////////////////////////////////////////////////////////////////
namespace udev
{

////////////////////////////////////////////////////////////////////////////////
monitor::monitor() : udev_{udev::instance()},
    mon_{ impl::udev_monitor_new_from_netlink(&*udev_.udev_, "udev") }
{
    if (!mon_) throw error{"monitor::monitor()"};
}

monitor::monitor(monitor&& rhs) noexcept :
    udev_{std::move(rhs.udev_)}, mon_{std::move(rhs.mon_)}, fd_{rhs.fd_}
{
    rhs.fd_ = -1;
}

monitor& monitor::operator=(monitor rhs) noexcept
{
    std::swap(udev_, rhs.udev_);
    std::swap(mon_, rhs.mon_);
    std::swap(fd_, rhs.fd_);
    return (*this);
}

////////////////////////////////////////////////////////////////////////////////
void monitor::match_device(std::string_view subsystem, std::string_view type)
{
    auto p = type.size() ? type.data() : nullptr;
    auto code = impl::udev_monitor_filter_add_match_subsystem_devtype(&*mon_, subsystem.data(), p);
    if (code) throw error{"monitor::match_device"};
}

void monitor::match_tag(std::string_view name)
{
    auto code = impl::udev_monitor_filter_add_match_tag(&*mon_, name.data());
    if (code) throw error{"monitor::match_tag"};
}

////////////////////////////////////////////////////////////////////////////////
device monitor::try_get_for_(const monitor::msec& time)
{
    if(!is_active())
    {
        auto code = impl::udev_monitor_enable_receiving(&*mon_);
        if (code) throw error{"monitor::try_get_for_()"};

        fd_ = impl::udev_monitor_get_fd(&*mon_);
        if(fd_ < 0) throw error{fd_, "monitor::try_get_for_()"};
    }

    pollfd fd{fd_, POLLIN, 0};

    auto count = ::poll(&fd, 1,
        time == msec::max() ? -1 : static_cast<int>(time.count())
    );
    if(count == -1) throw error{"monitor::try_get_for_()"};

    return fd.events & fd.revents
        ? device{impl::udev_monitor_receive_device(&*mon_)}
        : device{};
}

////////////////////////////////////////////////////////////////////////////////
}
