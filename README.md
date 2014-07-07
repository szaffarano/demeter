## Demeter

Controlador de riego que incorpora algunas mejoras con respecto a su predecesor baudino relacionadas con el manejo remoto y ampliación de sensores que maneja.

Está implementado en un microcontrolador AVR ATMega328 **sin bootloeader ni librerías Arduino**.

Versión preliminar con una implementación parcial del protocolo modbus.

### Roadmap

- ~~Terminar de mapear registros modbus a sensores, parámetros y comandos del controlador.~~
- Implementar servicio rest para interactuar remotamente.
- Realizar prueba de concepto para usar NRF24L01+ en lugar del módulo bluetooth.
- ~~Documentar esquemáticos en kicad~~.
- Armar una webapp para administrar el controlador.
- Incorporar interrupción watchdog para preveer posibles fallos.
- Implementar los relays usando triacs+opto.
- Agregar sensor de humedad de tierra.
- Desdoblar los controles para riego tradicional (tierra) e hidropónico en forma independiente.
- Optimizar lógica en los mapeos para ahorrarse algunos ciclos de cpu y memoria.
- Agregar en el cliente python funciones de alto nivel (ej get_date que lea grupo de registros de la fecha en vez de llamar a read_holding_registers).
- ~~Refactorizar mapeo de registros modbus para optimizar bits y que no quede todo en uint16_t.~~ (DUPLICADO).
- Optimizar algoritmo en los callbacks modbus.
- Generar tickets github para no mantener un archivo de pendientes...
