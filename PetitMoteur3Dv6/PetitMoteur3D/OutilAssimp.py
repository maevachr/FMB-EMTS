import os, sys
import subprocess
pathCopy = "C:\Cours\Infographie\FMB-MT\PetitMoteur3Dv6\PetitMoteur3D"
print(sys.argv[1]+r"OutilAssimp.exe")
print(sys.argv[2])
print(sys.argv[3])
subprocess.run([sys.argv[1]+r"OutilAssimp.exe", sys.argv[2], sys.argv[1]+sys.argv[3]])
