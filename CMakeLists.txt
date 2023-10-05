cmake_minimum_required(VERSION 3.24)
project(deca_ranging)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_C_STANDARD 17)

include_directories(application)
include_directories(compiler)
include_directories(decadriver)
include_directories(platform)

add_executable(deca_ranging
        application/deca_util.c
        application/deca_util.h
        application/instance.c
        application/instance.h
        application/instance_calib.c
        application/instance_common.c
        application/instance_log.c
        application/lib.h
        application/resource.h
        application/targetver.h
        compiler/compiler.h
        decadriver/deca_device.c
        decadriver/deca_device_api.h
        decadriver/deca_param_types.h
        decadriver/deca_params_init.c
        decadriver/deca_range_tables.c
        decadriver/deca_regs.h
        decadriver/deca_types.h
        decadriver/deca_version.h
        platform/cheetah.c
        platform/cheetah.h
        platform/comport.c
        platform/comport.h
        platform/deca_mutex.c
        platform/deca_spi.c
        platform/deca_spi.h
        platform/deca_vcspi.c
        platform/deca_vcspi.h)
