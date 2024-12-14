#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
using namespace std;

int main(int argc, char* argv[]) {
    vector<string> args(&argv[1], &argv[argc]);
    sort(args.begin(), args.end());
    for (auto& s : args) {
        cout << s << endl;
    } 
    exit(0);
}