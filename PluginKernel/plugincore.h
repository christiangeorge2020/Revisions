// -----------------------------------------------------------------------------
//    ASPiK Plugin Kernel File:  plugincore.h
//
/**
    \file   plugincore.h
    \author Will Pirkle
    \date   17-September-2018
    \brief  base class interface file for ASPiK plugincore object
    		- http://www.aspikplugins.com
    		- http://www.willpirkle.com
*/
// -----------------------------------------------------------------------------
#ifndef __pluginCore_h__
#define __pluginCore_h__

#include "pluginbase.h"
#include "fourbanddynamics.h"

// **--0x7F1F--**

// --- Plugin Variables controlID Enumeration 

enum controlID {
	splitLow = 2,
	splitMid = 4,
	splitHigh = 6,
	threshold1_db = 11,
	ratio1 = 21,
	attack1_ms = 31,
	release1_ms = 41,
	gain1_db = 51,
	enableMute1 = 8,
	enableSolo1 = 9,
	knee1 = 61,
	dryVolume = 49,
	hardLimit1 = 10,
	mode1 = 20,
	threshold2_db = 13,
	threshold3_db = 15,
	threshold4_db = 17,
	ratio2 = 23,
	ratio3 = 25,
	ratio4 = 27,
	attack2_ms = 33,
	attack3_ms = 35,
	attack4_ms = 37,
	release2_ms = 43,
	release3_ms = 45,
	release4_ms = 47,
	gain2_db = 53,
	gain3_db = 55,
	gain4_db = 57,
	knee2 = 63,
	knee3 = 65,
	knee4 = 67,
	inputMeter1 = 30,
	outputMeter1 = 40,
	reductionMeter1 = 50,
	enableMute2 = 18,
	enableMute3 = 28,
	enableMute4 = 38,
	enableSolo2 = 19,
	enableSolo3 = 29,
	enableSolo4 = 39,
	inputMeter2 = 32,
	inputMeter3 = 34,
	inputMeter4 = 36,
	outputMeter2 = 42,
	outputMeter3 = 44,
	outputMeter4 = 46,
	reductionMeter2 = 52,
	reductionMeter3 = 54,
	reductionMeter4 = 56,
	hardLimit2 = 12,
	hardLimit3 = 14,
	hardLimit4 = 16,
	mode2 = 22,
	mode3 = 24,
	mode4 = 26,
	bypass1 = 60,
	bypass2 = 62,
	bypass3 = 64,
	bypass4 = 66,
	masterInputMeter = 58,
	masterOutputMeter = 59,
	enableSidechain = 88,
	scTargetAll = 89,
	scTarget1 = 99,
	scTarget2 = 109,
	scTarget3 = 119,
	scTarget4 = 129,
	enableMS = 81,
	midsideView = 82,
	saturation1 = 71,
	saturation2 = 73,
	saturation3 = 75,
	saturation4 = 77,
	midThreshold = 91,
	sideThreshold = 92,
	midRatio = 101,
	sideRatio = 102,
	bandView = 48,
	midAttack = 111,
	midRelease = 121,
	sideAttack = 112,
	sideRelease = 122,
	midGain = 131,
	sideGain = 132,
	midReductionMeter = 90,
	sideReductionMeter = 93,
	midMode = 100,
	sideMode = 103,
	midKnee = 141,
	sideKnee = 142,
	midSaturation = 151,
	sideSaturation = 152,
	midMute = 105,
	sideMute = 115,
	midSolo = 104,
	sideSolo = 114,
	midHardLimit = 110,
	sideHardLimit = 113,
	masterOutput = 69
};

	// **--0x0F1F--**

/**
\class PluginCore
\ingroup ASPiK-Core
\brief
The PluginCore object is the default PluginBase derived object for ASPiK projects.
Note that you are fre to change the name of this object (as long as you change it in the compiler settings, etc...)


PluginCore Operations:
- overrides the main processing functions from the base class
- performs reset operation on sub-modules
- processes audio
- processes messages for custom views
- performs pre and post processing functions on parameters and audio (if needed)

\author Will Pirkle http://www.willpirkle.com
\remark This object is included in Designing Audio Effects Plugins in C++ 2nd Ed. by Will Pirkle
\version Revision : 1.0
\date Date : 2018 / 09 / 7
*/
class PluginCore : public PluginBase
{
public:
    PluginCore();

	/** Destructor: empty in default version */
    virtual ~PluginCore(){}

	// --- PluginBase Overrides ---
	//
	/** this is the creation function for all plugin parameters */
	bool initPluginParameters();

	/** called when plugin is loaded, a new audio file is playing or sample rate changes */
	virtual bool reset(ResetInfo& resetInfo);

