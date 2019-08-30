#include "Digitizer.hpp"
#include <iostream>
#include <cmath>
#include "fft.hpp"
#include "X742CorrectionRoutines.hpp"
#include <thread>
#include <chrono>



#define NPOINTS 2
#define NACQS   50
#define VME_INTERRUPT_LEVEL      1
#define VME_INTERRUPT_STATUS_ID  0xAAAA
#define INTERRUPT_TIMEOUT        200  // ms



/*! \fn      int WriteRegisterBitmask(int32_t handle, uint32_t address, uint32_t data, uint32_t mask)
*   \brief   writes 'data' on register at 'address' using 'mask' as bitmask
*
*   \param   handle :   Digitizer handle
*   \param   address:   Address of the Register to write
*   \param   data   :   Data to Write on the Register
*   \param   mask   :   Bitmask to use for data masking
*   \return  0 = Success; negative numbers are error codes
*/
int Digitizer::WriteRegisterBitmask(uint32_t address, uint32_t data, uint32_t mask) 
{
    int32_t ret = CAEN_DGTZ_Success;
    uint32_t d32 = 0xFFFFFFFF;

    ret = CAEN_DGTZ_ReadRegister(handle, address, &d32);
    if(ret != CAEN_DGTZ_Success)
        return ret;

    data &= mask;
    d32 &= ~mask;
    d32 |= data;
    ret = CAEN_DGTZ_WriteRegister(handle, address, d32);
    return ret;
}

/*! \fn      int ProgramDigitizer(int handle, WaveDumpConfig_t WDcfg)
*   \brief   configure the digitizer according to the parameters read from
*            the cofiguration file and saved in the WDcfg data structure
*
*   \param   handle   Digitizer handle
*   \param   WDcfg:   WaveDumpConfig data structure
*   \return  0 = Success; negative numbers are error codes
*/
int Digitizer::ProgramDigitizer()
{
    int ret = 0;
    /* reset the digitizer */
    ret |= CAEN_DGTZ_Reset(handle);
    if (ret != 0) 
    {
        std::cout<<"Error: Unable to reset digitizer.\nPlease reset digitizer manually then restart the program"<<std::endl;
        return -1;
    }
    // Set the waveform test bit for debugging
    if (dat.WDcfg.TestPattern) ret |= CAEN_DGTZ_WriteRegister(handle, CAEN_DGTZ_BROAD_CH_CONFIGBIT_SET_ADD, 1<<3);
    // custom setting for X742 boards
    if (dat.BoardInfo.FamilyCode == CAEN_DGTZ_XX742_FAMILY_CODE) 
    {
        ret |= CAEN_DGTZ_SetFastTriggerDigitizing(handle,CAEN_DGTZ_EnaDis_t(dat.WDcfg.FastTriggerEnabled));
        ret |= CAEN_DGTZ_SetFastTriggerMode(handle,dat.WDcfg.FastTriggerMode);
    }
    if ((dat.BoardInfo.FamilyCode == CAEN_DGTZ_XX751_FAMILY_CODE) || (dat.BoardInfo.FamilyCode == CAEN_DGTZ_XX731_FAMILY_CODE)) 
		{
        ret |= CAEN_DGTZ_SetDESMode(handle, dat.WDcfg.DesMode);
    }
    ret |= CAEN_DGTZ_SetRecordLength(handle, dat.WDcfg.RecordLength);
    ret |= CAEN_DGTZ_GetRecordLength(handle, &dat.WDcfg.RecordLength);

    if (dat.BoardInfo.FamilyCode == CAEN_DGTZ_XX740_FAMILY_CODE || dat.BoardInfo.FamilyCode == CAEN_DGTZ_XX724_FAMILY_CODE) 
		{
        ret |= CAEN_DGTZ_SetDecimationFactor(handle, dat.WDcfg.DecimationFactor);
    }
    ret |= CAEN_DGTZ_SetPostTriggerSize(handle, dat.WDcfg.PostTrigger);
    if(dat.BoardInfo.FamilyCode != CAEN_DGTZ_XX742_FAMILY_CODE) 
		{
        uint32_t pt;
        ret |= CAEN_DGTZ_GetPostTriggerSize(handle, &pt);
        dat.WDcfg.PostTrigger = pt;
    }
    ret |= CAEN_DGTZ_SetIOLevel(handle, dat.WDcfg.FPIOtype);
    if( dat.WDcfg.InterruptNumEvents > 0) 
		{
        // Interrupt handling
        if( ret |= CAEN_DGTZ_SetInterruptConfig(handle,CAEN_DGTZ_ENABLE,VME_INTERRUPT_LEVEL, VME_INTERRUPT_STATUS_ID,(uint16_t)dat.WDcfg.InterruptNumEvents, INTERRUPT_MODE)!= CAEN_DGTZ_Success) 
        {
                std::cout<<"\nError configuring interrupts. Interrupts disabled\n"<<std::endl;
                dat.WDcfg.InterruptNumEvents = 0;
        }
    }
	
    ret |= CAEN_DGTZ_SetMaxNumEventsBLT(handle, dat.WDcfg.NumEvents);
    ret |= CAEN_DGTZ_SetAcquisitionMode(handle, CAEN_DGTZ_SW_CONTROLLED);
    ret |= CAEN_DGTZ_SetExtTriggerInputMode(handle, dat.WDcfg.ExtTriggerMode);

    if ((dat.BoardInfo.FamilyCode == CAEN_DGTZ_XX740_FAMILY_CODE) || (dat.BoardInfo.FamilyCode == CAEN_DGTZ_XX742_FAMILY_CODE))
		{
        ret |= CAEN_DGTZ_SetGroupEnableMask(handle, dat.WDcfg.EnableMask);
        for(std::size_t i=0; i<(dat.WDcfg.Nch/8); i++) 
				{
            if (dat.WDcfg.EnableMask & (1<<i)) 
						{
                if(dat.BoardInfo.FamilyCode == CAEN_DGTZ_XX742_FAMILY_CODE) 
								{
                    for(std::size_t j=0; j<8; j++) 
										{
                        if (dat.WDcfg.DCoffsetGrpCh[i][j] != -1)ret |= CAEN_DGTZ_SetChannelDCOffset(handle,(i*8)+j, dat.WDcfg.DCoffsetGrpCh[i][j]);
												else	ret |= CAEN_DGTZ_SetChannelDCOffset(handle, (i * 8) + j, dat.WDcfg.DCoffset[i]);
                    }
                }
                else 
								{
                    if(dat.WDcfg.Version_used[i] == 1)	ret |= Set_calibrated_DCO(i);
										else	ret |= CAEN_DGTZ_SetGroupDCOffset(handle, i, dat.WDcfg.DCoffset[i]);
                    ret |= CAEN_DGTZ_SetGroupSelfTrigger(handle, dat.WDcfg.ChannelTriggerMode[i], (1<<i));
                    ret |= CAEN_DGTZ_SetGroupTriggerThreshold(handle, i, dat.WDcfg.Threshold[i]);
                    ret |= CAEN_DGTZ_SetChannelGroupMask(handle, i, dat.WDcfg.GroupTrgEnableMask[i]);
                } 
                ret |= CAEN_DGTZ_SetTriggerPolarity(handle, i, CAEN_DGTZ_TriggerPolarity_t(dat.WDcfg.PulsePolarity[i])); //.TriggerEdge
            }
        }
    } 
		else 
		{
        ret |= CAEN_DGTZ_SetChannelEnableMask(handle, dat.WDcfg.EnableMask);
        for (std::size_t i = 0; i < dat.WDcfg.Nch; i++) 
				{
            if (dat.WDcfg.EnableMask & (1<<i)) 
						{
							if (dat.WDcfg.Version_used[i] == 1)ret |= Set_calibrated_DCO(i);
							else	ret |= CAEN_DGTZ_SetChannelDCOffset(handle, i, dat.WDcfg.DCoffset[i]);
              if (dat.BoardInfo.FamilyCode != CAEN_DGTZ_XX730_FAMILY_CODE && dat.BoardInfo.FamilyCode != CAEN_DGTZ_XX725_FAMILY_CODE)
                    ret |= CAEN_DGTZ_SetChannelSelfTrigger(handle, dat.WDcfg.ChannelTriggerMode[i], (1<<i));
                ret |= CAEN_DGTZ_SetChannelTriggerThreshold(handle, i, dat.WDcfg.Threshold[i]);
                ret |= CAEN_DGTZ_SetTriggerPolarity(handle, i, CAEN_DGTZ_TriggerPolarity_t(dat.WDcfg.PulsePolarity[i])); //.TriggerEdge
            }
        }
        if (dat.BoardInfo.FamilyCode == CAEN_DGTZ_XX730_FAMILY_CODE || dat.BoardInfo.FamilyCode == CAEN_DGTZ_XX725_FAMILY_CODE) 
				{
            // channel pair settings for x730 boards
            for (std::size_t i = 0; i < dat.WDcfg.Nch; i += 2) 
						{
                if (dat.WDcfg.EnableMask & (0x3 << i)) 
								{
                    CAEN_DGTZ_TriggerMode_t mode = dat.WDcfg.ChannelTriggerMode[i];
                    uint32_t pair_chmask = 0;

                    // Build mode and relevant channelmask. The behaviour is that,
                    // if the triggermode of one channel of the pair is DISABLED,
                    // this channel doesn't take part to the trigger generation.
                    // Otherwise, if both are different from DISABLED, the one of
                    // the even channel is used.
                    if (dat.WDcfg.ChannelTriggerMode[i] != CAEN_DGTZ_TRGMODE_DISABLED) 
										{
                        if (dat.WDcfg.ChannelTriggerMode[i + 1] == CAEN_DGTZ_TRGMODE_DISABLED) pair_chmask = (0x1 << i);
                        else pair_chmask = (0x3 << i);
                    }
                    else 
										{
                        mode = dat.WDcfg.ChannelTriggerMode[i + 1];
                        pair_chmask = (0x2 << i);
                    }

                    pair_chmask &= dat.WDcfg.EnableMask;
                    ret |= CAEN_DGTZ_SetChannelSelfTrigger(handle, mode, pair_chmask);
                }
            }
        }
    }
    if (dat.BoardInfo.FamilyCode == CAEN_DGTZ_XX742_FAMILY_CODE) 
		{
        for(std::size_t i=0; i<(dat.WDcfg.Nch/8); i++) 
				{
            ret |= CAEN_DGTZ_SetDRS4SamplingFrequency(handle, dat.WDcfg.DRS4Frequency);
            ret |= CAEN_DGTZ_SetGroupFastTriggerDCOffset(handle,i,dat.WDcfg.FTDCoffset[i]);
            ret |= CAEN_DGTZ_SetGroupFastTriggerThreshold(handle,i,dat.WDcfg.FTThreshold[i]);
        }
    }
    /* execute generic write commands */
    for(std::size_t i=0; i<dat.WDcfg.GWn; i++) ret |= WriteRegisterBitmask(dat.WDcfg.GWaddr[i], dat.WDcfg.GWdata[i], dat.WDcfg.GWmask[i]);
    if (ret) std::cout<<"Warning: errors found during the programming of the digitizer.\nSome settings may not be executed\n"<<std::endl;
    return 0;
}


