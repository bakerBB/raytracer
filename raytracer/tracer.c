/*
 * Reference Page(s):
 * http://www.codermind.com/articles/Raytracer-in-C++-Introduction-What-is-ray-tracing.html
 * http://www.siggraph.org/education/materials/HyperGraph/raytrace/rtrace0.htm
 */

#include <stdint.h>
#include "algebra.h"
#include "bitmap.h"
#include "color.h"
#include "objects.h"
#include "tracer.h"
#include "utility.h"

static colored_sphere_t sphere_list[] = {
  { {{+000.0, +000.0, +120.0}, +020.0}, {0.3412, 0.2667, 1.0000, 1.0000}},
  { {{-100.0, +000.0, +200.0}, +060.0}, {0.5098, 1.0000, 0.2275, 1.0000}},
  { {{+100.0, +000.0, +200.0}, +060.0}, {1.0000, 0.3529, 0.2392, 1.0000}}
};

static light_t light_list[] = {
  {{+000.0, +400.0, -100.0}},
  {{+000.0, -400.0, -100.0}},
  {{+400.0, +400.0, +000.0}}
};

static int sphere_list_count = NELEMENTS(sphere_list);
static int light_list_count = NELEMENTS(light_list);
static point3_t eye_origin = {0.0, 0.0, 0.0};

#define frame_width 1024
#define frame_height 768
bmp_pixel32_t frame_buffer[frame_height][frame_width];

/*******************************************************************************
 * A primary ray is shot through each pixel and tested for intersection against
 * all objects in the scene. If there is an intersection with an object then
 * several other rays are generated. Shadow rays are sent towards all light
 * sources to determine if any objects occlude the intersection spot. In the
 * figure below, the shadow rays are labeled Si and are sent towards the two
 * light sources LA and LB. If the surface is reflective then a reflected ray,
 * Ri, is generated. If the surface is not opaque, then a transmitted ray, Ti,
 * is generated. Each of the secondary rays is tested against all the objects
 * in the scene.
 *
 * Definitions:
 * occlude: To obstruct, cover, or otherwise block an opening.
 * opaque: Impenetrable to light.  Neither reflecting nor emitting light. */

/*******************************************************************************
 * For each pixel we must generate a primary ray and test for intersection with
 * all of the objects in the scene. If there is more than one ray-object
 * intersection then we must choose the closest intersection (the smallest
 * positive value of t).To ensure that there are no objects intersected in
 * front of the image plane (this is called near plane clipping), we keep the
 * distance of the primary ray to the screen and test all intersections against
 * this distance. If the t value is less than this distance, then we ignore the
 * object.
 *
 * If there is an intersection then we must compute the shadow rays and the
 * reflection rays. */
