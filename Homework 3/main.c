#define _CRT_SECURE_NO_WARNINGS
#include "stdio.h"
#include "restaurant.h"

void main()
{
	FILE* instructions, * dishes, * output;
	PItem menu = NULL;
	POrder tables_order;

	if ((instructions = fopen(INSTRUCTIONS_FILE, "rt")) == NULL)
		ConsoleErrorMsg("Could not locate file! Exiting...");
	if ((dishes = fopen(ORDERS_FILE, "rt")) == NULL)
		ConsoleErrorMsg("Could not locate file! Exiting...");
	if ((output = fopen(OUTPUT_FILE, "wt")) == NULL)
		ConsoleErrorMsg("Could not create file! Exiting...");

	CreateProducts(dishes, output, &menu);

	fclose(instructions);
	fclose(dishes);
}