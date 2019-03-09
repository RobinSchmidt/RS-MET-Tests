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
  testHarmonicResynthesis(name, x, fs, true, true);
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

  //x = loadSample("bell_2a", &fs);

  testHarmonicResynthesis("bell_light_1");
  // very bad artifacts - cycle-lengths alternate between 40 and 50 cycles - but it's an inharmonic
  // spectrum (lowest freq around 1 kHz) ..maybe try more harmonic sounds first..


  //testHarmonicResynthesis("bell_2a");
  // we get a crash with this - it seems to be not safe to assume that the initial and final 
  // sections are shorter than one cycle...well...in particular, the final section...maybe 
  // when computing the maxLength, we cannot just use the differences of the cycle-marks but
  // must prepend 0 and append N-1 and *then* take the difference and find the maximum
  // ..nope - that doesn'T help - the final section is longe than the blockSize *after* 
  // stretching 
  // scenario: last full cycle is very short and final incomplete cycle very long - now the
  // final incomplete cycle gets stretched by the factor determined by the short cycle, i.e.
  // it gets longer - it may happen that it becomes longer than the blockSize - the same is true
  // for the first full cycle whene there's a long incomplete one before it.



  // todo: check for memleaks (maybe move the memleak checking code to rosic)

    
  return 0;
}
