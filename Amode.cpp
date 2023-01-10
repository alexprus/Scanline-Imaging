#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <cmath>
#include <algorithm>
using namespace std;

float **createDataMatrix(int numElement, int numSample)
{
    // Create a 2D data matrix of size numElement and numSample

    
    float **RFData; // Initialize double pointer, RFData

    RFData = new float*[numElement];

    for (int i = 0; i <numElement; i++){
        RFData[i] = new float[numSample];
    }

    return RFData;
}

int loadRFData(float **RFData, const char *fileName, int numElement, int numSample)
{
    // Open the text file fileName, read the data and store into RFData
    // You can use the getline() command to extract the data lines from the txt files

    ifstream inFile;

    inFile.open(fileName); // Open the "fileName" file using ifstream

    char temp[18]; // Create a temporary variable 


    if (inFile.fail()){
        return -1; // Program runs an error if the file does not open properly
    }

    for(int i = 0; i < numElement; i++){

        for( int subi = 0; subi < numSample; subi++){

            inFile.getline(temp, 20); 
            RFData[i][subi] = atof(temp); 
        }

    }

    return 0;
}

// Create an array containing the depth location (in z-direction) for each pixel on the scanline

float *genScanlineLocation(int &numPixel) // Generates scanline depth locations
{
    float depth;
    cout << "How deep should the scan line be?" << endl; // User prompt
        cin >> depth; // User input for the depth of the scanline

    cout << "How many pixels should the scan line be?" << endl;  // User prompt
        cin >> numPixel; // User input for the amount of pixels present in the scanline

    float *scanlineLocation = new float[numPixel]; // Array, scanlineLocation based on user input

    for (int i = 0; i < numPixel; i++){

        // Creates the spacing between points

        float spacing; 
        spacing = (depth/(numPixel - 1));
        scanlineLocation[i] = spacing*i;
    }


    return scanlineLocation; 
}

// Create an array containing the element location (in x-direction) of the ultrasound transducer
float *genElementLocation(int numElement, float PITCH){

float *eleLocation = new float[numElement]; // Creates array, eleLocation

   for (int i = 0; i < numElement; i++){

        eleLocation[i] = ((i - ((float)numElement-1)/2))*PITCH; 
       
   }

   return eleLocation;
}

// Allocate memory to store the beamformed scanline

float *createScanline(int numPixel)
{
    
    float *scanLine = new float[numPixel]; // New array, scanLine, stores results from beamform

    return scanLine; 

}

// Beamform the A-mode scanline
void beamform(float *scanline, float **realRFData, float **imagRFData, float *scanlinePosition, float *elementPosition, int numElement, int numSample, int numPixel, float FS, float SoS)
{
    // Initialize float variables for equations needed for beamforming

    float tforward = 0;
    float tback = 0;
    float ttotal = 0;
    int tsample = 0;
    float Preal = 0;
    float Pimag = 0; 
    
for (int i = 0; i < numPixel; i++){

    tforward = (scanlinePosition[i])/SoS; // tforward, tback, and ttotal are related to the time of flight of the ultrasound wave

    Preal = 0;
    Pimag = 0;

    for (int k = 0; k < numElement; k++){

    tback = sqrt((pow(scanlinePosition[i], 2))+(pow(elementPosition[k], 2)))/SoS;

    ttotal = tforward + tback;

    tsample = floor(ttotal*FS); // sampling frequency which data was sampled at

    if (tsample < numSample){

    Preal = Preal + realRFData[k][tsample];
    Pimag = Pimag + imagRFData[k][tsample];

    }

    cout << Preal << "," << Pimag << endl;

    scanline[i] = sqrt((pow(Preal, 2))+(pow(Pimag, 2)));
    

}

}
}

// Write the scanline to a csv file
int outputScanline(const char *fileName, float *scanlinePosition, float *scanline, int numPixel)
{

// Outputs a new file 

ofstream outFile;
outFile.open(fileName);

if(outFile.fail()){

    cout << "Error generated" << endl;
    return -1; // If outFile fails, there is an error generated
}

for(int i = 0; i < numPixel; i++){

    outFile << scanlinePosition[i] << "," << scanline[i] << endl;
    cout << scanlinePosition[i] << "," << scanline[i] << endl; // Outputs a coordinate

}

outFile.close(); 

    return 0;
}

// Destroy all the allocated memory, all pointers free up space in memory
void destroyAllArrays(float *scanline, float **realRFData, float **imagRFData, float *scanlinePosition, float *elementPosition, int numElement, int numSample, int numPixel)
{

    // Delete arrays
    delete scanline;
    delete scanlinePosition;
    delete elementPosition;

    for (int i = 0; i < numElement; i++){ // Delete arrays: 2D arrays realRFData and imagRFData

        delete realRFData[i];
        delete imagRFData [i];

    }

    delete realRFData;
    delete imagRFData;
}