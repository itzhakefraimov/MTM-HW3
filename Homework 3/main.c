#include "restaurant.h"

void main()
{
	FILE* instructions, * products, * output;
	Restaurant res;

	res.MenuHead = NULL;

	if ((instructions = fopen(INSTRUCTIONS_FILE, "rt")) == NULL)
		ConsoleErrorMsg(ERROR_LOCATE_FILE_MSG);
	if ((products = fopen(MENU_FILE, "rt")) == NULL)
		ConsoleErrorMsg(ERROR_LOCATE_FILE_MSG);
	if ((output = fopen(OUTPUT_FILE, "wt")) == NULL)
		ConsoleErrorMsg(ERROR_LOCATE_FILE_MSG);

	InputInstructions(instructions, products, output, &res);

	fclose(instructions);
	fclose(products);
	fclose(output);
	DeallocateRestaurant(&res);
}