#include "TestTools.cpp"

/*
ToDo:
-rsPartialBeatingRemover<T>::removeAmplitudeBeating - let the user set setMaxEnvelopeSampleSpacing
 ...or provide meaningful defaults
-introduce a multiplier > 1 for the maxSpacing
-debeating has still problems when the amplitude is mis-estimated as zero - but this mis-estimation
 is a problem, we need to fix anyway
 better: use the maximum distance between found peaks as the minimum distance between de-beated
 env datapoints
-maybe factor out a meta-envelope extractor
-the debeating may sometimes introduce a sort of glitch at the transient - maybe this is due to 
 filtering the phases?

-figure out, why the analysis may produce negative amplitudes for the DC component

-figure out why there's a glitch at the end with debeating the rhodes sample
 -it also occurs when analyzing and resynthesizing only the DC component and it's not present in 
  the output synthesized from the unmodified model -> bug must be in the de-beater
-figure out why the debeating with Rhodes Tuned F3 V12TX -16.4 10-17-16 triggers the assertion
 for the invalid sinusoidal model - do the amplitudes become negative? maybe print debug info in
 the SinusoidalPartial::isDataValid
 ->the debeater produces a nan value for the amplitude last datapoint of the DC component
 ->it seems rsEnvelopeExtractor<T>::setupEndValues appends a last datapoint with the same 
   time-stamp as the second-to-last - this leads to a division by zero whe trying to interpolate
   ->seems fixed
*/


//-------------------------------------------------------------------------------------------------

