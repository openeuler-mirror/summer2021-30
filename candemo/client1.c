#include <stdio.h>
#include <net/if.h>
#include <stdlib.h>
#include <linux/can.h>
#include <linux/can/raw.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
int main()
{
    int s,nbytes;
    struct sockaddr_can addr;
    struct ifreq ifr;
    struct can_frame frame[2]= {{0}};
    s = socket(PF_CAN, SOCK_RAW, CAN_RAW);
    strcpy(ifr.ifr_name, "vcan0");
    ioctl(s, SIOGIFINDEX, &ifr);
    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;
    bind(s, (struct sockaddr*)&addr, sizeof(addr));
    setsockopt(s, SOL_CAN_RAW, CAN_RAW_FILTER, NULL, 0);
    frame[0].can_id = 0x11;
    frame[0].data[0] = 'Y';
    frame[0].can_dlc = 1;
    frame[1].can_id = 0x22;
    frame[1].data[0] = 'N';
    frame[1].can_dlc = 1;
    while(1)
    {
        nbytes = write(s, &frame[0], sizeof(frame[0]));
        if(nbytes != sizeof(frame[0]))
        {
            printf("Send error frame[0]\n");
            //break;
        }
        sleep(1);
        nbytes = write(s, &frame[1], sizeof(frame[1]));
        if(nbytes != sizeof(frame[1]))
        {
            printf("Send error frame[1]\n");
            //break;
        }
        sleep(1);
    }
    close(s);
    return 0;
}