#define _CRT_SECURE_NO_WARNINGS
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "restaurant.h"

void ConsoleErrorMsg(const char* msg)
{
	printf("\n%s", msg);
	exit(EXIT_FAILURE);
}

void InputInstructions(FILE* f)
{
	return;
}

void CreateProducts(FILE* in, FILE* out, PItem* menu_head)
{
	char temp[MAX_PRODUCT_NAME + 1];
	PItem NewNode, last;

	while (fscanf(in, "%s", temp) != EOF)
	{
		if (IsNameExistsInMenu(*menu_head, temp))
		{
			fprintf(out, "\nThe dish %s already exists in the menu", temp);
			fscanf(in, "%*d%*d"); // Skips Quantity and Price values of existing Item
		}

		NewNode = (PItem)malloc(sizeof(struct Item));
		if (NewNode == NULL)
		{
			// TODO:
			// Free all allocated memory
			ConsoleErrorMsg("Failed to allocate memory! Exiting...");
		}

		NewNode->Dish.ProductName = (char*)malloc((strlen(temp) + 1) * sizeof(char));
		if (NewNode->Dish.ProductName == NULL)
		{
			// TODO:
			// Free all allocated memory
			free(NewNode);
			ConsoleErrorMsg("Failed to allocate memory! Exiting...");
		}

		strcpy(NewNode->Dish.ProductName, temp);
		fscanf(in, "%d%d", &NewNode->Dish.Quantity, &NewNode->Dish.Price);
		NewNode->Next = NULL;

		if (NewNode->Dish.Quantity <= 0 || NewNode->Dish.Price <= 0)
		{
			fprintf(out, "\nThe %s of the dish cannot be negative", NewNode->Dish.Quantity < 0 ? "quantity" : "price");
			free(NewNode->Dish.ProductName);
			free(NewNode);
			continue;
		}

		/* If the List is empty, sets the Item as the Head */
		if (*menu_head == NULL)
			*menu_head = NewNode;
		else
		{
			last = *menu_head;
			/* Appends new Item to the end of the List */
			while (last->Next != NULL)
				last = last->Next;
			last->Next = NewNode;
		}
	}
	fputs("The kitchen was created", out);
}

void AddItems(FILE* out, PItem* menu_head, const char name[], int quantity)
{
	PItem item = IsNameExistsInMenu(*menu_head, name);
	if (item == NULL)
	{
		fprintf(out, "\nCan't add to %s if it doesn't exist", name);
		return;
	}

	if (quantity <= 0)
	{
		fprintf(out, "\nInvalid quantity was given (%d) to add to %s", quantity, name);
		return;
	}

	item->Dish.Quantity += quantity;
	fprintf(out, "\n%d %s were added to the kitchen", quantity, name);
}

struct Item* IsNameExistsInMenu(PItem menu_head, const char name[])
{
	if (menu_head == NULL)
		return NULL;

	while (menu_head->Next != NULL)
	{
		if (!strcmp(menu_head->Dish.ProductName, name))
			return menu_head;

		menu_head = menu_head->Next;
	}
	return NULL;
}