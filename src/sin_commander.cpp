// code written by Frank Qian CXQ41@case.edu for EECS 473 Homework PS1 node: 
#include <ros/ros.h> 
#include <std_msgs/Float64.h> 
#include <string>
//Project specific include:
#include <math.h>
#include <iostream>
//Service specific
#include <cxq41_ps2/SinComponent.h>
//Global Variable Initiation
std_msgs::Float64 commander_amplitude; //Global variable for storing amplitude
std_msgs::Float64 commander_frequency; //Global variable for storing frequency
std_msgs::Float64 sin_output; //Global Variable for the sinusoidal output
const double PI  =3.141592653589793238463; //Yep too fucking lazy to figure out proper way to express pi!

//Service callback
bool callback(cxq41_ps2::SinComponentRequest& request, cxq41_ps2::SinComponentResponse& response) {
    //Get amplitude
    commander_amplitude.data = request.amplitude;
    ROS_INFO("Obtained amplitude is: %f", commander_amplitude.data);
    //Get frequency
    commander_frequency.data = request.frequency;
    ROS_INFO("Obtained frequency is: %f", commander_frequency.data);
    response.obtained = true;
}


int main(int argc, char **argv) {
    //Node Initiation
    ros::init(argc, argv, "sin_commander"); //name this node 

    //node handler
    ros::NodeHandle nh_commander; // node handle

    //Announce the service
    ros::ServiceServer service = nh_commander.advertiseService("SinComponentExchange",callback); //Get info from service

    //Announcer for topic
    ros::Publisher sinWave_outputter = nh_commander.advertise<std_msgs::Float64>("vel_cmd", 1);

    //Initiate sin_output
    sin_output.data = 0.00;

    //Initiate Time for sine generation:
    double current_time; // Initate the time
    current_time = 0.00; //Resets time to 0

    //This will both set the sleeper rate as well as the "T" for the sin wave generator
    double dt = 0.01;
    double sample_rate = 1.0 / dt; 
    ros::Rate naptime(sample_rate);


    while (ros::ok()) {
        //sin wave is generated below:
        sin_output.data = commander_amplitude.data * sin(2.00 * commander_frequency.data * PI * current_time);
        current_time = current_time + dt; //time is ticking!
        sinWave_outputter.publish(sin_output);
        ROS_INFO("Current sin_ouput is: %f", sin_output.data); //Debug info spit!
        //----Do not change below as they are critical for operation normally
        ros::spinOnce(); 
        naptime.sleep();
    }
    return 0; // Not used unless some dummy kills roscore 
} 
