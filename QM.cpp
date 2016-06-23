#include "stdafx.h"
#include <iostream>
#include <vector>
#include <string>
#include <stdlib.h>
using namespace std;

int MIN_BITS = 4;		//минимальные биты(минимальное количество переменных)
vector<unsigned> input_values;
bool show_mid = false;		//показать половину выполнения процесса

struct B_number {
	unsigned number;
	unsigned dashes;
	bool used;
};

vector<vector<B_number> > table;	//исходная таблица
vector<vector<B_number> > p_group;	//таблица середины процесса
vector<vector<B_number> > final_group;	//заключительная таблица
vector<B_number> printed_numbers; //избегает прорисовку(вывода) повторяющихся комбинаций

unsigned count_1s(unsigned number); //длина числа
void print_binary(unsigned number);//печатает число в двоичном формате
void create_table();		//составляет сортированую исходную таблицу
void print_table();		//печатает таблицу
B_number init_B_number(unsigned n, int d, bool u);//инициализирует B_number
void create_p_group();		//составляет таблицу середины процесса
void print_p_group();		//печатает таблицу середины процесса
void print_p_binary(unsigned n, unsigned d);//печатает таблицу(бинарную) середины процесса
void create_final_group();		//составляет заключительную таблицу
void print_final_group();		//печатает заключительную таблицу и неиспользованные сроки
bool is_printed(B_number n);		//не печатает сроки, которые были уже напечатаны
void init();				//запускает создание таблиц и печать
void getinput();			//ввод от пользователя
unsigned count_bits(unsigned n);	//минимальные количество битов, чтобы представить число
				
int main() {

	getinput();
	system("CLS");
	init();
	system("pause");
	return 0;

}

/* получает размер числа в битовой форме */
unsigned count_1s(unsigned number) {
	short bit = 0;
	int count = 0;
	while (number>0) {
		bit = number % 2;
		number >>= 1;
		if (bit) {
			count++;
		}
	}
	return count;
}

void print_binary(unsigned number) {
	unsigned * bits = new unsigned[MIN_BITS];
	int count = 0;

	while (number>0 || count<MIN_BITS) {
		bits[count] = number % 2;
		number >>= 1;
		count++;
	}
	for (int i = count - 1;i >= 0;i--)
		cout << bits[i];
}
/*составление первой таблицы: добавляет текущее число массиву, расположенному в
таблице*/
void create_table() {
	short tmp;
	B_number temp_num;
	for (int i = 0;i<input_values.size();i++) {
		tmp = count_1s(input_values[i]);
		if (tmp + 1>table.size())
			table.resize(tmp + 1);

		temp_num = init_B_number(input_values[i], 0, false);
		table[tmp].push_back(temp_num);
	}
}

void print_table() {

	cout << endl << "Table:" << endl;
	for (int i = 0;i<table.size();i++) {
		cout << i;
		for (int j = 0;j<table[i].size();j++) {
			cout << "\tm" << table[i][j].number << "\t";
			print_binary(table[i][j].number);
			cout << endl;
		}
		cout << "\n___________________________________" << endl;
	}
}
/* инициализируйте переменную B_number - как конструктор */
B_number init_B_number(unsigned	n, int d, bool u) {
	B_number num;
	num.number = n;
	num.dashes = d;
	num.used = u;
	return num;
}
/*таблица, числа которой от исходной, но представляются в таком виде: например оригинал A=0011 B=1011, новое число
-011, который представляется как C.number=A&B=0011,C.dashes=A^B=1000*/
void create_p_group() {
	short tmp;
	B_number temp_num;
	unsigned temp_number, temp_dashes;
	for (int i = 0;i<table.size() - 1;i++) {
		for (int j = 0;j<table[i].size();j++) {
			for (int k = 0;k<table[i + 1].size();k++) {
				temp_number = table[i][j].number & table[i + 1][k].number;
				temp_dashes = table[i][j].number ^ table[i + 1][k].number;

				if (count_1s(temp_dashes) == 1) {
					table[i][j].used = true;
					table[i + 1][k].used = true;


					tmp = count_1s(temp_number);

					if (tmp + 1>p_group.size())
						p_group.resize(tmp + 1);

					temp_num = init_B_number(temp_number, temp_dashes, false);
					p_group[tmp].push_back(temp_num);
				}
			}
		}
	}
}