/*! \brief   Write the event data on x742 boards into the output files
*
*   \param   WDrun Pointer to the WaveDumpRun data structure
*   \param   WDcfg Pointer to the WaveDumpConfig data structure
*   \param   EventInfo Pointer to the EventInfo data structure
*   \param   Event Pointer to the Event to write
*/
void Digitizer::calibrate() 
{
    std::cout<<std::endl;
    if (BoardSupportsCalibration()) 
		{
        if (dat.WDrun.AcqRun == 0) 
				{
            int32_t ret = CAEN_DGTZ_Calibrate(handle);
            if (ret == CAEN_DGTZ_Success) std::cout<<"ADC Calibration successfully executed."<<std::endl;
            else std::cout<<"ADC Calibration failed. CAENDigitizer ERR "<<ret<<std::endl;
            std::cout<<std::endl;
        }
				else std::cout<<"Can't run ADC calibration while acquisition is running."<<std::endl;
    }
		else std::cout<<"ADC Calibration not needed for this board family."<<std::endl;
}


/*! \fn      void GoToNextEnabledGroup(WaveDumpRun_t *WDrun, WaveDumpConfig_t *WDcfg)
*   \brief   selects the next enabled group for plotting
*
*   \param   WDrun:   Pointer to the WaveDumpRun_t data structure
*   \param   WDcfg:   Pointer to the WaveDumpConfig_t data structure
*/
void Digitizer::GoToNextEnabledGroup() 
{
    if ((dat.WDcfg.EnableMask) && (dat.WDcfg.Nch>8)) 
		{
        int orgPlotIndex = dat.WDrun.GroupPlotIndex;
        do 
				{
            dat.WDrun.GroupPlotIndex = (++dat.WDrun.GroupPlotIndex)%(dat.WDcfg.Nch/8);
        } 
				while( !((1 << dat.WDrun.GroupPlotIndex)& dat.WDcfg.EnableMask));
        if( dat.WDrun.GroupPlotIndex != orgPlotIndex) std::cout<<"Plot group set to "<<dat.WDrun.GroupPlotIndex<<std::endl;
    }
}




/*! \fn      void Calibrate_XX740_DC_Offset(int handle, WaveDumpConfig_t WDcfg, CAEN_DGTZ_BoardInfo_t BoardInfo)
*   \brief   calibrates DAC of enabled channel groups (only if BASELINE_SHIFT is in use)
*
*   \param   handle   Digitizer handle
*   \param   WDcfg:   Pointer to the WaveDumpConfig_t data structure
*   \param   BoardInfo: structure with the board info
*/
void Digitizer::Calibrate_XX740_DC_Offset()
{
	float cal[MAX_CH];
	float offset[MAX_CH] = { 0 };
	for (std::size_t i = 0; i < MAX_CH; i++) cal[i] = 1;
	int ret;
	CAEN_DGTZ_AcqMode_t mem_mode;
	uint32_t  AllocatedSize;

	uint32_t BufferSize;
	CAEN_DGTZ_EventInfo_t       EventInfo;
	char *buffer = nullptr;
	char *EventPtr = nullptr;
	CAEN_DGTZ_UINT16_EVENT_t    *Event16 = nullptr;

	float avg_value[NPOINTS][MAX_CH] = { 0 };
	uint32_t dc[NPOINTS] = { 25,75 }; //test values (%)
	uint32_t groupmask = 0;

	ret = CAEN_DGTZ_GetAcquisitionMode(handle, &mem_mode);//chosen value stored
	if (ret) std::cout<<"Error trying to read acq mode!!"<<std::endl;
	ret = CAEN_DGTZ_SetAcquisitionMode(handle, CAEN_DGTZ_SW_CONTROLLED);
	if (ret) std::cout<<"Error trying to set acq mode!!"<<std::endl;
	ret = CAEN_DGTZ_SetExtTriggerInputMode(handle, CAEN_DGTZ_TRGMODE_DISABLED);
	if (ret) std::cout<<"Error trying to set ext trigger!!"<<std::endl;
	ret = CAEN_DGTZ_SetMaxNumEventsBLT(handle, 1);
	if (ret) std::cout<<"Warning: error setting max BLT number"<<std::endl;
	ret = CAEN_DGTZ_SetDecimationFactor(handle, 1);
	if (ret) std::cout<<"Error trying to set decimation factor!!"<<std::endl;
	for (std::size_t g = 0; g< (int32_t)dat.BoardInfo.Channels; g++) /*BoardInfo.Channels is number of groups for x740 boards*/ groupmask |= (1 << g);
	ret = CAEN_DGTZ_SetGroupSelfTrigger(handle, CAEN_DGTZ_TRGMODE_DISABLED, groupmask);
	if (ret) std::cout<<"Error disabling self trigger"<<std::endl;
	ret = CAEN_DGTZ_SetGroupEnableMask(handle, groupmask);
	if (ret) std::cout<<"Error enabling channel groups."<<std::endl;
	///malloc
	ret = CAEN_DGTZ_MallocReadoutBuffer(handle, &buffer, &AllocatedSize);
	if (ret) Quit(ERR_MALLOC);
	ret = CAEN_DGTZ_AllocateEvent(handle, (void**)&Event16);
	if (ret != CAEN_DGTZ_Success)  Quit(ERR_MALLOC);
	
	std::cout<<"Starting DAC calibration..."<<std::endl;

	for(std::size_t p = 0; p < NPOINTS; p++)
	{
		for (std::size_t i = 0; i < (int32_t)dat.BoardInfo.Channels; i++) 
		{ //BoardInfo.Channels is number of groups for x740 boards
				ret = CAEN_DGTZ_SetGroupDCOffset(handle, (uint32_t)i, (uint32_t)((float)(fabs(dc[p] - 100))*(655.35)));
				if (ret) std::cout<<"Error setting group "<<i<<" test offset"<<std::endl;
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(200));
	CAEN_DGTZ_ClearData(handle);
	ret = CAEN_DGTZ_SWStartAcquisition(handle);
	if (ret) 
  {
		std::cout<<"Error starting X740 acquisition"<<std::endl;
		Quit(ERR_DUMMY_LAST);
	}
	int value[NACQS][MAX_CH] = { 0 }; //baseline values of the NACQS
	for (std::size_t acq = 0; acq < NACQS; acq++) 
  {
		CAEN_DGTZ_SendSWtrigger(handle);

		ret = CAEN_DGTZ_ReadData(handle, CAEN_DGTZ_SLAVE_TERMINATED_READOUT_MBLT, buffer, &BufferSize);
		if (ret) Quit(ERR_READOUT);
		if (BufferSize == 0)continue;
		ret = CAEN_DGTZ_GetEventInfo(handle, buffer, BufferSize, 0, &EventInfo, &EventPtr);
		if (ret)  Quit(ERR_EVENT_BUILD);
		// decode the event //
		ret = CAEN_DGTZ_DecodeEvent(handle, EventPtr, (void**)&Event16);
		if (ret) Quit(ERR_EVENT_BUILD);
		for (std::size_t g = 0; g < (int32_t)dat.BoardInfo.Channels; g++) 
    {
				for (std::size_t k = 1; k < 21; k++) //mean over 20 samples
					value[acq][g] += (int)(Event16->DataChannel[g * 8][k]);
				value[acq][g] = (value[acq][g] / 20);
		}
	}//for acq

	///check for clean baselines
	for (std::size_t g = 0; g < (int32_t)dat.BoardInfo.Channels; g++) 
  {
			int max = 0;
			int mpp = 0;
			int size = (int)pow(2, (double)dat.BoardInfo.ADC_NBits);
			int *freq = static_cast<int*>(calloc(size, sizeof(int)));
			//find the most probable value mpp
			for (std::size_t k = 0; k < NACQS; k++) 
      {
				if (value[k][g] > 0 && value[k][g] < size) 
        {
					freq[value[k][g]]++;
					if (freq[value[k][g]] > max) 
          {
						max = freq[value[k][g]];
						mpp = value[k][g];
					}
				}
			}
			free(freq);
			//discard values too far from mpp
			int ok = 0;
			for (std::size_t k = 0; k < NACQS; k++) 
      {
				if (value[k][g] >= (mpp - 5) && value[k][g] <= (mpp + 5)) 
        {
					avg_value[p][g] = avg_value[p][g] + (float)value[k][g];
					ok++;
				}
			}
			avg_value[p][g] = (avg_value[p][g] / (float)ok)*100. / (float)size;
	}

	CAEN_DGTZ_SWStopAcquisition(handle);
	}//close for p

	for (std::size_t g = 0; g < (int32_t)dat.BoardInfo.Channels; g++) 
  {
			cal[g] = ((float)(avg_value[1][g] - avg_value[0][g]) / (float)(dc[1] - dc[0]));
			offset[g] = (float)(dc[1] * avg_value[0][g] - dc[0] * avg_value[1][g]) / (float)(dc[1] - dc[0]);
			std::cout<<"Group "<<g<<" DAC calibration ready."<<std::endl;
			std::cout<<"Cal :"<<cal[g]<<"   offset "<<offset[g]<<std::endl;
			dat.WDcfg.DAC_Calib.cal[g] = cal[g];
			dat.WDcfg.DAC_Calib.offset[g] = offset[g];
	}
	CAEN_DGTZ_ClearData(handle);
	///free events e buffer
	CAEN_DGTZ_FreeReadoutBuffer(&buffer);
	CAEN_DGTZ_FreeEvent(handle, (void**)&Event16);
	ret |= CAEN_DGTZ_SetMaxNumEventsBLT(handle, dat.WDcfg.NumEvents);
	ret |= CAEN_DGTZ_SetDecimationFactor(handle,dat.WDcfg.DecimationFactor);
	ret |= CAEN_DGTZ_SetPostTriggerSize(handle, dat.WDcfg.PostTrigger);
	ret |= CAEN_DGTZ_SetAcquisitionMode(handle, mem_mode);
	ret |= CAEN_DGTZ_SetExtTriggerInputMode(handle, dat.WDcfg.ExtTriggerMode);
	ret |= CAEN_DGTZ_SetGroupEnableMask(handle, dat.WDcfg.EnableMask);
	for (std::size_t i = 0; i < dat.BoardInfo.Channels; i++) 
	{
		if (dat.WDcfg.EnableMask & (1 << i))
			ret |= CAEN_DGTZ_SetGroupSelfTrigger(handle, dat.WDcfg.ChannelTriggerMode[i], (1 << i));
	}
	if (ret) std::cout<<"Error setting recorded parameters"<<std::endl;
	::Save_DAC_Calibration_To_Flash(handle, dat.WDcfg, dat.BoardInfo);
}


