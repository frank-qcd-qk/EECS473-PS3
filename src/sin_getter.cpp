// code written by Frank Qian CXQ41@case.edu for EECS 473 Homework PS2:
// This node is responsible for getting values and pass along: 
#include <ros/ros.h>
#include <std_msgs/Float64.h> 
#include <string>
//IO specific
#include <iostream>
#include <math.h>
//Service specificfrequency
#include <cxq41_ps2/SinComponent.h>

int main(int argc, char **argv) {
   //Node Initiation
    ros::init(argc, argv, "sin_getter");
    
    //node handler
    ros::NodeHandle getter_nh;
    
    //Announce the service
    ros::ServiceClient client = getter_nh.serviceClient<cxq41_ps2::SinComponent>("SinComponentExchange"); //publish info to service

    //Message type specification
    cxq41_ps2::SinComponent srv;

    while (ros::ok()) {
        //Get input for amplitude
        srv.request.amplitude = 0.00;
        std::cout << std::endl;
        std::cout << "Please input the amplitude you want to set.......(Use CTRL+C to Quit)" << std::endl << "Input: ";
        std::cin >> srv.request.amplitude;
        while(std::cin.fail()) {
            ROS_ERROR("Input is not valid! Please try again....(Use CTRL+C to Quit)");
            std::cout<< "Input: ";
            std::cin.clear();
            std::cin.ignore(256,'\n');
            std::cin >> srv.request.amplitude;
        }
        ROS_INFO("The amplitude you set is: %f", srv.request.amplitude);
        //Get input for frequency:
        srv.request.frequency = 0.00;
        std::cout << std::endl;
        std::cout << "Please input the frequency you want to set.......(Use CTRL+C to Quit)" << std::endl << "Input: ";
        std::cin >> srv.request.frequency;
        while(std::cin.fail()) {
            ROS_ERROR("Input is not valid! Please try again....(Use CTRL+C to Quit)");
            std::cout<< "Input: ";
            std::cin.clear();
            std::cin.ignore(256,'\n');
            std::cin >> srv.request.frequency;
        }
        ROS_INFO("The frequency you set is: %f", srv.request.frequency);

        //Confirmation of get
        if(client.call(srv)&&srv.response.obtained){
            ROS_INFO("Both input obtained! Handover to commander.......");
        } else {
            ROS_ERROR("Failed to call service SinComponentExchange");
            return 1;
        }
    }
    return 0; // Not used unless some dummy kills roscore 
}
