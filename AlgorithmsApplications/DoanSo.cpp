#include <iostream>
#include <cstdlib>
#include <ctime>
  
using namespace std;

int sum = 0, temp;
double probability0, probability1;
double count0 = 0, count1 = 0;


void TinhXacSuat()
{	
	double totalCount = count0+count1;
	probability0 = count0/(totalCount);
	probability1 = count1/(totalCount);
	if(probability0 > probability1)
	{
		cout << "Co phai ban nghi so 0";
		cout << "\nXac suat so 0 la: " << probability0*100 << "%";
	}
	else 
	{
		cout << "Co phai ban nghi so 1";
		cout << "\nXac suat so 1 la: " << probability1*100 << "%";
	}
	cout << "\nNhap so ban vua nghi: ";	
	cin >> temp; 
	if(temp == 0) count0++;
	else count1++;	
}

void DoanSo()
{
	if(sum < 7)
	{
		srand(time(NULL)); 
		int finalNum = rand()%(1-0+1)+0;
		cout << "Co phai ban nghi so " << finalNum;
		cout << "\nNhap so ban vua nghi: ";
		cin >> temp;
		sum++;
		if(temp == 0) count0++;
		else count1++;
	}else
		TinhXacSuat();
	
}

int main()
{
	int choice;
	while(true)
	{
		cout << "---------------------\n";
		cout<<"1. Bat dau doan so\n";
		cout<<"0. Thoat\n";
		cout<<"Nhap lua chon cua ban: ";
		cin >> choice;
		system("cls");
		switch(choice)
		{
			case 1:
				DoanSo();
				break;
			case 0:
				exit(0);
				break;
			default:
				cout<<"\nLua chon khong phu hop!!!";
				break;
		}
	}
}

