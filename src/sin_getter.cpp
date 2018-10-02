// code written by Frank Qian CXQ41@case.edu for EECS 473 Homework PS3:
// This node is responsible for getting values and pass along:
#include <ros/ros.h>
#include <std_msgs/Float64.h>
#include <string>
// IO specific vg
#include <math.h>
#include <iostream>
// Action specificfrequency
#include <actionlib/client/simple_action_client.h>  //Mandatory for Action Server
#include <cxq41_ps3/SinComponentAction.h>

void doneCb(const actionlib::SimpleClientGoalState& state,
            const cxq41_ps3::SinComponentResultConstPtr& result) {
    ROS_INFO("doneCb: server responded with [%s]", state.toString().c_str());
}

int main(int argc, char** argv) {
    // Node Initiation
    ros::init(argc, argv, "ps3_sin_getter");

    // Goal establisher
    cxq41_ps3::SinComponentGoal goal;

    // Link action server
    actionlib::SimpleActionClient<cxq41_ps3::SinComponentAction> action_client(
        "sin_commander", true);

    // Attempt to connect to the server:
    ROS_INFO("waiting for server: ");
    bool server_exists = action_client.waitForServer(
        ros::Duration(5.0));  // wait for up to 5 seconds
    // Bail if server not exits
    if (!server_exists) {
        ROS_FATAL("Server not available!!!");
        return 3;
    }

    ROS_INFO("Action server has been reached!");

    while (ros::ok()) {
        // Get input for amplitude
        goal.amplitude = 0.00;
        std::cout << std::endl;
        std::cout << "Please input the amplitude you want to set.......(Use "
                     "CTRL+C to Quit)"
                  << std::endl
                  << "Input: ";
        std::cin >> goal.amplitude;
        while (std::cin.fail()) {
            ROS_ERROR(
                "Input is not valid! Please try again....(Use CTRL+C to Quit)");
            std::cout << "Input: ";
            std::cin.clear();
            std::cin.ignore(256, '\n');
            std::cin >> goal.amplitude;
        }
        ROS_INFO("The amplitude you set is: %f", goal.amplitude);
        // Get input for frequency:
        goal.frequency = 0.00;
        std::cout << std::endl;
        std::cout << "Please input the frequency you want to set.......(Use "
                     "CTRL+C to Quit)"
                  << std::endl
                  << "Input: ";
        std::cin >> goal.frequency;
        while (std::cin.fail()) {
            ROS_ERROR(
                "Input is not valid! Please try again....(Use CTRL+C to Quit)");
            std::cout << "Input: ";
            std::cin.clear();
            std::cin.ignore(256, '\n');
            std::cin >> goal.frequency;
        }
        ROS_INFO("The frequency you set is: %f", goal.frequency);
        // Get input for cycles
        goal.cycles = 0.00;
        std::cout << std::endl;
        std::cout << "Please input the cycles you want to set.......(Use "
                     "CTRL+C to Quit)"
                  << std::endl
                  << "Input: ";
        std::cin >> goal.cycles;
        while (std::cin.fail()) {
            ROS_ERROR(
                "Input is not valid! Please try again....(Use CTRL+C to Quit)");
            std::cout << "Input: ";
            std::cin.clear();
            std::cin.ignore(256, '\n');
            std::cin >> goal.cycles;
        }
        ROS_INFO("The cycles you set is: %f", goal.cycles);

        // Send Goals
        ROS_INFO("Sending Goals Now....");
        action_client.sendGoal(goal, &doneCb);  // Send goal to server
        bool finished_before_timeout = action_client.waitForResult(
            ros::Duration());
        // Time Checker
        if (!finished_before_timeout) {
            ROS_FATAL("Giving up waiting....");
            return 1;
        } else {
            ROS_INFO("Send Goal Complete... Handing over to sin commander.....");
        }
    }
    return 0;  // Not used unless some dummy kills roscore
}
