#ifndef __GPS_KALMAN_H__
#define __GPS_KALMAN_H__

#include "MatrixMath.h"
#include <math.h>

const uint64_t Rearth	= 6378137;

const float Matrix_R[2][2] = {
  {2.21017383364137, 3.51637078682249},
  {3.51637078682249, 13.8032720993553},
};
const float Matrix_Q[4][4] = {
  {0.01,    0,       0,      0    },
  {0,       0.01,    0,      0    },
  {0,       0,       0.001,  0    },
  {0,       0,       0,      0.001},
};

const float Matrix_H[2][4] = {
  {1.0, 0.0, 0.0, 0.0},
  {0.0, 1.0, 0.0, 0.0},
};
const float Matrix_H_Trans[4][2] = {
  {1.0, 0.0},
  {0.0, 1.0},
  {0.0, 0.0},
  {0.0, 0.0},
};

const float Matrix_I[4][4] = {
  {1, 0, 0, 0},
  {0, 1, 0, 0},
  {0, 0, 1, 0},
  {0, 0, 0, 1},
};

void GPS_update_kalman();

#endif
