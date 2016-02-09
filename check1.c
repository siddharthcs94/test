



#include<stdio.h>
#include<string.h>

struct student {
			char name[20];
			int roll;
		};

int main()
{


FILE *fp = fopen("file1.txt","wb");
if(!fp)
printf("\nError in opening file\n");

int ch;
char c,nam[20];
struct student s;
int i;


for(i=0;i<3;i++)
	{	
		//printf("\n%d\n",i);

		fgets(s.name,10,stdin);
		fflush(stdin);

		scanf(" %d",&s.roll);
		//fflush(stdin);

		printf("\nNumber is : %d \n",s.roll);
		while(getc(stdin)!='\n');
		fwrite(&s,sizeof(s),1,fp);

	}



}

