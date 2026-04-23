#include <gtest/gtest.h>

#include <vector>
#include <string>
#include "code/core/message_system/test/example/actor_classes.h"
#include "code/core/message_system/test/example/message_types.h"
#include "code/core/message_system/test/example/simulation.h"

////////////////////////////////////////////////////////
// The intricacies of the failure cases should be covered in 
//  the unit tests for the classes. This just tests the example and 
//  that it operates as expected.

// The Robbery case is the most verbose of the tests, redundant tests 
//  (i.e. test if the Simulation created) are not retested because of noise

////////////////////////////////////////////////////////
TEST(MessageSystem_Example, MessageSystem_Example_Robbery) {
    ScenarioSimulation sim;

    EXPECT_TRUE(sim.m_operator);
    EXPECT_TRUE(sim.m_civilian);
    EXPECT_TRUE(sim.m_home_owner);
    EXPECT_TRUE(sim.m_store_owner);
    EXPECT_TRUE(sim.m_police_car);
    EXPECT_TRUE(sim.m_fire_engine);
    EXPECT_TRUE(sim.m_ambulance);

    // Start the message transfer
    EXPECT_TRUE(sim.start_robbery());

    EXPECT_EQ(sim.m_civilian->m_sent_messages.size(), 0);
    EXPECT_EQ(sim.m_civilian->m_received_messages.size(), 0);
    EXPECT_EQ(sim.m_home_owner->m_sent_messages.size(), 0);
    EXPECT_EQ(sim.m_home_owner->m_received_messages.size(), 0);
    EXPECT_EQ(sim.m_store_owner->m_sent_messages.size(), 2);
    EXPECT_EQ(sim.m_store_owner->m_received_messages.size(), 1);
    EXPECT_EQ(sim.m_police_car->m_sent_messages.size(), 1);
    EXPECT_EQ(sim.m_police_car->m_received_messages.size(), 2);
    EXPECT_EQ(sim.m_fire_engine->m_sent_messages.size(), 0);
    EXPECT_EQ(sim.m_fire_engine->m_received_messages.size(), 0);
    EXPECT_EQ(sim.m_ambulance->m_sent_messages.size(), 0);
    EXPECT_EQ(sim.m_ambulance->m_received_messages.size(), 0);

    std::shared_ptr<MessageData> sent = sim.m_store_owner->m_sent_messages.front();
    std::shared_ptr<MessageData> received = sim.m_police_car->m_received_messages.front();
    Robbery* store_robbery = static_cast<Robbery*>(sent.get());
    Robbery* police_robbery = static_cast<Robbery*>(received.get());
    EXPECT_EQ(store_robbery->get_name(), "Robbery");
    EXPECT_EQ(police_robbery->get_name(), "Robbery");
    EXPECT_EQ(store_robbery->location.street_address, "7534 State Street");
    EXPECT_EQ(store_robbery->location.city, "Boston");
    EXPECT_EQ(store_robbery->location.state, "MA");
    EXPECT_EQ(store_robbery->location.street_address, police_robbery->location.street_address);
    EXPECT_EQ(store_robbery->location.city, police_robbery->location.city);
    EXPECT_EQ(store_robbery->location.state, police_robbery->location.state);
    sim.m_store_owner->m_sent_messages.pop();
    sim.m_police_car->m_received_messages.pop();

    sent = sim.m_police_car->m_sent_messages.front();
    received = sim.m_store_owner->m_received_messages.front();
    Interview* police_interview = static_cast<Interview*>(sent.get());
    Interview* store_interview = static_cast<Interview*>(received.get());
    EXPECT_EQ(store_interview->get_name(), "Interview");
    EXPECT_EQ(police_interview->get_name(), "Interview");
    EXPECT_EQ(sim.m_police_car->get_case_number(), 90210);
    EXPECT_EQ(store_interview->case_number, sim.m_police_car->get_case_number());
    EXPECT_EQ(store_interview->case_number, police_interview->case_number);
    sim.m_police_car->m_sent_messages.pop();
    sim.m_store_owner->m_received_messages.pop();

    sent = sim.m_store_owner->m_sent_messages.front();
    received = sim.m_police_car->m_received_messages.front();
    SuspectDescription* store_suspect = static_cast<SuspectDescription*>(sent.get());
    SuspectDescription* police_suspect = static_cast<SuspectDescription*>(received.get());
    EXPECT_EQ(store_suspect->get_name(), "SuspectDescription");
    EXPECT_EQ(police_suspect->get_name(), "SuspectDescription");
    EXPECT_EQ(sim.m_police_car->get_case_number(), 90210);
    EXPECT_EQ(store_suspect->case_number, sim.m_police_car->get_case_number());
    EXPECT_FLOAT_EQ(store_suspect->height, 54.5f);
    EXPECT_EQ(store_suspect->clothing, "White and black striped clothes, Black and yellow cape, Black hat, Red Tie");
    EXPECT_EQ(store_suspect->identifiers, "Red hair, Mask, Says \'robble, robble\'");
    EXPECT_EQ(store_suspect->case_number, police_suspect->case_number);
    EXPECT_FLOAT_EQ(store_suspect->height, police_suspect->height);
    EXPECT_EQ(store_suspect->clothing, police_suspect->clothing);
    EXPECT_EQ(store_suspect->identifiers, police_suspect->identifiers);
    sim.m_store_owner->m_sent_messages.pop();
    sim.m_police_car->m_received_messages.pop();
    
    EXPECT_EQ(sim.m_store_owner->m_sent_messages.size(), 0);
    EXPECT_EQ(sim.m_store_owner->m_received_messages.size(), 0);
    EXPECT_EQ(sim.m_police_car->m_sent_messages.size(), 0);
    EXPECT_EQ(sim.m_police_car->m_received_messages.size(), 0);

    // if the operator is removed, all connections are lost.
    sim.m_operator.reset();
    EXPECT_FALSE(sim.m_operator);
    EXPECT_FALSE(sim.start_robbery());

    EXPECT_EQ(sim.m_civilian->m_sent_messages.size(), 0);
    EXPECT_EQ(sim.m_civilian->m_received_messages.size(), 0);
    EXPECT_EQ(sim.m_home_owner->m_sent_messages.size(), 0);
    EXPECT_EQ(sim.m_home_owner->m_received_messages.size(), 0);
    EXPECT_EQ(sim.m_store_owner->m_sent_messages.size(), 1);   //< sent, but never received by anyone
    EXPECT_EQ(sim.m_store_owner->m_received_messages.size(), 0);
    EXPECT_EQ(sim.m_police_car->m_sent_messages.size(), 0);
    EXPECT_EQ(sim.m_police_car->m_received_messages.size(), 0);
    EXPECT_EQ(sim.m_fire_engine->m_sent_messages.size(), 0);
    EXPECT_EQ(sim.m_fire_engine->m_received_messages.size(), 0);
    EXPECT_EQ(sim.m_ambulance->m_sent_messages.size(), 0);
    EXPECT_EQ(sim.m_ambulance->m_received_messages.size(), 0);
}

