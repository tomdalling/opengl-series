solution "01_project_skeleton"
	location("./")
	targetdir("./bin")
	configurations { "debug", "release" }
	objdir("obj/" .. os.get() .. "/")

	project "01_project_skeleton-app"
		kind "ConsoleApp"
		language "C++"
		files { "source/*.cpp" }

		configuration "windows"
			links {"glu32", "opengl32", "gdi32", "winmm", "user32","GLEW"}

		configuration "linux"
			links {"GL","glfw","GLEW"}
		
		configuration "macosx"
			links {"GL","glfw","GLEW", "CoreFoundation.framework"}
			libdirs {"/opt/local/lib"}
			includedirs {"/opt/local/include"}
		
		configuration "haiku"
			links {"GL","glfw","GLEW"}

		configuration "freebsd"
			links {"GL","glfw","GLEW"}
		
		configuration "debug"
			defines { "DEBUG" }
			flags { "Symbols" }
			buildoptions{ "-Wall" }
			targetname "01_project_skeleton-debug"

		configuration "release"
			defines { "NDEBUG" }
			flags { "Optimize" }
			buildoptions{ "-Wall" }
			targetname "01_project_skeleton-release"
