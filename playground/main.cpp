using namespace std;
#include <iostream>




int main() {

    cout << "Hello World!" << endl;

    string command;

    while (command != "EXIT") {
        cin >> command;
        if (command == "hello") {
            cout << "Hey there! " << endl;
        }
    }

    return 0;
}