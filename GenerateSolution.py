import subprocess
import sys
sys.path.insert(0, '.\\BuildScripts')

import DgBuild

DgBuild.Make_vpaths("./DgLib/src/", "./DgLib_vpaths.lua")

subprocess.call("DgLib/3rdParty/premake/premake5.exe vs2022")
wait = input("PRESS ENTER TO CONTINUE.")
