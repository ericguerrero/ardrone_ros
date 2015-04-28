#include "ros/ros.h"
#include "ar_pose/ARMarkers.h"
#include "geometry_msgs/Twist.h"

#include <dynamic_reconfigure/server.h>
#include <ardrone_sim/dynparamsConfig.h>

float x, y, z;
float x_d, y_d, z_d;
float ex, ey, ez;



void callback(ardrone_sim::dynparamsConfig &config, uint32_t level)
{
    x_d= config.x_d;
    y_d= config.y_d;
    z_d= config.z_d;
}


void frameCallback(const ar_pose::ARMarkers::ConstPtr& frame)
{

    x= frame->markers.at(0).pose.pose.position.x;
    y= frame->markers.at(0).pose.pose.position.y;
    z= frame->markers.at(0).pose.pose.position.z;
    ROS_INFO(">>Desired  Position: [x, y, z] = [%f, %f, %f]",x_d, y_d, z_d);
    ROS_INFO(">>Actual   Position: [x, y, z] = [%f, %f, %f]",x,y,z);

    ex=x-x_d;
    ey=y-y_d;
    ez=z-z_d;
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "ardrone_sim_node");
    ros::NodeHandle n;
    ros::Rate loop_rate(1000);

    ros::Subscriber sub = n.subscribe("ar_pose_marker", 1000, frameCallback);

    dynamic_reconfigure::Server<ardrone_sim::dynparamsConfig> server;
    dynamic_reconfigure::Server<ardrone_sim::dynparamsConfig>::CallbackType f;
    f = boost::bind(&callback, _1, _2);
    server.setCallback(f);

    ros::Publisher cmd_vel_pub = n.advertise<geometry_msgs::Twist>("cmd_vel", 1000);
    ros::Publisher error_pub = n.advertise<geometry_msgs::Twist>("error", 1);

    while (ros::ok())
    {
        geometry_msgs::Twist error;
        error.linear.x=ex;
        error.linear.y=ey;
        error.linear.z=ez;
        error_pub.publish(error);


        //Declares the message to be sent
        geometry_msgs::Twist cmd_vel;
        if ( ex>0.2 ) {
          cmd_vel.linear.x=-0.5;
            ROS_INFO("ex>0.2");
        }
        else if ( ex<-0.2 ) {
          cmd_vel.linear.x=0.5;
          ROS_INFO("ex<-0.2");
        }
        if ( ey>0.2 ) {
          cmd_vel.linear.y=-0.5;
          ROS_INFO("ey>0.2");
        }
        else if ( ey<-0.2 ) {
          cmd_vel.linear.y=0.5;
          ROS_INFO("ey<-0.2");
        }
        if ( ez>0.2 ) {
          cmd_vel.linear.z=-0.5;
          ROS_INFO("ez>0.2");
        }
        else if ( ez<-0.2 ) {
          cmd_vel.linear.z=0.5;
          ROS_INFO("ez<-0.2");
        }
        //Publish the message
        cmd_vel_pub.publish(cmd_vel);

        ros::spinOnce();
        loop_rate.sleep();
    }
    return 0;
}
