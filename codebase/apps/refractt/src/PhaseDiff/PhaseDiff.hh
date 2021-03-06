// *=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=* 
// ** Copyright UCAR (c) 1990 - 2016                                         
// ** University Corporation for Atmospheric Research (UCAR)                 
// ** National Center for Atmospheric Research (NCAR)                        
// ** Boulder, Colorado, USA                                                 
// ** BSD licence applies - redistribution and use in source and binary      
// ** forms, with or without modification, are permitted provided that       
// ** the following conditions are met:                                      
// ** 1) If the software is modified to produce derivative works,            
// ** such modified software should be clearly marked, so as not             
// ** to confuse it with the version available from UCAR.                    
// ** 2) Redistributions of source code must retain the above copyright      
// ** notice, this list of conditions and the following disclaimer.          
// ** 3) Redistributions in binary form must reproduce the above copyright   
// ** notice, this list of conditions and the following disclaimer in the    
// ** documentation and/or other materials provided with the distribution.   
// ** 4) Neither the name of UCAR nor the names of its contributors,         
// ** if any, may be used to endorse or promote products derived from        
// ** this software without specific prior written permission.               
// ** DISCLAIMER: THIS SOFTWARE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS  
// ** OR IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED      
// ** WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.    
// *=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=* 
/**
 *
 * @file PhaseDiff.hh
 *
 * @class PhaseDiff
 *
 * PhaseDiff is the top level application class.
 *  
 * @date 2/18/2010
 *
 */

#ifndef PhaseDiff_HH
#define PhaseDiff_HH

#include <string>
#include <sys/time.h>
#include <vector>

#include <dsdata/DsTrigger.hh>
#include <Mdv/DsMdvx.hh>
#include <Mdv/MdvxPjg.hh>
#include <toolsa/DateTime.hh>

#include "Args.hh"
#include "Params.hh"

using namespace std;


/** 
 * @class PhaseDiff
 */

class PhaseDiff
{
 public:

  ////////////////////
  // Public members //
  ////////////////////

  /**
   * @brief Flag indicating whether the program status is currently okay.
   */

  bool okay;


  ////////////////////
  // Public methods //
  ////////////////////

  //////////////////////////////
  // Constructors/Destructors //
  //////////////////////////////

  /**
   * @brief Destructor
   */

  ~PhaseDiff(void);
  

  /**
   * @brief Retrieve the singleton instance of this class.
   *
   * @param[in] argc Number of command line arguments.
   * @param[in] argv Command line arguments.
   */

  static PhaseDiff *Inst(int argc, char **argv);


  /**
   * @brief Retrieve the singleton instance of this class.
   */

  static PhaseDiff *Inst();
  

  /**
   * @brief Initialize the local data.
   *
   * @return Returns true if the initialization was successful, false
   *         otherwise.
   */

  bool init();
  

  /////////////////////
  // Running methods //
  /////////////////////

  /**
   * @brief Run the program.
   */

  void run();
  

 private:

  ///////////////////////
  // Private constants //
  ///////////////////////

  /**
   * @brief The value to use to identify missing data in generated fields.
   */

  static const fl32 MISSING_DATA_VALUE;

  /**
   * @brief
   */

  static const double SNR_NOISE_MAX;
  
  /********************* UPDATE *******************************
   * Should this be float instead?
   */

  /**
   * @brief Very large data value.
   */

  static const int VERY_LARGE;

  /**
   * @brief 
   */

  static const double OFFSET_ABOVE_AVERAGE;

  /**
   * @brief
   */

  static const double DM_NOISE;
  

  /////////////////////
  // Private members //
  /////////////////////

  /**
   * @brief Singleton instance pointer
   */

  static PhaseDiff *_instance;
  
  /**
   * @brief Program name.
   */

  char *_progName;

  /**
   * @brief Command line arguments.
   */

  Args *_args;

  /**
   * @brief Parameter file parameters.
   */

  Params *_params;
  
  /**
   * @brief Triggering object
   */

  DsTrigger *_dataTrigger;
  


  /////////////////////
  // Private methods //
  /////////////////////

  void _createPhaseColorscale() const;
  void _createNiqColorscale() const;
  
  /**
   * @brief Constructor
   *
   * @param[in] argc Number of command line arguments.
   * @param[in] argv Command line arguments.
   *
   * @note The constructor is private because this is a singleton object.
   */

  PhaseDiff(int argc, char **argv);
  

  /**
   * @brief Calculate the I/Q fields from the NIQ/AIQ fields given in the
   *        input file.  Replace the NIQ/AIQ fields with the calculated I/Q
   *        fields so the file will look the same as it would if the I/Q
   *        fields were read from disk.
   *
   * @param[in,out] niq_field The input NIQ field which is replaced with
   *                          the I field on return.
   * @param[in,out] aiq_field The input AIQ field which is replaced with
   *                          the Q field on return.
   * @param[in] snr_field The input SNR field.
   */

  void _calcIQ(MdvxField &niq_field,
	       MdvxField &aiq_field,
	       const MdvxField &snr_field) const;
  

  /**
   * @brief Calculate the phase difference fields.  Add these fields to
   *        mdvx2 for output.
   *
   * @param[in] mdvx1 The first MDV file.  This one occurs earlier in time.
   * @param[in,out] mdvx2 The second MDV file.  This one occurs later in time.
   *
   * @return Returns true on success, false on failure.
   */

  bool _calcPhaseDiff(const DsMdvx &mdvx1,
		      DsMdvx &mdvx2) const;
  

  /**
   * @brief Calculate the SNR field from the power field given in the input
   *        file.  Replace the power filed with the calculated SNR field so
   *        the file will look the same as it would if the SNR field were
   *        read from disk.
   *
   * @param[in,out] power_field The input power field which is replaced with
   *                            the SNR field on return.
   */

  void _calcSnr(MdvxField &power_field) const;
  

  /**
   * @brief Create a blank MDV field with the given field name.
   *
   * @param[in] proj The data projection.
   * @param[in] field_name The field name.
   * @param[in] elevation The elevation angle of the tilt we are processing.
   *
   * @return Returns a pointer to the new field on success, 0 on failure.
   *         The calling method takes ownership of the pointer and must
   *         delete it when no longer needed.
   */

  MdvxField *_createBlankField(const MdvxPjg &proj,
			       const string &field_name,
			       const fl32 elevation) const;
  

  /**
   * @brief Initialize the data trigger.
   *
   * Returns true on success, false on failure.
   */

  bool _initTrigger(void);
  

  /**
   * @brief Process data for the given trigger time.
   *
   * @param trigger_time The current trigger time.  Data for this time
   *                     should be processed.
   *
   * Returns true on success, false on failure.
   */

  bool _processData(const DateTime &trigger_time);
  

  /**
   * @brief Read the specified input file.  The returned file will have the
   *        raw I field as the first field and the raw Q field as the second.
   *
   * @param[out] mdvx The input file.
   * @param[in] data_time Data time to read.
   * @param[in] search_margin Data time search margin in seconds.
   *
   * @return Returns true on success, false on failure.
   */

  bool _readInputFile(DsMdvx &mdvx,
		      const DateTime &data_time,
		      const int search_margin) const;
  

};


#endif
