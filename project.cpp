/*
* File name: project.cpp
* Project: SENG1050 - project
* Programmer: Mohit Chodavdeeya (8963902)
*			  Raj Rana (8960902)
* Date of version created: 2024-08-07
* Description: The below presented program calculate the different statistics about the	items to be delivered 
*			   through courier company using Hash Table and Tree Structure.
*/

#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <limits>
#pragma warning (dissable : 4996) 

using namespace std;

struct Parcel
{
	char* destination;
	int weight;
	float valuation;
	Parcel* left;
	Parcel* right;
	int height;
};

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