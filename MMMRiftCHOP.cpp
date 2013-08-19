#include "MMMRiftCHOP.h"

#include <stdio.h>
#include <string.h>
#include <math.h>

#include <iostream>

// These functions are basic C function, which the DLL loader can find
// much easier than finding a C++ Class.
// The DLLEXPORT prefix is needed so the compile exports these functions from the .dll
// you are creating
extern "C"
{

DLLEXPORT
int
GetCHOPAPIVersion(void)
{
	// Always return CHOP_CPLUSPLUS_API_VERSION in this function.
	return CHOP_CPLUSPLUS_API_VERSION;
}

DLLEXPORT
CHOP_CPlusPlusBase*
CreateCHOPInstance(const CHOP_NodeInfo *info)
{
	// Return a new instance of your class every time this is called.
	// It will be called once per CHOP that is using the .dll
	return new MMMRiftCHOP(info);
}

DLLEXPORT
void
DestroyCHOPInstance(CHOP_CPlusPlusBase *instance)
{
	// Delete the instance here, this will be called when
	// Touch is shutting down, when the CHOP using that instance is deleted, or
	// if the CHOP loads a different DLL
	delete (MMMRiftCHOP*)instance;
	System::Destroy();
}

};

MMMRiftCHOP::MMMRiftCHOP(const CHOP_NodeInfo *info) : myNodeInfo(info)
{
	didOutputSettings = false;
	System::Init();
	pManager = *DeviceManager::Create();
	pHMD = *pManager->EnumerateDevices<HMDDevice>().CreateDevice();
	if (pHMD){
		InfoLoaded = pHMD->GetDeviceInfo(&Info);
		pSensor = *pHMD->GetSensor();
	} else {
	   pSensor = *pManager->EnumerateDevices<SensorDevice>().CreateDevice();
	}

	if (pSensor){
	   FusionResult.AttachToSensor(pSensor);

		// Stereo Rendering Setup
		stereo.SetFullViewport(Viewport(0,0, Info.HResolution, Info.VResolution));
		stereo.SetStereoMode(Stereo_LeftRight_Multipass);
		stereo.SetHMDInfo(Info);
		stereo.SetDistortionFitPointVP(-1.0f, 0.0f);
		renderScale = stereo.GetDistortionScale();
	}
}

MMMRiftCHOP::~MMMRiftCHOP()
{
	pSensor.Clear();
	pHMD.Clear();
	pManager.Clear();
}

void
MMMRiftCHOP::getGeneralInfo(CHOP_GeneralInfo *ginfo)
{
	// This will cause the node to cook every frame
	ginfo->cookEveryFrameIfAsked = true;
	ginfo->timeslice = false;
	ginfo->inputMatchIndex = 0;
}

bool
MMMRiftCHOP::getOutputInfo(CHOP_OutputInfo *info)
{
	info->numChannels = mmm::Channel::count;

	// Since we are outputting a timeslice, the system will dictate
	// the length and startIndex of the CHOP data
	info->length = 1;
	info->startIndex = 0;

	// For illustration we are going to output 120hz data
	//info->sampleRate = 120;
	return true;
}

