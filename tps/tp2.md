# Trabajo Práctico N°2 : Firmware y Arquitectura

### Completen la tabla: 

Usando el test ``fw/tests/test_fw_status.c`` como base completen la siguiente tabla, donde en cada iteración tienen que modificar el FW, el compilador o el HW.

Asegurense que en ``fw/Makefile`` estemos compilado el fw que queremos. Ejecuten un ``make clean`` antes de pasar a la siguiente línea de la tabla. 

| Caso (test_fw_status.c)  | Flag march (fw/Makefile) | Config de RTL (top.v)  | (*)Tamaño de FW | Ciclos totales | (**)Uso de celdas | Accesos a memoria |
| ------------------------ | ------------------------ | ------------------- | ---------- | ----------------- | -------------- | ---------- |
| Original                 | rv32i                    | original            |  976 Bytes  |   13876          |   3,9 MB              | Data:894 - Inst:3893        |
| Aumentar iteraciones x10  | rv32i                    | original            |      976 Bytes      |   210132                |         3,9 MB       | Data:8414 - Inst:59105          |
| Anterior                 | rv32ic                   | original            |    748 Bytes        |                  12 |           3,9 MB     | Data:0 - Inst:4          |
| Anterior                 | rv32ic                   | COMPRESSED_ISA(1)   |       748 Bytes         |    214367            |      157 MB      | Data:8414 - Inst:52872          |
| Agregar multiplicaciones      | rv32ic                   | anterior            |     868 Bytes      |       683255            |     157 MB           |   Data:11414 - Inst:189828         |
| Anterior                 | rv32imc                  | anterior            |      636 Bytes     |                  1605 |       157 MB          |       Data:118 - Inst:366    |
| Anterior                 | rv32imc                  | ENABLE_MUL(1)       |   636 Bytes         |        169176           |     158 MB           |      Data:10214 - Inst:22585      |
| Anterior                 | rv32imc                  | ENABLE_FAST_MUL(1)  |  636 Bytes          | 148776                 |       157 MB |   Data:10214 - Inst:22585         | 

(*)  utilizar dirección de la última instrucción

(**) estimación usando tamaño de compilación de verilator: ``du obj_dir/``

### Caso base con iteraciones x10

Tenemos un procesador con hardware mínimo, ideal para un sistema embebido que requiera el uso de pocos recursos. 
Aumentar las iteraciones por 10 provoca un aumento mayor en la cantidad de accesos a memoria de instrucciones, por 15. Esto se debe a que, a  medida que las iteraciones aumentan, la rutina de impresion  print_dec() requiere mas ciclos para completarse.   
### Extension C sin soporte en picorv32


Al no contar con soporte en hardware, el procesador no reconoce las instrucciones con formato comprimido y por lo tanto no las puede ejecutar. 
Esto genera una excepción que picorv32 señaliza mediante la flag trap.


La variable wire trap del testbench.sv no se encontraba conectada. Para que la señal de trap finalice la ejecución del programa primero tuve que añadir en el módulo top.v la señal output wire trap  y luego conectarla en  el testbench.sv.






El programa se ejecuta hasta que llega la primer instrucción que no se reconoce.
### Extensión C habilitada en picorv32

Disminuye el tamaño del firmware y la cantidad de accesos a la memoria de instrucciones. Es una buena configuración para usar si contamos con poca memoria de instrucciones  o queremos generar menos accesos para disminuir la latencia.

Una desventaja es que aumentan los ciclos de clock. Esto seguramente se debe a un incremento en los ciclos necesarios para decodificar cada instrucción, ya que ahora se tienen dos tamaños posibles. Otra desventaja es que aumenta el número de celdas, algo a tener en cuenta según el hardware disponible.





### Multiplicaciones



Agregar las líneas de código que realizan multiplicaciones genera 3,5 veces más accesos a memoria de instrucciones respecto al caso anterior. La cantidad de ciclos de clock también se incrementó en unas 3 veces. En comparación, el acceso a memoria de datos solo aumentó en 1,3.

Esto se debe a cómo el compilador genera multiplicaciones. Sin tener la extensión M habilitada utiliza otro tipo de instrucciones que necesitan ejecutarse una mayor cantidad de veces para llegar al resultado. No es eficiente si nuestra aplicación necesita realizar multiplicaciones constantemente, sólo si estamos limitados en recursos es una opción válida.

### Extensión M sin soporte en picorv32

El procesador ejecuta el programa hasta la primera instrucción mul, que no reconoce. En ese momento no continúa con la ejecucion normal del programa, sino que genera una excepción,  señalizada mediante la flag trap.

Como la primer instrucción ilegal llega después del primer mensaje enviado por uart, en la ejecución se puede ver:


```
UART: -- running test_fw_status --
```
### Extensión M  habilitada en picorv32

Además de habilitar las multiplicaciones en picorv32 también hay que habilitar las divisiones mediante ENABLE_DIV(1) para que el programa funcione, ya que la extensión m incluye multiplicación y división.

En verilator no hay un aumento significativo del tamaño de obj_dir, imagino que se debe a cómo se simula el hardware para las multiplicaciones. Utilizando la herramienta de síntesis yosis para obtener con mejor precisión la cantidad de celdas, llegue a estos resultados:
- picorv32_compressed isa = 11620 celdas

- picorv32_pcpi_mul = 1019 celdas 

- picorv32_pcpi_div = 1704 celdas







picorv32_compressed isa es el procesador propiamente dicho, pcpi mul y div son módulos separados.


La cantidad de ciclos se reduce en un 75%, el acceso a memoria de instrucciones un 88%, por un aumento de celdas de solo un 23%. El tamaño del binario también disminuye. Esto se debe a que el compilador emite instrucciones específicas de multiplicación y división para realizar los cálculos, generando una menor cantidad de instrucciones y que necesitan ejecutarse una menor cantidad de veces para completar el programa.

Esta configuración es preferible para aplicaciones intensivas en cálculo de multiplicaciones. Nos da mejor performance pero teniendo en cuenta que pagamos el costo con un poco más de hardware.





### Extension M con ENABLE_FAST_MUL(1)

- picorv32_pcpi_fast_mul = 9492 celdas

Disminuye la cantidad de ciclos de clocks pero aumenta la cantidad de celdas. Es justificable si nuestra aplicación debe tener baja latencia y tenemos mas hardware disponible ya que la diferencia entre picorv32_pcpi_mul y picorv32_pcpi_fast_mul es de unas 8000 celdas, según yosis.

