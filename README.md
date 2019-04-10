# Herramienta de desarrollo de aplicaciones dinámicamente reconfigurables en FPGA #

La reconfiguración dinámica es una técnica que permite reprogramar regiones de la FPGA sin que ésta tenga que detenerse, es decir, uno puede adaptar diseños ya implementados agregando o modificando funcionalidades a la FPGA en tiempo de ejecución.

Dado que explotar esta técnica con las herramientas de diseño actuales puede llegar a ser complejo o poco intuitivo para el usuario, **el objetivo es desarrollar una herramienta que simplifique el proceso necesario para diseñar una aplicación dinámicamente reconfigurable**.


## Uso de librerías externas ##
A continuación se muestran las librerías utilizadas por la herramienta de este TFG. Es necesario disponer de estas librerías antes de compilar la herramienta.

* [CTemplate](https://github.com/OlafvdSpek/ctemplate). Proporciona un sistema de plantillas de texto fácil, rápido y eficiente para programas escritos en C++. Utilizado para generar los ficheros que describen el diseño del sistema (design.tcl).

    **Instalación**

    `$ unzip ctemplate-master.zip`
    
    `$ cd ctemplate-master`
    
    `$ ./autogen.sh`
    
    `$ ./configure`
    
    `$ make`
    
    `# make install`

* [Qt5](https://www.qt.io/download/). Librería utilizada para el desarrollo de la interfaz gráfica en C++ (licencia GNU LGPL).

    **Instalación**

    `# apt-get install qt5-default`


## Compilar, instalar y ejecutar ##
La compilación y la ejecución de la herramienta se realiza a través de _make_:

`$ make all`

`$ make install`

`$ make run`

