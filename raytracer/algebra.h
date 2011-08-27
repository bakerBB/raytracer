#ifndef _alebra_h_
#define _alebra_h_

typedef struct {
  double x,y,z;
} point3_t, vector3_t;

typedef struct {
  point3_t  origin;
  vector3_t vector;
} ray3_t;

typedef struct {
  point3_t  center;
  double    radius;
} sphere_t;


void normalize_vector(vector3_t* vector);
void scale_vector(vector3_t* vector_ptr, double scale);
double calc_dot_product(vector3_t* Vin1, vector3_t* Vin2);
void calc_reflected_vector(vector3_t* Rout, vector3_t* Rin, vector3_t* N);
int find_ray_sphere_intersect(ray3_t* ray, sphere_t* sphere, double* ray_length_ptr);

#endif /* _alebra_h_ */
