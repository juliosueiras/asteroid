-- premake5.lua
workspace "Asteriod"
	configurations { "Debug", "Release" }

project "Asteriod"
	kind "ConsoleApp"
	language "C++"
	targetdir "bin"
	objdir "bin/obj"

	includedirs { "include" }
	files { "include/**.h", "src/**.cpp", "media/**" }
	copylocal { "media/**" }

	filter "action:gmake"
		system "Linux"
		os.mkdir("bin/media")
		ok, err = os.copyfile("media/**", "bin/media/")
		flags { "Symbols" }
		buildoptions { "`pkg-config --cflags sdl2`" }
		linkoptions { "`pkg-config --libs sdl2`", "`pkg-config --libs SDL2_image`"}

	filter "action:vs2013"
		system "Windows"
		os.mkdir("bin/media")
		ok, err = os.copyfile("media/**", "bin/media/")
		includedirs { "libs/windows/SDL2/include" }
		libdirs { "libs/windows/SDL2/lib/x86" }
		debugdir "bin/"
		links {
			"SDL2",
			"SDL2main",
			"SDL2_image"
		}
		postbuildcommands {
			-- Copy the SDL2 dll to the Bin folder.
			'{COPY} "%{wks.location}libs/windows/SDL2/lib/x86/SDL2.dll" "%{cfg.targetdir}"',
			'{COPY} "%{wks.location}libs/windows/SDL2/lib/x86/lib*.dll" "%{cfg.targetdir}"',
			'{COPY} "%{wks.location}libs/windows/SDL2/lib/x86/zlib1.dll" "%{cfg.targetdir}"',
			'{COPY} "%{wks.location}media" "%{cfg.targetdir}"'
		}
