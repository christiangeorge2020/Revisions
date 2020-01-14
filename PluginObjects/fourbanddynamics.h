#pragma once

#ifndef __FourBandDynamics__
#define __FourBandDynamics__

#include "fxobjects.h"

/**
\struct FourBandDynamicsParameters
\ingroup FX-Objects
\brief
Custom parameter structure for the FourBandDynamics object.

\author <Christian George> <http://www.yourwebsite.com>
\remark <Put any remarks or notes here>
\version Revision : 1.0
\date Date : 2019 / 01 / 31
*/


enum class modeSelection {kCompressor, kExpander};
enum class msSelection {kSummed, kMid, kSide};

struct FourBandDynamicsParameters
{
	FourBandDynamicsParameters() {}

	/** all FXObjects parameter objects require overloaded= operator so remember to add new entries if you add new variables. */
	FourBandDynamicsParameters& operator=(const FourBandDynamicsParameters& params)	// need this override for collections to work
	{
		// --- it is possible to try to make the object equal to itself
		//     e.g. thisObject = thisObject; so this code catches that
		//     trivial case and just returns this object
		if (this == &params)
			return *this;

		// --- copy from params (argument) INTO our variables
		dryVolume = params.dryVolume;

		// --- split frequency
		for (int i = 0; i < 3; i++)
			splitF[i] = params.splitF[i];
		
		// --- threshold
		for (int i = 0; i < 6; i++)
		{
			threshold[i] = params.threshold[i];
			ratio[i] = params.ratio[i];
			attack[i] = params.attack[i];
			release[i] = params.release[i];
			gain[i] = params.gain[i];
			knee[i] = params.knee[i];

			hardLimitGate[i] = params.hardLimitGate[i];
			dynamicsMode[i] = params.dynamicsMode[i];

			bypass[i] = params.bypass[i];

			enableMute[i] = params.enableMute[i];
			enableSolo[i] = params.enableSolo[i];
			saturation[i] = params.saturation[i];

			inputMeter[i] = params.inputMeter[i];
			outputMeter[i] = params.outputMeter[i];
			reductionMeter[i] = params.reductionMeter[i];

			scTarget[i] = params.scTarget[i];

		}
		

		enableMS = params.enableMS;
		enableSidechain = params.enableSidechain;
		msView = params.msView;

		masterOutputVolume = params.masterOutputVolume;
		

		// --- MUST be last
		return *this;
	}

	// --- split frequencies
	double splitF[3];

	double threshold[6];
	double ratio[6];
	double attack[6];
	double release[6];
	double gain[6];
	double knee[6];

	bool hardLimitGate[6] = { false, false, false, false };
	modeSelection dynamicsMode[6];

	bool enableMute[6] = { false, false, false, false };
	bool enableSolo[6] = { false, false, false, false };

	double dryVolume = 0.0;

	double bypass[6];

	double saturation[6];

	float inputMeter[6];
	float outputMeter[6];
	float reductionMeter[6];
	float masterInputMeter = 0.f;
	float masterOutputMeter = 0.f;

	// MS Compression
	bool enableMS;
	msSelection msView = msSelection::kSummed;

	// --- Sidechain
	bool enableSidechain;
	bool scTargetAll;
	bool scTarget[4];

	double masterOutputVolume;

	
};


