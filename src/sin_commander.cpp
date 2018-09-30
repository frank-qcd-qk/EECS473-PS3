// code written by Frank Qian CXQ41@case.edu for EECS 473 Homework PS1 node: 
#include <ros/ros.h> 
#include <std_msgs/Float64.h> 
#include <string>

//Project specific include:
#include <math.h>
#include <iostream>

std_msgs::Float64 amplitude; //Global variable for storing amplitude
std_msgs::Float64 frequency; //Global variable for storing frequency
std_msgs::Float64 sin_output; //Global Variable for the sinusoidal output
const double PI  =3.141592653589793238463; //Yep too fucking lazy to figure out proper way to express pi!

int main(int argc, char **argv) {
    //----Do not change below as they are critical for operation normally
    ros::init(argc, argv, "sin_commander"); //name this node 
    // when this compiled code is run, ROS will recognize it as a node called sin_commander" 
    ros::NodeHandle nh; // node handle
    //----Do not change above as they are critical for operation normally    
    //Announcer for topic
    ros::Publisher sinWave_outputter = nh.advertise<std_msgs::Float64>("vel_cmd", 1);

    //IO handler
    //Get input for amplitude:
    amplitude.data = 0.00;
    std::cout << std::endl;
    std::cout << "Please input the amplitude you want to set.......(Use CTRL+C to Quit)" << std::endl << "Input: ";
    std::cin >> amplitude.data;
    while(std::cin.fail()) {
        ROS_ERROR("Input is not valid! Please try again....(Use CTRL+C to Quit)");
        std::cout<< "Input: ";
        std::cin.clear();
        std::cin.ignore(256,'\n');
        std::cin >> amplitude.data;
    }
    ROS_INFO("The amplitude you set is: %f", amplitude.data);
    //Get input for frequency:
    frequency.data = 0.00;
    std::cout << std::endl;
    std::cout << "Please input the frequency you want to set.......(Use CTRL+C to Quit)" << std::endl << "Input: ";
    std::cin >> frequency.data;
    while(std::cin.fail()) {
        ROS_ERROR("Input is not valid! Please try again....(Use CTRL+C to Quit)");
        std::cout<< "Input: ";
        std::cin.clear();
        std::cin.ignore(256,'\n');
        std::cin >> frequency.data;
    }
    ROS_INFO("The frequency you set is: %f", frequency.data);
    ROS_INFO("Both input obtained! Now operating!.......");

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
        sin_output.data = amplitude.data * sin(2.00 * frequency.data * PI * current_time);
        current_time = current_time + dt; //time is ticking!
        sinWave_outputter.publish(sin_output);
        ROS_INFO("Current sin_ouput is: %f", sin_output.data); //Debug info spit!
        //----Do not change below as they are critical for operation normally
        ros::spinOnce(); 
        naptime.sleep();
    }
    return 0; // Not used unless some dummy kills roscore 
} 
