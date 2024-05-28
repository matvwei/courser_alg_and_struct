

#include <iostream>
#include <iterator>
#include <ctime>
#include <stdio.h> 
#include <time.h> 
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <locale>
#include <cstring>

using namespace std;



//курсовая работа
struct PetTemp
{
	int id;
	string name;
	PetTemp* next;
	PetTemp* prev;
};


//Структура с чанками для оптимизации
struct ChunksPets {
	int id;
	ChunksPets* nextPetChunk;
	ChunksPets* prevPetChunk;
	PetTemp* petInHead;
};
//Самый первый элемент
PetTemp* headPetTemp;

//Создание предмета
PetTemp* createPetTemp(int id, string name)
{
	struct PetTemp* pet = new PetTemp;

	pet->id = id;
	pet->name = name;
	pet->next = NULL;
	pet->prev = NULL;
	return pet;
}

//Добавление предмета в массив
PetTemp* addPetTemp(int id, string name, PetTemp* h) {
	PetTemp* newPet = createPetTemp(id, name);

	if (h != NULL) {
		while (h->next)
		{
			h = h->next;
		}
		newPet->prev = h;
		h->next = newPet;

	}
	else {
		headPetTemp = newPet;
	}
	return newPet;
}

//Добавление элемента в начало
void addFirstTemp(int id, string name, PetTemp*& h) {
	PetTemp* tempPet = createPetTemp(id, name);
	tempPet->next = h;
	headPetTemp = tempPet;
}

//Вывод элементов
void coutPetTemp(PetTemp* h) {
	while (h)
	{
		cout << h->id << " : " << h->name << "\n";
		h = h->next;
	}
}


//Поиск элементов
PetTemp* searchPetTemp(int id, PetTemp* h) {
	PetTemp* temp = NULL;
	while (h)
	{
		if (h->id == id) {
			temp = h;
			break;
		}
		h = h->next;
	}
	return temp;
}

//Удаление элементов
static bool deletePetTemp(int id, PetTemp* h) {
	PetTemp* temp = NULL;
	if (h->id == id) {
		headPetTemp = h->next;
		return true;
	}
	while (h->next) {
		temp = h->next;
		if (temp->id == id) {
			h->next = temp->next;
			delete temp;

			return true;
			break;
		}
		h = h->next;
	}
	return false;
}


/*Удаление элемента по номеру
* Можно передать чанк и номер элемента, который в данном чанке
*/
bool deletePetTempForNumber(int number, PetTemp* h) {
	PetTemp* temp = NULL;
	int temp_number = 0;
	if (number==1) {
		return deletePetTemp(h->id, headPetTemp);
	}
	while (h->next) {
		temp_number++;
		temp = h->next;
		if (temp_number == number-1) {
			return deletePetTemp(temp->id, headPetTemp);
		}
		h = h->next;
	}
	return false;
}

ChunksPets* headChunk;

//Создание чанка
ChunksPets* createChunk(int id, PetTemp* pet)
{
	struct ChunksPets* chunk = new ChunksPets;

	chunk->id = id;
	chunk->petInHead = pet;
	chunk->nextPetChunk = NULL;
	chunk->prevPetChunk = NULL;

	return chunk;
}


//Добавление чанка в стек
ChunksPets* addChunk(int id, PetTemp* pet, ChunksPets* hChunk) {
	ChunksPets* newChunk = createChunk(id, pet);

	if (hChunk != NULL) {
		while (hChunk->nextPetChunk)
		{
			hChunk = hChunk->nextPetChunk;
		}
		if (id > 1) {
			newChunk->prevPetChunk = hChunk;
		}
		hChunk->nextPetChunk = newChunk;

	}
	else {
		headChunk = newChunk;
	}
	return newChunk;
}


//Поиск чанка
ChunksPets* searchChunk(int number, ChunksPets* hChunk) {
	ChunksPets* temp = NULL;

	while (hChunk)
	{
		if (hChunk->id == number) {
			temp = hChunk;
			break;
		}
		hChunk = hChunk->nextPetChunk;
	}
	return temp;

}

//Вывод активных чанков
void coutChunks(ChunksPets* h) {
	while (h)
	{
		cout << h->id << " : " << h->petInHead->name << "\n";
		h = h->nextPetChunk;
	}
}

/*Перемещение первых элементов в предыдущий чанк
* Передавать чанк, после которого нужно передвинть все первые элементы в предыдущий чанк
* и поставить заместо первого элемента второй
* если чанк пустой, то он удаляется
*/

