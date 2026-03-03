/********************************************************************/
/*                      Copyright 2026                              */
/*           Created and Maintained by Thomas Preisner              */
/********************************************************************/

#include "character_base.h"

void CharacterBase::on_update(const std::chrono::milliseconds& dt) {
    m_behavior_tree.tickOnce();
}
