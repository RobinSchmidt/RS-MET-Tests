#include "../JuceLibraryCode/JuceHeader.h"
using namespace RAPT;
using namespace rosic;


// todo: use https://en.cppreference.com/w/cpp/filesystem/path (this is a C++17 feature)


std::string getRootSampleDirectory()
{
  return std::string("../../../../Data/Samples/");
}

std::vector<std::string> getSampleDirectories()
{
  std::string rootSampleDir = getRootSampleDirectory();
  std::vector<std::string> sampleDirs;
  sampleDirs.push_back(rootSampleDir);
  sampleDirs.push_back(rootSampleDir + "Elan/");
  sampleDirs.push_back(rootSampleDir + "Misc/");
  return sampleDirs;
}

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
  //std::vector<std::string> sampleDirs 
  //  = { "../../../../Data/Samples/", 
  //  "../../../../Data/Samples/Elan/",
  //  "../../../../Data/Samples/Misc/"};

  std::vector<std::string> sampleDirs = getSampleDirectories();
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


template<class T>
inline std::vector<T> rsExtractRange(std::vector<T>& v, size_t first, size_t last)
{
  rsAssert(last >= first && last < v.size(), "invalid range");
  size_t N = last - first + 1;
  std::vector<T> r(N);
  RAPT::rsArray::copyBuffer(&v[first], &r[0], (int) N);
  return r;
}
// maybe move to rapt or rosic

/** Analyzes and resynthesizes the wavefile with given name and write the results into 
wavefiles. you may optionally pass a fundamental frequency - if none is passed, it will be 
auto-detected. */
void testHarmonicResynthesis(const std::string& name, double f0 = 0)
{
  double fs;
  std::vector<double> x = loadSample(name, &fs);

  //x = rsExtractRange(x, 0, 15000); // analyze only a portion
  bool plot = false;
  // plot = x.size() <= 20000;   // plotting large wavefiles is not advisable

  // to test the creation of false harmonics, we aggressively lowpass the audio at 5 kHz - any
  // content above that in the resynthesized signal then has to be considered to be artifacts:
  //RAPT::rsBiDirectionalFilter::applyButterworthLowpass(
  //  &x[0], &x[0], (int)x.size(), 2000.0, fs, 20);
  //RAPT::rsArray::normalize(&x[0], (int) x.size(), 1.0);

  testHarmonicResynthesis(name, x, fs, f0, true, plot); 
}

void testMakeHarmonic(const std::string& name, double targetFundamental, 
  double inharmonicity = 0, double originalFundamental = 0)
{
  double fs;
  std::vector<double> x = loadSample(name, &fs);
  testMakeHarmonic(name, x, fs, targetFundamental, inharmonicity, originalFundamental);
}

void testModalResynthesis(const std::string& name, double f0 = 0)
{
  double fs;
  std::vector<double> x = loadSample(name, &fs);
  testModalResynthesis(name, x, fs, f0); 
}

void testDeBeating(const std::string& name, double f0 = 0)
{
  double fs;
  std::vector<double> x = loadSample(name, &fs);
  testDeBeating(name, x, fs, f0); 
}

void testEnvelopeMatching(const std::string& name1, const std::string& name2)
{
  double fs;
  std::vector<double> x1 = loadSample(name1, &fs);
  std::vector<double> x2 = loadSample(name2, &fs);
  testEnvelopeMatching(x1, x2);
}

void testEnvelopeMatching2(const std::string& name1, const std::string& name2)
{
  double fs;
  std::vector<double> x1 = loadSample(name1, &fs);
  std::vector<double> x2 = loadSample(name2, &fs);
  testEnvelopeMatching2(x1, x2);
}

void plotPhaseTrajectories(const std::string& name, std::vector<int> indices)
{
  double fs;
  std::vector<double> x = loadSample(name, &fs);
}
