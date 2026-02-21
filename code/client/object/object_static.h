/********************************************************************/
/*                      Copyright 2026                              */
/*           Created and Maintained by Thomas Preisner              */
/********************************************************************/
#pragma once
//  The ObjectStatic class exist to provide a common base interface 
//      for the enemies, player characters, and non-player characters.

#include "object_base.h"

class ObjectStatic : public ObjectBase {
 protected:
    ObjectStatic();
};
