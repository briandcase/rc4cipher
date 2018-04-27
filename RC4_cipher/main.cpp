// Brian Case
#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>
#define SWAP(a, b) ((a) ^= (b), (b) ^= (a), (a) ^= (b))
using namespace std;

class RC4
{
	public:
		RC4()
		{
			memset(sbox, 0, 256);
			memset(key, 0, 256);
		}
		
		char *Encrypt(char *pszText, const char *pszKey)
		{
			i = 0, j = 0, n = 0;
			ilen = (int)strlen(pszKey);

			for (m = 0; m < 256; m++)
			{
				*(key + m) = *(pszKey + (m % ilen));
				*(sbox + m) = m;
			}

			for (m = 0; m < 256; m++)
			{
				n = (n + *(sbox + m) + *(key + m)) &0xff;
				SWAP(*(sbox +m),*(sbox + n));
			}

		
			for (m = 0; m < 3072; m++)
			{
				i = (i + 1) & 0xff;
				j = (j + *(sbox + i)) & 0xff;
				temp = *(sbox + i);
				SWAP(*(sbox + i), *(sbox + j));
				*(sbox + j) = temp;
			}

			ilen = (int)strlen(pszText);
			for (m = 0; m < ilen; m++)
			{
				i = (i + 1) &0xff;
				j = (j + *(sbox + i)) & 0xff;
				SWAP(*(sbox+i), *(sbox + j));
				k = *(sbox + ((*(sbox + i) + *(sbox + j)) & 0xff));
				if (k == *(pszText + m))
				{
					k = 0;
				}
				*(pszText + m) ^= k;
			}
			return pszText;
		}
		
		char *Decrypt(char *pszText, const char *pszKey)
		{
			return Encrypt(pszText, pszKey);
		}

	private:
		unsigned char sbox[256];
		unsigned char key[256], k;
		int m, n, i, j, ilen, temp;
};

int main()
{
	char key[256];
	fstream fileStream ("rc4Input.txt");
	RC4 myObject;
	string line;

	if (!fileStream.is_open())
	{
		cout << " File cannot be opened\n";
		exit(EXIT_FAILURE);
	}
	else
	{
		
		cout << "This is the plainttext ->" << endl;
		while (getline(fileStream, line))
		{
			cout << line << '\n';
			
		}
	}
	

	char char_array[5000];
	
	strncpy_s(char_array, line.c_str(), sizeof(char_array));
	cout << endl;

	cout << "Please enter the RC4 key\n";
	cin >> key;
	cout << "Your key is -> " << key << endl;

	myObject.Encrypt(char_array, key);
	cout << "Encoded message : " << char_array << endl;
	ofstream myOutput;
	myOutput.open("output.hex", ios::out | ios::binary);
	int arrSize = sizeof(char_array);
	for (int i = 0; i < arrSize; ++i)
	{
		myOutput << hex << char_array[i];
	}
	myOutput << dec;
	myOutput.close();

	myObject.Decrypt(char_array, key);
	cout << "This is the decrypted string: " << char_array << endl;
	ofstream myDecoded;
	myDecoded.open("decoded.txt");
	myDecoded << char_array << endl;
	myDecoded.close();

	fileStream.close();
	system("pause");
	return 0;
}

