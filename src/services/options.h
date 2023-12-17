#pragma once

namespace Services
{

enum class Option
{
    None = 0,
    Authenticated = 1,
    LowPriority = 2
};

Q_DECLARE_FLAGS(Options, Services::Option)

} // namespace Services

Q_DECLARE_OPERATORS_FOR_FLAGS(Services::Options)
