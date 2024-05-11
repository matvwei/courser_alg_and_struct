

#include <iostream>
#include <string> 
#include <iterator>
#include <ctime>
#include <stdio.h> 
#include <time.h> 

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

bool dzozefusWithChunk() {
	


	int size_arr = 10000;

	//Размер массива в основными элементами
	int temp_size = size_arr;

	//Размер чанка
	int chunk_size = 500;

	PetTemp* tempPetT = NULL;

	//Количество чанков динаминчное
	int temp_chunk_size = 0;


	//Цикл по заполнение массива с основными элементами и заполнению чанков
	for (int i = 0; i < size_arr; i++)
	{
		PetTemp* tempPet = addPetTemp(i + 1, "test_" + to_string(i + 1), headPetTemp);
		if (i == chunk_size * temp_chunk_size) {
			addChunk(temp_chunk_size + 1, tempPet, headChunk);
			//chunks_petTemp[temp_chunk_size ] = tempPet;
			temp_chunk_size++;
		}

	}

	//coutChunks(headChunk);

	//cout << endl << "Start array:" << endl;
	//coutPetTemp(headPetTemp);

	int last_delete = 0;

	for (int i = 1; i < size_arr; i++)
	{
		int randint = rand() % size_arr + 1;
		//cout << "random = " << randint;
		int num_chunk = 1;
		ChunksPets* tempChunk = NULL;


		//Остаток от размера массива
		int dif = (last_delete + randint) % temp_size;
		if (dif == 0) {
			if (randint + last_delete >= temp_size) {

				if (temp_size > chunk_size)
				{
					num_chunk = div_ceil((double)(temp_size), (double)chunk_size);
				}

				tempChunk = searchChunk(num_chunk, headChunk);

				int tempDiv = temp_size % chunk_size;
				if (tempDiv == 0) tempDiv = chunk_size;

				deletePetWithChunks(tempDiv, tempChunk->petInHead, tempChunk);
				last_delete = temp_size;
			}
			else {
				if (randint + last_delete > chunk_size)
				{
					num_chunk = div_ceil((double)(randint + last_delete), (double)chunk_size);
				}

				tempChunk = searchChunk(num_chunk, headChunk);

				int tempDiv = temp_size % chunk_size;
				if (tempDiv == 0) tempDiv = chunk_size;

				deletePetWithChunks(randint + last_delete, tempChunk->petInHead, tempChunk);
				last_delete = randint + last_delete;
			}
		}
		else {
			if (dif > chunk_size)
			{

				num_chunk = div_ceil((double)(dif), (double)chunk_size);
			}

			tempChunk = searchChunk(num_chunk, headChunk);


			int tempDiv = dif % chunk_size;
			if (tempDiv == 0) tempDiv = chunk_size;

			deletePetWithChunks(tempDiv, tempChunk->petInHead, tempChunk);

			last_delete = dif;
		}
		temp_size--;
		//cout << " last_delete = " << last_delete << endl;
		//coutPetTemp(headPetTemp);
		//cout << endl << endl;
		//coutChunks(headChunk);

	}

	coutPetTemp(headPetTemp);


	return true;

}

bool dzozefus() {
	//Алгоритм без чанков

	int size_arr = 10000;
	int temp_size = size_arr;
	headPetTemp = NULL;

	for (int i = 0; i < size_arr; i++)
	{
		PetTemp* tempPet = addPetTemp(i + 1, "test_" + to_string(i + 1), headPetTemp);
	}

	cout << endl << "Start array:" << endl;
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


	return true;
}

int main()
{
	srand(time(0));

	clock_t start = clock();

	dzozefusWithChunk();

	clock_t end = clock();
	double seconds = (double)(end - start) / CLOCKS_PER_SEC;
	printf("The time: %f seconds\n", seconds);


	clock_t start2 = clock();

	dzozefus();
	
	clock_t end2 = clock();
	double seconds2 = (double)(end2 - start2) / CLOCKS_PER_SEC;
	printf("The time: %f seconds\n", seconds2);

	exit(0);
}

