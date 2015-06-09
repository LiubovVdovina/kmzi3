//
//  main.cpp
//  LSB_Vdovina
//  Copyright (c) 2015 Lyuba_Vdovina. All rights reserved.


#include <fstream>
#include <iostream>
using namespace std;
bool check_format(long long pos, char *buffer);

int main(int argc, char* argv[])
{
    if (argc != 4)
    {
        cout << "Incorrect string! Enter arguments as: input.wav output.wav input.txt" << endl;
        return 0;
    }
    
    //копирование исходного файла
    ifstream infile;
    infile.open(argv[1], ios::binary | ios::in);
    ofstream outfile;
    outfile.open(argv[2], ios::binary | ios::trunc);
    ifstream in_messg;
    in_messg.open(argv[3], ios::binary | ios::in);
    if ((infile.fail()) || (outfile.fail()) || (in_messg.fail()))
    {
        cout << "Error: openning file."   << endl;
        return 0;
    }
    
    long long pos;
    char * buffer;
    buffer = new char;
    char *message_byte;
    message_byte = new char;
    
    infile.read(buffer, sizeof(char));
    while (!infile.eof())
    {
        outfile.write(buffer, sizeof(char)); //заголовок копируем без изменений
        infile.read(buffer, sizeof(char));
        pos = infile.tellg();
		if (check_format(pos, buffer) == 0)
			return 0;
		if (pos == 100)
        {
            while (!in_messg.eof())
            {
                in_messg.read(message_byte, sizeof(char)); //считываем байт сообщения
                for (int bitIndex = 0; bitIndex < 8; bitIndex++) //записываем каждый из 8 битов каждого байта информации
                {
                    infile.read(buffer, sizeof(char)); //считываем байт исходного wav
                    if (*message_byte & 1 << bitIndex) //если текущий бит сообщения == 1
                        *buffer |= 1 << 7; //меняем наименее значащий бит wav данных на 1
                    else //если текущий бит == 0
                        *buffer &= ~(1 << 7); //меняем наименее значащий бит wav данных на 0
                    outfile.write(buffer, sizeof(char)); //записываем измененный байт данных в выходной wav файл
                }
            }
        }
    }
    infile.close();
    outfile.close();
    in_messg.close();
    return 0;
}
bool check_format(long long pos, char *buffer)
{
	if ((pos == 9) && (*buffer != 'W'))
	{
		cout << "Error: file must be wav format." << endl;
		return 0;
	}
	if ((pos == 10) && (*buffer != 'A'))
	{
		cout << "Error: file must be wav format." << endl;
		return 0;
	}
	if ((pos == 11) && (*buffer != 'V'))
	{
		cout << "Error: file must be wav format." << endl;
		return 0;
	}
	if ((pos == 12) && (*buffer != 'E'))
	{
		cout << "Error: file must be wav format." << endl;
		return 0;
	}
	return 1;
}
