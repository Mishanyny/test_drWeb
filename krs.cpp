//#include "pch.h"
#include <iostream>

#include <stdio.h>

#include <string.h>

#include <cstdlib>

#include <vector>

#include <fstream>

#include <math.h>

using namespace std;

static int incount = 0; //переменная для подсчета входа в функцию(счетчик)
class absMatrix
{
protected:
	int N; //размерность матрицы
	unsigned int typesize; //кол-ва байтов типа
public:
	virtual void float2char(float*, int) {};
	virtual float char2float(int index) { return 0; }
	virtual char* get_cell(int, int) ;	
    virtual char* summ_row(int) ;	
    virtual char* max_in_col(int);								// извлекаем массив байтов из матрицы
	virtual char* sum(char*, char*) ;								// сумма двух элементов матрицы в байтовом формате
	virtual char* multiply(char*, char*) ;							// умножение двух элементов матрицы в байтовом формате
	virtual char* divide(char*, char*);
	virtual bool more(char*, char*);
	void set_cell(char*, int i, int j) ;
	void setDimension(int);
	int getDimension();
	char * get_matr() { return matrx; }
	void set_matrx(char* p) { matrx = p; }
	absMatrix(const absMatrix&);
	absMatrix(int,unsigned int);
	absMatrix();
	~absMatrix();
	absMatrix& operator=(const absMatrix&);
	absMatrix operator*(absMatrix&);
	absMatrix operator+(absMatrix&);
	absMatrix& operator++(int);
	absMatrix& operator--(int);
	bool operator>(absMatrix&);

private:
	char * matrx;
};

char * absMatrix::get_cell(int, int)
{
	return NULL;
}

char * absMatrix::summ_row(int)
{
	return NULL;
}

char * absMatrix::max_in_col(int)
{
    return NULL;
}

char * absMatrix::sum(char *, char *)
{
	return NULL;
}

char * absMatrix::multiply(char *, char *)
{
	return NULL;
}

char * absMatrix::divide(char *, char *)
{
	return NULL;
}


bool absMatrix::more(char *, char *)
{
	return false;
}

void absMatrix::set_cell(char *part,int i,int j)
{
	char *dest = this->matrx + (i*N + j)*typesize;
	memcpy(dest, part, sizeof(float));
}

void absMatrix::setDimension(int p)
{
	this->N = p;
}

int absMatrix::getDimension()
{
	return this->N ;
}

absMatrix::absMatrix(const absMatrix &other)
{
	this->N = other.N;
	char* matrx = other.matrx;
	this->typesize = other.typesize;
	char *temp = new char[this->N*this->N*this->typesize];
	for (unsigned int i = 0; i < N*N*this->typesize; i++) {
		temp[i] = matrx[i];
	}
	this->set_matrx(temp);
	
}

absMatrix::absMatrix(int p,unsigned int size)
{
	this->typesize = size;
	this->N = p;
	this->matrx = new char[size*p*p];
	for (unsigned int i = 0; i < size*p*p;i++) {
		matrx[i] = 0;
	}
}

absMatrix::absMatrix()
{
	this->N = 0;
	this->matrx = 0;
}

absMatrix::~absMatrix()
{
	this->N = 0;
	this->typesize = 0;
	if (matrx) delete [] matrx;
}

absMatrix& absMatrix::operator=(const absMatrix& other)
{
	this->~absMatrix();
	
	this->N = other.N;
	this->typesize = other.typesize;
	char *temp = new char[this->N*this->N*this->typesize];
	char *matrx = other.matrx;
	for (unsigned int i = 0; i < N*N*this->typesize; i++) {
		temp[i] = matrx[i];
	}
	this->set_matrx(temp);

	return *this;
}

absMatrix absMatrix::operator*(absMatrix &sec)
{
	if (this->N != sec.N) cout << "Размерности не совпадают";
	else {
		absMatrix newObj(this->N,this->typesize);
		char* line;	// Сумма всех произведений
		char* mult = new char[this->typesize];	// Промежуточный результат произведения двух элементов матриц
		for (int i = 0; i < this->N; i++) {
			for (int j = 0; j < N; j++)
			{
				line = new char[this->typesize];
				for (int k = 0; k < typesize; k++)
				{
					line[k] = 0;
				}
				for (int r = 0; r < N; r++)
				{
					
					mult = multiply(get_cell(i, r), sec.get_cell(r, j));
					line = sum(mult, line);
				}
				newObj.set_cell(line,i,j);
				delete [] line ;
			}
		}
		delete[] mult;
		return newObj;
	}
	return *this;
}

