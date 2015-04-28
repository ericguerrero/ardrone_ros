#include "ros/ros.h"
#include "std_msgs/String.h"

void chatterCallback(const std_msgs::String::ConstPtr& msg)
{
  ROS_INFO("I heard: [%s]", msg->data.c_str());
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "simple_subscriber");
  ros::NodeHandle n;
  ros::Rate loop_rate(10);

  ros::Subscriber sub = n.subscribe("chatter", 1000, chatterCallback);

  ROS_INFO("-- Starting simple subscriber node");

  while (ros::ok())
  {
    ros::spinOnce();
    loop_rate.sleep();
  }

  return 0;
}
