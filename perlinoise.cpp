#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"


#include<iostream>
#include<cmath>
#include<vector>
#include<cstdlib>
#include<ctime>

#include<unistd.h>

#include<string>
#include<fstream> // for std::ifstream

typedef struct{
	float x, y;
} vector2;

//static unsigned long int perlinRandTable[256] = {
//1000000033, 1000001453, 1000000799, 1000000019, 1000001111, 1000001919, 1000002103, 1000000847,
//    1000002211, 1000000363, 1000002069, 1000000609, 1000002437, 1000000241, 1000000289, 1000001637,
//    1000001237, 1000001193, 1000001481, 1000000333, 3791322487, 4378845491, 5785545827, 9992095787,
//    8988917363, 6211088437, 4791668137, 7967411383, 3489655667, 6574135511, 1000000841, 1000000907,
//    1000001013, 1000000867, 1000001597, 1000000759, 1000001217, 1000002157, 1000001699, 1000002097,
//    1000001831, 1000002053, 1000001311, 1000002077, 1000000583, 1000001851, 1000001619, 1000001987,
//    1000002349, 1000000769, 1000001723, 1000001087, 1000001741, 1000000317, 1000000283, 1000001811,
//    1000001613, 1000000781, 1000001537, 1000002153, 1000000261, 1000000381, 1000001341, 1000002341,
//    1000000123, 1000000087, 1000002357, 1000002381, 1000001021, 1000001153, 1000001293, 1000000973,
//    1000000531, 1000001441, 1000000173, 1000002027, 1000002417, 1000001837, 1000000277, 1000001777,
//    1000000467, 1000000567, 1000001521, 1000000661, 1000002399, 1000001559, 1000001403, 1000002511,
//    1000000647, 1000002179, 1000002011, 1000002093, 1000001847, 1000002173, 1000000951, 1000001451,
//    1000000923, 1000001997, 1000001381, 1000000427, 1000000181, 1000001171, 1000000579, 1000002129,
//    1000001129, 1000001117, 1000001361, 1000001147, 1000001633, 1000000773, 1000001677, 1000002217,
//    1000002243, 1000000897, 1000001839, 8330026847, 1000000523, 1000002021, 1000001049, 1000002481,
//    1000002117, 1000001531, 1000001511, 1000002143, 1000001079, 1000001759, 1000001033, 1000001733,
//    1000001969, 1000000297, 1000002393, 1000001201, 1000001223, 1000001907, 1000001579, 1000002073,
//    1000001483, 1000001639, 1000001091, 1000000251, 1000001849, 1000000623, 1000002497, 1000000379,
//    1000002161, 1000001761, 1000000163, 1000002003, 1000002337, 1000000509, 1000001949, 1000002191,
//    1000002107, 1000002221, 1000000013, 1000000473, 1000000619, 1000000669, 1000001319, 1000001351,
//    1000001567, 1000000487, 3284157443, 1000002371, 1000001139, 1000001373, 1000000849, 1000001259,
//    1000001471, 1000001667, 1000000703, 1000000683, 1000002133, 1000001029, 1000001877, 1000001681,
//    1000001267, 1000000037, 1000000453, 1000001163, 1911520717, 1000000447, 1000002441, 1000000957,
//    1000001109, 1000002197, 1000001043, 1798835459, 1000002059, 1000001229, 1000002491, 1000000961,
//    1000002111, 1000000727, 1000000463, 1000000627, 1000001411, 1000002249, 1000001943, 1000001873,
//    1000001489, 1000000301, 1000000411, 1000001093, 1000002039, 1000002131, 1000001891, 1000000351,
//    2048419325, 1000001857, 1000001881, 1000002247, 1000002029, 1000002293, 1000001819, 1000002431,
//    1000001123, 1000001343, 1000002489, 1000002209, 1000002273, 1000002141, 1000000253, 1000001513,
//    1000000047, 1000000719, 1000000881, 1000001609, 1000001823, 1000002447, 1000000651, 1000001793,
//    1000002159, 1000000313, 1000000129, 1000000169, 1000000553, 1000001549, 1000001379, 1000001039,
//    1000002493, 1000000877, 1000001169, 1000000461, 1000001269, 1000000059
//};
//
//
////picks a random value from perlin_randtab[] and returns it 
//static unsigned int randomPerlinValue(){
//	int randomIndex = rand() % 256;
//	int randomValue = perlinRandTable[randomIndex];
//
//	return randomValue;
//}
//static bool isPrime(unsigned n) {
//    if (n <= 1) return false;
//    if (n <= 3) return true;
//    if (n % 2 == 0 || n % 3 == 0) return false;
//    for (unsigned i = 5; i * i <= n; i += 6) {
//        if (n % i == 0 || n % (i + 2) == 0) return false;
//    }
//    return true;
//}

// Function to generate a random prime in a given range
//unsigned generateRandomPrime(unsigned min, unsigned max) {
//    unsigned candidate;
//    do {
//        candidate = min + rand() % (max - min);
//    } while (!isPrime(candidate));
//    return candidate;
//}


//func for genarating unqiue filename
std::string generateUnqiueFilename(const std::string& baseName, const std::string& extension){
	int counter = 1;
	//defines how filename string would be (name + counter + extension (png, jpg, etc))
	std::string filename = baseName + std::to_string(counter) + extension;

	//check if file already exists
	while(std::ifstream(filename)){
		counter++;
		filename = baseName + std::to_string(counter) + extension; //update filename
	}

	return filename;
}


