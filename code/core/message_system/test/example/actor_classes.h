/********************************************************************/
/*                      Copyright 2026                              */
/*           Created and Maintained by Thomas Preisner              */
/********************************************************************/
#pragma once

#include <memory>
#include <queue>
#include <string>
#include <vector>
#include "message_types.h"
#include "code/core/message_system/message_publisher.h"
#include "code/core/message_system/message_subscriber.h"
#include "code/core/message_system/message_switchboard.h"


////////////////////////////////////////////////////////
class BaseActor {
public:
    std::queue<std::shared_ptr<MessageData>> m_sent_messages;
    std::queue<std::shared_ptr<MessageData>> m_received_messages;
};

////////////////////////////////////////////////////////
class Civilian : public BaseActor {
public:
    explicit Civilian(core::MessageSwitchboard& switchboard);

    bool send_car_accident();
    bool send_cat_stuck_in_tree();
private:
    std::shared_ptr<core::MessagePublisher<CarAccident>> m_accident_pub;
    std::shared_ptr<core::MessagePublisher<CatStuckInTree>> m_cat_in_tree_pub;
};

////////////////////////////////////////////////////////
class HomeOwner : public BaseActor {
public:
    explicit HomeOwner(core::MessageSwitchboard& switchboard);

    bool send_fire_report();
    bool send_medical_event();
private:
    std::shared_ptr<core::MessagePublisher<FireReport>> m_fire_pub;
    std::shared_ptr<core::MessagePublisher<MedicalEvent>> m_medical_pub;
};

////////////////////////////////////////////////////////
class StoreOwner : public BaseActor {
public:
    explicit StoreOwner(core::MessageSwitchboard& switchboard);

    bool send_robbery_report();
private:
    bool send_suspect_description(const int case_number);
    void receive_interview_request(const Interview& msg);

    std::shared_ptr<core::MessagePublisher<Robbery>> m_robbery_pub;
    std::shared_ptr<core::MessagePublisher<SuspectDescription>> m_suspect_pub;
    std::shared_ptr<core::MessageSubscriber<Interview>> m_interview_sub;
};

////////////////////////////////////////////////////////
class PoliceCar : public BaseActor {
public:
    explicit PoliceCar(core::MessageSwitchboard& switchboard);

    int get_case_number() const { return case_number; }
private:
    bool send_interview_request(const int case_number);
    void receive_accident_report(const CarAccident& msg);
    void receive_fire_report(const FireReport& msg);
    void receive_robbery_report(const Robbery& msg);
    void receive_suspect_decription(const SuspectDescription& msg);

    int case_number = 90210;
    std::shared_ptr<core::MessagePublisher<Interview>> m_interview_pub;
    std::shared_ptr<core::MessageSubscriber<CarAccident>> m_accident_sub;
    std::shared_ptr<core::MessageSubscriber<FireReport>> m_fire_sub;
    std::shared_ptr<core::MessageSubscriber<Robbery>> m_robbery_sub;
    std::shared_ptr<core::MessageSubscriber<SuspectDescription>> m_suspect_sub;
};

////////////////////////////////////////////////////////
class FireEngine : public BaseActor {
public:
    explicit FireEngine(core::MessageSwitchboard& switchboard);

private:
    void receive_cat_stuck_in_tree(const CatStuckInTree& msg);
    void receive_fire_report(const FireReport& msg);
    void receive_medical_event(const MedicalEvent& msg);

    std::shared_ptr<core::MessageSubscriber<CatStuckInTree>> m_cat_in_tree_sub;
    std::shared_ptr<core::MessageSubscriber<FireReport>> m_fire_sub;
    std::shared_ptr<core::MessageSubscriber<MedicalEvent>> m_medical_sub;
};

////////////////////////////////////////////////////////
class Ambulance : public BaseActor {
public:
    explicit Ambulance(core::MessageSwitchboard& switchboard);

private:
    void receive_accident_report(const CarAccident& msg);
    void receive_fire_report(const FireReport& msg);
    void receive_medical_event(const MedicalEvent& msg);

    std::shared_ptr<core::MessageSubscriber<CarAccident>> m_accident_sub;
    std::shared_ptr<core::MessageSubscriber<FireReport>> m_fire_sub;
    std::shared_ptr<core::MessageSubscriber<MedicalEvent>> m_medical_sub;
};

////////////////////////////////////////////////////////
class Operator {
public:
    Operator();

    std::shared_ptr<core::MessageSwitchboard> m_switchboard;
};