bool moveFirstPetInChunk(ChunksPets*& chunk) {
	ChunksPets* temp = NULL;
	PetTemp* tempPet = NULL;

	while (chunk->nextPetChunk) {
		temp = chunk->nextPetChunk;
		if (temp->petInHead) {
			tempPet = temp->petInHead;
			if (tempPet->next) {
				temp->petInHead = tempPet->next;
			}
			else {
				chunk->nextPetChunk = NULL;
				delete temp;
				return true;
			}
		}
		else {
			chunk->nextPetChunk = NULL;
			return true;
		}
		chunk = chunk->nextPetChunk;
	}
	return false;
}


/* Удаление элемента из чанка
*/
bool deletePetWithChunks(int number, PetTemp* h, ChunksPets*& chunk) {
	PetTemp* temp = NULL;
	int temp_number = 0;
	if (number == 1) {
		if (h->next) {
			if (h->prev) {
				h->next->prev = h->prev;
				h->prev->next = h->next;
			}
			else {
				h->next->prev = NULL;
				headPetTemp = h->next;
			}
			chunk->petInHead = h->next;
			moveFirstPetInChunk(chunk);

		}
		else {
			
			chunk->prevPetChunk->nextPetChunk = NULL;
			h->prev->next = NULL;
		}
		delete temp;
		return true;
	}
	while (h->next) {

		temp_number++;
		temp = h->next;
		if (temp_number == number - 1) {
			if (temp->next) {
				temp->next->prev = temp->prev;
				temp->prev->next = temp->next;
				moveFirstPetInChunk(chunk);
			}
			else {
				temp->prev->next = NULL;
			}
			delete temp;
			return true;
			//return deletePetTemp(temp->id, h);
		}
		h = h->next;
	}
	return false;
}



static int round_up(double value) {
	return (int)ceil(value);
}

//Деленение с окргулнением вниз
static int div_floor(double divined,double divider) {
	return (int)floor(divined / divider);
}


//Деленение с окргулнением вверх
static int div_ceil(double divined, double divider) {
	return (int)ceil(divined / divider);
}

int read_file(string filename = "clients.txt", int chunk_size = 5000) {
	
	ifstream file("clients.txt");

	// Проверяем, успешно ли открылся файл
	if (!file.is_open()) {
		cerr << "Не удалось открыть файл." << endl;
		return 0;
	}

	string line;

	int size_arr = 0;
	while (getline(file, line)) {
		// Используем stringstream для разбивки строки на номер и имя клиента

		stringstream ss(line);
		string client_number, client_name;

		// Извлекаем номер и имя клиента
		getline(ss, client_number, ':');
		getline(ss, client_name);

		PetTemp* tempPet = addPetTemp(size_arr + 1, client_name, headPetTemp);

		size_arr++;
	}

	file.close();

	return size_arr;
}


int read_file_chunks(string filename = "clients.txt", int chunk_size = 5000) {
	ifstream file(filename);

	// Проверяем, успешно ли открылся файл
	if (!file.is_open()) {
		cerr << "Не удалось открыть файл." << endl;
		return 0;
	}
	string line;

	int size_arr = 0;
	int temp_chunk_size = 0;

	// Читаем строки из файла
	while (getline(file, line)) {
		// Используем stringstream для разбивки строки на номер и имя клиента

		stringstream ss(line);
		string client_number, client_name;

		// Извлекаем номер и имя клиента
		getline(ss, client_number, ':');
		getline(ss, client_name);

		PetTemp* tempPet = addPetTemp(size_arr + 1, client_name, headPetTemp);
		if (size_arr == chunk_size * temp_chunk_size) {
			addChunk(temp_chunk_size + 1, tempPet, headChunk);
			//chunks_petTemp[temp_chunk_size ] = tempPet;
			temp_chunk_size++;
		}
		size_arr++;
	}

	file.close();


	return size_arr;
}

int inside_calc_delete(int chunk_size, int num_chunk, int temp_size, int last_delete) {
	ChunksPets* tempChunk = NULL;
	if (temp_size > chunk_size)
	{
		num_chunk = div_ceil((double)(temp_size), (double)chunk_size);
	}

	tempChunk = searchChunk(num_chunk, headChunk);

	int tempDiv = temp_size % chunk_size;
	//Проверка на то, что это последний элемент в чанке
	if (tempDiv == 0) tempDiv = chunk_size;

	deletePetWithChunks(tempDiv, tempChunk->petInHead, tempChunk);
	return temp_size;
}

