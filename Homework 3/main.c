#define _CRT_SECURE_NO_WARNINGS
#include "stdio.h"
#include "restaurant.h"

void main()
{
	FILE* instructions, * dishes, * output;
	Restaurant res;

	res.MenuHead = NULL;

	if ((instructions = fopen(INSTRUCTIONS_FILE, "rt")) == NULL)
		ConsoleErrorMsg("Could not locate file! Exiting...");
	if ((dishes = fopen(ORDERS_FILE, "rt")) == NULL)
		ConsoleErrorMsg("Could not locate file! Exiting...");
	if ((output = fopen(OUTPUT_FILE, "wt")) == NULL)
		ConsoleErrorMsg("Could not create file! Exiting...");

	InputInstructions(instructions, dishes, output, &res);

	fclose(instructions);
	fclose(dishes);
}