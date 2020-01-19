// OFC_BRUT_NWW

#include <iostream>
#include <map>
#include <string>
#include <cmath>

struct rangeLCM {
	long int first = -1;
	long int last = -1;
	void range(long int f, long int l)
	{
		first = f;
		last = l;
	}
};

long long int GCD(long long int a, long long int b) //was HCF
{
	return a == 0 ? b : GCD(b % a, a);
	/*
	long long int ad;
	while (b != 0)
	{
		ad = b;
		b = a % b;
		a = ad;
	}
	return a;
	*/
}

long long int LCM(long long int a, long long int b)
{
	return (a * b) / GCD(a, b);
}
long long int multiLCM(long long int a, long long int b)
{
	if (a == b)
		return -1;
	long long int ad = LCM(a, a + 1);
	a++;
	while (a != b)
	{
	    a++;
		ad = LCM(ad, a);
	}
	return ad;
}
void reverseLCM(long long int& value, std::map<long long int, std::string>& fMap)
{
	std::string answer = "";
	if (value % 2 != 0)
	{
		answer = "NIE\n";
		fMap.insert({ value, answer });
		std::cout << answer;
		return;
	}
	rangeLCM currentRange;
	currentRange.range(1, 2);
	if (value == 2)	// one of every 2 countinous numbers is even or odd - 1/2, 2/3 etc. so we can exclude number that not divide by 2
	{
		answer = std::to_string(currentRange.first) + ' ' + std::to_string(currentRange.last) + '\n';
		fMap.insert({ value, answer });
		std::cout << answer;
		return;
	}
	for (long int i = 2; i <= (long int)ceil(sqrt(value)) + 1; i++)	// + 1 just for make sure, that all happens
	{
		if (value % i != 0)
		{
			if (multiLCM(currentRange.first, currentRange.last) == value)
			{
				answer = std::to_string(currentRange.first) + ' ' + std::to_string(currentRange.last) + '\n';
				fMap.insert({ value, answer });
				std::cout << answer;
				return;
			}
			currentRange.range(-1, -1);
			continue;
		}
		if (currentRange.first == -1)
		{
			currentRange.first = i;
		}
		if (i == currentRange.last + 1 || currentRange.last == -1)
		{
			currentRange.last = i;
		}
	}
	answer = "NIE\n";
	fMap.insert({ value, answer });
	std::cout << answer;
}

int main()
{
	std::ios_base::sync_with_stdio(false);
	//std::cin.tie(NULL);
	long long int z;
	std::cin >> z;
	long long int M;
	std::map<long long int, std::string > foundM;
	while (z--)
	{
		std::cin >> M;
		auto search = foundM.find(M);
		if (search != foundM.end())
		{
			std::cout << search->second;
			continue;
		}
		reverseLCM(M, foundM);
	}
}
