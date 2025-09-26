////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2017-2025 Dimitry Ishenko
// Contact: dimitry (dot) ishenko (at) (gee) mail (dot) com
//
// Distributed under the GNU GPL license. See the LICENSE.md file for details.

////////////////////////////////////////////////////////////////////////////////
#ifndef UDEV_UDEV_HPP
#define UDEV_UDEV_HPP

////////////////////////////////////////////////////////////////////////////////
namespace impl { struct udev; }

////////////////////////////////////////////////////////////////////////////////
namespace udev
{

/**
 * @class udev::udev
 * @brief Manages udev context.
 *
 * Provides access to udev functionality for the `enumerate` and `monitor`
 * classes.
 *
 * @sa udev::enumerate, udev::monitor
 */
class udev
{
public:
    ////////////////////
    udev();
    ~udev();

    udev(const udev&) noexcept;
    udev(udev&&) noexcept;

    udev& operator=(udev) noexcept;

    ////////////////////
    static udev instance();

private:
    ////////////////////
    impl::udev* udev_;

    friend class enumerate;
    friend class monitor;
};

}

////////////////////////////////////////////////////////////////////////////////
#endif