const char*
MMMRiftCHOP::getChannelName(int index, void* reserved)
{
	mmm::Channel channel_num = (mmm::Channel)index;
	switch (channel_num)
	{
	case mmm::Channel::hmd_found:
		return "hmd_found";
		break;
	case mmm::Channel::sensor_found:
		return "sensor_found";
		break;
	case mmm::Channel::sensor_yaw:
		return "sensor_yaw";
		break;
	case mmm::Channel::sensor_pitch:
		return "sensor_pitch";
		break;
	case mmm::Channel::sensor_roll:
		return "sensor_roll";
		break;
	case mmm::Channel::version:
		return "version";
		break;
	case mmm::Channel::h_resolution:
		return "h_resolution";
		break;
	case mmm::Channel::v_resolution:
		return "v_resolution";
		break;
	case mmm::Channel::h_screen_size:
		return "h_screen_size";
		break;
	case mmm::Channel::v_screen_size:
		return "v_screen_size";
		break;
	case mmm::Channel::v_screen_center:
		return "v_screen_center";
		break;
	case mmm::Channel::eye_to_screen_distance:
		return "eye_to_screen_distance";
		break;
	case mmm::Channel::lens_separation_distance:
		return "lens_separation_distance";
		break;
	case mmm::Channel::interpupillary_distance:
		return "interpupillary_distance";
		break;
	case mmm::Channel::distortion_k_0:
		return "distortion_k_0";
		break;
	case mmm::Channel::distortion_k_1:
		return "distortion_k_1";
		break;
	case mmm::Channel::distortion_k_2:
		return "distortion_k_2";
		break;
	case mmm::Channel::left_eye_projection_0_0:
		return "left_eye_projection_0_0";
		break;
	case mmm::Channel::left_eye_projection_0_1:
		return "left_eye_projection_0_1";
		break;
	case mmm::Channel::left_eye_projection_0_2:
		return "left_eye_projection_0_2";
		break;
	case mmm::Channel::left_eye_projection_0_3:
		return "left_eye_projection_0_3";
		break;
	case mmm::Channel::left_eye_projection_1_0:
		return "left_eye_projection_1_0";
		break;
	case mmm::Channel::left_eye_projection_1_1:
		return "left_eye_projection_1_1";
		break;
	case mmm::Channel::left_eye_projection_1_2:
		return "left_eye_projection_1_2";
		break;
	case mmm::Channel::left_eye_projection_1_3:
		return "left_eye_projection_1_3";
		break;
	case mmm::Channel::left_eye_projection_2_0:
		return "left_eye_projection_2_0";
		break;
	case mmm::Channel::left_eye_projection_2_1:
		return "left_eye_projection_2_1";
		break;
	case mmm::Channel::left_eye_projection_2_2:
		return "left_eye_projection_2_2";
		break;
	case mmm::Channel::left_eye_projection_2_3:
		return "left_eye_projection_2_3";
		break;
	case mmm::Channel::left_eye_projection_3_0:
		return "left_eye_projection_3_0";
		break;
	case mmm::Channel::left_eye_projection_3_1:
		return "left_eye_projection_3_1";
		break;
	case mmm::Channel::left_eye_projection_3_2:
		return "left_eye_projection_3_2";
		break;
	case mmm::Channel::left_eye_projection_3_3:
		return "left_eye_projection_3_3";
		break;
	case mmm::Channel::render_scale:
		return "render_scale";
		break;
	case mmm::Channel::shader_scale:
		return "shader_scale";
		break;
	case mmm::Channel::count:
		return "this_should_never_happen";
		break;
	default:
		return "unnamed channel";
		break;
	}

}

