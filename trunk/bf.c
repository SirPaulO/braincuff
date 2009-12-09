
#include "bflib.h"
#include "wc.bf"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include "XGetOpt.h"

void PrintMemory();
void AnalyzeLoops();
int  ParseArguments(int argc, char **argv);

char bfprogram[] =
	MAIN
	;

char *loopanalysis;

char memory[60000];
char* memptr = &memory[30000];
char* instptr = bfprogram;

char * loopstack[30000];
char ** loopstackptr = loopstack;

char *minmemptr = &memory[30000];
char *maxmemptr = &memory[30000];

char g_debug = 1;
char *g_filename = NULL;

int main(int argc, char **argv)
{
	FILE *fin = NULL;

	if (ParseArguments(argc, argv) == -1)
		return -1;

	memset(memory, 0, 60000);
	memset(loopstack, 0, 30000);
	if (g_debug)
	{
		loopanalysis = strdup(bfprogram);
		AnalyzeLoops();
		printf("Use:\n\tx: quit\n\tg: goto breakpoint\n\tr: run till the end\n\tarrows: nagivate code\n\tany key: step\n\n");
	}
	
	if (g_filename != NULL)
		fin = fopen(g_filename, "rb");

	if (fin == NULL)
		fin = stdin;

	while (*instptr != 0)
	{
		if (g_debug)
			PrintMemory();

		switch (*instptr)
		{
			case '+':
				(*memptr)++;
				instptr++;
				break;
			case '-':
				(*memptr)--;
				instptr++;
				break;
			case '>':
				memptr++;
				if (memptr >= memory+60000)
				{
					printf("Memory overflow +\n");
					exit(1);
				}
				if (memptr > maxmemptr) maxmemptr = memptr;
				instptr++;
				break;
			case '<':
				memptr--;
				if (memptr <= memory)
				{
					printf("Memory overflow -\n");
					exit(1);
				}
				if (memptr < minmemptr) minmemptr = memptr;
				instptr++;
				break;
			case '.':
				printf("%c", *memptr);
				instptr++;
				break;
			case ',':
				*memptr = fgetc(fin);
				if (*memptr == -1)
					*memptr = 4;
				instptr++;
				break;
			case '[':
				if (*memptr == 0)
				{
					int skipcounter = 0;
					instptr++;
					do
					{
						switch (*instptr)
						{
							case 0:
								printf("No loop end\n");
								exit(1);
							case '[':
								skipcounter++;
								break;
							case ']':
								skipcounter--;
								break;
							default:
								break;
						}
						instptr++;
					} while (*instptr != ']' || skipcounter != 0);
					instptr++;
				}
				else
				{
					*loopstackptr = instptr;
					instptr++;
					loopstackptr++;
					if (loopstackptr >= loopstack+30000)
					{
						printf("Stack overflow +\n");
						exit(1);
					}
				}
				break;
			case ']':
				loopstackptr--;
				if (loopstackptr < loopstack)
				{
					printf("No loop beginning\n");
					exit(1);
				}
				instptr = *loopstackptr;
				break;
			default:
				instptr++;
		}
	}
	if (g_debug)
	{
		PrintMemory();
		getch();
	}
	return 0;
}

