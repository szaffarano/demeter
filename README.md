## Demeter

Controlador de riego que incorpora algunas mejoras con respecto a su predecesor baudino relacionadas con el manejo remoto y ampliación de sensores que maneja.

Está implementado en un microcontrolador AVR ATMega328 **sin bootloeader ni librerías Arduino**.

Versión preliminar con una implementación parcial del protocolo modbus.

### Roadmap

- Terminar de mapear registros modbus a sensores, parámetros y comandos del controlador.
- Implementar servicio rest para interactuar remotamente.
- Realizar prueba de concepto para usar NRF24L01+ en lugar del módulo bluetooth.
- Documentar esquemáticos en kicad.
- Armar una webapp para administrar el controlador.
- Incorporar interrupción watchdog para preveer posibles fallos.
- Implementar los relays usando triacs+opto.
- Agregar sensor de humedad de tierra.
- Desdoblar los controles para riego tradicional (tierra) e hidropónico en forma independiente.
