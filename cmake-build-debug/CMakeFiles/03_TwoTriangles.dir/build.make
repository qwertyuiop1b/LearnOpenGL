# CMAKE generated file: DO NOT EDIT!
# Generated by "NMake Makefiles" Generator, CMake Version 3.25

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE
NULL=nul
!ENDIF
SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = "D:\Program Files\JetBrains\CLion 2023.1.2\bin\cmake\win\x64\bin\cmake.exe"

# The command to remove a file.
RM = "D:\Program Files\JetBrains\CLion 2023.1.2\bin\cmake\win\x64\bin\cmake.exe" -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = E:\CodeProjects\LearnOpenGL

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = E:\CodeProjects\LearnOpenGL\cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles\03_TwoTriangles.dir\depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles\03_TwoTriangles.dir\compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles\03_TwoTriangles.dir\progress.make

# Include the compile flags for this target's objects.
include CMakeFiles\03_TwoTriangles.dir\flags.make

CMakeFiles\03_TwoTriangles.dir\src\01_Start\03_TwoTriangles.cpp.obj: CMakeFiles\03_TwoTriangles.dir\flags.make
CMakeFiles\03_TwoTriangles.dir\src\01_Start\03_TwoTriangles.cpp.obj: E:\CodeProjects\LearnOpenGL\src\01_Start\03_TwoTriangles.cpp
CMakeFiles\03_TwoTriangles.dir\src\01_Start\03_TwoTriangles.cpp.obj: CMakeFiles\03_TwoTriangles.dir\compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=E:\CodeProjects\LearnOpenGL\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/03_TwoTriangles.dir/src/01_Start/03_TwoTriangles.cpp.obj"
	$(CMAKE_COMMAND) -E cmake_cl_compile_depends --dep-file=CMakeFiles\03_TwoTriangles.dir\src\01_Start\03_TwoTriangles.cpp.obj.d --working-dir=E:\CodeProjects\LearnOpenGL\cmake-build-debug --filter-prefix="ע��: �����ļ�:  " -- D:\PROGRA~2\MICROS~1\2022\COMMUN~1\VC\Tools\MSVC\1433~1.316\bin\Hostx64\x64\cl.exe @<<
 /nologo /TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) /showIncludes /FoCMakeFiles\03_TwoTriangles.dir\src\01_Start\03_TwoTriangles.cpp.obj /FdCMakeFiles\03_TwoTriangles.dir\ /FS -c E:\CodeProjects\LearnOpenGL\src\01_Start\03_TwoTriangles.cpp
<<

CMakeFiles\03_TwoTriangles.dir\src\01_Start\03_TwoTriangles.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/03_TwoTriangles.dir/src/01_Start/03_TwoTriangles.cpp.i"
	D:\PROGRA~2\MICROS~1\2022\COMMUN~1\VC\Tools\MSVC\1433~1.316\bin\Hostx64\x64\cl.exe > CMakeFiles\03_TwoTriangles.dir\src\01_Start\03_TwoTriangles.cpp.i @<<
 /nologo /TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E E:\CodeProjects\LearnOpenGL\src\01_Start\03_TwoTriangles.cpp
<<

CMakeFiles\03_TwoTriangles.dir\src\01_Start\03_TwoTriangles.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/03_TwoTriangles.dir/src/01_Start/03_TwoTriangles.cpp.s"
	D:\PROGRA~2\MICROS~1\2022\COMMUN~1\VC\Tools\MSVC\1433~1.316\bin\Hostx64\x64\cl.exe @<<
 /nologo /TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) /FoNUL /FAs /FaCMakeFiles\03_TwoTriangles.dir\src\01_Start\03_TwoTriangles.cpp.s /c E:\CodeProjects\LearnOpenGL\src\01_Start\03_TwoTriangles.cpp
<<