extern int thr_file[MAX_CH];

/*! \fn      void Set_relative_Threshold(int handle, WaveDumpConfig_t *WDcfg, CAEN_DGTZ_BoardInfo_t BoardInfo)
*   \brief   sets the threshold relative to the baseline (only if BASELINE_SHIFT is in use)
*
*   \param   handle   Digitizer handle
*   \param   WDcfg:   Pointer to the WaveDumpConfig_t data structure
*   \param   BoardInfo: structure with the board info
*/
void Digitizer::Set_relative_Threshold()
{

	//preliminary check: if baseline shift is not enabled for any channel quit
	int should_start = 0;
	for (std::size_t ch = 0; ch < (int32_t)dat.BoardInfo.Channels; ch++) 
  {
		if (dat.WDcfg.EnableMask & (1 << ch) && dat.WDcfg.Version_used[ch] == 1) 
		{
			should_start = 1;
			break;
		}
	}
	if (!should_start) return;

	int ret;
	uint32_t  AllocatedSize;
	uint32_t BufferSize;
	CAEN_DGTZ_EventInfo_t       EventInfo;
	char *buffer = nullptr;
	char *EventPtr = nullptr;
	CAEN_DGTZ_UINT16_EVENT_t    *Event16 = nullptr;
	CAEN_DGTZ_UINT8_EVENT_t     *Event8 = nullptr;
	uint32_t custom_posttrg = 50, dco, custom_thr;
	float expected_baseline;
	float dco_percent;
	int baseline[MAX_CH] = { 0 }, size = 0, samples = 0;
	int no_self_triggered_event[MAX_CH] = {0};
	int sw_trigger_needed = 0;
	int event_ch;
	///malloc
	ret = CAEN_DGTZ_MallocReadoutBuffer(handle, &buffer, &AllocatedSize);
	if (ret) Quit(ERR_MALLOC);
	if (dat.WDcfg.Nbit == 8) ret = CAEN_DGTZ_AllocateEvent(handle, (void**)&Event8);
	else ret = CAEN_DGTZ_AllocateEvent(handle, (void**)&Event16);
	if (ret != CAEN_DGTZ_Success) Quit(ERR_MALLOC);

	//some custom settings
	ret = CAEN_DGTZ_SetPostTriggerSize(handle, custom_posttrg);
	if (ret) 
  {
		std::cout<<"Threshold calc failed. Error trying to set post trigger!!"<<std::endl;
		return;
	}
	//try to set a small threshold to get a self triggered event
	for (std::size_t ch = 0; ch < (int32_t)dat.BoardInfo.Channels; ch++) 
  {
		if (dat.WDcfg.EnableMask & (1 << ch) && dat.WDcfg.Version_used[ch] == 1) 
		{
			if (dat.BoardInfo.FamilyCode == CAEN_DGTZ_XX740_FAMILY_CODE) ret = CAEN_DGTZ_GetGroupDCOffset(handle, ch, &dco);
			else ret = CAEN_DGTZ_GetChannelDCOffset(handle, ch, &dco);
			if (ret) 
      {
				std::cout<<"Threshold calc failed. Error trying to get DCoffset values!!"<<std::endl;
				return;
			}
			dco_percent = (float)dco / 65535.;
			expected_baseline = pow(2, (double)dat.BoardInfo.ADC_NBits) * (1.0 - dco_percent);

			custom_thr = (dat.WDcfg.PulsePolarity[ch] == CAEN_DGTZ_PulsePolarityPositive) ? ((uint32_t)expected_baseline + 100) : ((uint32_t)expected_baseline - 100);
			
			if (dat.BoardInfo.FamilyCode == CAEN_DGTZ_XX740_FAMILY_CODE) ret = CAEN_DGTZ_SetGroupTriggerThreshold(handle, ch, custom_thr);
			else ret = CAEN_DGTZ_SetChannelTriggerThreshold(handle, ch, custom_thr);
			if (ret) 
      {
				std::cout<<"Threshold calc failed. Error trying to set custom threshold value!!"<<std::endl;
				return;
			}
		}
	}

	CAEN_DGTZ_SWStartAcquisition(handle);
  std::this_thread::sleep_for(std::chrono::milliseconds(300));

	ret = CAEN_DGTZ_ReadData(handle, CAEN_DGTZ_SLAVE_TERMINATED_READOUT_MBLT, buffer, &BufferSize);
	if (ret) Quit(ERR_READOUT);
	//we have some self-triggered event 
	if (BufferSize > 0) 
  {
		ret = CAEN_DGTZ_GetEventInfo(handle, buffer, BufferSize, 0, &EventInfo, &EventPtr);
		if (ret) Quit(ERR_EVENT_BUILD);
		// decode the event //
		if (dat.WDcfg.Nbit == 8)ret = CAEN_DGTZ_DecodeEvent(handle, EventPtr, (void**)&Event8);
		else ret = CAEN_DGTZ_DecodeEvent(handle, EventPtr, (void**)&Event16);
		if (ret) Quit(ERR_EVENT_BUILD);

		for (std::size_t ch = 0; ch < (int32_t)dat.BoardInfo.Channels; ch++) 
		{
			if (dat.WDcfg.EnableMask & (1 << ch) && dat.WDcfg.Version_used[ch] == 1) 
			{
				event_ch = (dat.BoardInfo.FamilyCode == CAEN_DGTZ_XX740_FAMILY_CODE) ? (ch * 8) : ch;//for x740 boards shift to channel 0 of next group
				size = (dat.WDcfg.Nbit == 8) ? Event8->ChSize[event_ch] : Event16->ChSize[event_ch];
				if (size == 0) 
				{//no data from channel ch
					no_self_triggered_event[ch] = 1;
					sw_trigger_needed = 1;
					continue;
				}
				else 
				{//use only one tenth of the pre-trigger samples to calculate the baseline
					samples = (int)(size * ((100 - custom_posttrg) / 2) / 100);
					for (std::size_t i = 0; i < samples; i++) //mean over some pre trigger samples
					{
						if (dat.WDcfg.Nbit == 8) baseline[ch] += (int)(Event8->DataChannel[event_ch][i]);
						else	baseline[ch] += (int)(Event16->DataChannel[event_ch][i]);
					}
					baseline[ch] = (baseline[ch] / samples);
				}
				if (dat.WDcfg.PulsePolarity[ch] == CAEN_DGTZ_PulsePolarityPositive) dat.WDcfg.Threshold[ch] = (uint32_t)baseline[ch] + thr_file[ch];
				else 	if (dat.WDcfg.PulsePolarity[ch] == CAEN_DGTZ_PulsePolarityNegative) dat.WDcfg.Threshold[ch] = (uint32_t)baseline[ch] - thr_file[ch];

				if (dat.WDcfg.Threshold[ch] < 0) dat.WDcfg.Threshold[ch] = 0;
				size = (int)pow(2, (double)dat.BoardInfo.ADC_NBits);
				if (dat.WDcfg.Threshold[ch] > (uint32_t)size) dat.WDcfg.Threshold[ch] = size;

				if (dat.BoardInfo.FamilyCode == CAEN_DGTZ_XX740_FAMILY_CODE) ret = CAEN_DGTZ_SetGroupTriggerThreshold(handle, ch, dat.WDcfg.Threshold[ch]);
				else ret = CAEN_DGTZ_SetChannelTriggerThreshold(handle, ch, dat.WDcfg.Threshold[ch]);
				if (ret) std::cout<<"Warning: error setting ch "<<ch<<" corrected threshold"<<std::endl;
			}
		}
	}
	else 
  {
		sw_trigger_needed = 1;
		for(std::size_t ch = 0; ch < (int32_t)dat.BoardInfo.Channels; ch++) no_self_triggered_event[ch] = 1;
	}

	CAEN_DGTZ_ClearData(handle);

	//if from some channels we had no self triggered event, we send a software trigger
	if (sw_trigger_needed) 
	{
		CAEN_DGTZ_SendSWtrigger(handle);
		ret = CAEN_DGTZ_ReadData(handle, CAEN_DGTZ_SLAVE_TERMINATED_READOUT_MBLT, buffer, &BufferSize);
		if (ret) Quit(ERR_READOUT);
		if (BufferSize == 0) return;

		ret = CAEN_DGTZ_GetEventInfo(handle, buffer, BufferSize, 0, &EventInfo, &EventPtr);
		if (ret) Quit(ERR_EVENT_BUILD);
		// decode the event //
		if (dat.WDcfg.Nbit == 8) ret = CAEN_DGTZ_DecodeEvent(handle, EventPtr, (void**)&Event8);
		else ret = CAEN_DGTZ_DecodeEvent(handle, EventPtr, (void**)&Event16);

		if (ret) Quit(ERR_EVENT_BUILD);

		for (std::size_t ch = 0; ch < (int32_t)dat.BoardInfo.Channels; ch++) 
    {
			if (dat.WDcfg.EnableMask & (1 << ch) && dat.WDcfg.Version_used[ch] == 1) 
			{
				event_ch = (dat.BoardInfo.FamilyCode == CAEN_DGTZ_XX740_FAMILY_CODE) ? (ch * 8) : ch;//for x740 boards shift to channel 0 of next group
				size = (dat.WDcfg.Nbit == 8) ? Event8->ChSize[event_ch] : Event16->ChSize[event_ch];
				if (!no_self_triggered_event[ch])//we already have a good baseline for channel ch
					continue;

				//use some samples to calculate the baseline
				for (std::size_t i = 1; i < 11; i++)
				{ //mean over 10 samples
					if (dat.WDcfg.Nbit == 8) baseline[ch] += (int)(Event8->DataChannel[event_ch][i]);
					else baseline[ch] += (int)(Event16->DataChannel[event_ch][i]);
				}
					baseline[ch] = (baseline[ch] / 10);
			}

			if (dat.WDcfg.PulsePolarity[ch] == CAEN_DGTZ_PulsePolarityPositive) dat.WDcfg.Threshold[ch] = (uint32_t)baseline[ch] + thr_file[ch];
			else 	if (dat.WDcfg.PulsePolarity[ch] == CAEN_DGTZ_PulsePolarityNegative) dat.WDcfg.Threshold[ch] = (uint32_t)baseline[ch] - thr_file[ch];

			if (dat.WDcfg.Threshold[ch] < 0) dat.WDcfg.Threshold[ch] = 0;
				size = (int)pow(2, (double)dat.BoardInfo.ADC_NBits);
			if (dat.WDcfg.Threshold[ch] > (uint32_t)size) dat.WDcfg.Threshold[ch] = size;

			if (dat.BoardInfo.FamilyCode == CAEN_DGTZ_XX740_FAMILY_CODE) ret = CAEN_DGTZ_SetGroupTriggerThreshold(handle, ch, dat.WDcfg.Threshold[ch]);
			else ret = CAEN_DGTZ_SetChannelTriggerThreshold(handle, ch, dat.WDcfg.Threshold[ch]);
			if (ret) std::cout<<"Warning: error setting ch "<<ch<<" corrected threshold"<<std::endl;
		}
	}//end sw trigger event analysis

	CAEN_DGTZ_SWStopAcquisition(handle);

	//reset posttrigger
	ret = CAEN_DGTZ_SetPostTriggerSize(handle, dat.WDcfg.PostTrigger);
	if (ret) std::cout<<"Error resetting post trigger."<<std::endl;

	CAEN_DGTZ_ClearData(handle);

	CAEN_DGTZ_FreeReadoutBuffer(&buffer);
	if (dat.WDcfg.Nbit == 8) CAEN_DGTZ_FreeEvent(handle, (void**)&Event8);
	else CAEN_DGTZ_FreeEvent(handle, (void**)&Event16);
}





