#include <gtest/gtest.h>

#include <iostream>
#include <string>
#include <vector>

#include "behaviortree_cpp/behavior_tree.h"
#include "behaviortree_cpp/bt_factory.h"

std::vector<std::string> g_bt_node_test;

//////////////////////////////////////////////////////////////
struct Position2D
{
    double x, y;
};

// It is recommended (or, in some cases, mandatory) to define a template
// specialization of convertFromString that converts a string to Position2D.
namespace BT {
template <>
inline Position2D convertFromString(BT::StringView str)
{
    g_bt_node_test.push_back("Converting string: " + std::string(str.data()));

    // real numbers separated by semicolons
    auto parts = splitString(str, ';');
    if (parts.size() != 2) {
        throw RuntimeError("invalid input)");
    } else {
        Position2D output;
        output.x = convertFromString<double>(parts[0]);
        output.y = convertFromString<double>(parts[1]);
        return output;
    }
}
}

//////////////////////////////////////////////////////////////
BT::NodeStatus CheckBattery() {
    g_bt_node_test.push_back("[ Battery: OK ]");
    return BT::NodeStatus::SUCCESS;
}

//////////////////////////////////////////////////////////////
class GripperInterface
{
public:
    GripperInterface() = default;
    BT::NodeStatus open() {
        _opened = true;
        g_bt_node_test.push_back("GripperInterface::open _opened:" + std::to_string(_opened));
        return BT::NodeStatus::SUCCESS;
    }

    BT::NodeStatus close() {
        _opened = false;
        g_bt_node_test.push_back("GripperInterface::close _opened:" + std::to_string(_opened));
        return BT::NodeStatus::SUCCESS;
    }

private:
    bool _opened = true;
};

//////////////////////////////////////////////////////////////
// Example of custom SyncActionNode (synchronous action)
// without ports.
class ApproachObject : public BT::SyncActionNode
{
public:
    ApproachObject(const std::string& name) : BT::SyncActionNode(name, {})
    {}

    // You must override the virtual function tick()
    BT::NodeStatus tick() override {
        g_bt_node_test.push_back("ApproachObject:" + this->name());
        return BT::NodeStatus::SUCCESS;
    }
};

//////////////////////////////////////////////////////////////
class CalculateGoal : public BT::SyncActionNode
{
public:
    CalculateGoal(const std::string& name, const BT::NodeConfig& config)
        : SyncActionNode(name, config)
    {}

    BT::NodeStatus tick() override {
        Position2D mygoal = { 1.1, 2.3 };
        setOutput("goal", mygoal);
        g_bt_node_test.push_back("CalculateGoal: goal `1.1, 2.3`");
        return BT::NodeStatus::SUCCESS;
    }

    static BT::PortsList providedPorts() {
        return { BT::OutputPort<Position2D>("goal") };
    }
};

//////////////////////////////////////////////////////////////
class PrintTarget : public BT::SyncActionNode
{
public:
    PrintTarget(const std::string& name, const BT::NodeConfig& config)
        : SyncActionNode(name, config)
    {}

    BT::NodeStatus tick() override {
        auto res = getInput<Position2D>("target");
        if(!res) {
            throw BT::RuntimeError("error reading port [target]:", res.error());
        }
        Position2D goal = res.value();
        g_bt_node_test.push_back("PrintTarget: positions: [ " + std::to_string(goal.x) + ", " +  std::to_string(goal.y) + " ]");
        return BT::NodeStatus::SUCCESS;
    }

    static BT::PortsList providedPorts() {
        // Optionally, a port can have a human readable description
        const char* description = "Simply print the target on console...";
        return { BT::InputPort<Position2D>("target", description) };
    }
};

