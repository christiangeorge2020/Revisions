#pragma once

#ifndef __FourWayBandSplitter__
#define __FourWayBandSplitter__

#include "fxobjects.h"

/**
\struct FourWayBandSplitterParameters
\ingroup FX-Objects
\brief
Custom parameter structure for the FourWayBandSplitter object.

\author <Christian George> <http://www.yourwebsite.com>
\remark <Put any remarks or notes here>
\version Revision : 1.0
\date Date : 2019 / 01 / 31
*/

enum class splitSelection {kSplitter, kLPF, kLowBand, kHighBand, kHPF};

struct FourWayBandSplitterParameters
{
	FourWayBandSplitterParameters() {}

	/** all FXObjects parameter objects require overloaded= operator so remember to add new entries if you add new variables. */
	FourWayBandSplitterParameters& operator=(const FourWayBandSplitterParameters& params)	// need this override for collections to work
	{
		// --- it is possible to try to make the object equal to itself
		//     e.g. thisObject = thisObject; so this code catches that
		//     trivial case and just returns this object
		if (this == &params)
			return *this;

		// --- copy from params (argument) INTO our variables
		lowSplit = params.lowSplit;
		midSplit = params.midSplit;
		highSplit = params.highSplit;

		splitView = params.splitView;
		
		// --- volume
		for (int i = 0; i < 4; i++)
			volume[i] = params.volume[i];

		dryVolume = params.dryVolume;

		// --- saturation
		for (int i = 0; i < 4; i++)
			saturation[i] = params.saturation[i];

		
		enableSplitter = params.enableSplitter;
		enableModFilter = params.enableModFilter;

		splitterBoost = params.splitterBoost;



		// --- MUST be last
		return *this;
	}

	// --- individual parameters
	double lowSplit = 400.0;
	double midSplit = 1000.0;
	double highSplit = 10000.0;

	splitSelection splitView = splitSelection::kSplitter;

	double volume[4];
	double dryVolume = 0.0;

	double saturation[4];


	bool enableSplitter = false;
	bool enableModFilter = false;

	double splitterBoost = 0.0;
};


/**
\class FourWayBandSplitter
\ingroup FX-Objects
\brief
The FourWayBandSplitter object implements ....

Audio I/O:
- Processes mono input to mono output.
- *** Optionally, process frame *** Modify this according to your object functionality

Control I/F:
- Use FourWayBandSplitterParameters structure to get/set object params.

\author <Your Name> <http://www.yourwebsite.com>
\remark <Put any remarks or notes here>
\version Revision : 1.0
\date Date : 2019 / 01 / 31
*/
class FourWayBandSplitter : public IAudioSignalProcessor
{
public:
	FourWayBandSplitter(void) {}	/* C-TOR */
	~FourWayBandSplitter(void) {}	/* D-TOR */

public:
	/** reset members t initialized state */
	virtual bool reset(double _sampleRate)
	{
		// --- store the sample rate
		sampleRate = _sampleRate;

		// --- do any other per-audio-run inits here

		for (int i = 0; i < 6; i++)
			splitterFilters[i].reset(sampleRate);

		return true;
	}

	/** process MONO input */
	/**
	\param xn input
	\return the processed sample
	*/
	virtual double processAudioSample(double xn)
	{
		// --- done
		return xn;
	}

	/** query to see if this object can process frames */
	virtual bool canProcessAudioFrame() { return true; } // <-- change this!

