////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2017-2021 Dimitry Ishenko
// Contact: dimitry (dot) ishenko (at) (gee) mail (dot) com
//
// Distributed under the GNU GPL license. See the LICENSE.md file for details.

////////////////////////////////////////////////////////////////////////////////
#include "udev++/monitor.hpp"

#include <cerrno>
#include <system_error>

#include <poll.h>

////////////////////////////////////////////////////////////////////////////////
namespace impl
{

// move udev stuff into impl namespace
#include <libudev.h>

void monitor_deleter::operator()(udev_monitor* x) { udev_monitor_unref(x); }

}

////////////////////////////////////////////////////////////////////////////////
namespace
{

void throw_on(int code)
{
    if(code) throw std::system_error{
        std::error_code{ code < 0 ? -code : code, std::generic_category() }
    };
}

}

////////////////////////////////////////////////////////////////////////////////
namespace udev
{

////////////////////////////////////////////////////////////////////////////////
monitor::monitor(udev ctx) :
    udev_{ std::move(ctx) },
    mon_{ impl::udev_monitor_new_from_netlink(udev_.get(), "udev") }
{
    if(!mon_) throw std::system_error{
        std::error_code{ errno, std::generic_category() }
    };
}

////////////////////////////////////////////////////////////////////////////////
void monitor::match_device(const string& subsystem, const string& type)
{
    throw_on(impl::udev_monitor_filter_add_match_subsystem_devtype(
        mon_.get(), subsystem.data(), type.size() ? type.data() : nullptr
    ));
}

////////////////////////////////////////////////////////////////////////////////
void monitor::match_tag(const string& name)
{
    throw_on(impl::udev_monitor_filter_add_match_tag(
        mon_.get(), name.data()
    ));
}

////////////////////////////////////////////////////////////////////////////////
device monitor::try_get_for_(const monitor::msec& time)
{
    if(!active())
    {
        throw_on(impl::udev_monitor_enable_receiving(mon_.get()));

        fd_ = impl::udev_monitor_get_fd(mon_.get());
        if(fd_ < 0) throw_on(fd_);
    }

    pollfd fd{ fd_, POLLIN, 0 };

    auto count = ::poll(&fd, 1,
        time == msec::max() ? -1 : static_cast<int>(time.count())
    );
    if(count == -1) throw std::system_error{
        std::error_code{ errno, std::generic_category() }
    };

    return fd.events & fd.revents
        ? device{ udev_, impl::udev_monitor_receive_device(mon_.get()) }
        : device{ };
}

////////////////////////////////////////////////////////////////////////////////
}
