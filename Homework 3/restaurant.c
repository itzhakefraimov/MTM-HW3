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

void InputInstructions(FILE* ins, FILE* menu, FILE* out, PRestaurant res)
{
	int i,
		TablesCount, Instruction,
		TableNum, Quantity;
	char Name[MAX_PRODUCT_NAME + 1];

	fscanf(ins, "%d%d", &TablesCount, &Instruction);

	if (TablesCount <= 0)
		ConsoleErrorMsg("Tables Number must be positive! Exiting...");

	if (Instruction != 1)
		ConsoleErrorMsg("First Instruction must be number 1! Exiting...");
	
	if ((res->Tables = (PTable)malloc(TablesCount * sizeof(Table))) == NULL)
		ConsoleErrorMsg(MEM_ALLOCATION_MSG);

	for (i = 0; i < TablesCount; i++)
		res->Tables[i].OrderHead = NULL;

	res->MaxTables = TablesCount;
	CreateProducts(menu, out, res);

	while (fscanf(ins, "%d", &Instruction) != EOF)
	{
		switch (Instruction)
		{
		case 2:
			fscanf(ins, "%s%d", Name, &Quantity);
			AddItems(out, &res->MenuHead, Name, Quantity);
			break;
		case 3:
			fscanf(ins, "%d%s%d", &TableNum, Name, &Quantity);
			OrderItem(out, res, TableNum, Name, Quantity);
			break;
		case 4:
			fscanf(ins, "%d%s%d", &TableNum, Name, &Quantity);
			RemoveItem(out, res, TableNum, Name, Quantity);
			break;
		}
	}

}

void CreateProducts(FILE* in, FILE* out, PRestaurant res)
{
	char temp[MAX_PRODUCT_NAME + 1];
	PItem NewNode, last;

	while (fscanf(in, "%s", temp) != EOF)
	{
		if (IsNameExistsInMenu(res->MenuHead, temp))
		{
			fprintf(out, "\nThe dish %s already exists in the menu", temp);
			fscanf(in, "%*d%*d"); // Skips Quantity and Price values of existing Item
			continue;
		}

		NewNode = (PItem)malloc(sizeof(struct Item));
		if (NewNode == NULL)
		{
			// TODO:
			// Free all allocated memory
			ConsoleErrorMsg(MEM_ALLOCATION_MSG);
		}

		NewNode->Dish.ProductName = (char*)malloc((strlen(temp) + 1) * sizeof(char));

		if (NewNode->Dish.ProductName == NULL)
		{
			// TODO:
			// Free all allocated memory
			free(NewNode);
			ConsoleErrorMsg(MEM_ALLOCATION_MSG);
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
		if (res->MenuHead == NULL)
			res->MenuHead = NewNode;
		else
		{
			last = res->MenuHead;
			/* Appends new Item to the end of the List */
			while (last->Next != NULL)
				last = last->Next;
			last->Next = NewNode;
		}
	}
	fputs("The kitchen was created", out);
}

void OrderItem(FILE* out, PRestaurant res, int table_num, const char name[], int quantity)
{
	POrder OrderInfo;
	PItem ItemFromMenu;
	PTable Table;

	if (table_num > res->MaxTables || table_num <= 0)
	{
		fprintf(out, "\nTable number %d doesn't exist", table_num);
		return;
	}

	if (quantity <= 0)
	{
		fprintf(out, "\nOrder Quantity must be positive number");
		return;
	}

	if ((ItemFromMenu = IsNameExistsInMenu(res->MenuHead, name)) == NULL)
	{
		fprintf(out, "\nWe don't have %s, sorry!", name);
		return;
	}

	if (quantity > ItemFromMenu->Dish.Quantity)
	{
		fprintf(out, "\nWe have only %d %s, sorry!", ItemFromMenu->Dish.Quantity, name);
		return;
	}

	if ((OrderInfo = (POrder)malloc(sizeof(Order))) == NULL)
	{
		// TODO:
		// Free all allocated memory
		ConsoleErrorMsg(MEM_ALLOCATION_MSG);
	}

	Table = &res->Tables[table_num - 1];
	OrderInfo->Next = Table->OrderHead;
	OrderInfo->Prev = NULL;
	OrderInfo->Data = ItemFromMenu->Dish;
	OrderInfo->Data.Quantity = quantity;

	if (Table->OrderHead != NULL)
		Table->OrderHead->Prev = NULL;
	else
		Table->Bill = 0;

	Table->OrderHead = OrderInfo;

	Table->Bill += (quantity * ItemFromMenu->Dish.Price);
	ItemFromMenu->Dish.Quantity -= quantity;
	fprintf(out, "\n%d %s were added to table number %d", quantity, name, table_num);
}

void AddItems(FILE* out, PItem* menu_head, const char name[], int quantity)
{
	PItem ItemFromMenu = IsNameExistsInMenu(*menu_head, name);

	if (ItemFromMenu == NULL)
	{
		fprintf(out, "\nCan't add to %s if it doesn't exist", name);
		return;
	}

	if (quantity <= 0)
	{
		fprintf(out, "\nInvalid quantity was given (%d) to add to %s", quantity, name);
		return;
	}

	ItemFromMenu->Dish.Quantity += quantity;
	fprintf(out, "\n%d %s were added to the kitchen", quantity, name);
}

void RemoveItem(FILE* out, PRestaurant res, int table_num, const char name[], int quantity)
{
	POrder OrderInfo;

	if (table_num > res->MaxTables || table_num <= 0)
	{
		fprintf(out, "\nTable number %d doesn't exist", table_num);
		return;
	}

	if (quantity <= 0)
	{
		fprintf(out, "\nReturn Quantity must be a positive number");
		return;
	}

	if ((OrderInfo = IsTableHasOrder(res->Tables[table_num - 1].OrderHead, name)) == NULL)
	{
		fprintf(out, "\nYou didn't order %s, therefore you cannot return it", name);
		return;
	}

	if (quantity > OrderInfo->Data.Quantity)
	{
		fprintf(out, "\nYou can't return %d %s if you only ordered %d", quantity, name, OrderInfo->Data.Quantity);
		return;
	}

	res->Tables[table_num - 1].Bill -= (quantity * OrderInfo->Data.Price);
	OrderInfo->Data.Quantity -= quantity;

	/* if the customer returned exactly the amount he orderd
	*  the order will be removed and previous and next nodes
	*  will be connected to each other					  */
	if (!OrderInfo->Data.Quantity)
	{
		OrderInfo->Prev->Next = OrderInfo->Next;
		OrderInfo->Next->Prev = OrderInfo->Prev;
		free(OrderInfo);
	}
	fprintf(out, "\n%d %s was returned to the kitchen from table number %d", quantity, name, table_num);
}

PItem IsNameExistsInMenu(PItem menu_head, const char name[])
{
	if (menu_head == NULL)
		return NULL;

	while (menu_head != NULL)
	{
		if (!strcmp(menu_head->Dish.ProductName, name))
			return menu_head;

		menu_head = menu_head->Next;
	}
	return NULL;
}

POrder IsTableHasOrder(POrder order_head, const char name[])
{
	if (order_head == NULL)
		return NULL;

	while (order_head != NULL)
	{
		if (!strcmp(order_head->Data.ProductName, name))
			return order_head;

		order_head = order_head->Next;
	}
	return NULL;
}