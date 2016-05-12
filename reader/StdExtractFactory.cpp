/*
 * Collision-Code
 * Free software to calculate collision cross-section with Helium.
 * Université de Rouen
 * 2016
 *
 * Anthony BREANT
 * Clement POINSOT
 * Jeremie PANTIN
 * Mohamed TAKHTOUKH
 * Thomas CAPET
 */

#include "StdExtractFactory.h"

#include "MolFileReader.h"
#include "PdbFileReader.h"
#include "LogFileReader.h"
#include "XyzFileReader.h"
#include "MfjFileReader.h"

std::string getFileExt(const std::string& s);

StdExtractFactory::StdExtractFactory()
{

}

StdExtractFactory::~StdExtractFactory()
{

}

FileReader* StdExtractFactory::getReader(std::string fileName) {
  FileReader* fR = nullptr;
  std::string extension = getFileExt(fileName);

  if (extension == "mol") {
    fR = new MolFileReader(fileName);
  } else if (extension == "pdb") {
    fR = new PdbFileReader(fileName);
  } else if (extension == "mfj") {
    fR = new MfjFileReader(fileName);
  } else if (extension == "log" || extension == "out") {
    fR = new LogFileReader(fileName);
  } else if (extension == "xyz") {
    fR = new XyzFileReader(fileName);
  }

  return fR;
}

std::string getFileExt(const std::string& s) {

   size_t i = s.rfind('.', s.length());
   if (i != std::string::npos) {
      return(s.substr(i+1, s.length() - i));
   }

   return("");
}
