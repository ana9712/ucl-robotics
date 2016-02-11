PARALLAX_FOLDER=/opt/parallax
SIMPLE_LIBRARY=${HOME}/SimpleIDE/Learn/Simple\ Libraries
PORT=/dev/ttyUSB0

PROPGCC=${PARALLAX_FOLDER}/bin/propeller-elf-gcc
PROPLOAD=${PARALLAX_FOLDER}/bin/propeller-load
PROPDUMP=${PARALLAX_FOLDER}/bin/propeller-elf-objdump
LIBS=-I . -L . -I ${SIMPLE_LIBRARY}/Robotics/ActvityBot/libabdrive -L ${SIMPLE_LIBRARY}/Robotics/ActvityBot/libabdrive/cmm -I ${SIMPLE_LIBRARY}/Utility/libsimpletools -L ${SIMPLE_LIBRARY}/Utility/libsimpletools/cmm/ -I ${SIMPLE_LIBRARY}/TextDevices/libsimpletext -L ${SIMPLE_LIBRARY}/TextDevices/libsimpletext/cmm/ -I ${SIMPLE_LIBRARY}/Protocol/libsimplei2c -L ${SIMPLE_LIBRARY}/Protocol/libsimplei2c/cmm/
CFLAGS=-mcmm -m32bit-doubles -fno-exceptions -std=c99

kernel: sensors.c effectors.c cogDefinitions.c synchronousTimer.c
	$(PROPGCC) $(LIBS) $(CFLAGS) -c sensors.c -o cmm/sensors.o
	$(PROPGCC) $(LIBS) $(CFLAGS) -c effectors.c -o cmm/effectors.o
	$(PROPGCC) $(LIBS) $(CFLAGS) -c cogDefinitions.c -o cmm/cogDefinitions.o
	$(PROPGCC) $(LIBS) $(CFLAGS) -c synchronousTimer.c -o cmm/synchronousTimer.o

quickstart: quickstartConfig.c testFunctions.c kernel
	$(PROPGCC) $(LIBS) $(CFLAGS) -c testFunctions.c -o cmm/testFunctions.o
	$(PROPGCC) $(LIBS) $(CFLAGS) -c quickstartConfig.c -o cmm/quickstartConfig.o
	$(PROPGCC) $(LIBS) -o cmm/quickstartControl.elf $(CFLAGS) cmm/sensors.o cmm/effectors.o cmm/cogDefinitions.o cmm/testFunctions.o cmm/quickstartConfig.o cmm/synchronousTimer.o botControl.c -lm -lsimpletools -lsimpletext -lsimplei2c -lm -lsimpletools -lsimpletext -lm -lsimpletools -lm

breadboard: breadboardConfig.c testFunctions.c kernel
	$(PROPGCC) $(LIBS) $(CFLAGS) -c testFunctions.c -o cmm/testFunctions.o
	$(PROPGCC) $(LIBS) $(CFLAGS) -c breadboardConfig.c -o cmm/breadboardConfig.o
	$(PROPGCC) $(LIBS) -o cmm/breadboardControl.elf $(CFLAGS) cmm/sensors.o cmm/effectors.o cmm/cogDefinitions.o cmm/testFunctions.o cmm/breadboardConfig.o cmm/synchronousTimer.o botControl.c -lm -lsimpletools -lsimpletext -lsimplei2c -lm -lsimpletools -lsimpletext -lm -lsimpletools -lm

loadBreadboard: breadboard
	$(PROPLOAD) -s cmm/breadboardControl.elf
	$(PROPDUMP) -h cmm/breadboardControl.elf
	$(PROPLOAD) -Dreset=dtr -I ${PARALLAX_FOLDER}/propeller-load/ -b RCFAST cmm/breadboardControl.elf -r -p ${PORT}

loadQuickstart: quickstart
	$(PROPLOAD) -s cmm/quickstartControl.elf
	$(PROPDUMP) -h cmm/quickstartControl.elf
	$(PROPLOAD) -Dreset=dtr -I ${PARALLAX_FOLDER}/propeller-load/ -b RCFAST cmm/quickstartControl.elf -r -p ${PORT}

loadThis: ${FILENAME}.c
	$(PROPGCC) $(LIBS) -o cmm/${FILENAME}.elf $(CFLAGS) ${FILENAME}.c -lm -lsimpletools -lsimpletext -lsimplei2c -lm -lsimpletools -lsimpletext -lm -lsimpletools -lm
	$(PROPLOAD) -s cmm/${FILENAME}.elf
	$(PROPDUMP) -h cmm/${FILENAME}.elf
	$(PROPLOAD) -Dreset=dtr -I ${PARALLAX_FOLDER}/propeller-load -b RCFAST cmm/${FILENAME}.elf -r -p ${PORT} -t

