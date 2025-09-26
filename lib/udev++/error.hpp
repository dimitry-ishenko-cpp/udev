////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2025 Dimitry Ishenko
// Contact: dimitry (dot) ishenko (at) (gee) mail (dot) com
//
// Distributed under the GNU GPL license. See the LICENSE.md file for details.

////////////////////////////////////////////////////////////////////////////////
#ifndef UDEV_ERROR_HPP
#define UDEV_ERROR_HPP

#include <cerrno>
#include <cmath>
#include <string>
#include <system_error>

////////////////////////////////////////////////////////////////////////////////
namespace udev
{

/**
 * @fn udev::error_category
 * @brief Represents category for udev errors.
 */
const std::error_category& error_category();

/**
 * @class udev::error
 * @brief Represents udev errors.
 */
class error : public std::system_error
{
public:
    ////////////////////
    explicit error(int ev) : std::system_error{std::abs(ev), error_category()} { }
    error(int ev, const char* msg) : std::system_error{std::abs(ev), error_category(), msg} { }
    error(int ev, const std::string& msg) : std::system_error{std::abs(ev), error_category(), msg} { }

    explicit error() : std::system_error{errno, error_category()} { }
    explicit error(const char* msg) : std::system_error{errno, error_category(), msg} { }
    explicit error(const std::string& msg) : std::system_error{errno, error_category(), msg} { }
};

}

////////////////////////////////////////////////////////////////////////////////
#endif
