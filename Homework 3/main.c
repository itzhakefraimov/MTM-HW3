#include "restaurant.h"

void main()
{
	FILE* instructions, * products, * output;
	Restaurant res;

	res.MenuHead = NULL;

	if ((instructions = fopen(INSTRUCTIONS_FILE, "rt")) == NULL)
		ConsoleErrorMsg("Could not locate file! Exiting...");
	if ((products = fopen(MENU_FILE, "rt")) == NULL)
		ConsoleErrorMsg("Could not locate file! Exiting...");
	if ((output = fopen(OUTPUT_FILE, "wt")) == NULL)
		ConsoleErrorMsg("Could not create file! Exiting...");

	InputInstructions(instructions, products, output, &res);

	fclose(instructions);
	fclose(products);
	fclose(output);
	DeallocateRestaurant(&res);
}