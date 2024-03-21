#include <linux/module.h>
// para usar KERN_INFO
#include <linux/kernel.h>
// para info_ram
#include <linux/mm.h>

//Header para los macros module_init y module_exit
#include <linux/init.h>
//Header necesario porque se usara proc_fs
#include <linux/proc_fs.h>
/* for copy_from_user */
#include <asm/uaccess.h>
/* Header para usar la lib seq_file y manejar el archivo en /proc*/
#include <linux/seq_file.h>

#include <linux/fs.h>


#define MEMINFO_PATH "/proc/meminfo"


const long bytes_to_mb = 1024 * 1024;

// Obteniendo estadísticas del sistema
struct sysinfo si;


MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Modulo de RAM, Laboratorio Sistemas Operativos 1");
MODULE_AUTHOR("andre-corado");

//Funcion que se ejecutara cada vez que se lea el archivo con el comando CAT
static int escribir_archivo(struct seq_file *archivo, void *v)
{
    unsigned long total, used, notused;
    unsigned long porc;
    si_meminfo(&si);

    total = si.totalram * si.mem_unit / bytes_to_mb;
    used = (si.freeram * si.mem_unit + si.bufferram * si.mem_unit + si.sharedram * si.mem_unit)/ bytes_to_mb;
    porc = (used * 100) / total;
    notused = total - used;

    seq_printf(archivo, "{\"totalRam\":%lu, \"memoriaEnUso\":%lu, \"ram\":%lu, \"libre\":%lu, ", total, used, porc, notused);

    return 0;
}

//Funcion que se ejecutara cada vez que se lea el archivo con el comando CAT
static int al_abrir(struct inode *inode, struct file *file)
{
    return single_open(file, escribir_archivo, NULL);
}

//Si el kernel es 5.6 o mayor se usa la estructura proc_ops
static struct proc_ops operaciones =
{
    .proc_open = al_abrir,
    .proc_read = seq_read
};

//Funcion a ejecuta al insertar el modulo en el kernel con insmod
static int _insert(void)
{
    proc_create("ram_so1_1s2024", 0, NULL, &operaciones);
    printk(KERN_INFO "202100154\n");
    return 0;
}

//Funcion a ejecuta al remover el modulo del kernel con rmmod
static void _remove(void)
{
    remove_proc_entry("ram_so1_1s2024", NULL);
    printk(KERN_INFO "202100154\n");
}

module_init(_insert);
module_exit(_remove);
