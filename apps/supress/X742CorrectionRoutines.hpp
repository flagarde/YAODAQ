#pragma once

#include "CAENDigitizerType.h"

/*! \brief   Corrects 'data' depending on the informations contained in 'CTable'
 *
 *   \param   CTable              :  Pointer to the Table containing the Data
 * Corrections \param   frequency           :  The operational Frequency of the
 * board \param   CorrectionLevelMask :  Mask of Corrections to be applied
 *   \param   data                :  Data to be corrected
 */
void ApplyDataCorrection(CAEN_DGTZ_DRS4Correction_t* CTable, CAEN_DGTZ_DRS4Frequency_t frequency, int CorrectionLevelMask, CAEN_DGTZ_X742_GROUP_t* data);

/*! \brief   Write the correction table of a x742 boards into the output files
 *
 *   \param   Filename of output file
 *   \param   Group Mask of Tables to be saved
 *   \param   Pointer to the DataCorrection group tables
 */
int SaveCorrectionTables(const char* outputFileName, uint32_t groupMask, CAEN_DGTZ_DRS4Correction_t* tables);

/*! \brief   Reads the correction table of a x742 boards from txt files
 *
 *   \param   Base Filename of input file. Actual filenames loaded will be:
 *               a) baseInputFileName + "_cell.txt"
 *               b) baseInputFileName + "_nsample.txt"
 *               c) baseInputFileName + "_time.txt"
 *   \param   DataCorrection table to be filled
 */
int LoadCorrectionTable(char* baseInputFileName, CAEN_DGTZ_DRS4Correction_t* tb);
