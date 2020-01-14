// -----------------------------------------------------------------------------
//    ASPiK Plugin Kernel File:  plugincore.cpp
//
/**
    \file   plugincore.cpp
    \author Will Pirkle
    \date   17-September-2018
    \brief  Implementation file for PluginCore object
    		- http://www.aspikplugins.com
    		- http://www.willpirkle.com
*/
// -----------------------------------------------------------------------------
#include "plugincore.h"
#include "plugindescription.h"

/**
\brief PluginCore constructor is launching pad for object initialization

Operations:
- initialize the plugin description (strings, codes, numbers, see initPluginDescriptors())
- setup the plugin's audio I/O channel support
- create the PluginParameter objects that represent the plugin parameters (see FX book if needed)
- create the presets
*/
PluginCore::PluginCore()
{
    // --- describe the plugin; call the helper to init the static parts you setup in plugindescription.h
    initPluginDescriptors();

    // --- default I/O combinations
	// --- for FX plugins
	if (getPluginType() == kFXPlugin)
	{
		addSupportedIOCombination({ kCFMono, kCFMono });
		addSupportedIOCombination({ kCFMono, kCFStereo });
		addSupportedIOCombination({ kCFStereo, kCFStereo });
	}
	else // --- synth plugins have no input, only output
	{
		addSupportedIOCombination({ kCFNone, kCFMono });
		addSupportedIOCombination({ kCFNone, kCFStereo });
	}

	// --- for sidechaining, we support mono and stereo inputs; auxOutputs reserved for future use
	addSupportedAuxIOCombination({ kCFMono, kCFNone });
	addSupportedAuxIOCombination({ kCFStereo, kCFNone });

	// --- create the parameters
    initPluginParameters();

    // --- create the presets
    initPluginPresets();
}

