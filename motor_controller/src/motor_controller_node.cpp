#include <ros/ros.h>
#include <std_msgs/String.h>
#include <sstream>
#include <fcntl.h>

#define GPIO_DIRECTION_PATH "/sys/class/gpio/gpio%d/direction"
#define GPIO_VALUE_PATH "/sys/class/gpio/gpio%d/value"
#define GPIO_EXPORT_PATH "/sys/class/gpio/export"
#define BUFFER 60

#define PIN 23


int gpio_init(int pin_num);
int gpio_on(int pin_num, int val);

int main(int argc, char **argv){
	int sig = 1;
	int gpio_err = 0;
	int gpio_check =0;

	ros::init(argc, argv, "motor_controller_node");
	ros::NodeHandle nh;
	ros::Publisher chatter_pub = nh.advertise<std_msgs::String>("say_gpio", 1000);
	ros::Rate loop_rate(5);
	int count = 0;

	gpio_err = gpio_init(PIN);

	while(ros::ok() && gpio_err == 1){

		std_msgs::String msg;
		std::stringstream ss;
		ss << "Signal: " << sig;
		msg.data = ss.str();
		ROS_INFO("%s", msg.data.c_str());
		chatter_pub.publish(msg);
		ros::spinOnce();
		loop_rate.sleep();
		if(gpio_err == 1)	gpio_check = gpio_on(PIN, sig);
		else	std::cout << "GPIO Error\n";
		sig = 1 - sig;
	}
	return 0;
}

int gpio_init(int pin_num){
	char gpio_dir_path[BUFFER];
	char gpio_val_path[BUFFER];
	char gpio_exp_path[BUFFER];

	int fd_export, fd_val, fd_dir = -1;
	int err;
	int exp_check = 0;

	sprintf(gpio_exp_path, GPIO_EXPORT_PATH);
	sprintf(gpio_dir_path, GPIO_DIRECTION_PATH, pin_num);
	sprintf(gpio_val_path, GPIO_VALUE_PATH, pin_num);

	fd_dir = open(gpio_dir_path, O_WRONLY);

	while(fd_dir < 0){
		fd_export = open(gpio_exp_path, O_WRONLY);

		if(fd_export < 0)	return -1;
		else if(exp_check != 1){
			char buf[15];
			sprintf(buf, "%d", pin_num);
			err = write(fd_export, buf, sizeof(buf));
			if(err == -1)	return -1;
			exp_check = 1;
		}
		close(fd_export);
		fd_dir = open(gpio_dir_path, O_WRONLY);
	}

	err = write(fd_dir, "out", sizeof("out"));
	if(err == -1)	return -1;
	close(fd_dir);

	fd_val = open(gpio_val_path, O_WRONLY);
	if(fd_val < 0)	return -1;
	close(fd_val);

	return 1;
}

int gpio_on(int pin_num, int val){
	char gpio_val_path[BUFFER];

	int fd_val;
	int err;

	sprintf(gpio_val_path, GPIO_VALUE_PATH, pin_num);
	fd_val = open(gpio_val_path, O_WRONLY);
	if(fd_val < 0)	return -1;

	char buf;
	sprintf(&buf, "%d", val);
	err = write(fd_val, &buf, sizeof(&buf));
	if(err == -1)	return err;
	close(fd_val);

	return 1;
}
