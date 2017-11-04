#include <ros/ros.h>
#include <sensor_msgs/PointCloud2.h>
#include <pcl_conversions/pcl_conversions.h>
#include <pcl/conversions.h> //<pcl/ros/conversions.h>
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>
#include <iostream>
#include <pcl/io/io.h>
#include <pcl/io/pcd_io.h>
#include <boost/lexical_cast.hpp>
#include "string"

class recordPC
{
public:	
	ros::NodeHandle& nodeHandle_;

  	ros::Subscriber PCsub_;

  	pcl::PCDWriter writer;

	int count;
	int maxsample;
	std::string modname;

	recordPC(ros::NodeHandle& nodeHandle): nodeHandle_(nodeHandle)
	{
		if (!readParameters()) {
	    ROS_ERROR("Could not read parameters.");
	    ros::requestShutdown();
	  }
	  count=0;
	  PCsub_=nodeHandle_.subscribe("/pc_tut/cluster0",1,&recordPC::recordCB,this);
	  ROS_INFO("Successfully launchednode.");



	}
	virtual ~recordPC()
	{
	}

	
	bool readParameters()
	{
		if (!nodeHandle_.getParam("maxsample",
                            maxsample))
    		return false;
    	if (!nodeHandle_.getParam("modname",
                            modname))
    		return false;

    	return true;


	}

	void recordCB(const sensor_msgs::PointCloud2ConstPtr& input)
	{
		pcl::PointCloud<pcl::PointXYZ> cloud;
		pcl::fromROSMsg (*input, cloud);
		std::stringstream ss;
		ss<<"cloud_cluster_"<<modname<<"_"<<count<<".pcd";
		writer.write<pcl::PointXYZ>(ss.str(),cloud,false);
		count++;

	}


	


};

int main(int argc, char** argv)
{
  ros::init(argc, argv, "ras_group8_PC_record");
  ros::NodeHandle nodeHandle("~");
  ros::Rate loop_rate(1.0);
  recordPC rd(nodeHandle);

  for (;;) {
      ros::spinOnce();
      loop_rate.sleep();

  }
  ros::spin();
  return 0;
}



