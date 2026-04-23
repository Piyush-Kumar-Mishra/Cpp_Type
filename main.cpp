#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<ctime>
#include<conio.h>
#include<sstream>
#include<iomanip>
#include<direct.h>
#include<windows.h>

using namespace std;

const int MAX_HISTORY=10;
const string DATA_FOLDER="History";


// SetConsoleTextAttribute changes text color in Windows terminal
// GetStdHandle(STD_OUTPUT_HANDLE) gets the console output handle
// 7=white, 10=green, 12=red, 14=yellow, 11=cyan, 8=gray
void setColor(int color){
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);    
}


void clearScreen(){
    system("cls");
}

bool fileExists(string filename){
    ifstream file(filename);
    bool exists=file.good();
    file.close();
    return exists;
}

void createFolder(string folder){
    _mkdir(folder.c_str());  // c_str() converts string to char array needed by _mkdir
}

string getRandomSentence(){
    vector<string> sentences = {
        "I pushed the code and then immediately found a bug in production.",
        "Debugging sometimes takes longer than writing the actual feature.",
        "I spent more time naming variables than solving the problem itself.",
        "The code works fine but I still do not fully trust it.",
        "I forgot to save the file and lost all my changes again."
    };

    int index = rand() % sentences.size();
    return sentences[index];
}

struct Record{
    double netWpm;
    double accuracy;
    int totalErrors;
};


vector<Record> loadHistory(string username){
    vector<Record> records;
    string filepath=DATA_FOLDER+"/"+username+".txt";

    if(!fileExists(filepath)){
        return records;
    }

    ifstream file(filepath);
    string line;

    while(getline(file,line)){
        if(line.empty()) continue;

        Record r;
        stringstream ss(line);
        string token;

        getline(ss,token,','); r.netWpm=stod(token);
        getline(ss,token,','); r.accuracy=stod(token);
        getline(ss,token,','); r.totalErrors=stoi(token);

        records.push_back(r);
    }
    file.close();
    return records;
}

void saveHistory(string username, vector<Record> records){
    while(records.size()>MAX_HISTORY){
        records.erase(records.begin());
    }

    string filepath=DATA_FOLDER+"/"+username+".txt";
    ofstream file(filepath); 

    for(int i=0;i<records.size();i++){
        file<<fixed<<setprecision(1);
        file<<records[i].netWpm<<","
            <<records[i].accuracy<<","
            <<records[i].totalErrors<<endl;
    }
    file.close();
}

void displayHistory(string username){
    vector<Record> records=loadHistory(username);

    cout<<endl;
    setColor(11);
    cout<<"------------------------------------------"<<endl;
    cout<<"     HISTORY - "<<username<<endl;
    cout<<"------------------------------------------"<<endl;
    setColor(7);

    if(records.empty()){
        setColor(14);
        cout<<"No previous records found"<<endl;
        setColor(7);
        cout<<endl;
        return;
    }

    setColor(8);
    cout<<"  |-----|----------|---------|------|"<<endl;
    setColor(11);
    cout<<"  | "<<setw(3)<<left<<"#"
        <<" | "<<setw(8)<<left<<"Net WPM"
        <<" | "<<setw(7)<<left<<"Acc%"
        <<" | "<<setw(4)<<left<<"Err"
        <<" |"<<endl;
    setColor(8);
    cout<<"|-----|----------|---------|------|"<<endl;
    setColor(7);

    for(int i=0;i<records.size();i++){
        cout<<"  | "<<setw(3)<<left<<(i+1)
            <<" | "<<setw(8)<<left<<fixed<<setprecision(1)<<records[i].netWpm
            <<" | "<<setw(6)<<left<<records[i].accuracy<<"%"
            <<" | "<<setw(4)<<left<<records[i].totalErrors
            <<" |"<<endl;
    }
    setColor(8);
    cout<<"  +-----+----------+---------+------+"<<endl;
    setColor(7);

    // best score
    double bestWpm=0;
    int bestIndex=0;
    for(int i=0;i<records.size();i++){
        if(records[i].netWpm>bestWpm){
            bestWpm=records[i].netWpm;
            bestIndex=i;
        }
    }

    cout<<endl;
    setColor(14);
    cout<<"  Previous Score : Net WPM = "<<fixed<<setprecision(1)<<records.back().netWpm
        <<" | Accuracy = "<<records.back().accuracy<<"%"<<endl;
    setColor(10);
    cout<<"  Best Score     : Net WPM = "<<bestWpm
        <<" | Accuracy = "<<records[bestIndex].accuracy<<"%"<<endl;
    setColor(7);
    cout<<endl;
}

