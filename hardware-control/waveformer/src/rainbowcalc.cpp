#include<iostream>
#include<string>
#include<cmath>
#include<vector>

using std::cout; using std::cin; using std::endl; using std::vector; using std::string;
using std::cos;

vector<int> nu;

void rainbowcalc( int s, int m, bool expon ), outputrainbow( int s );

int main(){
	int sep;
	cout << "SEPRATE : " << endl;
	cin >> sep;

	int maxv;
	cout << "MAX VALUE : " << endl;
	cin >> maxv;

	string expsel;
	cout << "EXP()? [Y/N] : " << endl;
	cin >> expsel;

	rainbowcalc( sep, maxv, ( expsel == "Y" ) );

	outputrainbow( sep );

	return 0;
}

void rainbowcalc( int s, int m, bool expon ) {
	long double rng = s / 3;

	for( int theta = 0; theta <= rng; theta++ ) {
		int waveform;
		if( expon ) waveform = ( m * exp( cos( 2.0 * M_PI * ( (long double) theta / rng ) ) ) / M_E );
		else waveform = ( m / 2 ) - ( ( m / 2 - 1 ) * cos( 2.0 * M_PI * ( (long double) theta / rng ) ) );
		nu.push_back( waveform );
	}
	return;
}

void outputrainbow( int s ) {
	for( int i = 0; i < ( s / 3 ) - 1; i++ ) {
		cout << nu[i] << " , ";
	}
	cout << nu[ s / 3 ] << endl;
	return;
}