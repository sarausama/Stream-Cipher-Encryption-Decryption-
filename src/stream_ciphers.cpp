#include <iostream>
#include <cctype>
#include <cmath>

int main();

char *encode( char *plaintext, unsigned long key);
char *decode( char *ciphertext, unsigned long key);

int main(){
	unsigned long key{51323};//the key used to encode the text

	char str0[]{"Hello world!"}; // an example of text to encode
	std::cout<<"\""<<str0<<"\""<<std::endl;
	char *ciphertext{encode(str0, key)};
	std::cout<<"\""<<ciphertext<<"\""<<std::endl;
	char *plaintext{decode(ciphertext, key)};
	std::cout<<"\""<<plaintext<<"\""<<std::endl;
	delete[] ciphertext;
	delete[] plaintext;
	ciphertext = nullptr;
	plaintext = nullptr;
}

char *encode( char *plaintext, unsigned long key){
	//my encode function
	unsigned char s[256];
	unsigned char R;
	for(std::size_t k{0}; k<256; ++k){
		s[k] = k;
	}
	std::size_t i{0}, j{0}, k{0};
	for(int o{0}; o<256; ++o){
		k = i % 64;
		j = (j + s[i] + ((key >> k) & 1L))%256;
		unsigned char temp{};
		temp = s[i];
		s[i] = s[j];
		s[j] = temp;
		i = (i+1)%256;
	}
	unsigned long a{0}, counter{0};
	while(plaintext[a] != '\0'){
		++counter;
		++a;
	}
//	std::cout<<counter;

	std::size_t n = std::ceil (counter/4.0);
	unsigned char good_array[4*n+1];
	for(std::size_t h{0}; h< n*4+1; ++h){
		if(h<counter){
			good_array[h] = plaintext[h];
		} else {
			good_array[h]='\0';
		}
	}
	unsigned char result[4*n+1];

//std::cout<< 4*n<<std::endl;
	for(std::size_t y{0}; y< 4*n; ++y){
		i = (i+1)%256;
		j = (j + s[i] )%256;
		unsigned char temp{};
		temp = s[i];
		s[i] = s[j];
		s[j] = temp;
		std::size_t r{};
		r = (s[i]+ s[j]) % 256;
		R = s[r];
		result [y] = R ^ good_array[y];
	}

//std::cout<<n*5<<std::endl;
	char armour[n*5 + 1]{};
	for(std::size_t o{0}; o<n; ++o){
		unsigned int temp{};
		unsigned int temp1 = result[4*o]<<24;
		unsigned int temp2 = result[4*o + 1]<<16;
		unsigned int temp3 = result[4*o + 2]<<8;
		unsigned int temp4 = result[4*o + 3];
		temp = temp1+temp2+temp3+temp4;
		for(std::size_t x{0}; x<5; ++x){
			armour[(4-x) + 5*o] = '!' + temp % 85;
			temp = temp / 85;
		}
}
	armour[n*5] = '\0';
	char *pointer{new char[n*5 + 1]};
	for(std::size_t a{0}; a<= 5*n; ++a){
		pointer[a] = armour[a];
	}
//	std::cout<< 5*n+1;
	return pointer;

}

char *decode( char *ciphertext, unsigned long key){
	//my decode function
	std::size_t a{0}, counter{0};
	while(ciphertext[a] != '\0'){
		++counter;
		++a;
	}
	unsigned int n{static_cast<unsigned int>((counter*4.0)/5.0)};
	unsigned char result[n];
	char text[n+1];
	unsigned int R{};
	for(std::size_t o{0}; o< counter/5.0; ++o){
		unsigned int temp{};
		temp = (ciphertext[o*5] - '!') * pow(85,4)
			+  (ciphertext[o*5+1] - '!') * pow(85,3)
			+  (ciphertext[o*5+2] - '!') * pow(85,2)
			+  (ciphertext[o*5+3] - '!') * pow(85,1)
			+  (ciphertext[o*5+4] - '!') * pow(85,0);
//		std::cout<<temp<<" ";
		result[4*o]	   = temp>>24;
		result[4*o + 1]= temp>>16;
		result[4*o + 2]= temp >>8;
		result[4*o + 3]= temp;
		/*
		result[4*o]	   = temp>>24;
		result[4*o + 1]= (temp - (result[4*o]<<24))>>16;
		result[4*o + 2]= (temp - (result[4*o]<<24) - (result[4*o + 1]<<16))>>8;
		result[4*o + 3]=
						(temp -
						(result[4*o]<<24) -
						(result[4*o + 1]<<16) -
						(result[4*o + 2]<<8));
*/
	}

	unsigned char state[256];
	for(std::size_t k{0}; k<256; ++k){
		state[k] = k;
	}
	std::size_t i{0}, j{0}, k{0};
	for(int o{0}; o<256; ++o){
		k = i % 64;
		j = (j + state[i] + ((key >> k) & 1))%256;
		unsigned char temp{};
		temp = state[i];
		state[i] = state[j];
		state[j] = temp;
		i = (i+1)%256;
	}

	for(std::size_t y{0}; y< n; ++y){
		i = (i+1)%256;
		j = (j + state[i] )%256;
		unsigned char temp{};
		temp = state[i];
		state[i] = state[j];
		state[j] = temp;
		std::size_t r{};
		r = (state[i]+ state[j]) % 256;
		R = state[r];
		text [y] = R ^ result[y];
	}

	text[n] = '\0';
	char *pointer{new char[n+1]};
	for(std::size_t a{0}; a<=n; ++a){
		pointer[a] = text[a];
	}
//	std::cout<<pointer;
	return pointer;
}