/**
\class FourBandDynamics
\ingroup FX-Objects
\brief
The FourBandDynamics object implements ....

Audio I/O:
- Processes mono input to mono output.
- *** Optionally, process frame *** Modify this according to your object functionality

Control I/F:
- Use FourBandDynamicsParameters structure to get/set object params.

\author <Your Name> <http://www.yourwebsite.com>
\remark <Put any remarks or notes here>
\version Revision : 1.0
\date Date : 2019 / 01 / 31
*/
class FourBandDynamics : public IAudioSignalProcessor
{
public:
	FourBandDynamics(void) {}	/* C-TOR */
	~FourBandDynamics(void) {}	/* D-TOR */

public:
	/** reset members t initialized state */
	virtual bool reset(double _sampleRate)
	{
		// --- store the sample rate
		sampleRate = _sampleRate;

		for (int i = 0; i < 6; i++)
			splitterFilters[i].reset(sampleRate);

		for (int i = 0; i < 6; i++)
			dynamicsProcessor[i].reset(sampleRate);

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
		double yn[2];

		xn[0] = inputFrame[0];
		xn[1] = inputFrame[1];

		// --- Filter Bank Outputs
		FilterBankOutput lowSplit[2];
		FilterBankOutput midSplit[2];
		FilterBankOutput highSplit[2];

		// --- Channel Bands
		double lpfOutput[2];
		double lowBandOutput[2];
		double highBandOutput[2];
		double hpfOutput[2];

		// --- Dry Signal
		double dryInput[2];

		// --- MS Conversion
		double msOutput[2];


		
		// --- for loop for Stereo Processing
		for (int i = 0; i < 2; i++)
		{
			// --- Filter Bank Outputs
			lowSplit[i] = splitterFilters[i].processFilterBank(xn[i]);
			midSplit[i] = splitterFilters[i + 2].processFilterBank(lowSplit[i].HFOut);
			highSplit[i] = splitterFilters[i + 4].processFilterBank(midSplit[i].HFOut);

			// --- Channel Bands
			lpfOutput[i] = lowSplit[i].LFOut;
			lowBandOutput[i] = midSplit[i].LFOut;
			highBandOutput[i] = highSplit[i].LFOut;
			hpfOutput[i] = highSplit[i].HFOut;

			// --- Dry Signal
			dryInput[i] = (lowSplit[i].LFOut + lowSplit[i].HFOut
				+ midSplit[i].LFOut + midSplit[i].HFOut
				+ highSplit[i].LFOut + highSplit[i].HFOut);

			msOutput[0] = 0.5 * (dryInput[0] + dryInput[1]);
			msOutput[1] = 0.5 * (dryInput[0] - dryInput[1]);

			// --- Input Meter
			parameters.inputMeter[0] = 0.5 * (lpfOutput[0]+ lpfOutput[1]);
			parameters.inputMeter[1] = 0.5 * (lowBandOutput[0] + lpfOutput[1]);
			parameters.inputMeter[2] = 0.5 * (highBandOutput[0] + highBandOutput[1]);
			parameters.inputMeter[3] = 0.5 * (hpfOutput[0] + hpfOutput[1]);

			// ** COMPRESSION **
			lpfOutput[i] = dynamicsProcessor[0].processAudioSample(lpfOutput[i]);
			lowBandOutput[i] = dynamicsProcessor[1].processAudioSample(lowBandOutput[i]);
			highBandOutput[i] = dynamicsProcessor[2].processAudioSample(highBandOutput[i]);
			hpfOutput[i] = dynamicsProcessor[3].processAudioSample(hpfOutput[i]);

			// --- Saturation
			if (parameters.saturation[0] > 1)
				lpfOutput[i] = tanh(lpfOutput[i] * k[0]) / tanh(k[0]);

			if (parameters.saturation[1] > 1)
				lowBandOutput[i] = tanh(lowBandOutput[i] * k[1]) / tanh(k[1]);

			if (parameters.saturation[2] > 1)
				highBandOutput[i] = tanh(highBandOutput[i] * k[2]) / tanh(k[2]);

			if (parameters.saturation[3] > 1)
				hpfOutput[i] = tanh(hpfOutput[i] * k[3]) / tanh(k[3]);

			


			// ** MUTE/SOLO **
			for (int j = 0; j < 6; j++) {
				if (parameters.enableSolo[j])
				{
					for (int x = 0; x < 6; x++)
						parameters.enableMute[x] = true;
				}
			}

			for (int j = 0; j < 6; j++)
			{
				if (parameters.enableSolo[j])
					parameters.enableMute[j] = false;
			}

			if (parameters.enableMute[0])
				lpfOutput[i] = 0.0;
			if (parameters.enableMute[1])
				lowBandOutput[i] = 0.0;
			if (parameters.enableMute[2])
				highBandOutput[i] = 0.0;
			if (parameters.enableMute[3])
				hpfOutput[i] = 0.0;
			if (parameters.enableMute[4])
				msOutput[0] = 0.0;
			if (parameters.enableMute[5])
				msOutput[1] = 0.0;

			
			// ** OUTPUT **
			yn[i] = (lpfOutput[i] + lowBandOutput[i] + highBandOutput[i] + hpfOutput[i]);
		}

		// Add MS signal to output

		// ** MS **
			// --- compression
		msOutput[0] = dynamicsProcessor[4].processAudioSample(msOutput[0]);
		msOutput[1] = dynamicsProcessor[5].processAudioSample(msOutput[1]);

		// --- saturation
		if (parameters.saturation[4] > 1)
			msOutput[0] = tanh(msOutput[0] * k[4]) / tanh(k[4]);
		if (parameters.saturation[5] > 1)
			msOutput[1] = tanh(msOutput[1] * k[5]) / tanh(k[5]);


		yn[0] += 1 * (msOutput[0] + msOutput[1]);
		yn[1] += 1 * (msOutput[0] - msOutput[1]);

		// --- Add dry signal to output
		for (int i = 0; i < 2; i++)
		{
			yn[i] += dryInput[i] * dryVolume_cooked;
			yn[i] *= pow(10, parameters.masterOutputVolume / 20);
		}

		// add master volume
		

		// --- Gain Reduction Meters
 		DynamicsProcessorParameters params[6];


		for (int j = 0; j < 6; j++)
		{
			params[j] = dynamicsProcessor[j].getParameters();
			parameters.reductionMeter[j] = (params[j].gainReduction * -1) + 1;
		}

		// --- Output Meters
		parameters.outputMeter[0] = 0.5 * (lpfOutput[0] + lpfOutput[1]);
		parameters.outputMeter[1] = 0.5 * (lowBandOutput[0] + lowBandOutput[1]);
		parameters.outputMeter[2] = 0.5 * (highBandOutput[0] + highBandOutput[1]);
		parameters.outputMeter[3] = 0.5 * (hpfOutput[0] + hpfOutput[1]);

		parameters.masterInputMeter = 0.5 * (inputFrame[0] + inputFrame[1]);
		parameters.masterOutputMeter = 0.5 * (yn[0] + yn[1]);
		

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
	\return FourBandDynamicsParameters custom data structure
	*/
	FourBandDynamicsParameters getParameters()
	{
		return parameters;
	}

	/** set parameters: note use of custom structure for passing param data */
	/**
	\param FourBandDynamicsParameters custom data structure
	*/
	void setParameters(const FourBandDynamicsParameters& params)
	{
		// --- copy them; note you may choose to ignore certain items
		//     and copy the variables one at a time, or you may test
		//     to see if cook-able variables have changed; if not, then
		//     do not re-cook them as it just wastes CPU

		parameters = params;
		
		// ** COMPRESSOR **
		DynamicsProcessorParameters dynaParams[6];

		for (int i = 0; i < 6; i++)
		{

			k[i] = parameters.saturation[i];

			dynaParams[i] = dynamicsProcessor[i].getParameters();

			dynaParams[i].threshold_dB = parameters.threshold[i];
			dynaParams[i].ratio = parameters.ratio[i];
			dynaParams[i].attackTime_mSec = parameters.attack[i];
			dynaParams[i].releaseTime_mSec = parameters.release[i];
			dynaParams[i].outputGain_dB = parameters.gain[i];
			dynaParams[i].kneeWidth_dB = parameters.knee[i];

			dynaParams[i].hardLimitGate = parameters.hardLimitGate[i];
			dynaParams[i].calculation = convertIntToEnum(parameters.dynamicsMode[i], dynamicsProcessorType);

			dynamicsProcessor[i].setParameters(dynaParams[i]);

		}
		

		// ** FILTERBANK **
		// --- set range of dry volume
		if (parameters.dryVolume <= -15.0)
			dryVolume_cooked = 0.0;	
		else
			dryVolume_cooked = pow(10, parameters.dryVolume / 20);
		
		
		// --- set filter frequency
		LRFilterBankParameters bankParams[3];

		for (int i = 0; i < 3; i++)
			bankParams[i] = splitterFilters[i * 2].getParameters();
		
		for (int i = 0; i < 2; i++)
		{
			if (parameters.splitF[i] > parameters.splitF[i + 1])
				parameters.splitF[i] = parameters.splitF[i + 1];
		}

		for (int i = 1; i < 3; i++)
		{
			if (parameters.splitF[i] < parameters.splitF[i - 1])
				parameters.splitF[i] = parameters.splitF[i - 1];
		}
		
		for (int i = 0; i < 3; i++)
			bankParams[i].splitFrequency = parameters.splitF[i];

		splitterFilters[0].setParameters(bankParams[0]);
		splitterFilters[1].setParameters(bankParams[0]);
		splitterFilters[2].setParameters(bankParams[1]);
		splitterFilters[3].setParameters(bankParams[1]);
		splitterFilters[4].setParameters(bankParams[2]);
		splitterFilters[5].setParameters(bankParams[2]);

	}


private:
	FourBandDynamicsParameters parameters; ///< object parameters

	// ** COMPRESSOR **
	DynamicsProcessor dynamicsProcessor[6];

	double threshold_cooked[4];


	// ** FILTERBANK **
	LRFilterBank splitterFilters[6];
	
	double volume_cooked[4];
	double dryVolume_cooked = 0.0;
	double k[6];

	// --- local variables used by this object
	double sampleRate = 0.0;	///< sample rate

};

#endif