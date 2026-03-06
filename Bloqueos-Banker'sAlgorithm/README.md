# Algoritmo del Banquero (Banker's Algorithm)

Implementación en C++ del algoritmo del Banker's Algorithm para prevencion de deadlocks. En este caso lo usamos para determinar si dados estados son seguros.


## Compilar y ejecutar

```bash
g++ -o BankAl BankAl.cpp
./BankAl
```

El programa prueba con cada estado de prueba y el usuario puede verificar si es seguro y simular que un hilo solicite recursos.

## Sobre el algoritmo

- **Available**: Vector que indica cuántos recursos de cada tipo están disponibles en el sistema.

- **Max**: Matriz que define la demanda máxima de cada thread. Max[i][j] = máximo de recursos tipo j que el thread i puede solicitar.

- **Allocation**: Matriz que indica cuántos recursos de cada tipo tiene asignados cada thread actualmente. Allocation[i][j] = recursos tipo j asignados al thread i.

- **Need**: Matriz que indica cuántos recursos adicionales necesita cada thread para terminar. Se calcula como Need[i] = Max[i] - Allocation[i].

- **Work**: Variable temporal del algoritmo que representa los recursos disponibles en cada paso de la simulación. Inicia como Work = Available y va creciendo conforme los threads terminan y devuelven sus recursos.

### Funcionamiento:

1. Iniciamos con Work = Available
2. Buscasmos un thread que pueda terminar con lo disponible: Need[i] <= Work
3. Si lo encuentramos, simulamos que termina y devuelve sus recursos: Work = Work + Allocation[i]
4. Repetimos hasta que todos terminen (Seguro) o nadie pueda avanzar (Inseguro)

---

## Problema 8.27

Dado el siguiente snapshot del sistema:

| Thread | Allocation (A,B,C,D) | Max (A,B,C,D) |
|--------|---------------------|---------------|
| T0     | 1, 2, 0, 2          | 4, 3, 1, 6    |
| T1     | 0, 1, 1, 2          | 2, 4, 2, 4    |
| T2     | 1, 2, 4, 0          | 3, 6, 5, 1    |
| T3     | 1, 2, 0, 1          | 2, 6, 2, 3    |
| T4     | 1, 0, 0, 1          | 3, 1, 1, 2    |

Usando el algoritmo del banquero, determina si los siguients estados son seguros. Si el estado es seguro, ilustra el orden en que los procesos podrían cumplir. De otra forma, ilustra porque el estado es inseguro.

### Matriz Need

```
Need[i] = Max[i] - Allocation[i]
```

| Thread | Need (A,B,C,D) |
|--------|----------------|
| T0     | 3, 1, 1, 4     |
| T1     | 2, 3, 1, 2     |
| T2     | 2, 4, 1, 1     |
| T3     | 1, 4, 2, 2     |
| T4     | 2, 1, 1, 1     |

---

## Análisis por inciso

### a) Available = (2, 2, 2, 3) -- Seguro

```
Work = (2,2,2,3)

1. T4: Need(2,1,1,1) <= Work(2,2,2,3)? SI
   T4 termina, Work = Work(2,2,2,3) + AllocationT4(1,0,0,1) = (3,2,2,4)

2. T0: Need(3,1,1,4) <= Work(3,2,2,4)? SI
   T0 termina, Work = Work(3,2,2,4) + AllocationT0(1,2,0,2) = (4,4,2,6)

3. T1: Need(2,3,1,2) <= Work(4,4,2,6)? SI
   T1 termina, Work = Work(4,4,2,6) + AllocationT1(0,1,1,2) = (4,5,3,8)

4. T2: Need(2,4,1,1) <= Work(4,5,3,8)? SI
   T2 termina, Work = Work(4,5,3,8) + AllocationT2(1,2,4,0) = (5,7,7,8)

5. T3: Need(1,4,2,2) <= Work(5,7,7,8)? SI
   T3 termina

Todos terminaron -- Seguro
Secuencia: T4 - T0 - T1 - T2 - T3
```

