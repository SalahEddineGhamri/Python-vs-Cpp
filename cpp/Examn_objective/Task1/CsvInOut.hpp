// Header file of CsvClass - Task1App
// Author: Salah Eddine Ghamri
#ifndef CSVINOUT_HPP
#define CSVINOUT_HPP

//==============================================================================
// Included dependencies:
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
//==============================================================================
// Type definitions:
// We can use "using" too.
typedef std::vector< std::vector<double> > Array;
//==============================================================================

class CsvClass{
    Array Data;
 public:
     CsvClass();
     void ReadData(std::string FilePath, char Delimiter = ';');
     Array FilterData();
     void WriteData(Array data, std::string FilePath, char Delimiter = ';');
     Array GetData();
     ~CsvClass();
};

#endif // ifndef CSVINOUT_HPP
