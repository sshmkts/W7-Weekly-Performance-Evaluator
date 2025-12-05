// Weekly Performance Evaluator - Week 7 (PlayerWeek + Amateur/Semi-Pro/Pro)

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
using namespace std;

// Constants
const double PRO_TRAIN_HRS = 6.0;
const double INT_TRAIN_HRS = 3.0;
const double SLEEP_MIN_OK = 7.0;
const double SLEEP_GOOD_MIN = 7.5;
const double SLEEP_GOOD_MAX = 9.0;
const double W_TRAIN = 10.0;
const double W_SLEEP = 5.0;

const int    INPUT_FLUSH = 10000;
const int    MAX_SESSIONS = 5;     // max number of sessions user can enter
const double SHIFT_MIN = 10.0;

// No magic numbers:
const int    MIN_AGE = 1;
const double MIN_TRAINING_HOURS = 0.0;
const double MIN_SLEEP_HOURS = 0.01;

const int    MENU_MIN_CHOICE = 0;
const int    MENU_MAX_CHOICE = 3;

const double PRO_TECH_MINS = 50.0;
const double PRO_COND_MINS = 30.0;
const double INT_TECH_MINS = 45.0;
const double INT_COND_MINS = 25.0;
const double BEG_TECH_MINS = 35.0;
const double BEG_COND_MINS = 20.0;

const int    REST_DAYS_HIGH = 2;
const int    REST_DAYS_MODERATE = 1;
const int    REST_DAYS_LOW = 0;

// Enum for player performance level
enum PlayerLevel { LEVEL_AMATEUR, LEVEL_SEMI_PRO, LEVEL_PRO };

// Struct representing one player's week summary
struct PlayerWeek
{
    string      name;
    int         age;
    int         sessionCount;
    double      totalTraining;
    double      avgTraining;
    double      sleepHours;
    PlayerLevel level;
    double      readinessScore;
    string      advice;
};

// Function Prototypes
void setConsoleColor();
void showBanner();

int    getValidInt(const string& prompt, int minValue);
double getValidDouble(const string& prompt, double minValue);

void collectPlayerBasics(PlayerWeek& pw);

// Array-related functions:
void fillTrainingSessions(double sessions[], int& sessionCount, int maxSessions);
void computeTrainingStats(const double sessions[], int sessionCount,
    PlayerWeek& pw);

void evaluateLevel(PlayerWeek& pw);

int getMenuChoice();

void handleLevelReport(const PlayerWeek& pw,
    const double trainingSessions[], int sessionCount);

void handleTrainingPlanReport(const PlayerWeek& pw);

void handleRecoveryReport(const PlayerWeek& pw);

string levelToString(PlayerLevel level);

void  printSessionsTable(const double sessions[], int sessionCount);

// -------------------------------
// FUNCTIONS
// -------------------------------

// console color 
void setConsoleColor()
{
    system("color 1E");    // для Windows консолі
}

// Display program banner
void showBanner()
{
    cout << "===================================\n";
    cout << "    Weekly Performance Evaluator\n";
    cout << "===================================\n\n";
}

// Safe integer input validation
int getValidInt(const string& prompt, int minValue)
{
    int value;
    cout << prompt;
    while (!(cin >> value) || value < minValue) {
        cin.clear();
        cin.ignore(INPUT_FLUSH, '\n');
        cout << "Invalid input. " << prompt;
    }
    cin.ignore(INPUT_FLUSH, '\n');
    return value;
}

// Safe double input validation
double getValidDouble(const string& prompt, double minValue)
{
    double value;
    cout << prompt;
    while (!(cin >> value) || value < minValue) {
        cin.clear();
        cin.ignore(INPUT_FLUSH, '\n');
        cout << "Invalid input. " << prompt;
    }
    cin.ignore(INPUT_FLUSH, '\n');
    return value;
}