CMakeFiles\03_TwoTriangles.dir\src\glad.c.obj: CMakeFiles\03_TwoTriangles.dir\flags.make
CMakeFiles\03_TwoTriangles.dir\src\glad.c.obj: E:\CodeProjects\LearnOpenGL\src\glad.c
CMakeFiles\03_TwoTriangles.dir\src\glad.c.obj: CMakeFiles\03_TwoTriangles.dir\compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=E:\CodeProjects\LearnOpenGL\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/03_TwoTriangles.dir/src/glad.c.obj"
	$(CMAKE_COMMAND) -E cmake_cl_compile_depends --dep-file=CMakeFiles\03_TwoTriangles.dir\src\glad.c.obj.d --working-dir=E:\CodeProjects\LearnOpenGL\cmake-build-debug --filter-prefix="ע��: �����ļ�:  " -- D:\PROGRA~2\MICROS~1\2022\COMMUN~1\VC\Tools\MSVC\1433~1.316\bin\Hostx64\x64\cl.exe @<<
 /nologo $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) /showIncludes /FoCMakeFiles\03_TwoTriangles.dir\src\glad.c.obj /FdCMakeFiles\03_TwoTriangles.dir\ /FS -c E:\CodeProjects\LearnOpenGL\src\glad.c
<<

CMakeFiles\03_TwoTriangles.dir\src\glad.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/03_TwoTriangles.dir/src/glad.c.i"
	D:\PROGRA~2\MICROS~1\2022\COMMUN~1\VC\Tools\MSVC\1433~1.316\bin\Hostx64\x64\cl.exe > CMakeFiles\03_TwoTriangles.dir\src\glad.c.i @<<
 /nologo $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E E:\CodeProjects\LearnOpenGL\src\glad.c
<<

CMakeFiles\03_TwoTriangles.dir\src\glad.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/03_TwoTriangles.dir/src/glad.c.s"
	D:\PROGRA~2\MICROS~1\2022\COMMUN~1\VC\Tools\MSVC\1433~1.316\bin\Hostx64\x64\cl.exe @<<
 /nologo $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) /FoNUL /FAs /FaCMakeFiles\03_TwoTriangles.dir\src\glad.c.s /c E:\CodeProjects\LearnOpenGL\src\glad.c
<<

CMakeFiles\03_TwoTriangles.dir\src\utils\Shader.cpp.obj: CMakeFiles\03_TwoTriangles.dir\flags.make
CMakeFiles\03_TwoTriangles.dir\src\utils\Shader.cpp.obj: E:\CodeProjects\LearnOpenGL\src\utils\Shader.cpp
CMakeFiles\03_TwoTriangles.dir\src\utils\Shader.cpp.obj: CMakeFiles\03_TwoTriangles.dir\compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=E:\CodeProjects\LearnOpenGL\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/03_TwoTriangles.dir/src/utils/Shader.cpp.obj"
	$(CMAKE_COMMAND) -E cmake_cl_compile_depends --dep-file=CMakeFiles\03_TwoTriangles.dir\src\utils\Shader.cpp.obj.d --working-dir=E:\CodeProjects\LearnOpenGL\cmake-build-debug --filter-prefix="ע��: �����ļ�:  " -- D:\PROGRA~2\MICROS~1\2022\COMMUN~1\VC\Tools\MSVC\1433~1.316\bin\Hostx64\x64\cl.exe @<<
 /nologo /TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) /showIncludes /FoCMakeFiles\03_TwoTriangles.dir\src\utils\Shader.cpp.obj /FdCMakeFiles\03_TwoTriangles.dir\ /FS -c E:\CodeProjects\LearnOpenGL\src\utils\Shader.cpp
<<

CMakeFiles\03_TwoTriangles.dir\src\utils\Shader.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/03_TwoTriangles.dir/src/utils/Shader.cpp.i"
	D:\PROGRA~2\MICROS~1\2022\COMMUN~1\VC\Tools\MSVC\1433~1.316\bin\Hostx64\x64\cl.exe > CMakeFiles\03_TwoTriangles.dir\src\utils\Shader.cpp.i @<<
 /nologo /TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E E:\CodeProjects\LearnOpenGL\src\utils\Shader.cpp
