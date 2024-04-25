

#include <iostream>
#include <string> 
#include <iterator>
#include <ctime>

using namespace std;



//курсовая работа
struct PetTemp
{
	int id;
	string name;
	PetTemp* next;
};


//Структура с чанками для оптимизации
struct ChunksPets {
	int id;
	ChunksPets* nextPetChunk;
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
		hChunk->nextPetChunk = newChunk;

	}
	else {
		headChunk = newChunk;
	}
	return newChunk;
}




static int round_up(double value) {
	return (int)ceil(value);
}

int main()
{
	srand(time(0));

	int size_arr = 21;
	int temp_size = size_arr;
		
	int chunk_size = 5;
	int size_arr_chunks = round_up((double)size_arr/ (double)chunk_size);

	PetTemp* chunks_petTemp = new PetTemp[size_arr_chunks];

	//for (int i = 0; i < size_arr_chunks; i++)
	//{
	//	for (int j = 1; j <= chunk_size; j++)
	//	{
	//		cout << (i * chunk_size) + j << endl;
	//	}
	//}
	int temp_chunk_size = 1;
	for (int i = 0; i < size_arr; i++)
	{
		PetTemp* tempPet = addPetTemp(i+1, "test_" + to_string(i+1), headPetTemp);
		if (i == chunk_size*temp_chunk_size) {
			addChunk(temp_chunk_size, tempPet, headChunk);
			//chunks_petTemp[temp_chunk_size ] = tempPet;
			temp_chunk_size++;

		}
	}

	for (int i = 0; i < size_arr_chunks; i++)
	{
		cout << chunks_petTemp[i].id << endl;
	}
	

	cout << endl << "Start array:" << endl;
	coutPetTemp(headPetTemp);
	cout << endl << "algorithm:" << endl << "=========================" << endl<<endl;


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
				deletePetTempForNumber(randint+ last_delete, headPetTemp);
				last_delete = randint;
			}
		}
		else {
			deletePetTempForNumber(dif, headPetTemp);
			last_delete = dif;
		}

		
		temp_size--;
		cout << "random = " << randint <<" last_delete = "<< last_delete << endl;
		coutPetTemp(headPetTemp);
	}
	cout <<  endl << "=========================" << endl;

	cout << endl<<"Result:"<<endl;
	coutPetTemp(headPetTemp);

	exit(0);
}

