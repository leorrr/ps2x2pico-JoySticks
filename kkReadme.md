
# PS2x2Pico&Joystick's

**forked from  https://github.com/No0ne/ps2x2pico, https://github.com/destroyer-dcf/usb2ps2**



USB keyboard/mouse to PS/2 interface converter using a Raspberry Pi Pico by No0ne

He diseñado una placa nueva inspirada en la placa con Rp2040 Zero de **Destroyer** ( **https://github.com/destroyer-dcf/usb2ps2** ),
![PcB](./images/IMG_20241110_194500.jpg)
![Esquema](./images/Ps2x2pico&joySticksEsquema.jpg)

Cuando la vi pense que lastima no aprovechar los pines que sobran.... y este es el resultado esta en fase beta, pero en principio es funcional,  el soporte para gamepad usb y los 2 puertos DB9 norma Atari es operativo mapaeado con los Ps2 Key ESpecial codes para usar con emulador ESPectrum, en resumen:

 - Gamepad1 USB se corresponde al joystick 1 en emulador ESPectrum.
 - Gamepad2 USB se corresponde al joystick 2 en emulador ESPectrum.
 - El DB9 1 al joystick 1 en emulador ESPectrum.
 - El DB92 al joystick 2 en emulador ESPectrum.

Pendiente dar soporte para gamepad de sega de tres y seis botones al igual que el proyecto de  **David Carrion** (  **https://github.com/dacarsoft/DB9_2_Keyboard** ) o el fork del mismo de **Antonio Taimaron**, PowaJoy ( **https://github.com/hash6iron/PowaJoy**),

Añadido soporte para GamePad USB imitacion  Nintendo Snes
![GamePad](./images/SnesUsbControler.jpg)

### **Mapeando los botones con los especial ps2 Keycodes para su uso en emulador ESPectrum**

Es necesario el uso de un HUB USB para poder tener conectados todos los dispositivos a la vez, he probado varios, no todos funcionan, el que mejor resultado me dado es siguiente ![HUB_USB](./images/HUB_USB·.0TypeC.JPG), que pude ser encontrado en el siguiente link https://es.aliexpress.com/item/1005007512518100.html?srcSns=sns_Telegram&spreadType=socialShare&bizType=ProductDetail&social_params=61017559658&aff_fcid=0e625e375f134f83a835d0010177047f-1741529235758-02796-_EweRPKs&tt=MG&aff_fsk=_EweRPKs&aff_platform=default&sk=_EweRPKs&aff_trace_key=0e625e375f134f83a835d0010177047f-1741529235758-02796-_EweRPKs&shareId=61017559658&businessType=ProductDetail&platform=AE&terminal_id=60930d633abd4d279204fca963580e98&afSmartRedirect=y 

### Importante asegurarse de comprar el TYPE-C, que puede ser conectado directamente al RP2040-Zero

En la carpeta PCB se pueden encontrar los pdf generados con eagle para quien quiera hacerse la placa, yo uso papel de transfer para circuitos, los imprimo con la impresora laser y seguidamente los plancho literalmente con la plancha de casa para transferirlo a la placa, depues agujeros, y a continuacion atacado del cobre con un baño de 3 partes de salfuman y 2 de agua oxigenada, y una vez eliminado el cobre sobrante limpieza con acetona para quitar el toner,   pintar con colofonia (resina de pino) para proteger las pista y facilitar su soldado y ya finalmente soldar los componente.

Para compilar, en pricipio instalar en vscode la extensión oficial Raspberry Pi Pico`v0.17.1`y seguir las instrucciones que se pueden encontar en la pagina de https://github.com/No0ne/ps2x2pico.

Copiar el fichero  .uf2 situado en el directorio release en la raiz del Rp2040 Zero, pulsar boton boot y conectar al usb.


**LeoCroms@2025**
