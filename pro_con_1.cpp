#include <windows.h>
#include <iostream>
#include <ctime>
using namespace std;

//typedef int buffer_item;
#define BUFFER_SIZE 5	

HANDLE emp = CreateSemaphore(NULL, 5, 5, NULL);
HANDLE full = CreateSemaphore(NULL, 0, 5, NULL);
HANDLE mutex = CreateMutex(NULL, NULL, NULL);

DWORD WINAPI producer(PVOID para);
DWORD WINAPI consumer(PVOID para);

//insert function
int insert_item(int line[]) {
	for (int i1 = 0; i1 < BUFFER_SIZE; i1++) {
		if (line[i1] == -1) {
			line[i1] = rand()%100+i1;
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
// remove function
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

DWORD WINAPI producer(PVOID para)
{
	int *mypara = (int*)para;
	int times = 10;
	srand(time(0));
	do {
		WaitForSingleObject(emp, INFINITE);
		WaitForSingleObject(mutex, INFINITE);
		insert_item(mypara);
		ReleaseSemaphore(full, 1, NULL);
		ReleaseSemaphore(emp, -1, NULL);
		ReleaseMutex(mutex);
	} while (times--);
	return 0;
}

DWORD WINAPI consumer(PVOID para)
{
	int *mypara = (int*)para;
	int times = 10;
	do {
		WaitForSingleObject(full, INFINITE);
		WaitForSingleObject(mutex, INFINITE);
		remove_item(mypara);
		ReleaseSemaphore(emp, 1, NULL);
		ReleaseSemaphore(full, -1, NULL);
		ReleaseMutex(mutex);
	} while (times--);
	return 0;
}

int main(int argc, char *argv[])
{
	// Creation of the buffer zone
	// Initialization of product line
	int product[BUFFER_SIZE] = { -1,-1,-1,-1,-1 };
	int num_pro = 10;
	int num_con = 10;
	int time_thr = 60;
	int n_pro = 0;
	int n_con = 0;
	HANDLE pro[11];
	HANDLE con[11];

	//srand(time(0));

	//Get the command of user
	cout << "How many producers do you want?(no more than 10)";
	cin >> num_pro;
	cout << "How many consumers do you want?(no more than 10)";
	cin >> num_con;
	cout << "How long do you want to run the program?(no more than 60)";
	cin >> time_thr;

	//Create the producer and the customer threads
	for (; n_pro < num_pro;n_pro++) {
		pro[n_pro] = CreateThread(NULL, 0, producer, product, 0, NULL);
	}
	for (; n_con < num_con; n_con++) {
		con[n_con] = CreateThread(NULL, 0, consumer, product, 0, NULL);
	}


	Sleep(time_thr * 100);

	//Wait for the threads
	WaitForMultipleObjects(11, pro, TRUE, 0);
	WaitForMultipleObjects(11, pro, TRUE, 0);

	//Close the handles
	for (n_pro = 0; n_pro < num_pro; n_pro++) {
		CloseHandle(pro[n_pro]);
	}
	for (n_con = 0; n_con < num_con; n_con++) {
		CloseHandle(con[n_con]);
	}

	system("pause");
	return 0;
}