////////////////////////////////////////////////////////
TEST(MessageSystem_Example, MessageSystem_Example_CarAccident) {
    ScenarioSimulation sim;

    // Start the message transfer
    EXPECT_TRUE(sim.start_car_accident());

    EXPECT_EQ(sim.m_civilian->m_sent_messages.size(), 1);
    EXPECT_EQ(sim.m_civilian->m_received_messages.size(), 0);
    EXPECT_EQ(sim.m_home_owner->m_sent_messages.size(), 0);
    EXPECT_EQ(sim.m_home_owner->m_received_messages.size(), 0);
    EXPECT_EQ(sim.m_store_owner->m_sent_messages.size(), 0);
    EXPECT_EQ(sim.m_store_owner->m_received_messages.size(), 0);
    EXPECT_EQ(sim.m_police_car->m_sent_messages.size(), 0);
    EXPECT_EQ(sim.m_police_car->m_received_messages.size(), 1);
    EXPECT_EQ(sim.m_fire_engine->m_sent_messages.size(), 0);
    EXPECT_EQ(sim.m_fire_engine->m_received_messages.size(), 0);
    EXPECT_EQ(sim.m_ambulance->m_sent_messages.size(), 0);
    EXPECT_EQ(sim.m_ambulance->m_received_messages.size(), 1);

    std::shared_ptr<MessageData> sent = sim.m_civilian->m_sent_messages.front();
    std::shared_ptr<MessageData> received = sim.m_police_car->m_received_messages.front();
    std::shared_ptr<MessageData> received_two = sim.m_ambulance->m_received_messages.front();
    CarAccident* civilian_report = static_cast<CarAccident*>(sent.get());
    CarAccident* police_report = static_cast<CarAccident*>(received.get());
    CarAccident* ambulance_report = static_cast<CarAccident*>(received_two.get());
    EXPECT_EQ(civilian_report->get_name(), "CarAccident");
    EXPECT_EQ(police_report->get_name(), "CarAccident");
    EXPECT_EQ(ambulance_report->get_name(), "CarAccident");

    EXPECT_EQ(civilian_report->location.street_address, "10\'6\" Overpass, Storrow Drive");
    EXPECT_EQ(civilian_report->location.city, "Boston");
    EXPECT_EQ(civilian_report->location.state, "MA");
    EXPECT_EQ(civilian_report->patient_name, "Lowe, Clarence");
    EXPECT_EQ(civilian_report->car_make, "Box Truck");
    EXPECT_EQ(civilian_report->car_model, "U-Haul");

    EXPECT_EQ(civilian_report->location.street_address, police_report->location.street_address);
    EXPECT_EQ(civilian_report->location.city, police_report->location.city);
    EXPECT_EQ(civilian_report->location.state, police_report->location.state);
    EXPECT_EQ(civilian_report->patient_name, police_report->patient_name);
    EXPECT_EQ(civilian_report->car_make, police_report->car_make);
    EXPECT_EQ(civilian_report->car_model, police_report->car_model);

    EXPECT_EQ(civilian_report->location.street_address, ambulance_report->location.street_address);
    EXPECT_EQ(civilian_report->location.city, ambulance_report->location.city);
    EXPECT_EQ(civilian_report->location.state, ambulance_report->location.state);
    EXPECT_EQ(civilian_report->patient_name, ambulance_report->patient_name);
    EXPECT_EQ(civilian_report->car_make, ambulance_report->car_make);
    EXPECT_EQ(civilian_report->car_model, ambulance_report->car_model);

}

