#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"


#include<iostream>
#include<cmath>
#include<vector>
#include<cstdlib>
#include<ctime>

#include<string>
#include<fstream> // for std::ifstream

typedef struct{
	float x, y;
} vector2;

static bool isPrime(unsigned n) {
    if (n <= 1) return false;
    if (n <= 3) return true;
    if (n % 2 == 0 || n % 3 == 0) return false;
    for (unsigned i = 5; i * i <= n; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0) return false;
    }
    return true;
}

// Function to generate a random prime in a given range
unsigned generateRandomPrime(unsigned min, unsigned max) {
    unsigned candidate;
    do {
        candidate = min + rand() % (max - min);
    } while (!isPrime(candidate));
    return candidate;
}


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
vector2 randomGradient(int ix, int iy){

	//no precomputed gradient means this works for any number of grid coords
	const unsigned w = 8 * sizeof(unsigned);
	const unsigned s = w / 2;
	unsigned a = ix, b = iy;
	//a *= 3284157443;
	a *= generateRandomPrime(1000000000, 4294967295); // Use a randomly generated prime


	b ^= a << s | a >> w - s;
	//b *= 1911520717;
	b *= generateRandomPrime(1000000000, 4294967295); 

	a ^= b << s | b >> w - s;
	//a *= 2048419325;
	a *= generateRandomPrime(1000000000, 4294967295);

	float random = a * (3.14159265 / ~(~0u >> 1)); //in [0, 2*Pi]
	
	// Create the vector from the angle 
	vector2 v;
	v.x = sin(random);
	v.y = cos(random);

	return v;
}


//compute the dot product of the distance and gradient vectors
float dotGridGradient(int ix, int iy, float x, float y){
	//get gradient from interger coords
	vector2 gradient = randomGradient(ix, iy);
	
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
float perlin(float x, float y){
	
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
	float n0 = dotGridGradient(x0, y0, x, y);
	float n1 = dotGridGradient(x1, y0, x ,y);
	float ix0 = interpolate(n0, n1, sx);

	//compute and interpolate bottom two corners
	n0 = dotGridGradient(x0, y1, x, y);
	n1 = dotGridGradient(x1, y1, x ,y);
	float ix1 = interpolate(n0, n1, sx);

	//Final step: interpolate between the two previously interpolated values, now in y
	float value = interpolate(ix0, ix1, sy);



	return value;
}


int main() {

	const int windowWidth = 1920;
	const int windowHeight = 1080;

	const int GRID_SIZE = 400;

	srand(static_cast<unsigned>(time(0))); // Seed the random number generator

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
				val += perlin(x * freq / GRID_SIZE, y * freq / GRID_SIZE) * amp;
				
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
