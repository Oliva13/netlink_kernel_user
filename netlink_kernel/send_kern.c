#include <linux/module.h>
#include <net/sock.h>
#include <linux/netlink.h>
#include <linux/skbuff.h>
#include <linux/delay.h>

#define NETLINK_SWM  25
#define MAX_PAYLOAD 1024

#define M_GROUP	5

#define _MAX_M_GROUP  	32

#define MAX_M_GROUP	(_MAX_M_GROUP - 1)


struct sock *nl_sk = NULL;

static void netlink_test(void)
{
    struct sk_buff *skb_out = NULL;
    struct nlmsghdr *nlh = NULL;
    int err;
    u32 pid;
    int msg_size;

    char *msg ="Hello from kernel";
   
    printk("Entering: %s\n",__FUNCTION__);
    
    msg_size = strlen(msg);
    
    skb_out = nlmsg_new(msg_size, 0);
    if(skb_out == NULL)
    {
	printk("\n [KERNEL]: alloc_skb failed \n");
    }

    nlh = nlmsg_put(skb_out, 0, 0, NLMSG_DONE, msg_size, 0);

    NETLINK_CB(skb_out).pid = 0;  // from kernel 
    NETLINK_CB(skb_out).dst_group = M_GROUP;

    printk("\n [KERNEL]: sending msg......\n");
    
    strncpy(nlmsg_data(nlh), msg, msg_size);
        
    ssleep(15);
        
    err = netlink_broadcast(nl_sk, skb_out, 0, M_GROUP, GFP_KERNEL);

    if(err < 0)
    {
	printk(KERN_ALERT "Error durning broadcast: %d\n",err);
	if(err == -3)
	{
	    printk(KERN_ALERT "no such process\n");
	}
    }
    else
    {
	printk(KERN_INFO "broadcasted message\n");
    }
}

void knetlink_input( struct sk_buff * skb)
{
    printk("\nFunction %s() called", __FUNCTION__);
}


static int __init m_netlink_init(void)
{
    printk("Entering: %s\n",__FUNCTION__);

    if (nl_sk != NULL)
    {
	printk("knetlink_init: sock already present\n");
	return 1;
    }
    nl_sk = netlink_kernel_create(&init_net, NETLINK_M, MAX_M_GROUP, knetlink_input, NULL, THIS_MODULE);
    if ( nl_sk == NULL )
    {
	printk("knetlink_init: sock fail\n");
	return 1;
    }

    printk("knetlink_init: sock %p\n", (void*)nl_sk );
    printk(KERN_INFO "starting netlink_test\n");
    netlink_test();
    return 0;
}

static void __exit m_netlink_exit(void)
{
    printk("Exiting: %s\n", __FUNCTION__);
   
    if ( nl_sk != NULL )
    {
        printk("knetlink_exit: release sock %p\n", (void*)nl_sk);
	netlink_kernel_release(nl_sk);  
        
    }
    else
    {
        printk("knetlink_exit: warning sock is NULL\n");
    }
}

module_init(m_netlink_init);
module_exit(m_netlink_exit);

MODULE_LICENSE("GPL");