	/** process audio frame: implement this function if you answer "true" to above query */
	virtual bool processAudioFrame(const float* inputFrame,	/* ptr to one frame of data: pInputFrame[0] = left, pInputFrame[1] = right, etc...*/
		float* outputFrame,
		uint32_t inputChannels,
		uint32_t outputChannels)
	{
		double xn[2];

		xn[0] = inputFrame[0];
		xn[1] = inputFrame[1];

		double yn[2];

		
		// --- Filter Bank Outputs
		FilterBankOutput lowSplit[2];
		FilterBankOutput midSplit[2];
		FilterBankOutput highSplit[2];
		
		for (int i = 0; i < 2; i++)
		{
			lowSplit[i] = splitterFilters[i].processFilterBank(xn[0]);
			midSplit[i] = splitterFilters[i + 2].processFilterBank(lowSplit[i].HFOut);
			highSplit[i] = splitterFilters[i + 4].processFilterBank(midSplit[i].HFOut);

		}

		// --- Channel Bands
		double lpf[2];
		double lowBand[2];
		double highBand[2];
		double hpf[2];

		for (int i = 0; i < 2; i++)
		{
			lpf[i] = lowSplit[i].LFOut * volume_cooked[0];
			lowBand[i] = midSplit[i].LFOut * volume_cooked[1];
			highBand[i] = highSplit[i].LFOut * volume_cooked[2];
			hpf[i] = highSplit[i].HFOut * volume_cooked[3];
		}


		// --- Saturation
		for (int i = 0; i < 2; i++)
		{
			if (parameters.saturation[0] > 1)
				lpf[i] = tanh(lpf[i] * k[0]) / tanh(k[0]);
			
			if (parameters.saturation[1] > 1)
				lowBand[i] = tanh(lowBand[i] * k[1]) / tanh(k[1]);
			
			if (parameters.saturation[2] > 1)
				highBand[i] = tanh(highBand[i] * k[2]) / tanh(k[2]);
			
			if (parameters.saturation[3] > 1)
				hpf[i] = tanh(hpf[i] * k[3]) / tanh(k[3]);
			
		}
				
			

		// --- Dry Signal
		double dryInput[2];

		for (int i = 0; i < 2; i++)
		{
			dryInput[i] = (lowSplit[i].LFOut + lowSplit[i].HFOut
				+ midSplit[i].LFOut + midSplit[i].HFOut
				+ highSplit[i].LFOut + highSplit[i].HFOut)
				* dryVolume_cooked;
		}


		// --- Channel Split Output
		for (int i = 0; i < 2; i++)
		{
			if (parameters.splitView == splitSelection::kSplitter)
				yn[i] = (lpf[i] + lowBand[i] + highBand[i] + hpf[i]) * boostCooked + dryInput[i];
			
			else if (parameters.splitView == splitSelection::kLPF)
				yn[i] = lpf[i];
			
			else if (parameters.splitView == splitSelection::kLowBand)
				yn[i] = lowBand[i];
			
			else if (parameters.splitView == splitSelection::kHighBand)
				yn[i] = highBand[i];
			
			else if (parameters.splitView == splitSelection::kHPF)
				yn[i] = hpf[i];
			
		}

		
		// --- Mono
		if (inputChannels == 1 &&
			outputChannels == 1)
		{
			outputFrame[0] = yn[0];
			return true; /// processed
		}


		// --- Mono-In/Stereo-Out
		else if (inputChannels == 1 &&
				 outputChannels == 2)
		{
			outputFrame[0] = yn[0];
			outputFrame[1] = yn[0];

			return true; /// processed
		}

		// --- Stereo-In/Stereo-Out
		else if (inputChannels == 2 &&
				outputChannels == 2)
		{
			outputFrame[0] = yn[0];
			outputFrame[1] = yn[1];

			return true; /// processed
		}

		// --- do nothing
		return false; // NOT handled
	}


	/** get parameters: note use of custom structure for passing param data */
	/**
	\return FourWayBandSplitterParameters custom data structure
	*/
	FourWayBandSplitterParameters getParameters()
	{
		return parameters;
	}

	/** set parameters: note use of custom structure for passing param data */
	/**
	\param FourWayBandSplitterParameters custom data structure
	*/
	void setParameters(const FourWayBandSplitterParameters& params)
	{
		// --- copy them; note you may choose to ignore certain items
		//     and copy the variables one at a time, or you may test
		//     to see if cook-able variables have changed; if not, then
		//     do not re-cook them as it just wastes CPU
		parameters = params;

		

		// --- cook band volumes
		for (int i = 0; i < 4; i++)
			volume_cooked[i] = pow(10, parameters.volume[i] / 20);

		boostCooked = pow(10, parameters.splitterBoost / 20);

		// --- set range of dry volume
		if (parameters.dryVolume <= -15.0)
			dryVolume_cooked = 0.0;	
		else
			dryVolume_cooked = pow(10, parameters.dryVolume / 20);
		

		// --- Cook Saturation
		for (int i = 0; i < 4; i++)
			k[i] = parameters.saturation[i];
		
	
		
		LRFilterBankParameters bankParams = splitterFilters[0].getParameters();

		 // --- Low Split
		if (parameters.lowSplit >= parameters.midSplit)
			parameters.lowSplit = parameters.midSplit;

		// Frequency
		bankParams.splitFrequency = parameters.lowSplit;
		splitterFilters[0].setParameters(bankParams);
		splitterFilters[1].setParameters(bankParams);

		// Mid Split
		if (parameters.midSplit <= parameters.lowSplit)
			parameters.midSplit = parameters.lowSplit;
		else if (parameters.midSplit >= parameters.highSplit)
			parameters.midSplit = parameters.highSplit;

		bankParams.splitFrequency = parameters.midSplit;
		splitterFilters[2].setParameters(bankParams);
		splitterFilters[3].setParameters(bankParams);

		// High Split
		if (parameters.highSplit <= parameters.midSplit)
			parameters.highSplit = parameters.midSplit;

		bankParams.splitFrequency = parameters.highSplit;
		splitterFilters[4].setParameters(bankParams);
		splitterFilters[5].setParameters(bankParams);

	}


private:
	FourWayBandSplitterParameters parameters; ///< object parameters

	LRFilterBank splitterFilters[6];

	double volume_cooked[4];
	double dryVolume_cooked = 0.0;

	double k[4];

	double boostCooked = 0.0;

	// --- local variables used by this object
	double sampleRate = 0.0;	///< sample rate

};

#endif