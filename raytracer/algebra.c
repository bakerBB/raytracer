#include <math.h>
#include "algebra.h"
#include "utility.h"

void normalize_vector(vector3_t* vector) {
  double discriminant =
    (vector->x * vector->x) +
    (vector->y * vector->y) +
    (vector->z * vector->z);

  if (discriminant != 1.0) {
    double magnitude = sqrt(discriminant);
    vector->x /= magnitude;
    vector->y /= magnitude;
    vector->z /= magnitude;
  }
}

void scale_vector(vector3_t* vector_ptr, double scale) {
  vector_ptr->x *= scale;
  vector_ptr->y *= scale;
  vector_ptr->z *= scale;
}

double calc_dot_product(vector3_t* Vin1, vector3_t* Vin2) {
  return ((Vin1->x * Vin2->x) + (Vin1->y * Vin2->y) + (Vin1->z * Vin2->z));
}

void calc_reflected_vector(vector3_t* Rout, vector3_t* Rin, vector3_t* N) {
  double dot_product = calc_dot_product(Rin, N);
  Rout->x = Rin->x - (2 * dot_product * N->x);
  Rout->y = Rin->y - (2 * dot_product * N->y);
  Rout->z = Rin->z - (2 * dot_product * N->z);
}

/* Resource: http://www.siggraph.org/education/materials/HyperGraph/raytrace/rtinter1.htm */
int find_ray_sphere_intersect(ray3_t* ray, sphere_t* sphere, double* ray_length_ptr)
{
  int    bIntersectionExists = 0;
  double discriminant = 0.0;
  double length1 = 0.0;
  double length2 = 0.0;
  double temp = 0.0;

  /* Ray origin. */
  double Rox = ray->origin.x;
  double Roy = ray->origin.y;
  double Roz = ray->origin.z;

  /* Ray vector. */
  double Rdx = ray->vector.x;
  double Rdy = ray->vector.y;
  double Rdz = ray->vector.z;

  /* Sphere center. */
  double Scx = sphere->center.x;
  double Scy = sphere->center.y;
  double Scz = sphere->center.z;

  /* Sphere radius. */
  double Sr = sphere->radius;

  /* Intermediate variables. */
  /*double A = pow(Rdx,2) + pow(Rdy,2) + pow(Rdz, 2);*/
  double B = 2.0 * (Rdx * (Rox - Scx) + Rdy * (Roy - Scy) + Rdz * (Roz - Scz));
  double C = pow((Rox - Scx), 2) + pow((Roy - Scy), 2) + pow((Roz - Scz), 2) - pow(Sr,2);

  /* Determine if intersection exists. */
  if (!ISTINY(C)) {
    /*discriminant = pow(B,2) - 4*A*C;  */
    discriminant = B*B - 4*C;
    if (discriminant > 0.0) {
      bIntersectionExists = 1;
    }
  }

  /* Need to only calculate the closest intersection.
   * t0, t1 = (- B + (B^2 - 4*C)^1/2) / 2 where t0 is for (-) and t1 is for (+)
   */
  if (0 != bIntersectionExists) {
    /* Calculate closest intersection location. */
    temp = sqrt(discriminant);
    length1 = 0.5 * (-B + temp);
    length2 = 0.5 * (-B - temp);

    /* Intersection point. */
    *ray_length_ptr = MIN(length1, length2);
  }

  return bIntersectionExists;
}
