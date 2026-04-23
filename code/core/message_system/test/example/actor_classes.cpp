/********************************************************************/
/*                      Copyright 2026                              */
/*           Created and Maintained by Thomas Preisner              */
/********************************************************************/
#include "actor_classes.h"

////////////////////////////////////////////////////////
Civilian::Civilian(core::MessageSwitchboard& switchboard) {
    m_accident_pub = std::make_shared<core::MessagePublisher<CarAccident>>(switchboard);
    m_cat_in_tree_pub = std::make_shared<core::MessagePublisher<CatStuckInTree>>(switchboard);
}

bool Civilian::send_car_accident() {
    Location address = {"10\'6\" Overpass, Storrow Drive", "Boston", "MA"};
    CarAccident car_accident;
    car_accident.location = address;
    car_accident.patient_name = "Lowe, Clarence";
    car_accident.car_make = "Box Truck";
    car_accident.car_model = "U-Haul";

    m_sent_messages.push(std::make_shared<CarAccident>(car_accident));
    return m_accident_pub->publish_message(car_accident);
}

bool Civilian::send_cat_stuck_in_tree() {
    Location address = {"87 Oak Tree Lane", "Somerville", "MA"};
    CatStuckInTree cat_in_tree;
    cat_in_tree.location = address;
    cat_in_tree.cat_name = "Mittens";

    m_sent_messages.push(std::make_shared<CatStuckInTree>(cat_in_tree));
    return m_cat_in_tree_pub->publish_message(cat_in_tree);
}

////////////////////////////////////////////////////////
HomeOwner::HomeOwner(core::MessageSwitchboard& switchboard) {
    m_fire_pub = std::make_shared<core::MessagePublisher<FireReport>>(switchboard);
    m_medical_pub = std::make_shared<core::MessagePublisher<MedicalEvent>>(switchboard);
}

bool HomeOwner::send_fire_report() {
    Location address = {"10 Adams Road", "Milton", "MA"};
    std::vector<std::string> residents = {"Tom Brady", "Larry Bird", "Zdeno Chara", "Bobby Orr"};
    FireReport fire_report;
    fire_report.location = address;
    fire_report.occupants = residents;

    m_sent_messages.push(std::make_shared<FireReport>(fire_report));
    return m_fire_pub->publish_message(fire_report);
}

bool HomeOwner::send_medical_event() {
    Location address = {"17 Forbes Street", "Medford", "MA"};
    MedicalEvent medical_event;
    medical_event.location = address;
    medical_event.patient_name = "Justin Case";

    m_sent_messages.push(std::make_shared<MedicalEvent>(medical_event));
    return m_medical_pub->publish_message(medical_event);
}

////////////////////////////////////////////////////////
StoreOwner::StoreOwner(core::MessageSwitchboard& switchboard) {
    m_robbery_pub = std::make_shared<core::MessagePublisher<Robbery>>(switchboard);
    m_suspect_pub = std::make_shared<core::MessagePublisher<SuspectDescription>>(switchboard);
    m_interview_sub = std::make_shared<core::MessageSubscriber<Interview>>(switchboard);

    m_interview_sub->register_callback([this](const Interview& msg) {
        receive_interview_request(msg);
    });
}

bool StoreOwner::send_robbery_report() {
    Location address = {"7534 State Street", "Boston", "MA"};
    Robbery robbery_report;
    robbery_report.location = address;

    m_sent_messages.push(std::make_shared<Robbery>(robbery_report));
    return m_robbery_pub->publish_message(robbery_report);
}

bool StoreOwner::send_suspect_description(const int case_number) {
    SuspectDescription suspect_desc;
    suspect_desc.case_number = case_number;
    suspect_desc.height = 54.5f; //inches
    suspect_desc.clothing = "White and black striped clothes, Black and yellow cape, Black hat, Red Tie";
    suspect_desc.identifiers = "Red hair, Mask, Says \'robble, robble\'";

    m_sent_messages.push(std::make_shared<SuspectDescription>(suspect_desc));
    return m_suspect_pub->publish_message(suspect_desc);
}

void StoreOwner::receive_interview_request(const Interview& msg) {
    m_received_messages.push(std::make_shared<Interview>(msg));

    send_suspect_description(msg.case_number);
}

