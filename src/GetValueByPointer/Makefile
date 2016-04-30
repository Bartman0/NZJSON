###############################################################
# MODULE MAKEFILE: GetValueByPointer
# Created on: 04/29/2016
###############################################################

include header.mk


MYARGS=-I../include

###################################
T := GetValueByPointer
TARGETS += $(T)
$T_TYPES	:= udx
$T_SOURCES	:= $T.cpp 
$T_SIG		:= $T(varchar(any),varchar(any))
$T_RETURN	:= VARCHAR(1000)
$T_CLASS	:= $T
$T_CCARGS	:= --args '$(MYARGS)'
## Place for --deps --fenced -g --lastcall --mask --nondet --noparallel --nullcall --sputype --varargs --version
$T_REGARGS	:= --nullcall --version 2


###################################
#T := _sp_utl_tableExists
#TARGETS += $(T)
#$T_TYPES := sp
##Additional parameters to run SQL file
#$T_SQLFLAGS := -q

###################################
#T := mpi_ae_pingpong_play
#TARGETS += $(T)
#$(T)_TYPES := ae
#$(T)_SIG := $(T)(VARCHAR(1000))
#$(T)_RETURN := TABLE (msg VARCHAR(1000))
#$(T)_REGARGS := \
#       --unfenced

###################################
#T := NzaeComm
#TARGETS += $(T)
#$(T)_TYPES := so
#$(T)_SOURCES := $T.cpp
#$(T)_CCARGS := -Wall
#$T_INSTALL_HEADERS := $(T).h
#$(T)_REGARGS := DEPENDENCIES libnzaechild


include footer.mk