// Collect player name and age into struct
void collectPlayerBasics(PlayerWeek& pw)
{
    cout << "Enter player's full name: ";
    getline(cin, pw.name);

    pw.age = getValidInt("Enter player's age: ", MIN_AGE);

    cout << fixed << showpoint << setprecision(2);
}

// Ask for number of sessions + fill array with user input
void fillTrainingSessions(double sessions[], int& sessionCount, int maxSessions)
{
    cout << "\nHow many training sessions did you have this week (1-"
        << maxSessions << ")? ";

    while (!(cin >> sessionCount) || sessionCount < 1 || sessionCount > maxSessions) {
        cin.clear();
        cin.ignore(INPUT_FLUSH, '\n');
        cout << "Invalid. Enter a number between 1 and "
            << maxSessions << ": ";
    }
    cin.ignore(INPUT_FLUSH, '\n');

    cout << "\nEnter training hours for each session:\n";

    for (int i = 0; i < sessionCount; ++i) {
        sessions[i] = getValidDouble("  Session " + to_string(i + 1) + ": ",
            MIN_TRAINING_HOURS);
    }

    // Fill remaining unused array elements with zeros
    for (int i = sessionCount; i < maxSessions; ++i) {
        sessions[i] = 0.0;
    }
}

// Calculates total and average hours from array into PlayerWeek
void computeTrainingStats(const double sessions[], int sessionCount,
    PlayerWeek& pw)
{
    pw.totalTraining = 0.0;

    for (int i = 0; i < sessionCount; ++i) {
        pw.totalTraining += sessions[i];
    }

    pw.avgTraining = (sessionCount > 0) ? pw.totalTraining / sessionCount : 0.0;
    pw.sessionCount = sessionCount;
}

// Evaluate player level using average training + sleep quality
void evaluateLevel(PlayerWeek& pw)
{
    pw.readinessScore = (pw.avgTraining * W_TRAIN)
        + ((pw.sleepHours - SLEEP_MIN_OK) * W_SLEEP);

    if (pw.avgTraining >= PRO_TRAIN_HRS &&
        (pw.sleepHours >= SLEEP_GOOD_MIN && pw.sleepHours <= SLEEP_GOOD_MAX))
    {
        pw.level = LEVEL_PRO;
    }
    else if (pw.avgTraining >= INT_TRAIN_HRS && pw.sleepHours >= SLEEP_MIN_OK)
    {
        pw.level = LEVEL_SEMI_PRO;
    }
    else
    {
        pw.level = LEVEL_AMATEUR;
    }

    if (pw.sleepHours < SLEEP_MIN_OK || pw.avgTraining < INT_TRAIN_HRS)
        pw.advice = "Build base: 3 sessions + 8h sleep.";
    else if (pw.avgTraining >= PRO_TRAIN_HRS &&
        !(pw.sleepHours >= SLEEP_GOOD_MIN && pw.sleepHours <= SLEEP_GOOD_MAX))
        pw.advice = "Improve sleep routine.";
    else
        pw.advice = "Good balance.";
}

// Menu system
int getMenuChoice()
{
    int choice;
    cout << "\nMenu:\n";
    cout << "  1) Evaluate Level\n";
    cout << "  2) Training Plan\n";
    cout << "  3) Recovery\n";
    cout << "  0) Quit\n";
    cout << "Enter choice: ";

    while (!(cin >> choice) || choice < MENU_MIN_CHOICE || choice > MENU_MAX_CHOICE) {
        cin.clear();
        cin.ignore(INPUT_FLUSH, '\n');
        cout << "Invalid. Enter " << MENU_MIN_CHOICE << "-"
            << MENU_MAX_CHOICE << ": ";
    }

    cin.ignore(INPUT_FLUSH, '\n');
    return choice;
}