/**
\brief create all of your plugin parameters here

\return true if parameters were created, false if they already existed
*/
bool PluginCore::initPluginParameters()
{
	if (pluginParameterMap.size() > 0)
		return false;

    // --- Add your plugin parameter instantiation code bewtween these hex codes
	// **--0xDEA7--**


	// --- Declaration of Plugin Parameter Objects 
	PluginParameter* piParam = nullptr;

	// --- continuous control: Low Split
	piParam = new PluginParameter(controlID::splitLow, "Low Split", "Hz", controlVariableType::kDouble, 100.000000, 1000.000000, 200.000000, taper::kLinearTaper);
	piParam->setParameterSmoothing(false);
	piParam->setSmoothingTimeMsec(100.00);
	piParam->setBoundVariable(&splitLow, boundVariableType::kDouble);
	addPluginParameter(piParam);

	// --- continuous control: Mid Split
	piParam = new PluginParameter(controlID::splitMid, "Mid Split", "Hz", controlVariableType::kDouble, 100.000000, 5000.000000, 1000.000000, taper::kLinearTaper);
	piParam->setParameterSmoothing(false);
	piParam->setSmoothingTimeMsec(100.00);
	piParam->setBoundVariable(&splitMid, boundVariableType::kDouble);
	addPluginParameter(piParam);

	// --- continuous control: High Split
	piParam = new PluginParameter(controlID::splitHigh, "High Split", "Hz", controlVariableType::kDouble, 1000.000000, 15000.000000, 10000.000000, taper::kLinearTaper);
	piParam->setParameterSmoothing(false);
	piParam->setSmoothingTimeMsec(100.00);
	piParam->setBoundVariable(&splitHigh, boundVariableType::kDouble);
	addPluginParameter(piParam);

	// --- continuous control: Low Threshold
	piParam = new PluginParameter(controlID::threshold1_db, "Low Threshold", "dB", controlVariableType::kDouble, -40.000000, 0.000000, -10.000000, taper::kLinearTaper);
	piParam->setParameterSmoothing(false);
	piParam->setSmoothingTimeMsec(100.00);
	piParam->setBoundVariable(&threshold1_db, boundVariableType::kDouble);
	addPluginParameter(piParam);

	// --- continuous control: Low Ratio
	piParam = new PluginParameter(controlID::ratio1, "Low Ratio", "", controlVariableType::kDouble, 1.000000, 20.000000, 4.000000, taper::kLinearTaper);
	piParam->setParameterSmoothing(false);
	piParam->setSmoothingTimeMsec(100.00);
	piParam->setBoundVariable(&ratio1, boundVariableType::kDouble);
	addPluginParameter(piParam);

	// --- continuous control: Low Attack
	piParam = new PluginParameter(controlID::attack1_ms, "Low Attack", "ms", controlVariableType::kDouble, 0.000000, 100.000000, 5.000000, taper::kAntiLogTaper);
	piParam->setParameterSmoothing(false);
	piParam->setSmoothingTimeMsec(100.00);
	piParam->setBoundVariable(&attack1_ms, boundVariableType::kDouble);
	addPluginParameter(piParam);

	// --- continuous control: Low Release
	piParam = new PluginParameter(controlID::release1_ms, "Low Release", "ms", controlVariableType::kDouble, 1.000000, 500.000000, 200.000000, taper::kAntiLogTaper);
	piParam->setParameterSmoothing(false);
	piParam->setSmoothingTimeMsec(100.00);
	piParam->setBoundVariable(&release1_ms, boundVariableType::kDouble);
	addPluginParameter(piParam);

	// --- continuous control: Low Gain
	piParam = new PluginParameter(controlID::gain1_db, "Low Gain", "dB", controlVariableType::kDouble, -20.000000, 20.000000, -3.000000, taper::kLinearTaper);
	piParam->setParameterSmoothing(false);
	piParam->setSmoothingTimeMsec(100.00);
	piParam->setBoundVariable(&gain1_db, boundVariableType::kDouble);
	addPluginParameter(piParam);

	// --- discrete control: Low Mute
	piParam = new PluginParameter(controlID::enableMute1, "Low Mute", "SWITCH OFF,SWITCH ON", "SWITCH OFF");
	piParam->setBoundVariable(&enableMute1, boundVariableType::kInt);
	piParam->setIsDiscreteSwitch(true);
	addPluginParameter(piParam);

	// --- discrete control: Low Solo
	piParam = new PluginParameter(controlID::enableSolo1, "Low Solo", "SWITCH OFF,SWITCH ON", "SWITCH OFF");
	piParam->setBoundVariable(&enableSolo1, boundVariableType::kInt);
	piParam->setIsDiscreteSwitch(true);
	addPluginParameter(piParam);

	// --- continuous control: Low Knee
	piParam = new PluginParameter(controlID::knee1, "Low Knee", "", controlVariableType::kDouble, 0.000000, 20.000000, 5.000000, taper::kLinearTaper);
	piParam->setParameterSmoothing(false);
	piParam->setSmoothingTimeMsec(100.00);
	piParam->setBoundVariable(&knee1, boundVariableType::kDouble);
	addPluginParameter(piParam);

	// --- continuous control: Dry Volume
	piParam = new PluginParameter(controlID::dryVolume, "Dry Volume", "dB", controlVariableType::kDouble, -60.000000, 0.000000, -60.000000, taper::kLinearTaper);
	piParam->setParameterSmoothing(false);
	piParam->setSmoothingTimeMsec(100.00);
	piParam->setBoundVariable(&dryVolume, boundVariableType::kDouble);
	addPluginParameter(piParam);

	// --- discrete control: Limiter/Gate 1
	piParam = new PluginParameter(controlID::hardLimit1, "Limiter/Gate 1", "SWITCH OFF,SWITCH ON", "SWITCH OFF");
	piParam->setBoundVariable(&hardLimit1, boundVariableType::kInt);
	piParam->setIsDiscreteSwitch(true);
	addPluginParameter(piParam);

	// --- discrete control: Low Mode
	piParam = new PluginParameter(controlID::mode1, "Low Mode", "Compressor,Expander", "Compressor");
	piParam->setBoundVariable(&mode1, boundVariableType::kInt);
	piParam->setIsDiscreteSwitch(true);
	addPluginParameter(piParam);

	// --- continuous control: LowBand Threshold
	piParam = new PluginParameter(controlID::threshold2_db, "LowBand Threshold", "dB", controlVariableType::kDouble, -40.000000, 0.000000, -10.000000, taper::kLinearTaper);
	piParam->setParameterSmoothing(false);
	piParam->setSmoothingTimeMsec(100.00);
	piParam->setBoundVariable(&threshold2_db, boundVariableType::kDouble);
	addPluginParameter(piParam);

	// --- continuous control: MidBand Threshold
	piParam = new PluginParameter(controlID::threshold3_db, "MidBand Threshold", "dB", controlVariableType::kDouble, -40.000000, 0.000000, -10.000000, taper::kLinearTaper);
	piParam->setParameterSmoothing(false);
	piParam->setSmoothingTimeMsec(100.00);
	piParam->setBoundVariable(&threshold3_db, boundVariableType::kDouble);
	addPluginParameter(piParam);

	// --- continuous control: High Threshold
	piParam = new PluginParameter(controlID::threshold4_db, "High Threshold", "dB", controlVariableType::kDouble, -40.000000, 0.000000, -10.000000, taper::kLinearTaper);
	piParam->setParameterSmoothing(false);
	piParam->setSmoothingTimeMsec(100.00);
	piParam->setBoundVariable(&threshold4_db, boundVariableType::kDouble);
	addPluginParameter(piParam);

	// --- continuous control: LowBand Ratio
	piParam = new PluginParameter(controlID::ratio2, "LowBand Ratio", "", controlVariableType::kDouble, 1.000000, 20.000000, 4.000000, taper::kLinearTaper);
	piParam->setParameterSmoothing(false);
	piParam->setSmoothingTimeMsec(100.00);
	piParam->setBoundVariable(&ratio2, boundVariableType::kDouble);
	addPluginParameter(piParam);

	// --- continuous control: MidBand Ratio
	piParam = new PluginParameter(controlID::ratio3, "MidBand Ratio", "", controlVariableType::kDouble, 1.000000, 20.000000, 4.000000, taper::kLinearTaper);
	piParam->setParameterSmoothing(false);
	piParam->setSmoothingTimeMsec(100.00);
	piParam->setBoundVariable(&ratio3, boundVariableType::kDouble);
	addPluginParameter(piParam);

	// --- continuous control: High Ratio
	piParam = new PluginParameter(controlID::ratio4, "High Ratio", "", controlVariableType::kDouble, 1.000000, 20.000000, 4.000000, taper::kLinearTaper);
	piParam->setParameterSmoothing(false);
	piParam->setSmoothingTimeMsec(100.00);
	piParam->setBoundVariable(&ratio4, boundVariableType::kDouble);
	addPluginParameter(piParam);

	// --- continuous control: LowBand Attack
	piParam = new PluginParameter(controlID::attack2_ms, "LowBand Attack", "ms", controlVariableType::kDouble, 0.000000, 100.000000, 5.000000, taper::kAntiLogTaper);
	piParam->setParameterSmoothing(false);
	piParam->setSmoothingTimeMsec(100.00);
	piParam->setBoundVariable(&attack2_ms, boundVariableType::kDouble);
	addPluginParameter(piParam);

	// --- continuous control: MidBand Attack
	piParam = new PluginParameter(controlID::attack3_ms, "MidBand Attack", "ms", controlVariableType::kDouble, 0.000000, 100.000000, 5.000000, taper::kAntiLogTaper);
	piParam->setParameterSmoothing(false);
	piParam->setSmoothingTimeMsec(100.00);
	piParam->setBoundVariable(&attack3_ms, boundVariableType::kDouble);
	addPluginParameter(piParam);

	// --- continuous control: High Attack
	piParam = new PluginParameter(controlID::attack4_ms, "High Attack", "ms", controlVariableType::kDouble, 0.000000, 100.000000, 5.000000, taper::kAntiLogTaper);
	piParam->setParameterSmoothing(false);
	piParam->setSmoothingTimeMsec(100.00);
	piParam->setBoundVariable(&attack4_ms, boundVariableType::kDouble);
	addPluginParameter(piParam);

	// --- continuous control: LowBand Release
	piParam = new PluginParameter(controlID::release2_ms, "LowBand Release", "ms", controlVariableType::kDouble, 1.000000, 1000.000000, 200.000000, taper::kAntiLogTaper);
	piParam->setParameterSmoothing(false);
	piParam->setSmoothingTimeMsec(100.00);
	piParam->setBoundVariable(&release2_ms, boundVariableType::kDouble);
	addPluginParameter(piParam);

	// --- continuous control: MidBand Release
	piParam = new PluginParameter(controlID::release3_ms, "MidBand Release", "ms", controlVariableType::kDouble, 1.000000, 1000.000000, 200.000000, taper::kAntiLogTaper);
	piParam->setParameterSmoothing(false);
	piParam->setSmoothingTimeMsec(100.00);
	piParam->setBoundVariable(&release3_ms, boundVariableType::kDouble);
	addPluginParameter(piParam);

	// --- continuous control: High Release
	piParam = new PluginParameter(controlID::release4_ms, "High Release", "ms", controlVariableType::kDouble, 1.000000, 1000.000000, 200.000000, taper::kAntiLogTaper);
	piParam->setParameterSmoothing(false);
	piParam->setSmoothingTimeMsec(100.00);
	piParam->setBoundVariable(&release4_ms, boundVariableType::kDouble);
	addPluginParameter(piParam);

	// --- continuous control: LowBand Gain
	piParam = new PluginParameter(controlID::gain2_db, "LowBand Gain", "dB", controlVariableType::kDouble, -20.000000, 20.000000, -3.000000, taper::kLinearTaper);
	piParam->setParameterSmoothing(false);
	piParam->setSmoothingTimeMsec(100.00);
	piParam->setBoundVariable(&gain2_db, boundVariableType::kDouble);
	addPluginParameter(piParam);

	// --- continuous control: MidBand Gain
	piParam = new PluginParameter(controlID::gain3_db, "MidBand Gain", "dB", controlVariableType::kDouble, -20.000000, 20.000000, -3.000000, taper::kLinearTaper);
	piParam->setParameterSmoothing(false);
	piParam->setSmoothingTimeMsec(100.00);
	piParam->setBoundVariable(&gain3_db, boundVariableType::kDouble);
	addPluginParameter(piParam);

	// --- continuous control: High Gain
	piParam = new PluginParameter(controlID::gain4_db, "High Gain", "dB", controlVariableType::kDouble, -20.000000, 20.000000, -3.000000, taper::kLinearTaper);
	piParam->setParameterSmoothing(false);
	piParam->setSmoothingTimeMsec(100.00);
	piParam->setBoundVariable(&gain4_db, boundVariableType::kDouble);
	addPluginParameter(piParam);

	// --- continuous control: LowBand Knee
	piParam = new PluginParameter(controlID::knee2, "LowBand Knee", "", controlVariableType::kDouble, 0.000000, 20.000000, 5.000000, taper::kLinearTaper);
	piParam->setParameterSmoothing(false);
	piParam->setSmoothingTimeMsec(100.00);
	piParam->setBoundVariable(&knee2, boundVariableType::kDouble);
	addPluginParameter(piParam);

	// --- continuous control: MidBand Knee
	piParam = new PluginParameter(controlID::knee3, "MidBand Knee", "", controlVariableType::kDouble, 0.000000, 20.000000, 5.000000, taper::kLinearTaper);
	piParam->setParameterSmoothing(false);
	piParam->setSmoothingTimeMsec(100.00);
	piParam->setBoundVariable(&knee3, boundVariableType::kDouble);
	addPluginParameter(piParam);

	// --- continuous control: High Knee
	piParam = new PluginParameter(controlID::knee4, "High Knee", "", controlVariableType::kDouble, 0.000000, 20.000000, 5.000000, taper::kLinearTaper);
	piParam->setParameterSmoothing(false);
	piParam->setSmoothingTimeMsec(100.00);
	piParam->setBoundVariable(&knee4, boundVariableType::kDouble);
	addPluginParameter(piParam);

	// --- meter control: Input 1
	piParam = new PluginParameter(controlID::inputMeter1, "Input 1", 10.00, 250.00, ENVELOPE_DETECT_MODE_PEAK, meterCal::kLogMeter);
	piParam->setBoundVariable(&inputMeter1, boundVariableType::kFloat);
	addPluginParameter(piParam);

	// --- meter control: Output1
	piParam = new PluginParameter(controlID::outputMeter1, "Output1", 10.00, 250.00, ENVELOPE_DETECT_MODE_PEAK, meterCal::kLogMeter);
	piParam->setBoundVariable(&outputMeter1, boundVariableType::kFloat);
	addPluginParameter(piParam);

	// --- meter control: Low GR
	piParam = new PluginParameter(controlID::reductionMeter1, "Low GR", 0.00, 0.00, ENVELOPE_DETECT_MODE_PEAK, meterCal::kLinearMeter);
	piParam->setInvertedMeter(true);
	piParam->setBoundVariable(&reductionMeter1, boundVariableType::kFloat);
	addPluginParameter(piParam);

	// --- discrete control: LowBand Mute
	piParam = new PluginParameter(controlID::enableMute2, "LowBand Mute", "SWITCH OFF,SWITCH ON", "SWITCH OFF");
	piParam->setBoundVariable(&enableMute2, boundVariableType::kInt);
	piParam->setIsDiscreteSwitch(true);
	addPluginParameter(piParam);

	// --- discrete control: MidBand Mute
	piParam = new PluginParameter(controlID::enableMute3, "MidBand Mute", "SWITCH OFF,SWITCH ON", "SWITCH OFF");
	piParam->setBoundVariable(&enableMute3, boundVariableType::kInt);
	piParam->setIsDiscreteSwitch(true);
	addPluginParameter(piParam);

	// --- discrete control: High Mute
	piParam = new PluginParameter(controlID::enableMute4, "High Mute", "SWITCH OFF,SWITCH ON", "SWITCH OFF");
	piParam->setBoundVariable(&enableMute4, boundVariableType::kInt);
	piParam->setIsDiscreteSwitch(true);
	addPluginParameter(piParam);

	// --- discrete control: LowBand Solo
	piParam = new PluginParameter(controlID::enableSolo2, "LowBand Solo", "SWITCH OFF,SWITCH ON", "SWITCH OFF");
	piParam->setBoundVariable(&enableSolo2, boundVariableType::kInt);
	piParam->setIsDiscreteSwitch(true);
	addPluginParameter(piParam);

	// --- discrete control: MidBand Solo
	piParam = new PluginParameter(controlID::enableSolo3, "MidBand Solo", "SWITCH OFF,SWITCH ON", "SWITCH OFF");
	piParam->setBoundVariable(&enableSolo3, boundVariableType::kInt);
	piParam->setIsDiscreteSwitch(true);
	addPluginParameter(piParam);

	// --- discrete control: High Solo
	piParam = new PluginParameter(controlID::enableSolo4, "High Solo", "SWITCH OFF,SWITCH ON", "SWITCH OFF");
	piParam->setBoundVariable(&enableSolo4, boundVariableType::kInt);
	piParam->setIsDiscreteSwitch(true);
	addPluginParameter(piParam);

	// --- meter control: Input 2
	piParam = new PluginParameter(controlID::inputMeter2, "Input 2", 10.00, 250.00, ENVELOPE_DETECT_MODE_PEAK, meterCal::kLogMeter);
	piParam->setBoundVariable(&inputMeter2, boundVariableType::kFloat);
	addPluginParameter(piParam);

	// --- meter control: Input 3
	piParam = new PluginParameter(controlID::inputMeter3, "Input 3", 10.00, 250.00, ENVELOPE_DETECT_MODE_PEAK, meterCal::kLogMeter);
	piParam->setBoundVariable(&inputMeter3, boundVariableType::kFloat);
	addPluginParameter(piParam);

	// --- meter control: Input 4
	piParam = new PluginParameter(controlID::inputMeter4, "Input 4", 10.00, 250.00, ENVELOPE_DETECT_MODE_PEAK, meterCal::kLogMeter);
	piParam->setBoundVariable(&inputMeter4, boundVariableType::kFloat);
	addPluginParameter(piParam);

	// --- meter control: Output 2
	piParam = new PluginParameter(controlID::outputMeter2, "Output 2", 10.00, 250.00, ENVELOPE_DETECT_MODE_PEAK, meterCal::kLogMeter);
	piParam->setBoundVariable(&outputMeter2, boundVariableType::kFloat);
	addPluginParameter(piParam);

	// --- meter control: Output 3
	piParam = new PluginParameter(controlID::outputMeter3, "Output 3", 10.00, 250.00, ENVELOPE_DETECT_MODE_PEAK, meterCal::kLogMeter);
	piParam->setBoundVariable(&outputMeter3, boundVariableType::kFloat);
	addPluginParameter(piParam);

	// --- meter control: Output 4
	piParam = new PluginParameter(controlID::outputMeter4, "Output 4", 10.00, 250.00, ENVELOPE_DETECT_MODE_PEAK, meterCal::kLogMeter);
	piParam->setBoundVariable(&outputMeter4, boundVariableType::kFloat);
	addPluginParameter(piParam);

	// --- meter control: GR 2
	piParam = new PluginParameter(controlID::reductionMeter2, "GR 2", 0.00, 0.00, ENVELOPE_DETECT_MODE_PEAK, meterCal::kLinearMeter);
	piParam->setInvertedMeter(true);
	piParam->setBoundVariable(&reductionMeter2, boundVariableType::kFloat);
	addPluginParameter(piParam);

	// --- meter control: GR 3
	piParam = new PluginParameter(controlID::reductionMeter3, "GR 3", 0.00, 0.00, ENVELOPE_DETECT_MODE_PEAK, meterCal::kLinearMeter);
	piParam->setInvertedMeter(true);
	piParam->setBoundVariable(&reductionMeter3, boundVariableType::kFloat);
	addPluginParameter(piParam);

	// --- meter control: GR 4
	piParam = new PluginParameter(controlID::reductionMeter4, "GR 4", 0.00, 0.00, ENVELOPE_DETECT_MODE_PEAK, meterCal::kLinearMeter);
	piParam->setInvertedMeter(true);
	piParam->setBoundVariable(&reductionMeter4, boundVariableType::kFloat);
	addPluginParameter(piParam);

	// --- discrete control: Limiter/Gate 2
	piParam = new PluginParameter(controlID::hardLimit2, "Limiter/Gate 2", "SWITCH OFF,SWITCH ON", "SWITCH OFF");
	piParam->setBoundVariable(&hardLimit2, boundVariableType::kInt);
	piParam->setIsDiscreteSwitch(true);
	addPluginParameter(piParam);

	// --- discrete control: Limiter/Gate 3
	piParam = new PluginParameter(controlID::hardLimit3, "Limiter/Gate 3", "SWITCH OFF,SWITCH ON", "SWITCH OFF");
	piParam->setBoundVariable(&hardLimit3, boundVariableType::kInt);
	piParam->setIsDiscreteSwitch(true);
	addPluginParameter(piParam);

	// --- discrete control: Limiter/Gate 4
	piParam = new PluginParameter(controlID::hardLimit4, "Limiter/Gate 4", "SWITCH OFF,SWITCH ON", "SWITCH OFF");
	piParam->setBoundVariable(&hardLimit4, boundVariableType::kInt);
	piParam->setIsDiscreteSwitch(true);
	addPluginParameter(piParam);

	// --- discrete control: Mode 2
	piParam = new PluginParameter(controlID::mode2, "Mode 2", "Compressor,Expander", "Compressor");
	piParam->setBoundVariable(&mode2, boundVariableType::kInt);
	piParam->setIsDiscreteSwitch(true);
	addPluginParameter(piParam);

	// --- discrete control: Mode 3
	piParam = new PluginParameter(controlID::mode3, "Mode 3", "Compressor,Expander", "Compressor");
	piParam->setBoundVariable(&mode3, boundVariableType::kInt);
	piParam->setIsDiscreteSwitch(true);
	addPluginParameter(piParam);

	// --- discrete control: Mode 4
	piParam = new PluginParameter(controlID::mode4, "Mode 4", "Compressor,Expander", "Compressor");
	piParam->setBoundVariable(&mode4, boundVariableType::kInt);
	piParam->setIsDiscreteSwitch(true);
	addPluginParameter(piParam);

	// --- discrete control: Bypass 1
	piParam = new PluginParameter(controlID::bypass1, "Bypass 1", "SWITCH OFF,SWITCH ON", "SWITCH OFF");
	piParam->setBoundVariable(&bypass1, boundVariableType::kInt);
	piParam->setIsDiscreteSwitch(true);
	addPluginParameter(piParam);

	// --- discrete control: Bypass 2
	piParam = new PluginParameter(controlID::bypass2, "Bypass 2", "SWITCH OFF,SWITCH ON", "SWITCH OFF");
	piParam->setBoundVariable(&bypass2, boundVariableType::kInt);
	piParam->setIsDiscreteSwitch(true);
	addPluginParameter(piParam);

	// --- discrete control: Bypass 3
	piParam = new PluginParameter(controlID::bypass3, "Bypass 3", "SWITCH OFF,SWITCH ON", "SWITCH OFF");
	piParam->setBoundVariable(&bypass3, boundVariableType::kInt);
	piParam->setIsDiscreteSwitch(true);
	addPluginParameter(piParam);

	// --- discrete control: Bypass 4
	piParam = new PluginParameter(controlID::bypass4, "Bypass 4", "SWITCH OFF,SWITCH ON", "SWITCH OFF");
	piParam->setBoundVariable(&bypass4, boundVariableType::kInt);
	piParam->setIsDiscreteSwitch(true);
	addPluginParameter(piParam);

	// --- meter control: Master Input
	piParam = new PluginParameter(controlID::masterInputMeter, "Master Input", 10.00, 250.00, ENVELOPE_DETECT_MODE_PEAK, meterCal::kLogMeter);
	piParam->setBoundVariable(&masterInputMeter, boundVariableType::kFloat);
	addPluginParameter(piParam);

	// --- meter control: Master Output
	piParam = new PluginParameter(controlID::masterOutputMeter, "Master Output", 10.00, 250.00, ENVELOPE_DETECT_MODE_PEAK, meterCal::kLogMeter);
	piParam->setBoundVariable(&masterOutputMeter, boundVariableType::kFloat);
	addPluginParameter(piParam);

	// --- discrete control: Enable Sidechain
	piParam = new PluginParameter(controlID::enableSidechain, "Enable Sidechain", "SWITCH OFF,SWITCH ON", "SWITCH OFF");
	piParam->setBoundVariable(&enableSidechain, boundVariableType::kInt);
	piParam->setIsDiscreteSwitch(true);
	addPluginParameter(piParam);

	// --- discrete control: Target All
	piParam = new PluginParameter(controlID::scTargetAll, "Target All", "SWITCH OFF,SWITCH ON", "SWITCH OFF");
	piParam->setBoundVariable(&scTargetAll, boundVariableType::kInt);
	piParam->setIsDiscreteSwitch(true);
	addPluginParameter(piParam);

	// --- discrete control: Target 1
	piParam = new PluginParameter(controlID::scTarget1, "Target 1", "SWITCH OFF,SWITCH ON", "SWITCH OFF");
	piParam->setBoundVariable(&scTarget1, boundVariableType::kInt);
	piParam->setIsDiscreteSwitch(true);
	addPluginParameter(piParam);

	// --- discrete control: Target 2
	piParam = new PluginParameter(controlID::scTarget2, "Target 2", "SWITCH OFF,SWITCH ON", "SWITCH OFF");
	piParam->setBoundVariable(&scTarget2, boundVariableType::kInt);
	piParam->setIsDiscreteSwitch(true);
	addPluginParameter(piParam);

	// --- discrete control: Target 3
	piParam = new PluginParameter(controlID::scTarget3, "Target 3", "SWITCH OFF,SWITCH ON", "SWITCH OFF");
	piParam->setBoundVariable(&scTarget3, boundVariableType::kInt);
	piParam->setIsDiscreteSwitch(true);
	addPluginParameter(piParam);

	// --- discrete control: Target 4
	piParam = new PluginParameter(controlID::scTarget4, "Target 4", "SWITCH OFF,SWITCH ON", "SWITCH OFF");
	piParam->setBoundVariable(&scTarget4, boundVariableType::kInt);
	piParam->setIsDiscreteSwitch(true);
	addPluginParameter(piParam);

	// --- discrete control: Enable MS
	piParam = new PluginParameter(controlID::enableMS, "Enable MS", "SWITCH OFF,SWITCH ON", "SWITCH OFF");
	piParam->setBoundVariable(&enableMS, boundVariableType::kInt);
	piParam->setIsDiscreteSwitch(true);
	addPluginParameter(piParam);

	// --- discrete control: MS View
	piParam = new PluginParameter(controlID::midsideView, "MS View", "Summed,Mid,Side", "Summed");
	piParam->setBoundVariable(&midsideView, boundVariableType::kInt);
	piParam->setIsDiscreteSwitch(true);
	addPluginParameter(piParam);

	// --- continuous control: Saturation 1
	piParam = new PluginParameter(controlID::saturation1, "Saturation 1", "", controlVariableType::kDouble, 1.000000, 10.000000, 1.000000, taper::kLinearTaper);
	piParam->setParameterSmoothing(false);
	piParam->setSmoothingTimeMsec(100.00);
	piParam->setBoundVariable(&saturation1, boundVariableType::kDouble);
	addPluginParameter(piParam);

	// --- continuous control: Saturation 2
	piParam = new PluginParameter(controlID::saturation2, "Saturation 2", "", controlVariableType::kDouble, 1.000000, 10.000000, 1.000000, taper::kLinearTaper);
	piParam->setParameterSmoothing(false);
	piParam->setSmoothingTimeMsec(100.00);
	piParam->setBoundVariable(&saturation2, boundVariableType::kDouble);
	addPluginParameter(piParam);

	// --- continuous control: Saturation 3
	piParam = new PluginParameter(controlID::saturation3, "Saturation 3", "", controlVariableType::kDouble, 1.000000, 10.000000, 1.000000, taper::kLinearTaper);
	piParam->setParameterSmoothing(false);
	piParam->setSmoothingTimeMsec(100.00);
	piParam->setBoundVariable(&saturation3, boundVariableType::kDouble);
	addPluginParameter(piParam);

	// --- continuous control: Saturation 4
	piParam = new PluginParameter(controlID::saturation4, "Saturation 4", "", controlVariableType::kDouble, 1.000000, 10.000000, 1.000000, taper::kLinearTaper);
	piParam->setParameterSmoothing(false);
	piParam->setSmoothingTimeMsec(100.00);
	piParam->setBoundVariable(&saturation4, boundVariableType::kDouble);
	addPluginParameter(piParam);

	// --- continuous control: Mid Threshold
	piParam = new PluginParameter(controlID::midThreshold, "Mid Threshold", "dB", controlVariableType::kDouble, -40.000000, 0.000000, -10.000000, taper::kLinearTaper);
	piParam->setParameterSmoothing(false);
	piParam->setSmoothingTimeMsec(100.00);
	piParam->setBoundVariable(&midThreshold, boundVariableType::kDouble);
	addPluginParameter(piParam);

	// --- continuous control: Side Threshold
	piParam = new PluginParameter(controlID::sideThreshold, "Side Threshold", "dB", controlVariableType::kDouble, -40.000000, 0.000000, -10.000000, taper::kLinearTaper);
	piParam->setParameterSmoothing(false);
	piParam->setSmoothingTimeMsec(100.00);
	piParam->setBoundVariable(&sideThreshold, boundVariableType::kDouble);
	addPluginParameter(piParam);

	// --- continuous control: Mid Ratio
	piParam = new PluginParameter(controlID::midRatio, "Mid Ratio", "", controlVariableType::kDouble, 1.000000, 20.000000, 4.000000, taper::kLinearTaper);
	piParam->setParameterSmoothing(false);
	piParam->setSmoothingTimeMsec(100.00);
	piParam->setBoundVariable(&midRatio, boundVariableType::kDouble);
	addPluginParameter(piParam);

	// --- continuous control: Side Ratio
	piParam = new PluginParameter(controlID::sideRatio, "Side Ratio", "", controlVariableType::kDouble, 1.000000, 20.000000, 4.000000, taper::kLinearTaper);
	piParam->setParameterSmoothing(false);
	piParam->setSmoothingTimeMsec(100.00);
	piParam->setBoundVariable(&sideRatio, boundVariableType::kDouble);
	addPluginParameter(piParam);

	// --- discrete control: Band View
	piParam = new PluginParameter(controlID::bandView, "Band View", "Low,Lowband,Midband,High,Mid,Side", "Low");
	piParam->setBoundVariable(&bandView, boundVariableType::kInt);
	piParam->setIsDiscreteSwitch(true);
	addPluginParameter(piParam);

	// --- continuous control: Mid Attack
	piParam = new PluginParameter(controlID::midAttack, "Mid Attack", "ms", controlVariableType::kDouble, 0.000000, 100.000000, 5.000000, taper::kAntiLogTaper);
	piParam->setParameterSmoothing(false);
	piParam->setSmoothingTimeMsec(100.00);
	piParam->setBoundVariable(&midAttack, boundVariableType::kDouble);
	addPluginParameter(piParam);

	// --- continuous control: Mid Release
	piParam = new PluginParameter(controlID::midRelease, "Mid Release", "ms", controlVariableType::kDouble, 1.000000, 1000.000000, 250.000000, taper::kAntiLogTaper);
	piParam->setParameterSmoothing(false);
	piParam->setSmoothingTimeMsec(100.00);
	piParam->setBoundVariable(&midRelease, boundVariableType::kDouble);
	addPluginParameter(piParam);

	// --- continuous control: Side Attack
	piParam = new PluginParameter(controlID::sideAttack, "Side Attack", "ms", controlVariableType::kDouble, 0.000000, 100.000000, 5.000000, taper::kAntiLogTaper);
	piParam->setParameterSmoothing(false);
	piParam->setSmoothingTimeMsec(100.00);
	piParam->setBoundVariable(&sideAttack, boundVariableType::kDouble);
	addPluginParameter(piParam);

	// --- continuous control: Side Release
	piParam = new PluginParameter(controlID::sideRelease, "Side Release", "ms", controlVariableType::kDouble, 1.000000, 1000.000000, 250.000000, taper::kAntiLogTaper);
	piParam->setParameterSmoothing(false);
	piParam->setSmoothingTimeMsec(100.00);
	piParam->setBoundVariable(&sideRelease, boundVariableType::kDouble);
	addPluginParameter(piParam);

	// --- continuous control: Mid Gain
	piParam = new PluginParameter(controlID::midGain, "Mid Gain", "dB", controlVariableType::kDouble, -20.000000, 20.000000, -6.000000, taper::kLinearTaper);
	piParam->setParameterSmoothing(false);
	piParam->setSmoothingTimeMsec(100.00);
	piParam->setBoundVariable(&midGain, boundVariableType::kDouble);
	addPluginParameter(piParam);

	// --- continuous control: Side Gain
	piParam = new PluginParameter(controlID::sideGain, "Side Gain", "dB", controlVariableType::kDouble, -20.000000, 20.000000, -6.000000, taper::kLinearTaper);
	piParam->setParameterSmoothing(false);
	piParam->setSmoothingTimeMsec(100.00);
	piParam->setBoundVariable(&sideGain, boundVariableType::kDouble);
	addPluginParameter(piParam);

	// --- meter control: Mid Reduction
	piParam = new PluginParameter(controlID::midReductionMeter, "Mid Reduction", 0.00, 0.00, ENVELOPE_DETECT_MODE_PEAK, meterCal::kLinearMeter);
	piParam->setInvertedMeter(true);
	piParam->setBoundVariable(&midReductionMeter, boundVariableType::kFloat);
	addPluginParameter(piParam);

	// --- meter control: Side Reduction
	piParam = new PluginParameter(controlID::sideReductionMeter, "Side Reduction", 0.00, 0.00, ENVELOPE_DETECT_MODE_PEAK, meterCal::kLinearMeter);
	piParam->setInvertedMeter(true);
	piParam->setBoundVariable(&sideReductionMeter, boundVariableType::kFloat);
	addPluginParameter(piParam);

	// --- discrete control: Mid Mode
	piParam = new PluginParameter(controlID::midMode, "Mid Mode", "Compressor,Expander", "Compressor");
	piParam->setBoundVariable(&midMode, boundVariableType::kInt);
	piParam->setIsDiscreteSwitch(true);
	addPluginParameter(piParam);

	// --- discrete control: Side Mode
	piParam = new PluginParameter(controlID::sideMode, "Side Mode", "Compressor,Expander", "Compressor");
	piParam->setBoundVariable(&sideMode, boundVariableType::kInt);
	piParam->setIsDiscreteSwitch(true);
	addPluginParameter(piParam);

	// --- continuous control: Mid Knee
	piParam = new PluginParameter(controlID::midKnee, "Mid Knee", "dB", controlVariableType::kDouble, 1.000000, 20.000000, 4.000000, taper::kLinearTaper);
	piParam->setParameterSmoothing(false);
	piParam->setSmoothingTimeMsec(100.00);
	piParam->setBoundVariable(&midKnee, boundVariableType::kDouble);
	addPluginParameter(piParam);

	// --- continuous control: Side Knee
	piParam = new PluginParameter(controlID::sideKnee, "Side Knee", "db", controlVariableType::kDouble, 1.000000, 20.000000, 4.000000, taper::kLinearTaper);
	piParam->setParameterSmoothing(false);
	piParam->setSmoothingTimeMsec(100.00);
	piParam->setBoundVariable(&sideKnee, boundVariableType::kDouble);
	addPluginParameter(piParam);

	// --- continuous control: Mid Saturation
	piParam = new PluginParameter(controlID::midSaturation, "Mid Saturation", "", controlVariableType::kDouble, 1.000000, 10.000000, 1.000000, taper::kLinearTaper);
	piParam->setParameterSmoothing(false);
	piParam->setSmoothingTimeMsec(100.00);
	piParam->setBoundVariable(&midSaturation, boundVariableType::kDouble);
	addPluginParameter(piParam);

	// --- continuous control: Side Saturation
	piParam = new PluginParameter(controlID::sideSaturation, "Side Saturation", "", controlVariableType::kDouble, 1.000000, 10.000000, 1.000000, taper::kLinearTaper);
	piParam->setParameterSmoothing(false);
	piParam->setSmoothingTimeMsec(100.00);
	piParam->setBoundVariable(&sideSaturation, boundVariableType::kDouble);
	addPluginParameter(piParam);

	// --- discrete control: Mid Mute
	piParam = new PluginParameter(controlID::midMute, "Mid Mute", "SWITCH OFF,SWITCH ON", "SWITCH OFF");
	piParam->setBoundVariable(&midMute, boundVariableType::kInt);
	piParam->setIsDiscreteSwitch(true);
	addPluginParameter(piParam);

	// --- discrete control: Side Mute
	piParam = new PluginParameter(controlID::sideMute, "Side Mute", "SWITCH OFF,SWITCH ON", "SWITCH OFF");
	piParam->setBoundVariable(&sideMute, boundVariableType::kInt);
	piParam->setIsDiscreteSwitch(true);
	addPluginParameter(piParam);

	// --- discrete control: Mid Solo
	piParam = new PluginParameter(controlID::midSolo, "Mid Solo", "SWITCH OFF,SWITCH ON", "SWITCH OFF");
	piParam->setBoundVariable(&midSolo, boundVariableType::kInt);
	piParam->setIsDiscreteSwitch(true);
	addPluginParameter(piParam);

	// --- discrete control: Side Solo
	piParam = new PluginParameter(controlID::sideSolo, "Side Solo", "SWITCH OFF,SWITCH ON", "SWITCH OFF");
	piParam->setBoundVariable(&sideSolo, boundVariableType::kInt);
	piParam->setIsDiscreteSwitch(true);
	addPluginParameter(piParam);

	// --- discrete control: Mid Limit
	piParam = new PluginParameter(controlID::midHardLimit, "Mid Limit", "SWITCH OFF,SWITCH ON", "SWITCH OFF");
	piParam->setBoundVariable(&midHardLimit, boundVariableType::kInt);
	piParam->setIsDiscreteSwitch(true);
	addPluginParameter(piParam);

	// --- discrete control: Side Limit
	piParam = new PluginParameter(controlID::sideHardLimit, "Side Limit", "SWITCH OFF,SWITCH ON", "SWITCH OFF");
	piParam->setBoundVariable(&sideHardLimit, boundVariableType::kInt);
	piParam->setIsDiscreteSwitch(true);
	addPluginParameter(piParam);

	// --- continuous control: Master Output
	piParam = new PluginParameter(controlID::masterOutput, "Master Output", "dB", controlVariableType::kDouble, -20.000000, 20.000000, 0.000000, taper::kLinearTaper);
	piParam->setParameterSmoothing(false);
	piParam->setSmoothingTimeMsec(100.00);
	piParam->setBoundVariable(&masterOutput, boundVariableType::kDouble);
	addPluginParameter(piParam);

	// --- Aux Attributes
	AuxParameterAttribute auxAttribute;

	// --- RAFX GUI attributes
	// --- controlID::splitLow
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(2147483648);
	setParamAuxAttribute(controlID::splitLow, auxAttribute);

	// --- controlID::splitMid
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(2147483648);
	setParamAuxAttribute(controlID::splitMid, auxAttribute);

	// --- controlID::splitHigh
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(2147483648);
	setParamAuxAttribute(controlID::splitHigh, auxAttribute);

	// --- controlID::threshold1_db
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(2147483648);
	setParamAuxAttribute(controlID::threshold1_db, auxAttribute);

	// --- controlID::ratio1
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(2147483648);
	setParamAuxAttribute(controlID::ratio1, auxAttribute);

	// --- controlID::attack1_ms
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(2147483648);
	setParamAuxAttribute(controlID::attack1_ms, auxAttribute);

	// --- controlID::release1_ms
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(2147483648);
	setParamAuxAttribute(controlID::release1_ms, auxAttribute);

	// --- controlID::gain1_db
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(2147483648);
	setParamAuxAttribute(controlID::gain1_db, auxAttribute);

	// --- controlID::enableMute1
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(1073741826);
	setParamAuxAttribute(controlID::enableMute1, auxAttribute);

	// --- controlID::enableSolo1
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(1073741826);
	setParamAuxAttribute(controlID::enableSolo1, auxAttribute);

	// --- controlID::knee1
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(2147483648);
	setParamAuxAttribute(controlID::knee1, auxAttribute);

	// --- controlID::dryVolume
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(2147483648);
	setParamAuxAttribute(controlID::dryVolume, auxAttribute);

	// --- controlID::hardLimit1
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(1073741824);
	setParamAuxAttribute(controlID::hardLimit1, auxAttribute);

	// --- controlID::mode1
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(805306368);
	setParamAuxAttribute(controlID::mode1, auxAttribute);

	// --- controlID::threshold2_db
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(2147483648);
	setParamAuxAttribute(controlID::threshold2_db, auxAttribute);

	// --- controlID::threshold3_db
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(2147483648);
	setParamAuxAttribute(controlID::threshold3_db, auxAttribute);

	// --- controlID::threshold4_db
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(2147483648);
	setParamAuxAttribute(controlID::threshold4_db, auxAttribute);

	// --- controlID::ratio2
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(2147483648);
	setParamAuxAttribute(controlID::ratio2, auxAttribute);

	// --- controlID::ratio3
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(2147483648);
	setParamAuxAttribute(controlID::ratio3, auxAttribute);

	// --- controlID::ratio4
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(2147483648);
	setParamAuxAttribute(controlID::ratio4, auxAttribute);

	// --- controlID::attack2_ms
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(2147483648);
	setParamAuxAttribute(controlID::attack2_ms, auxAttribute);

	// --- controlID::attack3_ms
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(2147483648);
	setParamAuxAttribute(controlID::attack3_ms, auxAttribute);

	// --- controlID::attack4_ms
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(2147483648);
	setParamAuxAttribute(controlID::attack4_ms, auxAttribute);

	// --- controlID::release2_ms
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(2147483648);
	setParamAuxAttribute(controlID::release2_ms, auxAttribute);

	// --- controlID::release3_ms
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(2147483648);
	setParamAuxAttribute(controlID::release3_ms, auxAttribute);

	// --- controlID::release4_ms
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(2147483648);
	setParamAuxAttribute(controlID::release4_ms, auxAttribute);

	// --- controlID::gain2_db
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(2147483648);
	setParamAuxAttribute(controlID::gain2_db, auxAttribute);

	// --- controlID::gain3_db
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(2147483648);
	setParamAuxAttribute(controlID::gain3_db, auxAttribute);

	// --- controlID::gain4_db
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(2147483648);
	setParamAuxAttribute(controlID::gain4_db, auxAttribute);

	// --- controlID::knee2
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(2147483648);
	setParamAuxAttribute(controlID::knee2, auxAttribute);

	// --- controlID::knee3
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(2147483648);
	setParamAuxAttribute(controlID::knee3, auxAttribute);

	// --- controlID::knee4
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(2147483648);
	setParamAuxAttribute(controlID::knee4, auxAttribute);

	// --- controlID::inputMeter1
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(201326592);
	setParamAuxAttribute(controlID::inputMeter1, auxAttribute);

	// --- controlID::outputMeter1
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(201326592);
	setParamAuxAttribute(controlID::outputMeter1, auxAttribute);

	// --- controlID::reductionMeter1
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(201326592);
	setParamAuxAttribute(controlID::reductionMeter1, auxAttribute);

	// --- controlID::enableMute2
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(1073741826);
	setParamAuxAttribute(controlID::enableMute2, auxAttribute);

	// --- controlID::enableMute3
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(1073741826);
	setParamAuxAttribute(controlID::enableMute3, auxAttribute);

	// --- controlID::enableMute4
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(1073741826);
	setParamAuxAttribute(controlID::enableMute4, auxAttribute);

	// --- controlID::enableSolo2
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(1073741826);
	setParamAuxAttribute(controlID::enableSolo2, auxAttribute);

	// --- controlID::enableSolo3
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(1073741826);
	setParamAuxAttribute(controlID::enableSolo3, auxAttribute);

	// --- controlID::enableSolo4
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(1073741826);
	setParamAuxAttribute(controlID::enableSolo4, auxAttribute);

	// --- controlID::inputMeter2
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(201326592);
	setParamAuxAttribute(controlID::inputMeter2, auxAttribute);

	// --- controlID::inputMeter3
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(201326592);
	setParamAuxAttribute(controlID::inputMeter3, auxAttribute);

	// --- controlID::inputMeter4
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(201326592);
	setParamAuxAttribute(controlID::inputMeter4, auxAttribute);

	// --- controlID::outputMeter2
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(201326592);
	setParamAuxAttribute(controlID::outputMeter2, auxAttribute);

	// --- controlID::outputMeter3
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(201326592);
	setParamAuxAttribute(controlID::outputMeter3, auxAttribute);

	// --- controlID::outputMeter4
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(201326592);
	setParamAuxAttribute(controlID::outputMeter4, auxAttribute);

	// --- controlID::reductionMeter2
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(201326592);
	setParamAuxAttribute(controlID::reductionMeter2, auxAttribute);

	// --- controlID::reductionMeter3
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(201326592);
	setParamAuxAttribute(controlID::reductionMeter3, auxAttribute);

	// --- controlID::reductionMeter4
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(201326592);
	setParamAuxAttribute(controlID::reductionMeter4, auxAttribute);

	// --- controlID::hardLimit2
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(1073741824);
	setParamAuxAttribute(controlID::hardLimit2, auxAttribute);

	// --- controlID::hardLimit3
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(1073741824);
	setParamAuxAttribute(controlID::hardLimit3, auxAttribute);

	// --- controlID::hardLimit4
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(1073741824);
	setParamAuxAttribute(controlID::hardLimit4, auxAttribute);

	// --- controlID::mode2
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(805306368);
	setParamAuxAttribute(controlID::mode2, auxAttribute);

	// --- controlID::mode3
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(805306368);
	setParamAuxAttribute(controlID::mode3, auxAttribute);

	// --- controlID::mode4
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(805306368);
	setParamAuxAttribute(controlID::mode4, auxAttribute);

	// --- controlID::bypass1
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(1073741824);
	setParamAuxAttribute(controlID::bypass1, auxAttribute);

	// --- controlID::bypass2
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(1073741824);
	setParamAuxAttribute(controlID::bypass2, auxAttribute);

	// --- controlID::bypass3
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(1073741824);
	setParamAuxAttribute(controlID::bypass3, auxAttribute);

	// --- controlID::bypass4
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(1073741824);
	setParamAuxAttribute(controlID::bypass4, auxAttribute);

	// --- controlID::masterInputMeter
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(201326592);
	setParamAuxAttribute(controlID::masterInputMeter, auxAttribute);

	// --- controlID::masterOutputMeter
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(201326592);
	setParamAuxAttribute(controlID::masterOutputMeter, auxAttribute);

	// --- controlID::enableSidechain
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(1073741824);
	setParamAuxAttribute(controlID::enableSidechain, auxAttribute);

	// --- controlID::scTargetAll
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(1073741824);
	setParamAuxAttribute(controlID::scTargetAll, auxAttribute);

	// --- controlID::scTarget1
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(1073741824);
	setParamAuxAttribute(controlID::scTarget1, auxAttribute);

	// --- controlID::scTarget2
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(1073741824);
	setParamAuxAttribute(controlID::scTarget2, auxAttribute);

	// --- controlID::scTarget3
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(1073741824);
	setParamAuxAttribute(controlID::scTarget3, auxAttribute);

	// --- controlID::scTarget4
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(1073741824);
	setParamAuxAttribute(controlID::scTarget4, auxAttribute);

	// --- controlID::enableMS
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(1073741824);
	setParamAuxAttribute(controlID::enableMS, auxAttribute);

	// --- controlID::midsideView
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(805306368);
	setParamAuxAttribute(controlID::midsideView, auxAttribute);

	// --- controlID::saturation1
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(2147483648);
	setParamAuxAttribute(controlID::saturation1, auxAttribute);

	// --- controlID::saturation2
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(2147483648);
	setParamAuxAttribute(controlID::saturation2, auxAttribute);

	// --- controlID::saturation3
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(2147483648);
	setParamAuxAttribute(controlID::saturation3, auxAttribute);

	// --- controlID::saturation4
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(2147483648);
	setParamAuxAttribute(controlID::saturation4, auxAttribute);

	// --- controlID::midThreshold
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(2147483648);
	setParamAuxAttribute(controlID::midThreshold, auxAttribute);

	// --- controlID::sideThreshold
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(2147483648);
	setParamAuxAttribute(controlID::sideThreshold, auxAttribute);

	// --- controlID::midRatio
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(2147483648);
	setParamAuxAttribute(controlID::midRatio, auxAttribute);

	// --- controlID::sideRatio
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(2147483648);
	setParamAuxAttribute(controlID::sideRatio, auxAttribute);

	// --- controlID::bandView
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(805306368);
	setParamAuxAttribute(controlID::bandView, auxAttribute);

	// --- controlID::midAttack
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(2147483648);
	setParamAuxAttribute(controlID::midAttack, auxAttribute);

	// --- controlID::midRelease
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(2147483648);
	setParamAuxAttribute(controlID::midRelease, auxAttribute);

	// --- controlID::sideAttack
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(2147483648);
	setParamAuxAttribute(controlID::sideAttack, auxAttribute);

	// --- controlID::sideRelease
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(2147483648);
	setParamAuxAttribute(controlID::sideRelease, auxAttribute);

	// --- controlID::midGain
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(2147483648);
	setParamAuxAttribute(controlID::midGain, auxAttribute);

	// --- controlID::sideGain
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(2147483648);
	setParamAuxAttribute(controlID::sideGain, auxAttribute);

	// --- controlID::midReductionMeter
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(201326592);
	setParamAuxAttribute(controlID::midReductionMeter, auxAttribute);

	// --- controlID::sideReductionMeter
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(201326592);
	setParamAuxAttribute(controlID::sideReductionMeter, auxAttribute);

	// --- controlID::midMode
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(805306368);
	setParamAuxAttribute(controlID::midMode, auxAttribute);

	// --- controlID::sideMode
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(805306368);
	setParamAuxAttribute(controlID::sideMode, auxAttribute);

	// --- controlID::midKnee
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(2147483648);
	setParamAuxAttribute(controlID::midKnee, auxAttribute);

	// --- controlID::sideKnee
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(2147483648);
	setParamAuxAttribute(controlID::sideKnee, auxAttribute);

	// --- controlID::midSaturation
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(2147483648);
	setParamAuxAttribute(controlID::midSaturation, auxAttribute);

	// --- controlID::sideSaturation
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(2147483648);
	setParamAuxAttribute(controlID::sideSaturation, auxAttribute);

	// --- controlID::midMute
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(1073741824);
	setParamAuxAttribute(controlID::midMute, auxAttribute);

	// --- controlID::sideMute
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(1073741824);
	setParamAuxAttribute(controlID::sideMute, auxAttribute);

	// --- controlID::midSolo
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(1073741824);
	setParamAuxAttribute(controlID::midSolo, auxAttribute);

	// --- controlID::sideSolo
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(1073741824);
	setParamAuxAttribute(controlID::sideSolo, auxAttribute);

	// --- controlID::midHardLimit
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(1073741824);
	setParamAuxAttribute(controlID::midHardLimit, auxAttribute);

	// --- controlID::sideHardLimit
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(1073741824);
	setParamAuxAttribute(controlID::sideHardLimit, auxAttribute);

	// --- controlID::masterOutput
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(2147483648);
	setParamAuxAttribute(controlID::masterOutput, auxAttribute);


	// **--0xEDA5--**
   
    // --- BONUS Parameter
    // --- SCALE_GUI_SIZE
    PluginParameter* piParamBonus = new PluginParameter(SCALE_GUI_SIZE, "Scale GUI", "tiny,small,medium,normal,large,giant", "normal");
    addPluginParameter(piParamBonus);

	// --- create the super fast access array
	initPluginParameterArray();

    return true;
}