/*! \brief   return TRUE if board descriped by 'BoardInfo' supports
*            calibration or not.
*
*   \param   BoardInfo board descriptor
*/
int32_t Digitizer::BoardSupportsCalibration() 
{
    return
		dat.BoardInfo.FamilyCode == CAEN_DGTZ_XX761_FAMILY_CODE ||
        dat.BoardInfo.FamilyCode == CAEN_DGTZ_XX751_FAMILY_CODE ||
        dat.BoardInfo.FamilyCode == CAEN_DGTZ_XX730_FAMILY_CODE ||
        dat.BoardInfo.FamilyCode == CAEN_DGTZ_XX725_FAMILY_CODE;
}

/*! \brief   return TRUE if board descriped by 'BoardInfo' supports
*            temperature read or not.
*
*   \param   BoardInfo board descriptor
*/
int32_t Digitizer::BoardSupportsTemperatureRead() 
{
    return
        dat.BoardInfo.FamilyCode == CAEN_DGTZ_XX751_FAMILY_CODE ||
        dat.BoardInfo.FamilyCode == CAEN_DGTZ_XX730_FAMILY_CODE ||
        dat.BoardInfo.FamilyCode == CAEN_DGTZ_XX725_FAMILY_CODE;
}




/*! \fn      void Calibrate_DC_Offset(int handle, WaveDumpConfig_t WDcfg, CAEN_DGTZ_BoardInfo_t BoardInfo)
*   \brief   calibrates DAC of enabled channels (only if BASELINE_SHIFT is in use)
*
*   \param   handle   Digitizer handle
*   \param   WDcfg:   Pointer to the WaveDumpConfig_t data structure
*   \param   BoardInfo: structure with the board info
*/
void Digitizer::Calibrate_DC_Offset()
{
	float cal[MAX_CH];
	float offset[MAX_CH] = { 0 };
	for (std::size_t i = 0; i < MAX_CH; i++) cal[i] = 1;
	int ret;
	CAEN_DGTZ_AcqMode_t mem_mode;
	uint32_t  AllocatedSize;

	uint32_t BufferSize;
	CAEN_DGTZ_EventInfo_t       EventInfo;
	char *buffer = nullptr;
	char *EventPtr = nullptr;
	CAEN_DGTZ_UINT16_EVENT_t    *Event16 = nullptr;
	CAEN_DGTZ_UINT8_EVENT_t     *Event8 = nullptr;

	float avg_value[NPOINTS][MAX_CH] = { 0 };
	uint32_t dc[NPOINTS] = { 25,75 }; //test values (%)
	uint32_t chmask = 0;

	ret = CAEN_DGTZ_GetAcquisitionMode(handle, &mem_mode);//chosen value stored
	if (ret) std::cout<<"Error trying to read acq mode!!"<<std::endl;
	ret = CAEN_DGTZ_SetAcquisitionMode(handle, CAEN_DGTZ_SW_CONTROLLED);
	if (ret) std::cout<<"Error trying to set acq mode!!"<<std::endl;
	ret = CAEN_DGTZ_SetExtTriggerInputMode(handle, CAEN_DGTZ_TRGMODE_DISABLED);
	if (ret) std::cout<<"Error trying to set ext trigger!!"<<std::endl;
	for (std::size_t ch = 0; ch < (int32_t)dat.BoardInfo.Channels; ch++) chmask |= (1 << ch);
	ret = CAEN_DGTZ_SetChannelSelfTrigger(handle, CAEN_DGTZ_TRGMODE_DISABLED, chmask);
	if (ret) std::cout<<"Warning: error disabling channels self trigger"<<std::endl;
	ret = CAEN_DGTZ_SetChannelEnableMask(handle, chmask);
	if (ret) std::cout<<"Warning: error enabling channels."<<std::endl;
	ret = CAEN_DGTZ_SetMaxNumEventsBLT(handle, 1);
	if (ret) std::cout<<"Warning: error setting max BLT number"<<std::endl;
	if (dat.BoardInfo.FamilyCode == CAEN_DGTZ_XX740_FAMILY_CODE || dat.BoardInfo.FamilyCode == CAEN_DGTZ_XX724_FAMILY_CODE) 
	{
		ret = CAEN_DGTZ_SetDecimationFactor(handle, 1);
		if (ret) std::cout<<"Error trying to set decimation factor!!"<<std::endl;
	}

	///malloc
	ret = CAEN_DGTZ_MallocReadoutBuffer(handle, &buffer, &AllocatedSize);
	if (ret) Quit(ERR_MALLOC);
	if (dat.WDcfg.Nbit == 8) ret = CAEN_DGTZ_AllocateEvent(handle, (void**)&Event8);
	else ret = CAEN_DGTZ_AllocateEvent(handle, (void**)&Event16);
	if (ret != CAEN_DGTZ_Success) Quit(ERR_MALLOC);

	std::cout<<"Starting DAC calibration..."<<std::endl;
	
	for (std::size_t p = 0; p < NPOINTS; p++)
	{
		//set new dco  test value to all channels
		for (std::size_t ch = 0; ch < (int32_t)dat.BoardInfo.Channels; ch++) 
		{
				ret = CAEN_DGTZ_SetChannelDCOffset(handle, (uint32_t)ch, (uint32_t)((float)(std::fabs(dc[p] - 100))*(655.35)));
				if (ret) std::cout<<"Error setting ch "<<ch<<" test offset"<<std::endl;
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(200));
		CAEN_DGTZ_ClearData(handle);

		ret = CAEN_DGTZ_SWStartAcquisition(handle);
		if (ret)
		{
			std::cout<<"Error starting acquisition"<<std::endl;
			Quit(ERR_DUMMY_LAST);
		}
		
		int value[NACQS][MAX_CH] = { 0 };//baseline values of the NACQS
		for (std::size_t acq = 0; acq < NACQS; acq++)
		{
			CAEN_DGTZ_SendSWtrigger(handle);

			ret = CAEN_DGTZ_ReadData(handle, CAEN_DGTZ_SLAVE_TERMINATED_READOUT_MBLT, buffer, &BufferSize);
			if (ret) Quit(ERR_READOUT);
			if (BufferSize == 0) continue;
			ret = CAEN_DGTZ_GetEventInfo(handle, buffer, BufferSize, 0, &EventInfo, &EventPtr);
			if (ret) Quit(ERR_EVENT_BUILD);
			// decode the event //
			if (dat.WDcfg.Nbit == 8) ret = CAEN_DGTZ_DecodeEvent(handle, EventPtr, (void**)&Event8);
			else ret = CAEN_DGTZ_DecodeEvent(handle, EventPtr, (void**)&Event16);

			if (ret) Quit(ERR_EVENT_BUILD);

			for (std::size_t ch = 0; ch < (int32_t)dat.BoardInfo.Channels; ch++)
			{
					for (std::size_t i = 1; i < 21; i++) //mean over 20 samples
					{
						if (dat.WDcfg.Nbit == 8) value[acq][ch] += (int)(Event8->DataChannel[ch][i]);
						else value[acq][ch] += (int)(Event16->DataChannel[ch][i]);
					}
					value[acq][ch] = (value[acq][ch] / 20);
			}
		}//for acq

		///check for clean baselines
		for (std::size_t ch = 0; ch < (int32_t)dat.BoardInfo.Channels; ch++) 
		{
				int max = 0, ok = 0;
				int mpp = 0;
				int size = (int)pow(2, (double)dat.BoardInfo.ADC_NBits);
				int *freq = static_cast<int*>(calloc(size, sizeof(int)));
				//find most probable value mpp
				for (std::size_t k = 0; k < NACQS; k++) 
				{
					if (value[k][ch] > 0 && value[k][ch] < size) 
					{
						freq[value[k][ch]]++;
						if (freq[value[k][ch]] > max) 
						{
							max = freq[value[k][ch]];
							mpp = value[k][ch];
						}
					}
				}
				free(freq);
				//discard values too far from mpp
				for (std::size_t k = 0; k < NACQS; k++) 
				{
					if (value[k][ch] >= (mpp - 5) && value[k][ch] <= (mpp + 5)) 
					{
						avg_value[p][ch] = avg_value[p][ch] + (float)value[k][ch];
						ok++;
					}
				}
				//calculate final best average value
				avg_value[p][ch] = (avg_value[p][ch] / (float)ok)*100. / (float)size;
		}
		CAEN_DGTZ_SWStopAcquisition(handle);
	}//close for p

	for (std::size_t ch = 0; ch < (int32_t)dat.BoardInfo.Channels; ch++) 
	{
			cal[ch] = ((float)(avg_value[1][ch] - avg_value[0][ch]) / (float)(dc[1] - dc[0]));
			offset[ch] = (float)(dc[1] * avg_value[0][ch] - dc[0] * avg_value[1][ch]) / (float)(dc[1] - dc[0]);
			std::cout<<"Channel "<<ch<<" DAC calibration ready."<<std::endl;
			//printf("Channel %d --> Cal %f   offset %f\n", ch, cal[ch], offset[ch]);
			dat.WDcfg.DAC_Calib.cal[ch] = cal[ch];
			dat.WDcfg.DAC_Calib.offset[ch] = offset[ch];
	}

	CAEN_DGTZ_ClearData(handle);

	 ///free events e buffer
	CAEN_DGTZ_FreeReadoutBuffer(&buffer);
	if (dat.WDcfg.Nbit == 8) CAEN_DGTZ_FreeEvent(handle, (void**)&Event8);
	else CAEN_DGTZ_FreeEvent(handle, (void**)&Event16);
	//reset settings
	ret |= CAEN_DGTZ_SetMaxNumEventsBLT(handle, dat.WDcfg.NumEvents);
	ret |= CAEN_DGTZ_SetPostTriggerSize(handle, dat.WDcfg.PostTrigger);
	ret |= CAEN_DGTZ_SetAcquisitionMode(handle, mem_mode);
	ret |= CAEN_DGTZ_SetExtTriggerInputMode(handle, dat.WDcfg.ExtTriggerMode);
	ret |= CAEN_DGTZ_SetChannelEnableMask(handle, dat.WDcfg.EnableMask);
	if (dat.BoardInfo.FamilyCode == CAEN_DGTZ_XX740_FAMILY_CODE || dat.BoardInfo.FamilyCode == CAEN_DGTZ_XX724_FAMILY_CODE)ret |= CAEN_DGTZ_SetDecimationFactor(handle,dat.WDcfg.DecimationFactor);
	if (ret) std::cout<<"Error resetting some parameters after DAC calibration"<<std::endl;

	//reset self trigger mode settings
	if (dat.BoardInfo.FamilyCode == CAEN_DGTZ_XX730_FAMILY_CODE || dat.BoardInfo.FamilyCode == CAEN_DGTZ_XX725_FAMILY_CODE) 
	{
		// channel pair settings for x730 boards
		for (std::size_t i = 0; i < dat.WDcfg.Nch; i += 2) 
		{
			if (dat.WDcfg.EnableMask & (0x3 << i)) 
			{
				CAEN_DGTZ_TriggerMode_t mode = dat.WDcfg.ChannelTriggerMode[i];
				uint32_t pair_chmask = 0;
				if (dat.WDcfg.ChannelTriggerMode[i] != CAEN_DGTZ_TRGMODE_DISABLED) 
				{
					if (dat.WDcfg.ChannelTriggerMode[i + 1] == CAEN_DGTZ_TRGMODE_DISABLED) pair_chmask = (0x1 << i);
					else	pair_chmask = (0x3 << i);
				}
				else 
				{
					mode = dat.WDcfg.ChannelTriggerMode[i + 1];
					pair_chmask = (0x2 << i);
				}
				pair_chmask &= dat.WDcfg.EnableMask;
				ret |= CAEN_DGTZ_SetChannelSelfTrigger(handle, mode, pair_chmask);
			}
		}
	}
	else 
	{
		for (std::size_t i = 0; i < dat.WDcfg.Nch; i++) 
		{
			if (dat.WDcfg.EnableMask & (1 << i)) ret |= CAEN_DGTZ_SetChannelSelfTrigger(handle, dat.WDcfg.ChannelTriggerMode[i], (1 << i));
		}
	}
	if (ret) std::cout<<"Error resetting self trigger mode after DAC calibration"<<std::endl;
	Save_DAC_Calibration_To_Flash(handle, dat.WDcfg, dat.BoardInfo);
}















