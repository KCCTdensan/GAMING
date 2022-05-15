#include<iostream>
#include<cmath>
#include<vector>

using std::cout; using std::cin; using std::endl; using std::vector;
using std::cos;

vector<int> nu;

void rainbowcalc(int s),outputrainbow(int s);

int main(){
	int sep;
	cout << "SEPRATE : " << endl;
	cin >> sep;

	rainbowcalc(sep);

	outputrainbow(sep);

	return 0;
}

void rainbowcalc(int s){
	long double rng=s/3;

	for(int theta=0;theta<rng;theta++){
		int waveform=128 - (127 * cos(2.0 * M_PI * ((long double)theta / rng)) );
		nu.push_back(waveform);
	}
	return;
}

void outputrainbow(int s){
	for(int i=0;i<(s/3)-1;i++){
		cout << nu[i] << " , ";
	}
	cout << nu[s/3];
	return;
}