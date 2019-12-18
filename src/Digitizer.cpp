#include "Digitizer.hpp"
#include "X742CorrectionRoutines.hpp"
#include <chrono>
#include <cmath>
#include <cstring>
#include <iostream>
#include <thread>
#include "flash.cpp"

#define NPOINTS 2
#define NACQS 50
#define VME_INTERRUPT_LEVEL 1
#define VME_INTERRUPT_STATUS_ID 0xAAAA
#define INTERRUPT_TIMEOUT 200 // ms

/*! \fn      int WriteRegisterBitmask(int32_t handle, uint32_t address, uint32_t
 * data, uint32_t mask) \brief   writes 'data' on register at 'address' using
 * 'mask' as bitmask
 *
 *   \param   handle :   Digitizer handle
 *   \param   address:   Address of the Register to write
 *   \param   data   :   Data to Write on the Register
 *   \param   mask   :   Bitmask to use for data masking
 *   \return  0 = Success; negative numbers are error codes
 */
int Digitizer::WriteRegisterBitmask(uint32_t address, uint32_t data,
                                    uint32_t mask) {
  int32_t ret = CAEN_DGTZ_Success;
  uint32_t d32 = 0xFFFFFFFF;

  ret = CAEN_DGTZ_ReadRegister(handle, address, &d32);
  if (ret != CAEN_DGTZ_Success)
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
void Digitizer::ProgramDigitizer() 
{
  
  int ret = 0;
  /* reset the digitizer */
  ret |= CAEN_DGTZ_Reset(handle);
  if (ret != 0) 
  {
    std::cout << "Error: Unable to reset digitizer.\nPlease reset digitizer "
                 "manually then restart the program"
              << std::endl;
     if (ret) Quit(ERR_DGZ_PROGRAM);
  }
  // Set the waveform test bit for debugging
  if (dat.WDcfg.TestPattern)
    ret |= CAEN_DGTZ_WriteRegister(handle, CAEN_DGTZ_BROAD_CH_CONFIGBIT_SET_ADD,
                                   1 << 3);
  // custom setting for X742 boards
  if (dat.BoardInfo.FamilyCode == CAEN_DGTZ_XX742_FAMILY_CODE) {
    ret |= CAEN_DGTZ_SetFastTriggerDigitizing(
        handle, CAEN_DGTZ_EnaDis_t(dat.WDcfg.FastTriggerEnabled));
    ret |= CAEN_DGTZ_SetFastTriggerMode(handle, dat.WDcfg.FastTriggerMode);
  }
  if ((dat.BoardInfo.FamilyCode == CAEN_DGTZ_XX751_FAMILY_CODE) ||
      (dat.BoardInfo.FamilyCode == CAEN_DGTZ_XX731_FAMILY_CODE)) {
    ret |= CAEN_DGTZ_SetDESMode(handle, dat.WDcfg.DesMode);
  }
  ret |= CAEN_DGTZ_SetRecordLength(handle, dat.WDcfg.RecordLength);
  ret |= CAEN_DGTZ_GetRecordLength(handle, &dat.WDcfg.RecordLength);

  if (dat.BoardInfo.FamilyCode == CAEN_DGTZ_XX740_FAMILY_CODE ||
      dat.BoardInfo.FamilyCode == CAEN_DGTZ_XX724_FAMILY_CODE) {
    ret |= CAEN_DGTZ_SetDecimationFactor(handle, dat.WDcfg.DecimationFactor);
  }
  ret |= CAEN_DGTZ_SetPostTriggerSize(handle, dat.WDcfg.PostTrigger);
  if (dat.BoardInfo.FamilyCode != CAEN_DGTZ_XX742_FAMILY_CODE) {
    uint32_t pt;
    ret |= CAEN_DGTZ_GetPostTriggerSize(handle, &pt);
    dat.WDcfg.PostTrigger = pt;
  }
  ret |= CAEN_DGTZ_SetIOLevel(handle, dat.WDcfg.FPIOtype);
  if (dat.WDcfg.InterruptNumEvents > 0) {
    // Interrupt handling
    if (ret |=
        CAEN_DGTZ_SetInterruptConfig(
            handle, CAEN_DGTZ_ENABLE, VME_INTERRUPT_LEVEL,
            VME_INTERRUPT_STATUS_ID, (uint16_t)dat.WDcfg.InterruptNumEvents,
            INTERRUPT_MODE) != CAEN_DGTZ_Success) {
      std::cout << "\nError configuring interrupts. Interrupts disabled\n"
                << std::endl;
      dat.WDcfg.InterruptNumEvents = 0;
    }
  }

  ret |= CAEN_DGTZ_SetMaxNumEventsBLT(handle, dat.WDcfg.NumEvents);
  ret |= CAEN_DGTZ_SetAcquisitionMode(handle, CAEN_DGTZ_SW_CONTROLLED);
  ret |= CAEN_DGTZ_SetExtTriggerInputMode(handle, dat.WDcfg.ExtTriggerMode);

  if ((dat.BoardInfo.FamilyCode == CAEN_DGTZ_XX740_FAMILY_CODE) ||
      (dat.BoardInfo.FamilyCode == CAEN_DGTZ_XX742_FAMILY_CODE)) {
    ret |= CAEN_DGTZ_SetGroupEnableMask(handle, dat.WDcfg.EnableMask);
    for (std::size_t i = 0; i < (dat.WDcfg.Nch / 8); i++) {
      if (dat.WDcfg.EnableMask & (1 << i)) {
        if (dat.BoardInfo.FamilyCode == CAEN_DGTZ_XX742_FAMILY_CODE) {
          for (std::size_t j = 0; j < 8; j++) {
            if (dat.WDcfg.DCoffsetGrpCh[i][j] != -1)
              ret |= CAEN_DGTZ_SetChannelDCOffset(
                  handle, (i * 8) + j, dat.WDcfg.DCoffsetGrpCh[i][j]);
            else
              ret |= CAEN_DGTZ_SetChannelDCOffset(handle, (i * 8) + j,
                                                  dat.WDcfg.DCoffset[i]);
          }
        } else {
          if (dat.WDcfg.Version_used[i] == 1)
            ret |= Set_calibrated_DCO(i);
          else
            ret |= CAEN_DGTZ_SetGroupDCOffset(handle, i, dat.WDcfg.DCoffset[i]);
          ret |= CAEN_DGTZ_SetGroupSelfTrigger(
              handle, dat.WDcfg.ChannelTriggerMode[i], (1 << i));
          ret |= CAEN_DGTZ_SetGroupTriggerThreshold(handle, i,
                                                    dat.WDcfg.Threshold[i]);
          ret |= CAEN_DGTZ_SetChannelGroupMask(handle, i,
                                               dat.WDcfg.GroupTrgEnableMask[i]);
        }
        ret |= CAEN_DGTZ_SetTriggerPolarity(
            handle, i,
            CAEN_DGTZ_TriggerPolarity_t(
                dat.WDcfg.PulsePolarity[i])); //.TriggerEdge
      }
    }
  } else {
    ret |= CAEN_DGTZ_SetChannelEnableMask(handle, dat.WDcfg.EnableMask);
    for (std::size_t i = 0; i < dat.WDcfg.Nch; i++) {
      if (dat.WDcfg.EnableMask & (1 << i)) {
        if (dat.WDcfg.Version_used[i] == 1)
          ret |= Set_calibrated_DCO(i);
        else
          ret |= CAEN_DGTZ_SetChannelDCOffset(handle, i, dat.WDcfg.DCoffset[i]);
        if (dat.BoardInfo.FamilyCode != CAEN_DGTZ_XX730_FAMILY_CODE &&
            dat.BoardInfo.FamilyCode != CAEN_DGTZ_XX725_FAMILY_CODE)
          ret |= CAEN_DGTZ_SetChannelSelfTrigger(
              handle, dat.WDcfg.ChannelTriggerMode[i], (1 << i));
        ret |= CAEN_DGTZ_SetChannelTriggerThreshold(handle, i,
                                                    dat.WDcfg.Threshold[i]);
        ret |= CAEN_DGTZ_SetTriggerPolarity(
            handle, i,
            CAEN_DGTZ_TriggerPolarity_t(
                dat.WDcfg.PulsePolarity[i])); //.TriggerEdge
      }
    }
    if (dat.BoardInfo.FamilyCode == CAEN_DGTZ_XX730_FAMILY_CODE ||
        dat.BoardInfo.FamilyCode == CAEN_DGTZ_XX725_FAMILY_CODE) {
      // channel pair settings for x730 boards
      for (std::size_t i = 0; i < dat.WDcfg.Nch; i += 2) {
        if (dat.WDcfg.EnableMask & (0x3 << i)) {
          CAEN_DGTZ_TriggerMode_t mode = dat.WDcfg.ChannelTriggerMode[i];
          uint32_t pair_chmask = 0;

          // Build mode and relevant channelmask. The behaviour is that,
          // if the triggermode of one channel of the pair is DISABLED,
          // this channel doesn't take part to the trigger generation.
          // Otherwise, if both are different from DISABLED, the one of
          // the even channel is used.
          if (dat.WDcfg.ChannelTriggerMode[i] != CAEN_DGTZ_TRGMODE_DISABLED) {
            if (dat.WDcfg.ChannelTriggerMode[i + 1] ==
                CAEN_DGTZ_TRGMODE_DISABLED)
              pair_chmask = (0x1 << i);
            else
              pair_chmask = (0x3 << i);
          } else {
            mode = dat.WDcfg.ChannelTriggerMode[i + 1];
            pair_chmask = (0x2 << i);
          }

          pair_chmask &= dat.WDcfg.EnableMask;
          ret |= CAEN_DGTZ_SetChannelSelfTrigger(handle, mode, pair_chmask);
        }
      }
    }
  }
  if (dat.BoardInfo.FamilyCode == CAEN_DGTZ_XX742_FAMILY_CODE) {
    for (std::size_t i = 0; i < (dat.WDcfg.Nch / 8); i++) {
      ret |=
          CAEN_DGTZ_SetDRS4SamplingFrequency(handle, dat.WDcfg.DRS4Frequency);
      ret |= CAEN_DGTZ_SetGroupFastTriggerDCOffset(handle, i,
                                                   dat.WDcfg.FTDCoffset[i]);
      ret |= CAEN_DGTZ_SetGroupFastTriggerThreshold(handle, i,
                                                    dat.WDcfg.FTThreshold[i]);
    }
  }
  /* execute generic write commands */
  for (std::size_t i = 0; i < dat.WDcfg.GWn; i++)
    ret |= WriteRegisterBitmask(dat.WDcfg.GWaddr[i], dat.WDcfg.GWdata[i],
                                dat.WDcfg.GWmask[i]);
  if (ret)
    std::cout << "Warning: errors found during the programming of the "
                 "digitizer.\nSome settings may not be executed\n"
              << std::endl;
  if (ret) Quit(ERR_DGZ_PROGRAM);
}

/*! \brief   Write the event data on x742 boards into the output files*/
void Digitizer::calibrate() {
  std::cout << std::endl;
  if (BoardSupportsCalibration()) {
    if (isStarted == false) {
      int32_t ret = CAEN_DGTZ_Calibrate(handle);
      if (ret == CAEN_DGTZ_Success)
        std::cout << "ADC Calibration successfully executed." << std::endl;
      else
        std::cout << "ADC Calibration failed. CAENDigitizer ERR " << ret
                  << std::endl;
      std::cout << std::endl;
    } else
      std::cout << "Can't run ADC calibration while acquisition is running."
                << std::endl;
  } else
    std::cout << "ADC Calibration not needed for this board family."
              << std::endl;
}


/*! \fn      void Calibrate_XX740_DC_Offset(int handle, WaveDumpConfig_t WDcfg,
 * CAEN_DGTZ_BoardInfo_t BoardInfo) \brief   calibrates DAC of enabled channel
 * groups (only if BASELINE_SHIFT is in use)*/
void Digitizer::Calibrate_XX740_DC_Offset() {
  float cal[MAX_CH];
  float offset[MAX_CH] = {0};
  for (std::size_t i = 0; i < MAX_CH; i++)
    cal[i] = 1;
  int ret;
  CAEN_DGTZ_AcqMode_t mem_mode;

  float avg_value[NPOINTS][MAX_CH] = {0};
  uint32_t dc[NPOINTS] = {25, 75}; // test values (%)
  uint32_t groupmask = 0;

  ret = CAEN_DGTZ_GetAcquisitionMode(handle, &mem_mode); // chosen value stored
  if (ret)
    std::cout << "Error trying to read acq mode!!" << std::endl;
  ret = CAEN_DGTZ_SetAcquisitionMode(handle, CAEN_DGTZ_SW_CONTROLLED);
  if (ret)
    std::cout << "Error trying to set acq mode!!" << std::endl;
  ret = CAEN_DGTZ_SetExtTriggerInputMode(handle, CAEN_DGTZ_TRGMODE_DISABLED);
  if (ret)
    std::cout << "Error trying to set ext trigger!!" << std::endl;
  ret = CAEN_DGTZ_SetMaxNumEventsBLT(handle, 1);
  if (ret)
    std::cout << "Warning: error setting max BLT number" << std::endl;
  ret = CAEN_DGTZ_SetDecimationFactor(handle, 1);
  if (ret)
    std::cout << "Error trying to set decimation factor!!" << std::endl;
  for (std::size_t g = 0; g < (int32_t)dat.BoardInfo.Channels;
       g++) /*BoardInfo.Channels is number of groups for x740 boards*/
    groupmask |= (1 << g);
  ret = CAEN_DGTZ_SetGroupSelfTrigger(handle, CAEN_DGTZ_TRGMODE_DISABLED,
                                      groupmask);
  if (ret)
    std::cout << "Error disabling self trigger" << std::endl;
  ret = CAEN_DGTZ_SetGroupEnableMask(handle, groupmask);
  if (ret)
    std::cout << "Error enabling channel groups." << std::endl;
  /// malloc
  ret = CAEN_DGTZ_MallocReadoutBuffer(handle, &buffer, &AllocatedSize);
  if (ret)
    Quit(ERR_MALLOC);
  ret = CAEN_DGTZ_AllocateEvent(handle, (void **)&dat.Event16);
  if (ret != CAEN_DGTZ_Success)
    Quit(ERR_MALLOC);

  std::cout << "Starting DAC calibration..." << std::endl;

  for (std::size_t p = 0; p < NPOINTS; p++) {
    for (std::size_t i = 0; i < (int32_t)dat.BoardInfo.Channels;
         i++) { // BoardInfo.Channels is number of groups for x740 boards
      ret = CAEN_DGTZ_SetGroupDCOffset(
          handle, (uint32_t)i,
          (uint32_t)((float)(fabs(dc[p] - 100)) * (655.35)));
      if (ret)
        std::cout << "Error setting group " << i << " test offset" << std::endl;
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    CAEN_DGTZ_ClearData(handle);
    ret = CAEN_DGTZ_SWStartAcquisition(handle);
    if (ret) {
      std::cout << "Error starting X740 acquisition" << std::endl;
      Quit(ERR_DUMMY_LAST);
    }
    int value[NACQS][MAX_CH] = {0}; // baseline values of the NACQS
    for (std::size_t acq = 0; acq < NACQS; acq++) {
      CAEN_DGTZ_SendSWtrigger(handle);

      ret = CAEN_DGTZ_ReadData(handle, CAEN_DGTZ_SLAVE_TERMINATED_READOUT_MBLT,
                               buffer, &BufferSize);
      if (ret)
        Quit(ERR_READOUT);
      if (BufferSize == 0)
        continue;
      GetEvent(0);
      // decode the event //
      DecodeEvent();
      for (std::size_t g = 0; g < (int32_t)dat.BoardInfo.Channels; g++) {
        for (std::size_t k = 1; k < 21; k++) // mean over 20 samples
          value[acq][g] += (int)(dat.Event16->DataChannel[g * 8][k]);
        value[acq][g] = (value[acq][g] / 20);
      }
    } // for acq

    /// check for clean baselines
    for (std::size_t g = 0; g < (int32_t)dat.BoardInfo.Channels; g++) {
      int max = 0;
      int mpp = 0;
      int size = (int)pow(2, (double)dat.BoardInfo.ADC_NBits);
      int *freq = static_cast<int *>(calloc(size, sizeof(int)));
      // find the most probable value mpp
      for (std::size_t k = 0; k < NACQS; k++) {
        if (value[k][g] > 0 && value[k][g] < size) {
          freq[value[k][g]]++;
          if (freq[value[k][g]] > max) {
            max = freq[value[k][g]];
            mpp = value[k][g];
          }
        }
      }
      free(freq);
      // discard values too far from mpp
      int ok = 0;
      for (std::size_t k = 0; k < NACQS; k++) {
        if (value[k][g] >= (mpp - 5) && value[k][g] <= (mpp + 5)) {
          avg_value[p][g] = avg_value[p][g] + (float)value[k][g];
          ok++;
        }
      }
      avg_value[p][g] = (avg_value[p][g] / (float)ok) * 100. / (float)size;
    }

    CAEN_DGTZ_SWStopAcquisition(handle);
  } // close for p

  for (std::size_t g = 0; g < (int32_t)dat.BoardInfo.Channels; g++) {
    cal[g] =
        ((float)(avg_value[1][g] - avg_value[0][g]) / (float)(dc[1] - dc[0]));
    offset[g] = (float)(dc[1] * avg_value[0][g] - dc[0] * avg_value[1][g]) /
                (float)(dc[1] - dc[0]);
    std::cout << "Group " << g << " DAC calibration ready." << std::endl;
    std::cout << "Cal :" << cal[g] << "   offset " << offset[g] << std::endl;
    dat.WDcfg.DAC_Calib.cal[g] = cal[g];
    dat.WDcfg.DAC_Calib.offset[g] = offset[g];
  }
  CAEN_DGTZ_ClearData(handle);
  /// free events e buffer
  FreeBuffer();
  FreeEvent();
  ret |= CAEN_DGTZ_SetMaxNumEventsBLT(handle, dat.WDcfg.NumEvents);
  ret |= CAEN_DGTZ_SetDecimationFactor(handle, dat.WDcfg.DecimationFactor);
  ret |= CAEN_DGTZ_SetPostTriggerSize(handle, dat.WDcfg.PostTrigger);
  ret |= CAEN_DGTZ_SetAcquisitionMode(handle, mem_mode);
  ret |= CAEN_DGTZ_SetExtTriggerInputMode(handle, dat.WDcfg.ExtTriggerMode);
  ret |= CAEN_DGTZ_SetGroupEnableMask(handle, dat.WDcfg.EnableMask);
  for (std::size_t i = 0; i < dat.BoardInfo.Channels; i++) {
    if (dat.WDcfg.EnableMask & (1 << i))
      ret |= CAEN_DGTZ_SetGroupSelfTrigger(
          handle, dat.WDcfg.ChannelTriggerMode[i], (1 << i));
  }
  if (ret)
    std::cout << "Error setting recorded parameters" << std::endl;
  Save_DAC_Calibration_To_Flash();
}

extern int thr_file[MAX_CH];

/*! \fn      void Set_relative_Threshold(int handle, WaveDumpConfig_t *WDcfg,
 * CAEN_DGTZ_BoardInfo_t BoardInfo) \brief   sets the threshold relative to the
 * baseline (only if BASELINE_SHIFT is in use)
 *
 *   \param   handle   Digitizer handle
 *   \param   WDcfg:   Pointer to the WaveDumpConfig_t data structure
 *   \param   BoardInfo: structure with the board info
 */
void Digitizer::Set_relative_Threshold() {

  // preliminary check: if baseline shift is not enabled for any channel quit
  int should_start = 0;
  for (std::size_t ch = 0; ch < (int32_t)dat.BoardInfo.Channels; ch++) {
    if (dat.WDcfg.EnableMask & (1 << ch) && dat.WDcfg.Version_used[ch] == 1) {
      should_start = 1;
      break;
    }
  }
  if (!should_start)
    return;

  int ret;
  uint32_t custom_posttrg = 50, dco, custom_thr;
  float expected_baseline;
  float dco_percent;
  int baseline[MAX_CH] = {0}, size = 0, samples = 0;
  int no_self_triggered_event[MAX_CH] = {0};
  int sw_trigger_needed = 0;
  int event_ch;
  /// malloc
  ret = CAEN_DGTZ_MallocReadoutBuffer(handle, &buffer, &AllocatedSize);
  if (ret)
    Quit(ERR_MALLOC);
  if (dat.WDcfg.Nbit == 8)
    ret = CAEN_DGTZ_AllocateEvent(handle, (void **)&dat.Event8);
  else
    ret = CAEN_DGTZ_AllocateEvent(handle, (void **)&dat.Event16);
  if (ret != CAEN_DGTZ_Success)
    Quit(ERR_MALLOC);

  // some custom settings
  ret = CAEN_DGTZ_SetPostTriggerSize(handle, custom_posttrg);
  if (ret) {
    std::cout << "Threshold calc failed. Error trying to set post trigger!!"
              << std::endl;
    return;
  }
  // try to set a small threshold to get a self triggered event
  for (std::size_t ch = 0; ch < (int32_t)dat.BoardInfo.Channels; ch++) {
    if (dat.WDcfg.EnableMask & (1 << ch) && dat.WDcfg.Version_used[ch] == 1) {
      if (dat.BoardInfo.FamilyCode == CAEN_DGTZ_XX740_FAMILY_CODE)
        ret = CAEN_DGTZ_GetGroupDCOffset(handle, ch, &dco);
      else
        ret = CAEN_DGTZ_GetChannelDCOffset(handle, ch, &dco);
      if (ret) {
        std::cout
            << "Threshold calc failed. Error trying to get DCoffset values!!"
            << std::endl;
        return;
      }
      dco_percent = (float)dco / 65535.;
      expected_baseline =
          pow(2, (double)dat.BoardInfo.ADC_NBits) * (1.0 - dco_percent);

      custom_thr =
          (dat.WDcfg.PulsePolarity[ch] == CAEN_DGTZ_PulsePolarityPositive)
              ? ((uint32_t)expected_baseline + 100)
              : ((uint32_t)expected_baseline - 100);

      if (dat.BoardInfo.FamilyCode == CAEN_DGTZ_XX740_FAMILY_CODE)
        ret = CAEN_DGTZ_SetGroupTriggerThreshold(handle, ch, custom_thr);
      else
        ret = CAEN_DGTZ_SetChannelTriggerThreshold(handle, ch, custom_thr);
      if (ret) {
        std::cout << "Threshold calc failed. Error trying to set custom "
                     "threshold value!!"
                  << std::endl;
        return;
      }
    }
  }

  CAEN_DGTZ_SWStartAcquisition(handle);
  std::this_thread::sleep_for(std::chrono::milliseconds(300));

  ret = CAEN_DGTZ_ReadData(handle, CAEN_DGTZ_SLAVE_TERMINATED_READOUT_MBLT,
                           buffer, &BufferSize);
  if (ret)
    Quit(ERR_READOUT);
  // we have some self-triggered event
  if (BufferSize > 0) {
    GetEvent(0);
    // decode the event //
    DecodeEvent();

    for (std::size_t ch = 0; ch < (int32_t)dat.BoardInfo.Channels; ch++) {
      if (dat.WDcfg.EnableMask & (1 << ch) && dat.WDcfg.Version_used[ch] == 1) {
        event_ch = (dat.BoardInfo.FamilyCode == CAEN_DGTZ_XX740_FAMILY_CODE)
                       ? (ch * 8)
                       : ch; // for x740 boards shift to channel 0 of next group
        size = (dat.WDcfg.Nbit == 8) ? dat.Event8->ChSize[event_ch]
                                     : dat.Event16->ChSize[event_ch];
        if (size == 0) { // no data from channel ch
          no_self_triggered_event[ch] = 1;
          sw_trigger_needed = 1;
          continue;
        } else { // use only one tenth of the pre-trigger samples to calculate
                 // the baseline
          samples = (int)(size * ((100 - custom_posttrg) / 2) / 100);
          for (std::size_t i = 0; i < samples;
               i++) // mean over some pre trigger samples
          {
            if (dat.WDcfg.Nbit == 8)
              baseline[ch] += (int)(dat.Event8->DataChannel[event_ch][i]);
            else
              baseline[ch] += (int)(dat.Event16->DataChannel[event_ch][i]);
          }
          baseline[ch] = (baseline[ch] / samples);
        }
        if (dat.WDcfg.PulsePolarity[ch] == CAEN_DGTZ_PulsePolarityPositive)
          dat.WDcfg.Threshold[ch] = (uint32_t)baseline[ch] + thr_file[ch];
        else if (dat.WDcfg.PulsePolarity[ch] == CAEN_DGTZ_PulsePolarityNegative)
          dat.WDcfg.Threshold[ch] = (uint32_t)baseline[ch] - thr_file[ch];

        if (dat.WDcfg.Threshold[ch] < 0)
          dat.WDcfg.Threshold[ch] = 0;
        size = (int)pow(2, (double)dat.BoardInfo.ADC_NBits);
        if (dat.WDcfg.Threshold[ch] > (uint32_t)size)
          dat.WDcfg.Threshold[ch] = size;

        if (dat.BoardInfo.FamilyCode == CAEN_DGTZ_XX740_FAMILY_CODE)
          ret = CAEN_DGTZ_SetGroupTriggerThreshold(handle, ch,
                                                   dat.WDcfg.Threshold[ch]);
        else
          ret = CAEN_DGTZ_SetChannelTriggerThreshold(handle, ch,
                                                     dat.WDcfg.Threshold[ch]);
        if (ret)
          std::cout << "Warning: error setting ch " << ch
                    << " corrected threshold" << std::endl;
      }
    }
  } else {
    sw_trigger_needed = 1;
    for (std::size_t ch = 0; ch < (int32_t)dat.BoardInfo.Channels; ch++)
      no_self_triggered_event[ch] = 1;
  }

  CAEN_DGTZ_ClearData(handle);

  // if from some channels we had no self triggered event, we send a software
  // trigger
  if (sw_trigger_needed) {
    CAEN_DGTZ_SendSWtrigger(handle);
    ret = CAEN_DGTZ_ReadData(handle, CAEN_DGTZ_SLAVE_TERMINATED_READOUT_MBLT,
                             buffer, &BufferSize);
    if (ret)
      Quit(ERR_READOUT);
    if (BufferSize == 0)
      return;

    GetEvent(0);
    DecodeEvent();

    for (std::size_t ch = 0; ch < (int32_t)dat.BoardInfo.Channels; ch++) {
      if (dat.WDcfg.EnableMask & (1 << ch) && dat.WDcfg.Version_used[ch] == 1) {
        event_ch = (dat.BoardInfo.FamilyCode == CAEN_DGTZ_XX740_FAMILY_CODE)
                       ? (ch * 8)
                       : ch; // for x740 boards shift to channel 0 of next group
        size = (dat.WDcfg.Nbit == 8) ? dat.Event8->ChSize[event_ch]
                                     : dat.Event16->ChSize[event_ch];
        if (!no_self_triggered_event[ch]) // we already have a good baseline for
                                          // channel ch
          continue;

        // use some samples to calculate the baseline
        for (std::size_t i = 1; i < 11; i++) { // mean over 10 samples
          if (dat.WDcfg.Nbit == 8)
            baseline[ch] += (int)(dat.Event8->DataChannel[event_ch][i]);
          else
            baseline[ch] += (int)(dat.Event16->DataChannel[event_ch][i]);
        }
        baseline[ch] = (baseline[ch] / 10);
      }

      if (dat.WDcfg.PulsePolarity[ch] == CAEN_DGTZ_PulsePolarityPositive)
        dat.WDcfg.Threshold[ch] = (uint32_t)baseline[ch] + thr_file[ch];
      else if (dat.WDcfg.PulsePolarity[ch] == CAEN_DGTZ_PulsePolarityNegative)
        dat.WDcfg.Threshold[ch] = (uint32_t)baseline[ch] - thr_file[ch];

      if (dat.WDcfg.Threshold[ch] < 0)
        dat.WDcfg.Threshold[ch] = 0;
      size = (int)pow(2, (double)dat.BoardInfo.ADC_NBits);
      if (dat.WDcfg.Threshold[ch] > (uint32_t)size)
        dat.WDcfg.Threshold[ch] = size;

      if (dat.BoardInfo.FamilyCode == CAEN_DGTZ_XX740_FAMILY_CODE)
        ret = CAEN_DGTZ_SetGroupTriggerThreshold(handle, ch,
                                                 dat.WDcfg.Threshold[ch]);
      else
        ret = CAEN_DGTZ_SetChannelTriggerThreshold(handle, ch,
                                                   dat.WDcfg.Threshold[ch]);
      if (ret)
        std::cout << "Warning: error setting ch " << ch
                  << " corrected threshold" << std::endl;
    }
  } // end sw trigger event analysis

  CAEN_DGTZ_SWStopAcquisition(handle);

  // reset posttrigger
  ret = CAEN_DGTZ_SetPostTriggerSize(handle, dat.WDcfg.PostTrigger);
  if (ret)
    std::cout << "Error resetting post trigger." << std::endl;

  CAEN_DGTZ_ClearData(handle);

  FreeBuffer();
  FreeEvent();
}

/*! \brief   return TRUE if board descriped by 'BoardInfo' supports
 *            calibration or not.
 *
 *   \param   BoardInfo board descriptor
 */
int32_t Digitizer::BoardSupportsCalibration() {
  return dat.BoardInfo.FamilyCode == CAEN_DGTZ_XX761_FAMILY_CODE ||
         dat.BoardInfo.FamilyCode == CAEN_DGTZ_XX751_FAMILY_CODE ||
         dat.BoardInfo.FamilyCode == CAEN_DGTZ_XX730_FAMILY_CODE ||
         dat.BoardInfo.FamilyCode == CAEN_DGTZ_XX725_FAMILY_CODE;
}

/*! \brief   return TRUE if board descriped by 'BoardInfo' supports
 *            temperature read or not.
 *
 *   \param   BoardInfo board descriptor
 */
int32_t Digitizer::BoardSupportsTemperatureRead() {
  return dat.BoardInfo.FamilyCode == CAEN_DGTZ_XX751_FAMILY_CODE ||
         dat.BoardInfo.FamilyCode == CAEN_DGTZ_XX730_FAMILY_CODE ||
         dat.BoardInfo.FamilyCode == CAEN_DGTZ_XX725_FAMILY_CODE;
}

/*! \fn      void Calibrate_DC_Offset(int handle, WaveDumpConfig_t WDcfg,
 * CAEN_DGTZ_BoardInfo_t BoardInfo) \brief   calibrates DAC of enabled channels
 * (only if BASELINE_SHIFT is in use)
 *
 *   \param   handle   Digitizer handle
 *   \param   WDcfg:   Pointer to the WaveDumpConfig_t data structure
 *   \param   BoardInfo: structure with the board info
 */
void Digitizer::Calibrate_DC_Offset() {
  float cal[MAX_CH];
  float offset[MAX_CH] = {0};
  for (std::size_t i = 0; i < MAX_CH; i++)
    cal[i] = 1;
  int ret;
  CAEN_DGTZ_AcqMode_t mem_mode;

  float avg_value[NPOINTS][MAX_CH] = {0};
  uint32_t dc[NPOINTS] = {25, 75}; // test values (%)
  uint32_t chmask = 0;

  ret = CAEN_DGTZ_GetAcquisitionMode(handle, &mem_mode); // chosen value stored
  if (ret)
    std::cout << "Error trying to read acq mode!!" << std::endl;
  ret = CAEN_DGTZ_SetAcquisitionMode(handle, CAEN_DGTZ_SW_CONTROLLED);
  if (ret)
    std::cout << "Error trying to set acq mode!!" << std::endl;
  ret = CAEN_DGTZ_SetExtTriggerInputMode(handle, CAEN_DGTZ_TRGMODE_DISABLED);
  if (ret)
    std::cout << "Error trying to set ext trigger!!" << std::endl;
  for (std::size_t ch = 0; ch < (int32_t)dat.BoardInfo.Channels; ch++)
    chmask |= (1 << ch);
  ret = CAEN_DGTZ_SetChannelSelfTrigger(handle, CAEN_DGTZ_TRGMODE_DISABLED,
                                        chmask);
  if (ret)
    std::cout << "Warning: error disabling channels self trigger" << std::endl;
  ret = CAEN_DGTZ_SetChannelEnableMask(handle, chmask);
  if (ret)
    std::cout << "Warning: error enabling channels." << std::endl;
  ret = CAEN_DGTZ_SetMaxNumEventsBLT(handle, 1);
  if (ret)
    std::cout << "Warning: error setting max BLT number" << std::endl;
  if (dat.BoardInfo.FamilyCode == CAEN_DGTZ_XX740_FAMILY_CODE ||
      dat.BoardInfo.FamilyCode == CAEN_DGTZ_XX724_FAMILY_CODE) {
    ret = CAEN_DGTZ_SetDecimationFactor(handle, 1);
    if (ret)
      std::cout << "Error trying to set decimation factor!!" << std::endl;
  }

  /// malloc
  ret = CAEN_DGTZ_MallocReadoutBuffer(handle, &buffer, &AllocatedSize);
  if (ret)
    Quit(ERR_MALLOC);
  if (dat.WDcfg.Nbit == 8)
    ret = CAEN_DGTZ_AllocateEvent(handle, (void **)&dat.Event8);
  else
    ret = CAEN_DGTZ_AllocateEvent(handle, (void **)&dat.Event16);
  if (ret != CAEN_DGTZ_Success)
    Quit(ERR_MALLOC);

  std::cout << "Starting DAC calibration..." << std::endl;

  for (std::size_t p = 0; p < NPOINTS; p++) {
    // set new dco  test value to all channels
    for (std::size_t ch = 0; ch < (int32_t)dat.BoardInfo.Channels; ch++) {
      ret = CAEN_DGTZ_SetChannelDCOffset(
          handle, (uint32_t)ch,
          (uint32_t)((float)(std::fabs(dc[p] - 100)) * (655.35)));
      if (ret)
        std::cout << "Error setting ch " << ch << " test offset" << std::endl;
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    CAEN_DGTZ_ClearData(handle);

    ret = CAEN_DGTZ_SWStartAcquisition(handle);
    if (ret) {
      std::cout << "Error starting acquisition" << std::endl;
      Quit(ERR_DUMMY_LAST);
    }

    int value[NACQS][MAX_CH] = {0}; // baseline values of the NACQS
    for (std::size_t acq = 0; acq < NACQS; acq++) {
      CAEN_DGTZ_SendSWtrigger(handle);

      ret = CAEN_DGTZ_ReadData(handle, CAEN_DGTZ_SLAVE_TERMINATED_READOUT_MBLT,
                               buffer, &BufferSize);
      if (ret)
        Quit(ERR_READOUT);
      if (BufferSize == 0)
        continue;
      GetEvent(0);
      // decode the event //
      DecodeEvent();

      for (std::size_t ch = 0; ch < (int32_t)dat.BoardInfo.Channels; ch++) {
        for (std::size_t i = 1; i < 21; i++) // mean over 20 samples
        {
          if (dat.WDcfg.Nbit == 8)
            value[acq][ch] += (int)(dat.Event8->DataChannel[ch][i]);
          else
            value[acq][ch] += (int)(dat.Event16->DataChannel[ch][i]);
        }
        value[acq][ch] = (value[acq][ch] / 20);
      }
    } // for acq

    /// check for clean baselines
    for (std::size_t ch = 0; ch < (int32_t)dat.BoardInfo.Channels; ch++) {
      int max = 0, ok = 0;
      int mpp = 0;
      int size = (int)pow(2, (double)dat.BoardInfo.ADC_NBits);
      int *freq = static_cast<int *>(calloc(size, sizeof(int)));
      // find most probable value mpp
      for (std::size_t k = 0; k < NACQS; k++) {
        if (value[k][ch] > 0 && value[k][ch] < size) {
          freq[value[k][ch]]++;
          if (freq[value[k][ch]] > max) {
            max = freq[value[k][ch]];
            mpp = value[k][ch];
          }
        }
      }
      free(freq);
      // discard values too far from mpp
      for (std::size_t k = 0; k < NACQS; k++) {
        if (value[k][ch] >= (mpp - 5) && value[k][ch] <= (mpp + 5)) {
          avg_value[p][ch] = avg_value[p][ch] + (float)value[k][ch];
          ok++;
        }
      }
      // calculate final best average value
      avg_value[p][ch] = (avg_value[p][ch] / (float)ok) * 100. / (float)size;
    }
    CAEN_DGTZ_SWStopAcquisition(handle);
  } // close for p

  for (std::size_t ch = 0; ch < (int32_t)dat.BoardInfo.Channels; ch++) {
    cal[ch] =
        ((float)(avg_value[1][ch] - avg_value[0][ch]) / (float)(dc[1] - dc[0]));
    offset[ch] = (float)(dc[1] * avg_value[0][ch] - dc[0] * avg_value[1][ch]) /
                 (float)(dc[1] - dc[0]);
    std::cout << "Channel " << ch << " DAC calibration ready." << std::endl;
    // printf("Channel %d --> Cal %f   offset %f\n", ch, cal[ch], offset[ch]);
    dat.WDcfg.DAC_Calib.cal[ch] = cal[ch];
    dat.WDcfg.DAC_Calib.offset[ch] = offset[ch];
  }

  CAEN_DGTZ_ClearData(handle);

  /// free events e buffer
  FreeBuffer();
  FreeEvent();
  // reset settings
  ret |= CAEN_DGTZ_SetMaxNumEventsBLT(handle, dat.WDcfg.NumEvents);
  ret |= CAEN_DGTZ_SetPostTriggerSize(handle, dat.WDcfg.PostTrigger);
  ret |= CAEN_DGTZ_SetAcquisitionMode(handle, mem_mode);
  ret |= CAEN_DGTZ_SetExtTriggerInputMode(handle, dat.WDcfg.ExtTriggerMode);
  ret |= CAEN_DGTZ_SetChannelEnableMask(handle, dat.WDcfg.EnableMask);
  if (dat.BoardInfo.FamilyCode == CAEN_DGTZ_XX740_FAMILY_CODE ||
      dat.BoardInfo.FamilyCode == CAEN_DGTZ_XX724_FAMILY_CODE)
    ret |= CAEN_DGTZ_SetDecimationFactor(handle, dat.WDcfg.DecimationFactor);
  if (ret)
    std::cout << "Error resetting some parameters after DAC calibration"
              << std::endl;

  // reset self trigger mode settings
  if (dat.BoardInfo.FamilyCode == CAEN_DGTZ_XX730_FAMILY_CODE ||
      dat.BoardInfo.FamilyCode == CAEN_DGTZ_XX725_FAMILY_CODE) {
    // channel pair settings for x730 boards
    for (std::size_t i = 0; i < dat.WDcfg.Nch; i += 2) {
      if (dat.WDcfg.EnableMask & (0x3 << i)) {
        CAEN_DGTZ_TriggerMode_t mode = dat.WDcfg.ChannelTriggerMode[i];
        uint32_t pair_chmask = 0;
        if (dat.WDcfg.ChannelTriggerMode[i] != CAEN_DGTZ_TRGMODE_DISABLED) {
          if (dat.WDcfg.ChannelTriggerMode[i + 1] == CAEN_DGTZ_TRGMODE_DISABLED)
            pair_chmask = (0x1 << i);
          else
            pair_chmask = (0x3 << i);
        } else {
          mode = dat.WDcfg.ChannelTriggerMode[i + 1];
          pair_chmask = (0x2 << i);
        }
        pair_chmask &= dat.WDcfg.EnableMask;
        ret |= CAEN_DGTZ_SetChannelSelfTrigger(handle, mode, pair_chmask);
      }
    }
  } else {
    for (std::size_t i = 0; i < dat.WDcfg.Nch; i++) {
      if (dat.WDcfg.EnableMask & (1 << i))
        ret |= CAEN_DGTZ_SetChannelSelfTrigger(
            handle, dat.WDcfg.ChannelTriggerMode[i], (1 << i));
    }
  }
  if (ret)
    std::cout << "Error resetting self trigger mode after DAC calibration"
              << std::endl;
  Save_DAC_Calibration_To_Flash();
}

std::vector<std::string> Digitizer::ErrMsg{
    "No Error",
    "Configuration File not found",
    "Can't open the digitizer",
    "Can't read the Board Info",
    "Can't run WaveDump for this digitizer",
    "Can't program the digitizer",
    "Can't allocate the memory for the readout buffer",
    "Interrupt Error",
    "Readout Error",
    "Event Build Error",
    "Unhandled board type",
    "Over Temperature",
    "UNKNOWN"};



void Digitizer::Disconnect()
{


CAEN_DGTZ_CloseDigitizer(handle);


}

void Digitizer::FreeEvent()
{
  if (dat.Event8)
    CAEN_DGTZ_FreeEvent(handle, (void **)&dat.Event8);
  if (dat.Event16)
    CAEN_DGTZ_FreeEvent(handle, (void **)&dat.Event16);
  if (dat.Event742)
    CAEN_DGTZ_FreeEvent(handle, (void **)&dat.Event742);
}

void Digitizer::FreeBuffer()
{
	CAEN_DGTZ_FreeReadoutBuffer(&buffer);
  if (buffer != nullptr)
    delete buffer;
}



Digitizer::~Digitizer() {
  /* stop the acquisition */
  CAEN_DGTZ_SWStopAcquisition(handle);
  /* close the device and free the buffers */
  FreeEvent();
  FreeBuffer();
  if(EventPtr!=nullptr) delete EventPtr;
  CAEN_DGTZ_CloseDigitizer(handle);
}

void Digitizer::Quit(const int &error) {
  if (error != 0) {
    std::cout << ErrMsg[error] << std::endl;
    std::exit(error);
  }
}

void Digitizer::Connect() {
  if (CAEN_DGTZ_OpenDigitizer(CAEN_DGTZ_ConnectionType(dat.WDcfg.LinkType),
                              dat.WDcfg.LinkNum, dat.WDcfg.ConetNode,
                              dat.WDcfg.BaseAddress, &handle))
    Quit(ERR_DGZ_OPEN);
}

void Digitizer::GetInfos() {
  if (CAEN_DGTZ_GetInfo(handle, &dat.BoardInfo))
    Quit(ERR_BOARD_INFO_READ);
}

/*! \fn      int GetMoreBoardNumChannels(CAEN_DGTZ_BoardInfo_t BoardInfo,
 * WaveDumpConfig_t *WDcfg) \brief   calculate num of channels, num of bit and
 * sampl period according to the board type
 *
 *   \param   BoardInfo   Board Type
 *   \param   WDcfg       pointer to the config. struct
 *   \return  0 = Success; -1 = unknown board type
 */
void Digitizer::GetMoreBoardInfo() {
  int ret{0};
  switch (dat.BoardInfo.FamilyCode) {
    CAEN_DGTZ_DRS4Frequency_t freq;

  case CAEN_DGTZ_XX724_FAMILY_CODE:
  case CAEN_DGTZ_XX781_FAMILY_CODE:
  case CAEN_DGTZ_XX780_FAMILY_CODE:
    dat.WDcfg.Nbit = 14;
    dat.WDcfg.Ts = 10.0;
    break;
  case CAEN_DGTZ_XX720_FAMILY_CODE:
    dat.WDcfg.Nbit = 12;
    dat.WDcfg.Ts = 4.0;
    break;
  case CAEN_DGTZ_XX721_FAMILY_CODE:
    dat.WDcfg.Nbit = 8;
    dat.WDcfg.Ts = 2.0;
    break;
  case CAEN_DGTZ_XX731_FAMILY_CODE:
    dat.WDcfg.Nbit = 8;
    dat.WDcfg.Ts = 2.0;
    break;
  case CAEN_DGTZ_XX751_FAMILY_CODE:
    dat.WDcfg.Nbit = 10;
    dat.WDcfg.Ts = 1.0;
    break;
  case CAEN_DGTZ_XX761_FAMILY_CODE:
    dat.WDcfg.Nbit = 10;
    dat.WDcfg.Ts = 0.25;
    break;
  case CAEN_DGTZ_XX740_FAMILY_CODE:
    dat.WDcfg.Nbit = 12;
    dat.WDcfg.Ts = 16.0;
    break;
  case CAEN_DGTZ_XX725_FAMILY_CODE:
    dat.WDcfg.Nbit = 14;
    dat.WDcfg.Ts = 4.0;
    break;
  case CAEN_DGTZ_XX730_FAMILY_CODE:
    dat.WDcfg.Nbit = 14;
    dat.WDcfg.Ts = 2.0;
    break;
  case CAEN_DGTZ_XX742_FAMILY_CODE:
    dat.WDcfg.Nbit = 12;
    if ((ret = CAEN_DGTZ_GetDRS4SamplingFrequency(handle, &freq)) !=
        CAEN_DGTZ_Success)
      ret = CAEN_DGTZ_CommError;
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
      dat.WDcfg.Ts = (float)(1.0 / 750.0) * 1000.0;
      break;
    }
    switch (dat.BoardInfo.FormFactor) {
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
  default:
    ret = -1;
  }
  if (((dat.BoardInfo.FamilyCode == CAEN_DGTZ_XX751_FAMILY_CODE) ||
       (dat.BoardInfo.FamilyCode == CAEN_DGTZ_XX731_FAMILY_CODE)) &&
      dat.WDcfg.DesMode)
    dat.WDcfg.Ts /= 2;

  switch (dat.BoardInfo.FamilyCode) {
  case CAEN_DGTZ_XX724_FAMILY_CODE:
  case CAEN_DGTZ_XX781_FAMILY_CODE:
  case CAEN_DGTZ_XX780_FAMILY_CODE:
  case CAEN_DGTZ_XX720_FAMILY_CODE:
  case CAEN_DGTZ_XX721_FAMILY_CODE:
  case CAEN_DGTZ_XX751_FAMILY_CODE:
  case CAEN_DGTZ_XX761_FAMILY_CODE:
  case CAEN_DGTZ_XX731_FAMILY_CODE:
    switch (dat.BoardInfo.FormFactor) {
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
    switch (dat.BoardInfo.FormFactor) {
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
    switch (dat.BoardInfo.FormFactor) {
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
    switch (dat.BoardInfo.FormFactor) {
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
    ret = -1;
  }
  if (ret != 0)
    Quit(ERR_INVALID_BOARD_TYPE);
}

void Digitizer::GetEvent(std::size_t i) {
  int ret = CAEN_DGTZ_GetEventInfo(handle, buffer, BufferSize, i,
                                   &dat.EventInfo, &EventPtr);
  if (ret) {
    Quit(ERR_EVENT_BUILD);
  }
}

void Digitizer::DecodeEvent() {
  int ret{0};
  if (dat.WDcfg.Nbit == 8)
    ret = CAEN_DGTZ_DecodeEvent(handle, EventPtr, (void **)&dat.Event8);
  else if (dat.BoardInfo.FamilyCode != CAEN_DGTZ_XX742_FAMILY_CODE) {
    ret = CAEN_DGTZ_DecodeEvent(handle, EventPtr, (void **)&dat.Event16);
  } else {
    ret = CAEN_DGTZ_DecodeEvent(handle, EventPtr, (void **)&dat.Event742);
    if (dat.WDcfg.useCorrections != -1) { // if manual corrections
      uint32_t gr;
      for (gr = 0; gr < dat.WDcfg.MaxGroupNumber; gr++) {
        if (((dat.WDcfg.EnableMask >> gr) & 0x1) == 0)
          continue;
        ::ApplyDataCorrection(&(X742Tables[gr]), dat.WDcfg.DRS4Frequency,
                              dat.WDcfg.useCorrections,
                              &(dat.Event742->DataGroup[gr]));
      }
    }
  }
  dat.WDrun.isNewEvent=true;
  if (ret) {
    Quit(ERR_EVENT_BUILD);
  }
}

void Digitizer::InterruptTimeout() 
{
  static uint64_t CurrentTime{0};
  static uint64_t ElapsedTime{0};
  int nCycles = 0;
  static int Nb{0};
  static int Ne{0};
  /* Calculate throughput and trigger rate (every second) */
  Nb += BufferSize;
  Ne += NumEvents;
  CurrentTime = get_time();
  ElapsedTime = CurrentTime - PrevRateTime;

  nCycles++;
  if (ElapsedTime > 1000) 
  {
    if (Nb == 0)
    {
      if (dat.isTimeOut==true)
      {
        std::cout << "Timeout..." << std::endl;
        dat.isTimeOut==false;
      }
      else
      {
        m_TimeOutInfos.set(0.,0.0);
      }
    }
    else
    {
        m_TimeOutInfos.set(Nb /(ElapsedTime * 1048.576),Ne * 1000.0/ElapsedTime);
    }
    nCycles = 0;
    Nb = 0;
    Ne = 0;
    PrevRateTime = CurrentTime;
  }

 

}

/*! \fn      static long get_time()
 *   \brief   Get time in milliseconds
 *
 *   \return  time in msec
 */
long Digitizer::get_time() {
  return std::chrono::duration_cast<std::chrono::milliseconds>(
             std::chrono::system_clock::now().time_since_epoch())
      .count();
}




bool Digitizer::Interrupt() {
  int ret;
  int32_t boardId;
  int VMEHandle = -1;
  int InterruptMask = (1 << VME_INTERRUPT_LEVEL);
  bool doInterruptTimeout{false};
  BufferSize = 0;
  NumEvents = 0;
  // Interrupt handling
  if (isVMEDevice()) {
    ret = CAEN_DGTZ_VMEIRQWait((CAEN_DGTZ_ConnectionType)dat.WDcfg.LinkType,
                               dat.WDcfg.LinkNum, dat.WDcfg.ConetNode,
                               (uint8_t)InterruptMask, INTERRUPT_TIMEOUT,
                               &VMEHandle);
  } else
    ret = CAEN_DGTZ_IRQWait(handle, INTERRUPT_TIMEOUT);
  if (ret == CAEN_DGTZ_Timeout) 
  { // No active interrupt requests
    dat.isTimeOut=true;
    doInterruptTimeout=true;
    return doInterruptTimeout;
  }
  if (ret != CAEN_DGTZ_Success) {
    Quit(ERR_INTERRUPT);
  }
  // Interrupt Ack
  if (isVMEDevice()) {
    ret = CAEN_DGTZ_VMEIACKCycle(VMEHandle, VME_INTERRUPT_LEVEL, &boardId);
    if ((ret != CAEN_DGTZ_Success) || (boardId != VME_INTERRUPT_STATUS_ID)) {
     doInterruptTimeout=true;
    return doInterruptTimeout;
    } else {
      if (INTERRUPT_MODE == CAEN_DGTZ_IRQ_MODE_ROAK)
        ret = CAEN_DGTZ_RearmInterrupt(handle);
    }
  }
  return doInterruptTimeout;
}


extern int dc_file[MAX_CH];
/*! \fn      void Set_calibrated_DCO(int handle, WaveDumpConfig_t *WDcfg,
 * CAEN_DGTZ_BoardInfo_t BoardInfo) \brief   sets the calibrated DAC value using
 * calibration data (only if BASELINE_SHIFT is in use)
 *
 *   \param   handle   Digitizer handle
 *   \param   WDcfg:   Pointer to the WaveDumpConfig_t data structure
 *   \param   BoardInfo: structure with the board info
 */
int Digitizer::Set_calibrated_DCO(const int &ch) {
  int ret = CAEN_DGTZ_Success;
  if (dat.WDcfg.Version_used[ch] ==
      0) /*old DC_OFFSET config, skip calibration*/
    return ret;
  if (dat.WDcfg.PulsePolarity[ch] == CAEN_DGTZ_PulsePolarityPositive) {
    dat.WDcfg.DCoffset[ch] =
        (uint32_t)((float)(std::fabs(
                       (((float)dc_file[ch] - dat.WDcfg.DAC_Calib.offset[ch]) /
                        dat.WDcfg.DAC_Calib.cal[ch]) -
                       100.)) *
                   (655.35));
    if (dat.WDcfg.DCoffset[ch] > 65535)
      dat.WDcfg.DCoffset[ch] = 65535;
    if (dat.WDcfg.DCoffset[ch] < 0)
      dat.WDcfg.DCoffset[ch] = 0;
  } else if (dat.WDcfg.PulsePolarity[ch] == CAEN_DGTZ_PulsePolarityNegative) {
    dat.WDcfg.DCoffset[ch] =
        (uint32_t)((float)(std::fabs(((fabs(dc_file[ch] - 100.) -
                                       dat.WDcfg.DAC_Calib.offset[ch]) /
                                      dat.WDcfg.DAC_Calib.cal[ch]) -
                                     100.)) *
                   (655.35));
    if (dat.WDcfg.DCoffset[ch] < 0)
      dat.WDcfg.DCoffset[ch] = 0;
    if (dat.WDcfg.DCoffset[ch] > 65535)
      dat.WDcfg.DCoffset[ch] = 65535;
  }
  if (dat.BoardInfo.FamilyCode == CAEN_DGTZ_XX740_FAMILY_CODE) {
    ret = CAEN_DGTZ_SetGroupDCOffset(handle, (uint32_t)ch,
                                     dat.WDcfg.DCoffset[ch]);
    if (ret)
      std::cout << "Error setting group " << ch << " offset" << std::endl;
  } else {
    ret = CAEN_DGTZ_SetChannelDCOffset(handle, (uint32_t)ch,
                                       dat.WDcfg.DCoffset[ch]);
    if (ret)
      std::cout << "Error setting channel " << ch << " offset" << std::endl;
  }
  return ret;
}

void Digitizer::Load_DAC_Calibration_From_Flash() {
  FLASH_API_ERROR_CODES err = FLASH_API_SUCCESS;
  uint8_t *buffer;
  int ch = 0;
  float calibration_data[2 * MAX_SET];

  err = SPIFlash_init(handle); // init flash
  if (err != FLASH_API_SUCCESS) {
    std::cout << "Error in flash init" << std::endl;
    return;
  }

  // buffer = (uint8_t*)malloc(1 + VIRTUAL_PAGE_SIZE * sizeof(uint8_t));
  // memset(buffer, 0, 1 + VIRTUAL_PAGE_SIZE * sizeof(uint8_t));

  err = SPIFlash_read_virtual_page(handle, OFFSET_CALIBRATION_VIRTUAL_PAGE,
                                   buffer);
  if (err != FLASH_API_SUCCESS) {
    std::cout << "Error reading flash page size" << std::endl;
    return;
  }
  if (buffer[0] != 0xD) {
    std::cout << "\nNo DAC Calibration data found in board flash. Use option "
                 "'D' to calibrate DAC.\n"
              << std::endl;
    // free(buffer);
    return;
  } else {
    memcpy(calibration_data, buffer, 2 * MAX_SET * sizeof(float));
    for (std::size_t ch = 0; ch < (int)dat.BoardInfo.Channels; ch++) {
      dat.WDcfg.DAC_Calib.cal[ch] = calibration_data[2 * ch];
      dat.WDcfg.DAC_Calib.offset[ch] = calibration_data[1 + 2 * ch];
    }
  }

  // free(buffer);
  std::cout << "DAC calibration correctly loaded from board flash."
            << std::endl;
}

void Digitizer::Save_DAC_Calibration_To_Flash() {
  FLASH_API_ERROR_CODES err = FLASH_API_SUCCESS;
  uint8_t *buffer;
  int ch = 0;
  float calibration_data[2 * MAX_SET];

  for (std::size_t ch = 0; ch < (int)dat.BoardInfo.Channels; ch++) {
    calibration_data[2 * ch] = dat.WDcfg.DAC_Calib.cal[ch];
    calibration_data[1 + 2 * ch] = dat.WDcfg.DAC_Calib.offset[ch];
  }

  err = SPIFlash_init(handle); // init flash
  if (err != FLASH_API_SUCCESS) {
    std::cout << "Error in flash init." << std::endl;
    return;
  }

  buffer = (uint8_t *)malloc(1 + VIRTUAL_PAGE_SIZE * sizeof(uint8_t));
  memset(buffer, 0, 1 + VIRTUAL_PAGE_SIZE * sizeof(uint8_t));

  buffer[0] = 0xD;
  memcpy((buffer + 1), calibration_data,
         VIRTUAL_PAGE_SIZE * sizeof(uint8_t)); // copy cal vector to buffer

  err = SPIFlash_write_virtual_page(handle, OFFSET_CALIBRATION_VIRTUAL_PAGE,
                                    buffer);
  if (err != FLASH_API_SUCCESS) {
    std::cout << "Error writing flash page" << std::endl;
    return;
  }

  free(buffer);
  std::cout << "DAC calibration correctly saved on flash." << std::endl;
}