////////////////////////////////////////////////////////
TEST(MessageSystem_Example, MessageSystem_Example_CatInTree) {
    ScenarioSimulation sim;

    // Start the message transfer
    EXPECT_TRUE(sim.start_cat_stuck_in_tree());

    EXPECT_EQ(sim.m_civilian->m_sent_messages.size(), 1);
    EXPECT_EQ(sim.m_civilian->m_received_messages.size(), 0);
    EXPECT_EQ(sim.m_home_owner->m_sent_messages.size(), 0);
    EXPECT_EQ(sim.m_home_owner->m_received_messages.size(), 0);
    EXPECT_EQ(sim.m_store_owner->m_sent_messages.size(), 0);
    EXPECT_EQ(sim.m_store_owner->m_received_messages.size(), 0);
    EXPECT_EQ(sim.m_police_car->m_sent_messages.size(), 0);
    EXPECT_EQ(sim.m_police_car->m_received_messages.size(), 0);
    EXPECT_EQ(sim.m_fire_engine->m_sent_messages.size(), 0);
    EXPECT_EQ(sim.m_fire_engine->m_received_messages.size(), 1);
    EXPECT_EQ(sim.m_ambulance->m_sent_messages.size(), 0);
    EXPECT_EQ(sim.m_ambulance->m_received_messages.size(), 0);

    std::shared_ptr<MessageData> sent = sim.m_civilian->m_sent_messages.front();
    std::shared_ptr<MessageData> received = sim.m_fire_engine->m_received_messages.front();
    CatStuckInTree* civilian_report = static_cast<CatStuckInTree*>(sent.get());
    CatStuckInTree* fire_engine_report = static_cast<CatStuckInTree*>(received.get());
    EXPECT_EQ(civilian_report->get_name(), "CatStuckInTree");
    EXPECT_EQ(fire_engine_report->get_name(), "CatStuckInTree");

    EXPECT_EQ(civilian_report->location.street_address, "87 Oak Tree Lane");
    EXPECT_EQ(civilian_report->location.city, "Somerville");
    EXPECT_EQ(civilian_report->location.state, "MA");
    EXPECT_EQ(civilian_report->cat_name, "Mittens");

    EXPECT_EQ(civilian_report->location.street_address, fire_engine_report->location.street_address);
    EXPECT_EQ(civilian_report->location.city, fire_engine_report->location.city);
    EXPECT_EQ(civilian_report->location.state, fire_engine_report->location.state);
    EXPECT_EQ(civilian_report->cat_name, fire_engine_report->cat_name);
}