/**
\brief initialize object for a new run of audio; called just before audio streams

Operation:
- store sample rate and bit depth on audioProcDescriptor - this information is globally available to all core functions
- reset your member objects here

\param resetInfo structure of information about current audio format

\return true if operation succeeds, false otherwise
*/
bool PluginCore::reset(ResetInfo& resetInfo)
{
    // --- save for audio processing
    audioProcDescriptor.sampleRate = resetInfo.sampleRate;
    audioProcDescriptor.bitDepth = resetInfo.bitDepth;

	fourBandDynamics.reset(resetInfo.sampleRate);

    // --- other reset inits
    return PluginBase::reset(resetInfo);
}

/**
\brief one-time initialize function called after object creation and before the first reset( ) call

Operation:
- saves structure for the plugin to use; you can also load WAV files or state information here
*/
bool PluginCore::initialize(PluginInfo& pluginInfo)
{
	// --- add one-time init stuff here

	return true;
}

/**
\brief do anything needed prior to arrival of audio buffers

Operation:
- syncInBoundVariables when preProcessAudioBuffers is called, it is *guaranteed* that all GUI control change information
  has been applied to plugin parameters; this binds parameter changes to your underlying variables
- NOTE: postUpdatePluginParameter( ) will be called for all bound variables that are acutally updated; if you need to process
  them individually, do so in that function
- use this function to bulk-transfer the bound variable data into your plugin's member object variables

\param processInfo structure of information about *buffer* processing

\return true if operation succeeds, false otherwise
*/
bool PluginCore::preProcessAudioBuffers(ProcessBufferInfo& processInfo)
{
    // --- sync internal variables to GUI parameters; you can also do this manually if you don't
    //     want to use the auto-variable-binding
    syncInBoundVariables();

    return true;
}

