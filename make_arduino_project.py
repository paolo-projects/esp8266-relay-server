#!/usr/bin/python3

import os
from shutil import copyfile

PROJECT_NAME = "ino_relay_server"
INCLUDE_DIR = "./include"
SRC_DIR = "./src"
MAIN_FILE = "relay_controller.cpp"
DEST_DIR = f"./{PROJECT_NAME}"


def main():
    # Create arduino project dir
    print(f"Creating project directory: {DEST_DIR}")
    if not os.path.exists(DEST_DIR):
        os.mkdir(DEST_DIR)
    # Copy headers
    print(f"Copying header files from {INCLUDE_DIR} to {DEST_DIR}")
    for filename in os.listdir(INCLUDE_DIR):
        if filename.endswith('.h'):
            copyfile(os.path.join(INCLUDE_DIR, filename),
                     os.path.join(DEST_DIR, filename))
    # Copy sources
    print(f"Copying source files from {SRC_DIR} to {DEST_DIR}")
    for filename in os.listdir(SRC_DIR):
        if filename.endswith('.cpp'):
            copyfile(os.path.join(SRC_DIR, filename),
                     os.path.join(DEST_DIR, filename))

    print(f"Renaming main project file from {MAIN_FILE} to {PROJECT_NAME}.ino")
    os.rename(f"{DEST_DIR}/{MAIN_FILE}", f"{DEST_DIR}/{PROJECT_NAME}.ino")

    print("Done")


if __name__ == "__main__":
    main()
