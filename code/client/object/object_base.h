/********************************************************************/
/*                      Copyright 2026                              */
/*           Created and Maintained by Thomas Preisner              */
/********************************************************************/
#pragma once
//  The ObjectBase class exist to provide a common base interface 
//      for all objects in the game. From static walls to the player character.
#ifdef _WIN32 // Windows
    #include <guiddef.h>
#else // Linux
    #include <uuid/uuid.h>
#endif

#include <chrono>
#include <string>

#include <Eigen/Dense>

enum class ObjectType {
    STATIC,
    DYNAMIC,
};

class ObjectBase {
 public:
    virtual void on_update(const std::chrono::milliseconds& dt) = 0;

 protected:
    ObjectBase(ObjectType type);
    ObjectType m_objectType;
    Eigen::Vector3d m_position;
    Eigen::Vector3d m_lookDir;

    std::string m_graphics_uuid = ""; // A uuid link to the graphical representation of the character.

 private:
    ObjectBase();  // No one should create this class like this
};