	/** one-time post creation init function; pluginInfo contains path to this plugin */
	virtual bool initialize(PluginInfo& _pluginInfo);

	// --- preProcess: sync GUI parameters here; override if you don't want to use automatic variable-binding
	virtual bool preProcessAudioBuffers(ProcessBufferInfo& processInfo);

	/** process frames of data */
	virtual bool processAudioFrame(ProcessFrameInfo& processFrameInfo);

	// --- uncomment and override this for buffer processing; see base class implementation for
	//     help on breaking up buffers and getting info from processBufferInfo
	//virtual bool processAudioBuffers(ProcessBufferInfo& processBufferInfo);

	/** preProcess: do any post-buffer processing required; default operation is to send metering data to GUI  */
	virtual bool postProcessAudioBuffers(ProcessBufferInfo& processInfo);

	/** called by host plugin at top of buffer proccess; this alters parameters prior to variable binding operation  */
	virtual bool updatePluginParameter(int32_t controlID, double controlValue, ParameterUpdateInfo& paramInfo);

	/** called by host plugin at top of buffer proccess; this alters parameters prior to variable binding operation  */
	virtual bool updatePluginParameterNormalized(int32_t controlID, double normalizedValue, ParameterUpdateInfo& paramInfo);

	/** this can be called: 1) after bound variable has been updated or 2) after smoothing occurs  */
	virtual bool postUpdatePluginParameter(int32_t controlID, double controlValue, ParameterUpdateInfo& paramInfo);

	/** this is ony called when the user makes a GUI control change */
	virtual bool guiParameterChanged(int32_t controlID, double actualValue);

	/** processMessage: messaging system; currently used for custom/special GUI operations */
	virtual bool processMessage(MessageInfo& messageInfo);

	/** processMIDIEvent: MIDI event processing */
	virtual bool processMIDIEvent(midiEvent& event);

	/** specialized joystick servicing (currently not used) */
	virtual bool setVectorJoystickParameters(const VectorJoystickData& vectorJoysickData);

	/** create the presets */
	bool initPluginPresets();

	// --- BEGIN USER VARIABLES AND FUNCTIONS -------------------------------------- //
	//	   Add your variables and methods here
	FourBandDynamics fourBandDynamics;
	void updateParameters();


	// --- END USER VARIABLES AND FUNCTIONS -------------------------------------- //

private:
	//  **--0x07FD--**

	// --- Continuous Plugin Variables 
	double splitLow = 0.0;
	double splitMid = 0.0;
	double splitHigh = 0.0;
	double threshold1_db = 0.0;
	double ratio1 = 0.0;
	double attack1_ms = 0.0;
	double release1_ms = 0.0;
	double gain1_db = 0.0;
	double knee1 = 0.0;
	double dryVolume = 0.0;
	double threshold2_db = 0.0;
	double threshold3_db = 0.0;
	double threshold4_db = 0.0;
	double ratio2 = 0.0;
	double ratio3 = 0.0;
	double ratio4 = 0.0;
	double attack2_ms = 0.0;
	double attack3_ms = 0.0;
	double attack4_ms = 0.0;
	double release2_ms = 0.0;
	double release3_ms = 0.0;
	double release4_ms = 0.0;
	double gain2_db = 0.0;
	double gain3_db = 0.0;
	double gain4_db = 0.0;
	double knee2 = 0.0;
	double knee3 = 0.0;
	double knee4 = 0.0;
	double saturation1 = 0.0;
	double saturation2 = 0.0;
	double saturation3 = 0.0;
	double saturation4 = 0.0;
	double midThreshold = 0.0;
	double sideThreshold = 0.0;
	double midRatio = 0.0;
	double sideRatio = 0.0;
	double midAttack = 0.0;
	double midRelease = 0.0;
	double sideAttack = 0.0;
	double sideRelease = 0.0;
	double midGain = 0.0;
	double sideGain = 0.0;
	double midKnee = 0.0;
	double sideKnee = 0.0;
	double midSaturation = 0.0;
	double sideSaturation = 0.0;
	double masterOutput = 0.0;

	// --- Discrete Plugin Variables 
	int enableMute1 = 0;
	enum class enableMute1Enum { SWITCH_OFF,SWITCH_ON };	// to compare: if(compareEnumToInt(enableMute1Enum::SWITCH_OFF, enableMute1)) etc... 

	int enableSolo1 = 0;
	enum class enableSolo1Enum { SWITCH_OFF,SWITCH_ON };	// to compare: if(compareEnumToInt(enableSolo1Enum::SWITCH_OFF, enableSolo1)) etc... 

	int hardLimit1 = 0;
	enum class hardLimit1Enum { SWITCH_OFF,SWITCH_ON };	// to compare: if(compareEnumToInt(hardLimit1Enum::SWITCH_OFF, hardLimit1)) etc... 

