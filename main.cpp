#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <ctime>
#include <cstdlib>

using namespace std;

struct Score {
    string name;
    int attempts;
    string difficulty;
};

// Funkcja do czyszczenia konsoli
void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void drawHeader(string title) {
    cout << "========================================" << endl;
    cout << "      " << title << endl;
    cout << "========================================" << endl;
}

// Obsługa plików
void saveScore(Score s) {
    ofstream file("scores.txt", ios::app);
    if (file.is_open()) {
        file << s.name << " " << s.attempts << " " << s.difficulty << endl;
        file.close();
    }
}

vector<Score> loadScores() {
    vector<Score> scores;
    ifstream file("scores.txt");
    string n, d;
    int a;
    while (file >> n >> a >> d) {
        scores.push_back({n, a, d});
    }
    file.close();
    return scores;
}

void showScores() {
    clearScreen();
    drawHeader("RANKING TOP 5");
    vector<Score> allScores = loadScores();
    
    string diffs[] = {"Latwy", "Sredni", "Trudny"};
    
    for (const string& d : diffs) {
        cout << "\n--- Poziom: " << d << " ---" << endl;
        vector<Score> filtered;
        for (const auto& s : allScores) {
            if (s.difficulty == d) filtered.push_back(s);
        }
        
        sort(filtered.begin(), filtered.end(), [](const Score& a, const Score& b) {
            return a.attempts < b.attempts;
        });

        int limit = min((int)filtered.size(), 5);
        if (limit == 0) cout << "Brak wynikow." << endl;
        for (int i = 0; i < limit; i++) {
            cout << i + 1 << ". " << filtered[i].name << " - Proby: " << filtered[i].attempts << endl;
        }
    }
    
    cout << "\nNacisnij Enter, aby wrocic...";
    cin.ignore();
    cin.get();
}

string getRandomMessage(bool tooLow) {
    string lowMsgs[] = {"Za malo! Celuj wyzej.", "Pudlo! Ta liczba jest wieksza.", "Niestety, musisz dodac wiecej."};
    string highMsgs[] = {"Przesadziles! Za duzo.", "Oj, hamuj! Liczba jest mniejsza.", "Za wysoko! Sprobuj odjac nieco."};
    return tooLow ? lowMsgs[rand() % 3] : highMsgs[rand() % 3];
}

void playGame() {
    clearScreen();
    drawHeader("NOWA GRA");
    cout << "Wybierz poziom: 1. Latwy (1-50) | 2. Sredni (1-100) | 3. Trudny (1-250)" << endl;
    int choice;
    cin >> choice;

    int maxRange = (choice == 1) ? 50 : (choice == 2) ? 100 : 250;
    string diffStr = (choice == 1) ? "Latwy" : (choice == 2) ? "Sredni" : "Trudny";
    
    int betLimit = 0;
    cout << "Czy chcesz postawic zaklad o max liczbe prob? (y/n): ";
    char betChoice;
    cin >> betChoice;
    if (betChoice == 'y') {
        cout << "Podaj maksymalna liczbe prob: ";
        cin >> betLimit;
    }

    int target = rand() % maxRange + 1;
    int attempt = 0;
    int guess = 0;
    bool won = false;

    while (true) {
        attempt++;
        clearScreen();
        drawHeader("ZGADNIJ LICZBE");
        cout << "Poziom: " << diffStr << " (1-" << maxRange << ")" << endl;
        cout << "Proba numer: " << attempt << (betLimit > 0 ? " / " + to_string(betLimit) : "") << endl;
        cout << "Podaj liczbe: ";
        cin >> guess;

        if (guess == target) {
            cout << "\nGRATULACJE! Trafiles w " << attempt << " probie!" << endl;
            won = true;
            break;
        } else if (betLimit > 0 && attempt >= betLimit) {
            cout << "\nPRZEGRALES! Skonczyly Ci sie proby. Liczba to: " << target << endl;
            break;
        } else {
            cout << getRandomMessage(guess < target) << endl;
            cout << "Nacisnij Enter, aby kontynuowac...";
            cin.ignore();
            cin.get();
        }
    }

    if (won) {
        cout << "Podaj swoje imie (bez spacji): ";
        string name;
        cin >> name;
        saveScore({name, attempt, diffStr});
    } else {
        cout << "Nacisnij Enter, aby wrocic do menu...";
        cin.ignore();
        cin.get();
    }
}

int main() {
    srand(time(0));
    while (true) {
        clearScreen();
        drawHeader("MENU GLOWNE");
        cout << "1. Rozpocznij nową grę" << endl;
        
        vector<Score> scores = loadScores();
        if (!scores.empty()) {
            cout << "2. TOP 5 Wynikow" << endl;
        }
        cout << "3. Wyjdz" << endl;
        
        int menuChoice;
        cin >> menuChoice;

        if (menuChoice == 1) playGame();
        else if (menuChoice == 2 && !scores.empty()) showScores();
        else if (menuChoice == 3) break;
    }
    return 0;
}