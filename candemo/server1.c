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
    int s, nbytes;
    struct sockaddr_can addr;
    struct ifreq ifr;
    struct can_frame frame;
    struct can_filter rfilter[1];
    
    s = socket(PF_CAN, SOCK_RAW, CAN_RAW);
    strcpy(ifr.ifr_name, "vcan1");
    ioctl(s, SIOGIFINDEX, &ifr);
    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;
    bind(s, (struct sockaddr*)&addr, sizeof(addr));
    rfilter[0].can_id = 0x11;
    rfilter[0].can_mask = CAN_SFF_MASK;
    setsockopt(s, SOL_CAN_RAW, CAN_RAW_FILTER, &rfilter, sizeof(rfilter));
    while(1)
    {
        nbytes = read(s, &frame, sizeof(frame));
        if(nbytes > 0)
        {
            printf("ID=0x%X DLC=%d data[0]=0x%X\n", frame.can_id, frame.can_dlc, frame.data[0]);
        }
        
    }
    close(s);
    return 0;
}