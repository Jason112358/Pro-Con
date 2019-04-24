#include <pthread.h>
#include <iostream>
#include <ctime>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
using namespace std;

#define BUFFER_SIZE 5

pthread_mutex_t mutex;
sem_t emp;
sem_t full;

void *producer(void *arg);
void *consumer(void *arg);

//insert function
int insert_item(int line[]) {
	for (int i1 = 0; i1 < BUFFER_SIZE; i1++) {
		if (line[i1] == -1) {
			line[i1] = rand()%100;
			for (int count = 0; count < BUFFER_SIZE; count++) {
				if (line[count] == -1) cout << "INV" << '\t';
				else cout << line[count] << '\t';
			}
			cout << "||||||||||---Producer produce " << line[i1] << "\n";
			return 0;
		}
	}
	cout << endl;
	return -1;
}

//remove function
int remove_item(int line[]) {
	int tmp;
	for (int i1 = 0; i1 < BUFFER_SIZE; i1++) {
		if (line[i1] != -1) {
			tmp = line[i1];
			line[i1] = -1;
			for (int count = 0; count < BUFFER_SIZE; count++) {
				if (line[count] == -1) cout << "INV" << '\t';
					else cout << line[count] << '\t';
			}
			cout << "||||||||||---Consumer consume " << tmp<<"\n";
			return 0;
		}
	}
	return -1;
}

//producer
void *producer(void *arg)
{
	int *mypara = (int*)arg;
	int times = 10;
	srand(time(0));
	do {
		sem_wait(&emp);
		pthread_mutex_lock(&mutex);
		insert_item(mypara);
        pthread_mutex_unlock(&mutex);
		sem_post(&full);
        sleep(0.01);
	} while (times);
	return 0;
}

//consumer
void *consumer(void *arg)
{
	int *mypara = (int*)arg;
	int times = 10;
	do {
		sem_wait(&full);
		pthread_mutex_lock(&mutex);
		remove_item(mypara);
		pthread_mutex_unlock(&mutex);
		sem_post(&emp);
        sleep(0.01);
	} while (times);
	return 0;
}

int main()
{
	// Creation of the buffer zone
	// Initialization of product line
	int product[BUFFER_SIZE] = { -1,-1,-1,-1,-1 };
	int num_pro = 10;
	int num_con = 10;
	int time_thr = 60;
	int n_pro = 0;
	int n_con = 0;

    pthread_mutex_init(&mutex,NULL);
    sem_init(&emp,0,5);
    sem_init(&full,0,0);

	//Get the command of user
	cout << "How many producers do you want?(no more than 10)";
	cin >> num_pro;
	cout << "How many consumers do you want?(no more than 10)";
	cin >> num_con;
	cout << "How long do you want to run the program?(no more than 60)";
	cin >> time_thr;

    pthread_t pro[11];
    pthread_t con[11];

	//Create the producer and the customer threads
	for (; n_pro < num_pro;n_pro++) {
        pro[n_pro] = pthread_create(&pro[n_pro], NULL, producer, product);
	}
	for (; n_con < num_con; n_con++) {
		con[n_con] = pthread_create(&con[n_con], NULL, consumer, product);
	}

	//Close the threads
	sleep(time_thr);
	for (n_pro = 0; n_pro < num_pro; n_pro++) {
		pthread_join(pro[n_pro],NULL);
	}
	for (n_con = 0; n_con < num_con; n_con++) {
		pthread_join(con[n_con],NULL);
    }

	system("pause");
	return 0;
}