// Level Report + array output to file
void handleLevelReport(const PlayerWeek& pw,
    const double trainingSessions[], int sessionCount)
{
    cout << "\n----- LEVEL REPORT -----\n";
    cout << "Player: " << pw.name << "\n";
    cout << "Age: " << pw.age << "\n";
    cout << "Level: " << levelToString(pw.level) << "\n";
    cout << "Total training this week: " << pw.totalTraining << "\n";
    cout << "Average training per session: " << pw.avgTraining << "\n";
    cout << "Average sleep: " << pw.sleepHours << "\n";
    cout << "Readiness Score: " << pw.readinessScore << "\n";
    cout << "Advice: " << pw.advice << "\n";

    // show sessions in a formatted table on console
    printSessionsTable(trainingSessions, sessionCount);

    ofstream out("report.txt");
    if (out) {
        out << "WEEKLY PERFORMANCE REPORT (Week 7)\n";
        out << "Section: LEVEL\n";
        out << "----------------------------------\n";
        out << left << setw(22) << "Player:" << right << setw(20) << pw.name << "\n";
        out << left << setw(22) << "Age:" << right << setw(20) << pw.age << "\n";
        out << left << setw(22) << "Total training:" << right << setw(20) << pw.totalTraining << "\n";
        out << left << setw(22) << "Avg training:" << right << setw(20) << pw.avgTraining << "\n";
        out << left << setw(22) << "Avg sleep:" << right << setw(20) << pw.sleepHours << "\n";
        out << left << setw(22) << "Level:" << right << setw(20) << levelToString(pw.level) << "\n";
        out << left << setw(22) << "Readiness:" << right << setw(20) << pw.readinessScore << "\n";
        out << left << setw(22) << "Advice:" << right << setw(20) << pw.advice << "\n\n";

        out << "Session details:\n";
        out << left << setw(10) << "Session"
            << setw(15) << "Hours" << "\n";

        for (int i = 0; i < sessionCount; ++i) {
            out << left << setw(10) << (i + 1)
                << setw(15) << trainingSessions[i] << "\n";
        }

        out.close();
    }
}

// Training Plan: single overall plan 
void handleTrainingPlanReport(const PlayerWeek& pw)
{
    string planFocus;
    double techMins;
    double condMins;

    // Choose base plan by level
    switch (pw.level)
    {
    case LEVEL_PRO:
        techMins = PRO_TECH_MINS;
        condMins = PRO_COND_MINS;
        planFocus = "High Tempo";
        break;
    case LEVEL_SEMI_PRO:
        techMins = INT_TECH_MINS;
        condMins = INT_COND_MINS;
        planFocus = "Balanced";
        break;
    default: // LEVEL_AMATEUR
        techMins = BEG_TECH_MINS;
        condMins = BEG_COND_MINS;
        planFocus = "Fundamentals";
        break;
    }

    // Adjust conditioning if sleep is low
    if (pw.sleepHours < SLEEP_MIN_OK && condMins >= SHIFT_MIN) {
        techMins += SHIFT_MIN;
        condMins -= SHIFT_MIN;
    }

    cout << "\n------- TRAINING PLAN -------\n";
    cout << left << setw(20) << "Player:" << right << setw(20) << pw.name << "\n";
    cout << left << setw(20) << "Level:" << right << setw(20) << levelToString(pw.level) << "\n";
    cout << left << setw(20) << "Focus:" << right << setw(20) << planFocus << "\n";
    cout << left << setw(20) << "Technical work:" << right << setw(20) << techMins << " min\n";
    cout << left << setw(20) << "Conditioning:" << right << setw(20) << condMins << " min\n";

    ofstream out("report.txt");
    if (out) {
        out << "WEEKLY PERFORMANCE REPORT (Week 7)\n";
        out << "Section: TRAINING PLAN\n";
        out << "----------------------------------\n";
        out << left << setw(20) << "Player:" << right << setw(20) << pw.name << "\n";
        out << left << setw(20) << "Level:" << right << setw(20) << levelToString(pw.level) << "\n";
        out << left << setw(20) << "Focus:" << right << setw(20) << planFocus << "\n";
        out << left << setw(20) << "Technical work:" << right << setw(20) << techMins << " min\n";
        out << left << setw(20) << "Conditioning:" << right << setw(20) << condMins << " min\n";
        out.close();
    }
}

