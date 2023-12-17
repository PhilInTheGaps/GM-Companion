#pragma once

#include <QFlags>

namespace Files
{

enum class Option
{
    None = 0,
    AllowCache = 1,
    LowPriority = 2
};
Q_DECLARE_FLAGS(Options, Files::Option)

} // namespace Files

Q_DECLARE_OPERATORS_FOR_FLAGS(Files::Options)
