#include<iostream>
#include<sys/types.h>
#include<unistd.h>
int main() {
	pid_t p = fork();
	if (p == 0) {
		while (true) {
			sleep(2);
			std::cout << "i am child1" << std::endl;

		}
	} else {
		pid_t t = fork();
		if (t == 0) {
			sleep(2);
			std::cout << "i am child2" << std::endl;
			std::cout << 5/0;
		} else {
			while (true) {
				sleep(2);
				std::cout << "i am father" << std::endl;
			}
		}
	}
}