std::vector<std::string> Digitizer::ErrMsg{"No Error","Configuration File not found","Can't open the digitizer","Can't read the Board Info","Can't run WaveDump for this digitizer","Can't program the digitizer","Can't allocate the memory for the readout buffer","Interrupt Error","Readout Error","Event Build Error","Can't allocate the memory for the histograms","Unhandled board type","Output file write error","Over Temperature","UNKNOWN"};


/*! \brief   Write the event data into the output files
*

*   \param   WDrun Pointer to the WaveDumpRun data structure
*   \param   WDcfg Pointer to the WaveDumpConfig data structure
*   \param   EventInfo Pointer to the EventInfo data structure
*   \param   Event Pointer to the Event to write
*/
int Digitizer::WriteOutputFiles(void *Event)
{
    int ch, j, ns;
    CAEN_DGTZ_UINT16_EVENT_t  *Event16 = nullptr;
    CAEN_DGTZ_UINT8_EVENT_t   *Event8 = nullptr;
    if (dat.WDcfg.Nbit == 8) Event8 = (CAEN_DGTZ_UINT8_EVENT_t *)Event;
    else Event16 = (CAEN_DGTZ_UINT16_EVENT_t *)Event;

    for (std::size_t ch = 0; ch < dat.WDcfg.Nch; ch++) 
    {
        int Size = (dat. WDcfg.Nbit == 8) ? Event8->ChSize[ch] : Event16->ChSize[ch];
        if (Size <= 0) continue;
        // Check the file format type
        if( dat.WDcfg.OutFileFlags& OFF_BINARY) 
				{
            // Binary file format
            uint32_t BinHeader[6];
            BinHeader[0] = (dat.WDcfg.Nbit == 8) ? Size + 6*sizeof(*BinHeader) : Size*2 + 6*sizeof(*BinHeader);
            BinHeader[1] = dat.EventInfo.BoardId;
            BinHeader[2] = dat.EventInfo.Pattern;
            BinHeader[3] = ch;
            BinHeader[4] = dat.EventInfo.EventCounter;
            BinHeader[5] = dat.EventInfo.TriggerTimeTag;
            if (!dat.WDrun.fout[ch]) {
                char fname[100];
                sprintf(fname, "wave%d.dat", ch);
                if ((dat.WDrun.fout[ch] = fopen(fname, "wb")) == nullptr)
                    return -1;
            }
            if( dat.WDcfg.OutFileFlags & OFF_HEADER) {
                // Write the Channel Header
                if(fwrite(BinHeader, sizeof(*BinHeader), 6, dat.WDrun.fout[ch]) != 6) {
                    // error writing to file
                    fclose(dat.WDrun.fout[ch]);
                    dat.WDrun.fout[ch]= nullptr;
                    return -1;
                }
            }
            if (dat.WDcfg.Nbit == 8)
                ns = (int)fwrite(Event8->DataChannel[ch], 1, Size, dat.WDrun.fout[ch]);
            else
                ns = (int)fwrite(Event16->DataChannel[ch] , 1 , Size*2, dat.WDrun.fout[ch]) / 2;
            if (ns != Size) {
                // error writing to file
                fclose(dat.WDrun.fout[ch]);
                dat.WDrun.fout[ch]= nullptr;
                return -1;
            }
        } else {
            // Ascii file format
            if (!dat.WDrun.fout[ch]) {
                char fname[100];
                sprintf(fname, "wave%d.txt", ch);
                if ((dat.WDrun.fout[ch] = fopen(fname, "w")) == nullptr)
                    return -1;
            }
            if( dat.WDcfg.OutFileFlags & OFF_HEADER) {
                // Write the Channel Header
                fprintf(dat.WDrun.fout[ch], "Record Length: %d\n", Size);
                fprintf(dat.WDrun.fout[ch], "BoardID: %2d\n", dat.EventInfo.BoardId);
                fprintf(dat.WDrun.fout[ch], "Channel: %d\n", ch);
                fprintf(dat.WDrun.fout[ch], "Event Number: %d\n", dat.EventInfo.EventCounter);
                fprintf(dat.WDrun.fout[ch], "Pattern: 0x%04X\n", dat.EventInfo.Pattern & 0xFFFF);
                fprintf(dat.WDrun.fout[ch], "Trigger Time Stamp: %u\n", dat.EventInfo.TriggerTimeTag);
                fprintf(dat.WDrun.fout[ch], "DC offset (DAC): 0x%04X\n", dat.WDcfg.DCoffset[ch] & 0xFFFF);
            }
            for(std::size_t j=0; j<Size; j++) {
                if (dat.WDcfg.Nbit == 8) fprintf(dat.WDrun.fout[ch], "%d\n", Event8->DataChannel[ch][j]);
                else fprintf(dat.WDrun.fout[ch], "%d\n", Event16->DataChannel[ch][j]);
            }
        }
        if (dat.WDrun.SingleWrite) {
            fclose(dat.WDrun.fout[ch]);
            dat.WDrun.fout[ch]= nullptr;
        }
    }
    return 0;

}



