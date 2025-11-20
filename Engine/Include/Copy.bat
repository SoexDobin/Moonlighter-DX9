xcopy	/y		"..\Bin\Engine.dll"		"..\..\Client\Bin\"
xcopy	/y		"..\Bin\Engine.lib"		"..\..\Reference\Library\"
xcopy	/y		"..\Header\*.*"			"..\..\Reference\Header\"

xcopy	/y		".\FMOD\*.h*"			"..\..\Reference\Header\"
xcopy	/y		".\FMOD\fmod.dll"		"..\..\Client\Bin\"