////////////////////////////////////////////////////////
PoliceCar::PoliceCar(core::MessageSwitchboard& switchboard) {
    m_interview_pub = std::make_shared<core::MessagePublisher<Interview>>(switchboard);
    m_accident_sub = std::make_shared<core::MessageSubscriber<CarAccident>>(switchboard);
    m_fire_sub = std::make_shared<core::MessageSubscriber<FireReport>>(switchboard);
    m_robbery_sub = std::make_shared<core::MessageSubscriber<Robbery>>(switchboard);
    m_suspect_sub = std::make_shared<core::MessageSubscriber<SuspectDescription>>(switchboard);
    
    m_accident_sub->register_callback([this](const CarAccident& msg) {
        receive_accident_report(msg);
    });
    m_fire_sub->register_callback([this](const FireReport& msg) {
        receive_fire_report(msg);
    });
    m_robbery_sub->register_callback([this](const Robbery& msg) {
        receive_robbery_report(msg);
    });
    m_suspect_sub->register_callback([this](const SuspectDescription& msg) {
        receive_suspect_decription(msg);
    });
}

bool PoliceCar::send_interview_request(const int case_number) {
    Interview interview_request;
    interview_request.case_number = case_number;

    m_sent_messages.push(std::make_shared<Interview>(interview_request));
    return m_interview_pub->publish_message(interview_request);
}

void PoliceCar::receive_accident_report(const CarAccident& msg) {
    m_received_messages.push(std::make_shared<CarAccident>(msg));
}

void PoliceCar::receive_fire_report(const FireReport& msg) {
    m_received_messages.push(std::make_shared<FireReport>(msg));
}

void PoliceCar::receive_robbery_report(const Robbery& msg) {
    m_received_messages.push(std::make_shared<Robbery>(msg));
    send_interview_request(case_number);
}

void PoliceCar::receive_suspect_decription(const SuspectDescription& msg) {
    m_received_messages.push(std::make_shared<SuspectDescription>(msg));
}

////////////////////////////////////////////////////////
FireEngine::FireEngine(core::MessageSwitchboard& switchboard) {
    m_cat_in_tree_sub = std::make_shared<core::MessageSubscriber<CatStuckInTree>>(switchboard);
    m_fire_sub = std::make_shared<core::MessageSubscriber<FireReport>>(switchboard);
    m_medical_sub = std::make_shared<core::MessageSubscriber<MedicalEvent>>(switchboard);

    m_cat_in_tree_sub->register_callback([this](const CatStuckInTree& msg) {
        receive_cat_stuck_in_tree(msg);
    });
    m_fire_sub->register_callback([this](const FireReport& msg) {
        receive_fire_report(msg);
    });
    m_medical_sub->register_callback([this](const MedicalEvent& msg) {
        receive_medical_event(msg);
    });
}

void FireEngine::receive_cat_stuck_in_tree(const CatStuckInTree& msg) {
    m_received_messages.push(std::make_shared<CatStuckInTree>(msg));
}

void FireEngine::receive_fire_report(const FireReport& msg) {
    m_received_messages.push(std::make_shared<FireReport>(msg));
}

void FireEngine::receive_medical_event(const MedicalEvent& msg) {
    m_received_messages.push(std::make_shared<MedicalEvent>(msg));
}

////////////////////////////////////////////////////////
Ambulance::Ambulance(core::MessageSwitchboard& switchboard) {
    m_accident_sub = std::make_shared<core::MessageSubscriber<CarAccident>>(switchboard);
    m_fire_sub = std::make_shared<core::MessageSubscriber<FireReport>>(switchboard);
    m_medical_sub = std::make_shared<core::MessageSubscriber<MedicalEvent>>(switchboard);

    m_accident_sub->register_callback([this](const CarAccident& msg) {
        receive_accident_report(msg);
    });
    m_fire_sub->register_callback([this](const FireReport& msg) {
        receive_fire_report(msg);
    });
    m_medical_sub->register_callback([this](const MedicalEvent& msg) {
        receive_medical_event(msg);
    });
}

void Ambulance::receive_accident_report(const CarAccident& msg) {
    m_received_messages.push(std::make_shared<CarAccident>(msg));
}

void Ambulance::receive_fire_report(const FireReport& msg) {
    m_received_messages.push(std::make_shared<FireReport>(msg));
}

void Ambulance::receive_medical_event(const MedicalEvent& msg) {
    m_received_messages.push(std::make_shared<MedicalEvent>(msg));
}

////////////////////////////////////////////////////////
Operator::Operator() {
    m_switchboard = std::make_shared<core::MessageSwitchboard>();
}
