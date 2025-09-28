////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2017-2025 Dimitry Ishenko
// Contact: dimitry (dot) ishenko (at) (gee) mail (dot) com
//
// Distributed under the GNU GPL license. See the LICENSE.md file for details.

////////////////////////////////////////////////////////////////////////////////
#ifndef UDEV_MONITOR_HPP
#define UDEV_MONITOR_HPP

////////////////////////////////////////////////////////////////////////////////
#include "udev++/device.hpp"
#include "udev++/udev.hpp"

#include <asio/io_context.hpp>
#include <asio/posix/stream_descriptor.hpp>
#include <functional>
#include <memory>
#include <string_view>

namespace impl
{
struct udev_monitor;
struct monitor_delete { void operator()(udev_monitor*); };
}

////////////////////////////////////////////////////////////////////////////////
namespace udev
{

using monitor_callback = std::function<void(device)>;

/**
 * @class udev::monitor
 * @brief Monitors udev device events.
 *
 * Call one or more of the `match_*()` functions to filter for specific devices,
 * and any of the `on_device_*()` functions to set callbacks for specific
 * events. Then, call the `enable()` function to begin monitoring.
 *
 * @sa udev::device
 */
class monitor
{
public:
    ////////////////////
    explicit monitor(asio::io_context&);
    ~monitor();

    ////////////////////
    void match_device(std::string_view subsystem, std::string_view type = {});
    void match_tag(std::string_view);
    void enable();

    void on_device_added(monitor_callback cb) { added_cb_ = std::move(cb); }
    void on_device_removed(monitor_callback cb) { removed_cb_ = std::move(cb); }
    void on_device_other(monitor_callback cb) { other_cb_ = std::move(cb); }

private:
    ////////////////////
    udev udev_;
    std::unique_ptr<impl::udev_monitor, impl::monitor_delete> mon_;
    asio::posix::stream_descriptor desc_;

    monitor_callback added_cb_, removed_cb_, other_cb_;
    void async_wait();
};

}

////////////////////////////////////////////////////////////////////////////////
#endif
