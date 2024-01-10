#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>

#define NUM_READER 3
#define NUM_ITRETION 5

pthread_mutex_t mutex;
pthread_barrier_t barrier;
int shread_data;

void *writer(void *arg){
	for(int i=0; i< NUM_ITRETION; i++){
		int random = rand()%100;
		pthread_mutex_lock(&mutex);
		shread_data = random;
		printf("\nwriter writes %d \n",random);
		pthread_mutex_unlock(&mutex);
		sleep(1);
	}
	
}

void *reader(void *arg){
	int reader_id = *((int *)arg); 
	for(int i=0;i<NUM_ITRETION;i++){
		int data;
		pthread_barrier_wait(&barrier);
		
		pthread_mutex_lock(&mutex);
		data = shread_data;
		printf("\nreader :%d reads %d",reader_id,data);
		pthread_mutex_unlock(&mutex);
		sleep(1);
	}
}


int main(){
	pthread_t writer_thread,reader_thread[NUM_READER];
	int reader_id[NUM_READER] = {1,2,3};
	
	pthread_barrier_init(&barrier,NULL,NUM_READER);
	
	pthread_create(&writer_thread,NULL,writer,NULL);
	for(int i=0;i<NUM_READER;i++){
		pthread_create(&reader_thread[i],NULL,reader,&reader_id[i]);
	}
	
	pthread_join(writer_thread,NULL);
	for(int i=0;i<NUM_READER;i++){
		pthread_join(reader_thread[i],NULL);
	}
	
	pthread_barrier_destroy(&barrier);
}

