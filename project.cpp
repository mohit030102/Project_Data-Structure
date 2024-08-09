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

// Array of pointers to Parcel structure, representing the hash table
Parcel* hashTable[HASH_TABLE_SIZE];

// djb2 hash function to generate a hash value from a string
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

// function to create a new parcel structure with the given country, weight and valuation
Parcel* createParcel(char* country, int weight, float valuation)
{
	Parcel* newParcel = (Parcel*)malloc(sizeof(Parcel)); // aalocate memory for new parcel
	newParcel->destination = (char*)malloc(strlen(country) + 1); // allocate memory for the country name
	strcpy_s(newParcel->destination, strlen(country) + 1, country); // copy the country name to the destination
	newParcel->weight = weight; // set the weight of the parcel
	newParcel->valuation = valuation; // set the valuation of parcel
	newParcel->left = newParcel->right = NULL; // initialize left and right child childeren to NULL
	return newParcel;
}

// function to insert a new parel into the BST based on weight
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

// function to load data from a file into the hash table
void loadData(const char* filename)
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

// function to perform an in-order traversal of BST and orint the details of each parcel
void inOrderTraversal(Parcel* root)
{
	if (root != NULL)
	{
		inOrderTraversal(root->left);   // travesrse the left subtree
		printf("Destoination: %s, Weight: %d, Valuation: %2.f\n", root->destination, root->weight, root->valuation);   // print the parcel details
		inOrderTraversal(root->right);   // traverse the right subtree
	}
}

// function to display all parcels for a given country
void displayParcelsByCountry(char* country)
{
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

// Function to display parcels for a given country based on weight (higher or lower than the given weight)
void displayPrcelsCountryandWeight(char* country, int weight, int higher)
{
	unsigned long index = hash(country); //generate the hash value for the country
	Parcel* root = hashTable[index];
	int found = 0;

	//Traverse the BST to find and display parcels based on the base condition
	while (root != NULL)
	{
		if ((higher && root->weight > weight) || (!higher && root->weight > weight))
		{
			printf("Destination:%s, weight: %d, Valuation: %2f\n, root-> destination, root-> weight, root->valuation");
			found = 1;
		}
		if (root->left!= NULL && root->weight>weight)
		{
			root = root->left; //will move to the left sub tree if the weight is less
		}
		else
		{
			root = root->right; //will move to the right sub tree if the weight is more or equal
		}
	}

	if (!found)
	{
		printf("No parcels found for %s with the specific weight condition\n, country"); //print a message if no parcels match the condition
	}
}

// Function to calculate and display the total weight and valuation of all parcels for a given country
void displayTotalLoadAndValuation(char* country) 
{
	unsigned long index = hash(country);  // generate the hash value for the country
	Parcel* root = hashTable[index];
	int totalWeight = 0;
	float totalValuation = 0.0;


// Traverse the BST to sum the weights and valuations
	while (root != NULL)
	{
		totalWeight += root->weight; //add weight to the total
		totalValuation = +root->valuation; //add value to the total 
		if (root->left != NULL && root->weight > root->left->weight)
		{
			root = root->left;// move to the left subtree
		}
		else
		{
			root = root->right; //move to the right subtree
		}


	}




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