	int mode1 = 0;
	enum class mode1Enum { Compressor,Expander };	// to compare: if(compareEnumToInt(mode1Enum::Compressor, mode1)) etc... 

	int enableMute2 = 0;
	enum class enableMute2Enum { SWITCH_OFF,SWITCH_ON };	// to compare: if(compareEnumToInt(enableMute2Enum::SWITCH_OFF, enableMute2)) etc... 

	int enableMute3 = 0;
	enum class enableMute3Enum { SWITCH_OFF,SWITCH_ON };	// to compare: if(compareEnumToInt(enableMute3Enum::SWITCH_OFF, enableMute3)) etc... 

	int enableMute4 = 0;
	enum class enableMute4Enum { SWITCH_OFF,SWITCH_ON };	// to compare: if(compareEnumToInt(enableMute4Enum::SWITCH_OFF, enableMute4)) etc... 

	int enableSolo2 = 0;
	enum class enableSolo2Enum { SWITCH_OFF,SWITCH_ON };	// to compare: if(compareEnumToInt(enableSolo2Enum::SWITCH_OFF, enableSolo2)) etc... 

	int enableSolo3 = 0;
	enum class enableSolo3Enum { SWITCH_OFF,SWITCH_ON };	// to compare: if(compareEnumToInt(enableSolo3Enum::SWITCH_OFF, enableSolo3)) etc... 

	int enableSolo4 = 0;
	enum class enableSolo4Enum { SWITCH_OFF,SWITCH_ON };	// to compare: if(compareEnumToInt(enableSolo4Enum::SWITCH_OFF, enableSolo4)) etc... 

	int hardLimit2 = 0;
	enum class hardLimit2Enum { SWITCH_OFF,SWITCH_ON };	// to compare: if(compareEnumToInt(hardLimit2Enum::SWITCH_OFF, hardLimit2)) etc... 

	int hardLimit3 = 0;
	enum class hardLimit3Enum { SWITCH_OFF,SWITCH_ON };	// to compare: if(compareEnumToInt(hardLimit3Enum::SWITCH_OFF, hardLimit3)) etc... 

	int hardLimit4 = 0;
	enum class hardLimit4Enum { SWITCH_OFF,SWITCH_ON };	// to compare: if(compareEnumToInt(hardLimit4Enum::SWITCH_OFF, hardLimit4)) etc... 

	int mode2 = 0;
	enum class mode2Enum { Compressor,Expander };	// to compare: if(compareEnumToInt(mode2Enum::Compressor, mode2)) etc... 

	int mode3 = 0;
	enum class mode3Enum { Compressor,Expander };	// to compare: if(compareEnumToInt(mode3Enum::Compressor, mode3)) etc... 

	int mode4 = 0;
	enum class mode4Enum { Compressor,Expander };	// to compare: if(compareEnumToInt(mode4Enum::Compressor, mode4)) etc... 

	int bypass1 = 0;
	enum class bypass1Enum { SWITCH_OFF,SWITCH_ON };	// to compare: if(compareEnumToInt(bypass1Enum::SWITCH_OFF, bypass1)) etc... 

	int bypass2 = 0;
	enum class bypass2Enum { SWITCH_OFF,SWITCH_ON };	// to compare: if(compareEnumToInt(bypass2Enum::SWITCH_OFF, bypass2)) etc... 

	int bypass3 = 0;
	enum class bypass3Enum { SWITCH_OFF,SWITCH_ON };	// to compare: if(compareEnumToInt(bypass3Enum::SWITCH_OFF, bypass3)) etc... 

	int bypass4 = 0;
	enum class bypass4Enum { SWITCH_OFF,SWITCH_ON };	// to compare: if(compareEnumToInt(bypass4Enum::SWITCH_OFF, bypass4)) etc... 

	int enableSidechain = 0;
	enum class enableSidechainEnum { SWITCH_OFF,SWITCH_ON };	// to compare: if(compareEnumToInt(enableSidechainEnum::SWITCH_OFF, enableSidechain)) etc... 

	int scTargetAll = 0;
	enum class scTargetAllEnum { SWITCH_OFF,SWITCH_ON };	// to compare: if(compareEnumToInt(scTargetAllEnum::SWITCH_OFF, scTargetAll)) etc... 

	int scTarget1 = 0;
	enum class scTarget1Enum { SWITCH_OFF,SWITCH_ON };	// to compare: if(compareEnumToInt(scTarget1Enum::SWITCH_OFF, scTarget1)) etc... 

	int scTarget2 = 0;
	enum class scTarget2Enum { SWITCH_OFF,SWITCH_ON };	// to compare: if(compareEnumToInt(scTarget2Enum::SWITCH_OFF, scTarget2)) etc... 

