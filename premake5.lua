-- premake5.lua
workspace "Asteroid"
	configurations { "Debug", "Release" }

project "Asteroid"
	kind "ConsoleApp"
	language "C++"
	targetdir "bin"
	objdir "bin/obj"

	includedirs { "include" }
	files { "include/**.h", "src/**.cpp", "media/**" }
	copylocal { "media/**" }

	filter "action:gmake"
		system "Linux"
		flags { "Symbols" }
		buildoptions { "`pkg-config --cflags sdl2`" }
		linkoptions { "`pkg-config --libs sdl2`", "`pkg-config --libs SDL2_image`"}

        postbuildcommands {
			'{COPY} "media" "%{cfg.targetdir}"'
        }

	filter "action:vs2013"
		system "Windows"
		os.mkdir("bin/media")
		ok, err = os.copyfile("media/**", "bin/media/")
		includedirs { "$(SDL2_INCLUDE)", "$(SDL2_MIXER_INCLUDE)" , "libs/windows/SDL2/include" }
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
