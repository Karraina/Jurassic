#include "jurr.h"
#include <algorithm>
#include <cctype>
#include <numeric>
#include <set>

using namespace std;

JurassicList::JurassicList()
{
	islands_["Isla Nublar"] = { {"Tyrannosaurus Rex", 1}, {"Velociraptor", 4},
	                            {"Triceratops", 6}, {"Brachiosaurus", 5} };
	islands_["Isla Sorna"]  = { {"Spinosaurus", 2}, {"Pteranodon", 12},
	                            {"Compsognathus", 40}, {"Velociraptor", 9} };
	islands_["Isla Tacano"] = { {"Baryonyx", 3}, {"Stegosaurus", 7} };
}

string JurassicList::normalize(const string& text)
{
	string result;
	bool newWord = true;
	for (unsigned char c : text) {
		if (isspace(c)) {
			if (!result.empty() && result.back() != ' ') result += ' ';
			newWord = true;
		} else {
			result += newWord ? static_cast<char>(toupper(c)) : static_cast<char>(tolower(c));
			newWord = false;
		}
	}
	if (!result.empty() && result.back() == ' ') result.pop_back();
	return result;
}

int JurassicList::islandTotal(const vector<DinoEntry>& dinos)
{
	return accumulate(dinos.begin(), dinos.end(), 0,
		[](int sum, const DinoEntry& d) { return sum + d.count; });
}

bool JurassicList::hasIsland(const string& island) const
{
	return islands_.find(normalize(island)) != islands_.end();
}

int JurassicList::grandTotal() const
{
	int total = 0;
	for (const auto& island : islands_) total += islandTotal(island.second);
	return total;
}

void JurassicList::displayAll() const
{
	cout << "\nJURASSIC WORLD DINOSAUR REGISTRY\n" << string(52, '=') << "\n";
	if (islands_.empty()) {
		cout << "No islands on record.\n";
		return;
	}
	for (const auto& island : islands_) {
		cout << "\n" << island.first << "\n"
		     << left << setw(26) << "  Species" << "Count\n"
		     << "  " << string(32, '-') << "\n";
		for_each(island.second.begin(), island.second.end(), [](const DinoEntry& d) {
			cout << "  " << left << setw(24) << d.species << d.count << "\n";
		});
		cout << "  " << string(32, '-') << "\n"
		     << "  " << left << setw(24) << "Island Total" << islandTotal(island.second) << "\n";
	}
	cout << "\n" << string(52, '=') << "\nTOTAL DINOSAURS ON ALL ISLANDS: " << grandTotal() << "\n";
}

void JurassicList::displayTotals() const
{
	map<string, int> totals;
	for_each(islands_.begin(), islands_.end(),
		[&totals](const pair<const string, vector<DinoEntry>>& island) {
			totals[island.first] = islandTotal(island.second);
		});
	displayMap(totals, "DINOSAUR TOTALS BY ISLAND");
	cout << "\nAll islands combined: " << grandTotal() << "\n";
}

void JurassicList::displayUniqueSpecies() const
{
	set<string> unique;
	for (const auto& island : islands_) {
		for_each(island.second.begin(), island.second.end(),
			[&unique](const DinoEntry& d) { unique.insert(d.species); });
	}
	displayList(unique, "UNIQUE SPECIES (" + to_string(unique.size()) + ")");
}

bool JurassicList::searchSpecies(const string& species) const
{
	string target = normalize(species);
	bool found = false;
	cout << "\nSearch results for \"" << target << "\":\n";
	for (const auto& island : islands_) {
		auto it = find_if(island.second.begin(), island.second.end(),
			[&target](const DinoEntry& d) { return d.species == target; });
		if (it != island.second.end()) {
			cout << "  " << left << setw(20) << island.first << it->count << "\n";
			found = true;
		}
	}
	if (!found) cout << "  Species not found on any island.\n";
	return found;
}

int JurassicList::filterByMinCount(int minCount) const
{
	int matches = 0;
	cout << "\nSpecies with at least " << minCount << " dinosaurs:\n";
	for (const auto& island : islands_) {
		matches += count_if(island.second.begin(), island.second.end(),
			[minCount](const DinoEntry& d) { return d.count >= minCount; });
		for (const auto& d : island.second) {
			if (d.count >= minCount)
				cout << "  " << left << setw(20) << island.first
				     << setw(24) << d.species << d.count << "\n";
		}
	}
	if (matches == 0) cout << "  None found.\n";
	return matches;
}

void JurassicList::addDino(const string& island, const string& species, int count)
{
	auto& dinos = islands_[normalize(island)];
	string name = normalize(species);
	auto it = find_if(dinos.begin(), dinos.end(),
		[&name](const DinoEntry& d) { return d.species == name; });
	if (it != dinos.end()) it->count += count;
	else dinos.push_back({name, count});
}

bool JurassicList::editDino(const string& island, const string& species, int newCount)
{
	auto islandIt = islands_.find(normalize(island));
	if (islandIt == islands_.end()) return false;
	string name = normalize(species);
	auto it = find_if(islandIt->second.begin(), islandIt->second.end(),
		[&name](const DinoEntry& d) { return d.species == name; });
	if (it == islandIt->second.end()) return false;
	it->count = newCount;
	return true;
}

bool JurassicList::removeDino(const string& island, const string& species)
{
	auto islandIt = islands_.find(normalize(island));
	if (islandIt == islands_.end()) return false;
	string name = normalize(species);
	auto& dinos = islandIt->second;
	auto newEnd = remove_if(dinos.begin(), dinos.end(),
		[&name](const DinoEntry& d) { return d.species == name; });
	if (newEnd == dinos.end()) return false;
	dinos.erase(newEnd, dinos.end());
	if (dinos.empty()) islands_.erase(islandIt);
	return true;
}

bool JurassicList::sortIsland(const string& island, bool byCount)
{
	auto islandIt = islands_.find(normalize(island));
	if (islandIt == islands_.end()) return false;
	sort(islandIt->second.begin(), islandIt->second.end(),
		[byCount](const DinoEntry& a, const DinoEntry& b) {
			return byCount ? a.count > b.count : a.species < b.species;
		});
	return true;
}
