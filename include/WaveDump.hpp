#ifndef _WAVEDUMP_H_
#define _WAVEDUMP_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
//#include <fstream.h>
#include <CAENDigitizer.h>
#include <CAENDigitizerType.h>

#ifdef WIN32

    #include <time.h>
    #include <sys/timeb.h>
    #include <conio.h>
    #include <process.h>

	#define getch _getch     /* redefine POSIX 'deprecated' */
	#define kbhit _kbhit     /* redefine POSIX 'deprecated' */

	#define		_PACKED_
	#define		_INLINE_		

#else
    #include <unistd.h>
    #include <stdint.h>   /* C99 compliant compilers: uint64_t */
    #include <ctype.h>    /* toupper() */
    #include <sys/time.h>

	#define		_PACKED_		__attribute__ ((packed, aligned(1)))
	#define		_INLINE_		__inline__ 

#endif

#define OUTFILENAME "wave"  /* The actual file name is wave_n.txt, where n is the channel */




#define PLOT_REFRESH_TIME 1000

#define VME_INTERRUPT_LEVEL      1
#define VME_INTERRUPT_STATUS_ID  0xAAAA
#define INTERRUPT_TIMEOUT        200  // ms
        


#define CFGRELOAD_CORRTABLES_BIT (0)
#define CFGRELOAD_DESMODE_BIT (1)

#define NPOINTS 2
#define NACQS   50

/* ###########################################################################
   Typedefs
   ###########################################################################
*/




/*
void Set_relative_Threshold(int handle, WaveDumpConfig_t *WDcfg, CAEN_DGTZ_BoardInfo_t BoardInfo);
void Calibrate_DC_Offset(int handle, WaveDumpConfig_t *WDcfg, CAEN_DGTZ_BoardInfo_t BoardInfo);
void Calibrate_XX740_DC_Offset(int handle, WaveDumpConfig_t *WDcfg, CAEN_DGTZ_BoardInfo_t BoardInfo);
int Set_calibrated_DCO(int handle, int ch, WaveDumpConfig_t *WDcfg, CAEN_DGTZ_BoardInfo_t BoardInfo);*/
#endif /* _WAVEDUMP__H */
