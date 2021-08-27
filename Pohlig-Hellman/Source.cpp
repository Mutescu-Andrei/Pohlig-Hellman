#include <iostream>
#include <cmath>
using namespace std;
struct primediv {														//tinem divizorii si puterea lor + marimea 
	int long long d[100];
	int putere[100], size;
};
struct coef {															//salvam coeficientii
	int long long aputere[100];
	int c[100];
	int size;
};


void initialprime(int long long pminus, primediv& div)								//calculam divizori primi
{
	int i = 2, j = 0;

	while (pminus > 1)
	{
		if (pminus % i == 0)
		{
			div.d[j] = i;
			div.putere[j] = 1;
			pminus = pminus / i;
			while (pminus % i == 0)
			{
				div.putere[j]++;
				pminus = pminus / i;
			}
			j++;


		}
		i++;
	}
	div.size = j;
}

int CalcX(int long long p, int long long b, int long long piminus)		//calculam x_i=x mod(d^putere)	ex: x_1=x mod (2^2)
{
	int prod = 1, nr = 0;
	while (nr < piminus)
	{
		prod = prod * b;
		prod = prod % p;
		nr++;

	}
	return prod;

}
int search(int long long val, coef a[100])										//cautam locul in struct dupa valoare
{
	for (int j = 0; j <= a->size; j++)
		if (val == a->aputere[j])
			return a->c[j];

	return 1;
}

int lowerX(int long long p, int long long b, int long long y, int loci, primediv& div, coef a[100])		//aici aflam  coeficientii lui x_i in funcite de dimensiune
{
	int found = 0;
	int x = 0;
	int PreCi;
	int j = 0;
	for (int x_i = 0; x_i < div.d[loci]; x_i++)							//calculam toate versiunile de a^[i*(p-1)/divizor] cu i<divizor
	{
		PreCi = CalcX(p, b, (p - 1) / div.d[loci] * x_i);				//pre-coeficient
		a->aputere[j] = PreCi;											//salvam a la putere
		a->c[j++] = x_i;
		cout << "Pentru x_" << div.d[loci] << " avem : ";
		cout << "a^" << "(" << x_i << "*" << p - 1 << "/" << div.d[loci] << ")" << "=" << PreCi << endl;


	}
	a->size = j;
	cout << endl << endl << "x = ";										//din ce e format x_i
	for (int i = 0; i < div.putere[loci]; i++)
	{
		cout << pow(div.d[loci], i) << "x" << i;
		if (i != div.putere[loci] - 1)cout << " + ";
	}


	cout << endl << endl << endl;

	for (int x_i = 0; x_i < div.putere[loci]; x_i++)					//divizam aici y cu a^divizor*c_i dupa facem mod p  
	{
		int long long putere1 = pow(div.d[loci], x_i + 1);


		PreCi = CalcX(p, y, (p - 1) / putere1);
		found = search(PreCi, a);

		cout << "y_" << x_i << "^(" << p - 1 << "/" << putere1 << ")= " << PreCi << endl << endl;

		x += pow(div.d[loci], x_i) * found;								//aflam solutia lui x adunand toti divizorii cu coeficientul gasit


		PreCi = -found;
		while (PreCi < 0)
			PreCi += p - 1;												//il reincadram

		PreCi = CalcX(p, b, pow(div.d[loci], x_i) * PreCi);				//calculam a la putere

		if (x_i != div.putere[loci] - 1)									//pentru estetica, nu sunt necesare
			cout << "Divizam y = " << y << " cu a^" << putere1 / div.d[loci] << "c" << x_i << endl;

		y = y * PreCi;
		y %= p;

		if (x_i != div.putere[loci] - 1)									//pentru estetica, nu sunt necesare
			cout << "y" << x_i + 1 << " = " << y << endl;
	}
	PreCi = pow(div.d[loci], div.putere[loci]);

	cout << "Solutia pentru x_" << div.d[loci] << " = " << x % PreCi << endl << endl << endl;

	return x % PreCi;
}

int long long Inverse(int long long a, int long long b)	//facem varianta iterativa la inversul modular
{
	int long long b0 = b;
	int long long y = 0, x = 1;
	if (b == 1) return 1;
	while (a > 1) {
		int long long q = a / b;
		int long long t = b;

		//Euclid
		b = a % b;
		a = t;
		t = y;

		//reinitializam
		y = x - q * y;
		x = t;
	}
	if (x < 0) x += b0;
	return x;
}

int long long ChineseTheorem(int long long p, primediv& div, coef a[100], int xF[100])
{
	int long long x = 0, M = 1;

	for (int i = 0; i < div.size; i++)
	{
		M = M * pow(div.d[i], div.putere[i]);
	}
	for (int i = 0; i < div.size; i++)
	{
		int long long D = pow(div.d[i], div.putere[i]);							//facem puterea o data
		int long long inv = Inverse(M / D, D);										//inversul

		cout << "Inversul lui M" << i << "(" << M / D << ")  = " << inv << endl;
		cout << endl;
		x = x + xF[i] * M / D * inv;

		if (x > p - 1)													//daca trece reducem
			x %= (p - 1);
		while (x < 0)													//il facem pozitiv
		{
			x += p - 1;
		}
	}


	return x;
}


int main()
{
	primediv div = {};
	coef a[100] = {};
	int x[100] = {};
	int long long p, b, y;
	cout << "Dati numarul prim (modulul) p=";
	cin >> p;

	cout << endl;
	cout << "Dati generatorul b=";
	cin >> b;
	cout << endl;

	cout << "Aflam x stiind ca y=";
	cin >> y;

	cout << endl;

	initialprime(p - 1, div);
	cout << endl;

	cout << "Puterile modulului sunt: " << endl;
	for (int i = 0; i < div.size; i++)
		cout << "div:" << div.d[i] << "^" << div.putere[i] << endl;

	cout << endl;

	for (int i = 0; i < div.size; i++)
		x[i] = lowerX(p, b, y, i, div, a);

	cout << endl;
	cout << endl;

	cout << "Avem solutiile: ";
	for (int i = 0; i < div.size; i++)
		cout << x[i] << " ";

	cout << endl;
	cout << endl;

	cout << "Folosind Teorema Chineza a resturilor x=" << ChineseTheorem(p, div, a, x);

	cout << endl;
	cout << endl;

	system("pause");
	return 0;
}