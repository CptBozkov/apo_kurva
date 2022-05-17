CC = arm-linux-gnueabihf-gcc

CFLAGS =-g -std=gnu99 -O1 -Wall
LDFLAGS = -lrt -lpthread -lm

SOURCES = main.c
SOURCES += player.c hardware.c
# threads
SOURCES += threads/draw.c threads/gameLoop.c threads/keyboard.c threads/pause.c
# utils
SOURCES += utils/buffer.c utils/font.c utils/colors.c utils/draw_objects.c
#libs
SOURCES += lib/mzapo_phys.c lib/mzapo_parlcd.c lib/font_rom8x16.c

TARGET_EXE = main
TARGET_IP = 192.168.223.212
TARGET_DIR = apo_kurva

OBJECTS += $(filter %.o,$(SOURCES:%.c=%.o))

LINKER = $(CC)
LDFLAGS += $(CFLAGS) $(CPPFLAGS)

lib/%.o:%.c
	$(CC) $(CFLAGS) -c $<

all: $(TARGET_EXE)

$(TARGET_EXE): $(OBJECTS)
	$(LINKER) $(LDFLAGS) -L. $^ -o $@ $(LDLIBS)

clean:
	rm -f *.o *.a $(OBJECTS) $(TARGET_EXE)

send:
	ssh root@$(TARGET_IP) "rm -rf $(TARGET_DIR)"
	exit
	scp -r ../$(TARGET_DIR) root@$(TARGET_IP):~
	ssh root@$(TARGET_IP) "cd $(TARGET_DIR) && make clean && make"
	exit
	ssh root@$(TARGET_IP) "(killall $(TARGET_EXE) || true) && cd $(TARGET_DIR) && ./$(TARGET_EXE)"