<<

CMakeFiles\03_TwoTriangles.dir\src\utils\Shader.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/03_TwoTriangles.dir/src/utils/Shader.cpp.s"
	D:\PROGRA~2\MICROS~1\2022\COMMUN~1\VC\Tools\MSVC\1433~1.316\bin\Hostx64\x64\cl.exe @<<
 /nologo /TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) /FoNUL /FAs /FaCMakeFiles\03_TwoTriangles.dir\src\utils\Shader.cpp.s /c E:\CodeProjects\LearnOpenGL\src\utils\Shader.cpp
<<

# Object files for target 03_TwoTriangles
03_TwoTriangles_OBJECTS = \
"CMakeFiles\03_TwoTriangles.dir\src\01_Start\03_TwoTriangles.cpp.obj" \
"CMakeFiles\03_TwoTriangles.dir\src\glad.c.obj" \
"CMakeFiles\03_TwoTriangles.dir\src\utils\Shader.cpp.obj"

# External object files for target 03_TwoTriangles
03_TwoTriangles_EXTERNAL_OBJECTS =

03_TwoTriangles.exe: CMakeFiles\03_TwoTriangles.dir\src\01_Start\03_TwoTriangles.cpp.obj
03_TwoTriangles.exe: CMakeFiles\03_TwoTriangles.dir\src\glad.c.obj
03_TwoTriangles.exe: CMakeFiles\03_TwoTriangles.dir\src\utils\Shader.cpp.obj
03_TwoTriangles.exe: CMakeFiles\03_TwoTriangles.dir\build.make
03_TwoTriangles.exe: CMakeFiles\03_TwoTriangles.dir\objects1
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=E:\CodeProjects\LearnOpenGL\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking CXX executable 03_TwoTriangles.exe"
	"D:\Program Files\JetBrains\CLion 2023.1.2\bin\cmake\win\x64\bin\cmake.exe" -E vs_link_exe --intdir=CMakeFiles\03_TwoTriangles.dir --rc=C:\PROGRA~2\WI3CF2~1\10\bin\100190~1.0\x64\rc.exe --mt=C:\PROGRA~2\WI3CF2~1\10\bin\100190~1.0\x64\mt.exe --manifests -- D:\PROGRA~2\MICROS~1\2022\COMMUN~1\VC\Tools\MSVC\1433~1.316\bin\Hostx64\x64\link.exe /nologo @CMakeFiles\03_TwoTriangles.dir\objects1 @<<
 /out:03_TwoTriangles.exe /implib:03_TwoTriangles.lib /pdb:E:\CodeProjects\LearnOpenGL\cmake-build-debug\03_TwoTriangles.pdb /version:0.0 /machine:x64 /debug /INCREMENTAL /subsystem:console   -LIBPATH:E:\CodeProjects\LearnOpenGL\lib  glfw3.lib kernel32.lib user32.lib gdi32.lib winspool.lib shell32.lib ole32.lib oleaut32.lib uuid.lib comdlg32.lib advapi32.lib 
<<

# Rule to build all files generated by this target.
CMakeFiles\03_TwoTriangles.dir\build: 03_TwoTriangles.exe
.PHONY : CMakeFiles\03_TwoTriangles.dir\build

CMakeFiles\03_TwoTriangles.dir\clean:
	$(CMAKE_COMMAND) -P CMakeFiles\03_TwoTriangles.dir\cmake_clean.cmake
.PHONY : CMakeFiles\03_TwoTriangles.dir\clean

CMakeFiles\03_TwoTriangles.dir\depend:
	$(CMAKE_COMMAND) -E cmake_depends "NMake Makefiles" E:\CodeProjects\LearnOpenGL E:\CodeProjects\LearnOpenGL E:\CodeProjects\LearnOpenGL\cmake-build-debug E:\CodeProjects\LearnOpenGL\cmake-build-debug E:\CodeProjects\LearnOpenGL\cmake-build-debug\CMakeFiles\03_TwoTriangles.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles\03_TwoTriangles.dir\depend
