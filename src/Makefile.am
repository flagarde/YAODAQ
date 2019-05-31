datadir=/etc/wavedump/
bin_PROGRAMS=wavedump
wavedump_SOURCES=WaveDump.c WDconfig.c WDplot.c X742CorrectionRoutines.c fft.c keyb.c flash.c spi.c
wavedump_LDADD = -lCAENComm -lCAENDigitizer -lm -lpthread
INCLUDES = -I../include
AM_CFLAGS=  -fPIC
dist_data_DATA=../Setup/WaveDumpConfig.txt ../Setup/WaveDumpConfig_X742.txt ../Setup/WaveDumpConfig_X740.txt ../ReleaseNotes