/*! \brief   Write the event data on x742 boards into the output files
*
*   \param   WDrun Pointer to the WaveDumpRun data structure
*   \param   WDcfg Pointer to the WaveDumpConfig data structure
*   \param   EventInfo Pointer to the EventInfo data structure
*   \param   Event Pointer to the Event to write
*/
int Digitizer::WriteOutputFilesx742(CAEN_DGTZ_X742_EVENT_t *Event)
{
    int gr,ch, j, ns;
    char trname[10], flag = 0; 
    for (std::size_t gr=0;gr<(dat.WDcfg.Nch/8);gr++) {
        if (Event->GrPresent[gr]) {
            for(ch=0; ch<9; ch++) {
                int Size = Event->DataGroup[gr].ChSize[ch];
                if (Size <= 0) continue;

                // Check the file format type
                if( dat.WDcfg.OutFileFlags& OFF_BINARY) {
                    // Binary file format
                    uint32_t BinHeader[6];
                    BinHeader[0] = (dat.WDcfg.Nbit == 8) ? Size + 6*sizeof(*BinHeader) : Size*4 + 6*sizeof(*BinHeader);
                    BinHeader[1] = dat.EventInfo.BoardId;
                    BinHeader[2] = dat.EventInfo.Pattern;
                    BinHeader[3] = ch;
                    BinHeader[4] = dat.EventInfo.EventCounter;
                    BinHeader[5] = dat.EventInfo.TriggerTimeTag;
                    if (!dat.WDrun.fout[(gr*9+ch)]) {
                        char fname[100];
                        if ((gr*9+ch) == 8) {
                            sprintf(fname, "TR_%d_0.dat", gr);
                            sprintf(trname,"TR_%d_0",gr);
                            flag = 1;
                        }
                        else if ((gr*9+ch) == 17) {
                            sprintf(fname, "TR_0_%d.dat", gr);
                            sprintf(trname,"TR_0_%d",gr);
                            flag = 1;
                        }
                        else if ((gr*9+ch) == 26) {
                            sprintf(fname, "TR_0_%d.dat", gr);
                            sprintf(trname,"TR_0_%d",gr);
                            flag = 1;
                        }
                        else if ((gr*9+ch) == 35) {
                            sprintf(fname, "TR_1_%d.dat", gr);
                            sprintf(trname,"TR_1_%d",gr);
                            flag = 1;
                        }
                        else 	{
                            sprintf(fname, "wave_%d.dat", (gr*8)+ch);
                            flag = 0;
                        }
                        if ((dat.WDrun.fout[(gr*9+ch)] = fopen(fname, "wb")) ==nullptr)
                            return -1;
                    }
                    if( dat.WDcfg.OutFileFlags & OFF_HEADER) {
                        // Write the Channel Header
                        if(fwrite(BinHeader, sizeof(*BinHeader), 6, dat.WDrun.fout[(gr*9+ch)]) != 6) {
                            // error writing to file
                            fclose(dat.WDrun.fout[(gr*9+ch)]);
                            dat.WDrun.fout[(gr*9+ch)]= nullptr;
                            return -1;
                        }
                    }
                    ns = (int)fwrite( Event->DataGroup[gr].DataChannel[ch] , 1 , Size*4, dat.WDrun.fout[(gr*9+ch)]) / 4;
                    if (ns != Size) {
                        // error writing to file
                        fclose(dat.WDrun.fout[(gr*9+ch)]);
                        dat.WDrun.fout[(gr*9+ch)]= nullptr;
                        return -1;
                    }
                } else {
                    // Ascii file format
                    if (!dat.WDrun.fout[(gr*9+ch)]) {
                        char fname[100];
                        if ((gr*9+ch) == 8) {
                            sprintf(fname, "TR_%d_0.txt", gr);
                            sprintf(trname,"TR_%d_0",gr);
                            flag = 1;
                        }
                        else if ((gr*9+ch) == 17) {
                            sprintf(fname, "TR_0_%d.txt", gr);
                            sprintf(trname,"TR_0_%d",gr);
                            flag = 1;
                        }
                        else if ((gr*9+ch) == 26) {
                            sprintf(fname, "TR_0_%d.txt", gr);
                            sprintf(trname,"TR_0_%d",gr);
                            flag = 1;
                        }
                        else if ((gr*9+ch) == 35) {
                            sprintf(fname, "TR_1_%d.txt", gr);
                            sprintf(trname,"TR_1_%d",gr);
                            flag = 1;
                        }
                        else 	{
                            sprintf(fname, "wave_%d.txt", (gr*8)+ch);
                            flag = 0;
                        }
                        if ((dat.WDrun.fout[(gr*9+ch)] = fopen(fname, "w")) == nullptr)
                            return -1;
                    }
                    if( dat.WDcfg.OutFileFlags & OFF_HEADER) {
                        // Write the Channel Header
                        fprintf(dat.WDrun.fout[(gr*9+ch)], "Record Length: %d\n", Size);
                        fprintf(dat.WDrun.fout[(gr*9+ch)], "BoardID: %2d\n", dat.EventInfo.BoardId);
                        if (flag)
                            fprintf(dat.WDrun.fout[(gr*9+ch)], "Channel: %s\n",  trname);
                        else
                            fprintf(dat.WDrun.fout[(gr*9+ch)], "Channel: %d\n",  (gr*8)+ ch);
                        fprintf(dat.WDrun.fout[(gr*9+ch)], "Event Number: %d\n", dat.EventInfo.EventCounter);
                        fprintf(dat.WDrun.fout[(gr*9+ch)], "Pattern: 0x%04X\n", dat.EventInfo.Pattern & 0xFFFF);
                        fprintf(dat.WDrun.fout[(gr*9+ch)], "Trigger Time Stamp: %u\n", Event->DataGroup[gr].TriggerTimeTag);
                        fprintf(dat.WDrun.fout[(gr*9+ch)], "DC offset (DAC): 0x%04X\n", dat.WDcfg.DCoffset[ch] & 0xFFFF);
                        fprintf(dat.WDrun.fout[(gr*9+ch)], "Start Index Cell: %d\n", Event->DataGroup[gr].StartIndexCell);
                        flag = 0;
                    }
                    for(std::size_t j=0; j<Size; j++) {
                        fprintf(dat.WDrun.fout[(gr*9+ch)], "%f\n", Event->DataGroup[gr].DataChannel[ch][j]);
                    }
                }
                if (dat.WDrun.SingleWrite) {
                    fclose(dat.WDrun.fout[(gr*9+ch)]);
                    dat.WDrun.fout[(gr*9+ch)]= nullptr;
                }
            }
        }
    }
    return 0;

}





Digitizer::~Digitizer()
{
    /* stop the acquisition */
    CAEN_DGTZ_SWStopAcquisition(handle);
    /* close the output files and free histograms*/
    for(std::size_t ch = 0; ch < dat.WDcfg.Nch; ch++) 
    {
        if (dat.WDrun.fout[ch]) fclose(dat.WDrun.fout[ch]);
        if (dat.WDrun.Histogram[ch]) free(dat.WDrun.Histogram[ch]);
    }
    /* close the device and free the buffers */
    if(dat.Event8) CAEN_DGTZ_FreeEvent(handle, (void**)&dat.Event8);
    if(dat.Event16) CAEN_DGTZ_FreeEvent(handle, (void**)&dat.Event16);
    if(dat.Event742) CAEN_DGTZ_FreeEvent(handle, (void**)&dat.Event742);
    CAEN_DGTZ_FreeReadoutBuffer(&dat.buffer);
    CAEN_DGTZ_CloseDigitizer(handle);
}

void Digitizer::Quit(const int& error)
{
	if (error!=0) 
	{
		std::cout<<ErrMsg[error]<<std::endl;
		 std::exit(error);
	}
}

void Digitizer::Connect()
{
 		if(CAEN_DGTZ_OpenDigitizer(CAEN_DGTZ_ConnectionType(dat.WDcfg.LinkType), dat.WDcfg.LinkNum, dat.WDcfg.ConetNode, dat.WDcfg.BaseAddress, &handle)) Quit(ERR_DGZ_OPEN);
}

void Digitizer::GetInfos()
{
    if (CAEN_DGTZ_GetInfo(handle, &dat.BoardInfo)) Quit(ERR_BOARD_INFO_READ);
}


/*! \fn      int GetMoreBoardNumChannels(CAEN_DGTZ_BoardInfo_t BoardInfo,  WaveDumpConfig_t *WDcfg)
*   \brief   calculate num of channels, num of bit and sampl period according to the board type
*
*   \param   BoardInfo   Board Type
*   \param   WDcfg       pointer to the config. struct
*   \return  0 = Success; -1 = unknown board type
*/
void Digitizer::GetMoreBoardInfo()
{
	int ret{0};
  switch(dat.BoardInfo.FamilyCode) {
        CAEN_DGTZ_DRS4Frequency_t freq;

    case CAEN_DGTZ_XX724_FAMILY_CODE:
    case CAEN_DGTZ_XX781_FAMILY_CODE:
    case CAEN_DGTZ_XX780_FAMILY_CODE:
        dat.WDcfg.Nbit = 14; dat.WDcfg.Ts = 10.0; break;
    case CAEN_DGTZ_XX720_FAMILY_CODE: dat.WDcfg.Nbit = 12; dat.WDcfg.Ts = 4.0;  break;
    case CAEN_DGTZ_XX721_FAMILY_CODE: dat.WDcfg.Nbit =  8; dat.WDcfg.Ts = 2.0;  break;
    case CAEN_DGTZ_XX731_FAMILY_CODE: dat.WDcfg.Nbit =  8; dat.WDcfg.Ts = 2.0;  break;
    case CAEN_DGTZ_XX751_FAMILY_CODE: dat.WDcfg.Nbit = 10; dat.WDcfg.Ts = 1.0;  break;
    case CAEN_DGTZ_XX761_FAMILY_CODE: dat.WDcfg.Nbit = 10; dat.WDcfg.Ts = 0.25;  break;
    case CAEN_DGTZ_XX740_FAMILY_CODE: dat.WDcfg.Nbit = 12; dat.WDcfg.Ts = 16.0; break;
    case CAEN_DGTZ_XX725_FAMILY_CODE: dat.WDcfg.Nbit = 14; dat.WDcfg.Ts = 4.0; break;
    case CAEN_DGTZ_XX730_FAMILY_CODE: dat.WDcfg.Nbit = 14; dat.WDcfg.Ts = 2.0; break;
    case CAEN_DGTZ_XX742_FAMILY_CODE: 
        dat.WDcfg.Nbit = 12; 
        if ((ret = CAEN_DGTZ_GetDRS4SamplingFrequency(handle, &freq)) != CAEN_DGTZ_Success) ret= CAEN_DGTZ_CommError;
        switch (freq) {
        case CAEN_DGTZ_DRS4_1GHz:
            dat.WDcfg.Ts = 1.0;
            break;
        case CAEN_DGTZ_DRS4_2_5GHz:
            dat.WDcfg.Ts = (float)0.4;
            break;
        case CAEN_DGTZ_DRS4_5GHz:
            dat.WDcfg.Ts = (float)0.2;
            break;
		case CAEN_DGTZ_DRS4_750MHz:
            dat.WDcfg.Ts = (float)(1.0/750.0)*1000.0;
            break;
        }
        switch(dat.BoardInfo.FormFactor) {
        case CAEN_DGTZ_VME64_FORM_FACTOR:
        case CAEN_DGTZ_VME64X_FORM_FACTOR:
            dat.WDcfg.MaxGroupNumber = 4;
            break;
        case CAEN_DGTZ_DESKTOP_FORM_FACTOR:
        case CAEN_DGTZ_NIM_FORM_FACTOR:
        default:
            dat.WDcfg.MaxGroupNumber = 2;
            break;
        }
        break;
    default: ret=-1;
    }
    if (((dat.BoardInfo.FamilyCode == CAEN_DGTZ_XX751_FAMILY_CODE) ||
        (dat.BoardInfo.FamilyCode == CAEN_DGTZ_XX731_FAMILY_CODE) ) && dat.WDcfg.DesMode)
        dat.WDcfg.Ts /= 2;

    switch(dat.BoardInfo.FamilyCode) {
    case CAEN_DGTZ_XX724_FAMILY_CODE:
    case CAEN_DGTZ_XX781_FAMILY_CODE:
    case CAEN_DGTZ_XX780_FAMILY_CODE:
    case CAEN_DGTZ_XX720_FAMILY_CODE:
    case CAEN_DGTZ_XX721_FAMILY_CODE:
    case CAEN_DGTZ_XX751_FAMILY_CODE:
    case CAEN_DGTZ_XX761_FAMILY_CODE:
    case CAEN_DGTZ_XX731_FAMILY_CODE:
        switch(dat.BoardInfo.FormFactor) {
        case CAEN_DGTZ_VME64_FORM_FACTOR:
        case CAEN_DGTZ_VME64X_FORM_FACTOR:
            dat.WDcfg.Nch = 8;
            break;
        case CAEN_DGTZ_DESKTOP_FORM_FACTOR:
        case CAEN_DGTZ_NIM_FORM_FACTOR:
            dat.WDcfg.Nch = 4;
            break;
        }
        break;
    case CAEN_DGTZ_XX725_FAMILY_CODE:
    case CAEN_DGTZ_XX730_FAMILY_CODE:
        switch(dat.BoardInfo.FormFactor) {
        case CAEN_DGTZ_VME64_FORM_FACTOR:
        case CAEN_DGTZ_VME64X_FORM_FACTOR:
            dat.WDcfg.Nch = 16;
            break;
        case CAEN_DGTZ_DESKTOP_FORM_FACTOR:
        case CAEN_DGTZ_NIM_FORM_FACTOR:
            dat.WDcfg.Nch = 8;
            break;
        }
        break;
    case CAEN_DGTZ_XX740_FAMILY_CODE:
        switch( dat.BoardInfo.FormFactor) {
        case CAEN_DGTZ_VME64_FORM_FACTOR:
        case CAEN_DGTZ_VME64X_FORM_FACTOR:
            dat.WDcfg.Nch = 64;
            break;
        case CAEN_DGTZ_DESKTOP_FORM_FACTOR:
        case CAEN_DGTZ_NIM_FORM_FACTOR:
            dat.WDcfg.Nch = 32;
            break;
        }
        break;
    case CAEN_DGTZ_XX742_FAMILY_CODE:
        switch( dat.BoardInfo.FormFactor) {
        case CAEN_DGTZ_VME64_FORM_FACTOR:
        case CAEN_DGTZ_VME64X_FORM_FACTOR:
            dat.WDcfg.Nch = 36;
            break;
        case CAEN_DGTZ_DESKTOP_FORM_FACTOR:
        case CAEN_DGTZ_NIM_FORM_FACTOR:
            dat.WDcfg.Nch = 16;
            break;
        }
        break;
    default:
        ret= -1;
    }
    if(ret!=0) Quit(ERR_INVALID_BOARD_TYPE);
}








