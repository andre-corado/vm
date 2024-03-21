#include <linux/module.h>
#include <linux/init.h>
#include <linux/proc_fs.h>
#include <linux/sched/signal.h>
#include <linux/seq_file.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Andre Corado");
MODULE_DESCRIPTION("Informacion cpu");
MODULE_VERSION("1.0");

static int escribir_a_proc(struct seq_file *file_proc, void *v)
{
    struct task_struct *task;

    seq_printf(file_proc, "{\n\"pids\":[\n");

    // Iterar sobre cada proceso
    for_each_process(task) {
        seq_printf(file_proc, "%d", task->pid);
        if (task->tasks.next != &init_task.tasks)
            seq_puts(file_proc, ",");
        seq_putc(file_proc, '\n');
    }

    seq_printf(file_proc, "]\n}\n");

    return 0;
}

static int abrir_aproc(struct inode *inode, struct file *file)
{
    return single_open(file, escribir_a_proc, NULL);
}

static const struct proc_ops archivo_operaciones = {
    .proc_open = abrir_aproc,
    .proc_read = seq_read,
};

static int __init modulo_init(void)
{
    proc_create("getallpids", 0, NULL, &archivo_operaciones);
    printk(KERN_INFO "Insertar Modulo CPU\n");
    return 0;
}

static void __exit modulo_cleanup(void)
{
    remove_proc_entry("getallpids", NULL);
    printk(KERN_INFO "Remover Modulo CPU\n");
}

module_init(modulo_init);
module_exit(modulo_cleanup);
