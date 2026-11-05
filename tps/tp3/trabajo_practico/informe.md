# Trabajo Práctico N°3

## Hito 12: Optimización

| Ejecutable                       | text | data | bss | dec  | hex  |
|----------------------------------|-----:|-----:|----:|-----:|-----:|
| `build/building_process_demo`     | 6938 |  608 |   8 | 7554 | 1d82 |
| `build/building_process_demo_opt` | 5687 |  608 |  16 | 6311 | 18a7 |

Cambia principalmente gracias a las optimizaciones que se habilitan en el compilador cuando se usa la flag O2, que en este caso, además de reducir el tiempo de ejecución también disminuye el tamaño del código. 

Algunas optimizaciones visibles que realiza O2:

El linker, para ambos ejecutables, tiene habilitada la flag --gc-sections. Sin embargo, building_process_demo tiene 77 entradas en la tabla de símbolos mientras que building_process_demo_opt cuenta con 74. Los símbolos que difieren son ring_is_full, ring_is_empty y uart_send_byte.

- ring_is_full y ring_is_empty son funciones marcadas como inline por lo que el compilador hace un “copy-paste” del cuerpo de las funciones. 


- uart_send_byte se descarta ya que el compilador reduce el código de uart_send y no llama a uart_send_byte, por lo que luego el linker la puede eliminar.



## Hito 14: Preguntas con nm, objdump, readelf y size

### 1. Tres símbolos de mathlib.o y su tipo según nm
 
```text
0000000000000000 T math_factorial
0000000000000000 T math_gcd
0000000000000000 T math_is_prime
```

Los 3 símbolos se encuentran en .text y tienen alcance global. 

### 2. ¿Qué símbolos usa demo_embedded.o sin definirlos? ¿Dónde se resuelven?


Para encontrar qué símbolos no se encuentran definidos filtramos por símbolos del tipo U.

```text
❯ nm -n build/demo/demo_embedded.o | grep U
                 U print_dec
                 U print_hex
                 U print_str
                 U ring_init
                 U ring_pop
                 U ring_push
                 U __stack_chk_fail
                 U str_crc8
                 U timer_get_ms
                 U timer_init
                 U timer_tick
```
Usando nm -l sobre el ejecutable final encontramos en que archivos se definen.  
Se resuelven en los miembros hal_ring.o, hal_timer.o y strutils.o de libutils.a y el objeto platform/print.o. 

```
❯ nm -nl build/building_process_demo

000000000000175a T print_str	/home/ignafede/ing-comp/fw-10-2026/tps/tp3/trabajo_practico/src/platform/print.c:3
0000000000001794 T print_dec	/home/ignafede/ing-comp/fw-10-2026/tps/tp3/trabajo_practico/src/platform/print.c:9
0000000000001846 T print_hex	/home/ignafede/ing-comp/fw-10-2026/tps/tp3/trabajo_practico/src/platform/print.c:23

0000000000001cdb t ring_is_empty	/home/ignafede/ing-comp/fw-10-2026/tps/tp3/trabajo_practico/src/hal/hal_ring.h:20
0000000000001cf6 t ring_is_full	/home/ignafede/ing-comp/fw-10-2026/tps/tp3/trabajo_practico/src/hal/hal_ring.h:24
0000000000001d12 T ring_init	/home/ignafede/ing-comp/fw-10-2026/tps/tp3/trabajo_practico/src/hal/hal_ring.c:4
0000000000001d4f T ring_push	/home/ignafede/ing-comp/fw-10-2026/tps/tp3/trabajo_practico/src/hal/hal_ring.c:12
0000000000001ddc T ring_pop	/home/ignafede/ing-comp/fw-10-2026/tps/tp3/trabajo_practico/src/hal/hal_ring.c:22

0000000000001e6b T timer_init	/home/ignafede/ing-comp/fw-10-2026/tps/tp3/trabajo_practico/src/hal/hal_timer.c:5
0000000000001e80 T timer_tick	/home/ignafede/ing-comp/fw-10-2026/tps/tp3/trabajo_practico/src/hal/hal_timer.c:8
0000000000001e9a T timer_get_ms	/home/ignafede/ing-comp/fw-10-2026/tps/tp3/trabajo_practico/src/hal/hal_timer.c:11
000000000000400c b timer	/home/ignafede/ing-comp/fw-10-2026/tps/tp3/trabajo_practico/src/hal/hal_timer.c:3

0000000000001c68 T str_crc8	/home/ignafede/ing-comp/fw-10-2026/tps/tp3/trabajo_practico/src/lib/strutils.c:111

```

### 3. ¿Cuál es la sección más grande del ejecutable?
La sección más grande del ejecutable, sin contar las secciones con información de debug como .debug_info, es la sección .text.

```text
❯ readelf -S build/building_process_demo

Section Headers:
  [Nr] Name              Type             Address           Offset
       Size              EntSize          Flags  Link  Info  Align

  [16] .text             PROGBITS         00000000000010a0  000010a0
       0000000000000f49  0000000000000000  AX       0     0     16
```

### 4. Después del hito 11, ¿en qué miembro de libutils.a está math_factorial?
 Después del hito 11, math_factorial se encuentra en el miembro mathlib.o de libutils.


    ❯ readelf -s build/libutils.a
    
    File: build/libutils.a(mathlib.o)
    
    Symbol table '.symtab' contains 16 entries:
    12: 0000000000000000    54 FUNC    GLOBAL DEFAULT    4 math_factorial


### 5. ¿Por qué la aplicación necesita -lutils para enlazar, pero libutils.a no necesita a la aplicación?

Esto se debe a que la aplicación usa símbolos que se encuentran definidos en libutilis.a y para ejecutarse necesita la definición de estos. 


La librería, en cambio, cuenta con todos los símbolos definidos dentro de sus objetos. Además de que, al ser una librería, puede tener símbolos sin  definir al momento de compilación. Lo importante es que se puedan resolver en la etapa de linkeo.
