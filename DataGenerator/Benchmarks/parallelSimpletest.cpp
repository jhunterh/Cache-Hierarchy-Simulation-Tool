#include <iostream>
#include <string>
#include <omp.h>
int main()
{
	int arr[100];
	int i, sum = 0;
	for(i=0;i<100;i++)
		arr[i] = i;
	omp_set_num_threads(4);
	#pragma omp parallel for reduction(+:sum)
	for(i=0;i<100;i++)
		sum += arr[i];
	std::cout <<"Sum: " << sum << '\n';
}
