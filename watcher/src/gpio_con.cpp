#include "ros/ros.h"
#include "std_msgs/String.h"
#include "gpio.h"
#include <sstream>
#include <string>
#include <iostream>
using namespace std;

#define PIN_L 23
#define PIN_R 24
#define PIN_E 25

int gpio_err[3] = {0, 0, 0};
int gpio_check[3] ={0, 0, 0};

void chatterCallback(const std_msgs::String::ConstPtr& msg){

	string tmp = msg->data;

	ROS_INFO("Data: [%s]", tmp.c_str());

	if(gpio_err[0] == 1 && gpio_err[1] == 1 && gpio_err[2] == 1){
		const char *tmp_c = tmp.c_str();

		if(tmp_c[0] == 'w'){
			gpio_write(PIN_L, 1);
			gpio_write(PIN_R, 1);
			cout << "Forward" << endl;
		}
		else if(tmp_c[0] == 'a'){
			gpio_write(PIN_L, 0);
			gpio_write(PIN_R, 1);
			cout << "Left" << endl;
		}
		else if(tmp_c[0] == 'd'){
			gpio_write(PIN_L, 1);
			gpio_write(PIN_R, 0);
			cout << "Right" << endl;
		}
		else if(tmp_c[0] == 's'){
			gpio_write(PIN_L, 0);
			gpio_write(PIN_R, 0);
			cout << "Stop" << endl;
		}
	}

}

int main(int argc, char **argv){

	ros::init(argc, argv, "listen");

	ros::NodeHandle n;

	ros::Subscriber sub = n.subscribe("chatter", 1000, chatterCallback);
	gpio_err[0] = gpio_init(PIN_L, 1);
	gpio_err[1] = gpio_init(PIN_R, 1);
	gpio_err[2] = gpio_init(PIN_E, 1);
	ros::spin();

	return 0;
}
