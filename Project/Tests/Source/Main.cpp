/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic startup code for a Juce application.

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"

using namespace RAPT;
using namespace rosic;

// move to TestTools.cpp...maybe rename wo waveLoad or wavRead...maybe have wavWrite too, maybe 
// move to rosic..or - no - this function is specific to this project (it uses the project-specific
// sample path - but mayb factor out an rsWavRead that takes a path as argument (and not just a 
// name)
std::vector<double> loadSample(const std::string& name, double* sampleRate = nullptr)
{
  // load sample data into temporary buffer:
  std::string sampleDir = "../../../../Data/Samples/";
  std::string path      = sampleDir + name + ".wav";
  int numChannels = 0, numFrames = 0, iSampleRate = 0;
  double** data = readFromWaveFile(path.c_str(), numChannels, numFrames, iSampleRate);

  // assign sample-rate, if variable is passed:
  if(sampleRate != nullptr)
    *sampleRate = (double) iSampleRate;

  // copy data into std::vector and clean up memory:
  std::vector<double> v(numFrames);
  RAPT::rsArray::copyBuffer(data[0], &v[0], numFrames);
  MatrixTools::rsDeAllocateMatrix(data, numChannels, numFrames);
  return v;

  // todo: rename rosic::rsArray ...maybe to rsDynamicArray..or remove the class from rosic or 
  // replace by RAPT class
}


void testHarmonicResynthesis(const std::string& name)
{
  double fs;
  std::vector<double> x = loadSample(name, &fs);



  int dummy = 0;
}



// todo: 
// -move HarmonicAnalyzer and SinusoidalSynthesizer to RAPT
// -maybe create an rs_tests juce module that contains code tha is needed here and in the test
//  project of the main RS-MET repo


//==============================================================================
int main (int argc, char* argv[])
{

  std::vector<double> x;
  double fs;

  x = loadSample("bell_2a", &fs);

  testHarmonicResynthesis("bell_2a");



  // todo: check for memleaks (maybe move the memleak checking code to rosic)

    
  return 0;
}