absMatrix absMatrix::operator+(absMatrix &sec)
{
	absMatrix newObj(this->N, this->typesize);
	// посчитаем сумму j-го столбца для обеих матриц
	for (int j = 0; j < this->N; j++)
	{	
		char* temp_divider = new char[typesize];
		char* sum = new char[typesize];
        char* buf = new char[typesize];

        if (this->more(this->max_in_col(j), sec.max_in_col(j))){
    
            temp_divider = sec.max_in_col(j);
        }
        else{

            temp_divider = this->max_in_col(j);
        }


		for (int l = 0; l < typesize; l++)
		{
			buf[l] = 0;
		}
		for (int i = 0; i < this->N; i++)
		{
			buf = this->sum(this->get_cell(i, j), sec.get_cell(i, j));

			if(temp_divider){
                buf = this->divide(buf, temp_divider);
            }

            newObj.set_cell(buf, i, j);
		}
    }
	return newObj;
}

absMatrix& absMatrix::operator++(int l)
{
	absMatrix newObj(this->N + 1,this->typesize);
	for (int i = 0; i < this->N; i++)
	{
		for (int j = 0; j < this->N; j++)
		{
			char* temp =new char[typesize];
			temp = this->get_cell(i, j);
			newObj.set_cell(temp, i, j);
		}
	}
	delete []this->matrx;
	this->matrx = newObj.matrx;
	this->N = this->N + 1;
	newObj.matrx = 0;
	return *this;
}

absMatrix& absMatrix::operator--(int)
{
	absMatrix newObj(this->N - 1, this->typesize);
	for (int i = 0; i < this->N-1; i++)
	{
		for (int j = 0; j < this->N-1; j++)
		{
			char* temp = new char[typesize];
			temp = this->get_cell(i, j);
			newObj.set_cell(temp, i, j);
		}
	}
	delete[]this->matrx;
	this->matrx = newObj.matrx;
	this->N = this->N - 1;
	newObj.matrx = 0;
	return *this;
}

bool absMatrix::operator>(absMatrix &sec)
{
	char* buf_summ1=new char[typesize];
	char* buf_summ2 = new char[typesize];

	for (int i = 0; i < this->N; i++)
	{
        buf_summ1 = this->summ_row(i);
        buf_summ2 = sec.summ_row(i);

        if(this->more(buf_summ2, buf_summ1)){

            return false;
        }
	}

    return true;
}

class floatMatrix : public absMatrix
{
public:
	friend istream& operator>> (istream&, floatMatrix&);			//стандартный ввод
	friend ostream& operator<< (ostream&, floatMatrix&);			//стандартный вывод
	void float2char(float*, char*);									// функция для "разбития" floatа на char
	float char2float(char*);										//собираем из char float
	floatMatrix() :absMatrix() {};
	floatMatrix(int p) :absMatrix(p,sizeof(float)) {};
	~floatMatrix();
	char* get_cell(int,int) override;									// перегружаем виртуальные функции 
	char* sum(char*,char*) override;
    char* summ_row(int);
	char* multiply(char*, char*) override;
	char* divide(char*, char*) override;
    char* max_in_col(int) override;
	bool more(char*, char*) override;
private:

};

void floatMatrix::float2char(float* value, char* dest)
{
	for (int i = 0; i < 4; i++) {
		char *temp = (dest  + i * sizeof(char));
		char *tt = (char*)value + i * sizeof(char);
		memcpy(temp, tt, sizeof(char));
					//сначала нужный флоат + сдвиг внутри флоата
	}

}

float floatMatrix::char2float(char* scr)
{
	float* dest=new float;
	memcpy(dest, scr, sizeof(float));
	return *dest;
}

floatMatrix::~floatMatrix()
{
}
char * floatMatrix::get_cell(int i, int j) // где i, j индекс включая 0(первый элемент = 0,0)
{
	char* temp = new char(sizeof(float));
	memcpy(temp,this->get_matr() + (i * this->N + j)*sizeof(float),sizeof(float));
	return temp;
}

char * floatMatrix::max_in_col(int j)
{
    char* buf = new char[typesize];

	for (int l = 0; l < typesize; l++)
	{
		buf[l] = 0;
	}
    buf = this->get_cell(0, j);
	for (int i = 1; i < this->N; i++)
	{
		if(this->more(this->get_cell(i, j), buf)){ 
        
            buf = this->get_cell(i, j);
        }
	}

    return buf;
}



