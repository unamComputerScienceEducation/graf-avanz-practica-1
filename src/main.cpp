#include <iostream>
#include <string>

#include "color.h"
#include "vec3.h"
#include "ray.h"

// structs para Imagen y Camera 

struct ImageConfig {
    int width;          ///< Ancho de la imagen en píxeles
    int height;         ///< Alto de la imagen en píxeles
    double aspect_ratio; ///< Relación de aspecto (width/height)
};

struct Camera {
    point3 center;        ///< Posición de la cámara
    vec3   viewport_u;    ///< Vector horizontal del viewport
    vec3   viewport_v;    ///< Vector vertical del viewport
    vec3   pixel_delta_u; ///< Incremento horizontal por píxel
    vec3   pixel_delta_v; ///< Incremento vertical por píxel
    point3 pixel00_loc;   ///< Coordenadas del primer píxel (arriba-izquierda)
};

// Función que calcula un color base para un rayo
/**
 * @brief Calcula un color interpolando entre c1 y c2 según la dirección del rayo.
 * @param r Rayo que se está disparando.
 * @param c1 Color inicial (por defecto blanco).
 * @param c2 Color final (por defecto azul claro).
 * @return Color interpolado basado en la componente y de la dirección del rayo.
 */
color ray_color(const ray& r, const color& c1 = color(1.0, 1.0, 1.0), const color& c2 = color(0.5, 0.7, 1.0)) {
    vec3 unit_direction = unit_vector(r.direction());
    auto a = 0.5*(unit_direction.y() + 1.0);
    return (1.0 - a) * c1 + a * c2;
}

// Configuracion de imagen
ImageConfig make_image_config(int width, double aspect_ratio) {
    int height = static_cast<int>(width / aspect_ratio);
    if (height < 1) height = 1;
    return {width, height, aspect_ratio};
}

// Creacion de camara 
Camera make_camera(const ImageConfig& img, double viewport_height, double focal_length) {
    double viewport_width = viewport_height * (double(img.width) / img.height);

    auto center = point3(0, 0, 0);
    auto viewport_u = vec3(viewport_width, 0, 0);
    auto viewport_v = vec3(0, -viewport_height, 0);

    auto pixel_delta_u = viewport_u / img.width;
    auto pixel_delta_v = viewport_v / img.height;

    auto viewport_upper_left = center - vec3(0,0,focal_length)
                                      - viewport_u/2
                                      - viewport_v/2;

    auto pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

    return {center, viewport_u, viewport_v, pixel_delta_u, pixel_delta_v, pixel00_loc};
}

// ==========================================
// FUNCIONES QUE EL ALUMNO DEBE IMPLEMENTAR
// ==========================================

/**
 * @brief Ajusta el brillo de un color según la dirección del rayo.
 * @param r Rayo actual.
 * @param c Color base que se va a ajustar.
 * @return Color ajustado según la componente y del rayo (0 = más oscuro, 1 = más brillante).
 */
color adjust_brightness(const ray& r, const color& c) {
    // TODO: implementar
}

/**
 * @brief Mezcla dos colores según la posición del píxel en la imagen.
 * @param c1 Primer color.
 * @param c2 Segundo color.
 * @param i Coordenada x del píxel.
 * @param j Coordenada y del píxel.
 * @param image_width Ancho total de la imagen.
 * @param image_height Alto total de la imagen.
 * @return Color interpolado entre c1 y c2 usando t = (u+v)/2, donde u = i/(width-1), v = j/(height-1).
 */
color blend(const color& c1, const color& c2, int i, int j, int image_width, int image_height) {
    // TODO: implementar
}

/**
 * @brief Crea un gradiente cuadrante usando cuatro colores en las esquinas de la imagen.
 * @param i Coordenada x del píxel.
 * @param j Coordenada y del píxel.
 * @param image_width Ancho de la imagen.
 * @param image_height Alto de la imagen.
 * @return Color interpolado usando los cuatro colores de esquina:
 *         c00 = abajo-izquierda (1,0,0)
 *         c10 = abajo-derecha   (0,1,0)
 *         c01 = arriba-izquierda(0,0,1)
 *         c11 = arriba-derecha  (1,1,0)
 */
color quadrant_blend(int i, int j, int image_width, int image_height) {
    // TODO: implementar
}

// Renderizado
/**
 * @brief Renderiza la imagen en formato PPM según el modo seleccionado.
 * @param img Configuración de la imagen (width, height, aspect_ratio).
 * @param cam Configuración de la cámara.
 * @param mode Modo de renderizado: "brightness", "blend", "quadrant" o "default".
 */
void render(const ImageConfig& img, const Camera& cam, const std::string& mode) {
    std::cout << "P3\n" << img.width << " " << img.height << "\n255\n";

    for (int j = 0; j < img.height; j++) {
        std::clog << "\rScanlines remaining: " << (img.height - j) << ' ' << std::flush;
        for (int i = 0; i < img.width; i++) {

            auto pixel_center = cam.pixel00_loc + (i * cam.pixel_delta_u) + (j * cam.pixel_delta_v);
            auto ray_direction = pixel_center - cam.center;
            ray r(cam.center, ray_direction);

            // Color base calculado para el rayo
            color base_color = ray_color(r);

            // Selecciona el color según el modo
            color pixel_color;
            if (mode == "brightness") {
                pixel_color = adjust_brightness(r, base_color);
            } else if (mode == "blend") {
                pixel_color = blend(base_color, color(0,0,0), i, j, img.width, img.height);
            } else if (mode == "quadrant") {
                pixel_color = quadrant_blend(i, j, img.width, img.height);
            } else {
                pixel_color = base_color;
            }

            write_color(std::cout, pixel_color);
        }
    }
    std::clog << "\rDone.                 \n";
}

// ==========================================
// Función principal
// ==========================================
int main(int argc, char* argv[]) {
    std::string mode = "default"; ///< Modo por defecto

    if (argc >= 2) {
        mode = argv[1]; ///< Modo pasado por línea de comandos
    }

    auto img = make_image_config(400, 16.0/9.0);
    auto cam = make_camera(img, 2.0, 1.0);

    render(img, cam, mode);

    return 0;
}