int main (int /*argc*/, char* /*argv[]*/)
{

  // todo: 
  // -figure out why we get artifacts with the blackman window
  // -allow arbitrary cycleLength
  // -maybe allow each cycle to have its own length - bypass the time-warping
  // -plot the residual with the cycle-marks for sounds that show the buzz - maybe the buzz 
  //  impulses are at the cycle-marks - if so, that could be an important hint - yep - that
  //  seems to be indeed the case. or hmm - they are *close* to the marks but sometimes slightly
  //  off - maybe that is also a hint?
  // -could is be that at the marks, all partials "conspire" to produce an edge - this seems 
  //  plausible because at those time-instants, the phase is exactly defined by the datapoints 
  //  whereas in between, it is determined by what the interpolation produces..but that would be
  //  weird because in the original signal, there is no such edge
  //  ...well..actually - no - the time-stamps of the datapoints are actually in between the cycle
  //  marks - so at the cycle-marks, we are halfway between two datapoints
  // -switching the phase-interpolation algo in the synthesizer does not help, switching amplitude
  //  interpolation to cubic also doesn't help
  // -it seems to be the case that at the datapoints (i.e. in between the marks) the aggreement
  //  between original and resynthesized is best and at the marks (in between the datapoints) it
  //  is worst - which makes sense because in between the datapoints, the signal is determined
  //  maximally by the interpolation scheme
  // -maybe we need more datapoints to avoid phase-jumps? for example, we could use marks based
  //  on upward and downward zero crossings
  // -maybe at the marks, sometimes a phase jump occurs for certain partials because the wrapped
  //  interpolation of the phase selects different branches before and after the mark?
  // -try different phase and amplitude interpolation schemes and/or introduce bidiractional 
  //  smoothing filters
  // -the f0 trajectories show spikes at the ends - maybe we need a bandpass filter that rings 
  //  longer for isolating the fundamental - make the cycle-mark-finder settings available to user
  //  code - check, if we actually use the f0-zero-crossing algorithm
  // -allow the user to set the fundamental frequency
  // -improve frequency detection
  // maybe try some sample with vibrato...



  // sounds for which harmonic resynthesis works:

  //testHarmonicResynthesis("Vla_CE.L (2)");
  // Hamming leaves some harmonic content in the residual, with Blackman it's purely noisy


  //testHarmonicResynthesis("flute-C-octave0");

  //testHarmonicResynthesis("flute-C-octave1");
  // When passing the signal to an aggressive lowpass before analysis/resynthesis, the 
  // resynthesized signal will nevertheless show high-freq content in the area that has been 
  // removed. This is not due to the sinc-interpolator (the pitch-flattened signal looks fine) and
  // also not due the synthesis (removing partials above the lowpass cutoff removes the artifacts)
  // so it must indeed be the case that the analyzer sees nonzero amplitudes where the amplitude
  // should be zero ...could it be that the frequency modulation due to pitch-flattening produces
  // these? ...but no: then they should be visible in the pitch-flattened signal already
  // it's also interesting to note that these artifacts don't occur when the input is a sum
  // of the first 10 harmonics of a sawtooth wave - that rules out a bug in the FFT computation (or
  // makes it unlikely)
  // todo: plot the short-time spectra that occur during analysis (maybe on dB scale)
  // ...hmm...i'm not so sure anymore if the sinc-interpolator can be ruled out - try a different
  // window function
  // i now think, it's the amplitude modulation present on the lower partials that shows up as 
  // spurious harmonics - if the amplitude of a lower harmonic changes during the course of a 
  // cycle, we get spurious higher harmonics - maybe that also can be remedied by using better
  // window functions (and longer segments)

  // with nc=4, zp=4, wt=bm, there are weird artifacts - could it be that a harmonic is recorded
  // twice sometimes and once at other times? ..oh - not it seems to switch between 0 and 1 times
  // setting T minPeakWidth = T(0.0); does not change anything - this means the decsision in
  // isPeakPartial seems not to be at fault
  // doing if(kPeak == -1 /* || kPeak == kPeakOld */ ) { also doesn't help
  // doing return kCenter; at the beginning of findPeakBinNear does help
  // give the user the options to totally bypass the peakBin search and use kHarm always
  // -works for the flute well and does not porduce false harmonics
  // is has to do with what getSpectralPeakSearchWidth returns - if the width is too wide, it may 
  // return a value form the edge which is then discarded...or something


  //testHarmonicResynthesis("flute-C-octave2");
  // the anti-alias algo, if used, removes everything above the 5th harmonic - why would it remove 
  // valid harmonic content? maybe some isolated cycles get stretched a lot more than others,
  // causing the harmonic to be removed? maybe have different sorts of anti-alias options
  // some of the subharmonic (at 500 Hz) and its harmonics show up in the resynthesized signal.
  // maybe the subharmonic shows up as alternating DC? removing the DC component reduces the 500Hz
  // component but does not eliminate it completely. looking at the model data, it seems like there 
  // is some frequency modulation present at the start of the first harmonic - maybe this frequency
  // modulation somehow "encodes" the 500Hz component? ..there's also amplitude modulation - we 
  // need some plots...


  //testHarmonicResynthesis("whistle_3");
  // resynthesized sounds "cleaned up", residual is some sort of noise
  // bandpass-settings: width=1.0, steepness=3, freq-range: 20-5000
  // needs a somewhat wider width due to the frequency sweep


  //testHarmonicResynthesis("femalevoice_aa_A3");
  // resynthesized sounds somwhat synthetic but okay but actually the original already has that
  // character, residual is really loud and weird - could there be phase errors in resynthesis?
  // when using only the first 20000 samples, it hits an assert, 10000 samples works
  // the residual looks like noise-pulses at the fundamental frequency. each pulse looks different
  // but they occur at a regular rate - could these be caused by the sinc-interpolation? 
  // -> changing the kernel length does not seem to make a big difference
  // -actually, that sort of residual (a train of noisy pulses) seems to be typical also for other
  //  input signals
  // -plot a de-trended phase-trajectory of one of the higher partials - maybe the decoherence is
  //  due to phase behaving erratically at higher partials?
  // -it seems, that the resynthesized phases are randomly off from the original phases - at most 
  //  at time instants farthest away from datapoints
  // -try different phase-interpolation methods (linear, hermite, quintic, etc.) - maybe it's the
  //  interpolation error?
  // -maybe, if the freq-estimates are far enough off the actual freqs, the (preliminary) computed
  //  phase (by freq-integration) will end up in wrong cycle (wrong multiple of 2pi) - no amount of
  //  re-adjustment of the phase can fix this - this would explain why there tends to be so much
  //  high-freq in the noise bursts - higher freqs cycle faster from frame to frame so they can 
  //  more easily end up in the wrong cycle..hmm...or do they? ...hmm...nope - that doesn't seem
  //  to be the case



  //testHarmonicResynthesis("malevoice_oo_A2");

  //testHarmonicResynthesis("malevoice_aa_A2");

  //testHarmonicResynthesis("string-singlebow3");
  // f0 is a mess, even when reducing the detection bandwidth - we really need to plot the 
  // extracted fundamental
  // ahh - but using CYCLE_CORRELATION works well





  // original frequency is around 520Hz (midi-key 72, 523.25...Hz):
  //double freq = rsPitchToFreq(72.0);
  //testMakeHarmonic("flute-C-octave1", 440.0, 0.0);  
  //testMakeHarmonic("flute-C-octave1", freq, 0.1);  
  // this also has many partials at zero amplitude that would actually alias - clean up! this also
  // will make the synthesis faster

  //testMakeHarmonic("flute-C-octave1", 220.0);
  // output too loud (clips) - due to new phase relationships between the partials
  // maybe normalize output? and/or do not obtain the reference phase from the beginning of the 
  // sample but rather from somewhere in the middle - avoid change of phase-relationships


  //testHarmonicResynthesis("Cello_C1");
  //testMakeHarmonic("Cello_C1", 65.0);

  //testHarmonicResynthesis("BassClarinet_Cs1");

  //testHarmonicResynthesis("Rhodes_F3_Short");
  //testHarmonicResynthesis("Rhodes_F3");

  //testModalResynthesis("Rhodes_F3_Short");
  // it seems like the 9th harmonic (index 8 after removing DC) has a much too low amplitude in the
  // resynthesized signal - there seems to be an estimation error - this harmonic seems to have a 
  // bimodal amp env, and i think, it is one where the analyzed attack is longer than the decay 
  // (such that the attack is artificially shortened) - i think, the decay may be too fast due to
  // bimodality? maybe, we should de-beat the amp-env before? ...yes, that's very plausible - try
  // amp-envelope de-beating before modal modeling

  //testDeBeating("BeatingSines");

  //testDeBeating("Rhodes_F3_Short");
  // for partial 3, the meta-envelope is too short in rsEnvelopeExtractor<T>::connectPeaks
  // too see it, activate plotting in rsEnvelopeExtractor<T>::fillSparseAreas
  // ->try to come up with an artificial test signal that exposes this behavior

  //testDeBeating("Rhodes_F3");


  //testDeBeating("Rhodes Tuned F3 V12TX -16.4 10-17-16 shorter", 175); 
  //testDeBeating("Rhodes Tuned F3 V12TX -16.4 10-17-16 short",   175); 
  //testDeBeating("Rhodes Tuned F3 V12TX -16.4 10-17-16",         175);  // long sample
  //testDeBeating("Rhodes Tuned F#3 V14TX -12.1 1-12-17");
  //testDeBeating("Rhodes Tuned F#3 V14TX -12.1 1-12-17",   184);


  //testDeBeating("Rhodes Tuned F#3 V10TX -20.5 1-12-17",   184);
  //testDeBeating("Rhodes Tuned F#3 V10TX -20.5 1-12-17_96kHz",   184);

  //Rhodes Tuned F#3 V10TX -20.5 1-12-17.wav


  // the long sample has a frequency glitch at the beginning
  // -pitch estimation finds 175 - well, it actually IS 175
  // -even if we pass 350, the lowes frew in the model is still at 175
  // -the isDataValid assert triggers after de-beating - apparently, the process may create data 
  //  that's invalid
  // see comments in rsPartialBeatingRemover<T>::removeAmplitudeBeating

  // with the (short) rhodes-sample, the 3rd partial seems to get too loud towards the end
  // -take a look at the amplitude envelopes after de-beating
  //  ->we get problems when the envelope is monotonically decreasing - there are too few 
  //    datapoints in the de-beated envelope - we get straight linear fade-outs when they should
  //    be exponential
  // -maybe the cutoff frequency for the phase-smoother should not be the same for all partials 
  //  maybe it should depend on the frequency of the partial
  // -it seems, the amp-env for the DC component of the rhodes has negative values - how is that 
  //  possible? i think the filtering process has overshoot leading to the amplitude undershooting
  //  zero - maybe we should use a filter that doesn't have any overshoot - or clip at zero
  //  -actually, the DC component already has negative values in the unmodified analysis data - but
  //   the assert does not trigger - why not? it should! because the loop starts at partial 1
  //   DC is not taken into account
  // -the cyan curve really goes below zero at the end - that's not only a filter overshoot








  // sounds for which harmonic resynthesis doesn't work:




  //testHarmonicResynthesis("vocal_buzz_1");
  // gets f0 totally wrong


  //testHarmonicResynthesis("violin_bounce1");
  // has buzzing artifact - also with cycle-correlation pitch detection
  // one buzz impulse is 2389-2390, next at 3306 - yep, these locations are very close to the found
  // cycle marks - i think, this buzz is due to inharmonicity




  //testHarmonicResynthesis("piano_E2");
  // garbage at the beginning (cycle-mark-errors) with CYCLE_CORRELATION, it works, but there are 
  // artifacts - i think, maybe partials switching on and off? try stricter ot looser criteria
  // for considering a peak a mainlobe
  // setAllowInharmonics(false): removes the artifacts
  // setMinPeakWidth(0.0): does not help against it - why?

  //testHarmonicResynthesis("guitar-ac-E-octave0", 82);
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
  // plotting the cycle-length trajectory shows that the cycle-detections fails badly
  // bandpass-settings: width=0.1, steepness=3, freq-range: 50-100 ..there's still buzz but much
  //  less with these settings
  // -setting the fundamental manually improves it - but there's still buzz





  //testHarmonicResynthesis("bell_light_1");
  // very bad artifacts - cycle-lengths alternate between 40 and 50 cycles - but it's an inharmonic
  // spectrum (lowest freq around 1 kHz) ..maybe try more harmonic sounds first..
  // fundamental at around 980Hz - detected at 450 with zero-crossings - better results with 
  // cycle-correlation - but the resynthesized sound sounds more dirty than the original - this
  // is in contrast to hamronic sounds which tend to sound cleaned up - maybe the harmonic 
  // analysis should not be used for inharmonic material


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


  //testEnvelopeMatching(
  //  "MutedMallets/(0096)DPan_MutedMalletsEast`n=D2`tail=3",
  //  "MutedMallets/(0105)DPan_MutedMalletsEast`n=D2`tail=1");

  //testEnvelopeMatching(
  //  "MutedMallets/(0096)DPan_MutedMalletsEast`n=D2`tail=3",
  //  "MutedMallets/(0096)DPan_MutedMalletsEast`n=D2`tail=3_1");

  //testEnvelopeMatching2(
  //  "MutedMallets/(0042)DPan_MutedMalletsNorth`n=D2`tail=3",
  //  "MutedMallets/(0036)DPan_MutedMalletsNorth`n=D2`tail=1");

  //testEnvelopeMatching2(
  //  "MutedMallets/(0042)DPan_MutedMalletsNorth`n=D2`tail=3",
  //  "MutedMallets/(0037)DPan_MutedMalletsNorth`n=D2`tail=1");

  testEnvelopeMatching2(
    "MutedMallets/(0042)DPan_MutedMalletsNorth`n=D2`tail=3",
    "MutedMallets/(0039)DPan_MutedMalletsNorth`n=D2`tail=1");

  //testEnvelopeMatching2(
  //  "MutedMallets/(0042)DPan_MutedMalletsNorth`n=D2`tail=3",
  //  "MutedMallets/(0034)DPan_MutedMalletsNorth`n=D2`tail=1");

  //testEnvelopeMatching2(
  //  "MutedMallets/(0042)DPan_MutedMalletsNorth`n=D2`tail=3",
  //  "MutedMallets/(0031)DPan_MutedMalletsNorth`n=D2`tail=1");
  

  //testHarmonicResynthesis("Twang");
  // maybe make a function visualizeSineComponents or: plotAmpEnvelopes(minPartial, maxPartial),
  // plotFreqEnvelopes(minPartial, maxPartial)


  // todo: check for memleaks (maybe move the memleak checking code to rosic)

    
  return 0;
}