color_t trace_ray(ray3_t* test_ray, int depth) {
  int    occluded = 0;
  ray3_t shadow_ray;
  ray3_t reflected_ray;
  color_t ray_color = { 0.0, 0.0, 0.0, 0.0 };
  color_t nul_color = {0.0, 0.0, 0.0, 1.0};
  light_t* light_ptr = 0;
  double intersect_ray_length_curr = 0.0;
  double intersect_ray_length_close = 0.0;
  int     intersection_exists = 0;
  colored_sphere_t* sphere_close_ptr = 0;
  vector3_t intersection;
  vector3_t intersection_vector;
  int sphere_index = 0;
  int light_index = 0;
  int shadow_index;
  color_t add_color;

  /* test ray against all objects in scene, search until closest intersection
   * is found. */
  for (sphere_index = 0; sphere_index < sphere_list_count; ++sphere_index) {
    colored_sphere_t* sphere_ptr = &sphere_list[sphere_index];

    /* test for intersection between ray and scene object */
    if (1 == find_ray_sphere_intersect(test_ray, &sphere_ptr->sphere, &intersect_ray_length_curr)) {
      if (intersect_ray_length_curr < 0.0)
        continue;

      if (0 == intersection_exists) {
        intersection_exists = 1;
        intersect_ray_length_close = intersect_ray_length_curr;
        sphere_close_ptr = sphere_ptr;
      }
      else if (intersect_ray_length_curr < intersect_ray_length_close) {
        intersect_ray_length_curr = intersect_ray_length_close;
        sphere_close_ptr = sphere_ptr;
      }
    }
  }

  if (1 == intersection_exists) {
    intersection.x = test_ray->origin.x + test_ray->vector.x * intersect_ray_length_close;
    intersection.y = test_ray->origin.y + test_ray->vector.y * intersect_ray_length_close;
    intersection.z = test_ray->origin.z + test_ray->vector.z * intersect_ray_length_close;

    shadow_ray.origin = intersection;

    /* Shadow rays are sent towards all light sources to determine if any objects occlude the intersection spot. */
    for (light_index = 0; light_index < light_list_count; ++light_index) {
      occluded = 0;
      light_ptr = &light_list[light_index];
      shadow_ray.vector.x = (light_ptr->origin.x - shadow_ray.origin.x);
      shadow_ray.vector.y = (light_ptr->origin.y - shadow_ray.origin.y);
      shadow_ray.vector.z = (light_ptr->origin.z - shadow_ray.origin.z);
      normalize_vector(&shadow_ray.vector);

      /* Test intersection to determine if ray is in occlusion. */
      for (shadow_index = 0; shadow_index < sphere_list_count; ++shadow_index) {
        if (1 == find_ray_sphere_intersect(&shadow_ray, &sphere_list[shadow_index].sphere, &intersect_ray_length_curr)) {
          occluded = 1;
          break;
        }
      }

      if (occluded == 0) {
        /* Calculate the total light intensity. */
        double light_intensity = calc_dot_product(&shadow_ray.vector, &test_ray->vector);
        if (light_intensity < 0.0) light_intensity = -light_intensity;

        ray_color = mix_colors(&nul_color, &sphere_close_ptr->color, light_intensity);

        if (depth < 4) {
          reflected_ray.origin.x = intersection.x;
          reflected_ray.origin.y = intersection.y;
          reflected_ray.origin.z = intersection.z;

          intersection_vector.x = (intersection.x - sphere_close_ptr->sphere.center.x);
          intersection_vector.y = (intersection.y - sphere_close_ptr->sphere.center.y);
          intersection_vector.z = (intersection.z - sphere_close_ptr->sphere.center.z);

          normalize_vector(&intersection_vector);
          calc_reflected_vector(&reflected_ray.vector, &test_ray->vector, &intersection_vector);

          add_color = trace_ray(&reflected_ray, ++depth);
          if ((add_color.r != 0x00) ||
              (add_color.g != 0x00) ||
              (add_color.b != 0x00)) {
            double mix_ratio = calc_dot_product(&reflected_ray.vector, &intersection_vector);
            if (mix_ratio < 0.0) mix_ratio = -mix_ratio;
            ray_color = mix_colors(&ray_color, &add_color, mix_ratio);
          }
        }
      }
    }
  }

  return ray_color;
}

int ray_trace(char* filename) {
  ray3_t primary_ray;
  int frame_z = 300;
  int frame_x = 0;
  int frame_y = 0;
  color_t hicolor;

  for (frame_x = 0; frame_x < frame_width; ++frame_x) {
    for (frame_y = 0; frame_y < frame_height; ++frame_y) {

      /* Primary Ray (test ray) */
      primary_ray.origin = eye_origin;
      primary_ray.vector.x = (frame_x - (frame_width/2) - primary_ray.origin.x);
      primary_ray.vector.y = (frame_y - (frame_height/2) - primary_ray.origin.y);
      primary_ray.vector.z = (frame_z - primary_ray.origin.z);
      normalize_vector(&primary_ray.vector);

      hicolor = trace_ray(&primary_ray, 0);
      frame_buffer[frame_y][frame_x].r = (uint8_t) (hicolor.r * 0xFF);
      frame_buffer[frame_y][frame_x].g = (uint8_t) (hicolor.g * 0xFF);
      frame_buffer[frame_y][frame_x].b = (uint8_t) (hicolor.b * 0xFF);
      frame_buffer[frame_y][frame_x].a = (uint8_t) (hicolor.a * 0xFF);
    }
  }

  write_bmp(filename, frame_width, frame_height, frame_buffer);
  return 0;
}
