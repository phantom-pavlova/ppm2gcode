# standard Makefile: 
# SOURCES.c output from 'ls *.c'
# TARGET is pwd (the name of the directory you are working in)
# 'mkdir obj' -- not automatic, make new will create it
#
# the idea is to have a home directory called C,
#  within that each dir is the program name 
#  this makefile will install the basics and compile
#  'make help' gives the options (you can add some 
#   - it is the comment for each section)
#  
#  so copy this Makefile to ~/C/myprog
#  cd to ~/C/myprog
#  'make new'
#  edit main.c, put gobals in globals.h, prototypes in prototypes.h
#  all .c files will be seached for your prototypes, but do a 'make deps'
#  from time to time.
#
#  otherwise edit, make, run, think, start again .....
#
#
#
#
#
#
#
#
# check cflags and includes 
#  
# bugs:
#
# needs gnu make, which has logically consistant 
#  but otherwise incomprehensible documentation.
#
# cannot (quite) set itself up automatically
#  -mkdir obj .... make new
#
# deps, clean, ... should be declaired phoney ?? 
#
# check libraries exist otherwise warn
#
# for 32/64 bit system, you can check and pass a define to the source
# untested
#.............................................
#LBITS := $(shell getconf LONG_BIT)
#ifeq ($(LBITS),64)
#CFLAGS += -DSYS_IS_64_bit
#else
#CFLAGS += -DSYS_IS_32_bit
#endif
#.............................................
#
CFLAGS:=-g -pg -Wall -Wno-unused-variable -Wno-unused-but-set-variable
#
#
# if using xforms add -lforms thus:
#LDLIBS:=-lm -L/usr/X11R6/lib -lforms -lX11
# otherwise leave it out
LDLIBS:=-lm -L/usr/X11R6/lib -lX11 -lreadline
#
#
OBJDIR:=obj
#TOKEN:="deps start"
#TOKEN:=$(subst start,begin,$(TOKEN))
TOKEN:="deps begin"
#this file is now cut from LAST occurance 
SOURCES.c=$(shell ls *.c)


TARGET=$(shell basename $(CURDIR))

objects = $(addprefix $(OBJDIR)/, $(SOURCES.c:.c=.o))



all:	# 'make' on its own will compile anything that has changed scince the last make
all:	$(objects)
	$(CC) $(CFLAGS) -o $(TARGET) $(objects) $(LDLIBS)

$(OBJDIR)/%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

help: # Print help on Makefile - thanks to someone who is ungoogleable because of regex recursion (see https://xkcd.com/1313/)
	# for help to work, comment after each target/recipe 
	@grep '^[^.#]\+:\s\+.*#' Makefile | \
	sed "s/\(.\+\):\s*\(.*\) #\s*\(.*\)/`printf "\033[93m"`\1`printf "\033[0m"`	\3 [\2]/" | \
	expand -t20

# doesn't work!
#$(OBJDIR)/: # creates objdir if necessary 
#	@mkdir -p $@

deps: # redo dependencies 
	$(SOURCES)
	grep -n $(TOKEN) Makefile | cut -d: -f1 | tail -n1 | xargs -iRR head -n RR Makefile > Makefile.t
	@cc -MM $(SOURCES.c) | xargs -iRR echo xnewlinexRR | tr -d '\n' | sed 's|xnewlinex|\nobj/|g' >> Makefile.t
	@echo >> Makefile.t
	mv Makefile.t Makefile

clean: # for new target machine
	rm -f $(TARGET) *.o $(objects)

install: CFLAGS= -O2 -Wall
install: clean
install: all
	@echo now cp $(TARGET) to ~/bin
# cp to ~/bin ??


static: # static build for distis without xforms libraries (you may need to install .o files in obj/ ie cp /usr/local/xforms-1.2.4/lib/* obj/)
	CFLAGS= -Wl,-Bstatic -lforms -lXpm -Wl,-Bdynamic -lpthread -lm -lX11
static: clean
static: all

new: # mkdir ~/C/prog-name && cd ~/C/prog-name && cp ~/C/Makefile . && make new - then ready to go, shouldn't overwite anything
new:	copy
new:	deps
copy:
	# needs these
	@echo '/* '>> TREADME.txt 
	@echo $(TARGET) >> TREADME.txt 	
	@echo '*/ '>> TREADME.txt 
	cat ../README.skel.txt >> TREADME.txt
	cp -n TREADME.txt README.txt
	@rm TREADME.txt 
	cp -n ~/C/main.skel.c ./main.c
	cp -n ~/C/ourdefs.h .
	cp -n ~/C/Makefile .
	cp -n ~/C/prototypes.skel.h ./prototypes.h
	cp -n ~/C/globals.skel.h ./globals.h
	cp -n ~/C/README.skel.txt ./README.txt
	mkdir -p obj
	@echo
	@echo "deps ?? making anyway."
	@echo

test: #for playing with the Makefile
	@echo $(TOKEN) 




#deps begin

obj/main.o: main.c ourdefs.h prototypes.h globals.h
