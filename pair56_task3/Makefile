PARALLAX_FOLDER=/opt/parallax
SIMPLE_LIBRARY=${HOME}/SimpleIDE/Learn/Simple\ Libraries
PORT=/dev/ttyUSB0

PROPGCC=${PARALLAX_FOLDER}/bin/propeller-elf-gcc
PROPLOAD=${PARALLAX_FOLDER}/bin/propeller-load
PROPDUMP=${PARALLAX_FOLDER}/bin/propeller-elf-objdump
LIBS=-I . -L . -I ${SIMPLE_LIBRARY}/Utility/libsimpletools -L ${SIMPLE_LIBRARY}/Utility/libsimpletools/cmm/ -I ${SIMPLE_LIBRARY}/TextDevices/libsimpletext -L ${SIMPLE_LIBRARY}/TextDevices/libsimpletext/cmm/ -I ${SIMPLE_LIBRARY}/Protocol/libsimplei2c -L ${SIMPLE_LIBRARY}/Protocol/libsimplei2c/cmm/ -I ${SIMPLE_LIBRARY}/Robotics/ActvityBot/libabdrive -L ${SIMPLE_LIBRARY}/Robotics/ActvityBot/libabdrive/cmm -L ${SIMPLE_LIBRARY}/TextDevices/libfdserial -I ${SIMPLE_LIBRARY}/TextDevices/libfdserial/cmm
CFLAGS=-mcmm -m32bit-doubles -fno-exceptions -std=c99

compileLibrobot: librobot.c
	$(PROPGCC) $(LIBS) -Os -mcmm -m32bit-doubles -fno-exceptions -std=c99 -c librobot.c -o cmm/librobot.o

loadProg: ${FILENAME}.c
	compileLibrobot
	$(PROPGCC) $(LIBS) -o cmm/${FILENAME}.elf $(CFLAGS) cmm/librobot.o ${FILENAME}.c -lm -lsimpletools -lsimpletext -lsimplei2c -labdrive -lfdserial -lm -lsimpletools -lsimpletext -lsimplei2c -labdrive -lm -lsimpletools -lsimpletext -lsimplei2c -lm -lsimpletools -lsimpletext -lm -lsimpletools -lm
	$(PROPLOAD) -s cmm/${FILENAME}.elf
	$(PROPDUMP) -h cmm/${FILENAME}.elf
	$(PROPLOAD) -Dreset=dtr -I ${PARALLAX_FOLDER}/propeller-load -b RCFAST cmm/${FILENAME}.elf -r -p ${PORT} -t

