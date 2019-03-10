//#define RS_PLOTTING

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


template<class T>
inline std::vector<T> rsExtractRange(std::vector<T>& v, size_t first, size_t last)
{
  rsAssert(last >= first && last < v.size(), "invalid range");
  size_t N = last - first + 1;
  std::vector<T> r(N);
  RAPT::rsArray::copyBuffer(&v[first], &r[0], (int) N);
  return r;
}


void testHarmonicResynthesis(const std::string& name)
{
  double fs;
  std::vector<double> x = loadSample(name, &fs);

  //x = rsExtractRange(x, 0, 20000); // analyze only a portion

  testHarmonicResynthesis(name, x, fs, true, false); 
  // plotting large wavefiles is not advisable


  int dummy = 0;
}

//-------------------------------------------------------------------------------------------------

int main (int argc, char* argv[])
{
  // maybe try some sample with vibrato...

  /*
  // sounds for which harmonic resynthesis works well:
  testHarmonicResynthesis("Vla_CE.L (2)");
  testHarmonicResynthesis("flute-C-octave0");
  testHarmonicResynthesis("flute-C-octave1");
  testHarmonicResynthesis("flute-C-octave2");
  */

  // sounds for which harmonic resynthesis doesn't work:


  //testHarmonicResynthesis("violin_bounce1");
  // this hits an assert


  //testHarmonicResynthesis("string-singlebow3");


  //testHarmonicResynthesis("guitar-ac-E-octave0");
  // takes long to compute (the synthesis takes long), shows buzzing artifacts
  // i think, the cycle-marks may be wrong? ...hmm - but pitch-flattened version looks good (when
  // taking only samples 0.20000, at least) - maybe the synthesis is to blame? -> plot the model
  // the cycle-marks are at half-cycles - so we actually analyze each half-cycle separately - that
  // could plausibly lead to the buzzing: because both half-cycles are different, the partial 
  // amplitudes and phases alternate between the values for each half-cycle
  // -> improve the fundamental frequency estimation algorithm and/or allow the user to set the 
  // fundamental frequency (the freq to which the bandpass in the cycle-mark finder is tuned)
  // ..actually, the pitch-flattened version is also wrong already in later sections of the signal
  // -> plot cycle-marks for these problematic sections, maybe write the extracted fundamental
  // on which the cycle-marks are based into a file, too


  // the anti-alias algo, if used, removes everything above the 5th harmonic - why would it remove 
  // valid harmonic content? maybe some isolated cycles get stretched a lot more than others,
  // causing the harmonic to be removed? maybe have different sorts of anti-alias options
  // some of the subharmonic (at 500 Hz) and its harmonics show up in the resynthesized signal.
  // maybe the subharmonic shows up as alternating DC? removing the DC component reduces the 500Hz
  // component but does not eliminate it completely. looking at the model data, it seems like there 
  // is some frequency modulation present at the start of the first harmonic - maybe this frequency
  // modulation somehow "encodes" the 500Hz component? ..there's also amplitude modulation - we 
  // need some plots...


  //testHarmonicResynthesis("bell_light_1");
  // very bad artifacts - cycle-lengths alternate between 40 and 50 cycles - but it's an inharmonic
  // spectrum (lowest freq around 1 kHz) ..maybe try more harmonic sounds first..


  //testHarmonicResynthesis("bell_2a");
  // we get a crash with this - it seems to be not safe to assume that the initial and final 
  // sections are shorter than one cycle...well...in particular, the final section...maybe 
  // when computing the maxLength, we cannot just use the differences of the cycle-marks but
  // must prepend 0 and append N-1 and *then* take the difference and find the maximum
  // ..nope - that doesn't help - the final section is longer than the blockSize *after* 
  // stretching 
  // scenario: last full cycle is very short and final incomplete cycle very long - now the
  // final incomplete cycle gets stretched by the factor determined by the short cycle, i.e.
  // it gets longer - it may happen that it becomes longer than the blockSize - the same is true
  // for the first full cycle when there's a long incomplete one before it.



  // todo: check for memleaks (maybe move the memleak checking code to rosic)

    
  return 0;
}
