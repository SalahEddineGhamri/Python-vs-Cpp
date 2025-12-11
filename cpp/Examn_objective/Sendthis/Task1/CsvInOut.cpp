// Implementation file for CsvClass -Task1App
// Author: Salah Eddine Ghamri
//==============================================================================
#include "CsvInOut.hpp"
//==============================================================================

// CsvClass Constructor & Destructor
CsvClass::CsvClass() {}
CsvClass::~CsvClass() {}

void CsvClass::ReadData(std::string InputFilePath, char Delim) {
    //To Read from a file. It takes the file path and the delimiter character.
    std::fstream InputFile(InputFilePath, std::ios::in);
    if (InputFile.is_open()) {
        printf("Input file is opened.\n");

        std::string line, word;
        std::vector<double> row;
        std::stringstream linestream;

        while (getline(InputFile, line)) {
            linestream.str(line);
            while (std::getline(linestream, word, Delim)) {
                row.push_back(std::stod(word));
            }
            this->Data.push_back(row); //refering to the class variable
            row.clear();
            linestream.clear();
        }
        InputFile.close();
    } else {
        printf("Error opening Input file.\n");
    }
}

Array CsvClass::GetData(){
    //A getter for Data variable
    return this->Data;
}

void CsvClass::WriteData(Array data, std::string FilePath, char Delimiter){
    //To Write to a file, it takes file path and the delimiter character.
    std::fstream OutputFile(FilePath, std::ios::out);
    char EndLine;

    if (OutputFile.is_open()) {
        printf("Writing to output file.\n");
        for (int i = 0; i < data.size(); ++i) {
        for (int j = 0; j < data[i].size(); ++j){
            EndLine = (j == data[i].size() - 1) ? '\n':Delimiter;
            OutputFile << data[i][j] << EndLine;
            }
        }
    } else {
        printf("Error in opening output file or in creating it.");
    }
}

Array CsvClass::FilterData(){
    // Applies a filter to eliminate Zero values.
    // Interpolation of correct values is based on a median filtering.

    Array FData = this -> Data;
    std::vector<double> Window; // Sliding window m x n
    int MaxM, MinM, MaxN, MinN; // Sliding window limits
    std::vector<std::pair<int, int> > ZStack; // A stack for bad values indexes
    double MedValue = 0.0;
    int mid; // Index of median value

    //General loop to iterate all array elements
    for (int i = 0; i < FData.size(); ++i) {
    for (int j = 0; j < FData[i].size(); ++j) {

    // Calculating the limits the sliding window
    MaxM = (i + 2 < FData.size()) ? i + 2 : FData.size();
    MinM = (i - 1 >= 0) ? i - 1 : 0;
    MaxN = (j + 2 < FData[i].size()) ? j + 2 : FData[i].size();
    MinN = (j - 1 >= 0) ? j - 1 : 0;

    // Clear Zero values stack
    ZStack.clear();

    // We check each array element
    // We collect all of its neighbors
    for ( int m = MinM; m < MaxM; ++m ) {
    for ( int n = MinN; n < MaxN; ++n ) {
        if ( FData[m][n] == 0 ){
            // Stack bad values indexes
            ZStack.emplace_back(m, n);
            }
        Window.push_back(FData[m][n]);
        }
    }

    // calculate mediane =======================================================
    // Sorting half of the Window elements is enough:
    mid = (Window.size() + 1)/2;
    for (int e = 0; e <= mid ; ++e)
    {
        int min = e;
        for (int k = e + 1; k < Window.size(); ++k)
        if (Window[k] < Window[min])
            min = k;
        const double temp = Window[e];
        Window[e] = Window[min];
        Window[min] = temp;
    }

    // Median value ============================================================
    if ( Window.size() % 2 != 0 ) {
        // if impaire take the middle value.
        MedValue = Window[mid];
    } else {
        // else take the mean of the middle values.
        MedValue = (Window[mid-1] + Window[mid])/2;
    }
    //==========================================================================

    // If there are bad values, replace them.
    if ( ZStack.size() != 0 ) {
        for (std::pair<int, int> &ZS : ZStack)
        FData[ZS.first][ZS.second] = MedValue;
        }
    // clear sliding window
    Window.clear();
    }} // End general loop

    return FData;
}