---

### b) Available = (4, 4, 1, 1) -- Seguro

```
Work = (4,4,1,1)

1. T2: Need(2,4,1,1) <= Work(4,4,1,1)? SI
   T2 termina, Work = Work(4,4,1,1) + AllocationT2(1,2,4,0) = (5,6,5,1)

2. T4: Need(2,1,1,1) <= Work(5,6,5,1)? SI
   T4 termina, Work = Work(5,6,5,1) + AllocationT4(1,0,0,1) = (6,6,5,2)

3. T1: Need(2,3,1,2) <= Work(6,6,5,2)? SI
   T1 termina, Work = Work(6,6,5,2) + AllocationT1(0,1,1,2) = (6,7,6,4)

4. T0: Need(3,1,1,4) <= Work(6,7,6,4)? SI
   T0 termina, Work = Work(6,7,6,4) + AllocationT0(1,2,0,2) = (7,9,6,6)

5. T3: Need(1,4,2,2) <= Work(7,9,6,6)? SI
   T3 termina

Todos terminaron -- Seguro
Secuencia: T2 - T4 - T1 - T0 - T3
```

---

### c) Available = (3, 0, 1, 4) -- Inseguro

```
Work = (3,0,1,4)

- T0: Need(3,1,1,4) <= Work(3,0,1,4)? NO (1 > 0 para el recurso B)
- T1: Need(2,3,1,2) <= Work(3,0,1,4)? NO (3 > 0 para el recurso B)
- T2: Need(2,4,1,1) <= Work(3,0,1,4)? NO (4 > 0 para el recurso B)
- T3: Need(1,4,2,2) <= Work(3,0,1,4)? NO (4 > 0 para el recurso B)
- T4: Need(2,1,1,1) <= Work(3,0,1,4)? NO (1 > 0 para el recurso B)

Ningún thread puede ejecutar -- Inseguro

El sistema está en deadlock potencial porque todos los threads
necesitan al menos 1 recurso de tipo B, pero hay 0 disponibles.
```

---

### d) Available = (1, 5, 2, 2) -- Seguro

```
Work = (1,5,2,2)

1. T3: Need(1,4,2,2) <= Work(1,5,2,2)? SI
   T3 termina, Work = Work(1,5,2,2) + AllocationT3(1,2,0,1) = (2,7,2,3)

2. T1: Need(2,3,1,2) <= Work(2,7,2,3)? SI
   T1 termina, Work = Work(2,7,2,3) + AllocationT1(0,1,1,2) = (2,8,3,5)

3. T2: Need(2,4,1,1) <= Work(2,8,3,5)? SI
   T2 termina, Work = Work(2,8,3,5) + AllocationT2(1,2,4,0) = (3,10,7,5)

4. T0: Need(3,1,1,4) <= Work(3,10,7,5)? SI
   T0 termina, Work = Work(3,10,7,5) + AllocationT0(1,2,0,2) = (4,12,7,7)

5. T4: Need(2,1,1,1) <= Work(4,12,7,7)? SI
   T4 termina

Todos terminaron -- Seguro
Secuencia: T3 - T1 - T2 - T0 - T4
```

---

## Resumen

| Caso | Available   | Estado   | Secuencia Segura           |
|------|-------------|----------|----------------------------|
| a    | (2,2,2,3)   | Seguro     | T4 - T0 - T1 - T2 - T3     |
| b    | (4,4,1,1)   | Seguro     | T2 - T4 - T1 - T0 - T3     |
| c    | (3,0,1,4)   | Inseguro   | -                          |
| d    | (1,5,2,2)   | Seguro     | T3 - T1 - T2 - T0 - T4     |



---

## Referencia

Silberschatz, A., Galvin, P. B., & Gagne, G. (2018). Deadlocks. En *Operating System Concepts* (10th ed., Cap. 8, pp. 318-323). Wiley.