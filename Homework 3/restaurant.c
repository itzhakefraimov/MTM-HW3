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
		ConsoleErrorMsg("Tables Amount must be a positive number! Exiting...");

	if (Instruction != 1)
		ConsoleErrorMsg("First Instruction must be number 1! Exiting...");
	
	if ((res->Tables = (PTable)malloc(TablesCount * sizeof(Table))) == NULL)
		ConsoleErrorMsg(ERROR_MEM_ALLOCATION_MSG);

	for (i = 0; i < TablesCount; i++)
		res->Tables[i].OrderHead = NULL;

	res->MaxTables = TablesCount;
	CreateProducts(menu, out, res);

	while (fscanf(ins, "%d", &Instruction) != EOF)
	{
		switch (Instruction)
		{
		case 1:
			if (res->MenuHead != NULL)
				fprintf(out, "The kitchen was already created, disregarding Instruction");
			else
				CreateProducts(menu, out, res);
			break;
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
		case 5:
			fscanf(ins, "%d", &TableNum);
			RemoveTable(out, res, TableNum);
			break;
		}
	}
}

void CreateProducts(FILE* in, FILE* out, PRestaurant res)
{
	char Temp[MAX_PRODUCT_NAME + 1];
	PItem NewNode, Last;

	while (fscanf(in, "%s", Temp) != EOF)
	{
		if (IsNameExistsInMenu(res->MenuHead, Temp))
		{
			fprintf(out, "\nThe dish %s already exists in the menu\n", Temp);
			fscanf(in, "%*d%*d"); // Skips Quantity and Price values of existing Item
			continue;
		}

		if ((NewNode = (PItem)malloc(sizeof(struct Item)))== NULL)
		{
			DeallocateRestaurant(res);
			ConsoleErrorMsg(ERROR_MEM_ALLOCATION_MSG);
		}

		NewNode->Dish.ProductName = (char*)malloc((strlen(Temp) + 1) * sizeof(char));

		if (NewNode->Dish.ProductName == NULL)
		{
			DeallocateRestaurant(res);
			free(NewNode);
			ConsoleErrorMsg(ERROR_MEM_ALLOCATION_MSG);
		}

		strcpy(NewNode->Dish.ProductName, Temp);
		NewNode->TotalOrdered = 0;
		fscanf(in, "%d%d", &NewNode->Dish.Quantity, &NewNode->Dish.Price);
		NewNode->Next = NULL;

		if (NewNode->Dish.Quantity <= 0 || NewNode->Dish.Price <= 0)
		{
			fprintf(out, "The %s of the dish must be a positive number\n", NewNode->Dish.Quantity < 0 ? "quantity" : "price");
			free(NewNode->Dish.ProductName);
			free(NewNode);
			continue;
		}

		/* If the List is empty, sets the Item as the Head */
		if (res->MenuHead == NULL)
			res->MenuHead = NewNode;
		else
		{
			Last = res->MenuHead;
			/* Appends new Item to the end of the List */
			while (Last->Next != NULL)
				Last = Last->Next;
			Last->Next = NewNode;
		}
	}
	fputs("The kitchen was created", out);
}

void OrderItem(FILE* out, PRestaurant res, int table_num, const char name[], int quantity)
{
	POrder OrderInfo;
	PItem ItemFromMenu;
	PTable Table;

	if (!ValidateTableNumber(out, table_num, res->MaxTables))
		return;

	if ((ItemFromMenu = IsNameExistsInMenu(res->MenuHead, name)) == NULL)
	{
		fprintf(out, "\nWe don't have %s, sorry!", name);
		return;
	}

	if (quantity <= 0)
	{
		fprintf(out, "\nOrder Quantity must be positive number");
		return;
	}

	if (quantity > ItemFromMenu->Dish.Quantity)
	{
		fprintf(out, "\nWe don't have enough %s for your order, sorry!", name);
		return;
	}

	Table = &res->Tables[table_num - 1];

	if ((OrderInfo = IsTableHasOrder(Table->OrderHead, name)) == NULL)
	{
		if ((OrderInfo = (POrder)malloc(sizeof(Order))) == NULL)
		{
			DeallocateRestaurant(res);
			ConsoleErrorMsg(ERROR_MEM_ALLOCATION_MSG);
		}

		OrderInfo->Next = Table->OrderHead;
		OrderInfo->Prev = NULL;
		OrderInfo->Data = ItemFromMenu->Dish;
		OrderInfo->Data.Quantity = 0;

		if (Table->OrderHead != NULL)
			Table->OrderHead->Prev = OrderInfo;
		else
			Table->Bill = 0;

		Table->OrderHead = OrderInfo;
	}

	OrderInfo->Data.Quantity += quantity;
	Table->Bill += (quantity * ItemFromMenu->Dish.Price);
	ItemFromMenu->Dish.Quantity -= quantity;
	ItemFromMenu->TotalOrdered += quantity;
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
		fprintf(out, "\nQuantity of %s to add must be a positive number", name);
		return;
	}

	ItemFromMenu->Dish.Quantity += quantity;
	fprintf(out, "\n%d %s were added to the kitchen", quantity, name);
}