void PrintMemory()
{
	static printskip = 0;
	static gotobreakpoint = 0; // Only stops on breakpoints ('@')
	static gotoend = 0; // Does not stop at all
	unsigned char *printmemptr;
	char k;

	//return;

	if (gotoend)
		return;

	if (gotobreakpoint && (*instptr != '@'))
		return;
	else
		gotobreakpoint = 0;

	while (instptr-bfprogram < printskip) printskip -= 40;
	while (instptr-bfprogram >= printskip+79) printskip += 40;

	for (printmemptr=bfprogram+printskip ; printmemptr<bfprogram+printskip+79 && *printmemptr!=0 ; printmemptr++)
		if ((*printmemptr == '[') || (*printmemptr == ']'))
			printf("%d", loopanalysis[printmemptr-bfprogram]%10);
		else
			printf(" ");
	printf("\n");
	for (printmemptr=bfprogram+printskip ; printmemptr<bfprogram+printskip+79 && *printmemptr!=0 ; printmemptr++)
		printf("%c", *printmemptr);
	printf("\n");
	for (printmemptr=bfprogram+printskip ; printmemptr<bfprogram+printskip+79 && *printmemptr!=0 ; printmemptr++)
		printf("%c", (instptr==printmemptr)?'^':' ');
	printf("%9d^%9d^%9d^%9d^%9d^%9d^%9d^", printskip+10, printskip+20, printskip+30, printskip+40, printskip+50, printskip+60, printskip+70);
	printf("\n");

	for (printmemptr = minmemptr ; printmemptr <= maxmemptr ; printmemptr++)
		printf("  %c ",  (printmemptr==memptr)?'v':' ');
	printf("\n");
	for (printmemptr = minmemptr ; printmemptr <= maxmemptr ; printmemptr++)
		printf("%03d-", printmemptr - (&memory[30000]));
	printf("\n");
	for (printmemptr = minmemptr ; printmemptr <= maxmemptr ; printmemptr++)
		printf("%03d ", *printmemptr);
	printf("\n");
	
	switch(k=getch())
	{
		case 'x':
		case 'X':
			exit(0);
		case 'g':
		case 'G':
			gotobreakpoint = 1;
			break;
		case 'r':
		case 'R':
			gotoend = 1;
			break;
		case 75:
			if (printskip > 0)
			{
				printskip --;
				PrintMemory();
			}
			break;
		case 77:
			printskip ++;
			PrintMemory();
			break;
		//default:
			//printf("%d\n", k);
	}
	printf("\r");
}

void AnalyzeLoops()
{
	int i, loopdepth=0;
	for (i=0 ; loopanalysis[i] != 0 ; i++)
	{
		switch(loopanalysis[i])
		{
			case '[':
				loopdepth++;
				loopanalysis[i] = loopdepth;
				break;
			case ']':
				loopanalysis[i] = loopdepth;
				loopdepth--;
				break;
			default:
				loopanalysis[i] = loopdepth;
				break;
		}
	}
}

int ParseArguments(int argc, char **argv)
{
	int aflag = 0;
	int bflag = 0;
	char *cvalue = NULL;
	int c;

	while ((c = getopt (argc, argv, "dscah")) != -1)
	switch (c)
	{
		case 'd':
			g_debug = 1;
			break;
		case 's':
			{
				int count=0;
				while(*instptr != 0)
				{
					if (*instptr == '+' || *instptr == '-' ||
						*instptr == '[' || *instptr == ']' || 
						*instptr == '<' || *instptr == '>' || 
						*instptr == ',' || *instptr == '.')
					{
						count++;
					}
					instptr++;
				}
				printf("Code size: %d bytes", count);
			}
			return -1;
		case 'c':
			while(*instptr != 0)
			{
				if (*instptr == '+' || *instptr == '-' ||
					*instptr == '[' || *instptr == ']' || 
					*instptr == '<' || *instptr == '>' || 
					*instptr == ',' || *instptr == '.')
				{
					printf("%c", *instptr);
				}
				instptr++;
			}
			return -1;
		case 'a':
			printf("%s", bfprogram);
			return -1;
		case 'h':
			printf("Use:\n"
				"\t-d debug mode\n"
				"\t-s print source code size\n"
				"\t-c print source code\n"
				"\t-a print annotated source code\n"
				"\t-h print this help\n");
			return -1;
		case '?':
			/*
			if (optopt == 'c')
				fprintf (stderr, "Option -%c requires an argument.\n", optopt);
			else if (isprint (optopt))
				fprintf (stderr, "Unknown option `-%c'.\n", optopt);
			else
				fprintf (stderr,
					"Unknown option character `\\x%x'.\n",
					optopt);
			*/
			return -1;
		default:
			g_filename = strdup(optarg);
			return 0;
	}

	return 0;
}