// --- update parameters
void PluginCore::updateParameters()
{
	FourBandDynamicsParameters params = fourBandDynamics.getParameters();

	params.splitF[0] = splitLow;
	params.splitF[1] = splitMid;
	params.splitF[2] = splitHigh;
	
	params.threshold[0] = threshold1_db;
	params.threshold[1] = threshold2_db;
	params.threshold[2] = threshold3_db;
	params.threshold[3] = threshold4_db;
	params.threshold[4] = midThreshold;
	params.threshold[5] = sideThreshold;

	params.ratio[0] = ratio1;
	params.ratio[1] = ratio2;
	params.ratio[2] = ratio3;
	params.ratio[3] = ratio4;
	params.ratio[4] = midRatio;
	params.ratio[5] = sideRatio;

	params.attack[0] = attack1_ms;
	params.attack[1] = attack2_ms;
	params.attack[2] = attack3_ms;
	params.attack[3] = attack4_ms;
	params.attack[4] = midAttack;
	params.attack[5] = sideAttack;

	params.release[0] = release1_ms;
	params.release[1] = release2_ms;
	params.release[2] = release3_ms;
	params.release[3] = release4_ms;
	params.release[4] = midRelease;
	params.release[5] = sideRelease;

	params.gain[0] = gain1_db;
	params.gain[1] = gain2_db;
	params.gain[2] = gain3_db;
	params.gain[3] = gain4_db;
	params.gain[4] = midGain;
	params.gain[5] = sideGain;

	params.knee[0] = knee1;
	params.knee[1] = knee2;
	params.knee[2] = knee3;
	params.knee[3] = knee4;
	params.knee[4] = midKnee;
	params.knee[5] = sideKnee;

	params.hardLimitGate[0] = hardLimit1;
	params.hardLimitGate[1] = hardLimit2;
	params.hardLimitGate[2] = hardLimit3;
	params.hardLimitGate[3] = hardLimit4;
	params.hardLimitGate[4] = midHardLimit;
	params.hardLimitGate[5] = sideHardLimit;

	params.dynamicsMode[0] = convertIntToEnum(mode1, modeSelection);
	params.dynamicsMode[1] = convertIntToEnum(mode2, modeSelection);
	params.dynamicsMode[2] = convertIntToEnum(mode3, modeSelection);
	params.dynamicsMode[3] = convertIntToEnum(mode4, modeSelection);
	params.dynamicsMode[4] = convertIntToEnum(midMode, modeSelection);
	params.dynamicsMode[5] = convertIntToEnum(sideMode, modeSelection);

	params.enableMute[0] = enableMute1;
	params.enableMute[1] = enableMute2;
	params.enableMute[2] = enableMute3;
	params.enableMute[3] = enableMute4;
	params.enableMute[4] = midMute;
	params.enableMute[5] = sideMute;

	params.enableSolo[0] = enableSolo1;
	params.enableSolo[1] = enableSolo2;
	params.enableSolo[2] = enableSolo3;
	params.enableSolo[3] = enableSolo4;
	params.enableSolo[4] = midSolo;
	params.enableSolo[5] = sideSolo;

	params.saturation[0] = saturation1;
	params.saturation[1] = saturation2;
	params.saturation[2] = saturation3;
	params.saturation[3] = saturation4;
	params.saturation[4] = midSaturation;
	params.saturation[5] = sideSaturation;

	params.enableMS = enableMS;
	params.msView = convertIntToEnum(midsideView, msSelection);

	params.enableSidechain = enableSidechain;
	params.scTarget[0] = scTarget1;
	params.scTarget[1] = scTarget2;
	params.scTarget[2] = scTarget3;
	params.scTarget[3] = scTarget4;

	params.dryVolume = dryVolume;

	params.masterOutputVolume = masterOutput;


	fourBandDynamics.setParameters(params);
}

