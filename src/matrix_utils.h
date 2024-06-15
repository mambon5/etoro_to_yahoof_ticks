#include <algorithm>
#include <vector>
#include <iostream>
#include <string>

using namespace std; 

bool vD_sortCrit_2col(const vector<double>& v1, const vector<double>& v2)
{
    int col1 = 0; // columna 1
    int col2 = 1; // columna 2
    if(v1[col1] == v2[col1]) return v1[col2] > v2[col2]; // si la columna 1 és igual en les dues files, ordena doncs per la columna 2
    return v1[col1] < v2[col1];


}

void vD_sortBy2Col(vector<vector<double>>& vect) {
    sort(vect.begin(), vect.end(), vD_sortCrit_2col);
}



int findPos(const vector<string> & vector, const string & elem) { // returns index of elem in array or -1 if not found
    int posicio = 0;

    for(string str: vector) {
        if(str == elem) return posicio;
        posicio = posicio + 1;
    }
    return -1;
}
