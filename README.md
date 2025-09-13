## Práctica: Interpolación y Manipulación de Colores

### 1. Interpolación lineal

La interpolación lineal (a menudo abreviada como LERP) es una forma de obtener un valor intermedio entre dos puntos o colores. Si tenemos dos valores `A` y `B`, y un factor `t` entre 0 y 1, la fórmula es:

`LERP(A, B, t) = (1 - t) * A + t * B`

- Cuando `t`=0, el resultado es `A`
- Cuando `t`=1, el resultado es `B`
- Cuando `t`=0.5, el resultado es justo la mitad entre `A` y `B`. 

Por ejemplo, en colores: 

```cpp
color c1(1, 1, 1);  // blanco
color c2(1, 0, 0);  // rojo
double t = 0.25;
color result = (1 - t) * c1 + t * c2; // un blanco "tintado" hacia rojo
```

El código de `ray_color` devuelve un color de fondo en función de la dirección del rayo: 

```cpp
color ray_color(const ray& r, const color& c1, const color& c2) {
    vec3 unit_direction = unit_vector(r.direction());
    auto a = 0.5*(unit_direction.y() + 1.0);
    return (1.0 - a) * c1 + a * c2;
}
```

- Se toma la dirección del rayo `r`.
- Se normaliza y se usa la componente y (vertical).
- El valor a está en [0,1].
- Ese valor a decide qué tanto se mezcla entre c1 y c2

- Si el rayo apunta hacia abajo (y pequeña), se obtiene un color más cercano a c1
- Si el rayo apunta hacia arriba (y grande), se obtiene un color más cercano a c2.

[Ejemplo Interpolación Lineal ](images/base_color.ppm)


## Funciones a Implementar 

### `adjust_brightness`

#### `color adjust_brightness(const ray& r, const color& c);`

- Calcula un factor de brillo según la dirección del rayo.
- Usa la componente `y` del rayo normalizado.
- El factor debe estar entre 0 y 1.
- Multiplica cada componente del color por ese factor.

Esto hará que los colores se vean más oscuros o más brillantes dependiendo de la dirección del rayo.

[Ejemplo adjust_brightness ](images/brightness_example.ppm)

### `blend`

#### `color blend(const color& c1, const color& c2, int i, int j, int image_width, int image_height);`

- Se mezclan dos colores (`c1` y `c2`).  
- El factor de mezcla depende de la posición del píxel `(i, j)` en la imagen.  
- Normaliza la posición:  
```
u = i / (image_width - 1)
v = j / (image_height - 1)
```
- Combina `u` y `v` para obtener un `t` en `[0,1]`.  
- Devuelve la interpolación:  
`(1 - t) * c1 + t * c2`

Así, los colores cambian suavemente de un lado de la imagen al otro. 

[Ejemplo blend ](images/blend_example.ppm)

### `quadrant_blend`

#### `color quadrant_blend(int i, int j, int image_width, int image_height);`

- Divide la imagen en cuatro esquinas (cuadrantes).  
- A cada esquina se le asigna un color distinto los cuales ya estan predefinidos como `red`, `green`, `blue` y `yellow`. Sin embargo si el alumno desea otros colores, puede proponerlos, lo importante es que se noten los cuadrantes. 

- Abajo izquierda → rojo `(1,0,0)`  
- Abajo derecha → verde `(0,1,0)`  
- Arriba izquierda → azul `(0,0,1)`  
- Arriba derecha → amarillo `(1,1,0)`  

- Normaliza la posición:
```
u = i / (image_width - 1)
v = j / (image_height - 1)
```
- Interpola primero en el eje horizontal:  
```
bottom = (1 - u) * c00 + u * c10
top = (1 - u) * c01 + u * c11
```
- Luego interpola en el eje vertical:  
```
result = (1 - v) * bottom + v * top
```
Así se obtiene una **interpolación bilineal**, que mezcla los cuatro colores de las esquinas para producir un degradado suave en toda la imagen.  

[Ejemplo quadrant_blend ](images/quadrant_blend_example.ppm)

### Instrucciones de guardado

Una vez compilado el programa (y como se puede ver en la función `render`), la función que ejectua se determina con el primer parámetro (`brightness`, `blend`, `quadrant`) y las imagenes generadas deben ser guardadas en la carpeta `output`. Es importante que sean distintas a esas presentadas en la carpeta de `images`

**Ajuste de brillo**

`./build/practica_1 brightness > output/brightness.ppm`

**Gradiente usando blend**

`./build/practica_1 blend > output/blend.ppm`

**Gradiente de cuadrantes**

`./build/practica_1 quadrant > output/quadrant.ppm`


*Nota: Para visualizar los .`ppm` pueden usar [GIMP](https://www.gimp.org/) o la [extensión de VSCode](https://marketplace.visualstudio.com/items?itemName=ngtystr.ppm-pgm-viewer-for-vscode)* 

