#include <gtest/gtest.h>

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

TEST(BostUUIDTest, GeneratorTest) {
  // Use the boost uuid library to create a uuid to uniquely identify this status effect instance
  boost::uuids::random_generator r_gen;
  boost::uuids::uuid id = r_gen();
  std::string uuid_string = boost::uuids::to_string(id);

  EXPECT_NE(uuid_string, "");

  // convert the guid string back to a uuid
  boost::uuids::string_generator s_gen;
  EXPECT_EQ(s_gen(uuid_string), id);

  // Convert a string to a uuid then back to a string
  std::string test_uuid = "fa807cd6-2ff9-4f07-970a-41f97922f9e4";
  boost::uuids::uuid s_id = s_gen(test_uuid);
  EXPECT_EQ(test_uuid, boost::uuids::to_string(s_id));
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
 