// Recovery Report 
void handleRecoveryReport(const PlayerWeek& pw)
{
    string fatigue = "Low";
    int    rest = REST_DAYS_LOW;
    string tip;

    if (pw.sleepHours < SLEEP_MIN_OK)
        fatigue = "High";
    else if (pw.totalTraining >= PRO_TRAIN_HRS ||
        pw.totalTraining < INT_TRAIN_HRS)
        fatigue = "Moderate";

    if (fatigue == "High")
        rest = REST_DAYS_HIGH;
    else if (fatigue == "Moderate")
        rest = REST_DAYS_MODERATE;

    if (pw.sleepHours < SLEEP_MIN_OK)
        tip = "Aim for 8h sleep.";
    else if (pw.totalTraining >= PRO_TRAIN_HRS)
        tip = "Foam roll.";
    else
        tip = "Light stretching.";

    cout << "\n---- RECOVERY ----\n";
    cout << left << setw(18) << "Fatigue:" << right << setw(20) << fatigue << "\n";
    cout << left << setw(18) << "Rest Days:" << right << setw(20) << rest << "\n";
    cout << left << setw(18) << "Tip:" << right << setw(20) << tip << "\n";

    ofstream out("report.txt");
    if (out) {
        out << "WEEKLY PERFORMANCE REPORT (Week 7)\n";
        out << "Section: RECOVERY\n";
        out << "----------------------------------\n";
        out << left << setw(18) << "Fatigue:" << right << setw(20) << fatigue << "\n";
        out << left << setw(18) << "Rest Days:" << right << setw(20) << rest << "\n";
        out << left << setw(18) << "Tip:" << right << setw(20) << tip << "\n";
        out.close();
    }
}

// Convert enum to string
string levelToString(PlayerLevel level)
{
    switch (level)
    {
    case LEVEL_PRO:      return "Pro";
    case LEVEL_SEMI_PRO: return "Semi-Pro";
    case LEVEL_AMATEUR:  return "Amateur";
    default:             return "Unknown";
    }
}

// Show a nice table of all sessions on console
void printSessionsTable(const double sessions[], int sessionCount)
{
    cout << "\nSession Breakdown:\n";
    cout << left << setw(10) << "#"
        << setw(12) << "Hours" << "\n";

    for (int i = 0; i < sessionCount; ++i) {
        cout << left << setw(10) << (i + 1)
            << setw(12) << sessions[i]
            << "\n";
    }
}

// -------------------------------
// MAIN
// -------------------------------
int main()
{
    setConsoleColor();
    showBanner();

    PlayerWeek playerWeek;
    playerWeek.totalTraining = 0.0;
    playerWeek.avgTraining = 0.0;
    playerWeek.sessionCount = 0;
    playerWeek.sleepHours = 0.0;
    playerWeek.readinessScore = 0.0;

    collectPlayerBasics(playerWeek);

    // Arrays for training sessions
    double trainingSessions[MAX_SESSIONS] = { 0.0 };

    fillTrainingSessions(trainingSessions, playerWeek.sessionCount, MAX_SESSIONS);

    computeTrainingStats(trainingSessions, playerWeek.sessionCount, playerWeek);

    playerWeek.sleepHours = getValidDouble("Avg sleep hours per night: ",
        MIN_SLEEP_HOURS);

    evaluateLevel(playerWeek);

    // Menu loop 
    int choice;
    do {
        choice = getMenuChoice();

        switch (choice)
        {
        case 1:
            handleLevelReport(playerWeek, trainingSessions, playerWeek.sessionCount);
            break;

        case 2:
            handleTrainingPlanReport(playerWeek);
            break;

        case 3:
            handleRecoveryReport(playerWeek);
            break;

        case 0:
            cout << "\nExiting...\n";
            break;
        }

    } while (choice != 0);

    return 0;
}
