#include<iostream>
#include<string>
#include<cmath>
#include<vector>

using std::cout; using std::cerr; using std::cin; using std::endl; using std::vector; using std::string;
using std::cos;

vector<int> nu;

void rainbowcalc( int s, int m, bool expon ), outputrainbow( int s );

int main(){
	int sep;
	cerr << "SEPRATE : " << endl;
	cin >> sep;

	int maxv;
	cerr << "MAX VALUE : " << endl;
	cin >> maxv;

	string expsel;
	cerr << "EXP()? [Y/N] : " << endl;
	cin >> expsel;

	rainbowcalc( sep, maxv, ( expsel == "Y" ) );

	outputrainbow( sep );

	return 0;
}

void rainbowcalc( int s, int m, bool expon ) {
	long double rng = s / 3;

	for( int theta = 0; theta <= rng; theta++ ) {
		int waveform;
		long double cosvalue = cos( 2.0 * M_PI * ( (long double) theta / rng ) );
		if( expon ) waveform = ( m * ( ( exp( cosvalue - 1.0 ) ) + ( exp( -2.0 ) * ( cosvalue - 1.0 ) / 2.0 ) ) );
		else waveform = ( m / 2 ) - ( ( m / 2.0 - 1 ) * cosvalue );
		nu.push_back( waveform );
	}
	return;
}

void outputrainbow( int s ) {
	for( int i = 0; i < ( s / 3 ) - 1; i++ ) {
		cout << nu[i] << ", ";
	}
	cout << nu[ s / 3 ] << endl;
	return;
}