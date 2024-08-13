/*
* File name: project.cpp
* Project: SENG1050 - project
* Programmer: Mohit Chodavdeeya (8963902)
*			  Raj Rana (8960902)
* Date of version created: 2024-08-07
* Description: The below presented program calculate the different statistics about the	items to be delivered 
*			   through courier company using Hash Table and Tree Structure.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HASH_TABLE_SIZE 127

// Structure defination for parcel, representing each parcel in the system
typedef struct Parcel
{
	char* destination;   // dynamically allocated string for the destination country
	int weight;   // weight of the parcel in grams
	float valuation;   // valuation of the parcel in dollars
	struct Parcel* left;   // pointer to the left child in BST
	struct Parcel* right;   // pointer to the right child in BST
} Parcel;

//
// FUNCTION: hash
// DESCRIPTION: 
//		This function is generating hash value from a string like 
//		country name with the help of algorithm.
//PARAMETERS: 
//		char* str: the string like country name for which the hash value 
//		is getting generated.
// RETURNS: 
//		unsigned long: the hash value which is limited to the range of
//		hash table size.
//	
unsigned long hash(char* str)
{
	unsigned long hash = 5381;
	int c;
	while ((c = *str++))
	{
		hash = ((hash << 5) + hash) + c;
	}
	return hash % HASH_TABLE_SIZE;   // returns the hash value within the range of the hash table size
}

//
// FUNCTION:
//		createParcel
// DESCRIPTION: 
//		This function cretaes a new Parcel structure with provided country,
//		weight and valuation.
// PARAMETERS:
//		char* country: the destination country for the parcel.
//		int weight: the weight of the Parcel in grams.
//		float valuation: the valuation of the parcel in dollars.
// RETURNS:
//		Parcel*: which is a pointer to the newly created Parcel structure
//		or exits on memory allocation failure.
//
Parcel* createParcel(char* country, int weight, float valuation)
{
	Parcel* newParcel = (Parcel*)malloc(sizeof(Parcel)); // allocate memory for new parcel
	if (newParcel == NULL)
	{
		fprintf(stderr, "Error: Memory allocation failed for newParcel.\n");
		exit(1);   // Exit the program if memory allocation got failed
	}

	newParcel->destination = (char*)malloc(strlen(country) + 1); // allocate memory for the country name
	if (newParcel->destination == NULL)
	{
		fprintf(stderr, "Error: Memory allocation failed for destination.\n");
		free(newParcel);   // Free the already allocated memory for newParcel
		exit(1);   // Exit program if memory allocation got failed
	}

	strcpy_s(newParcel->destination, strlen(country) + 1, country); // copy the country name to the destination
	newParcel->weight = weight; // set the weight of the parcel
	newParcel->valuation = valuation; // set the valuation of parcel
	newParcel->left = newParcel->right = NULL; // initialize left and right child childeren to NULL
	return newParcel;
}

//
// FUNCTION: insertIntoBst
// DRSCRIPTION: 
//		This function insert a new parcel into the BST based on weight of the parcel.
// PARAMETERS:
//		Parcel** root: double pointer to the root of the BST where the parcel is to be inserted.
//		Parcel* newParcel: a pointer to the parcel that needs to be inserted into the BST.
// RETURNS:
//		void: this function does not return a value.
//
void insertIntoBst(Parcel** root, Parcel* newParcel)
{
	if (*root == NULL)
	{
		*root = newParcel;   // if the root is NULL, insert new parcel here
	}
	else if (newParcel->weight < (*root)->weight)
	{
		insertIntoBst(&(*root)->left, newParcel);   // recursively insert into the left subtree if weight is less
	}
	else
	{
		insertIntoBst(&(*root)->right, newParcel);   // recursively insert into the right subtree if weight is more or equal
	}
}

//
// FUNCTION: insertIntoHashTable
// DESCRIPTIPN: 
//		This function inserts a parcel into the hash table based on the country name.
// PARAMETERS: 
//		Parcel* hashTable[]: the hash table where the parcel will get inserted.
//		char* country: the destination country of the parcel.
//		int weight: the weight of the parcel in grams.
//		float valuation: the valuation of the parcel in dollars.
// RETURNS:
//		void: this function does not return a value.
//
void insertIntoHashTable(Parcel* hashTable[], char* country, int weight, float valuation) 
{
	unsigned long index = hash(country);   // generate a hash value based on the country name
	Parcel* newParcel = createParcel(country, weight, valuation);   // create a new parcel
	insertIntoBst(&hashTable[index], newParcel);   // insert the parcel into the appropriate BST within the hash table
}

// 
// FUNCTION: loadData
// DESCRIPTION: 
//		This function loads data from a file into the hash table.
// PARAMETERS: 
//		Parcel* hashTable[]: the hash table where the data will be loaded.
//		const char* filename: the name of the file which is containing data.
//		const char* validCountries[]: the list of valid country names.
//		size_t numCountries: the number of valid countries.
// RETURNS:
//		void: this function does not return a value.
//
void loadData(Parcel* hashTable[], const char* filename, const char* validCountries[], size_t numCountries)
{
	FILE* file;
	fopen_s(&file, filename, "r");   // open the file for reading
	if (!file)
	{
		fprintf(stderr, "Error: Unable to open file %s\n", filename);   // print error message if file can't be opened
		exit(1);
	}

	char country[21];
	int weight;
	float valuation;

	// reading each line pf the file and insert data into the hash table
	while (fscanf_s(file, "%20[^,], %d, %f\n", country, (unsigned)_countof(country), &weight, &valuation) != EOF)
	{
		insertIntoHashTable(hashTable, country, weight, valuation);
	}

	fclose(file);   // close the file after reading all data
}

// 
// FUNCTION: inOrderTraversal
// DESCRIPTION:
//		This function performs an in-order traversal of the BST and prints
//		the details of each parcel.
// PARAMETERS:
//		Parcel* root: a pointer to the root of the BST to be traversed.
// RETURNS:
//		void: this function does not return a value.
//
void inOrderTraversal(Parcel* root)
{
	if (root != NULL)
	{
		inOrderTraversal(root->left);   // travesrse the left subtree
		printf("Destoination: %s, Weight: %d, Valuation: %2.f\n", root->destination, root->weight, root->valuation);   // print the parcel details
		inOrderTraversal(root->right);   // traverse the right subtree
	}
}

//
// FUNCTION: isValidCountry
// DESCRIPTION: 
//		This function checks if the provided country is in the list of valid countries.
// PARAMETERS: 
//		const char* country: the name of the country for checking.
//		const char* validCountries[]: the list of valid country names.
//		size_t numCountries: the number of valid countries.
// RETURNS:
//		int: return 1 if the country is valid else return 0.
//
int isValidCountry(const char* country, const char* validCountries[], size_t numCountries)
{
	for (size_t i = 0; i < numCountries; i++)
	{
		if (strcmp(validCountries[i], country) == 0)
		{
			return 1;   // country got found in the list
		}
	}
	return 0;   // country got not found in the list
}

//
// FUNCTION: getValidWeight
// DESCRIPTION:
//		This function gets a valid weight input from user, verifying it is a number.
// PARAMETERS:
//		void: this function is not taking any parameters.
// RETURNS:
//		int: returns the valid weight entered by the user.
//
int getValidWeight()
{
	int weight;
	int result;

	while (1)
	{
		printf("Enter weight: ");
		result = scanf_s("%d", &weight);

		// clear the input buffer
		while (getchar() != '\n');

		if (result == 1 && weight > 0)
		{
			return weight;   // returns the valid weight
		}
		else
		{
			printf("Invalid input, please enter a valid weight.\n");
		}
	}
}

// 
// FUNCTION: displayParcelsByCountry
// DESCRIPTION:
//		This function displays all parcels for given country by performing an in-order traversal of the BST.
// PARAMETERS:
//		Parcel* hashTable[]: the hash table which is cointaining the parcels.
//		char* country: the name of the country whose parcels will get displayed.
//		const char* validCountries[]: the list of valid country names.
//		size_t numCountries: the number of valid countries.
// RETURNS:
//		void: this function does not return a value.
//
void displayParcelsByCountry(Parcel* hashTable[], char* country, const char* validCountries[], size_t numCountries)
{
	if (!isValidCountry(country, validCountries, numCountries))
	{
		printf("Error: Given country name is not in the list, please enter a valid country name.\n");
		return;
	}

	unsigned long index = hash(country);   // generate the hash value for the country
	if (hashTable[index] != NULL)
	{
		printf("Parcels for %s:\n", country);   // print country name
		inOrderTraversal(hashTable[index]);   // perform in-order traversal of BST to print all parcels
	}
	else
	{
		printf("No parcels found for %s.\n", country);   // print a message if no parcels are found
	}
}

//
// FUNCTION: findAndDisplayParcelsByWeight
// DESCRIPTION: 
//		This is the helper function which traverse the BST and
//		display parcels based on the weight condition (higher or lower).
// PARAMETERS:
//		Parcel* root: a pointer to the root of the BST to be traversed.
//		int weight: the weight condition to check.
//		int higher: flag indicating whether to check for weights higher (1) 
//		or lower (2) that the provided weight.
// RETURNS:
//		int: returns 1 if matching parcel got found else o.
//
int findAndDisplayParcelsByWeight(Parcel* root, int weight, int higher)
{
	int found = 0;

	if (root == NULL)
	{
		return found;
	}

	// traverse the left subtree
	found |= findAndDisplayParcelsByWeight(root->left, weight, higher);

	// chech if the current node meets the weight condition
	if ((higher && root->weight > weight) || (!higher && root->weight < weight))
	{
		printf("Destination: %s, Weight: %d, Valuation: %.2f\n", root->destination, root->weight, root->valuation);
		found = 1;
	}

	// traverse the right subtree
	found |= findAndDisplayParcelsByWeight(root->right, weight, higher);

	return found;
}

//
// FUNCTION: displayParcelsByCountryAndWeight
// DESCRIPTION:
//		This function displays parcels for a given country based on weight.(higher or lower than provided weight).
// PARAMETERS:
//		Parcel* hashTable[]: the hash table which is cointaining the parcels.
//		char* country: the name of the country whose parcels will get displayed.
//		int weight: the weight condition to check.
//		int higher: flag indicating whether to check for weights higher (1) 
//		or lower (2) that the provided weight.
//		const char* validCountries[]: the list of valid country names.
//		size_t numCountries: the number of valid countries.
// RETURNS:
//		void: This function does not return a value.
//
void displayPrcelsByCountryAndWeight(Parcel* hashTable[], char* country, int weight, int higher, const char* validCountries[], size_t numCountries)
{
	if (!isValidCountry(country, validCountries, numCountries))
	{
		printf("Erros: Given country name is not in the list.\n");
		return;
	}

	unsigned long index = hash(country); //generate the hash value for the country
	Parcel* root = hashTable[index];

	// Traverse the BST to find and display parcels based on the weight condition
	int found = findAndDisplayParcelsByWeight(root, weight, higher);

	if (!found)
	{
		printf("No parcel is found for specific weight conditon.\n");
	}
}

//
// FUNCTION: calculateTotalLoadAndValuation
// DESCRIPTION:
//		This is helper function to calculates the total weight and valuation of parcels in the BST.
// PARAMETERS:
//		Parcel*root: a pointer to the root of the BST to be traversed.
//		int* totalWeight: a pointer to the variable where total weight will get stored.
//		float* totalValuation: a pointer to the variable where total valuation will get stored.
// RETURNS:
//		void: This function does not return a value.
//
void calculateTotalLoadAndValuation(Parcel* root, int* totalweight, float* totalvaluation)
{
	if (root == NULL)
	{
		return;
	}

	// traverse the left subtree
	calculateTotalLoadAndValuation(root->left, totalweight, totalvaluation);

	// add weight and valuation of the current node to the total
	*totalweight += root->weight;
	*totalvaluation += root->valuation;

	// traverse the right subtree
	calculateTotalLoadAndValuation(root->right, totalweight, totalvaluation);
}

//
// FUNCTION: displayTotalLoadAndValuation
// DESCRIPTION: 
//		This function calculates and displays the total weight and valuation of all parcels for a given country.
// PARAMETERS:
//		Parcel* hashTable[]: the hash table which is cointaining the parcels.
//		char* country: the name of the country whose total load and valuation of parcels will get displayed.
//		const char* validCountries[]: the list of valid country names.
//		size_t numCountries: the number of valid countries.
// RETURNS:
//
void displayTotalLoadAndValuation(Parcel* hashTable[], char* country, const char* validCountries[], size_t numCountries)
{
	if (!isValidCountry(country, validCountries, numCountries))
	{
		printf("Error: Given country name is not in the list, please enter a valid country name.\n");
		return;
	}

	unsigned long index = hash(country);  // generate the hash value for the country
	Parcel* root = hashTable[index];
	int totalWeight = 0;
	float totalValuation = 0.0;

	// traverse the BST to do sum the weight and valuations
	calculateTotalLoadAndValuation(root, &totalWeight, &totalValuation);

	// print the total weight and valuation for the country
	if (totalWeight > 0 || totalValuation > 0.0)
	{
		printf("Total load for %s: %d grams\n", country, totalWeight);
		printf("Total valuation for %s: $%.2f\n", country, totalValuation);
	}
	else
	{
		printf("No parcels found for %s.\n", country);   // print a message if no parcels are found
	}
}

//
// FUNCTION: findCheapestAndMostExpensive
// DESCRIPTION: 
//		This function finds cheapest and most expensive parcel in the BST.
// PARAMETERS:
//		Parcel* root: pointer to the root of BST to be traversed.
//		Parcel** cheapest: double pointer to the variable where cheapest parcel will get stored.
//		Parcel** mostExpensive: double pointer to variable where most expensive parcel will get stored.
// RETURNS:
//		void: this function does not return a value.
//
void findCheapestAndMostExpensive(Parcel* root, Parcel** cheapest, Parcel** mostExpensive)
{
	if (root == NULL)
	{
		return;
	}

	// traverse left subtree
	findCheapestAndMostExpensive(root->left, cheapest, mostExpensive);

	// check if the current node is cheapest
	if (*cheapest == NULL || root->valuation < (*cheapest)->valuation)
	{
		*cheapest = root;
	}

	// check if the current node is most expensive
	if (*mostExpensive == NULL || root->valuation > (*mostExpensive)->valuation)
	{
		*mostExpensive = root;
	}

	// traverse right subtree
	findCheapestAndMostExpensive(root->right, cheapest, mostExpensive);
}

//
// FUNCTION: displayCheapestAndMostExpensive
// DESCRIPTION:
//		This function finds and displays the cheapest and most expensive parcels for a given country.
// PARAMETERS:
//		Parcel* hashTable[]: the hash table containing the parcels.
//		char* country: the name of the country whose cheapest and most expensive parcels are to be displayed.
//		const char* validCountries[]: the list of valid country names.
//		size_t numCountries: the number of valid countries.
// RETURNS :
//		void: This function does not return a value.
//
void displayCheapestAndMostExpensive(Parcel* hashTable[], char* country, const char* validCountries[], size_t numCountries) 
{
	if (!isValidCountry(country, validCountries, numCountries)) 
	{
		printf("Error: Given country name is not in the list, please enter a valid country name.\n");
		return;
	}

	unsigned long index = hash(country);  // generate the hash value for the country
	Parcel* root = hashTable[index];
	Parcel* cheapest = NULL;
	Parcel* mostExpensive = NULL;

	// Traverse the BST to find the cheapest and most expensive parcels
	findCheapestAndMostExpensive(root, &cheapest, &mostExpensive);

	// Print the details of the cheapest and most expensive parcels
	if (cheapest && mostExpensive) 
	{
		printf("Cheapest parcel for %s: Weight: %d, Valuation: %.2f\n", country, cheapest->weight, cheapest->valuation);
		printf("Most expensive parcel for %s: Weight: %d, Valuation: %.2f\n", country, mostExpensive->weight, mostExpensive->valuation);
	}
	else 
	{
		printf("No parcels found for %s.\n", country);  // print a message if no parcels are found
	}
}

//
// FUNCTION: findLighestAndHeaviest
// DESCRIPTION: 
//		This function find the lightest and heaviest parcel in BST.
// PARAMETERS:
//		Parcel* root: pointer to the root of BST to be traversed.
//		Parcel** lightest: double pointer to variable where lightest parcel will get stored.
//		Parcel** heaviest: double pointer to variable where heaviest parcel will get stored.
// RETURN:
//		void: this function does not return a value.
//
void findLightestAndHeaviest(Parcel* root, Parcel** lightest, Parcel** heaviest)
{
	if (root == NULL)
	{
		return;
	}

	// traverse left subtree
	findLightestAndHeaviest(root->left, lightest, heaviest);

	// check if current node is the lightest
	if (*lightest == NULL || root->weight < (*lightest)->weight)
	{
		*lightest = root;
	}

	// check if current node is the heaviest
	if (*heaviest == NULL || root->weight > (*heaviest)->weight)
	{
		*heaviest = root;
	}

	// traverse right subtree
	findLightestAndHeaviest(root->right, lightest, heaviest);
}

//
// FUNCTION: displayLightestAndHeaviest
// DESCRIPTION:
//		This function finds and displays the lightest and heaviest parcels for a given country.
// PARAMETERS:
//		Parcel* hashTable[]: the hash table containing the parcels.
//		char* country: the name of the country whose lightest and heaviest parcels are to be displayed.
//		const char* validCountries[]: the list of valid country names.
//		size_t numCountries: the number of valid countries.
// RETURNS:
//		void: This function does not return a value.
//
void displayLightestAndHeaviest(Parcel* hashTable[], char* country, const char* validCountries[], size_t numCountries)
	{
	 if (!isValidCountry(country, validCountries, numCountries)) 
	 {
		printf("Error: Given country name is not in the list, please enter a valid country name.\n");
		return;
	 }

	unsigned long index = hash(country);  // generate the hash value for the country
	Parcel* root = hashTable[index];
	Parcel* lightest = NULL;
	Parcel* heaviest = NULL;

	// Traverse the BST to find the lightest and heaviest parcels
	findLightestAndHeaviest(root, &lightest, &heaviest);

	// Print the details of the lightest and heaviest parcels
	if (lightest && heaviest) 
	{
		printf("Lightest parcel for %s: Weight: %d, Valuation: %.2f\n", country, lightest->weight, lightest->valuation);
		printf("Heaviest parcel for %s: Weight: %d, Valuation: %.2f\n", country, heaviest->weight, heaviest->valuation);
	}
	else 
	{
		printf("No parcels found for %s.\n", country);  // print a message if no parcels are found
	} 
}

//
// FUNCTION: cleanupBst
// DESCRIPTION: 
//		This function recursively free memory allocated for BST.
// PARAMETERS:
//		Parcel*root: pointer to root of BST to cleaned up.
// RETURNS:
//		void: This function does not return a value.
//
void cleanupBst(Parcel* root)
{
	if (root != NULL)
	{
		cleanupBst(root->left);   // recursively free left subtree
		cleanupBst(root->right);   // recursively free right subtree
		free(root->destination);   // free dynamically allocated memory 
		free(root);   // free Parcel structure
	}
}

//
// FUNCTION: cleanupMemory
// DESCRIPTION:
//		This function cleans up all memory allocated for the hash table.
// PARAMETERS:
//		Parcel* hashTable[]: the hash table to be cleaned up.
// RETURNS:
//		void: This function does not return a value.
//
void cleanupMemory(Parcel* hashTable[]) 
{
	for (int i = 0; i < HASH_TABLE_SIZE; i++) 
	{
		cleanupBst(hashTable[i]);  // clean up each BST in the hash table
	}
}

//
// FUNCTION: displayMenu
// DESCRIPTION:
//		This function display the main menu.
// PARAMETERS:
//		void: this function deos not take any parameters.
// RETURNS:
//		void: this function does not return a value.
//
void displayMenu()
{
	printf("\n1. Enter country name and display all the parcels details\n");
	printf("2. ENter country and weight pair\n");
	printf("3. Display the total parcel load and valuation for the country\n");
	printf("4. Enter the country name and display cheapest and most expensive parcel's details\n");
	printf("5. Enter the country name and display lightest and heaviest parcel for the country\n");
	printf("6. Exit the application\n");
}

//
// FUNCTION: handleMenuOption
// DESCRIPTION:
//		This function handle user menu selection. 
// PARAMETERS:
//		Parcel* hashTable[]: the hash table containing the parcels.
//		int option: the menu option selected by user.
//		const char* validCountries[]: list of valid country name.
//		size_t numCountries: number of valid countries.
// RETURNS:
//		void: this function does not return a value.
//
void handleMenuOption(Parcel* hashTable[], int option, const char* validCountries[], size_t numCountries)
{
	char country[21];
	int weight;
	int higher;
	int result;

	// process the selection of user based on menu option
	switch (option)
	{
	case 1:
		printf("Enter country name: ");
		scanf_s("%20s", country, (unsigned)_countof(country));   // read the country name from user
		displayParcelsByCountry(hashTable, country, validCountries, numCountries);  // display all parcel for country
		break;
	case 2:
		printf("Enter country name: ");
		scanf_s("%20s", country, (unsigned)_countof(country));  // read the country name from user
		if (!isValidCountry(country, validCountries, numCountries))
		{
			printf("Error: Given country name is not in the list, please enter a valid country name.\n");
			break;   // exit case if country is not valid
		}
		weight = getValidWeight();   // get valid weight from user

		// loop to ensure user select a valid option for higher or lower weight
		while (1)
		{
			printf("Select an option:\n1. Higher 2. Lower: ");
			result = scanf_s("%d", &higher);

			// clear input buffer if non-integer input entered
			while (getchar() != '\n');

			if (result == 1 && (higher == 1 || higher == 2))
			{
				break;   // exit loop if valid option is selected
			}
			else
			{
				printf("Invalid oprion. Please try again.\n");   // print error message if option is invalid
			}
		}

		displayPrcelsByCountryAndWeight(hashTable, country, weight, higher == 1, validCountries, numCountries);   // display parcel based on weight condition
		break;
	case 3:
		printf("Enter country name: ");
		scanf_s("%20s", country, (unsigned)_countof(country));   // read the country name from user
		displayTotalLoadAndValuation(hashTable, country, validCountries, numCountries);   // display total load and valuation of country
		break;
	case 4:
		printf("Enter country name: ");
		scanf_s("%20s", country, (unsigned)_countof(country));   // read the country name from user
		displayCheapestAndMostExpensive(hashTable, country, validCountries, numCountries);   // display cheapest and expensive parcel of country
		break;
	case 5:
		printf("Enter country name: ");
		scanf_s("%20s", country, (unsigned)_countof(country));   // read the country name from user
		displayLightestAndHeaviest(hashTable, country, validCountries, numCountries);   // display lightest and heaviest parcel of country
		break;
	case 6:
		cleanupMemory(hashTable);   // clean up all allocated memory
		exit(0);   // exti application
	default:
		printf("Invalid option. Please try again.\n");
	}
}

//
// FUNCTION: main
// DESCRIPTION:
//		This is main function that run application, display menu and handle user input.
// PARAMETERS:
//		void: this function is not taking any parameters.
// RETURNS:
//		void: returns o upon successful completion.
//
int main()
{
	Parcel* hashTable[HASH_TABLE_SIZE] = { NULL };   // initialize hash table with NULL pointer

	const char* validCountries[] = 
	{
		"Azerbaijan", "Italy", "Ukraine", "Germany", "Australia", "Vanuatu", "Bulgaria", "Mongolia", "Armenia",
		"Yemen", "Bahamas", "Trinidad & Tobago", "Spain", "Russian Federation", "Djobouti", "Ethiopia",
		"Eswatini", "Slovakia", "Aruba", "Nepal", "India", "Gambia", "Israel", "Kosovo", "Burkina", "Barbados",
		"Algeria", "Afghanistan", "Albania", "San Marino", "Costa Rica", "Marshall Islands", "Iceland",
		"Kuwait", "Japan", "Canada", "Oman", "Liberia", "Vietnam", "Taiwan", "Bhutan", "Indonesia",
		"Antigua & Deps", "Colombia", "France", "Brazil", "Congo", "Guatemala", "Guinea-Bissau", "Austria",
		"Slovenia", "Comoros", "Serbia", "China", "Belarus", "Dominica", "Madagascar", "Chile", "Denmark",
		"Tajikistan", "Angola", "Nicaragua", "Georgia", "Argentina", "Bangladesh", "Moldova"
	};
	size_t numCountries = sizeof(validCountries) / sizeof(validCountries[0]);

	loadData(hashTable, "couriers.txt", validCountries, numCountries);   // load data from file to hash table

	int option;
	int result;

	do
	{
		displayMenu();   // display menu
		printf("Enter your choice: ");
		result = scanf_s("%d", &option);   // read menu selection from user

		// clear input buffer if non-integer input entered
		while (getchar() != '\n');

		if (result == 1 && option >= 1 && option <= 6)
		{
			handleMenuOption(hashTable, option, validCountries, numCountries);   // handle menu selection
		}
		else
		{
			printf("Invalid option. Please try again.\n");   // print error message if option is invalid
		}
	} while (option != 6);   // repeat until user select option of exit

	cleanupMemory(hashTable);   // clean up memory before exiting

	return 0;
}