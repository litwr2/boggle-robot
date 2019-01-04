#include<iostream>
#include<fstream>
#include<string>
#include<set>
#include<cctype>
#include<utility>

#ifndef BOXSIZE
#define BOXSIZE 5
#endif

#ifndef MINWORDSIZE
#define MINWORDSIZE 4
#endif

#ifndef DICTFILENAME
#define DICTFILENAME "words"
#endif

using namespace std;

set<string> words;

struct Cell {
	pair<int, int> d[8]; //0 - right, clockwise
	char v;
} mesh[BOXSIZE][BOXSIZE];

void loadWords() {
	ifstream fi(DICTFILENAME);
	while (!fi.eof()) {
		string s;
		fi >> s;
		if (s.length()) words.insert(s);
	}
}

struct Less {
	bool operator()(const string& s1, const string& s2) const {
		if (s1.length() == s2.length())
			return s1 < s2;
		return s1.length() < s2.length();
	}
};

set<string, Less> results;

void initMesh() {
	int c, r;
	for (r = 0; r < BOXSIZE; r++)
		for (c = 0; c < BOXSIZE; c++)
			for (int d = 0; d < 8; d++)
				mesh[r][c].d[d] = {8, 8};
	for (r = 1; r < BOXSIZE - 1; r++)
		for (c = 1; c < BOXSIZE - 1; c++) {
			mesh[r][c].d[0] = {r, c - 1};
			mesh[r][c].d[1] = {r - 1, c - 1};
			mesh[r][c].d[2] = {r - 1, c};
			mesh[r][c].d[3] = {r - 1, c + 1};
			mesh[r][c].d[4] = {r, c + 1};
			mesh[r][c].d[5] = {r + 1, c + 1};
			mesh[r][c].d[6] = {r + 1, c};
			mesh[r][c].d[7] = {r + 1, c - 1};
		}
        c = 0;
	for (r = 1; r < BOXSIZE - 1; r++) {
		mesh[r][c].d[2] = {r - 1, c};
		mesh[r][c].d[3] = {r - 1, c + 1};
		mesh[r][c].d[4] = {r, c + 1};
		mesh[r][c].d[5] = {r + 1, c + 1};
		mesh[r][c].d[6] = {r + 1, c};
	}
        c = BOXSIZE - 1;
	for (r = 1; r < BOXSIZE - 1; r++) {
		mesh[r][c].d[0] = {r, c - 1};
		mesh[r][c].d[1] = {r - 1, c - 1};
		mesh[r][c].d[2] = {r - 1, c};
		mesh[r][c].d[6] = {r + 1, c};
		mesh[r][c].d[7] = {r + 1, c - 1};
	}
        r = 0;
	for (c = 1; c < BOXSIZE - 1; c++) {
		mesh[r][c].d[0] = {r, c - 1};
		mesh[r][c].d[4] = {r, c + 1};
		mesh[r][c].d[5] = {r + 1, c + 1};
		mesh[r][c].d[6] = {r + 1, c};
		mesh[r][c].d[7] = {r + 1, c - 1};
	}
        r = BOXSIZE - 1;
	for (c = 1; c < BOXSIZE - 1; c++) {
		mesh[r][c].d[0] = {r, c - 1};
		mesh[r][c].d[1] = {r - 1, c - 1};
		mesh[r][c].d[2] = {r - 1, c};
		mesh[r][c].d[3] = {r - 1, c + 1};
		mesh[r][c].d[4] = {r, c + 1};
	}
        r = c = 0;
	mesh[r][c].d[4] = {r, c + 1};
	mesh[r][c].d[5] = {r + 1, c + 1};
	mesh[r][c].d[6] = {r + 1, c};
        c = BOXSIZE - 1;
	mesh[r][c].d[0] = {r, c - 1};
	mesh[r][c].d[6] = {r + 1, c};
	mesh[r][c].d[7] = {r + 1, c - 1};
        r = BOXSIZE - 1;
	c = 0;
	mesh[r][c].d[2] = {r - 1, c};
	mesh[r][c].d[3] = {r - 1, c + 1};
	mesh[r][c].d[4] = {r, c + 1};
        c = BOXSIZE - 1;
	mesh[r][c].d[0] = {r, c - 1};
	mesh[r][c].d[1] = {r - 1, c - 1};
	mesh[r][c].d[2] = {r - 1, c};
}

void getSquare() {
	cerr << "Enter wordbox:\n";
	for (int r = 0; r < BOXSIZE; r++)
		for (int c = 0; c < BOXSIZE; c++) {
			char x;
			do
				x = cin.get();
			while (!isalpha(x));
			mesh[r][c].v = toupper(x);
		}
}

bool seekWord(int r, int c, const string& w, set<pair<int, int>> path) {
        if (w.length() == 0) return true;
	if (mesh[r][c].v != w[0]) return false;
	for (int d = 0; d < 8; d++)
		if (mesh[r][c].d[d].first != 8 && path.find({mesh[r][c].d[d].first, mesh[r][c].d[d].second}) == path.end()) {
			set<pair<int, int>> path1 = path;
			path1.insert({mesh[r][c].d[d].first, mesh[r][c].d[d].second});
			if (seekWord(mesh[r][c].d[d].first,
				mesh[r][c].d[d].second,
				w[0] == 'Q' ? w.substr(2) : w.substr(1),
				path1))
					return true;
		}
	return false;
}

void getResults() {
	for (auto p = words.begin(); p != words.end(); p++)
		for (int r = 0; r < BOXSIZE; r++)
			for (int c = 0; c < BOXSIZE; c++) {
				set<pair<int, int>> path;
				path.insert({r, c});
				if (seekWord(r, c, *p, path)) results.insert(*p);
			}
}

void printResults() {
	for (auto p = results.rbegin(); p != results.rend(); p++)
		if (p->length() >= MINWORDSIZE)
			cout << *p << endl;
}

int main() {
	loadWords();
	//for(auto p = words.begin(); p != words.end(); p++) cout << *p << endl; return 0;
	initMesh();
	getSquare();
#if 0
        for (int r = 0; r < BOXSIZE; r++) {
		for (int c = 0; c < BOXSIZE; c++) {
			cout << mesh[r][c].v;
			for (int d = 0; d < 8; d++) cout << mesh[r][c].d[d].first  << mesh[r][c].d[d].second << '-';
			cout << ' ';
		}
		cout << endl;
	}
	return 0;
#endif
	getResults();
	printResults();
	return 0;
}

