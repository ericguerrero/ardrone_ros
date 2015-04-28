#include <ros/ros.h>

#include <dynamic_reconfigure/server.h>
#include <dynrec/dynparamsConfig.h>

void callback(dynrec::dynparamsConfig &config, uint32_t level)
{
	ROS_INFO("Reconfigure Request: %d %f %s %s %d",
	            config.int_param, config.double_param,
	            config.str_param.c_str(),
	            config.bool_param?"True":"False",
	            config.size);
}

int main(int argc, char **argv)
{
	ros::init(argc, argv, "dynrec_node");
	ros::NodeHandle n;
	ros::Rate loop_rate(10);

	dynamic_reconfigure::Server < dynrec::dynparamsConfig > server;
	dynamic_reconfigure::Server<dynrec::dynparamsConfig>::CallbackType f;

	f = boost::bind(&callback, _1, _2);
	server.setCallback(f);

	ROS_INFO("dynrec_node Spinning");
	while (ros::ok())
	{
		ros::spinOnce();
		loop_rate.sleep();
	}

	return 0;
}
