# Copyright (c) Microsoft Corporation.
# Licensed under the MIT License.

set(MCPU_FLAGS "-mthumb -mcpu=cortex-m0plus")
set(VFP_FLAGS "-mfloat-abi=soft")
set(SPEC_FLAGS "--specs=nano.specs")
# set(LD_FLAGS "-nostartfiles")

include(${CMAKE_CURRENT_LIST_DIR}/arm-gcc-cortex-toolchain.cmake)
