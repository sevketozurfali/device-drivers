#include "header.h"
#include "data-types.h"

#define WR_DATA _IOW('a', 'a', data_t*)
#define RD_DATA _IOR('a', 'b', data_t*)

int main() {
    int fd;
    int32_t val,num;

    printf("\n IOCTL based character device driver operation from user space...\n");

    fd = open("/dev/ioctl_device", O_RDWR);

    if(fd < 0){
        
        printf("cannot open the device driver.\n");
        return 0;
    }

    struct data_t number;
    number.num1 = 10;
    number.num2 = 20;

    printf("Enter the data to send ... \n");
    

    printf("Writing value to the driver...\n");

    ioctl(fd, WR_DATA, &number);

    printf("Reading value from driver...\n");

    number.num1 = 0;
    number.num2 = 0;

    ioctl(fd, RD_DATA, &number);

    printf("Data number: %d - %d\n", number.num1, number.num2 );

    printf("Closing the driver... \n");

    close(fd);


}