/**
\brief frame-processing method

Operation:
- decode the plugin type - for synth plugins, fill in the rendering code; for FX plugins, delete the if(synth) portion and add your processing code
- note that MIDI events are fired for each sample interval so that MIDI is tightly sunk with audio
- doSampleAccurateParameterUpdates will perform per-sample interval smoothing

\param processFrameInfo structure of information about *frame* processing

\return true if operation succeeds, false otherwise
*/
bool PluginCore::processAudioFrame(ProcessFrameInfo& processFrameInfo)
{
    // --- fire any MIDI events for this sample interval
    processFrameInfo.midiEventQueue->fireMidiEvents(processFrameInfo.currentFrame);

	// --- do per-frame updates; VST automation and parameter smoothing
	doSampleAccurateParameterUpdates();


	updateParameters();

	bool processed = fourBandDynamics.processAudioFrame(processFrameInfo.audioInputFrame,
														processFrameInfo.audioOutputFrame,
														processFrameInfo.numAudioInChannels,
														processFrameInfo.numAudioOutChannels);

	if (processed)
	{
		FourBandDynamicsParameters params = fourBandDynamics.getParameters();

		inputMeter1 = params.inputMeter[0];
		inputMeter2 = params.inputMeter[1];
		inputMeter3 = params.inputMeter[2];
		inputMeter4 = params.inputMeter[3];

		outputMeter1 = params.outputMeter[0];
		outputMeter2 = params.outputMeter[1];
		outputMeter3 = params.outputMeter[2];
		outputMeter4 = params.outputMeter[3];

		reductionMeter1 = params.reductionMeter[0];
		reductionMeter2 = params.reductionMeter[1];
		reductionMeter3 = params.reductionMeter[2];
		reductionMeter4 = params.reductionMeter[3];
		midReductionMeter = params.reductionMeter[4];
		sideReductionMeter = params.reductionMeter[5];

		masterInputMeter = params.masterInputMeter;
		masterOutputMeter = params.masterOutputMeter;
	}

	return processed;
}


