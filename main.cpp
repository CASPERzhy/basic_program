#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <ctime>
#include <cstdlib>
#include <limits>

using namespace std;

struct Score {
    string name;
    int attempts;
    string difficulty;
};

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void validateInput() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

void randomChallenge() {
    if (rand() % 5 != 0) return; 

    int type = rand() % 2;
    if (type == 0) {
        cout << "\nIle masz palcow na rece? ";
        string answer;
        cin >> answer;
        if (answer == "5") {
            cout << ">>> Zapomniales chyba co masz robic?" << endl;
            cout << "Nacisnij Enter, aby wrocic do gry...";
            validateInput();
            cin.get();
        }
    } else {
        cout << "\n2+2=? ";
        string answer;
        cin >> answer;
        if (answer == "4") {
            cout << ">>> Masz zgadywac liczbe, czlowiek..." << endl;
            cout << "Nacisnij Enter, aby wrocic do gry...";
            validateInput();
            cin.get();
        }
    }
}

void drawHeader(string title) {
    cout << "========================================" << endl;
    cout << "      " << title << endl;
    cout << "========================================" << endl;
}

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
    validateInput();
    cin.get();
}

string getRandomMessage(bool tooLow) {
    string lowMsgs[] = {"Za malo! Celuj wyzej.", "Pudlo! Ta liczba jest wieksza.", "Niestety, musisz dodac wiecej."};
    string highMsgs[] = {"Przesadziles! Za duzo.", "Oj, hamuj! Liczba jest mniejsza.", "Za wysoko! Sprobuj odjac nieco."};
    return tooLow ? lowMsgs[rand() % 3] : highMsgs[rand() % 3];
}

void playGame() {
    int choice = 0;
    while (true) {
        clearScreen();
        drawHeader("NOWA GRA");
        cout << "1. Latwy (1-50) | 2. Sredni (1-100) | 3. Trudny (1-250)" << endl;
        cout << "Wybor: ";
        if (!(cin >> choice) || choice < 1 || choice > 3) {
            validateInput();
            continue;
        }
        break;
    }

    int maxRange = (choice == 1) ? 50 : (choice == 2) ? 100 : 250;
    string diffStr = (choice == 1) ? "Latwy" : (choice == 2) ? "Sredni" : "Trudny";
    
    int betLimit = 0;
    cout << "Zaklad o max liczbe prob? (y/n): ";
    char betChoice;
    cin >> betChoice;
    if (betChoice == 'y') {
        cout << "Podaj limit: ";
        while (!(cin >> betLimit) || betLimit <= 0) {
            validateInput();
        }
    }

    int target = rand() % maxRange + 1;
    int attempt = 0;
    int guess = 0;
    string hint = "Zacznij zgadywac!";

    while (true) {
        clearScreen();
        drawHeader("ZGADNIJ LICZBE");
        cout << "Podpowiedz: " << hint << endl;
        cout << "Poziom: " << diffStr << " (1-" << maxRange << ")" << endl;
        cout << "Proba: " << (attempt + 1) << (betLimit > 0 ? " / " + to_string(betLimit) : "") << endl;
        cout << "Podaj liczbe: ";
        
        if (!(cin >> guess)) {
            validateInput();
            continue;
        }

        attempt++;

        if (guess == target) {
            cout << "\nGRATULACJE! Trafiles w " << attempt << " probie!" << endl;
            cout << "Podaj swoje imie: ";
            string name;
            cin >> name;
            saveScore({name, attempt, diffStr});
            break;
        } else if (betLimit > 0 && attempt >= betLimit) {
            cout << "\nPRZEGRALES! Liczba to: " << target << endl;
            cout << "Nacisnij Enter...";
            validateInput();
            cin.get();
            break;
        } else {
            hint = getRandomMessage(guess < target);
            randomChallenge();
        }
    }
}

int main() {
    srand(time(0));
    while (true) {
        clearScreen();
        drawHeader("MENU GLOWNE");
        cout << "1. Rozpocznij nowa gre" << endl;
        vector<Score> scores = loadScores();
        if (!scores.empty()) cout << "2. TOP 5 Wynikow" << endl;
        cout << "3. Wyjdz" << endl;
        cout << "\nWybor: ";
        
        int menuChoice;
        if (!(cin >> menuChoice)) {
            validateInput();
            continue;
        }

        if (menuChoice == 1) playGame();
        else if (menuChoice == 2 && !scores.empty()) showScores();
        else if (menuChoice == 3) break;
    }
    return 0;
}
