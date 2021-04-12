# PG7232_Sprint3_Grupo3

El objetivo de este proyecto es desarrollar un producto de iluminación LED para encuentros sociales y fiestas, llamado Lighting Dancer, que se basa en distintas modalidades de control:
* Color fijo
* Cambio de color lento
* Cambio de color rápido
* Control basado en sonido para todas las frecuencias
* Control basado en sonido para frecuencias bajas
* Control basado en sonido para frecuencias medias
* Control basado en sonido para frecuencias altas
* Control DMX (*en desarrollo*)

El producto consta de tres tubos de ocho LEDs NeoPíxels cada uno, que se iluminan bajo las modalidades de control descritas y además incluye un módulo de feedback al usuario. Además, dispone de un selector de frecuencias para las modalidades de control y un sistema de protección por temperatura que apaga el sistema completo en caso de sobrecalentamiento. 

Este repositorio contiene el código fuente desarrollado en [MPALBX](https://www.microchip.com/en-us/development-tools-tools-and-software/mplab-x-ide) y la simulación del circuito electrónico en [Proteus](https://www.labcenter.com/simulation/), capaces de controlar NeoPíxels con el microcontrolador PIC16F1717. 

## Tabla de Contenidos
Para obtener mayor información sobre el funcionamiento del producto, los componentes empleados y el presupuesto de desarrollo accede al enlace de [Wiki](https://github.com/annacafaro/PG7232_Sprint3_Grupo3/wiki).
1. [Descripción](https://github.com/annacafaro/PG7232_Sprint3_Grupo3/wiki)     
2. [Funcionamiento del producto](https://github.com/annacafaro/PG7232_Sprint3_Grupo3/wiki/Funcionamiento-del-producto)      
3. [Presupuesto de desarrollo](https://github.com/annacafaro/PG7232_Sprint3_Grupo3/wiki/Presupuesto-de-desarrollo)      
   3.1. [Características de los componentes](https://github.com/annacafaro/PG7232_Sprint3_Grupo3/wiki/Caracter%C3%ADsticas-de-los-componentes)      
4. [Getting started](https://github.com/annacafaro/PG7232_Sprint3_Grupo3/wiki/Getting-Started)          
   4.1. [Dependencias](https://github.com/annacafaro/PG7232_Sprint3_Grupo3/wiki/Getting-Started#dependencias)              
   4.2. [Código Fuente](https://github.com/annacafaro/PG7232_Sprint3_Grupo3/wiki/Getting-Started#c%C3%B3digo-fuente)         
   4.3. [Configuración MCC del PIC16F1717](https://github.com/annacafaro/PG7232_Sprint3_Grupo3/wiki/Getting-Started#configuraci%C3%B3n-mcc-del-microcontrolador-pic16f1717)               

## Simulación 

Para el proyecto desarrollado se requirió del uso del 18.9% del espacio de la memoria disponible, como se muestra en las figuras. Además, es importante considerar la relación costo-pines no utilizados, en este caso, se seleccionó este microcontrolador debido a que dispone de la suficiente cantidad de pines con conversores Analógico-Digital, lo que permitió el desarrollo del producto con la cantidad de puertos y canales necesarios para adquirir toda la data deseada.

<img src="https://github.com/annacafaro/PG7232_Sprint3_Grupo3/blob/main/images/memory_summary.PNG" alt="drawing" width="600"/> 

<img src="https://github.com/annacafaro/PG7232_Sprint3_Grupo3/blob/main/images/memory_summary2.PNG" alt="drawing" width="400"/> 




Haz click en la imagen para ver el video del funcionamiento del producto. 

[![](https://github.com/annacafaro/PG7232_Sprint3_Grupo3/blob/main/images/Sprint%203%20Presentation.png)](https://youtu.be/tCnasQKqAL4)


## Contributors
* [Carlos Serrano](https://github.com/carlosluissb)
* [Diego Negrette](https://github.com/DiegoNegrette)
* [Jorge Aray](https://github.com/electropelon)
* [Mariana Jiménez](https://github.com/marijml)

## LICENSE
The [MIT](https://github.com/annacafaro/PG7232_Sprint3_Grupo3/blob/main/LICENSE) License 
