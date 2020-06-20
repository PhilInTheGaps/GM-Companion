# (c) 2014 Copyright ownCloud GmbH
# Redistribution and use is allowed according to the terms of the BSD license:
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
#
# 1. Redistributions of source code must retain the copyright
#    notice, this list of conditions and the following disclaimer.
# 2. Redistributions in binary form must reproduce the copyright
#    notice, this list of conditions and the following disclaimer in the
#    documentation and/or other materials provided with the distribution.
# 3. The name of the author may not be used to endorse or promote products 
#    derived from this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
# IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
# OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
# IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
# INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
# NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
# DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
# THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
# THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

# - Try to find QtKeychain
# Once done this will define
#  QTKEYCHAIN_FOUND - System has QtKeychain
#  QTKEYCHAIN_INCLUDE_DIRS - The QtKeychain include directories
#  QTKEYCHAIN_LIBRARIES - The libraries needed to use QtKeychain
#  QTKEYCHAIN_DEFINITIONS - Compiler switches required for using LibXml2

# CHANGES:
# Was modified to find qtkeychain even if it is called qt5keychain.
# Also, build the qtkeychain if no system lib can be found.

if (Qt5Core_DIR)
  find_path(QTKEYCHAIN_INCLUDE_DIR
    NAMES
      keychain.h
    PATH_SUFFIXES
      qt5keychain
  )

  find_library(QTKEYCHAIN_LIBRARY
    NAMES
      qt5keychain
      lib5qtkeychain
    PATHS
      /usr/lib
      /usr/lib/${CMAKE_ARCH_TRIPLET}
      /usr/local/lib
      /opt/local/lib
      ${CMAKE_LIBRARY_PATH}
      ${CMAKE_INSTALL_PREFIX}/lib
  )

else()
  find_path(QTKEYCHAIN_INCLUDE_DIR
    NAMES
      keychain.h
    PATH_SUFFIXES
      qtkeychain
  )

  find_library(QTKEYCHAIN_LIBRARY
    NAMES
      qtkeychain
      libqtkeychain
    PATHS
      /usr/lib
      /usr/lib/${CMAKE_ARCH_TRIPLET}
      /usr/local/lib
      /opt/local/lib
      ${CMAKE_LIBRARY_PATH}
      ${CMAKE_INSTALL_PREFIX}/lib
  )
  
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(QtKeychain DEFAULT_MSG QTKEYCHAIN_LIBRARY QTKEYCHAIN_INCLUDE_DIR)

mark_as_advanced(QTKEYCHAIN_INCLUDE_DIR QTKEYCHAIN_LIBRARY)

if (NOT QTKEYCHAIN_LIBRARY)
  message(FATAL_ERROR "Could NOT find QTKEYCHAIN")
else()
  message(STATUS "Found QTKEYCHAIN: ${QTKEYCHAIN_LIBRARY}")
endif()