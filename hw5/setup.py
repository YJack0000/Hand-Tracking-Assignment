import os
import sys
import subprocess
import platform

def install(package):
    subprocess.check_call([sys.executable, "-m", "pip", "install", package])

def install_all():
    install("numpy")
    install("mediapipe")
    install("opencv-python")
    install("opencv-contrib-python")

if __name__ == "__main__":
    if platform.system() == "Windows":
        install_all()
    elif platform.system() == "Linux":
        install_all()
    elif platform.system() == "Darwin":
        install_all()
    else:
        print("OS not supported")