bool dzozefusWithChunk() {
	
	cout << "Программа по реализоации алгоритма Джозефуса"<<endl;
	cout << "Введите название файла или оставьте пустым для открытия стандартного файла" << endl;
	char input[100];
	std::cin.getline(input, 100);
	string file_name = input;
	if (strlen(input) < 1) file_name = "clients.txt";
	//Размер чанка
	int chunk_size = 500;

	//Размер массива с записями
	int size_arr = read_file_chunks(file_name, chunk_size);
	if (size_arr < 1) {
		cout << "Файл не содержит записи";
		return false;
	}
	//Количество чанков динаминчное
	int temp_chunk_size = div_ceil(size_arr, chunk_size);

	//coutChunks(headChunk);

	cout << endl << "Начала алгоритма:" << endl;
	clock_t start = clock();
	//coutPetTemp(headPetTemp);
	//Размер массива в основными элементами
	int temp_size = size_arr;

	int last_delete = 0;

	for (int i = 1; i < size_arr; i++)
	{
		int randint = rand() % size_arr + 1;
		//cout << "random = " << randint;
		int num_chunk = 1;
		


		//Остаток от размера массива
		int dif = (last_delete + randint) % temp_size;
		//Тот случай когда остатка нет это 9/9 или 18/9
		if (dif == 0) {
			//Если рандомное число + последний удалившийся номер больше и остаток = 0, чем размер массива данных, нужный номер это размер массива
			if (randint + last_delete >= temp_size) {
				last_delete = inside_calc_delete (chunk_size,num_chunk, temp_size, last_delete);
			}
			else {
				last_delete = inside_calc_delete(chunk_size, num_chunk, randint + last_delete, last_delete);
			}
		}
		else {
			last_delete = inside_calc_delete( chunk_size, num_chunk, dif, last_delete);
		}
		temp_size--;
		//cout << " last_delete = " << last_delete << endl;
		//coutPetTemp(headPetTemp);
		//cout << endl << endl;
		//coutChunks(headChunk);

	}

	cout << endl << "Результат алгоритма Джозефуса"<<endl<<endl;
	coutPetTemp(headPetTemp);
	cout << endl;

	clock_t end = clock();
	double seconds = (double)(end - start) / CLOCKS_PER_SEC;
	printf("The time: %f seconds\n", seconds);

	return true;

}

bool dzozefus() {
	//Алгоритм без чанков
	headPetTemp = NULL;

	int size_arr =read_file("clients.txt");
	int temp_size = size_arr;

	temp_size = size_arr;


	//for (int i = 0; i < size_arr; i++)
	//{
	//	PetTemp* tempPet = addPetTemp(i + 1, "test_" + to_string(i + 1), headPetTemp);
	//}

	cout << endl << "Start array:" << endl;
	clock_t start = clock();

	//coutPetTemp(headPetTemp);
	//cout << endl << "algorithm:" << endl << "=========================" << endl << endl;


	int last_delete = 0;

	for (int i = 1; i < size_arr; i++)
	{
		int randint = rand() % size_arr + 1;

		int dif = (last_delete + randint) % temp_size;
		if (dif == 0) {
			if (randint + last_delete >= temp_size) {
				deletePetTempForNumber(temp_size, headPetTemp);
				last_delete = temp_size;
			}
			else {
				deletePetTempForNumber(randint + last_delete, headPetTemp);
				last_delete = randint;
			}
		}
		else {
			deletePetTempForNumber(dif, headPetTemp);
			last_delete = dif;
		}


		temp_size--;
		//cout << "random = " << randint << " last_delete = " << last_delete << endl;
		//coutPetTemp(headPetTemp);
	}
	cout << endl << "=========================" << endl;

	cout << endl << "Result:" << endl;
	coutPetTemp(headPetTemp);


	clock_t end = clock();
	double seconds = (double)(end - start) / CLOCKS_PER_SEC;
	printf("The time: %f seconds\n", seconds);

	return true;
}

int main()
{
	setlocale(LC_ALL, "Russian");

	srand(time(0));

	//clock_t start = clock();

	dzozefusWithChunk();

	//clock_t end = clock();
	//double seconds = (double)(end - start) / CLOCKS_PER_SEC;
	//printf("The time: %f seconds\n", seconds);


	//clock_t start2 = clock();

	//dzozefus();
	//
	//clock_t end2 = clock();
	//double seconds2 = (double)(end2 - start2) / CLOCKS_PER_SEC;
	//printf("The time: %f seconds\n", seconds2);

	exit(0);
}

