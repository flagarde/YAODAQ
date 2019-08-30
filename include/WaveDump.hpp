#ifndef _WAVEDUMP_H_
#define _WAVEDUMP_H_

#define OUTFILENAME "wave"  /* The actual file name is wave_n.txt, where n is the channel */




#define PLOT_REFRESH_TIME 1000

#define VME_INTERRUPT_LEVEL      1
#define VME_INTERRUPT_STATUS_ID  0xAAAA
#define INTERRUPT_TIMEOUT        200  // ms
        


#define CFGRELOAD_CORRTABLES_BIT (0)
#define CFGRELOAD_DESMODE_BIT (1)

#define NPOINTS 2
#define NACQS   50
#endif /* _WAVEDUMP__H */
