/********************************************************************/
/*                      Copyright 2026                              */
/*           Created and Maintained by Thomas Preisner              */
/********************************************************************/
#pragma once

#include <memory>
#include "actor_classes.h"

class ScenarioSimulation {
public:
    ScenarioSimulation() {
        m_operator = std::make_shared<Operator>();
        m_civilian = std::make_shared<Civilian>(*(m_operator->m_switchboard));
        m_home_owner = std::make_shared<HomeOwner>(*(m_operator->m_switchboard));
        m_store_owner = std::make_shared<StoreOwner>(*(m_operator->m_switchboard));
        m_police_car = std::make_shared<PoliceCar>(*(m_operator->m_switchboard));
        m_fire_engine = std::make_shared<FireEngine>(*(m_operator->m_switchboard));
        m_ambulance = std::make_shared<Ambulance>(*(m_operator->m_switchboard));
    }

    bool start_robbery() {
        if (m_store_owner) {
            return m_store_owner->send_robbery_report();
        }
        return false;
    }

    bool start_car_accident() {
        if (m_civilian) {
            return m_civilian->send_car_accident();
        }
        return false;
    }

    bool start_cat_stuck_in_tree() {
        if (m_civilian) {
            return m_civilian->send_cat_stuck_in_tree();
        }
        return false;
    }

    bool start_fire_report() {
        if (m_home_owner) {
            return m_home_owner->send_fire_report();
        }
        return false;
    }

    bool start_medical_emergency() {
        if (m_home_owner) {
            return m_home_owner->send_medical_event();
        }
        return false;
    }

    // These are public to make the access for the unit test functions easier
    std::shared_ptr<Operator> m_operator;
    std::shared_ptr<Civilian> m_civilian;
    std::shared_ptr<HomeOwner> m_home_owner;
    std::shared_ptr<StoreOwner> m_store_owner;
    std::shared_ptr<PoliceCar> m_police_car;
    std::shared_ptr<FireEngine> m_fire_engine;
    std::shared_ptr<Ambulance> m_ambulance;
};
