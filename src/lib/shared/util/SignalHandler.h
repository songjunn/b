#include <signal.h>
#include "logger.h"
#include "errno.h"

/*
(1) SIGHUP：当用户退出Shell时，由该Shell启的发所有进程都退接收到这个信号，默认动作为终止进程。
(2) SIGINT：用户按下<Ctrl + C>组合键时，用户端时向正在运行中的由该终端启动的程序发出此信号。默认动作为终止进程。
(3) SIGQUIT：当用户按下<Ctrl + />组合键时产生该信号，用户终端向正在运行中的由该终端启动的程序发出此信号。默认动作为终止进程并产生core文件。
(4) SIGILL ：CPU检测到某进程执行了非法指令。默认动作为终止进程并产生core文件。
(5) SIGTRAP：该信号由断点指令或其他trap指令产生。默认动作为终止进程并产生core文件。
(6) SIGABRT：调用abort函数时产生该信号。默认动作为终止进程并产生core文件。
(7) SIGBUS：非法访问内存地址，包括内存地址对齐（alignment）出错，默认动作为终止进程并产生core文件。
(8) SIGFPE：在发生致命的算术错误时产生。不仅包括浮点运行错误，还包括溢出及除数为0等所有的算术错误。默认动作为终止进程并产生core文件。
(9) SIGKILL：无条件终止进程。本信号不能被忽略、处理和阻塞。默认动作为终止进程。它向系统管理员提供了一种可以杀死任何进程的方法。
(10) SIGUSR1：用户定义的信号，即程序可以在程序中定义并使用该信号。默认动作为终止进程。
(11) SIGSEGV：指示进程进行了无效的内存访问。默认动作为终止进程并使用该信号。默认动作为终止进程。
(12) SIGUSR2：这是另外一个用户定义信号，程序员可以在程序中定义并使用该信号。默认动作为终止进程。
(13) SIGPIPE：Broken pipe：向一个没有读端的管道写数据。默认动作为终止进程。
(14) SIGALRM：定时器超时，超时的时间由系统调用alarm设置。默认动作为终止进程。
(15) SIGTERM：程序结束(terminate)信号，与SIGKILL不同的是，该信号可以被阻塞和处理。通常用来要求程序正常退出。执行Shell命令kill时，缺省产生这个信号。默认动作为终止进程。
(16) SIGSTKFLT：
(17) SIGCHLD：子程序结束时，父进程会收到这个信号。默认动作为忽略该信号。
(18) SIGCONT：让一个暂停的进程继续执行。
(19) SIGSTOP：停止(stopped)进程的执行。注意它和SIGTERM以及SIGINT的区别：该进程还未结束，只是暂停执行。本信号不能被忽略、处理和阻塞。默认作为暂停进程。
(20) SIGTSTP：停止进程的动作，但该信号可以被处理和忽略。按下<Ctrl + Z>组合键时发出该信号。默认动作为暂停进程。
(21) SIGTTIN：当后台进程要从用户终端读数据时，该终端中的所有进程会收到SIGTTIN信号。默认动作为暂停进程。
(22) SIGTTOU：该信号类似于SIGTIN，在后台进程要向终端输出数据时产生。默认动作为暂停进程。
(23) SIGURG：套接字（socket）上有紧急数据时，向当前正在运行的进程发出此信号，报告有紧急数据到达。默认动作为忽略该信号。
(24) SIGXCPU：进程执行时间超过了分配给该进程的CPU时间，系统产生该信号并发送给该进程。默认动作为终止进程。
(25) SIGXFSZ：超过文件最大长度的限制。默认动作为yl终止进程并产生core文件。
(26) SIGVTALRM：虚拟时钟超时时产生该信号。类似于SIGALRM，但是它只计算该进程占有用的CPU时间。默认动作为终止进程。
(27) SIGPROF：类似于SIGVTALRM，它不仅包括该进程占用的CPU时间还抱括执行系统调用的时间。默认动作为终止进程。
(28) SIGWINCH：窗口大小改变时发出。默认动作为忽略该信号。
(29) SIGIO：此信号向进程指示发出一个异步IO事件。默认动作为忽略。
(30) SIGPWR：关机。默认动作为终止进程。
(31) SIGSYS：
(34) SIGRTMIN~SIGRTMAX：Linux的实时信号，它没有固定的含义(或者说可以由用户自由使用)。注意，Linux线程机制使用了前3个实时信号。所有的实时信号的默认动作都是终止进程。

 程序不可捕获、阻塞或忽略的信号有：SIGKILL,SIGSTOP
 不能恢复至默认动作的信号有：SIGILL,SIGTRAP
 默认会导致进程流产的信号有：SIGABRT、SIGBUS、SIGFPE、SIGILL、SIGIOT、SIGQUIT、SIGSEGV、SIGTRAP、SIGXCPU、SIGXFSZ
 默认会导致进程退出的信号有：SIGALRM、SIGHUP、SIGINT、SIGKILL、SIGPIPE、SIGPOLL、SIGPROF、SIGSYS、SIGTERM、SIGUSR1、SIGUSR2、SIGVTALRM
 默认会导致进程停止的信号有：SIGSTOP、SIGTSTP、SIGTTIN、SIGTTOU
 默认进程忽略的信号有：SIGCHLD、SIGPWR、SIGURG、SIGWINCH
 
 在Shell下输入kill –l 可显示Linux 系统支持的全部信号 */


//不可靠信号处理
void unsigHandler(int signo)
{
	//Log.Error("[Signal] Recv Singal:%d", signo);
}

//不可靠信号注册
void unsigRegister()
{
#ifdef __linux__
	struct sigaction actions;

	bzero(&actions, sizeof(actions));
	sigemptyset(&actions.sa_mask);
	actions.sa_handler = unsigHandler;

	for(int sig = SIGHUP; sig <= SIGRTMAX; ++sig)
	{
		if (sig == SIGSEGV || sig == SIGFPE || sig == SIGILL || sig == SIGBUS 
				|| sig == SIGUSR1 || sig == SIGUSR2 || sig == SIGRTMIN ) {
			continue;
		}
		sigaction(sig, &actions, NULL);
	}

	sigset_t bset, oset;

    sigemptyset(&bset);
    sigaddset(&bset, SIGUSR1);
    sigaddset(&bset, SIGUSR2);
    sigaddset(&bset, SIGRTMIN);
    if (pthread_sigmask(SIG_BLOCK, &bset, &oset) != 0)
    	LOGGER_ERROR("Set pthread mask failed");
#endif
}