/**
\brief do anything needed prior to arrival of audio buffers

Operation:
- updateOutBoundVariables sends metering data to the GUI meters

\param processInfo structure of information about *buffer* processing

\return true if operation succeeds, false otherwise
*/
bool PluginCore::postProcessAudioBuffers(ProcessBufferInfo& processInfo)
{
	// --- update outbound variables; currently this is meter data only, but could be extended
	//     in the future
	updateOutBoundVariables();

    return true;
}

/**
\brief update the PluginParameter's value based on GUI control, preset, or data smoothing (thread-safe)

Operation:
- update the parameter's value (with smoothing this initiates another smoothing process)
- call postUpdatePluginParameter to do any further processing

\param controlID the control ID value of the parameter being updated
\param controlValue the new control value
\param paramInfo structure of information about why this value is being udpated (e.g as a result of a preset being loaded vs. the top of a buffer process cycle)

\return true if operation succeeds, false otherwise
*/
bool PluginCore::updatePluginParameter(int32_t controlID, double controlValue, ParameterUpdateInfo& paramInfo)
{
    // --- use base class helper
    setPIParamValue(controlID, controlValue);

    // --- do any post-processing
    postUpdatePluginParameter(controlID, controlValue, paramInfo);

    return true; /// handled
}

/**
\brief update the PluginParameter's value based on *normlaized* GUI control, preset, or data smoothing (thread-safe)

Operation:
- update the parameter's value (with smoothing this initiates another smoothing process)
- call postUpdatePluginParameter to do any further processing

\param controlID the control ID value of the parameter being updated
\param normalizedValue the new control value in normalized form
\param paramInfo structure of information about why this value is being udpated (e.g as a result of a preset being loaded vs. the top of a buffer process cycle)

\return true if operation succeeds, false otherwise
*/
bool PluginCore::updatePluginParameterNormalized(int32_t controlID, double normalizedValue, ParameterUpdateInfo& paramInfo)
{
	// --- use base class helper, returns actual value
	double controlValue = setPIParamValueNormalized(controlID, normalizedValue, paramInfo.applyTaper);

	// --- do any post-processing
	postUpdatePluginParameter(controlID, controlValue, paramInfo);

	return true; /// handled
}

