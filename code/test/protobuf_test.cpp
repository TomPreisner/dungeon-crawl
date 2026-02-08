#include <gtest/gtest.h>

#include <google/protobuf/util/time_util.h>
#include <iostream>
#include <string>

#include "code/test/test_data/test_data.pb.h"

TEST(ProtobufTest, protobuf_create_test) {
  code::test::test_data::AddressBook addrBook;

  /////////////////////////////////////////
  code::test::test_data::Person testPerson;
  testPerson.set_name("John Doe");
  testPerson.set_id(123456);
  testPerson.set_email("jdoe@email.com");
  
  auto phone = testPerson.add_phones();
  phone->set_number("(123)456-7890");
  phone->set_type(code::test::test_data::Person_PhoneType_MOBILE);
  
  phone = testPerson.add_phones();
  phone->set_number("(321)654-0987");
  phone->set_type(code::test::test_data::Person_PhoneType_WORK);
  
  /*
  Unix Timestamp	1746669305
  GMT	Thu May 08 2025 01:55:05 GMT+0000
  */
  *testPerson.mutable_last_updated() = google::protobuf::util::TimeUtil::SecondsToTimestamp(1746669305);

  *addrBook.add_people() = std::move(testPerson);

  /////////////////////////////////////////
  auto person = addrBook.add_people();
  *(person->mutable_name()) = "Steve Smith";
  person->set_id(234567);
  *(person->mutable_email()) = "steve.smith@geocities.com";
  
  phone = person->add_phones();
  phone->set_number("(987)654-3210");
  phone->set_type(code::test::test_data::Person_PhoneType_HOME);

  /*
  Unix Timestamp	1723563425
  GMT	Tue Aug 13 2024 15:37:05 GMT+0000
  */
  *(person->mutable_last_updated()) =  google::protobuf::util::TimeUtil::SecondsToTimestamp(1723563425);

  /////////////////////////////////////////
  person = addrBook.add_people();
  person->set_name("Jane Austin");
  person->set_id(456789);
  person->set_email("jaustin1234@aolonline.com");

  /*
  Unix Timestamp	1771108036
  GMT	Sat Feb 14 2026 22:27:16 GMT+0000
  */
  *(person->mutable_last_updated()) =  google::protobuf::util::TimeUtil::SecondsToTimestamp(1771108036); 

 // validate the person entries in the address book
  EXPECT_EQ(addrBook.people_size(), 3);

  /////////////////////////////////////////
  auto addrPerson = addrBook.people(0);
  EXPECT_EQ(addrPerson.name(), "John Doe");
  EXPECT_EQ(addrPerson.id(), 123456);
  EXPECT_EQ(addrPerson.email(), "jdoe@email.com");

  EXPECT_EQ(addrPerson.phones_size(), 2);

  auto addrPhone = addrPerson.phones(0);
  EXPECT_EQ(addrPhone.number(), "(123)456-7890");
  EXPECT_EQ(addrPhone.type(), code::test::test_data::Person_PhoneType_MOBILE);
  
  addrPhone = addrPerson.phones(1);
  EXPECT_EQ(addrPhone.number(), "(321)654-0987");
  EXPECT_EQ(addrPhone.type(), code::test::test_data::Person_PhoneType_WORK);

  EXPECT_EQ(addrPerson.last_updated(), google::protobuf::util::TimeUtil::SecondsToTimestamp(1746669305));
  EXPECT_EQ(addrPerson.last_updated().seconds(), 1746669305);
  EXPECT_EQ(google::protobuf::util::TimeUtil::ToString(addrPerson.last_updated()) , "2025-05-08T01:55:05Z");
  
  /////////////////////////////////////////
  addrPerson = addrBook.people(1);
  EXPECT_EQ(addrPerson.name(), "Steve Smith");
  EXPECT_EQ(addrPerson.id(), 234567);
  EXPECT_EQ(addrPerson.email(), "steve.smith@geocities.com");

  EXPECT_EQ(addrPerson.phones_size(), 1);

  addrPhone = addrPerson.phones(0);
  EXPECT_EQ(addrPhone.number(), "(987)654-3210");
  EXPECT_EQ(addrPhone.type(), code::test::test_data::Person_PhoneType_HOME);

  EXPECT_EQ(addrPerson.last_updated(), google::protobuf::util::TimeUtil::SecondsToTimestamp(1723563425));
  EXPECT_EQ(addrPerson.last_updated().seconds(), 1723563425);
  EXPECT_EQ(google::protobuf::util::TimeUtil::ToString(addrPerson.last_updated()) , "2024-08-13T15:37:05Z");

  /////////////////////////////////////////
  addrPerson = addrBook.people(2);
  EXPECT_EQ(addrPerson.name(), "Jane Austin");
  EXPECT_EQ(addrPerson.id(), 456789);
  EXPECT_EQ(addrPerson.email(), "jaustin1234@aolonline.com");

  EXPECT_EQ(addrPerson.phones_size(), 0);

  EXPECT_EQ(addrPerson.last_updated(), google::protobuf::util::TimeUtil::SecondsToTimestamp(1771108036));
  EXPECT_EQ(addrPerson.last_updated().seconds(), 1771108036);
  EXPECT_EQ(google::protobuf::util::TimeUtil::ToString(addrPerson.last_updated()) , "2026-02-14T22:27:16Z");
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}