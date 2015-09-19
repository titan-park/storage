#include<fcntl.h>

#define GPIO_DIRECTION_PATH "/sys/class/gpio/gpio%d/direction"
#define GPIO_VALUE_PATH "/sys/class/gpio/gpio%d/value"
#define GPIO_EXPORT_PATH "/sys/class/gpio/export"
#define BUFFER 60

int gpio_init(int pin_num, int dir);
int gpio_write(int pin_num, int val);
int gpio_read(int pin_num);

int gpio_init(int pin_num, int dir){
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

	if(dir == 1 ){
		err = write(fd_dir, "out", sizeof("out"));
		if(err == -1)	return -1;
	}
	else{
		err = write(fd_dir, "in", sizeof("in"));
		if(err == -1)	return -1;
	}
	close(fd_dir);

	fd_val = open(gpio_val_path, O_WRONLY);
	if(fd_val < 0)	return -1;
	close(fd_val);

	return 1;
}

int gpio_write(int pin_num, int val){
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

int gpio_read(int pin_num){
	char gpio_val_path[BUFFER];
	int fd_val;
	int err;

	sprintf(gpio_val_path, GPIO_VALUE_PATH, pin_num);
	fd_val = open(gpio_val_path, O_WRONLY);
	if(fd_val < 0)	return -1;

	char buf[10];
	err = read(fd_val, &buf, sizeof(&buf));
	if(err == -1)	return err;

	close(fd_val);

	return (int)(buf[0]);

}
