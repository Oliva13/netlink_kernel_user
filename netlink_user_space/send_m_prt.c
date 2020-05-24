#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <asm/types.h>
#include <linux/netlink.h>

#define NETLINK_M	 25
#define MAX_PAYLOAD	 1024  // maximum payload size

#define M_GROUP	16


struct msghdr msg;

int main(int argc, char *argv[])
{
    struct sockaddr_nl src_addr, dest_addr;
    struct nlmsghdr *nlh = NULL;
    struct iovec iov;
    int sock_fd;
    int status = 0;

    sock_fd = socket(PF_NETLINK, SOCK_RAW, NETLINK_M);
    if(sock_fd < 0)
    {
        perror("Cannot open netlink socket");
        return -1;
    }

    memset(&src_addr, 0, sizeof(src_addr));

    src_addr.nl_family = AF_NETLINK;
    src_addr.nl_pid = getpid();
    src_addr.nl_groups = M_GROUP;

    status = bind(sock_fd, (struct sockaddr*)&src_addr, sizeof(src_addr));
    if(status)
    {
	perror("Cannot bind netlink socket");
	return -1;
    }


    memset(&dest_addr, 0, sizeof(dest_addr));


    nlh = (struct nlmsghdr *)malloc(NLMSG_SPACE(MAX_PAYLOAD));
    memset(nlh, 0, NLMSG_SPACE(MAX_PAYLOAD));

    nlh->nlmsg_len = NLMSG_SPACE(MAX_PAYLOAD);
    nlh->nlmsg_pid = getpid();
    nlh->nlmsg_flags = 0;


    iov.iov_base = (void *)nlh;
    iov.iov_len = nlh->nlmsg_len;

    msg.msg_name = (void *)&dest_addr;
    msg.msg_namelen = sizeof(dest_addr);
    msg.msg_iov = &iov;
    msg.msg_iovlen = 1;

//    printf("Sending message to kernel\n");

//    status = sendmsg(sock_fd, &msg, 0);
  
//    if (status < 0) 
//    {
//	perror("Cannot talk to netlink");
//	return -1;
//    }

    printf("Waiting for message from kernel\n");
  
    // Read message from kernel
    status = recvmsg(sock_fd, &msg, 0);
    if (status < 0) 
    {
        fprintf(stderr, "netlink receive error %s (%d)\n", strerror(errno), errno);
        return -1;
    }

    if (status == 0) 
    {
	fprintf(stderr, "EOF on netlink\n");
	return -1;
    }
   
  
    printf("Received message payload: %s\n", (char *)NLMSG_DATA(nlh));
  
    close(sock_fd);
}


