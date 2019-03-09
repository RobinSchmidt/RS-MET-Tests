/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic startup code for a Juce application.

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"


std::vector<double> loadSample(const std::string& name, double* sampleRate = nullptr)
{
  std::string sampleDir = "../../../../Data/Samples/";
  std::string path      = sampleDir + name + ".wav";

  int numChannels = 0, numFrames = 0, iSampleRate = 0;
  double** data = rosic::readFromWaveFile(path.c_str(), numChannels, numFrames, iSampleRate);

  if(sampleRate != nullptr)
    *sampleRate = (double) iSampleRate;

  std::vector<double> v;

  // copy data to vector, clean up memory..


  return v;
}



//==============================================================================
int main (int argc, char* argv[])
{

  std::vector<double> x;
  double fs;

  loadSample("bell_2a", &fs);


  // todo: check for memleaks (maybe move the memleak checking code to rosic)

    
  return 0;
}