////////////////////////////////////////////////////////
TEST(MessageSystem_Example, MessageSystem_Example_FireReport) {
    ScenarioSimulation sim;

    // Start the message transfer
    EXPECT_TRUE(sim.start_fire_report());

    EXPECT_EQ(sim.m_civilian->m_sent_messages.size(), 0);
    EXPECT_EQ(sim.m_civilian->m_received_messages.size(), 0);
    EXPECT_EQ(sim.m_home_owner->m_sent_messages.size(), 1);
    EXPECT_EQ(sim.m_home_owner->m_received_messages.size(), 0);
    EXPECT_EQ(sim.m_store_owner->m_sent_messages.size(), 0);
    EXPECT_EQ(sim.m_store_owner->m_received_messages.size(), 0);
    EXPECT_EQ(sim.m_police_car->m_sent_messages.size(), 0);
    EXPECT_EQ(sim.m_police_car->m_received_messages.size(), 1);
    EXPECT_EQ(sim.m_fire_engine->m_sent_messages.size(), 0);
    EXPECT_EQ(sim.m_fire_engine->m_received_messages.size(), 1);
    EXPECT_EQ(sim.m_ambulance->m_sent_messages.size(), 0);
    EXPECT_EQ(sim.m_ambulance->m_received_messages.size(), 1);
    
    std::shared_ptr<MessageData> sent = sim.m_home_owner->m_sent_messages.front();
    std::shared_ptr<MessageData> received = sim.m_police_car->m_received_messages.front();
    std::shared_ptr<MessageData> received_two = sim.m_fire_engine->m_received_messages.front();
    std::shared_ptr<MessageData> received_three = sim.m_ambulance->m_received_messages.front();
    FireReport* home_owner_report = static_cast<FireReport*>(sent.get());
    FireReport* police_report = static_cast<FireReport*>(received.get());
    FireReport* fire_engine_report = static_cast<FireReport*>(received_two.get());
    FireReport* ambulance_report = static_cast<FireReport*>(received_three.get());
    EXPECT_EQ(home_owner_report->get_name(), "FireReport");
    EXPECT_EQ(police_report->get_name(), "FireReport");
    EXPECT_EQ(fire_engine_report->get_name(), "FireReport");
    EXPECT_EQ(ambulance_report->get_name(), "FireReport");
    
    EXPECT_EQ(home_owner_report->location.street_address, "10 Adams Road");
    EXPECT_EQ(home_owner_report->location.city, "Milton");
    EXPECT_EQ(home_owner_report->location.state, "MA");
    EXPECT_EQ(home_owner_report->occupants.size(), 4);
    EXPECT_EQ(home_owner_report->occupants[0], "Tom Brady");
    EXPECT_EQ(home_owner_report->occupants[1], "Larry Bird");
    EXPECT_EQ(home_owner_report->occupants[2], "Zdeno Chara");
    EXPECT_EQ(home_owner_report->occupants[3], "Bobby Orr");
    
    EXPECT_EQ(home_owner_report->location.street_address, police_report->location.street_address);
    EXPECT_EQ(home_owner_report->location.city, police_report->location.city);
    EXPECT_EQ(home_owner_report->location.state, police_report->location.state);
    EXPECT_EQ(home_owner_report->occupants.size(), police_report->occupants.size());
    EXPECT_EQ(home_owner_report->occupants[0], police_report->occupants[0]);
    EXPECT_EQ(home_owner_report->occupants[1], police_report->occupants[1]);
    EXPECT_EQ(home_owner_report->occupants[2], police_report->occupants[2]);
    EXPECT_EQ(home_owner_report->occupants[3], police_report->occupants[3]);

    EXPECT_EQ(home_owner_report->location.street_address, fire_engine_report->location.street_address);
    EXPECT_EQ(home_owner_report->location.city, fire_engine_report->location.city);
    EXPECT_EQ(home_owner_report->location.state, fire_engine_report->location.state);
    EXPECT_EQ(home_owner_report->occupants.size(), fire_engine_report->occupants.size());
    EXPECT_EQ(home_owner_report->occupants[0], fire_engine_report->occupants[0]);
    EXPECT_EQ(home_owner_report->occupants[1], fire_engine_report->occupants[1]);
    EXPECT_EQ(home_owner_report->occupants[2], fire_engine_report->occupants[2]);
    EXPECT_EQ(home_owner_report->occupants[3], fire_engine_report->occupants[3]);

    EXPECT_EQ(home_owner_report->location.street_address, ambulance_report->location.street_address);
    EXPECT_EQ(home_owner_report->location.city, ambulance_report->location.city);
    EXPECT_EQ(home_owner_report->location.state, ambulance_report->location.state);
    EXPECT_EQ(home_owner_report->occupants.size(), ambulance_report->occupants.size());
    EXPECT_EQ(home_owner_report->occupants[0], ambulance_report->occupants[0]);
    EXPECT_EQ(home_owner_report->occupants[1], ambulance_report->occupants[1]);
    EXPECT_EQ(home_owner_report->occupants[2], ambulance_report->occupants[2]);
    EXPECT_EQ(home_owner_report->occupants[3], ambulance_report->occupants[3]);
}

