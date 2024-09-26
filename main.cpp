#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <chrono>
#include <iomanip>

using namespace std;

void selectType(string &my_text) {
    string selected;
     cout<<"------------------------------------------------------------------"<<endl;
    cout << "Select one" << endl;
    cout << "1 option : Words" << endl;
    cout << "2 option : Custom" << endl;
    getline(cin, selected);

    if (selected == "words") {
        int selected_count;
        cout << "Select no of words : 10 / 15 / 30" << endl;
        cin >> selected_count;
        cin.ignore();

        if (selected_count == 10) {
            my_text = "Pointers in C++ allow efficient memory management and dynamic resource allocation";
        } else if (selected_count == 15) {
            my_text = "Object-oriented programming in C++ enables developers to create reusable code using classes, objects, and inheritance";
        } else if (selected_count == 30) {
            my_text = "C++ is a powerful language used for system programming, game development, and performance-critical applications. Its features like templates, polymorphism, and operator overloading allow developers to write flexible, efficient, and maintainable code";
        } else {
            cout << "Invalid!";
            exit(0);
        }
    } else if (selected == "custom") {
        cout << "Enter your Custom Sentence (press enter): ";
        cin.ignore();
        getline(cin, my_text);
    } else {
        cout << "Invalid!";
        exit(0);
    }
}

void output(const string &input, const string &my_text, double time) {
    int matching_chars = 0;
    int unmatched_chars = 0;
    int total_char = my_text.length();
    
    for (size_t i = 0; i < total_char; i++) {
        if (i < input.length() && input[i] == my_text[i]) {
            matching_chars++;
        } else {
            unmatched_chars++;
        }
    }
    
    unmatched_chars += max(0, static_cast<int>(input.length()) - total_char);

    double words_typed = count_if(input.begin(), input.end(), [](char c) {
        return isspace(c);
    }) + 1;
    
    double words_per_min = (time > 0) ? (words_typed / (time / 60.0)) : 0;
    double accuracy = (time > 0) ? (static_cast<double>(matching_chars) / total_char) * 100 : 0;

    if (time > 0) {
        cout <<endl<< "------------------------------- RESULTS ---------------------------" << endl;
        cout <<endl<< "            Average Speed                      Accuracy" << endl;
        cout << "              " << fixed << setprecision(2) << words_per_min << " wpm                     " << fixed << setprecision(2) << accuracy << "%" << endl;
        cout << "            Keystrokes                         Errors" << endl;
        cout << "                " << input.length() << " chars                     " << unmatched_chars << " mistakes" << endl;
    } else {
        cout << "Error! Time = 0" << endl;
    }
}

int main() {
    string my_text;

    selectType(my_text);
    cout<<endl;
    cout << "-----------------------------Enter The Text-------------------------------" << endl;
    cout << "Type: " << my_text << endl;

    // Start timing right after displaying the text
    auto start_time = chrono::high_resolution_clock::now();

    string input;
    getline(cin, input);  // Directly read the input after the prompt.

    auto end_time = chrono::high_resolution_clock::now();
    chrono::duration<double> time = end_time - start_time;

    output(input, my_text, time.count());

    cout<<"---------------------------------------------------------------------------";
    return 0;
}
