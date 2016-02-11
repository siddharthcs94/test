#include<stdio.h>
#include<alsa/asoundlib.h>
#include<sys/mman.h>


int main()
{

size_t page_size = (size_t)sysconf(_SC_PAGE_SIZE);

printf("\n%ld\n",page_size);




int prot,flag;
size_t len;
off_t offset;
int res;
res = open("sample.wav",O_RDONLY);
void *p;

p=mmap(0/*void *addr*/, len, PROT_READ, MAP_SHARED/*int flag*/, res/*int res*/, 0/* off_t offset*/);

}

