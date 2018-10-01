// code written by Frank Qian CXQ41@case.edu for EECS 473 Homework PS3 node:
#include <ros/ros.h>
#include <std_msgs/Float64.h>
#include <string>
// Project specific include:
#include <math.h>
#include <iostream>
// Action Server specific
#include <actionlib/server/simple_action_server.h>
#include <cxq41_ps3/SinComponentAction.h>
// Global Variable Initiation
std_msgs::Float64 commander_amplitude;  // Global variable for storing amplitude
std_msgs::Float64 commander_frequency;  // Global variable for storing frequency
std_msgs::Float64 commander_cycle;      // Global variable for storing cycles
std_msgs::Float64 sin_output;  // Global Variable for the sinusoidal output
const double PI = 3.141592653589793238463;  // Yep too fucking lazy to figure
                                            // out proper way to express pi!

class sin_commander {
   private:
    // we'll need a node handle; get one upon instantiation
    ros::NodeHandle nh_;
    // Establishes a simple action server with name "as_"
    actionlib::SimpleActionServer<cxq41_ps3::SinComponentAction> as_;
    // Define message type:
    cxq41_ps3::SinComponentGoal goal_;
    cxq41_ps3::SinComponentResult result_;
    cxq41_ps3::SinComponentFeedback feedback_;
    // Creates a Publisher
    ros::Publisher sinWave_outputter;

   public:
    // define the body of constructor outside class definition
    sin_commander();

    ~sin_commander(void) {}

    // action interface
    void executeCB(const actionlib::SimpleActionServer<
                   cxq41_ps3::SinComponentAction>::GoalConstPtr& goal);
}; //Need to have a semi-column or it will throw return errors....

sin_commander::sin_commander()
    : as_(nh_, "sin_commander",
          boost::bind(&sin_commander::executeCB, this, _1), false) {
    ROS_INFO("in constructor of sin_commander...");
    sinWave_outputter = nh_.advertise<std_msgs::Float64>("vel_cmd", 1);
    as_.start();
}

void sin_commander::executeCB(
    const actionlib::SimpleActionServer<
        cxq41_ps3::SinComponentAction>::GoalConstPtr& goal) {
    ROS_DEBUG("in executeCB"); //Debug indication

    commander_amplitude.data = goal->amplitude;
    ROS_INFO("Goal Amplitude is: %f", commander_amplitude);
    commander_frequency.data = goal->frequency;
    if (commander_frequency.data == 0) {
        ROS_FATAL(
            "Frequency of 0 means nothing hence not accepted!");  // Yell at
                                                                  // someone
                                                                  // being
                                                                  // stupid...
    }
    ROS_INFO("Goal Frequency is: %f", commander_frequency);
    commander_cycle.data = goal->cycles;
    ROS_INFO("Goal Cycles is: %f", commander_cycle);

    // Start of Control:
    // Initiate sin_output
    sin_output.data = 0.00;
    // Initiate Time for sine generation:
    double current_time;  // Initate the time
    current_time = 0.00;  // Resets time to 0

    double dt = 0.01;
    double sample_rate = 1.0 / dt;
    ros::Rate naptime(sample_rate);

    while (ros::ok() &&
           (current_time < (commander_cycle.data / commander_frequency.data))) {
        sin_output.data =
            commander_amplitude.data *
            sin(2.00 * commander_frequency.data * PI * current_time);
        current_time = current_time + dt;
        sinWave_outputter.publish(sin_output);
        ROS_INFO("Current sin_ouput is: %f", sin_output.data);
        // If you don't have below your sin wave will be squarish....Learned the
        // hard way....
        ros::spinOnce();
        naptime.sleep();
    }

    sin_output.data = 0.0;
    sinWave_outputter.publish(sin_output);

    result_.completion = true;
    as_.setSucceeded(result_);
}

int main(int argc, char** argv) {
    // Node Initiation
    ros::init(argc, argv, "ps3_sin_commander");  // name this node

    ROS_INFO("Instantiating the sin_commander action server...");

    sin_commander as_object;

    ros::spin();

    return 0;  // Not used unless some dummy kills roscore
}
