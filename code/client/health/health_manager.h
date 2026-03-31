/********************************************************************/
/*                      Copyright 2026                              */
/*           Created and Maintained by Thomas Preisner              */
/********************************************************************/
#pragma once

// The health manager is a class that manages incoming damage to a character
//  processing the damage and applying status effects. It consists of series of 
//  priority based modules that process the damage. These filter the damage and 
//  strips inactive items before finally applying it to the character.
// For example, the character has poison immunity and armor, and a melee attack that
//  has a status effect is applied. The armor takes a portion of the melee and the 
//  poison resistence strips the poison status effect from the incoming damage.
class HealthManager {

};