void Digitizer::InterruptTimeout()
{
	int ret{0};
	static uint64_t CurrentTime{0};
	static uint64_t ElapsedTime{0};
    int nCycles= 0;
        static int Nb{0};
				static int Ne{0};
 /* Calculate throughput and trigger rate (every second) */
        Nb += BufferSize;
        Ne += NumEvents;
        CurrentTime = get_time();
        ElapsedTime = CurrentTime - PrevRateTime;

        nCycles++;
        if (ElapsedTime > 1000) {
            if (Nb == 0)
                if (ret == CAEN_DGTZ_Timeout) std::cout<<"Timeout..."<<std::endl; else std::cout<<"No data..."<<std::endl;
            else
                printf("Reading at %.2f MB/s (Trg Rate: %.2f Hz)\n", (float)Nb/((float)ElapsedTime*1048.576f), (float)Ne*1000.0f/(float)ElapsedTime);
            nCycles= 0;
            Nb = 0;
            Ne = 0;
            PrevRateTime = CurrentTime;
        }

        /* Analyze data */
        for(std::size_t i = 0; i < (int)NumEvents; i++) {

            /* Get one event from the readout buffer */
            ret = CAEN_DGTZ_GetEventInfo(handle, dat.buffer, BufferSize, i, &dat.EventInfo, &dat.EventPtr);
            if (ret) {
                Quit(ERR_EVENT_BUILD);
            }
            /* decode the event */
            if (dat.WDcfg.Nbit == 8) 
                ret = CAEN_DGTZ_DecodeEvent(handle, dat.EventPtr, (void**)&dat.Event8);
            else
                if (dat.BoardInfo.FamilyCode != CAEN_DGTZ_XX742_FAMILY_CODE) {
                    ret = CAEN_DGTZ_DecodeEvent(handle, dat.EventPtr, (void**)&dat.Event16);
                }
                else {
                    ret = CAEN_DGTZ_DecodeEvent(handle, dat.EventPtr, (void**)&dat.Event742);
                    if (dat.WDcfg.useCorrections != -1) { // if manual corrections
                        uint32_t gr;
                        for (gr = 0; gr < dat.WDcfg.MaxGroupNumber; gr++) {
                            if ( ((dat.WDcfg.EnableMask >> gr) & 0x1) == 0)
                                continue;
                            ::ApplyDataCorrection( &(X742Tables[gr]), dat.WDcfg.DRS4Frequency, dat.WDcfg.useCorrections, &(dat.Event742->DataGroup[gr]));
                        }
                    }
                }
                if (ret) {
                    Quit(ERR_EVENT_BUILD);
                }

                /* Update Histograms */
                if (dat.WDrun.RunHisto) 
                {
                    for(std::size_t ch=0; ch<dat.WDcfg.Nch; ch++) 
										{
                        int chmask = ((dat.BoardInfo.FamilyCode == CAEN_DGTZ_XX740_FAMILY_CODE) || (dat.BoardInfo.FamilyCode == CAEN_DGTZ_XX742_FAMILY_CODE) )? (ch/8) : ch;
                        if (!(dat.EventInfo.ChannelMask & (1<<chmask))) continue;
                        if (dat.WDrun.Histogram[ch] == nullptr) 
												{
                            if ((dat.WDrun.Histogram[ch] = static_cast<uint32_t*>(malloc((uint64_t)(1<<dat.WDcfg.Nbit) * sizeof(uint32_t)))) == nullptr) Quit(ERR_HISTO_MALLOC);
                            //memset(dat.WDrun.Histogram[ch], 0, (uint64_t)(1<<dat.WDcfg.Nbit) * sizeof(uint32_t));
                        }
                        if (dat.WDcfg.Nbit == 8) for(std::size_t i=0; i<(int)dat.Event8->ChSize[ch]; i++) dat.WDrun.Histogram[ch][dat.Event8->DataChannel[ch][i]]++;
                        else {
                            if (dat.BoardInfo.FamilyCode != CAEN_DGTZ_XX742_FAMILY_CODE) {
                                for(i=0; i<(int)dat.Event16->ChSize[ch]; i++) dat.WDrun.Histogram[ch][dat.Event16->DataChannel[ch][i]]++;
                            }
                            else {
                                printf("Can't build samples histogram for this board: it has float samples.\n");
                                dat.WDrun.RunHisto = 0;
			       // dat.WDrun.PlotType = PLOT_WAVEFORMS;
                                break;
                            }
                        }
                    }
                }

                /* Write Event data to file */
                if (dat.WDrun.ContinuousWrite || dat.WDrun.SingleWrite) {
                    // Note: use a thread here to allow parallel readout and file writing
                    if (dat.BoardInfo.FamilyCode == CAEN_DGTZ_XX742_FAMILY_CODE) {	
                        ret = WriteOutputFilesx742(dat.Event742); 
                    }
                    else if (dat.WDcfg.Nbit == 8) {
                        ret = WriteOutputFiles(dat.Event8);
                    }
                    else {
                        ret = WriteOutputFiles(dat.Event16);
                    }
                    if (ret) {
                        Quit(ERR_OUTFILE_WRITE);
                    }
                    if (dat.WDrun.SingleWrite) {
                        printf("Single Event saved to output files\n");
                        dat.WDrun.SingleWrite = 0;
                    }
                }

                /* Plot Waveforms */
                if ((dat.WDrun.ContinuousPlot || dat.WDrun.SinglePlot) ) 
								{
                   /* int Ntraces = (dat.BoardInfo.FamilyCode == CAEN_DGTZ_XX740_FAMILY_CODE) ? 8 : dat.WDcfg.Nch;
                    if (dat.BoardInfo.FamilyCode == CAEN_DGTZ_XX742_FAMILY_CODE) Ntraces = 9;
                    if (dat.PlotVar == nullptr) 
		    						{
                        int TraceLength = std::max(dat.WDcfg.RecordLength, (uint32_t)(1 << dat.WDcfg.Nbit));
                        dat.PlotVar = OpenPlotter(dat.WDcfg.GnuPlotPath, Ntraces, TraceLength);
                        dat.WDrun.SetPlotOptions = 1;
                    }
                    if (dat.PlotVar == nullptr) 
		    						{
                        printf("Can't open the plotter\n");
                        dat.WDrun.ContinuousPlot = 0;
                        dat.WDrun.SinglePlot = 0;
                    } 
		    						else 
		    						{
                        int Tn = 0;
                        if (dat.WDrun.SetPlotOptions) 
												{
                            if ((dat.WDrun.PlotType == PLOT_WAVEFORMS) && (dat.BoardInfo.FamilyCode == CAEN_DGTZ_XX742_FAMILY_CODE)) 
			    									{
                                strcpy(dat.PlotVar->Title, "Waveform");
                                dat.PlotVar->Xscale = dat.WDcfg.Ts;
                                strcpy(dat.PlotVar->Xlabel, "ns");
                                strcpy(dat.PlotVar->Ylabel, "ADC counts");
                                dat.PlotVar->Yautoscale = 0;
                                dat.PlotVar->Ymin = 0;
                                dat.PlotVar->Ymax = (float)(1<<dat.WDcfg.Nbit);
                                dat.PlotVar->Xautoscale = 1;
                            } else if (dat.WDrun.PlotType == PLOT_WAVEFORMS) {
                                strcpy(dat.PlotVar->Title, "Waveform");
                                dat.PlotVar->Xscale = dat.WDcfg.Ts * dat.WDcfg.DecimationFactor/1000;
                                strcpy(dat.PlotVar->Xlabel, "us");
                                strcpy(dat.PlotVar->Ylabel, "ADC counts");
                                dat.PlotVar->Yautoscale = 0;
                                dat.PlotVar->Ymin = 0;
                                dat.PlotVar->Ymax = (float)(1<<dat.WDcfg.Nbit);
                                dat.PlotVar->Xautoscale = 1;
                            }  else if (dat.WDrun.PlotType == PLOT_FFT) {
                                strcpy(dat.PlotVar->Title, "FFT");
                                strcpy(dat.PlotVar->Xlabel, "MHz");
                                strcpy(dat.PlotVar->Ylabel, "dB");
                                dat.PlotVar->Yautoscale = 1;
                                dat.PlotVar->Ymin = -160;
                                dat.PlotVar->Ymax = 0;
                                dat.PlotVar->Xautoscale = 1;
                            } else if (dat.WDrun.PlotType == PLOT_HISTOGRAM) {
                                dat.PlotVar->Xscale = 1.0;
								strcpy(dat.PlotVar->Title, "Histogram");
                                strcpy(dat.PlotVar->Xlabel, "ADC channels");
                                strcpy(dat.PlotVar->Ylabel, "Counts");
                                dat.PlotVar->Yautoscale = 1;
                                dat.PlotVar->Xautoscale = 1;
                            }
                            SetPlotOptions();
                            dat.WDrun.SetPlotOptions = 0;
                        }
   			
                        for(std::size_t ch=0; ch<Ntraces; ch++) 
			{
                            int absCh = dat.WDrun.GroupPlotIndex * 8 + ch;
                            if (!((dat.WDrun.ChannelPlotMask >> ch) & 1)) continue;
                            if ((dat.BoardInfo.FamilyCode == CAEN_DGTZ_XX742_FAMILY_CODE) && ((ch != 0) && (absCh % 8) == 0)) sprintf(dat.PlotVar->TraceName[Tn], "TR %d", (int)((absCh-1) / 16));
                            else sprintf(dat.PlotVar->TraceName[Tn], "CH %d", absCh);
                   else if (dat.WDrun.PlotType == PLOT_FFT) {
                                int FFTns;
                                dat.PlotVar->DataType = PLOT_DATA_DOUBLE;
                                if(dat.WDcfg.Nbit == 8)
                                    FFTns = ::FFT(dat.Event8->DataChannel[absCh], static_cast<double*>(dat.PlotVar->TraceData[Tn]), dat.Event8->ChSize[absCh], HANNING_FFT_WINDOW, SAMPLETYPE_UINT8);
                                else if (dat.BoardInfo.FamilyCode == CAEN_DGTZ_XX742_FAMILY_CODE) {
                                    FFTns = ::FFT(dat.Event742->DataGroup[dat.WDrun.GroupPlotIndex].DataChannel[ch], static_cast<double*>(dat.PlotVar->TraceData[Tn]),
                                        dat.Event742->DataGroup[dat.WDrun.GroupPlotIndex].ChSize[ch], HANNING_FFT_WINDOW, SAMPLETYPE_FLOAT);
                                }
                                else
                                    FFTns = ::FFT(dat.Event16->DataChannel[absCh], static_cast<double*>(dat.PlotVar->TraceData[Tn]), dat.Event16->ChSize[absCh], HANNING_FFT_WINDOW, SAMPLETYPE_UINT16);
                                dat.PlotVar->Xscale = (1000/dat.WDcfg.Ts)/(2*FFTns);
                                dat.PlotVar->TraceSize[Tn] = FFTns;
                            } else if (dat.WDrun.PlotType == PLOT_HISTOGRAM) {
                                dat.PlotVar->DataType = PLOT_DATA_UINT32;
                                strcpy(dat.PlotVar->Title, "Histogram");
                                dat.PlotVar->TraceSize[Tn] = 1<<dat.WDcfg.Nbit;
                                memcpy(dat.PlotVar->TraceData[Tn], dat.WDrun.Histogram[absCh], (uint64_t)(1<<dat.WDcfg.Nbit) * sizeof(uint32_t));
                            }
                            Tn++;
                            if (Tn >= MAX_NUM_TRACES)
                                break;
                        }
                        dat.PlotVar->NumTraces = Tn;
                        if( PlotWaveforms() < 0) 
												{
                            dat.WDrun.ContinuousPlot = 0;
                            printf("Plot Error\n");
                        }
                        dat.WDrun.SinglePlot = 0;
                    }*/
                }
}


}