void
MMMRiftCHOP::execute(const CHOP_Output* output,
								const CHOP_InputArrays* inputs,
								void* reserved)
{
	myExecuteCount++;
	output->channels[mmm::Channel::hmd_found][0] = pHMD ? 1 : 0;
	output->channels[mmm::Channel::sensor_found][0] = pSensor ? 1 : 0;

	if(pSensor){
		Quatf quaternion = FusionResult.GetOrientation();
		float yaw, pitch, roll;
		quaternion.GetEulerAngles<Axis_Y, Axis_X, Axis_Z>(&yaw, &pitch, &roll);
		output->channels[mmm::Channel::sensor_yaw][0] = RadToDegree(yaw);
		output->channels[mmm::Channel::sensor_pitch][0] = RadToDegree(pitch);
		output->channels[mmm::Channel::sensor_roll][0] = RadToDegree(roll);

		if(!didOutputSettings){
			output->channels[mmm::Channel::version][0] = Info.Version;
			output->channels[mmm::Channel::h_resolution][0] = Info.HResolution;
			output->channels[mmm::Channel::v_resolution][0] = Info.VResolution;
			output->channels[mmm::Channel::h_screen_size][0] = Info.HScreenSize;
			output->channels[mmm::Channel::v_screen_size][0] = Info.VScreenSize;
			output->channels[mmm::Channel::v_screen_center][0] = Info.VScreenCenter;
			output->channels[mmm::Channel::eye_to_screen_distance][0] = Info.EyeToScreenDistance;
			output->channels[mmm::Channel::lens_separation_distance][0] = Info.LensSeparationDistance;
			output->channels[mmm::Channel::interpupillary_distance][0] = Info.InterpupillaryDistance;
			output->channels[mmm::Channel::distortion_k_0][0] = Info.DistortionK[0];
			output->channels[mmm::Channel::distortion_k_1][0] = Info.DistortionK[1];
			output->channels[mmm::Channel::distortion_k_2][0] = Info.DistortionK[2];
			output->channels[mmm::Channel::render_scale][0] = renderScale;
			output->channels[mmm::Channel::shader_scale][0] = 0.5f / renderScale;

			StereoEyeParams leftEye = stereo.GetEyeRenderParams(StereoEye_Left);
			Viewport leftVP = leftEye.VP;
			Matrix4f leftProjection = leftEye.Projection;
			Matrix4f leftViewAdjust = leftEye.ViewAdjust;

			output->channels[mmm::Channel::left_eye_projection_0_0][0] = leftProjection.M[0][0];
			output->channels[mmm::Channel::left_eye_projection_0_1][0] = leftProjection.M[1][0];
			output->channels[mmm::Channel::left_eye_projection_0_2][0] = leftProjection.M[2][0];
			output->channels[mmm::Channel::left_eye_projection_0_3][0] = leftProjection.M[3][0];
			output->channels[mmm::Channel::left_eye_projection_1_0][0] = leftProjection.M[0][1];
			output->channels[mmm::Channel::left_eye_projection_1_1][0] = leftProjection.M[1][1];
			output->channels[mmm::Channel::left_eye_projection_1_2][0] = leftProjection.M[2][1];
			output->channels[mmm::Channel::left_eye_projection_1_3][0] = leftProjection.M[3][1];
			output->channels[mmm::Channel::left_eye_projection_2_0][0] = leftProjection.M[0][2];
			output->channels[mmm::Channel::left_eye_projection_2_1][0] = leftProjection.M[1][2];
			output->channels[mmm::Channel::left_eye_projection_2_2][0] = leftProjection.M[2][2];
			output->channels[mmm::Channel::left_eye_projection_2_3][0] = leftProjection.M[3][2];
			output->channels[mmm::Channel::left_eye_projection_3_0][0] = leftProjection.M[0][3];
			output->channels[mmm::Channel::left_eye_projection_3_1][0] = leftProjection.M[1][3];
			output->channels[mmm::Channel::left_eye_projection_3_2][0] = leftProjection.M[2][3];
			output->channels[mmm::Channel::left_eye_projection_3_3][0] = leftProjection.M[3][3];
			didOutputSettings = true;
		}

	}
}

int
MMMRiftCHOP::getNumInfoCHOPChans()
{
	// We return the number of channel we want to output to any Info CHOP
	// connected to the CHOP. In this example we are just going to send one channel.
	return 1;
}

void
MMMRiftCHOP::getInfoCHOPChan(int index,
										CHOP_InfoCHOPChan *chan)
{
	// This function will be called once for each channel we said we'd want to return
	// In this example it'll only be called once.

	if (index == 0)
	{
		chan->name = "executeCount";
		chan->value = myExecuteCount;
	}
}

bool		
MMMRiftCHOP::getInfoDATSize(CHOP_InfoDATSize *infoSize)
{
	infoSize->rows = 1;
	infoSize->cols = 2;
	// Setting this to false means we'll be assigning values to the table
	// one row at a time. True means we'll do it one column at a time.
	infoSize->byColumn = false;
	return true;
}

void
MMMRiftCHOP::getInfoDATEntries(int index,
										int nEntries,
										CHOP_InfoDATEntries *entries)
{
	if (index == 0)
	{
		// It's safe to use static buffers here because Touch will make it's own
		// copies of the strings immediately after this call returns
		// (so the buffers can be reuse for each column/row)
		static char tempBuffer1[4096];
		static char tempBuffer2[4096];
		// Set the value for the first column
		strcpy(tempBuffer1, "executeCount");
		entries->values[0] = tempBuffer1;

		// Set the value for the second column
		sprintf(tempBuffer2, "%d", myExecuteCount);
		entries->values[1] = tempBuffer2;
	}
}