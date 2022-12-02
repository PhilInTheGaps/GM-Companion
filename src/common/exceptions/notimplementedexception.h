#pragma once

#include <QtGlobal>
#include <stdexcept>

class NotImplementedException : public std::logic_error
{
public:
    NotImplementedException() : std::logic_error("Function not yet implemented"){};
    ~NotImplementedException() override = default;
    Q_DISABLE_COPY_MOVE(NotImplementedException);
};