////////////////////////////////////////////////////////
TEST(MessageSystem_Example, MessageSystem_Example_MedicalEmergency) {
    ScenarioSimulation sim;

    // Start the message transfer
    EXPECT_TRUE(sim.start_medical_emergency());

    EXPECT_EQ(sim.m_civilian->m_sent_messages.size(), 0);
    EXPECT_EQ(sim.m_civilian->m_received_messages.size(), 0);
    EXPECT_EQ(sim.m_home_owner->m_sent_messages.size(), 1);
    EXPECT_EQ(sim.m_home_owner->m_received_messages.size(), 0);
    EXPECT_EQ(sim.m_store_owner->m_sent_messages.size(), 0);
    EXPECT_EQ(sim.m_store_owner->m_received_messages.size(), 0);
    EXPECT_EQ(sim.m_police_car->m_sent_messages.size(), 0);
    EXPECT_EQ(sim.m_police_car->m_received_messages.size(), 0);
    EXPECT_EQ(sim.m_fire_engine->m_sent_messages.size(), 0);
    EXPECT_EQ(sim.m_fire_engine->m_received_messages.size(), 1);
    EXPECT_EQ(sim.m_ambulance->m_sent_messages.size(), 0);
    EXPECT_EQ(sim.m_ambulance->m_received_messages.size(), 1);

    std::shared_ptr<MessageData> sent = sim.m_home_owner->m_sent_messages.front();
    std::shared_ptr<MessageData> received = sim.m_fire_engine->m_received_messages.front();
    std::shared_ptr<MessageData> received_two = sim.m_ambulance->m_received_messages.front();
    MedicalEvent* home_owner_report = static_cast<MedicalEvent*>(sent.get());
    MedicalEvent* fire_engine_report = static_cast<MedicalEvent*>(received.get());
    MedicalEvent* ambulance_report = static_cast<MedicalEvent*>(received_two.get());
    
    EXPECT_EQ(home_owner_report->get_name(), "MedicalEvent");
    EXPECT_EQ(fire_engine_report->get_name(), "MedicalEvent");
    EXPECT_EQ(ambulance_report->get_name(), "MedicalEvent");

    EXPECT_EQ(home_owner_report->location.street_address, "17 Forbes Street");
    EXPECT_EQ(home_owner_report->location.city, "Medford");
    EXPECT_EQ(home_owner_report->location.state, "MA");
    EXPECT_EQ(home_owner_report->patient_name, "Justin Case");

    EXPECT_EQ(home_owner_report->location.street_address, fire_engine_report->location.street_address);
    EXPECT_EQ(home_owner_report->location.city, fire_engine_report->location.city);
    EXPECT_EQ(home_owner_report->location.state, fire_engine_report->location.state);
    EXPECT_EQ(home_owner_report->patient_name, fire_engine_report->patient_name);

    EXPECT_EQ(home_owner_report->location.street_address, ambulance_report->location.street_address);
    EXPECT_EQ(home_owner_report->location.city, ambulance_report->location.city);
    EXPECT_EQ(home_owner_report->location.state, ambulance_report->location.state);
    EXPECT_EQ(home_owner_report->patient_name, ambulance_report->patient_name);
}