vector<string> getExistingUsers(){
    vector<string> users;
    string filepath=DATA_FOLDER+"/users.txt";

    if(!fileExists(filepath)) return users;

    ifstream file(filepath);
    string name;
    while(getline(file,name)){
        if(!name.empty()) users.push_back(name);
    }
    file.close();
    return users;
}

void showUsers(vector<string> users){
    if(users.empty()){
        setColor(14);
        cout<<"  No existing users found."<<endl;
        setColor(7);
        return;
    }
    setColor(11);
    cout<<"  Existing Users:"<<endl;
    setColor(7);
    for(int i=0;i<users.size();i++){
        cout<<"    "<<(i+1)<<". "<<users[i]<<endl;
    }
}

string selectUser(){
    createFolder(DATA_FOLDER);

    while(true){
        clearScreen();
        setColor(11);
        cout<<endl;
        cout<<"------------------------------------------------------"<<endl;
        cout<<"TYPING SPEED TEST"<<endl;
        cout<<"------------------------------------------------------"<<endl;
        setColor(7);
        cout<<endl;

        vector<string> users=getExistingUsers();
        showUsers(users);

        cout<<endl;
        setColor(14);
        cout<<"  1. Select existing user"<<endl;
        cout<<"  2. Create new user"<<endl;
        setColor(7);
        cout<<endl;
        cout<<"  Enter choice (1 or 2): ";

        string choiceStr;
        getline(cin,choiceStr);

        if(choiceStr!="1" && choiceStr!="2"){
            setColor(12);
            cout<<"  Invalid choice! Please enter 1 or 2."<<endl;
            setColor(7);
            cout<<"  Press any key to try again...";
            _getch();
            continue;
        }

        int choice=stoi(choiceStr);

        if(choice==1){
            if(users.empty()){
                setColor(12);
                cout<<"No users exist yet"<<endl;
                setColor(7);
                cout<<"Press any key to continue...";
                _getch();
                continue;
            }

            cout<<"  Enter user number: ";
            string numStr;
            getline(cin,numStr);

            bool valid=true;
            for(int i=0;i<numStr.size();i++){
                if(!isdigit(numStr[i])){valid=false; break;}
            }

            if(!valid || numStr.empty()){
                setColor(12);
                cout<<"  Invalid input"<<endl;
                setColor(7);
                cout<<"Press any key to try again";
                _getch();
                continue;
            }

            int num=stoi(numStr);
            if(num<1 || num>users.size()){
                setColor(12);
                cout<<"  Invalid user number! Choose between 1 and "<<users.size()<<"."<<endl;
                setColor(7);
                cout<<"  Press any key to try again";
                _getch();
                continue;
            }

            setColor(7);
            cout<<"Press any key to continue";
            _getch();
            return users[num-1];

        }
        else{
            cout<<"Enter your name (no spaces): ";
            string name;
            getline(cin,name);

            if(name.empty()){
                setColor(12);
                cout<<"  Name cannot be empty!"<<endl;
                setColor(7);
                cout<<"  Press any key to try again...";
                _getch();
                continue;
            }

            bool validName=true;
            for(int i=0;i<name.size();i++){
                if(!isalnum(name[i]) && name[i]!='_'){
                    validName=false;
                    break;
                }
            }

            if(!validName){
                setColor(12);
                cout<<"  Name can only contain letters, numbers, and underscores!"<<endl;
                setColor(7);
                cout<<"  Press any key to try again...";
                _getch();
                continue;
            }

            bool alreadyExists=false;
            for(int i=0;i<users.size();i++){
                if(users[i]==name){ alreadyExists=true; break; }
            }

            if(alreadyExists){
                setColor(14);
                cout<<"  User '"<<name<<"' already exists! Logging in..."<<endl;
                setColor(7);
                cout<<"  Press any key to continue...";
                _getch();
                return name;
            }

            ofstream userFile(DATA_FOLDER+"/users.txt",ios::app);
            userFile<<name<<endl;
            userFile.close();

            // create empty history file for new user
            ofstream histFile(DATA_FOLDER+"/"+name+".txt");
            histFile.close();

            setColor(10);
            cout<<"  User '"<<name<<"' created successfully!"<<endl;
            setColor(7);
            cout<<"  Press any key to continue...";
            _getch();
            return name;
        }
    }
}