void RemoveItem(FILE* out, PRestaurant res, int table_num, const char name[], int quantity)
{
	POrder OrderInfo;

	if (!ValidateTableNumber(out, table_num, res->MaxTables))
		return;

	if (res->Tables[table_num - 1].OrderHead == NULL)
	{
		fprintf(out, "\nTable number %d is not ordered yet", table_num);
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

void RemoveTable(FILE* out, PRestaurant res, int table_num)
{
	PItem MostOrderedItem;
	POrder OrderInfo;

	if (!ValidateTableNumber(out, table_num, res->MaxTables))
		return;

	if (res->Tables[table_num - 1].OrderHead == NULL)
	{
		fprintf(out, "\nTable number %d is not ordered yet", table_num);
		return;
	}

	OrderInfo = res->Tables[table_num - 1].OrderHead;

	fprintf(out, "\n");

	while (OrderInfo != NULL)
	{
		fprintf(out, "%d %s. ", OrderInfo->Data.Quantity, OrderInfo->Data.ProductName);
		OrderInfo = OrderInfo->Next;
	}

	fprintf(out, "%d nis, please!", res->Tables[table_num - 1].Bill);

	if (IsLastRemainingTable(res) && (MostOrderedItem = GetMostOrderedItem(res->MenuHead)) != NULL)
		fprintf(out, "\nThe most popular dish today is %s! (was ordered %d times)", MostOrderedItem->Dish.ProductName, MostOrderedItem->TotalOrdered);
	
	DeallocateTableData(&res->Tables[table_num - 1]);
}

BOOL ValidateTableNumber(FILE* out, const int table_num, const int max_tables)
{
	if (table_num > max_tables || table_num <= 0)
	{
		fprintf(out, "\nTable number %d doesn't exist", table_num);
		return false;
	}
	return true;
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

BOOL IsLastRemainingTable(PRestaurant res)
{
	int i, count = 0;

	for (i = 0; i < res->MaxTables; i++)
		if (res->Tables[i].OrderHead != NULL)
			count++;
	return (count == 1) ? true : false;
}

PItem GetMostOrderedItem(PItem menu_head)
{
	PItem res = menu_head;

	if (menu_head == NULL)
		return NULL;

	while (menu_head != NULL)
	{
		if (menu_head->TotalOrdered > res->TotalOrdered)
			res = menu_head;
		menu_head = menu_head->Next;
	}

	return res->TotalOrdered ? res : NULL;
}

void DeallocateTableData(PTable table)
{
	POrder Current = table->OrderHead, Next;

	table->Bill = 0;
	table->OrderHead = NULL;

	while (Current != NULL) {
		Next = Current->Next;
		Current->Data.ProductName = NULL;
		Current->Data.Quantity = 0;
		Current->Data.Price = 0;
		free(Current);
		Current = Next;
	}
}

void DeallocateRestaurant(PRestaurant res)
{
	PItem CurrentItem = res->MenuHead, Next;
	int i;

	for(i = 0; i < res->MaxTables; i++)
		DeallocateTableData(&res->Tables[i]);

	free(res->Tables);

	while (CurrentItem != NULL)
	{
		Next = CurrentItem->Next;
		free(CurrentItem->Dish.ProductName);
		free(CurrentItem);
		CurrentItem = Next;
	}
}