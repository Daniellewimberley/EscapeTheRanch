//**********************************************************************************************
//Author.....: Danielle Wimberley
//Assignment.: Final Project
//Description: A horse themed text-based game where you explore the ranch and gain experience to 
//             ultimately escape the ranch and reunite with your herd.
//***********************************************************************************************

#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <vector>
using namespace std;

// Struct for Game Locations
struct Location {
    string name;
    string description;
    bool hasObstacle;
};

// Function Declarations
void displayIntro();
void showMenu();
void explore(Location[], int&, vector<string>&);
bool tryEscape(int skillLevel);
bool tryEscape(string method); // Overloaded
void saveGame(int, vector<string>);
void loadGame(int&, vector<string>&);
void displayAsciiArt();
int getValidatedInput(int min, int max);

// Recursive Retry
void retryEscape(int attemptsLeft) {
    if (attemptsLeft <= 0) {
        cout << "\nYou're too tired to continue. You must wait until tomorrow.\n";
        return;
    }
    cout << "\nTry to escape again? (1 = Yes, 2 = No): ";
    int choice = getValidatedInput(1, 2);
    if (choice == 1) {
        if (tryEscape(rand() % 10)) {
            cout << "\nYou gallop into freedom! 🐎\n";
        } else {
            cout << "\nYou failed to escape.\n";
            retryEscape(attemptsLeft - 1);
        }
    }
}

int main() {
    srand(time(0));
    int skill = 5;
    vector<string> inventory = {"Carrot", "Horseshoe"};
    Location ranch[3] = {
        {"Barn", "Dusty and full of tools.", false},
        {"Pasture", "Wide open but watched.", true},
        {"Forest Edge", "A tempting path to freedom.", true}
    };

    displayAsciiArt();
    displayIntro();

    int choice;
    do {
        showMenu();
        choice = getValidatedInput(1, 5);

        switch (choice) {
            case 1:
                explore(ranch, skill, inventory);
                break;
            case 2:
                retryEscape(3);
                break;
            case 3:
                saveGame(skill, inventory);
                break;
            case 4:
                loadGame(skill, inventory);
                break;
            case 5:
                cout << "Goodbye! 🐴\n";
                break;
        }
    } while (choice != 5);

    return 0;
}

void displayIntro() {
    cout << "Welcome to Ranch Run – The Escape!\n";
    cout << "You're a clever horse planning your great escape.\n";
}

void displayAsciiArt() {
    cout << R"(
      __))
     /__.-\
    |  ||||  
     \_--_/
     ( oo )  << Run, horse, run! >>
     (_--_)
    )" << "\n\n";
}

void showMenu() {
    cout << "\nChoose an action:\n";
    cout << "1. Explore the ranch\n";
    cout << "2. Try to escape\n";
    cout << "3. Save progress\n";
    cout << "4. Load progress\n";
    cout << "5. Quit\n";
}

void explore(Location locs[], int &skill, vector<string>& inventory) {
    for (int i = 0; i < 3; ++i) {
        cout << i + 1 << ". " << locs[i].name << ": " << locs[i].description << "\n";
    }
    cout << "Pick a place to explore (1-3): ";
    int locChoice = getValidatedInput(1, 3);

    if (locs[locChoice - 1].hasObstacle) {
        cout << "You found an obstacle and gained experience.\n";
        skill++;
    } else {
        cout << "Nothing much here. You find an apple.\n";
        inventory.push_back("Apple");
    }
}

bool tryEscape(int skillLevel) {
    return skillLevel > 7;
}

bool tryEscape(string method) {
    return method == "gate";
}

void saveGame(int skill, vector<string> inv) {
    ofstream file("savegame.txt");
    file << skill << "\n";
    for (const auto& item : inv) file << item << "\n";
    file.close();
    cout << "Game saved!\n";
}

void loadGame(int &skill, vector<string>& inv) {
    ifstream file("savegame.txt");
    if (!file) {
        cout << "No save file found.\n";
        return;
    }
    inv.clear();
    file >> skill;
    string item;
    getline(file, item); // consume newline
    while (getline(file, item)) inv.push_back(item);
    file.close();
    cout << "Game loaded!\n";
}

int getValidatedInput(int min, int max) {
    int input;
    while (true) {
        cin >> input;
        if (cin.fail() || input < min || input > max) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Invalid input. Try again: ";
        } else {
            return input;
        }
    }
}
