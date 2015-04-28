#include "ros/ros.h"
#include "std_msgs/String.h"

int main(int argc, char **argv)
{
  ros::init(argc, argv, "simple_publisher");
  ros::NodeHandle n;
  ros::Rate loop_rate(10);

  ros::Publisher chatter_pub = n.advertise<std_msgs::String>("chatter", 1000);

  ROS_INFO("-- Starting simple publisher node");

  while (ros::ok())
  {
    std_msgs::String msg;
		msg.data = "Hello World!";
		chatter_pub.publish(msg);

    ros::spinOnce();
    loop_rate.sleep();
  }

  return 0;
}
