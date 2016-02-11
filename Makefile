PARALLAX_FOLDER=/opt/parallax
SIMPLE_LIBRARY=${HOME}/SimpleIDE/Learn/Simple\ Libraries
PORT=/dev/ttyUSB0

PROPGCC=${PARALLAX_FOLDER}/bin/propeller-elf-gcc
PROPLOAD=${PARALLAX_FOLDER}/bin/propeller-load
PROPDUMP=${PARALLAX_FOLDER}/bin/propeller-elf-objdump
LIBS=-I . -L . -I ${SIMPLE_LIBRARY}/Utility/libsimpletools -L ${SIMPLE_LIBRARY}/Utility/libsimpletools/lmm/ -I ${SIMPLE_LIBRARY}/TextDevices/libsimpletext -L ${SIMPLE_LIBRARY}/TextDevices/libsimpletext/lmm/ -I ${SIMPLE_LIBRARY}/Protocol/libsimplei2c -L ${SIMPLE_LIBRARY}/Protocol/libsimplei2c/lmm/
CFLAGS=-mlmm -m32bit-doubles -fno-exceptions -std=c99

kernel: sensors.c effectors.c cogDefinitions.c synchronousTimer.c
	$(PROPGCC) $(LIBS) $(CFLAGS) -c sensors.c -o lmm/sensors.o
	$(PROPGCC) $(LIBS) $(CFLAGS) -c effectors.c -o lmm/effectors.o
	$(PROPGCC) $(LIBS) $(CFLAGS) -c cogDefinitions.c -o lmm/cogDefinitions.o
	$(PROPGCC) $(LIBS) $(CFLAGS) -c synchronousTimer.c -o lmm/synchronousTimer.o

quickstart: quickstartConfig.c testFunctions.c kernel
	$(PROPGCC) $(LIBS) $(CFLAGS) -c testFunctions.c -o lmm/testFunctions.o
	$(PROPGCC) $(LIBS) $(CFLAGS) -c quickstartConfig.c -o lmm/quickstartConfig.o
	$(PROPGCC) $(LIBS) -o lmm/quickstartControl.elf $(CFLAGS) lmm/sensors.o lmm/effectors.o lmm/cogDefinitions.o lmm/testFunctions.o lmm/quickstartConfig.o lmm/synchronousTimer.o botControl.c -lm -lsimpletools -lsimpletext -lsimplei2c -lm -lsimpletools -lsimpletext -lm -lsimpletools -lm

breadboard: breadboardConfig.c testFunctions.c kernel
	$(PROPGCC) $(LIBS) $(CFLAGS) -c testFunctions.c -o lmm/testFunctions.o
	$(PROPGCC) $(LIBS) $(CFLAGS) -c breadboardConfig.c -o lmm/breadboardConfig.o
	$(PROPGCC) $(LIBS) -o lmm/breadboardControl.elf $(CFLAGS) lmm/sensors.o lmm/effectors.o lmm/cogDefinitions.o lmm/testFunctions.o lmm/breadboardConfig.o lmm/synchronousTimer.o botControl.c -lm -lsimpletools -lsimpletext -lsimplei2c -lm -lsimpletools -lsimpletext -lm -lsimpletools -lm

loadBreadboard: breadboard
	$(PROPLOAD) -s lmm/breadboardControl.elf
	$(PROPDUMP) -h lmm/breadboardControl.elf
	$(PROPLOAD) -Dreset=dtr -I ${PARALLAX_FOLDER}/propeller-load/ -b RCFAST lmm/breadboardControl.elf -r -p ${PORT}

loadQuickstart: quickstart
	$(PROPLOAD) -s lmm/quickstartControl.elf
	$(PROPDUMP) -h lmm/quickstartControl.elf
	$(PROPLOAD) -Dreset=dtr -I ${PARALLAX_FOLDER}/propeller-load/ -b RCFAST lmm/quickstartControl.elf -r -p ${PORT}

loadThis:
	$(PROPGCC) $(LIBS) $(CFLAGS) librobot.c ${FILENAME}.c
	$(PROPLOAD) -s lmm/${FILENAME}.elf
	$(PROPDUMP) -h lmm/${FILENAME}.elf
	$(PROPLOAD) -Dreset=dtr -I ${PARALLAX_FOLDER}/propeller-load -b RCFAST lmm/${FILENAME}.elf -r -p ${PORT}