////////////////////////////////////////////////////////
TEST(MessageSystem_Example, MessageSystem_Example_SendAll) {
    ScenarioSimulation sim;

    // Start the message transfer
    EXPECT_TRUE(sim.start_robbery());
    EXPECT_TRUE(sim.start_car_accident());
    EXPECT_TRUE(sim.start_cat_stuck_in_tree());
    EXPECT_TRUE(sim.start_fire_report());
    EXPECT_TRUE(sim.start_medical_emergency());

    EXPECT_EQ(sim.m_civilian->m_sent_messages.size(), 2);
    EXPECT_EQ(sim.m_civilian->m_received_messages.size(), 0);
    EXPECT_EQ(sim.m_home_owner->m_sent_messages.size(), 2);
    EXPECT_EQ(sim.m_home_owner->m_received_messages.size(), 0);
    EXPECT_EQ(sim.m_store_owner->m_sent_messages.size(), 2);
    EXPECT_EQ(sim.m_store_owner->m_received_messages.size(), 1);
    EXPECT_EQ(sim.m_police_car->m_sent_messages.size(), 1);
    EXPECT_EQ(sim.m_police_car->m_received_messages.size(), 4);
    EXPECT_EQ(sim.m_fire_engine->m_sent_messages.size(), 0);
    EXPECT_EQ(sim.m_fire_engine->m_received_messages.size(), 3);
    EXPECT_EQ(sim.m_ambulance->m_sent_messages.size(), 0);
    EXPECT_EQ(sim.m_ambulance->m_received_messages.size(), 3);

    EXPECT_EQ(sim.m_civilian->m_sent_messages.front()->get_name(), "CarAccident");
    sim.m_civilian->m_sent_messages.pop();
    EXPECT_EQ(sim.m_civilian->m_sent_messages.front()->get_name(), "CatStuckInTree");
    sim.m_civilian->m_sent_messages.pop();

    EXPECT_EQ(sim.m_home_owner->m_sent_messages.front()->get_name(), "FireReport");
    sim.m_home_owner->m_sent_messages.pop();
    EXPECT_EQ(sim.m_home_owner->m_sent_messages.front()->get_name(), "MedicalEvent");
    sim.m_home_owner->m_sent_messages.pop();

    EXPECT_EQ(sim.m_store_owner->m_sent_messages.front()->get_name(), "Robbery");
    sim.m_store_owner->m_sent_messages.pop();
    EXPECT_EQ(sim.m_store_owner->m_sent_messages.front()->get_name(), "SuspectDescription");
    sim.m_store_owner->m_sent_messages.pop();
    EXPECT_EQ(sim.m_store_owner->m_received_messages.front()->get_name(), "Interview");
    sim.m_store_owner->m_received_messages.pop();

    EXPECT_EQ(sim.m_police_car->m_sent_messages.front()->get_name(), "Interview");
    sim.m_police_car->m_sent_messages.pop();
    EXPECT_EQ(sim.m_police_car->m_received_messages.front()->get_name(), "Robbery");
    sim.m_police_car->m_received_messages.pop();
    EXPECT_EQ(sim.m_police_car->m_received_messages.front()->get_name(), "SuspectDescription");
    sim.m_police_car->m_received_messages.pop();
    EXPECT_EQ(sim.m_police_car->m_received_messages.front()->get_name(), "CarAccident");
    sim.m_police_car->m_received_messages.pop();
    EXPECT_EQ(sim.m_police_car->m_received_messages.front()->get_name(), "FireReport");
    sim.m_police_car->m_received_messages.pop();

    EXPECT_EQ(sim.m_fire_engine->m_received_messages.front()->get_name(), "CatStuckInTree");
    sim.m_fire_engine->m_received_messages.pop();
    EXPECT_EQ(sim.m_fire_engine->m_received_messages.front()->get_name(), "FireReport");
    sim.m_fire_engine->m_received_messages.pop();
    EXPECT_EQ(sim.m_fire_engine->m_received_messages.front()->get_name(), "MedicalEvent");
    sim.m_fire_engine->m_received_messages.pop();

    EXPECT_EQ(sim.m_ambulance->m_received_messages.front()->get_name(), "CarAccident");
    sim.m_ambulance->m_received_messages.pop();
    EXPECT_EQ(sim.m_ambulance->m_received_messages.front()->get_name(), "FireReport");
    sim.m_ambulance->m_received_messages.pop();
    EXPECT_EQ(sim.m_ambulance->m_received_messages.front()->get_name(), "MedicalEvent");
    sim.m_ambulance->m_received_messages.pop();

    EXPECT_EQ(sim.m_civilian->m_sent_messages.size(), 0);
    EXPECT_EQ(sim.m_civilian->m_received_messages.size(), 0);
    EXPECT_EQ(sim.m_home_owner->m_sent_messages.size(), 0);
    EXPECT_EQ(sim.m_home_owner->m_received_messages.size(), 0);
    EXPECT_EQ(sim.m_store_owner->m_sent_messages.size(), 0);
    EXPECT_EQ(sim.m_store_owner->m_received_messages.size(), 0);
    EXPECT_EQ(sim.m_police_car->m_sent_messages.size(), 0);
    EXPECT_EQ(sim.m_police_car->m_received_messages.size(), 0);
    EXPECT_EQ(sim.m_fire_engine->m_sent_messages.size(), 0);
    EXPECT_EQ(sim.m_fire_engine->m_received_messages.size(), 0);
    EXPECT_EQ(sim.m_ambulance->m_sent_messages.size(), 0);
    EXPECT_EQ(sim.m_ambulance->m_received_messages.size(), 0);
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
