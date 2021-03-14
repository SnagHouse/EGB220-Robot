
project "Test"
  configurations { "Debug", "Release" }

  dependson { "FractalEngine" }

  kind "ConsoleApp"
  architecture "x64"
  language "C++"
  characterset ("MBCS")

-- Set Directories
  targetdir (binPath)
  debugdir (binPath)
  objdir (buildsPath .. "/output/%{cfg.platform}_%{cfg.buildcfg}")
  symbolspath '$(OutDir)$(TargetName).pdb'

-- Includes
  includedirs { "../sketch/" } 

-- Project Files
  files { "**.cpp", "**.h", "**.inl" , "**.natvis" }

