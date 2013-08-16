#include "CHOP_CPlusPlusBase.h"
#include "OVR.h"
#include <iostream>
#include <conio.h>
/*
	Basic Oculus Rift Data CHOP
	Based on Minimal Oculus Application example at https://developer.oculusvr.com/wiki/Minimal_Oculus_Application?status=1
*/
using namespace OVR;
using namespace OVR::Util::Render;
using namespace std;

// To get more help about these functions, look at CHOP_CPlusPlusBase.h
class MMMRiftCHOP : public CHOP_CPlusPlusBase
{
public:
	MMMRiftCHOP(const CHOP_NodeInfo *info);
	virtual ~MMMRiftCHOP();

	virtual void		getGeneralInfo(CHOP_GeneralInfo *);
	virtual bool		getOutputInfo(CHOP_OutputInfo*);
	virtual const char*	getChannelName(int index, void* reserved);

	virtual void		execute(const CHOP_Output*,
								const CHOP_InputArrays*,
								void* reserved);


	virtual int			getNumInfoCHOPChans();
	virtual void		getInfoCHOPChan(int index,
										CHOP_InfoCHOPChan *chan);

	virtual bool		getInfoDATSize(CHOP_InfoDATSize *infoSize);
	virtual void		getInfoDATEntries(int index,
											int nEntries,
											CHOP_InfoDATEntries *entries);
private:

	// We don't need to store this pointer, but we do for the example.
	// The CHOP_NodeInfo class store information about the node that's using
	// this instance of the class (like its name).
	const CHOP_NodeInfo		*myNodeInfo;

	// In this example this value will be incremented each time the execute()
	// function is called, then passes back to the CHOP 
	int						 myExecuteCount;

	// Properties for the Rift
	Ptr<DeviceManager>	pManager;
	Ptr<HMDDevice>		pHMD;
	Ptr<SensorDevice>	pSensor;
	SensorFusion		FusionResult;
	HMDInfo				Info;
	bool				InfoLoaded;
	StereoConfig		stereo;
	float				renderScale;

};

namespace mmm {
   enum Channel { 
	  hmd_found,
	  sensor_found,
	  sensor_yaw,
	  sensor_pitch,
	  sensor_roll,
	  version,
	  h_resolution,
	  v_resolution,
	  h_screen_size,
	  v_screen_size,
	  v_screen_center,
	  eye_to_screen_distance,
	  lens_separation_distance,
	  interpupillary_distance,
	  distortion_k_0,
	  distortion_k_1,
	  distortion_k_2,
	  left_eye_projection_0_0,
	  left_eye_projection_0_1,
	  left_eye_projection_0_2,
	  left_eye_projection_0_3,
	  left_eye_projection_1_0,
	  left_eye_projection_1_1,
	  left_eye_projection_1_2,
	  left_eye_projection_1_3,
	  left_eye_projection_2_0,
	  left_eye_projection_2_1,
	  left_eye_projection_2_2,
	  left_eye_projection_2_3,
	  left_eye_projection_3_0,
	  left_eye_projection_3_1,
	  left_eye_projection_3_2,
	  left_eye_projection_3_3,
	  render_scale,
	  count
   } ;
}