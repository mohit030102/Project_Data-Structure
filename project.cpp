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
	Parcel* newParcel = (Parcel*)malloc(sizeof(Parcel)); // aalocate memory for new parcel
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
	insertIntoHashTable(&hashTable[index], newParcel);   // insert the parcel into the appropriate BST within the hash table
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
		insertIntoHashTable(country, weight, valuation);
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
	if (isValidCountry(country, validCountries, numCountries))
	{
		printf("Error: Given country name is not in the list, please enter a valid country name.\n");
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




struct AVLTree
{
	Parcel* root;

};

struct HashTable
{
	AVLTree* table[127];
};

unsigned long djb2(const char* str)
{
	unsigned long hash = 5381;
	int c;
	while ((c=*str++))
		hash = ((hash << 5) + hash) + c; //hash * 33 + c return hash % 127
}

int max(int a, int b)
{
	return (a > b) ? a : b;
}

//
// FUNCTION : displayCheapestAndMostExpensive
// DESCRIPTION :
// This function finds and displays the cheapest and most expensive parcels for a given country.
// PARAMETERS :
// Parcel* hashTable[] : the hash table containing the parcels.
// char* country : the name of the country whose cheapest and most expensive parcels are to be displayed.
// const char* validCountries[] : the list of valid country names.
// size_t numCountries : the number of valid countries.
// RETURNS :
// void : This function does not return a value.
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
	if (cheapest && mostExpensive) {
		printf("Cheapest parcel for %s: Weight: %d, Valuation: %.2f\n", country, cheapest->weight, cheapest->valuation);
		printf("Most expensive parcel for %s: Weight: %d, Valuation: %.2f\n", country, mostExpensive->weight, mostExpensive->valuation);
	}
	else {
		printf("No parcels found for %s.\n", country);  // print a message if no parcels are found
	}
}


//
// FUNCTION : displayLightestAndHeaviest
// DESCRIPTION :
// This function finds and displays the lightest and heaviest parcels for a given country.
// PARAMETERS :
// Parcel* hashTable[] : the hash table containing the parcels.
// char* country : the name of the country whose lightest and heaviest parcels are to be displayed.
// const char* validCountries[] : the list of valid country names.
// size_t numCountries : the number of valid countries.
// RETURNS :
// void : This function does not return a value.
//

void displayLightestAndHeaviest(Parcel* hashTable[], char* country, const char* validCountries[], size_t numCountries
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
		printf("Heaviest parcel for %s: Weight: %d, Valuation: %.2f\n", country, heaviest->weight, heaviest-	>valuation);
	}
	else 
	{
		printf("No parcels found for %s.\n", country);  // print a message if no parcels are found
	} 
}

//
// FUNCTION : cleanupMemory
// DESCRIPTION :
// This function cleans up all memory allocated for the hash table.
// PARAMETERS :
// Parcel* hashTable[] : the hash table to be cleaned up.
// RETURNS :
// void : This function does not return a value.
//
void cleanupMemory(Parcel* hashTable[]) 
{
	for (int i = 0; i < HASH_TABLE_SIZE; i++) 
	{
		cleanupBst(hashTable[i]);  // clean up each BST in the hash table
	}
}



