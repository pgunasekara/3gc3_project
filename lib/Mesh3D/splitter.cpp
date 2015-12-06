#include <string>
#include <fstream>
#include <iostream>
#include "splitter.h"
#include <stdio.h>
#include <vector>

using namespace std;

vector<string> split(string data, char c) {
	vector<string> vector;
	int split = 0;

	for (int i = 0;i < data.length();i++) {
		if (data[i] == c) {
			string sub = data.substr(split, i - split );
			vector.push_back(sub);
			split = i + 1;
		}
		else {}
	}
	string sub = data.substr(split, data.length() - split );
	vector.push_back(sub);
	
	return vector;
}