Record runTest(string sentence){
    clearScreen();
    Record result;
    
    setColor(14);
    cout<<"Type:"<<endl;
    setColor(7);
    cout<<sentence<<endl;
    cout<<endl;
    setColor(14);
    cout<<"Your input:"<<endl;
    setColor(7);
    cout<<"  ";

    string input="";
    int correctChars=0;
    int wrongChars=0;
    int extraChars=0;
    int totalKeystrokes=0;
    bool timerStarted=false;
    clock_t startTime, endTime;

    while(true){
        char ch=_getch();

        if(!timerStarted){
            startTime=clock();
            timerStarted=true;
        }

        // Enter(finish test)
        if(ch==13){
            if(input.size()>0){
                break;
            }
            continue;
        }

        // Backspace
        if(ch==8){
            if(input.size()>0){
                int pos=input.size()-1;
                if(pos<sentence.size()){
                    if(input[pos]==sentence[pos]){
                        correctChars--;
                    }
                    else{
                        wrongChars--;
                    }
                }
                else{
                    extraChars--;
                }

                input.pop_back();
                cout<<"\b \b";  // move cursor back, print space, move back again
            }
            continue;
        }

        if(ch==0 || ch==-32){
            _getch(); 
            continue;
        }

        totalKeystrokes++;
        int pos=input.size();

        if(pos<sentence.size()){
            if(ch==sentence[pos]){
                correctChars++;
                setColor(10);
                cout<<ch;
                setColor(7);
            }
            else{
                wrongChars++;
                setColor(12);
                cout<<ch;
                setColor(7);
            }
        }
        else{
            extraChars++;
            setColor(12);
            cout<<ch;
            setColor(7);
        }

        input+=ch;

        if(input.size()==sentence.size()){
            break;
        }
    }

    endTime=clock();
    double timeInSeconds=(double)(endTime-startTime)/CLOCKS_PER_SEC;
    double timeInMinutes=timeInSeconds/60.0;
    if(timeInMinutes<0.001) timeInMinutes=0.001;  // prevent division by zero

    int totalErrors=wrongChars+extraChars;
    double netWpm=((double)correctChars/5.0)/timeInMinutes;

    double accuracy=0;
    if(totalKeystrokes>0){
        accuracy=(correctChars*100.0)/totalKeystrokes;
    }

    result.netWpm=netWpm;
    result.accuracy=accuracy;
    result.totalErrors=totalErrors;

    return result;
}

void displayResults(Record result){
    cout<<endl<<endl;
    cout<<fixed<<setprecision(1);

    setColor(14);
    cout<<"  Net WPM       :  ";
    if(result.netWpm>=40) setColor(10);     
    else if(result.netWpm>=20) setColor(14);  
    else setColor(12);
    cout<<result.netWpm<<endl;
    setColor(7);

    setColor(14);
    cout<<"  Accuracy      :  ";
    if(result.accuracy>=90) setColor(10);
    else if(result.accuracy>=70) setColor(14);
    else setColor(12);
    cout<<result.accuracy<<"%"<<endl;
    setColor(7);

    setColor(14);
    cout<<"  Total Errors  :  ";
    if(result.totalErrors==0) setColor(10);
    else setColor(12);
    cout<<result.totalErrors<<endl;
    setColor(7);

    cout<<endl;
}

void showMenu(){
    setColor(14);
    cout<<"  1. Try Again"<<endl;
    cout<<"  2. View History"<<endl;
    cout<<"  3. Switch User"<<endl;
    setColor(12);
    cout<<"  0. Exit"<<endl;
    setColor(7);
    cout<<endl;
    cout<<"  Enter choice: ";
}

int main(){
    srand(time(0));
    createFolder(DATA_FOLDER);

    while(true){
        string username=selectUser();
        setColor(7);
        displayHistory(username);

        cout<<" Press any key to start";
        _getch();
        clearScreen();

        bool switchUser=false;
        while(!switchUser){
            clearScreen();
            setColor(14);
            cout<<"  1. Random Sentence"<<endl;
            cout<<"  2. Type your own"<<endl;
            setColor(7);
            cout<<"  Enter choice: ";
            string opt;
            getline(cin,opt);
            string sentence;
            if(opt=="2"){
                cout<<"  Enter your sentence: ";
                getline(cin,sentence);
                if(sentence.empty()){
                    sentence=getRandomSentence();
                }
            }
            else{
                sentence=getRandomSentence();
            }
            Record result=runTest(sentence);

            displayResults(result);
            vector<Record> history=loadHistory(username);
            history.push_back(result);
            saveHistory(username,history);

            while(true){
                showMenu();

                string choiceStr;
                getline(cin,choiceStr);

                if(choiceStr=="1"){
                    break;
                }
                else if(choiceStr=="2"){
                    clearScreen();
                    displayHistory(username);
                    cout<<"Press any key to go back";
                    _getch();
                    displayResults(result);
                    continue;
                }
                else if(choiceStr=="3"){
                    switchUser=true;
                    break;
                }
                else if(choiceStr=="0"){
                    return 0;

                }
                else{
                    setColor(12);
                    cout<<"Invalid"<<endl;
                    setColor(7);
                    continue;
                }
            }
        }
    }

    return 0;
}

