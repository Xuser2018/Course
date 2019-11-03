/**
 * Kernel module that communicates with /proc file system.
 *
 * This provides the base logic for Project 2 - displaying task information
 */

#include <linux/init.h>
#include <linux/slab.h>
#include <linux/sched.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/vmalloc.h>
#include <asm/uaccess.h>

#define BUFFER_SIZE 128
#define PROC_NAME "pid"

/* the current pid */
static long l_pid;

/**
 * Function prototypes
 */
static ssize_t proc_read(struct file *file, char *buf, size_t count, loff_t *pos);
static ssize_t proc_write(struct file *file, const char __user *usr_buf, size_t count, loff_t *pos);

static struct file_operations proc_ops = {
        .owner = THIS_MODULE,
        .read = proc_read,
        .write = proc_write,
};

/* This function is called when the module is loaded. */
static int proc_init(void)
{
        // creates the /proc/procfs entry
        proc_create(PROC_NAME, 0666, NULL, &proc_ops);

        printk(KERN_INFO "/proc/%s created\n", PROC_NAME);

	return 0;
}

/* This function is called when the module is removed. */
static void proc_exit(void) 
{
        // removes the /proc/procfs entry
        remove_proc_entry(PROC_NAME, NULL);

        printk( KERN_INFO "/proc/%s removed\n", PROC_NAME);
}

/**
 * This function is called each time the /proc/pid is read.
 * 
 * This function is called repeatedly until it returns 0, so
 * there must be logic that ensures it ultimately returns 0
 * once it has collected the data that is to go into the 
 * corresponding /proc file.
 */
static ssize_t proc_read(struct file *file, char __user *usr_buf, size_t count, loff_t *pos)
{
        int rv = 0;
        
        // buffer_point +=1000;
        static int completed = 0;
        struct task_struct *tsk = NULL;

        if (completed) {
                completed = 0;
                return 0;
        }    

        tsk = pid_task(find_vpid(1), PIDTYPE_PID);
        struct list_head *children = &tsk->children;
        printk("command = [%s] pid = [%d] state=[%ld]\n",tsk->comm,tsk->pid,tsk->state);
        // struct list_head *next = children->next;
        // tsk = list_entry(&children, struct task_struct, sibling);   
        int i = 0;
        int j = 0;
        char buffer[BUFFER_SIZE]; //buffsize 128
        char *usr_point = usr_buf;
        char *buffer_point = buffer;
        for_each_process(tsk){
        //     rv = printk("command = [%s] pid = [%d] state=[%ld]\n",tsk->comm,tsk->pid,tsk->state);
            rv = sprintf(usr_point,"command = [%s] pid = [%d] state=[%ld]\n",tsk->comm,tsk->pid,tsk->state);
        //     rv = sprintf(buffer_point,"command = [%s] pid = [%d] state=[%ld]\n",tsk->comm,tsk->pid,tsk->state);
            usr_point += rv;
        //     buffer_point += rv;
        //     rv = sprintf(buffer_point,"command = [%s]",tsk->comm);
        //     buffer_point += rv;
        //     i += rv;
        //     rv = sprintf(buffer_point,"pid = [%d] ",tsk->pid);
        //     buffer_point += rv;
        //     i += rv;
        //     rv = sprintf(buffer_point,"state=[%ld]\n",tsk->state);
        //     buffer_point += rv;
        //     copy_to_user(usr_buf, buffer, rv);
        //     rv = sprintf(buffer,"comman\n");
        //     buffer_point += 40;
            j+=1;
            i += rv;
        //     printk("%d ",rv);
        }
        // printk("%d %d\n",i,j);
        // copy_to_user(usr_buf, buffer, i);
        // rv = 0;
        // tsk = pid_task(find_vpid(l_pid), PIDTYPE_PID);

        completed = 1;
        if(tsk == NULL)
                rv = sprintf(buffer,"PID doesn't exit\n");
        else;
                // rv = sprintf(buffer,"command = [%s] pid = [%d] state=[%ld]\n",tsk->comm,tsk->pid,tsk->state);
                // rv = sprintf(buffer_point,"success");
                
            
        // copies the contents of kernel buffer to userspace usr_buf 
        // if (copy_to_user(usr_buf, buffer, rv)) {
        //         rv = -1;
        // }

        return i;
}

/**
 * This function is called each time we write to the /proc file system.
 */
static ssize_t proc_write(struct file *file, const char __user *usr_buf, size_t count, loff_t *pos)
{
        char *k_mem;

        // allocate kernel memory
        k_mem = kmalloc(count, GFP_KERNEL);

        /* copies user space usr_buf to kernel buffer */
        if (copy_from_user(k_mem, usr_buf, count)) {
		printk( KERN_INFO "Error copying from user\n");
                return -1;
        }

	/**
 	 * kstrol() will not work because the strings are not guaranteed
	 * to be null-terminated.
	 * 
	 * sscanf() must be used instead.
	 */
        sscanf(usr_buf,"%ld",&l_pid);
        // kstrtol(usr_buf,10,&l_pid);
        printk("%ld\n",l_pid);
        // printk("%s\n",usr_buf);
        kfree(k_mem);

        return count;
}

/* Macros for registering module entry and exit points. */
module_init( proc_init );
module_exit( proc_exit );

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Module");
MODULE_AUTHOR("SGG");