/*! \fn      static long get_time()
*   \brief   Get time in milliseconds
*
*   \return  time in msec
*/
long Digitizer::get_time()
{
    long time_ms;
#ifdef WIN32
    struct _timeb timebuffer;
    _ftime( &timebuffer );
    time_ms = (long)timebuffer.time * 1000 + (long)timebuffer.millitm;
#else
    struct timeval t1;
    struct timezone tz;
    gettimeofday(&t1, &tz);
    time_ms = (t1.tv_sec) * 1000 + t1.tv_usec / 1000;
#endif
    return time_ms;
}






void Digitizer::Interrupt()
{
          int ret;
          int32_t boardId;
            int VMEHandle = -1;
            int InterruptMask = (1 << VME_INTERRUPT_LEVEL);

            BufferSize = 0;
            NumEvents = 0;
            // Interrupt handling
            if (isVMEDevice()) 
            {
                ret = CAEN_DGTZ_VMEIRQWait ((CAEN_DGTZ_ConnectionType)dat.WDcfg.LinkType, dat.WDcfg.LinkNum, dat.WDcfg.ConetNode, (uint8_t)InterruptMask, INTERRUPT_TIMEOUT, &VMEHandle);
            }
            else ret = CAEN_DGTZ_IRQWait(handle, INTERRUPT_TIMEOUT);
            if (ret == CAEN_DGTZ_Timeout)
            {  // No active interrupt requests
                std::cout<<"toto1"<<std::endl;

                
                InterruptTimeout();
								return;
            }
            if (ret != CAEN_DGTZ_Success)  
            {
                Quit(ERR_INTERRUPT);
            }
            // Interrupt Ack
            if (isVMEDevice()) 
            {
                ret = CAEN_DGTZ_VMEIACKCycle(VMEHandle, VME_INTERRUPT_LEVEL, &boardId);
                if ((ret != CAEN_DGTZ_Success) || (boardId != VME_INTERRUPT_STATUS_ID)) 
 								{
												std::cout<<"toto2"<<std::endl;

                        InterruptTimeout();
								return;
                } 
								else 
								{
                    if (INTERRUPT_MODE == CAEN_DGTZ_IRQ_MODE_ROAK) ret = CAEN_DGTZ_RearmInterrupt(handle);
                }
            }







}


void Digitizer::NextGroup()
{
			if ((dat.BoardInfo.FamilyCode == CAEN_DGTZ_XX730_FAMILY_CODE) || (dat.BoardInfo.FamilyCode == CAEN_DGTZ_XX725_FAMILY_CODE) && (dat.WDcfg.Nch > 8))
			{
				if (dat.WDrun.GroupPlotSwitch == 0) 
				{
					dat.WDrun.GroupPlotSwitch = 1;
					std::cout<<"Channel group set to "<<dat.WDrun.GroupPlotSwitch<<": use numbers 0-7 for channels 8-15"<<std::endl;
				}
				else if(dat.WDrun.GroupPlotSwitch == 1)	
				{
					dat.WDrun.GroupPlotSwitch = 0;
					std::cout<<"Channel group set to "<<dat.WDrun.GroupPlotSwitch<<": use numbers 0-7 for channels 0-7"<<std::endl;
				}
			}
			else /* Update the group plot index*/ if ((dat.WDcfg.EnableMask) && (dat.WDcfg.Nch>8)) GoToNextEnabledGroup();
}















extern int dc_file[MAX_CH];
/*! \fn      void Set_calibrated_DCO(int handle, WaveDumpConfig_t *WDcfg, CAEN_DGTZ_BoardInfo_t BoardInfo)
*   \brief   sets the calibrated DAC value using calibration data (only if BASELINE_SHIFT is in use)
*
*   \param   handle   Digitizer handle
*   \param   WDcfg:   Pointer to the WaveDumpConfig_t data structure
*   \param   BoardInfo: structure with the board info
*/
int Digitizer::Set_calibrated_DCO(const int& ch) 
{
	int ret = CAEN_DGTZ_Success;
	if(dat.WDcfg.Version_used[ch] == 0) /*old DC_OFFSET config, skip calibration*/ return ret;
	if(dat.WDcfg.PulsePolarity[ch] == CAEN_DGTZ_PulsePolarityPositive) 
	{
		dat.WDcfg.DCoffset[ch] = (uint32_t)((float)(std::fabs((((float)dc_file[ch] - dat.WDcfg.DAC_Calib.offset[ch]) / dat.WDcfg.DAC_Calib.cal[ch]) - 100.))*(655.35));
		if (dat.WDcfg.DCoffset[ch] > 65535) dat.WDcfg.DCoffset[ch] = 65535;
		if (dat.WDcfg.DCoffset[ch] < 0) dat.WDcfg.DCoffset[ch] = 0;
	}
	else if (dat.WDcfg.PulsePolarity[ch] == CAEN_DGTZ_PulsePolarityNegative) 
  {
		dat.WDcfg.DCoffset[ch] = (uint32_t)((float)(std::fabs(((fabs(dc_file[ch] - 100.) - dat.WDcfg.DAC_Calib.offset[ch]) / dat.WDcfg.DAC_Calib.cal[ch]) - 100.))*(655.35));
		if (dat.WDcfg.DCoffset[ch] < 0) dat.WDcfg.DCoffset[ch] = 0;
		if (dat.WDcfg.DCoffset[ch] > 65535) dat.WDcfg.DCoffset[ch] = 65535;
	}
	if (dat.BoardInfo.FamilyCode == CAEN_DGTZ_XX740_FAMILY_CODE) 
  {
		ret = CAEN_DGTZ_SetGroupDCOffset(handle, (uint32_t)ch, dat.WDcfg.DCoffset[ch]);
		if (ret) std::cout<<"Error setting group "<<ch<<" offset"<<std::endl;
	}
	else 
	{
		ret = CAEN_DGTZ_SetChannelDCOffset(handle, (uint32_t)ch, dat.WDcfg.DCoffset[ch]);
		if (ret) std::cout<<"Error setting channel "<<ch<<" offset"<<std::endl;
	}
	return ret;
}


void Digitizer::EnableChannelForPloting(const int& ch)
{
    if ((dat.BoardInfo.FamilyCode == CAEN_DGTZ_XX740_FAMILY_CODE) || (dat.BoardInfo.FamilyCode == CAEN_DGTZ_XX742_FAMILY_CODE))
		{
    	if ( (dat.BoardInfo.FamilyCode == CAEN_DGTZ_XX742_FAMILY_CODE) && (dat.WDcfg.FastTriggerEnabled == 0) && (ch == 8)) dat.WDrun.ChannelPlotMask = dat.WDrun.ChannelPlotMask ;
			else dat.WDrun.ChannelPlotMask ^= (1 << ch);
      int toto=ch + dat.WDrun.GroupPlotIndex * 8;
			if ((dat.BoardInfo.FamilyCode == CAEN_DGTZ_XX740_FAMILY_CODE) && (ch == 8)) std::cout<<"Channel "<<toto<<" belongs to a different group"<<std::endl;
			else if (dat.WDrun.ChannelPlotMask & (1 << ch)) std::cout<<"Channel "<<toto<<" enabled for plotting"<<std::endl;
      else std::cout<<"Channel "<<toto<<" disabled for plotting "<<std::endl;
    } 
		else if((dat.BoardInfo.FamilyCode == CAEN_DGTZ_XX730_FAMILY_CODE) || (dat.BoardInfo.FamilyCode == CAEN_DGTZ_XX725_FAMILY_CODE) && (dat.WDcfg.Nch>8)) 
		{
			int toto = ch + 8 * dat.WDrun.GroupPlotSwitch;
			if(toto!= 8 && dat.WDcfg.EnableMask & (1 << toto))
			{		
				dat.WDrun.ChannelPlotMask ^= (1 << toto);
				if (dat.WDrun.ChannelPlotMask & (1 << toto)) std::cout<<"Channel "<<ch<<" enabled for plotting"<<std::endl;
				else std::cout<<"Channel "<<ch<<" disabled for plotting"<<std::endl;
			}
			else std::cout<<"Channel "<<ch<<" not enabled for acquisition"<<std::endl;
		}			
		else 
		{
   		dat.WDrun.ChannelPlotMask ^= (1 << ch);
      if (dat.WDrun.ChannelPlotMask & (1 << ch)) std::cout<<"Channel "<<ch<<" enabled for plotting"<<std::endl;
      else std::cout<<"Channel "<<ch<<" disabled for plotting"<<std::endl;
    }
}