void print_p_group() {
	cout << endl << "Mid process:" << endl;

	for (int i = 0;i<p_group.size();i++) {
		cout << i;
		for (int j = 0;j<p_group[i].size();j++) {
			cout << "\t\t";
			print_p_binary(p_group[i][j].number, p_group[i][j].dashes);
			cout << endl;
		}
		cout << "\n___________________________________" << endl;
	}

}
/*напечатайте числа такие как-001; это выделяет биты в массиве dash=2 тогда
при печати инвертируется массив */
void print_p_binary(unsigned n, unsigned d) {
	unsigned * bits = new unsigned[MIN_BITS];
	int count = 0;

	while (n>0 || count<MIN_BITS) {
		if (!(d % 2))
			bits[count] = n % 2;
		else
			bits[count] = 2;

		n >>= 1;
		d >>= 1;
		count++;
	}
	for (int i = count - 1;i >= 0;i--) {
		if (bits[i] != 2)
			cout << bits[i];
		else
			cout << "*";
	}
}
/*составляет заключительную таблицу. работа как p_group (). пример; в p_group мы имеем числа:
A =-001 B =-011-> C =-0-1, которые будут представлены как
C.number=A&B=0001&0011=0001, и C.dashes=A^B^A.dashes=0001^0011^1000=1010.
Вычисление делается только когда A.dashes = b.dashes*/
void create_final_group() {
	short tmp;
	B_number temp_num;
	unsigned temp_number, temp_dashes;
	for (int i = 0;i<p_group.size() - 1;i++) {
		for (int j = 0;j<p_group[i].size();j++) {
			for (int k = 0;k<p_group[i + 1].size();k++) {
				if (p_group[i][j].dashes == p_group[i + 1][k].dashes) {
					temp_number = p_group[i][j].number & p_group[i + 1][k].number;
					temp_dashes = p_group[i][j].number ^ p_group[i + 1][k].number;
					if (count_1s(temp_dashes) == 1) {
						temp_dashes ^= p_group[i][j].dashes;

						p_group[i][j].used = true;
						p_group[i + 1][k].used = true;

						tmp = count_1s(temp_number);

						if (tmp + 1>final_group.size())
							final_group.resize(tmp + 1);

						temp_num = init_B_number(temp_number, temp_dashes, true);
						final_group[tmp].push_back(temp_num);
					}
				}
			}
		}
	}
}
/*напечатает все значения из заключительной таблицы, за исключением копий,
а также все неиспользованные числа из исходной таблицы и таблицы середины процесса*/
void print_final_group() {
	cout << endl << "Resulut of minimization:\n___________________________________" << endl;
	int i, j;
	for (i = 0;i<final_group.size();i++) {
		for (j = 0;j<final_group[i].size();j++) {
			if (!is_printed(final_group[i][j])) {
				print_p_binary(final_group[i][j].number, final_group[i][j].dashes);
				cout << endl;
				printed_numbers.push_back(final_group[i][j]);
			}
		}
	}
	for (i = 0;i<p_group.size();i++) {
		for (j = 0;j<p_group[i].size();j++) {
			if (!p_group[i][j].used) {
				print_p_binary(p_group[i][j].number, p_group[i][j].dashes);
				cout << endl;
			}
		}
	}
	for (i = 0;i<table.size();i++) {
		for (j = 0;j<table[i].size();j++) {
			if (!table[i][j].used) {
				print_p_binary(table[i][j].number, table[i][j].dashes);
				cout << endl;
			}
		}
	}
	cout << "___________________________________" << endl;
}
/*используеться, во избежание печати копий, которые могут существовать в заключительной таблице*/
bool is_printed(B_number n) {
	for (int i = 0;i<printed_numbers.size();i++)
		if (n.number == printed_numbers[i].number && n.dashes == printed_numbers[i].dashes)
			return true;

	return false;
}
/*инициализируем все таблицы*/
void init() {
	table.resize(1);
	p_group.resize(1);
	final_group.resize(1);
	create_table();
	print_table();
	create_p_group();
	if (show_mid)
		print_p_group();
	create_final_group();
	print_final_group();
}

void getinput() {
	unsigned in;
	int num_bits = 0;
	cout << "\nInput value followed by ENTER[type 'end' for ends input]\n> ";
	while (cin >> in) {
		input_values.push_back(in);
		num_bits = count_bits(in);
		if (num_bits>MIN_BITS)
			MIN_BITS = num_bits;
		cout << "> ";
	}
}
/*возвращает минимальное число битов, которыми представляется число. используемый для лучшего вывода*/
unsigned count_bits(unsigned n) {
	short bit = 0;
	int count = 0;
	while (n>0) {
		bit = n % 2;
		n >>= 1;
		count++;
	}
	return count;
}

