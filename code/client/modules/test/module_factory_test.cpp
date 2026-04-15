#include <gtest/gtest.h>

#include <iostream>
#include <string>
#include <set>
#include "code/client/modules/module_factory.h"
#include "code/client/status_effect/status_effect_library.h"

std::string k_yaml_file = "code\\client\\modules\\test\\data\\test_module_factory_data.yaml";
std::string k_yaml_library_file = "code\\client\\modules\\test\\data\\test_status_effect_library_data.yaml";

TEST(ModuleFactory, Test_ModuleFactory) {
    Status::StatusEffectLibrary::get_Instance()->init_from_file(k_yaml_library_file);

    // A simple load of a yaml file and validate the contents
    YAML::Node data_set = YAML::LoadFile(k_yaml_file);

    EXPECT_TRUE(data_set.IsMap());

    std::vector<std::string> module_list = Module::ModuleFactory::get_Instance()->list_modules();

    EXPECT_EQ(data_set.size(), module_list.size());

    YAML::const_iterator end = data_set.end();
    for (YAML::iterator iter = data_set.begin(); iter != end; ++iter) {
        YAML::Node& item = iter->second;
        std::string item_name = item["name"].as<std::string>();

        // delete the entries in module_list as they are found
        for (auto mod_iter = module_list.begin(); mod_iter != module_list.end(); ++mod_iter) {
            if (*mod_iter == item_name) {
                module_list.erase(mod_iter);
                break;
            }
        }

        auto module = Module::ModuleFactory::get_Instance()->create_module(item_name, item["data"]);
        EXPECT_TRUE(module.get() != nullptr);
        EXPECT_TRUE(module->init_module(item["data"])); //< re init here to test that it worked.
    }

    // All modules should have been found
    EXPECT_TRUE(module_list.empty());

    Status::StatusEffectLibrary::get_Instance()->shutdown();
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