	int scTarget3 = 0;
	enum class scTarget3Enum { SWITCH_OFF,SWITCH_ON };	// to compare: if(compareEnumToInt(scTarget3Enum::SWITCH_OFF, scTarget3)) etc... 

	int scTarget4 = 0;
	enum class scTarget4Enum { SWITCH_OFF,SWITCH_ON };	// to compare: if(compareEnumToInt(scTarget4Enum::SWITCH_OFF, scTarget4)) etc... 

	int enableMS = 0;
	enum class enableMSEnum { SWITCH_OFF,SWITCH_ON };	// to compare: if(compareEnumToInt(enableMSEnum::SWITCH_OFF, enableMS)) etc... 

	int midsideView = 0;
	enum class midsideViewEnum { Summed,Mid,Side };	// to compare: if(compareEnumToInt(midsideViewEnum::Summed, midsideView)) etc... 

	int bandView = 0;
	enum class bandViewEnum { Low,Lowband,Midband,High,Mid,Side };	// to compare: if(compareEnumToInt(bandViewEnum::Low, bandView)) etc... 

	int midMode = 0;
	enum class midModeEnum { Compressor,Expander };	// to compare: if(compareEnumToInt(midModeEnum::Compressor, midMode)) etc... 

	int sideMode = 0;
	enum class sideModeEnum { Compressor,Expander };	// to compare: if(compareEnumToInt(sideModeEnum::Compressor, sideMode)) etc... 

	int midMute = 0;
	enum class midMuteEnum { SWITCH_OFF,SWITCH_ON };	// to compare: if(compareEnumToInt(midMuteEnum::SWITCH_OFF, midMute)) etc... 

	int sideMute = 0;
	enum class sideMuteEnum { SWITCH_OFF,SWITCH_ON };	// to compare: if(compareEnumToInt(sideMuteEnum::SWITCH_OFF, sideMute)) etc... 

	int midSolo = 0;
	enum class midSoloEnum { SWITCH_OFF,SWITCH_ON };	// to compare: if(compareEnumToInt(midSoloEnum::SWITCH_OFF, midSolo)) etc... 

	int sideSolo = 0;
	enum class sideSoloEnum { SWITCH_OFF,SWITCH_ON };	// to compare: if(compareEnumToInt(sideSoloEnum::SWITCH_OFF, sideSolo)) etc... 

	int midHardLimit = 0;
	enum class midHardLimitEnum { SWITCH_OFF,SWITCH_ON };	// to compare: if(compareEnumToInt(midHardLimitEnum::SWITCH_OFF, midHardLimit)) etc... 

	int sideHardLimit = 0;
	enum class sideHardLimitEnum { SWITCH_OFF,SWITCH_ON };	// to compare: if(compareEnumToInt(sideHardLimitEnum::SWITCH_OFF, sideHardLimit)) etc... 

	// --- Meter Plugin Variables
	float inputMeter1 = 0.f;
	float outputMeter1 = 0.f;
	float reductionMeter1 = 0.f;
	float inputMeter2 = 0.f;
	float inputMeter3 = 0.f;
	float inputMeter4 = 0.f;
	float outputMeter2 = 0.f;
	float outputMeter3 = 0.f;
	float outputMeter4 = 0.f;
	float reductionMeter2 = 0.f;
	float reductionMeter3 = 0.f;
	float reductionMeter4 = 0.f;
	float masterInputMeter = 0.f;
	float masterOutputMeter = 0.f;
	float midReductionMeter = 0.f;
	float sideReductionMeter = 0.f;

	// **--0x1A7F--**
    // --- end member variables

public:
    /** static description: bundle folder name

	\return bundle folder name as a const char*
	*/
    static const char* getPluginBundleName();

    /** static description: name

	\return name as a const char*
	*/
    static const char* getPluginName();

	/** static description: short name

	\return short name as a const char*
	*/
	static const char* getShortPluginName();

	/** static description: vendor name

	\return vendor name as a const char*
	*/
	static const char* getVendorName();

	/** static description: URL

	\return URL as a const char*
	*/
	static const char* getVendorURL();

	/** static description: email

	\return email address as a const char*
	*/
	static const char* getVendorEmail();

	/** static description: Cocoa View Factory Name

	\return Cocoa View Factory Name as a const char*
	*/
	static const char* getAUCocoaViewFactoryName();

	/** static description: plugin type

	\return type (FX or Synth)
	*/
	static pluginType getPluginType();

	/** static description: VST3 GUID

	\return VST3 GUID as a const char*
	*/
	static const char* getVSTFUID();

	/** static description: 4-char code

	\return 4-char code as int
	*/
	static int32_t getFourCharCode();

	/** initalizer */
	bool initPluginDescriptors();

};




#endif /* defined(__pluginCore_h__) */
