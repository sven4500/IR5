#include <iostream>
#include <fstream>
#include <sstream>
#include <clocale>
#include <string>
#include <vector>
#include <set>

void readIndex(std::string const& fileName, std::vector<char>& buf)
{
	std::ifstream fin;
	fin.open(fileName, std::ios::in | std::ios::binary);
	if(!fin)
		return;

	// �������� ������ �����.
	fin.seekg(0, std::ios::end);
	std::streampos const size = fin.tellg();
	fin.seekg(0, std::ios::beg);

	buf.resize(size);
	fin.read(&buf[0], size);
	fin.close();
}

void getDocID(void const* const data, std::set<unsigned int>& docID)
{
	unsigned int const* const iter = reinterpret_cast<unsigned int const*>(data);
	// �� ������ ���� �������������� ������� ������ ����. ���� ��� ���,
	// �� ������ ����� ��� ������ ����� ������� ������ �� ������.
	// ���� ���������� ���������� �������, �� ��� �� ������, ������
	// � ���� ������ ��� ���� ������ ������.
	if(iter[0] != 0xAAAAAAAA || iter[1] == 0 || iter[2] == 0)
		return;
	docID.insert(&iter[4], &iter[4] + iter[2]);
}

void getDocID(std::vector<char> const& data, std::set<unsigned int>& docID, unsigned int const termID = 0)
{
	if(data.empty())
		return;

	unsigned int const* iter = (unsigned int*)(&data[0] + 16);
	unsigned int const* const end = (unsigned int*)(&data[0] + data.size());

	while(true)
	{
		// ��������� ������ �� ��� ��� ����� ���������, � ������:
		// ����� ����� � ����� �� ������� ������.
		if(iter[0] != 0xAAAAAAAA || iter >= end)
			break;
		/*if(termID == 0 || (termID != 0 && termID == iter[1]))
			getDocID(iter, docID);
		if(termID != 0 && termID == iter[1])
			break;*/
		if(termID != 0)
		{
			if(termID == iter[1])
			{
				getDocID(iter, docID);
				return;
			}
		}
		else
		{
			getDocID(iter, docID);
		}
		iter += 4 + iter[2];
	}
}

int main(int argc, char** argv)
{
	std::setlocale(LC_CTYPE, "Russian");

	if(argc != 3)
	{
		std::cout << "IR4.exe index.dat ������" << std::endl;
		return 1;
	}

	// ��������� ���� ������ ������� � ���.
	std::vector<char> rawIndex;
	readIndex(argv[1], rawIndex);

	std::set<unsigned int> complDocID;
	getDocID(rawIndex, complDocID);

	// ������� ��������� ������.
	//std::stringstream ss(argv[2]);

	std::cout << complDocID.size() << std::endl;
	std::system("pause");
	return 0;
}