//////////////////////////////////////////////////////////////
void RegisterNodes(BT::BehaviorTreeFactory& factory)
{
    static GripperInterface grip_singleton;

    factory.registerSimpleCondition("CheckBattery", std::bind(CheckBattery));
    factory.registerSimpleAction("OpenGripper",
                                std::bind(&GripperInterface::open, &grip_singleton));
    factory.registerSimpleAction("CloseGripper",
                                std::bind(&GripperInterface::close, &grip_singleton));
    factory.registerNodeType<ApproachObject>("ApproachObject");
    factory.registerNodeType<CalculateGoal>("CalculateGoal");
    factory.registerNodeType<PrintTarget>("PrintTarget");
}

//////////////////////////////////////////////////////////////
TEST(BehaviorTreeCPP, BehaviorTreeCPP_simple) {
    BT::BehaviorTreeFactory factory;
    g_bt_node_test.clear();

    RegisterNodes(factory);
    std::string xml_file = "code\\test\\test_data\\simple_tree_BT.xml";
    // Trees are created at deployment-time (i.e. at run-time, but only once at the beginning).
    // The currently supported format is XML.
    // IMPORTANT: when the object "tree" goes out of scope, all the TreeNodes are destroyed
    auto tree = factory.createTreeFromFile(xml_file);
    
    // To "execute" a Tree you need to "tick" it.
    // The tick is propagated to the children based on the logic of the tree.
    // In this case, the entire sequence is executed, because all the children
    // of the Sequence return SUCCESS.
    tree.tickWhileRunning();
 
/*
[ Battery: OK ]
GripperInterface::open _opened:1
ApproachObject:approach_object
GripperInterface::close _opened:0

    for (const std::string& step : g_bt_node_test) {
        std::cout << step << std::endl;
    }
*/

    EXPECT_EQ(g_bt_node_test.size(), 4);
    if (g_bt_node_test.size() >= 4) {
        EXPECT_EQ(g_bt_node_test[0], "[ Battery: OK ]");
        EXPECT_EQ(g_bt_node_test[1], "GripperInterface::open _opened:1");
        EXPECT_EQ(g_bt_node_test[2], "ApproachObject:approach_object");
        EXPECT_EQ(g_bt_node_test[3], "GripperInterface::close _opened:0");
    }
}

//////////////////////////////////////////////////////////////
TEST(BehaviorTreeCPP, BehaviorTreeCPP_generic_ports) {
    BT::BehaviorTreeFactory factory;
    g_bt_node_test.clear();

    RegisterNodes(factory);
    std::string xml_file = "code\\test\\test_data\\generic_ports_BT.xml";
    // Trees are created at deployment-time (i.e. at run-time, but only once at the beginning).
    // The currently supported format is XML.
    // IMPORTANT: when the object "tree" goes out of scope, all the TreeNodes are destroyed
    auto tree = factory.createTreeFromFile(xml_file);
    
    // To "execute" a Tree you need to "tick" it.
    // The tick is propagated to the children based on the logic of the tree.
    // In this case, the entire sequence is executed, because all the children
    // of the Sequence return SUCCESS.
    tree.tickWhileRunning();
 
/*
CalculateGoal: goal `1.1, 2.3`
PrintTarget: positions: [ 1.100000, 2.300000 ]
Converting string: -1;3
PrintTarget: positions: [ -1.000000, 3.000000 ]

    for (const std::string& step : g_bt_node_test) {
        std::cout << step << std::endl;
    }
*/
    EXPECT_EQ(g_bt_node_test.size(), 4);
    if (g_bt_node_test.size() >= 4) {
        EXPECT_EQ(g_bt_node_test[0], "CalculateGoal: goal `1.1, 2.3`");
        EXPECT_EQ(g_bt_node_test[1], "PrintTarget: positions: [ 1.100000, 2.300000 ]");
        EXPECT_EQ(g_bt_node_test[2], "Converting string: -1;3");
        EXPECT_EQ(g_bt_node_test[3], "PrintTarget: positions: [ -1.000000, 3.000000 ]");
    }
}

//////////////////////////////////////////////////////////////
int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
 