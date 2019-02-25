#include<fstream>
#include<iostream>
//#include<new>
#include<string>
#include<stdlib.h>
#include<algorithm>
#include<cstring>

using std::cout;
using std::cin;
using std::endl;
using std::ifstream;
using std::ofstream;
using std::string;



class stemmer
{
	string b;	//actual word to be stemmed
	int k;		//length of word
	int j;		// generic pointer to string

public:
	stemmer()
	{
		b = "";
		j = 0;
		k = 0;
	}


	int cons(int i)		//check the character at index i is consonant
	{
		
		switch (b[i])
		{
		case 'a': case 'e': case 'i': case 'o': case 'u': return false;
		case 'y': return (i == 0) ? true : !cons(i - 1);
		default: return true;
		}

	}

	int m()		//calculate the m value of the given word
	{
		int i = 0, n = 0;

		while (true)
		{
			if (i > j)
			{

				return n;
			}
			if (!cons(i))
				break;
			i++;
		}
		i++;
		while (true)
		{
			while (true)
			{
				if (i > j)
				{
					return n;
				}
				if (cons(i))
					break;
				i++;
			}
			i++;
			n++;
			while (true)
			{
				if (i > j)
				{
					return n;
				}
				if (!cons(i))
					break;
				i++;
			}
			i++;

		}
	}

	int vowelinstem()		//check whether a vowel is present in stem
	{
		for (int i = 0; i <= j; i++)
			if (!cons(i))
				return true;
		return false;
	}

	int doublec(int j)		//check if the word is ending with double alphabets. eg. hopp, batt.
	{
		if (j < 1)
			return false;
		if (b.at(j) != b.at(j - 1))
			return false;
		return cons(j);
	}

	int ends(int length, string s)		//check if the word ends with a certain string
	{
		//cout << "Ends";
		if (length > k + 1)
			return false;

		if (memcmp(&b.at(k - length + 1), &s.at(0), length) != 0)
			return false;
		
		j = k - length;
		return true;
	}
	void setto(int length, string s)		//set the ending of a word to a given string
	{
		int z=0;
		k = j + length;
		//cout << "set to";
		for (int i = j; i < k ; i++)
		{
			b[i+1] = s[z];
			z++;
		}
		//memmove(&b.at(0) + j + 1, &s.at(0), length);
		
	}

	void r(int length, string s)		//check if the m value is greater than 0, if yes, set the ending of word to given string
	{
		if (m() > 0)
			setto(length, s);
	}

	int cvc(int i)		//check if consonant-vowel-consonant pattern is present
	{
		if (i < 2 || !cons(i) || cons(i - 1) || !cons(i - 2))
			return false;
		{
			int ch = b.at(i);
			if (ch == 'w' || ch == 'x' || ch == 'y')
				return false;
		}
		return true;
	}

	void step1ab()
	{

		//cout << "step1ab";

		if (b.at(k) == 's')
		{
			if (ends(4, "sses"))
				k -= 2;
			else if (ends(3, "ies"))
				setto(1, "i");
			else if (b.at(k - 1) != 's')
			{

				k--;
			}
		}
		if (ends(3, "eed"))
		{
			if (m() > 0)
			{
				//b.erase(k - 1, 2);
				k --;
			}
		}
		else if ((ends(2, "ed") || ends(3, "ing")) && vowelinstem())
		{

			k = j;
			if (ends(2, "at"))
				setto(3, "ate");
			else if (ends(2, "bl"))
				setto(3, "ble");
			else if (ends(2, "iz"))
				setto(3, "ize");
			else if (doublec(k))
			{
				k--;
				{  int ch = b.at(k);
				if (ch == 'l' || ch == 's' || ch == 'z') k++;
				}
			}
			else if (m() == 1 && cvc(k))
			{
				b.resize(k+1);
				b += 'e';
				k++;
			}
		}

	}

	void step1c()
	{
		if (ends(1, "y") && vowelinstem())
			b.at(k) = 'i';
	}

