# ArmOS
Exploration project for making an OS for ARM. Targets RaspberryPi and STM devices.

# Quickstart

[![Open in GitHub Codespaces](https://github.com/codespaces/badge.svg)](https://github.com/codespaces/new?hide_repo_select=true&ref=main&repo=581907267&machine=basicLinux32gb&devcontainer_path=.devcontainer%2Fdevcontainer.json&location=EastUs)

This OS is not _functional_ yet but it can still be toyed with. The easiest way to start is to use the pre-configured **VSCode devcontainer**. This method will boot you into a fully configured environment with all the build dependencies and scripts already installed. If you are on Github, you can use a **Codespace**, those are online versions of a devcontainer. Click the Codespace tag above to boot a Codespace instance.

If you want to test locally, you must first have **Docker** installed locally. This is because there is a *fairly large* amount of build toolchain dependencies that needs to be installed. For example, the OS requires a custom compiled version of GCC that requires a least 15 minutes to build and takes a couple of GB of space on your computer. This compiler, in turn, requires many sub-depdendencies to be installed. None of this is easy to set up on Windows. The devcontainer simplifies all that by providing a pre-built Docker image with everything set up.

To launch the devcontainer locally, just clone the repo and open the folder with VSCode. You should have a prompt asking you if you want to re-open the workspace from within the devcontainer. If you don't have the prompt, use the bottom-left Remote Connect (green button) and select Reopen in Container. It will initially take some time while it downloads the Docker image. Once the setup is complete, you will have the workspace open.

To start the OS, 2 operations are needed. First open a terminal (CTRL+\`) and `cd src/kernel` and then execute `make debug`. This builds the kernel and starts the QEMU emulator on the image file. The QEMU emulator will hang until a debugger is connected.

Open a second terminal pane and `cd src/kernel`. Once in that directory do `gdb-multiarch`. This will use the current directory's `.gdbinit` file to preconfigure some stuff and connect to the local GDB server opened by the QEMU emulator. I suggest you pull that terminal into its own tab.

You should get a window like this:

![This is an image](/support/assets/vscode_debug_window.png)

In the GDM command window, use the `s` command to step to the next instruction. Do it a dozen times and you should end up in the `kernel_main` function which should do some _stuff_. Or just use the `c` command to "continue" the execution until the next breakpoint. If you continue a few times you should be free of the initial kernel breakpoints and the kernel should do some stuff.

Have fun ! Or good luck ?