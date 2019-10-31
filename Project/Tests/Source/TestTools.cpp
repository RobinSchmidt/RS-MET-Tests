#include "../JuceLibraryCode/JuceHeader.h"
using namespace RAPT;
using namespace rosic;

// ...maybe rename to waveLoad or wavRead...maybe have wavWrite too, maybe 
// move to rosic..or - no - this function is specific to this project (it uses the project-specific
// sample path - but mayb factor out an rsWavRead that takes a path as argument (and not just a 
// name)
std::vector<double> loadSample(const std::string& name, double* sampleRate = nullptr)
{


  // load sample data into temporary buffer:
  int numChannels = 0, numFrames = 0, iSampleRate = 0;
  double** data = nullptr;

  /*
  // old - only one sample directory supprted:
  std::string sampleDir = "../../../../Data/Samples/";
  std::string path      = sampleDir + name + ".wav";
  data = readFromWaveFile(path.c_str(), numChannels, numFrames, iSampleRate);
  if(data == nullptr) {
  rsError("File not found");
  return std::vector<double>();
  }
  */

  // new - multiple sample directories supported:
  std::vector<std::string> sampleDirs 
    = { "../../../../Data/Samples/", 
    "../../../../Data/Samples/Elan/",
    "../../../../Data/Samples/Misc/"};
  for(size_t i = 0; i < sampleDirs.size(); i++) {
    std::string path = sampleDirs[i] + name + ".wav";
    data = readFromWaveFile(path.c_str(), numChannels, numFrames, iSampleRate);
    if(data != nullptr)
      break;   // file was found in current directory
  }
  if(data == nullptr) {
    rsError("File not found or invalid");
    // todo: give two separate error messages for the two conditions (->figure out, if the file 
    // exists - if so, it's an unsupported format)
    return std::vector<double>();
  }

  // assign sample-rate, if variable is passed:
  if(sampleRate != nullptr)
    *sampleRate = (double) iSampleRate;

  // copy data into std::vector and clean up memory:
  std::vector<double> v(numFrames);
  RAPT::rsArray::copy(data[0], &v[0], numFrames);
  MatrixTools::rsDeAllocateMatrix(data, numChannels, numFrames);
  return v;

  // todo: rename rosic::rsArray ...maybe to rsDynamicArray..or remove the class from rosic or 
  // replace by RAPT class
}