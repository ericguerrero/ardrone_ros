#include <ros/ros.h>

#include <dynamic_reconfigure/server.h>
#include <dynrec/desposeConfig.h>

#include <ar_pose/ARMarkers.h>

// Declare global variables
std::vector<double> desired_pose_;
bool print_screen_;

void dynrec_callback(dynrec::desposeConfig &config, uint32_t level)
{
	ROS_INFO("New Desired position: (%f,%f,%f). Print screen: %s", config.x, config.y, config.z,	config.print_screen ? "True" : "False");

	// Store new acquired values
	desired_pose_.clear();
	desired_pose_.push_back(config.x);
	desired_pose_.push_back(config.y);
	desired_pose_.push_back(config.z);
	print_screen_ = config.print_screen;
}

void ARtag_callback(const ar_pose::ARMarkers::ConstPtr& msg)
{
  //ROS_INFO("ArtagPbvsAlgNode::input_ARtag_callback: New Message Received");

	if (msg->markers.empty()){

  	// No marker detected
    ROS_INFO("Nor marker detected");
  }
  else {

  	// Get current detected position
  	std::vector<double> curr_pose(3);
  	curr_pose[0] = msg->markers[0].pose.pose.position.x;
  	curr_pose[1] = msg->markers[0].pose.pose.position.y;
  	curr_pose[2] = msg->markers[0].pose.pose.position.z;

  	// Compute error between desired and current positions
  	std::vector<double> error(3);
  	for (int ii = 0; ii < curr_pose.size(); ++ii)
  		error[ii] = curr_pose[ii]-desired_pose_[ii];

  	// Compute the norm of the error
  	double norm = sqrt(pow(error[0],2)+pow(error[1],2)+pow(error[2],2));

  	// Print screen if requested
  	if (print_screen_)
  		ROS_INFO("Norm of the error between desired and current marker pose: %f",norm);

  }
}


int main(int argc, char **argv)
{
	ros::init(argc, argv, "dynrec_node");
	ros::NodeHandle n;
	ros::Rate loop_rate(10);

	desired_pose_.resize(3);
	print_screen_ = false;

	ros::Subscriber sub = n.subscribe("/ar_pose_marker", 1000, ARtag_callback);

	dynamic_reconfigure::Server <dynrec::desposeConfig> server;
	dynamic_reconfigure::Server <dynrec::desposeConfig>::CallbackType f;

	f = boost::bind(&dynrec_callback, _1, _2);
	server.setCallback(f);

	ROS_INFO("Pose error node Spinning");
	while (ros::ok())
	{
		ros::spinOnce();
		loop_rate.sleep();
	}

	return 0;
}
