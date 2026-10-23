#pragma once
#ifndef JURR_LIST_H
#define JURR_LIST_H

#include <iomanip>
#include <iostream>
#include <map>
#include <string>
#include <vector>

struct DinoEntry {
	std::string species;
	int count;
};

template <typename K, typename V>
void displayMap(const std::map<K, V>& data, const std::string& title)
{
	std::cout << "\n" << title << "\n" << std::string(40, '-') << "\n";
	for (const auto& entry : data) {
		std::cout << std::left << std::setw(26) << entry.first << ": " << entry.second << "\n";
	}
}

template <typename Container>
void displayList(const Container& items, const std::string& title)
{
	std::cout << "\n" << title << "\n" << std::string(40, '-') << "\n";
	for (const auto& item : items) {
		std::cout << "  * " << item << "\n";
	}
}

class JurassicList {
public:
	JurassicList();

	void displayAll() const;
	void displayTotals() const;
	void displayUniqueSpecies() const;
	bool searchSpecies(const std::string& species) const;
	int  filterByMinCount(int minCount) const;

	void addDino(const std::string& island, const std::string& species, int count);
	bool editDino(const std::string& island, const std::string& species, int newCount);
	bool removeDino(const std::string& island, const std::string& species);
	bool sortIsland(const std::string& island, bool byCount);

	bool hasIsland(const std::string& island) const;
	int  grandTotal() const;

private:
	std::map<std::string, std::vector<DinoEntry>> islands_;

	static std::string normalize(const std::string& text);
	static int islandTotal(const std::vector<DinoEntry>& dinos);
};

#endif
