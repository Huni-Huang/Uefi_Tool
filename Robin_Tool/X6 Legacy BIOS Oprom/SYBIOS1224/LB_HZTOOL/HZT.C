#include	<stdio.h>
#include	<stdlib.h>
#include	<conio.h>


struct	hzk16_typ{
	unsigned code;
	unsigned array[16];
	};

int	main(int argc,char *argv[])
{
	int	i=0,j=0;
	struct	hzk16_typ  hz;
	FILE	*in,*out;
	if(argc != 3)
	{
		printf("Syntax:\thzt\tinfile\tdstfile\n");
		exit(1);
	}
	if(!(in = fopen(argv[1],"rb")))
	{
		printf("Can't open srcfile\n");
		exit(1);
	}
	if(!(out=fopen(argv[2],"w")))
	{
		printf("Can't open dstfile\n");
		exit(1);
	}

//	printf("total count:\t");
//	scanf("%d",&count);
//	puts("");
//	fprintf(out,"#define\t\thz_count\t\t%d\n",count);
	fprintf(out,"HZK16TABLE");
	for(i=0;!feof(in);i++)
	{
		fread(&hz,sizeof(hz),1,in);
		fprintf(out,"\n						DW  0%04xH,",hz.code);
		for(j=0;j<15;j++)
			fprintf(out,"0%04xH,",hz.array[j]);
		fprintf(out,"0%04x",hz.array[j]);
		fprintf(out,"H");

	}
	fprintf(out,"\n;");
	fclose(in);
	fclose(out);
	return	0;
}