/**
\brief perform any operations after the plugin parameter has been updated; this is one paradigm for
	   transferring control information into vital plugin variables or member objects. If you use this
	   method you can decode the control ID and then do any cooking that is needed. NOTE: do not
	   overwrite bound variables here - this is ONLY for any extra cooking that is required to convert
	   the GUI data to meaninful coefficients or other specific modifiers.

\param controlID the control ID value of the parameter being updated
\param controlValue the new control value
\param paramInfo structure of information about why this value is being udpated (e.g as a result of a preset being loaded vs. the top of a buffer process cycle)

\return true if operation succeeds, false otherwise
*/
bool PluginCore::postUpdatePluginParameter(int32_t controlID, double controlValue, ParameterUpdateInfo& paramInfo)
{
    // --- now do any post update cooking; be careful with VST Sample Accurate automation
    //     If enabled, then make sure the cooking functions are short and efficient otherwise disable it
    //     for the Parameter involved
    /*switch(controlID)
    {
        case 0:
        {
            return true;    /// handled
        }

        default:
            return false;   /// not handled
    }*/

    return false;
}

/**
\brief has nothing to do with actual variable or updated variable (binding)

CAUTION:
- DO NOT update underlying variables here - this is only for sending GUI updates or letting you
  know that a parameter was changed; it should not change the state of your plugin.

WARNING:
- THIS IS NOT THE PREFERRED WAY TO LINK OR COMBINE CONTROLS TOGETHER. THE PROPER METHOD IS
  TO USE A CUSTOM SUB-CONTROLLER THAT IS PART OF THE GUI OBJECT AND CODE.
  SEE http://www.willpirkle.com for more information

\param controlID the control ID value of the parameter being updated
\param actualValue the new control value

\return true if operation succeeds, false otherwise
*/
bool PluginCore::guiParameterChanged(int32_t controlID, double actualValue)
{
	/*
	switch (controlID)
	{
		case controlID::<your control here>
		{

			return true; // handled
		}

		default:
			break;
	}*/

	return false; /// not handled
}

/**
\brief For Custom View and Custom Sub-Controller Operations

NOTES:
- this is for advanced users only to implement custom view and custom sub-controllers
- see the SDK for examples of use

\param messageInfo a structure containing information about the incoming message

\return true if operation succeeds, false otherwise
*/
bool PluginCore::processMessage(MessageInfo& messageInfo)
{
	// --- decode message
	switch (messageInfo.message)
	{
		// --- add customization appearance here
	case PLUGINGUI_DIDOPEN:
	{
		return false;
	}

	// --- NULL pointers so that we don't accidentally use them
	case PLUGINGUI_WILLCLOSE:
	{
		return false;
	}

	// --- update view; this will only be called if the GUI is actually open
	case PLUGINGUI_TIMERPING:
	{
		return false;
	}

	// --- register the custom view, grab the ICustomView interface
	case PLUGINGUI_REGISTER_CUSTOMVIEW:
	{

		return false;
	}

	case PLUGINGUI_REGISTER_SUBCONTROLLER:
	case PLUGINGUI_QUERY_HASUSERCUSTOM:
	case PLUGINGUI_USER_CUSTOMOPEN:
	case PLUGINGUI_USER_CUSTOMCLOSE:
	case PLUGINGUI_EXTERNAL_SET_NORMVALUE:
	case PLUGINGUI_EXTERNAL_SET_ACTUALVALUE:
	{

		return false;
	}

	default:
		break;
	}

	return false; /// not handled
}


/**
\brief process a MIDI event

NOTES:
- MIDI events are 100% sample accurate; this function will be called repeatedly for every MIDI message
- see the SDK for examples of use

\param event a structure containing the MIDI event data

\return true if operation succeeds, false otherwise
*/
bool PluginCore::processMIDIEvent(midiEvent& event)
{
	return true;
}

/**
\brief (for future use)

NOTES:
- MIDI events are 100% sample accurate; this function will be called repeatedly for every MIDI message
- see the SDK for examples of use

\param vectorJoysickData a structure containing joystick data

\return true if operation succeeds, false otherwise
*/
bool PluginCore::setVectorJoystickParameters(const VectorJoystickData& vectorJoysickData)
{
	return true;
}

