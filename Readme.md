
# PS2x2Pico&Joystick's

**forked from  https://github.com/No0ne/ps2x2pico, https://github.com/destroyer-dcf/usb2ps2**



USB keyboard/mouse to PS/2 interface converter using a Raspberry Pi Pico by No0ne
 ( **https://github.com/No0ne/ps2x2pico** ) y Clasic Digital Joysticks support norma Atari gracias a la libreria de TuriSc RP2040-Button ( https://github.com/TuriSc/RP2040-Button), para su uso en emulador de Spectrum **https://zxespectrum.speccy.org/** by **Víctor Iborra & David Crespo**, proyecto actualizado en GitHub  ( **https://github.com/EremusOne/ESPectrum** ) .
 
He diseñado una placa nueva inspirada en la placa con Rp2040 Zero de **Destroyer** ( **https://github.com/destroyer-dcf/usb2ps2** ),
![PcB](./images/IMG_20241110_194500.jpg)
![Esquema](./images/Ps2x2pico&joySticksEsquema.jpg)

Cuando la vi pense que lastima no aprovechar los pines que sobran.... y este es el resultado esta en fase beta, pero en princio es funcional, falta trabajo por hacer, configurar las teclas del puerto de joystick 2, ahora  los dos puertos son emuladas las pulsaciones de las mismas teclas, cursores para movimiento y TAB para disparo igual es indistinto usar uno o otro conector, dar soporte para gamepad de sega de tres y seis botones al igual que el proyecto de  **David Carrion** (  **https://github.com/dacarsoft/DB9_2_Keyboard** ) 
o el fork del mismo de **Antonio Taimaron**, PowaJoy ( **https://github.com/hash6iron/PowaJoy**), el soporte para gamepad usb esta empezado, como puede verse en el codigo, pero no es apena usable, congela el emulador ESPectrum.


Animo a todo el que quiera unirse o desee continuarlo por su cuenta creando otro fork, lo haga libremente, yo por mi parte ire trabajando en el,  en la medida que mis conocimientos, me lo permitan... es lo que tiene programar de oido.
En la carpeta PCB se pueden encontrar los pdf generados con eagle para quien quiera hacerse la placa, yo uso papel de transfer para circuitos, los imprimo con la impresora laser y seguidamente los plancho literalmente con la plancha de casa para transferirlo a la placa, depues agujeros, y a continuacion atacado del cobre con un baño de 3 partes de salfuman y 2 de agua oxigenada, y una vez eliminado el cobre sobrante limpieza con acetona para quitar el toner,   pintar con colofonia (resina de pino) para proteger las pista y facilitar su soldado y ya finalmente soldar los componente.

Para compilar, en pricipio instalar en vscode la extensión oficial Raspberry Pi Pico`v0.17.1`y seguir las instrucciones que se pueden encontar en la pagina de https://github.com/No0ne/ps2x2pico.

Copiar el fichero 20241110_ps2x2pico.uf2 situado en el directorio build en la raiz del Rp2040 Zero, pulsar boton boot y conectar al usb.

**https://github.com/TuriSc/RP2040-Button**

**LeoCroms@2024**