////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2025 Dimitry Ishenko
// Contact: dimitry (dot) ishenko (at) (gee) mail (dot) com
//
// Distributed under the GNU GPL license. See the LICENSE.md file for details.

////////////////////////////////////////////////////////////////////////////////
#include "udev++/error.hpp"
#include <cstring> // std::strerror

////////////////////////////////////////////////////////////////////////////////
namespace udev
{

////////////////////////////////////////////////////////////////////////////////
namespace
{

class error_category_ : public std::error_category
{
public:
    ////////////////////
    virtual const char* name() const noexcept override { return "udev"; }
    virtual std::string message(int ev) const override { return std::strerror(ev); }

    virtual std::error_condition default_error_condition(int ev) const noexcept override
    {
        return std::system_category().default_error_condition(ev);
    }
};

}

////////////////////////////////////////////////////////////////////////////////
const std::error_category& error_category()
{
    static error_category_ instance;
    return instance;
}

////////////////////////////////////////////////////////////////////////////////
}
