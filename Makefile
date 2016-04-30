###############################################################
# PROJECT MAIN MAKEFILE
# Created on: 04/29/2016
###############################################################

-include Makefile.vars

###############################################################
# Project variables (exported) 
# there are special variables, which are common for all MODULES
###############################################################
export PROJECT_DIR := $(shell pwd)
export BUILD_TYPE ?= release
export LOCAL_BUILD ?= 1
export BUILD_PREFIX ?= products
export BUILD_DIR ?= $(PROJECT_DIR)/build
export CARTRIDGES_DIR ?= $(BUILD_DIR)/cartridges
export DEV_TOOLS_DIR ?= /nz/export/ae/tools/development_tools/current
export NZDOC_DIR ?= /nz/export/ae/tools/nzdoc/current
export MKS_DIR ?= $(DEV_TOOLS_DIR)/mks
export NZ_PROJECT_RULES_MK ?= $(MKS_DIR)/nz_project_rules.mk

###############################################################
# Checking dependencies
###############################################################

ifeq ($(shell [ ${LOCAL_BUILD} = 1 -a ! -d ${DEV_TOOLS_DIR} ] ; echo $$?), 0)
  $(error Please install development_tools cartidge)
else
ifeq ($(shell [ ${LOCAL_BUILD} = 0 -a ! -d ${DEV_TOOLS_DIR} ] ; echo $$?), 0)
  $(error Please build development_tools from src)
endif
endif

ifndef CARTRIDGE
ifeq ($(shell [ ${LOCAL_BUILD} = 1 -a ! -d ${NZDOC_DIR} ] ; echo $$?), 0)
  $(error Please install nzdoc cartidge)
else
ifeq ($(shell [ ${LOCAL_BUILD} = 0 -a ! -d ${NZDOC_DIR} ] ; echo $$?), 0)
  $(error Please build nzdoc from src)
endif
endif
endif

###############################################################
# Custom user project variables.
# They are common for all MODULES
###############################################################


###############################################################
# MODULES
# Define here all modules in project 
# (Can be a relative path such as "module/sub-module")
###############################################################

MODULES = GetValueByPointer

include $(NZ_PROJECT_RULES_MK)