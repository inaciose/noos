#include "defs.h"
#include "proc.h"
#include "syscall.h"

int sys_hello(void) {
    uart_puts("SYSCALL: hello world\r\n");
    return 0;
}

static int (*syscalls[])(void) = {
    [SYS_fork]      0,
    [SYS_exit]      0,
    [SYS_wait]      0,
    [SYS_pipe]      0,
    [SYS_read]      0,
    [SYS_kill]      0,
    [SYS_exec]      0,
    [SYS_fstat]     0,
    [SYS_chdir]     0,
    [SYS_dup]       0,
    [SYS_getpid]    0,
    [SYS_sbrk]      0,
    [SYS_sleep]     0,
    [SYS_uptime]    0,
    [SYS_open]      0,
    [SYS_write]     0,
    [SYS_mknod]     0,
    [SYS_unlink]    0,
    [SYS_link]      0,
    [SYS_mkdir]     0,
    [SYS_close]     0,
    [SYS_hello]     sys_hello,
};

void syscall(void) {
    uint num;
    int ret;

    num = curproc->tf->r0;
    if ((num > 0) && (num <= NELEM(syscalls) && syscalls[num])) {
        ret = syscalls[num]();

        if (num != SYS_exec) {
            curproc->tf->r0 = ret;
        }
    } else {
        uart_puts("unknown or not implement sys call: ");
        print_hex(num);
        uart_puts("curproc->pid: ");
        print_hex(curproc->pid);
        uart_puts("curproc->name: ");
        uart_puts(curproc->name);
        uart_puts("\r\n");
        curproc->tf->r0 = -1;
    }
}
