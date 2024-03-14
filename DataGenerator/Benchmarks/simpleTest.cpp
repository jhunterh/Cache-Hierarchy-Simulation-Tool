#include <iostream>
#include <string>

int main()
{
	int arr[100];
	int i=0, sum = 0;
	for(i=0;i<100;i++)
		arr[i] = i;
	for(i=0;i<100;i++)
		sum += arr[i];
	std::cout <<"Sum: " << sum << '\n';
}
