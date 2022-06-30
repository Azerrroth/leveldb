#include <fstream>
#include <iostream>
#include <stdlib.h>

using namespace std;

int main(int argc, char** argv) {
  string path = "names.data";
  string out = "testout.txt";

  ifstream in(path.c_str());
  ofstream ou(out.c_str());

  if (!in.is_open()) {
    cerr << "open file failed!" << endl;
    exit(-1);
  }

  if (!ou.is_open()) {
    cerr << "create file failed!" << endl;
    exit(-1);
  }

  string s = "";
  while (getline(in, s)) {
    cout << s << endl;
    ou << s << endl;
  }

  in.close();
  ou.close();

  return 0;
}
