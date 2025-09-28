////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2017-2025 Dimitry Ishenko
// Contact: dimitry (dot) ishenko (at) (gee) mail (dot) com
//
// Distributed under the GNU GPL license. See the LICENSE.md file for details.

////////////////////////////////////////////////////////////////////////////////
#include "udev++/error.hpp"
#include "udev++/monitor.hpp"

namespace impl
{
#include <libudev.h>
void monitor_delete::operator()(udev_monitor* mon) { udev_monitor_unref(mon); }
}

////////////////////////////////////////////////////////////////////////////////
namespace udev
{

////////////////////////////////////////////////////////////////////////////////
monitor::monitor(asio::io_context& io) :
    udev_{udev::instance()},
    mon_ {impl::udev_monitor_new_from_netlink(&*udev_.udev_, "udev")},
    desc_{io}
{
    if (!mon_) throw error{"monitor::monitor()"};

    auto fd = impl::udev_monitor_get_fd(&*mon_);
    if (fd < 0) throw error{fd, "monitor::monitor()"};
    desc_.assign(fd);

    async_wait();
}

monitor::~monitor() { desc_.release(); }

////////////////////////////////////////////////////////////////////////////////
void monitor::match_device(std::string_view subsystem, std::string_view type)
{
    auto s = type.size() ? type.data() : nullptr;
    auto res = impl::udev_monitor_filter_add_match_subsystem_devtype(&*mon_, subsystem.data(), s);
    if (res) throw error{"monitor::match_device"};
}

void monitor::match_tag(std::string_view name)
{
    auto res = impl::udev_monitor_filter_add_match_tag(&*mon_, name.data());
    if (res) throw error{"monitor::match_tag"};
}

////////////////////////////////////////////////////////////////////////////////
void monitor::enable()
{
    auto res = impl::udev_monitor_enable_receiving(&*mon_);
    if (res) throw error{"monitor::enable()"};
}

////////////////////////////////////////////////////////////////////////////////
void monitor::async_wait()
{
    desc_.async_wait(desc_.wait_read, [&](std::error_code ec)
    {
        if (!ec)
        {
            auto dev = device{impl::udev_monitor_receive_device(&*mon_)};
            switch (dev.action())
            {
            case added: if (added_cb_) added_cb_(std::move(dev)); break;
            case removed: if (removed_cb_) removed_cb_(std::move(dev)); break;
            case other: if (other_cb_) other_cb_(std::move(dev)); break;
            }

            async_wait();
        }
    });
}

////////////////////////////////////////////////////////////////////////////////
}
