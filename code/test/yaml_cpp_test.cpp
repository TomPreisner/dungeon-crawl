#include <gtest/gtest.h>

#include "yaml-cpp/yaml.h"

TEST(YamlTest, ReadYAMLFileTest) {
  std::string yaml_file = "code\\test\\test_data\\test_yaml.yaml";

  // A simple load of a yaml file and validate the contents
  YAML::Node config = YAML::LoadFile(yaml_file);

  EXPECT_TRUE(config.IsMap());

  /*
  app:
    name: "MyWebApp"
    version: 1.2.3
    environment: production
    debug: false
  */
  YAML::Node app = config["app"];
  EXPECT_TRUE(app.IsMap());
  
  EXPECT_TRUE(app["name"].IsScalar());
  EXPECT_EQ("MyWebApp", app["name"].as<std::string>());
  EXPECT_TRUE(app["version"].IsScalar());
  EXPECT_EQ("1.2.3", app["version"].as<std::string>());
  EXPECT_TRUE(app["environment"].IsScalar());
  EXPECT_EQ("production", app["environment"].as<std::string>());
  EXPECT_TRUE(app["debug"].IsScalar());
  EXPECT_FALSE(app["debug"].as<bool>());

  /*
  database:
    host: localhost
    port: 5432
    name: myapp_db
    credentials:
      username: admin
      password: secret123
  */
  YAML::Node database = config["database"];
  EXPECT_TRUE(database.IsMap());
  
  EXPECT_TRUE(database["host"].IsScalar());
  EXPECT_EQ("localhost", database["host"].as<std::string>());
  EXPECT_TRUE(database["port"].IsScalar());
  EXPECT_EQ(5432, database["port"].as<int>());
  EXPECT_TRUE(database["name"].IsScalar());
  EXPECT_EQ("myapp_db", database["name"].as<std::string>());
  EXPECT_TRUE(database["credentials"]["username"].IsScalar());
  EXPECT_EQ("admin", database["credentials"]["username"].as<std::string>());
  EXPECT_TRUE(database["credentials"]["password"].IsScalar());
  EXPECT_EQ("secret123", database["credentials"]["password"].as<std::string>());
  
  /*
  server:
    host: 0.0.0.0
    port: 8000
    max_connections: 100
  */
  YAML::Node server = config["server"];
  EXPECT_TRUE(server.IsMap());
  
  EXPECT_TRUE(server["host"].IsScalar());
  EXPECT_EQ("0.0.0.0", server["host"].as<std::string>());
  EXPECT_TRUE(server["port"].IsScalar());
  EXPECT_EQ(8000, server["port"].as<int>());
  EXPECT_TRUE(server["max_connections"].IsScalar());
  EXPECT_EQ(100, server["max_connections"].as<int>());
  
  /*
  features:
    enable_cache: true
    cache_ttl: 3600
  */
  YAML::Node features = config["features"];
  EXPECT_TRUE(features.IsMap());
  
  EXPECT_TRUE(features["enable_cache"].IsScalar());
  EXPECT_TRUE(features["enable_cache"].as<bool>());
  EXPECT_TRUE(features["cache_ttl"].IsScalar());
  EXPECT_EQ(3600, features["cache_ttl"].as<int>());
  
  /*
  services:
    - name: auth-service
      url: http://auth.example.com
      timeout: 30
    - name: payment-service
      url: http://payment.example.com
      timeout: 60
  */
  YAML::Node services = config["services"];
  EXPECT_TRUE(services.IsSequence());
  EXPECT_EQ(2, services.size());
  for (int i = 0; i < services.size(); ++i) {
    if (i == 0) {
      EXPECT_EQ("auth-service", services[i]["name"].as<std::string>());
      EXPECT_EQ("http://auth.example.com", services[i]["url"].as<std::string>());
      EXPECT_EQ(30, services[i]["timeout"].as<int>());
    } else if (i == 1) {
      EXPECT_EQ("payment-service", services[i]["name"].as<std::string>());
      EXPECT_EQ("http://payment.example.com", services[i]["url"].as<std::string>());
      EXPECT_EQ(60, services[i]["timeout"].as<int>());
    } else {
      EXPECT_TRUE(false); // Failure case, there are only two entries
    }
  }

}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
