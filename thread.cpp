#include<pthread.h>
#include<iostream>
#include<unistd.h>
void *thread1(void *arg) {
	while (true) {
		sleep(2);
		std::cout << "i am child1" << std::endl;
	}
}
void *thread2(void *arg) {
	while (true) {
		sleep(2);
		std::cout << "i am child2" << std::endl;
		std::cout << 5/0;
	}
}
int main() {
	pthread_t tid[2];
	pthread_create(&tid[0],NULL,thread1,NULL);
	pthread_create(&tid[1],NULL,thread2,NULL);
	while (true) {
		sleep(2);
		std::cout << "i am father" << std::endl;
	}
}	
