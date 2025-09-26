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

#include <chrono>
#include <memory>
#include <string_view>

namespace impl
{

struct udev_monitor;
struct monitor_deleter { void operator()(udev_monitor*); };

}

////////////////////////////////////////////////////////////////////////////////
namespace udev
{

////////////////////////////////////////////////////////////////////////////////
/**
 * @class udev::monitor
 * @brief Monitors udev device events.
 *
 * Call one or more of the `match_*()` functions to filter for specific devices.
 * Then, call any of the `get()` or `try_get*()` functions to monitor for device
 * events (addition, removal, etc).
 *
 * @sa udev::device
 */
class monitor
{
public:
    ////////////////////
    monitor();

    monitor(monitor&&) noexcept;
    monitor& operator=(monitor) noexcept;

    ////////////////////
    void match_device(std::string_view subsystem, std::string_view type = {});
    void match_tag(std::string_view);

    // NB: first call to any of the try_*() functions will put monitor into
    // "active" state and further calls to match_*() functions will be ignored
    bool is_active() const noexcept { return fd_ != -1; }

    device get(); // inifite wait
    device try_get(); // no wait

    template<typename Rep, typename Period>
    device try_get_for(const std::chrono::duration<Rep, Period>&);

    template<typename Clock, typename Duration>
    device try_get_until(const std::chrono::time_point<Clock, Duration>&);

private:
    ////////////////////
    udev udev_;
    std::unique_ptr<impl::udev_monitor, impl::monitor_deleter> mon_;
    int fd_ = -1;

    using msec = std::chrono::milliseconds;
    device try_get_for_(const msec&);
};

////////////////////////////////////////////////////////////////////////////////
inline device monitor::get() { return try_get_for_(msec::max()); }
inline device monitor::try_get() { return try_get_for_(msec::zero()); }

template<typename Rep, typename Period>
inline device monitor::try_get_for(const std::chrono::duration<Rep, Period>& d)
{
    return try_get_for_(std::chrono::duration_cast<msec>(d));
}

template<typename Clock, typename Duration>
inline device monitor::try_get_until(const std::chrono::time_point<Clock, Duration>& tp)
{
    auto now = Clock::now();
    return try_get_for(tp - (tp < now ? tp : now));
}

////////////////////////////////////////////////////////////////////////////////
}

////////////////////////////////////////////////////////////////////////////////
#endif
