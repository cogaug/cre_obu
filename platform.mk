.SUFFIXES : .c .cpp .o

.cpp.o:
	$(CCC) $(CCFLAGS) -c $(IPATHS) $<

.c.o:
	$(CC) $(CFLAGS) -c $(IPATHS) $<

.y.c:

# Platform specific definitions for Linux/GNU.

# The following variables are platform specific and may need
# to be edited if moving to a different system.

# compiler defs
#CC		  = gcc
#CCC	  = g++
LINKOPT_ = -o $@ $(LDFLAGS)
LINK = $(CC)

# File extensions
OBJ     = .o
SO      = .so

# Run-time library
LIBPFX  = lib
LIBEXT  = a
LPPFX   = -L
LLPFX   = -l
LLEXT   =
LLAEXT  =

A       = .$(LIBEXT)
MTA     = $(A)
MDA     = $(A)
IMP     = $(SO)
IMPEXT  = $(IMP)
IMPLINK =
DLL     = $(SO)
RTDIRSFX =

# Include and library paths
PS      = /
FS      = :
IPATHS_ =

# O/S commands
COPY     = cp -f
MOVE     = mv -f
MV       = $(MOVE)
RM       = rm -f
STRIP    = strip
MAKE     = make
RMDIR    = rm -rf
MKDIR    = mkdir -p

LLSYS = -Wl,-Bdynamic -lm -lpthread -ldl

# START EXT_LIB
# Link libraries
LLSTACK = -lwpmapi
LLSDK = -llte_v2x
LLAEROLINK = -lviicsec -laerolinkPKI -lmisbehaviorReport -lcrypto
LLASN1 = -lasn1oer -lasn1per -lasn1rt

# library file names
STACKLIBNAME = libwpmapi.a
SDKLIBNAME = liblte_v2x.so

# Run-time library sets used in library link
EXTLIBS = $(LLSDK) $(LLASN1) $(LLSTACK) $(LLAEROLINK)

# END EXT_LIB