	void step2()
	{
		switch (b.at(k - 1))
		{
		case 'a':
			if (ends(7, "ational"))
			{
				r(3, "ate");
				break;
			}
			if (ends(6, "tional"))
			{
				r(4, "tion");
				break;
			}
			break;
		case 'c':
			if (ends(4, "enci"))
			{
				r(4, "ence");
				break;
			}
			if (ends(4, "anci"))
			{
				r(4, "ance");
				break;
			}
			break;
		case 'e':
			if (ends(4, "izer"))
			{
				r(3, "ize");
				break;
			}
		case 'l':
			if (ends(3, "bli"))
			{
				r(3, "ble");
				break;
			}
			if (ends(4, "alli"))
			{
				r(2, "al");
				break;
			}
			if (ends(5, "entli"))
			{
				r(3, "ent");
				break;
			}
			if (ends(3, "eli"))
			{
				if (k > 2)
				r(1, "e");
				break;
			}
			if (ends(5, "ousli"))
			{
				r(3, "ous");
				break;
			}
			break;
		case 'o':
			if (ends(7, "ization"))
			{
				r(3, "ize");
				break;
			}
			if (ends(5, "ation")&&k>5)
			{
				r(3, "ate");
				break;
			}
			if (ends(4, "ator"))
			{
				r(3, "ate");
				break;
			}
			break;
		case 's':
			if (ends(5, "alism"))
			{
				r(2, "al");
				break;
			}
			if (ends(7, "iveness"))
			{
				r(3, "ive");
				break;
			}
			if (ends(7, "fulness"))
			{	if(k>6)
				r(3, "ful");
				break;
			}
			if (ends(7, "ousness"))
			{
				r(3, "ous");
				break;
			}
			break;
		case 't':
			if (ends(5, "aliti"))
			{
				r(2, "al");
				break;
			}
			if (ends(5, "iviti"))
			{
				r(3, "ive");
				break;
			}
			if (ends(6, "biliti"))
			{
				r(3, "ble");
				break;
			}
			break;
		case 'g':
			if (ends(4, "logi"))
			{
				r(3, "log");
				break;
			}
		}

	}
	void step3()
	{
		j = k;
		int a = m();
		switch (b.at(k))
		{
		case 'e':
			if (ends(5, "icate") && a > 2)
			{
				r(2, "ic");
				break;
			}
			if (ends(5, "ative")&& k>5)
			{
				r(0, "");
				break;
			}
			if (ends(5, "alize"))
			{
				r(2, "al");
				break;
			}
			break;
		case 'i':
			if (ends(5, "iciti"))
			{
				r(2, "ic");
				break;
			}
			break;
		case 'l':
			if (ends(4, "ical"))
			{
				r(2, "ic");
				break;
			}
			if (ends(3, "ful"))
			{
				r(0, "");
				break;
			}
			break;
		case 's':
			if (ends(4, "ness"))
			{
				r(0, "");
				break;
			}
			break;
		}
	}
	void step4()
	{
		
		switch (b.at(k - 1))
		{
			
		case 'a':
			if (ends(2, "al"))
				break;
			return;
		case 'c':
			if (ends(4, "ance"))
				break;
			if (ends(4, "ence"))
				break;
			return;
		case 'e':
			if (ends(2, "er"))
				break;
			return;
		case 'i':
			if (ends(2, "ic"))
				break;
			return;
		case 'l':
			if (ends(4, "able"))
				break;
			if (ends(4, "ible"))
				break;
			return;
		case 'n':
			if (ends(3, "ant"))
				break;
			if (ends(5, "ement"))
				break;
			if (ends(4, "ment"))
				break;
			if (ends(3, "ent"))
				break;
			return;
		case 'o':
			if (ends(3, "ion") && k>5 && j >= 0 && (b.at(j) == 's' || b.at(j) == 't'))
				break;
			if (ends(2, "ou"))
				break;
			return;
		case 's':
			if (ends(3, "ism"))
				break;
			return;
		case 't':
			if (ends(3, "ate"))
				break;
			if (ends(3, "iti"))
				break;
			return;
		case 'u':
			if (ends(3, "ous"))
				break;
			return;
		case 'v':
			if (ends(3, "ive"))
				//r(1, "e");
				break;
			return;
		case'z':
			if (ends(3, "ize"))
				break;
			return;
		default:
			return;
		}
		if (m() > 1)
			k = j;
	}
	void step5()
	{
		j = k;
		if (b.at(k) == 'e')
		{
			int a = m();
			if ((a > 1) || ((a == 1) && !(cvc(k-1))))
				k--;
		}
		if (b.at(k) == 'l' && doublec(k) && m() > 1)
			k--;
	}
	int stem(int a)		//use the steps in series using a single function
	{
		//cout << "stem";
		k = a;
		if (k <= 1)
			return k;

		
		step1ab();

		if (k > 0)
		{
			step1c();
			step2();
			step3();
			step4();
			step5();
		}
		//cout << b[k];
		return k;
	}

	void stemfile(ifstream& f, ofstream& o)		//use the stem function for files
	{

		
		while(f.good())
		{
			f >> b;
			
			j = 0;
			b[stem(b.length()-1) + 1] = 0;
			b.resize(k+1);
			
			o << b;
			cout << b;
			o << endl;
			cout << endl;
		
		}

	}
};
int main(int argc, char** argv)
{
	if (argc != 3)
	{
		cout << "Please enter the file path correctly as command line arguments";
		return 1;
	}
	ifstream fin(argv[1]);
	ofstream fout(argv[2]);
	stemmer z;


	if (fin.fail())
	{
		cout << "File not found";
		return 1;
	}
	z.stemfile(fin, fout);
	fin.close();
	fout.close();
	
	return 0;
}
