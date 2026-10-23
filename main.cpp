#include <iostream>
#include <limits>
#include <stdexcept>
#include <string>
#include "jurr.h"

using namespace std;

int getNumber(const string& prompt, int minValue, int maxValue)
{
	int number;
	while (true) {
		cout << prompt;
		if (cin >> number && number >= minValue && number <= maxValue) {
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			return number;
		}
		if (cin.eof()) throw runtime_error("Input stream closed!");
		cout << "Please enter a whole number from " << minValue << " to " << maxValue << ".\n";
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
	}
}

string getText(const string& prompt)
{
	string text;
	while (true) {
		cout << prompt;
		if (!getline(cin, text)) throw runtime_error("Input stream closed!");
		if (text.find_first_not_of(" \t") != string::npos) return text;
		cout << "This field cannot be empty.\n";
	}
}

int getMenuChoice()
{
	cout << "\nJURASSIC LIST\n"
	     << "------------------------------\n"
	     << " 1. Display all dinosaurs\n"
	     << " 2. Totals by island\n"
	     << " 3. Search for a species\n"
	     << " 4. Filter by minimum count\n"
	     << " 5. Sort an island\n"
	     << " 6. Add dinosaurs\n"
	     << " 7. Edit a species count\n"
	     << " 8. Remove a species\n"
	     << " 9. List unique species\n"
	     << "10. Exit\n";
	return getNumber("Choice: ", 1, 10);
}

int main()
{
	JurassicList list;
	const int MAX_COUNT = 100000;

	try {
		int choice = 0;
		while (choice != 10) {
			choice = getMenuChoice();
			switch (choice) {
			case 1: list.displayAll(); break;
			case 2: list.displayTotals(); break;
			case 3: list.searchSpecies(getText("Species to search for: ")); break;
			case 4: list.filterByMinCount(getNumber("Minimum count: ", 1, MAX_COUNT)); break;
			case 5: {
				string island = getText("Island to sort: ");
				if (!list.hasIsland(island)) { cout << "That island is not on record.\n"; break; }
				int how = getNumber("Sort by (1) species name or (2) count: ", 1, 2);
				list.sortIsland(island, how == 2);
				cout << "Island sorted.\n";
				break;
			}
			case 6: {
				string island = getText("Island name: ");
				string species = getText("Species name: ");
				int count = getNumber("Number of dinosaurs to add: ", 1, MAX_COUNT);
				list.addDino(island, species, count);
				cout << "Added successfully!\n";
				break;
			}
			case 7: {
				string island = getText("Island name: ");
				string species = getText("Species name: ");
				int count = getNumber("New count: ", 1, MAX_COUNT);
				cout << (list.editDino(island, species, count)
					? "Updated successfully!\n" : "Island or species not found.\n");
				break;
			}
			case 8: {
				string island = getText("Island name: ");
				string species = getText("Species to remove: ");
				cout << (list.removeDino(island, species)
					? "Removed successfully!\n" : "Island or species not found.\n");
				break;
			}
			case 9: list.displayUniqueSpecies(); break;
			case 10: cout << "Goodbye, and welcome to Jurassic World!\n"; break;
			}
		}
	} catch (const runtime_error& e) {
		cout << "\nError: " << e.what() << endl;
	}
	return 0;
}
