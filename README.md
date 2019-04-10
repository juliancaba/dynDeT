# dynDeT: **dyn**amic partial reconfiguration **De**sign **T**ool #

Dynamic reconfiguration is a technique that allows user to modify regions of the FPGA without stop it, that is, users can adapt designs already implemented by adding or modifying functionalities to the FPGA at runtime

Exploiting this technique with current design tools can be complex for users, so **the objective of this tool is to simplify the process necessary to design a dynamically reconfigurable application**



## Dependencies ##
dynDeT depends on other tools and libraries that are listed below.

* [CTemplate](https://github.com/OlafvdSpek/ctemplate). Provides an easy, fast and efficient system of text templates for programs written in C++. Used to generate the files describing the system design (design.tcl).

    **Instalation**

    `$ clone or download from gitHub ctemplate project`
    
    `$ cd ctemplate`
    
    `$ ./autogen.sh`
    
    `$ ./configure`
    
    `$ make`
    
    `# make install` // This command as sudo or root (privileged mode)

* [Qt5](https://www.qt.io/download/). Library used for the development of the graphical interface in C++ (GNU LGPL license).

    **Instalation**

    `# apt-get install qt5-default`

* [Vivado from Xilinx](https://www.xilinx.com/products/design-tools/vivado.html)


* [Make] is a build automation tool that automatically builds executable programs and libraries from source code by reading files called Makefiles which specify how to derive the target program.

    **Instalation**

    `# apt-get install make`



## Compile, install and run dynDeT ##

`$ make all`

`$ make install`

`$ make run`

