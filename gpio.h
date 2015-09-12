#include<fcntl.h>

#define GPIO_DIRECTION_PATH "/sys/class/gpio/gpio%d/direction"
#define GPIO_VALUE_PATH "/sys/class/gpio/gpio%d/value"
#define GPIO_EXPORT_PATH "/sys/class/gpio/export"
#define BUFFER 60
#define NUM_PIN 4
#define PIN214 214

int gpio_init(int pin_num, int direction){
	int pin_array[5] = {13, 182, 48, 49, 183};
	int check = 0;
	int i,j;
	int err;

	for(i=0; i<5;i++){
		if(pin_num == pin_array[i]){
			char gpio_dir_path[NUM_PIN][BUFFER];
			char gpio_val_path[NUM_PIN][BUFFER];
			char gpio_exp_path[BUFFER];

			int fd_export[NUM_PIN], fd_val[NUM_PIN], fd_dir[NUM_PIN];
			int err = 0;
			int gpio_pin[NUM_PIN];

			gpio_pin[0] = 214;
			gpio_pin[1] = 253+i;
			gpio_pin[2] = 221+i;
			gpio_pin[3] = pin_array[i];

			sprintf(gpio_exp_path, GPIO_EXPORT_PATH);
			for(j=0;j<NUM_PIN;j++){

				sprintf(gpio_dir_path[j], GPIO_DIRECTION_PATH, gpio_pin[j]);
				sprintf(gpio_val_path[j], GPIO_VALUE_PATH, gpio_pin[j]);

				fd_dir[j] = open(gpio_dir_path[j], O_WRONLY);

				if(fd_dir[j] < 0){

					fd_export[j] = open(gpio_exp_path, O_WRONLY);
					if(fd_export[j] < 0){
						check = -1;
					}
					else{
						char buf[15];
						sprintf(buf, "%d", gpio_pin[j]);
						err = write(fd_export[j], buf, sizeof(buf));
						if(err == -1){
							check = -1;
						}
					}
					err = close(fd_export[j]);
					fd_dir[j] = open(gpio_dir_path[j], O_WRONLY);
				}

			}

			err = write(fd_dir[0], "in", sizeof("in"));
			if(err == -1){
				check = -1;
			}

			err = write(fd_dir[1], "out", sizeof("out"));
			if(err == -1){
				check = -1;
			}

			err = write(fd_dir[2], "in", sizeof("in"));
			if(err == -1){
				check = -1;
			}

			err = write(fd_dir[3], "out", sizeof("out"));
			if(err == -1){
				check = -1;
			}

			err = write(fd_dir[0], "out", sizeof("out"));
			if(err == -1){
				check = -1;
			}

			for(j=0;j<NUM_PIN;j++){
				err = close(fd_dir[j]);
			}

			fd_val[0] = open(gpio_val_path[0], O_WRONLY);
			fd_val[1] = open(gpio_val_path[1], O_WRONLY);


			err = write(fd_val[0], "1", sizeof("1"));
			if(direction){
				err = write(fd_val[1], "1", sizeof("1"));
			}
			else{
				err = write(fd_val[0], "0", sizeof("0"));
			}
			err = close(fd_val[0]);
			err = close(fd_val[1]);

			if(check != -1)	check = 1;
		}
	}
	return check;
}

void gpio_write(int pin_num, int state){
	char gpio_val_path[BUFFER];
	int fd_val;
	int err;

	sprintf(gpio_val_path, GPIO_VALUE_PATH, pin_num);
	fd_val = open(gpio_val_path, O_WRONLY);
	if(state){
		err = write(fd_val, "1", sizeof("1"));
	}
	else{
		err = write(fd_val, "0", sizeof("1"));
	}
	err = close(fd_val);
}

int gpio_read(int pin_num){
	int val = 0;

	return val;

}