char * floatMatrix::multiply(char *a, char *b)
{
	char* temp = new char[sizeof(float)];
	float c=0;
	c = char2float(a) * char2float(b);
	float2char(&c,temp);
	return temp;
}
char * floatMatrix::divide(char *a, char *b)
{
	char* temp = new char[sizeof(float)];
	float c = 0;
	c = char2float(a) / char2float(b);
	float2char(&c, temp);
	return temp;
}
bool floatMatrix::more(char* a, char *b)
{
	return char2float(a) > char2float(b);
}
char * floatMatrix::sum(char *a, char *b)
{
	char* temp = new char[sizeof(float)];
	float c=0;
	c = char2float(a) + char2float(b);
	float2char(&c, temp);
	return temp;
}
char * floatMatrix::summ_row(int i)
{
    char* buf = new char[sizeof(float)];

    for (int l = 0; l < sizeof(float); l++){

			buf[l] = 0;
    }

	for(int j = 0; j < this->N; j++){

        buf = this->sum(buf, this->get_cell(i, j));
    }
    
    return buf;
}
void c_out(floatMatrix*& matrix1, floatMatrix*& matrix2, floatMatrix*& matrix3);
void c_in(floatMatrix*& matrix1, floatMatrix*& matrix2, floatMatrix*& matrix3);
void menu_out();
void f_in(floatMatrix*& matrix1, floatMatrix*& matrix2, floatMatrix*& matrix3);
void f_out(floatMatrix*& matrix1, floatMatrix*& matrix2, floatMatrix*& matrix3);
void fm_out(floatMatrix *matrix);
int main()
{
	setlocale(LC_ALL, "");
	
	floatMatrix* matr1Point=0;
	floatMatrix* matr2Point=0;
	floatMatrix* matr3Point=0;
	/*floatMatrix one(2);
	floatMatrix two(2);
	floatMatrix three(2);
	cin >> one >> two;
	cout << one << two;
	static_cast<absMatrix&>(three) = one+two;
	cout << three<< two;
	two--;
	cout << two;*/
	char number[2] = "";
	do
	{
        menu_out();
		cin >> number;
		switch (number[0])
		{
		case '1':
		{
			system("clear");
			cout << "Консольный\n";
			cout << "1)Ввод\n";
			if (incount > 0) cout << "2)Вывод\n";
			cin >> number;
			switch (number[0])
			{
			case '1':
			{
				incount++;
				c_in(matr1Point, matr2Point, matr3Point);
                system("clear");
				break;
			}
			case '2': if (incount > 0) c_out(matr1Point, matr2Point, matr3Point);
					  else cout << "Неверный выбор\n";  break;
			default: cout << "Неверный выбор\n"; break;
			}
            system("clear");
			break;
		}
		case '2':
		{
			system("clear");
			cout << "Файловый\n";
			cout << "1)Ввод\n";
			if (incount > 0) cout << "2)Вывод\n";
			cin >> number;
			switch (number[0])
			{
			case '1':
			{
				f_in(matr1Point, matr2Point, matr3Point);
				incount++;
                system("clear");
				break;
			}
			case '2':if (incount > 0)	f_out(matr1Point, matr2Point, matr3Point);
					 else cout << "Неверный выбор\n"; break;
			default: cout << "Неверный выбор\n"; break;
			}
            system("clear");
			break;
		}
		case '3':
			if (incount > 0) {
                system("clear");
			    cout << "Перегрузка\n";
			    cout << "1)++\n";
			    cout << "2)--\n";
			    cin >> number;

			    switch (number[0])
			    {
			        case '1':
			        {
                        int k;
				        cout << "Выберите матрицу->";
				        cin >> k;
				        switch (k) 
                        {
				            case 1: matr1Point->operator++(1); break;
				            case 2: matr2Point->operator++(1); break;
				            case 3: matr3Point->operator++(1); break;
				        }
			        }
			        case '2':
                    {
                        int k;
                        cout << "Выберите матрицу->";
				        cin >> k;
				        switch (k) 
                        {
				            case 1: matr1Point->operator--(1); break;
				            case 2: matr2Point->operator--(1); break;
				            case 3: matr3Point->operator--(1); break;
				        }
                    }
                
			    }
                system("clear");
			    break;

			}
			else cout << "Сначала заполните матрицы\n";

            system("clear");
			break;

		case '4':
			if (incount > 0)
			{
				system("clear");
				cout << "1)Перегрузка '+'\n";
				cout << "2)Перегрузка '>'\n";
				cin >> number;
				switch (number[0])
				{
				case '1': {
					if (matr3Point == 0) matr3Point = new floatMatrix(matr1Point->getDimension());
					static_cast<absMatrix&>(*matr3Point) = *matr1Point + *matr2Point;
				}break;
				case '2': { 
					bool flag = *matr1Point > *matr2Point;
					cout << flag;
					}; break;
				}
                system("clear");
				break;
			}
			else cout << "Сначала заполните матрицы\n"; system("clear"); break;
		case '5':
			if (incount > 0)
			{
				if (matr3Point == 0) matr3Point = new floatMatrix(matr1Point->getDimension());
				static_cast<absMatrix&>(*matr3Point) = *matr1Point * *matr2Point;
				cout << *matr3Point;
                system("clear");
				break;
			}
			else cout << "Сначала заполните матрицы\n"; system("clear"); break;

		default:
			if (!strcmp(number, "exit")) break;
			else cout << "Неверный выбор\n";
            system("clear");
			break;
		}
	} while (strcmp(number, "exit"));
}
void c_out(floatMatrix*& matrix1, floatMatrix*& matrix2, floatMatrix*& matrix3)
{
	int p;
	char k;
	bool flag = false; //Для корректной работы выхода из функции
	do
	{
		flag = false;
		cout << "Введите номер матрицы которую хотите вывести->";
		cin >> p;
		switch (p)
		{
		case 1: if(matrix1 != 0) cout << *matrix1; break;
		case 2: if(matrix2 != 0) cout << *matrix2; break;
		case 3: if(matrix3 != 0) cout << *matrix3; break;
		default: cout << "Нет такой матрицы" << endl;
		}
		cout << "Повторить? (Д/Н)\n";
		cin >> k;
		if (k == 'Д') flag = true;
	} while (flag);

}
void c_in(floatMatrix*& matrix1, floatMatrix*& matrix2, floatMatrix*& matrix3)
{
	char k;
	int p;
	bool flag = false;
	do
	{
		flag = false;
		cout << "Введите номер матрицы->";
		cin >> p;
		switch (p)
		{
		case 1: {
			int N; // размерность вводимой матрицы
			cout << "Введите размерность матрицы\n";
			cin >> N;
			if (matrix1 != 0) delete matrix1;
			matrix1 = new floatMatrix(N);
			cin >> *matrix1;
			break;
		}	
		case 2: {
			int N; // размерность вводимой матрицы
			cout << "Введите размерность матрицы\n";
			cin >> N;
			if (matrix2 != 0) delete matrix2;
			matrix2 = new floatMatrix(N);
			cin >> *matrix2;
			break;
		}
		case 3: {
			int N; // размерность вводимой матрицы
			cout << "Введите размерность матрицы\n";
			cin >> N;
			if (matrix3 != 0) delete matrix3;
			matrix3 = new floatMatrix(N);
			cin >> *matrix3;
			break;
		}
		default: cout << "Данная матрица недоступна "; break;
		}
		cout << "Повторить? (Д/Н)\n";
		cin >> k;
		if (k == 'Д') flag = true;
	} while (flag);
	incount++;
}
void menu_out()
{
	cout << "1)Консольный ввод/вывод\n";
    cout << "2)Файловый ввод/вывод\n";
	cout << "3)Операция ++/--\n";
	cout << "4)Индивидуальное задание\n";
	cout << "5)Операция *\n";
	cout << "Для выхода введите <exit> \n";
	
}
void f_in(floatMatrix*& matrix1, floatMatrix*& matrix2, floatMatrix*& matrix3)
{
	//Для определения размерности воспользуемся контейнерами класса вектор
	vector <float> container1, container2, container3;
	unsigned int  i, j, p;
	system("clear");
	ifstream f1, f2, f3; // Декларация потоков
	char k;
	bool flag = false;

	f1.open("matrix1.in.txt");
	f2.open("matrix2.in.txt");
	f3.open("matrix3.in.txt");
	if ((!f1.is_open()) || (!f2.is_open()) || (!f3.is_open())) // если файл не открыт
	{
		incount--;
		cout << "Один из файлов не может быть открыт!\n"; // сообщить об этом
		system("pause");
		return;
	}
	while (!f1.eof())
	{
		float temp = 0;//Временная переменная
		f1 >> temp;
		container1.push_back(temp);
	}
	while (!f2.eof())
	{
		float temp = 0;
		f2 >> temp;
		container2.push_back(temp);
	}
	while (!f3.eof())
	{
		float temp = 0;
		f3 >> temp;
		container3.push_back(temp);
	}
	if ((container1.size() != container2.size()) || (container2.size() != container3.size()))
	{
		cout << "Матрицы имееют разную размерность\n";
		incount--;
		return;
	}
	int n = sqrt(container1.size());//Присвоим н значение размерности матрицы
	do
	{
		flag = false;
		cout << "Введите номер матрицы, которую хотите ввести->";
		cin >> p;
		switch (p)
		{
		case 1: {		
			if (matrix1 == 0) delete[] matrix1;
			matrix1 = new floatMatrix(n);
			for (i = 0 ; i < n; i++)
				for (j = 0; j < n; j++)
				{
					char* temp = new char[sizeof(float)];
					matrix1->float2char(&container1[i*n + j], temp);
					matrix1->set_cell(temp, i, j);
				}	
			break;
		}
		case 2: {		
			if (matrix2 == 0) delete[] matrix2;
			matrix2 = new floatMatrix(n);
			for (i = 0; i < n; i++)
				for (j = 0; j < n; j++)
				{
					char* temp =new char[sizeof(float)];
					matrix2->float2char(&container2[i*n+j],temp);
					matrix2->set_cell(temp, i, j);
				}
			break;
		}
		case 3: {		
			if (matrix3 == 0) delete[] matrix3;
			matrix3 = new floatMatrix(n);
			for (i = 0; i < n; i++)
				for (j = 0; j < n; j++)
				{
					char* temp = new char[sizeof(float)];
					matrix3->float2char(&container3[i*n + j], temp);
					matrix3->set_cell(temp, i, j);
				}
			break;
		}
		default: cout << "Нет такой матрицы" << endl;
		}
		cout << "Повторить? (Д/Н)\n";
		cin >> k;
		if (k == 'Д') flag = true;
	} while (flag);
	f1.close();
	f2.close();
	f3.close();
	return;
}
void f_out(floatMatrix*& matrix1, floatMatrix*& matrix2, floatMatrix*& matrix3)
{
	int p;
	char k;
	bool flag = false;	//Для корректной работы выхода из функции
	do
	{
		flag = false;
		system("clear");
		cout << "Введите номер матрицы которую хотите вывести->";
		cin >> p;
		switch (p)
		{
		case 1: if (matrix1!= 0) fm_out(matrix1);
				else cout << "Матрица пустая\n"; break;
		case 2: if (matrix2 != 0) fm_out(matrix2);
				else cout << "Матрица пустая\n"; break;
		case 3: if (matrix3 != 0) fm_out(matrix3);
				else cout << "Матрица пустая\n"; break;
		}
		cout << "Повторить? (Д/Н)\n";
		cin >> k;
		if (k == 'Д') flag = true;
	} while (flag);

}
void fm_out(floatMatrix *matrix)
{
	fstream f;
	f.open("matrix.out.txt", ios::out);
	int i, j;
	if (!f.is_open()) //Проверка файла
		cout << "Файл не может быть открыт!\n";
	for (i = 0; i < matrix->getDimension(); i++)
	{
		for (j = 0; j < matrix->getDimension(); j++)
			f << matrix->char2float(matrix->get_cell(i,j)) << " ";
		f << "\n";
	}
	f.close();
	return;
}
istream & operator>>(istream &R, floatMatrix &obj)
{
	float temp;
	cout << "Введите матрицу:\n";
	for (int i = 0; i < obj.N*obj.N; i++) {
		R >> temp;
		obj.float2char(&temp, obj.get_matr()+ i* sizeof(float));
	}
	return R;
}

ostream & operator<<(ostream & W, floatMatrix & obj)
{
	W << "Вывод матрицы:\n";
	for (int i = 0; i < obj.N; i++) {
		for (int j = 0; j < obj.N; j++) {
			float temp = obj.char2float(obj.get_matr()+(i*obj.N+j)*sizeof(float));
			W << temp<<" ";
		}
		W << endl;
	}
	return W;
}

