#include "ros/ros.h"
#include <math.h>
#include "geometry_msgs/Twist.h"
#include "nav_msgs/Odometry.h"
#include "std_msgs/String.h"

using namespace std;

double X;
double Y;
double x_start;
double y_start;
int state=2;

void odomcallBack(nav_msgs::Odometry msg)
{
    X = msg.pose.pose.position.x;
    Y = msg.pose.pose.position.y;
    printf("%lf %lf\n",X,Y);
}

int main(int argc, char **argv)
{

  ros::init(argc,argv,"eight");

  ros::NodeHandle n;
  ros::Publisher vel_pub_0 = n.advertise<geometry_msgs::Twist>("/swarmbot0/cmd_vel",1);
  ros::Subscriber sub=n.subscribe<nav_msgs::Odometry>("/swarmbot0/odom",1000,odomcallBack);
  geometry_msgs::Twist cmd_vel;
  
  int count = 0;
  double z=1.8;
  ros::Rate loop_rate(10);

  while (ros::ok())
  {
  	
  	cmd_vel.linear.x = 2;
  	cmd_vel.linear.y = 0;
  	cmd_vel.linear.z = 0;
  	cmd_vel.angular.x = 0;
  	cmd_vel.angular.y = 0;
  	if(state==2)
  	{
  		x_start = X;
    	y_start = Y;
    	cmd_vel.angular.z = z;
  		printf("state- %d z: %lf \n", state,z);
    }
    if((abs(x_start-X)<0.01) &&(abs(y_start-Y)<0.01 ))
  	{
  		state=1;
  		if(state==0)
  		{
  			state=1;
  			x_start = X;
    		y_start = Y;
    		z=-z;
    		cmd_vel.angular.z = z;
  			printf("state- %d z: %lf \n", state,z);
    	}
	}
    else
    	state=0;

    vel_pub_0.publish(cmd_vel);
    ros::spinOnce();
    loop_rate.sleep();
    count++;
  }
  return 0;
}
