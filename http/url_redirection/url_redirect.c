/* This program is trying to modify the http packet when the packet
 * across the netfilter hook. Unforunately, it still didn't work yet.
 *
 * monkey-coder: 
 * Copyright (C) 2012 Shawn the R0ck, <citypw@gmail.com>
 * 
 * This program is free software: you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see
 * <http://www.gnu.org/licenses/>.
 *
 */

#include <linux/module.h>
#include <linux/string.h>
#include <linux/kernel.h>
#include <linux/ip.h>		/* For IP header */
#include <net/ip.h> /* ip_hdrlen() */
#include <linux/tcp.h>		/* For TCP header */
#include <net/tcp.h> /* tcp_v4_check() */
#include <linux/skbuff.h>
#include <linux/netdevice.h>
#include <linux/netfilter.h>
#ifdef __KERNEL__
#undef __KERNEL__
#include <linux/netfilter_ipv4.h>
#endif

#define __KERNEL__

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Shawn the R0ck");

#define BE_PRINT_IP(addr) \
	((unsigned char*)&addr)[0], \
	((unsigned char*)&addr)[1], \
	((unsigned char*)&addr)[2], \
	((unsigned char*)&addr)[3]

#define BITS16_REVERSE(addr) (((addr << 8) | (addr >> 8)))

/* This is the structure we shall use to register our function */
static struct nf_hook_ops nfho;


/* IP address we want to drop packets from, in network-byte order(Big-endian) */
static unsigned char *drop_ip = "\x7f\x00\x00\x01";

struct app {
	char name[32];
	unsigned char *deny_port;
};

char *url = "192.168.0.200";

/*char *fake_req = "HTTP/1.1 200 OK\r\nRefresh: 0; url=http://192.168.0.100\r\nContent-Type: text/html; charset=iso-8859-1\r\nContent-length: 57\r\n\r\nPlease follow <a href=\"http://192.168.0.100\">link</a>!\r\n\r\n";*/

char *fake_req = "";
char *fake_req2 = "HTTP/1.1 301 Moved Permanently\r\nLocation: http://192.168.0.100/\r\nContent-Type: text/html\r\nTransfer-Encoding: chunked\r\n\r\n<html><head><title>A web page that has to be greater than 337 ladkj;sfiekdfancvlkjdsalfuioerfdaj</title><meta http-equiv=\"refresh\" content=\"0; URL=http://192.168.0.100\"><meta name=\"keywords\" content=\"automatic redirection\"></head><body>hello</body></html>";


char *redirect_url = "192.168.0.100";

struct app http_packet;

char *replace(char *st, char *orig, char *repl)
{
  static char buffer[1518];
  char *ch;
  if (!(ch = strstr(st, orig)))
    return st;

  strncpy(buffer, st, ch-st);
  buffer[ch-st] = 0;
  sprintf(buffer+(ch-st), "%s%s", repl, ch+strlen(orig));
  return buffer;
}

/* filtering by TCP protocol */
static int check_tcp_packet(unsigned int hooknum,
			    struct sk_buff *skb,
			    const struct net_device *in,
			    const struct net_device *out,
			    int (*okfn) (struct sk_buff *))
{
	struct tcphdr *thead;
	struct iphdr *iph;
	unsigned short uport = 0;
	unsigned char* payload = NULL;
	int payload_len;
	unsigned int modify = 0;
	unsigned short port_tmp;


	struct sk_buff *sb = skb;

	iph = ip_hdr(sb);
	payload = (char*)iph + (iph->ihl * 4); /* ip payload */
	payload_len = ntohs(iph->tot_len) - (iph->ihl * 4);

	/* Be sure this is a TCP packet */
	if (iph->protocol != IPPROTO_TCP) {
		return NF_ACCEPT;
	}

	thead = (struct tcphdr *) (sb->data + (iph->ihl * 4));
	payload += thead->doff * 4; /* tcp payload */
	payload_len -= thead->doff * 4;

	/* We don't want any NULL pointers in the chain to IP header. */
	if (!sb)
		return NF_ACCEPT;
	if (!iph)
		return NF_ACCEPT;


	if ((thead->dest) == *(unsigned short *) (http_packet.deny_port)) {
		uport = BITS16_REVERSE(thead->dest);
		printk
		    ("droped tcp packet from...IP:%d.%d.%d.%d Port:%u Application:%s\n",
		     BE_PRINT_IP(iph->saddr), uport, http_packet.name);
		/* If SYN packet: IP PACKET 60bytes - IP HEADER 20bytes - TCP HEADER 40 bytes = 0? WTH */
		//		payload_len = ntohs(iph->tot_len) - (iph->ihl * 4) - (thead->doff * 4); 

		if(payload_len > 3){

		  if(skb_linearize(sb) != 0){
		    return NF_ACCEPT;
		  }

		  thead->dest = thead->source;
		  thead->source = *http_packet.deny_port;

		  printk("payload:%s, payload len:%d\n", payload, payload_len);
		  /*		  if(memcmp(payload, "GET ", 4) == 0)
				  printk("GET REQUEST!\n");*/
		  if(strstr(payload, url) != NULL)
		    printk("acess to: %s\n", url);
		  /* 147.2.207.136 */
		  memcpy(&iph->daddr, "\xc0\xa8\x0\x64", 4);
		  strcpy(payload, replace(payload, url, redirect_url));
		  payload[payload_len] = '\0';
		  //memset(payload, 0x00, payload_len);
		  /*		  if(strlen(fake_req) > payload_len)
		    {
		      modify = strlen(fake_req) + 1 - payload_len;
		      printk("original payload_len:%d, fake_req:%d, modify:%d\n", payload_len, strlen(fake_req) + 1, modify);
		      payload_len = strlen(fake_req) + 1;
		      skb_put(sb, modify);
		    }
		    memcpy(payload, fake_req, strlen(fake_req) + 1);*/

		  sb->csum = skb_checksum(sb, iph->ihl * 4, sb->len - iph->ihl * 4, 0);
		  thead->check = csum_tcpudp_magic(iph->saddr, iph->daddr, sb->len - (iph->ihl * 4), iph->protocol, sb->csum);
		  //		  thead->check = csum_tcpudp_magic(iph->saddr, iph->daddr, payload_len, iph->protocol, csum_partial((char*)thead, payload_len, 0));
		  //		  sb->ip_summed = CHECKSUM_UNNECESSARY;
		  printk("------------------------------\n%s---------------------payload_len:%d\n", payload, payload_len);
		}
		return NF_ACCEPT;
	}
	return NF_ACCEPT;
}

static int init_filter_if()
{
	printk("initializing the hooks!\n");

	memset(&http_packet, 0x00, sizeof(http_packet));
	strcpy(http_packet.name, "http_packet");
	http_packet.deny_port = "\x00\x50";

	/* remember which hook you specified */
	nfho.hook = check_tcp_packet;
	nfho.hooknum = NF_IP_PRE_ROUTING;
	nfho.pf = PF_INET;	/* ipv4 protocols */
	nfho.priority = NF_IP_PRI_FIRST;

	nf_register_hook(&nfho);

	return 0;
}

static void cleanup_filter_if()
{
	printk("bye hooks\n");
	nf_unregister_hook(&nfho);
}

module_init(init_filter_if);
module_exit(cleanup_filter_if);