/**
\brief use this method to add new presets to the list

NOTES:
- see the SDK for examples of use
- for non RackAFX users that have large paramter counts, there is a secret GUI control you
  can enable to write C++ code into text files, one per preset. See the SDK or http://www.willpirkle.com for details

\return true if operation succeeds, false otherwise
*/
bool PluginCore::initPluginPresets()
{
	// **--0xFF7A--**

	// --- Plugin Presets 
	int index = 0;
	PresetInfo* preset = nullptr;

	// --- Preset: Factory Preset
	preset = new PresetInfo(index++, "Factory Preset");
	initPresetParameters(preset->presetParameters);
	setPresetParameter(preset->presetParameters, controlID::splitLow, 200.000000);
	setPresetParameter(preset->presetParameters, controlID::splitMid, 1000.000000);
	setPresetParameter(preset->presetParameters, controlID::splitHigh, 10000.000000);
	setPresetParameter(preset->presetParameters, controlID::threshold1_db, -10.000000);
	setPresetParameter(preset->presetParameters, controlID::ratio1, 4.000000);
	setPresetParameter(preset->presetParameters, controlID::attack1_ms, 5.000000);
	setPresetParameter(preset->presetParameters, controlID::release1_ms, 200.000000);
	setPresetParameter(preset->presetParameters, controlID::gain1_db, -3.000000);
	setPresetParameter(preset->presetParameters, controlID::enableMute1, -0.000000);
	setPresetParameter(preset->presetParameters, controlID::enableSolo1, -0.000000);
	setPresetParameter(preset->presetParameters, controlID::knee1, 5.000000);
	setPresetParameter(preset->presetParameters, controlID::dryVolume, -60.000000);
	setPresetParameter(preset->presetParameters, controlID::hardLimit1, -0.000000);
	setPresetParameter(preset->presetParameters, controlID::mode1, -0.000000);
	setPresetParameter(preset->presetParameters, controlID::threshold2_db, -10.000000);
	setPresetParameter(preset->presetParameters, controlID::threshold3_db, -10.000000);
	setPresetParameter(preset->presetParameters, controlID::threshold4_db, -10.000000);
	setPresetParameter(preset->presetParameters, controlID::ratio2, 4.000000);
	setPresetParameter(preset->presetParameters, controlID::ratio3, 4.000000);
	setPresetParameter(preset->presetParameters, controlID::ratio4, 4.000000);
	setPresetParameter(preset->presetParameters, controlID::attack2_ms, 5.000000);
	setPresetParameter(preset->presetParameters, controlID::attack3_ms, 5.000000);
	setPresetParameter(preset->presetParameters, controlID::attack4_ms, 5.000000);
	setPresetParameter(preset->presetParameters, controlID::release2_ms, 200.000000);
	setPresetParameter(preset->presetParameters, controlID::release3_ms, 200.000000);
	setPresetParameter(preset->presetParameters, controlID::release4_ms, 200.000000);
	setPresetParameter(preset->presetParameters, controlID::gain2_db, -3.000000);
	setPresetParameter(preset->presetParameters, controlID::gain3_db, -3.000000);
	setPresetParameter(preset->presetParameters, controlID::gain4_db, -3.000000);
	setPresetParameter(preset->presetParameters, controlID::knee2, 5.000000);
	setPresetParameter(preset->presetParameters, controlID::knee3, 5.000000);
	setPresetParameter(preset->presetParameters, controlID::knee4, 5.000000);
	setPresetParameter(preset->presetParameters, controlID::enableMute2, -0.000000);
	setPresetParameter(preset->presetParameters, controlID::enableMute3, -0.000000);
	setPresetParameter(preset->presetParameters, controlID::enableMute4, -0.000000);
	setPresetParameter(preset->presetParameters, controlID::enableSolo2, -0.000000);
	setPresetParameter(preset->presetParameters, controlID::enableSolo3, -0.000000);
	setPresetParameter(preset->presetParameters, controlID::enableSolo4, -0.000000);
	setPresetParameter(preset->presetParameters, controlID::hardLimit2, -0.000000);
	setPresetParameter(preset->presetParameters, controlID::hardLimit3, -0.000000);
	setPresetParameter(preset->presetParameters, controlID::hardLimit4, -0.000000);
	setPresetParameter(preset->presetParameters, controlID::mode2, -0.000000);
	setPresetParameter(preset->presetParameters, controlID::mode3, -0.000000);
	setPresetParameter(preset->presetParameters, controlID::mode4, -0.000000);
	setPresetParameter(preset->presetParameters, controlID::bypass1, -0.000000);
	setPresetParameter(preset->presetParameters, controlID::bypass2, -0.000000);
	setPresetParameter(preset->presetParameters, controlID::bypass3, -0.000000);
	setPresetParameter(preset->presetParameters, controlID::bypass4, -0.000000);
	setPresetParameter(preset->presetParameters, controlID::enableSidechain, -0.000000);
	setPresetParameter(preset->presetParameters, controlID::scTargetAll, -0.000000);
	setPresetParameter(preset->presetParameters, controlID::scTarget1, -0.000000);
	setPresetParameter(preset->presetParameters, controlID::scTarget2, -0.000000);
	setPresetParameter(preset->presetParameters, controlID::scTarget3, -0.000000);
	setPresetParameter(preset->presetParameters, controlID::scTarget4, -0.000000);
	setPresetParameter(preset->presetParameters, controlID::enableMS, -0.000000);
	setPresetParameter(preset->presetParameters, controlID::midsideView, -0.000000);
	setPresetParameter(preset->presetParameters, controlID::saturation1, 1.000000);
	setPresetParameter(preset->presetParameters, controlID::saturation2, 1.000000);
	setPresetParameter(preset->presetParameters, controlID::saturation3, 1.000000);
	setPresetParameter(preset->presetParameters, controlID::saturation4, 1.000000);
	setPresetParameter(preset->presetParameters, controlID::midThreshold, -10.000000);
	setPresetParameter(preset->presetParameters, controlID::sideThreshold, -10.000000);
	setPresetParameter(preset->presetParameters, controlID::midRatio, 4.000000);
	setPresetParameter(preset->presetParameters, controlID::sideRatio, 4.000000);
	setPresetParameter(preset->presetParameters, controlID::bandView, -0.000000);
	setPresetParameter(preset->presetParameters, controlID::midAttack, 5.000000);
	setPresetParameter(preset->presetParameters, controlID::midRelease, 250.000000);
	setPresetParameter(preset->presetParameters, controlID::sideAttack, 5.000000);
	setPresetParameter(preset->presetParameters, controlID::sideRelease, 250.000000);
	setPresetParameter(preset->presetParameters, controlID::midGain, -6.000000);
	setPresetParameter(preset->presetParameters, controlID::sideGain, -6.000000);
	setPresetParameter(preset->presetParameters, controlID::midMode, -0.000000);
	setPresetParameter(preset->presetParameters, controlID::sideMode, -0.000000);
	setPresetParameter(preset->presetParameters, controlID::midKnee, 4.000000);
	setPresetParameter(preset->presetParameters, controlID::sideKnee, 4.000000);
	setPresetParameter(preset->presetParameters, controlID::midSaturation, 1.000000);
	setPresetParameter(preset->presetParameters, controlID::sideSaturation, 1.000000);
	setPresetParameter(preset->presetParameters, controlID::midMute, -0.000000);
	setPresetParameter(preset->presetParameters, controlID::sideMute, -0.000000);
	setPresetParameter(preset->presetParameters, controlID::midSolo, -0.000000);
	setPresetParameter(preset->presetParameters, controlID::sideSolo, -0.000000);
	setPresetParameter(preset->presetParameters, controlID::midHardLimit, -0.000000);
	setPresetParameter(preset->presetParameters, controlID::sideHardLimit, -0.000000);
	setPresetParameter(preset->presetParameters, controlID::masterOutput, 0.000000);
	addPreset(preset);


	// **--0xA7FF--**

    return true;
}

/**
\brief setup the plugin description strings, flags and codes; this is ordinarily done through the ASPiKreator or CMake

\return true if operation succeeds, false otherwise
*/
bool PluginCore::initPluginDescriptors()
{
    pluginDescriptor.pluginName = PluginCore::getPluginName();
    pluginDescriptor.shortPluginName = PluginCore::getShortPluginName();
    pluginDescriptor.vendorName = PluginCore::getVendorName();
    pluginDescriptor.pluginTypeCode = PluginCore::getPluginType();

	// --- describe the plugin attributes; set according to your needs
	pluginDescriptor.hasSidechain = kWantSidechain;
	pluginDescriptor.latencyInSamples = kLatencyInSamples;
	pluginDescriptor.tailTimeInMSec = kTailTimeMsec;
	pluginDescriptor.infiniteTailVST3 = kVSTInfiniteTail;

    // --- AAX
    apiSpecificInfo.aaxManufacturerID = kManufacturerID;
    apiSpecificInfo.aaxProductID = kAAXProductID;
    apiSpecificInfo.aaxBundleID = kAAXBundleID;  /* MacOS only: this MUST match the bundle identifier in your info.plist file */
    apiSpecificInfo.aaxEffectID = "aaxDeveloper.";
    apiSpecificInfo.aaxEffectID.append(PluginCore::getPluginName());
    apiSpecificInfo.aaxPluginCategoryCode = kAAXCategory;

    // --- AU
    apiSpecificInfo.auBundleID = kAUBundleID;   /* MacOS only: this MUST match the bundle identifier in your info.plist file */

    // --- VST3
    apiSpecificInfo.vst3FUID = PluginCore::getVSTFUID(); // OLE string format
    apiSpecificInfo.vst3BundleID = kVST3BundleID;/* MacOS only: this MUST match the bundle identifier in your info.plist file */
	apiSpecificInfo.enableVST3SampleAccurateAutomation = kVSTSAA;
	apiSpecificInfo.vst3SampleAccurateGranularity = kVST3SAAGranularity;

    // --- AU and AAX
    apiSpecificInfo.fourCharCode = PluginCore::getFourCharCode();

    return true;
}

// --- static functions required for VST3/AU only --------------------------------------------- //
const char* PluginCore::getPluginBundleName() { return kAUBundleName; }
const char* PluginCore::getPluginName(){ return kPluginName; }
const char* PluginCore::getShortPluginName(){ return kShortPluginName; }
const char* PluginCore::getVendorName(){ return kVendorName; }
const char* PluginCore::getVendorURL(){ return kVendorURL; }
const char* PluginCore::getVendorEmail(){ return kVendorEmail; }
const char* PluginCore::getAUCocoaViewFactoryName(){ return AU_COCOA_VIEWFACTORY_STRING; }
pluginType PluginCore::getPluginType(){ return kPluginType; }
const char* PluginCore::getVSTFUID(){ return kVSTFUID; }
int32_t PluginCore::getFourCharCode(){ return kFourCharCode; }
