/********************************************************************/
/*                      Copyright 2026                              */
/*           Created and Maintained by Thomas Preisner              */
/********************************************************************/
#pragma once

#include <string>

namespace Messages {

struct ApplyStatus {
    std::string status_effect_name;
};

struct ClearStatus {
    std::string status_effect_name;
};

} // namespace Messages