//TODO: random number Gen for gradients, to differ noise
vector2 randomGradient(int ix, int iy, unsigned int seed){


	//no precomputed gradient means this works for any number of grid coords
	const unsigned w = 8 * sizeof(unsigned);
	const unsigned s = w / 2;
	unsigned a = ix + seed;
	unsigned b = iy + seed;

	a *= (3284157443 ^ seed);
	//a *= generateRandomPrime(1000000000, 4294967295); // Use a randomly generated prime
	//a *= randomPerlinValue();

	b ^= a << s | a >> w - s;
	b *= (1911520717 ^ seed);
	//b *= generateRandomPrime(1000000000, 4294967295); 
	//b *= randomPerlinValue();
		
	a ^= b << s | b >> w - s;
	a *= (2048419325 ^ seed);
	//a *= generateRandomPrime(1000000000, 4294967295);
	//a *= randomPerlinValue();


	float random = a * (3.14159265 / ~(~0u >> 1)); //in [0, 2*Pi]
	
	// Create the vector from the angle 
	vector2 v;
	v.x = sin(random);
	v.y = cos(random);

	return v;
}


//compute the dot product of the distance and gradient vectors
float dotGridGradient(int ix, int iy, float x, float y, unsigned int seed){
	//get gradient from interger coords
	vector2 gradient = randomGradient(ix, iy, seed);
	
	//compute distance vector 
	float dx = x - (float)ix;
	float dy = y - (float)iy;
	
	//compute the dot-product
	return (dx * gradient.x + dy * gradient.y);

}


float interpolate(float a0, float a1, float w){
	return (a1 - a0) * (3.0 - w * 2.0) * w * w + a0;
}


//sample perlin noise at coords x, y
float perlin(float x, float y, unsigned int seed){
	
	//Determine grid cell corner coords -> done by converting floats (x, y)  to ints
	//add 1 to get to the other side of the grid box
	int x0 = (int)x;
	int y0 = (int)y;
	int x1 = x0 + 1;
	int y1 = y0 + 1;


	//compute interpolation weights
	float sx = x - (float)x0;
	float sy = y - (float)y0;
	
	//cpmpute and interpolate top two corners (of 1 grid box)
	float n0 = dotGridGradient(x0, y0, x, y, seed);
	float n1 = dotGridGradient(x1, y0, x ,y, seed);
	float ix0 = interpolate(n0, n1, sx);

	//compute and interpolate bottom two corners
	n0 = dotGridGradient(x0, y1, x, y, seed);
	n1 = dotGridGradient(x1, y1, x ,y, seed);
	float ix1 = interpolate(n0, n1, sx);

	//Final step: interpolate between the two previously interpolated values, now in y
	float value = interpolate(ix0, ix1, sy);



	return value;
}


int main(int argc, char *argv[]) {

	int windowWidth = 1920; // pic will be 1920 x 1080 by default
	int windowHeight = 1080;
	const int GRID_SIZE = 400;

	unsigned int seed = static_cast<unsigned int>(time(0)); // Default seed based on current time



	int opt;
	while ((opt = getopt(argc, argv, "r:s:")) != -1) {
		switch (opt) {
			case 'r':  // Resolution argument
			if (sscanf(optarg, "%dx%d", &windowWidth, &windowHeight) != 2) {
				std::cerr << "Invalid format for resolution. Use -rWIDTHxHEIGHT.\n";
				return EXIT_FAILURE;
			} break;
			
			case 's':  // Seed argument
				seed = static_cast<unsigned int>(std::stoul(optarg));
				break;
			
			default:
				std::cerr << "Usage: " << argv[0] << " [-r WIDTHxHEIGHT] [-s SEED]\n";
				return EXIT_FAILURE;
		}
	}
	
	srand(seed);  // Seed the random number generator
	std::cout << "Using seed: " << seed << std::endl;
	std::cout << "Resolution: " << windowWidth << "x" << windowHeight << std::endl;

	//create array to store RGB (Maily Alpha) values, 4 values per pixel
	std::vector<unsigned char> pixels(windowWidth * windowHeight * 4, 0);

	//sample noise in each pixel in the array
	for (int x = 0; x < windowWidth; x++){
		for(int y = 0; y < windowHeight; y++){
			//calcing the correct starting index in the array for the current pixel based on the X and Y position we are at
			int index = (y * windowWidth + x) * 4;

			float val = 0;

			float freq = 1;
			float amp = 1;

			//loop for the number of octives we want, in this case 12
			//for each loop cacling the freqs and amps 
			for (int i = 0; i < 12; i++){
				val += perlin(x * freq / GRID_SIZE, y * freq / GRID_SIZE, seed) * amp;
				
				//for gradient/convulution
				freq *= 2;
				amp /= 2;
			}

			//contrast
			val *= 1.2;

			//clipping/clamping, ensuring values are always between 1 and -1 only
			if (val > 1.0f){
				val = 1.0f;
			}
			else if (val < -1.0f){
				val = -1.0f;
			}

			//convert "1 to -1" int "255 to 0" for image
			int color = (int)(((val + 1.0f) * 0.5f) * 255);
			
			//set pixel colors
			pixels[index] = color;
			pixels[index + 1] = color;
			pixels[index + 2] = color;
			pixels[index + 3] = 255; //alpha channel (between black & white)
		}
	}

	//generating unique filename
	//TODO: arguments for different names and extensions
	std::string filename = generateUnqiueFilename("perlin_noise", ".png");

	//using stb to generate image out of pixels
	stbi_write_png(filename.c_str(), windowWidth, windowHeight, 4, pixels.data(), windowWidth * 4);
	

	return 0; 

}
