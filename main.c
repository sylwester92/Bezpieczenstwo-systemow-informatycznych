#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXLINES  1000
#define MAXCHARSPERLINE 41
#define bufSize 1024
int nlines = 0;
int nlines2 = 0;
char line[MAXLINES][MAXCHARSPERLINE];
char line2[1000][100];


/* Deklaracja funkcji dziel¹cej tekst na bloki */
ReadLines(char* filename)
{
	FILE* f;
	int n;
	char buf[MAXCHARSPERLINE];
	if ((f = fopen(filename, "r")) == NULL)
		return fprintf(stderr, "%: can't open for reading\n"), -1;
	for (nlines = 0; fgets(buf, MAXCHARSPERLINE, f) != NULL; nlines++)
	{
		n = strlen(buf);
		strcpy(line[nlines], buf);
	}
	return nlines;
}

/* Funckja sprawdzaj¹ca zgodnoœæ CRC */
check()
{
	FILE* fp;
	char buf[bufSize];
	int i = 0;
	int j = 1;
	int a = 0xFFFFFFFF;
	int x = 0xFFFFFFFF;
	if ((fp = fopen("crc_separated.txt", "r")) == NULL)
	{ 
		perror("fopen source-file");
		return 1;
	}

	while (fgets(buf, sizeof(buf), fp) != NULL)
	{
		buf[strlen(buf) - 1] = '\0'; 
		if (i % 2 == 0)
		{
			a = crc32b_2(buf); // Liczenie CRC na nowo
		}
		if (i % 2 != 0)
		{

			printf("CRC z pliku 'crc_separated.txt' dla %d wiersza tekstu : %s", j, buf);
			printf("   |     CRC obliczone na nowo: 0x%x\n", a);
			j++;
			/* Sprawdzenie, czy stringi s¹ równe */
			sscanf(buf, "%x", &x);

			if (x == a)
			{
				printf("[DANE POPRAWNE: 0x%x == 0x%x]\n", x,a);

			}
			else
			{
				printf("[DANE NIEPOPRAWNE!!!: 0x%x != 0x%x]\n", x, a);
			}
		}
		i++;
	
	}
	fclose(fp);
	return nlines2;
}
/* Deklaracja algorytmu CRC-32/JAMCRC z zapisywaniem do pliku tekstowego */
unsigned int crc32b(unsigned char* message, FILE* some_file)
{
	int i, j;
	unsigned int byte, crc, mask;
	i = 0;
	crc = 0xFFFFFFFF;
	while (message[i] != 0) {
		byte = message[i];
		crc = crc ^ byte;
		for (j = 7; j >= 0; j--) {
			mask = 0 - (crc & 1);
			crc = (crc >> 1) ^ (0xEDB88320 & mask);
		}
		i = i + 1;
	}
	printf("0x%x \n", crc);
	fprintf(some_file, "%s\n", message); // Zapis bloku do pliku tekstowego
	fprintf(some_file, "0x%x\n", crc); // Zapis CRC do pliku tekstowego
	return ~crc;

}

/* Deklaracja algorytmu CRC-32/JAMCRC bez zapisywania do pliku tekstowego */
crc32b_2(unsigned char* message)
{
	int i, j;
	unsigned int byte, crc, mask;
	i = 0;
	crc = 0xFFFFFFFF;
	while (message[i] != 0) {
		byte = message[i];
		crc = crc ^ byte;
		for (j = 7; j >= 0; j--) {
			mask = 0 - (crc & 1);
			crc = (crc >> 1) ^ (0xEDB88320 & mask);
		}
		i = i + 1;
	}
	return crc;

}

main()
{
	FILE* crc_file;
	crc_file = fopen("crc_separated.txt", "wb+");

	/* Czytanie i rozdzielanie tekstu na bloki */
	ReadLines("data.txt");

	/* Wyœwietlanie tekstu oraz CRC dla poszczególnych bloków */
	for (int i = 0; i < nlines; i++)
	{

		printf("Data: %s , CRC:", line[i]);
		crc32b(line[i], crc_file);
	}


	fclose(crc_file);
	printf("\nData also has been printed to crc_separated.txt file\n\n");
	check(); // Sprawdzanie zgodnoœci CRC
	
	
	for (int i = 0; i < nlines2; i=+2)
	{

		printf("Data: %s", line2[i]);
		
	}



}