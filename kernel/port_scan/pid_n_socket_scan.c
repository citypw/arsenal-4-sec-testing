/* This program can find out which process is using socket port. This
 * might be useful for digging out rootkit, which being activity on
 * network! But I'm not sure about it.
 *
 * monkey-coder: vedasolutions,
 *               Shawn the R0ck@copyleft, <citypw@gmail.com>
 *  
 * This code is also GPL'ed!
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/pid.h>
#include <linux/net.h>
#include <linux/sched.h>
#include <linux/fdtable.h>
#include <net/inet_sock.h>

/* trying to find out the connections between process and listening
   port */

static int pid_n_socket_scan(int pidnr)
{
	struct file *file;
	struct socket *sock;
	int fd = 0;
	struct inode *inode;
	struct task_struct *task;
	struct pid *mypid;
	struct files_struct *files;
	struct inet_sock *inet;

        /* find pid via find_pid_ns(), check valid pid? */
	mypid = find_vpid(pidnr);
	if (!mypid) {
		return -ESRCH;
	}

	/* find the task_struck by pid */
	task = pid_task(mypid, PIDTYPE_PID);
	if (!task) {
		printk("\nNo Such Process\n");
		return -ESRCH;
	}

	files = task->files;

	rcu_read_lock();
	file = fcheck_files(files, fd);
	rcu_read_unlock();
	while (file) {
		inode = file->f_path.dentry->d_inode;
                /* is socket or not? */
		if (S_ISSOCK(inode->i_mode)) {
			sock = file->private_data;
			inet = inet_sk(sock->sk);
			printk("Name:%s, pid:%d, type = %d, sport=%d, dport=%d\n", task->comm,
			       task->pid, sock->type, ntohs(inet->inet_sport), ntohs(inet->inet_dport));
		}
		fd++;
		rcu_read_lock();
		file = fcheck_files(files, fd);
		rcu_read_unlock();
	}

	return 0;
}

int mod_init(void)
{
	struct task_struct *task_list;
	
	for_each_process(task_list){
		pid_n_socket_scan(task_list->pid);
	}

	return 0;
}

void mod_exit(void)
{

}

module_init(mod_init);
module_exit(mod_exit);

MODULE_LICENSE("GPL");
