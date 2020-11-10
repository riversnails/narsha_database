#include <avr/pgmspace.h>
// #define X_DIR 0x20 // c
// #define X_STEP 0x80 // d
// #define X_STOP 0x04 // c 2

// #define Y_DIR 0x80 // c
// #define Y_STEP 0x40 // c
// #define Y_STOP 0x08 // c 3

// #define E_DIR 0x01 // b
// #define E_STEP 0x02 // b

// #define XYEENABLE 0x40 // d

// #define Z_DIR 0x04 // b
// #define Z_STEP 0x08 // b
// #define Z_STOP 0x10 // c
// #define ZENABLE 0x20 // a

//-------------------------------------
#define X_STEP 0x01 // f
#define X_DIR 0x02 // f
#define X_ENABLE 0x80 // d  
//#define X_STOP

#define Y_STEP 0x40 // f
#define Y_DIR 0x80 // f
#define Y_ENABLE 0x04 // f

#define Z_STEP 0x08 // l
#define Z_DIR 0x01 // l
#define Z_ENABLE 0x01 // k

#define E_STEP 0x10 // a
#define E_DIR 0x40 // a
#define E_ENABLE 0x02 // a

#define E1_STEP 0x02 // c
#define E1_DIR 0x08 // c
#define E1_ENABLE 0x80 // c
//-------------------------------------

#define ONE_MM 80
#define ONE_CM 800
#define Z_ONE_MM 400
#define Z_ONE_CM 4000

#define X_LEFT PORTF & ~(X_DIR)
#define X_RIGHT PORTF | X_DIR
#define Y_UP PORTF & ~(Y_DIR)
#define Y_DOWN PORTF | Y_DIR
#define Z_UP PORTL & ~(Z_DIR)
#define Z_DOWN PORTL | Z_DIR

enum { // 열거형  날자 표현할때 자주 사용 0,1,2,3이런식으로 나옴  define 대용
  x_left, x_right, y_up, y_down, z_up, z_down
};

volatile char x_reset = 0;
volatile int x_distance = 0;

volatile char y_reset = 0;
volatile int y_distance = 0;

volatile char z_reset = 0;
volatile int z_distance = 0;

volatile double currunt_x = 0; // 현재 좌표 && mm 단위로 봄
volatile double currunt_y = 0;

volatile int xz_changer = 0;

const float PROGMEM xy_pos[911][2] = {
  { 66.268, 82.377, }, { 66.527, 82.217, }, { 66.796, 82.073, }, { 67.072, 81.946, },
  { 67.356, 81.836, }, { 67.647, 81.743, }, { 67.942, 81.669, },
  { 68.241, 81.613, }, { 68.544, 81.576, }, { 68.848, 81.557, },
  { 131.152, 81.557, }, { 131.456, 81.576, }, { 131.759, 81.613, },
  { 132.058, 81.669, }, { 132.353, 81.743, }, { 132.644, 81.836, },
  { 132.928, 81.946, }, { 133.204, 82.073, }, { 133.473, 82.217, },
  { 133.732, 82.377, }, { 133.98, 82.553, }, { 134.218, 82.744, },
  { 134.443, 82.95, }, { 134.655, 83.168, }, { 134.853, 83.4, },
  { 135.036, 83.643, }, { 135.205, 83.897, }, { 135.357, 84.16, },
  { 135.493, 84.433, }, { 135.611, 84.714, }, { 135.713, 85.001, },
  { 135.796, 85.294, }, { 135.861, 85.591, }, { 135.908, 85.892, },
  { 135.936, 86.196, }, { 135.945, 86.5, }, { 135.936, 86.804, },
  { 135.908, 87.108, }, { 135.861, 87.409, }, { 135.796, 87.706, },
  { 135.713, 87.999, }, { 135.611, 88.286, }, { 135.493, 88.567, },
  { 135.357, 88.84, }, { 135.205, 89.103, }, { 135.036, 89.357, },
  { 134.853, 89.6, }, { 134.664, 89.821, }, { 134.225, 90.251, },
  { 103.227, 117.248, }, { 102.98, 117.446, }, { 102.732, 117.622, },
  { 102.473, 117.783, }, { 102.204, 117.927, }, { 101.928, 118.054, },
  { 101.644, 118.164, }, { 101.353, 118.257, }, { 101.058, 118.331, },
  { 100.759, 118.387, }, { 100.456, 118.424, }, { 100.152, 118.443, },
  { 99.848, 118.443, }, { 99.544, 118.424, }, { 99.241, 118.387, },
  { 98.942, 118.331, }, { 98.647, 118.257, }, { 98.356, 118.164, },
  { 98.072, 118.054, }, { 97.796, 117.927, }, { 97.527, 117.783, },
  { 97.268, 117.622, }, { 97.021, 117.447, }, { 96.737, 117.217, },
  { 65.775, 90.251, }, { 65.336, 89.821, }, { 65.147, 89.6, },
  { 64.964, 89.357, }, { 64.795, 89.103, }, { 64.643, 88.84, },
  { 64.507, 88.567, }, { 64.389, 88.286, }, { 64.287, 87.999, },
  { 64.204, 87.706, }, { 64.139, 87.409, }, { 64.092, 87.108, },
  { 64.064, 86.804, }, { 64.055, 86.5, }, { 64.064, 86.196, },
  { 64.092, 85.892, }, { 64.139, 85.591, }, { 64.204, 85.294, },
  { 64.287, 85.001, }, { 64.389, 84.714, }, { 64.507, 84.433, },
  { 64.643, 84.16, }, { 64.795, 83.897, }, { 64.964, 83.643, },
  { 65.147, 83.4, }, { 65.345, 83.168, }, { 65.557, 82.95, },
  { 65.782, 82.745, }, { 66.02, 82.553, }, { 65.586, 81.979, },
  { 65.87, 81.777, }, { 66.167, 81.593, }, { 66.475, 81.428, },
  { 66.792, 81.282, }, { 67.117, 81.156, }, { 67.449, 81.051, },
  { 67.788, 80.965, }, { 68.131, 80.901, }, { 68.477, 80.858, },
  { 68.826, 80.837, }, { 131.174, 80.837, }, { 131.523, 80.858, },
  { 131.869, 80.901, }, { 132.212, 80.965, }, { 132.551, 81.051, },
  { 132.883, 81.156, }, { 133.208, 81.282, }, { 133.525, 81.428, },
  { 133.833, 81.593, }, { 134.13, 81.777, }, { 134.414, 81.979, },
  { 134.686, 82.197, }, { 134.944, 82.433, }, { 135.187, 82.683, },
  { 135.414, 82.948, }, { 135.624, 83.227, }, { 135.817, 83.517, },
  { 135.992, 83.82, }, { 136.147, 84.132, }, { 136.283, 84.453, },
  { 136.399, 84.782, }, { 136.495, 85.118, }, { 136.569, 85.459, },
  { 136.623, 85.804, }, { 136.655, 86.151, }, { 136.666, 86.5, },
  { 136.655, 86.849, }, { 136.623, 87.196, }, { 136.569, 87.541, },
  { 136.495, 87.882, }, { 136.399, 88.218, }, { 136.283, 88.547, },
  { 136.147, 88.868, }, { 135.992, 89.18, }, { 135.817, 89.483, },
  { 135.624, 89.773, }, { 135.414, 90.052, }, { 135.19, 90.313, },
  { 134.714, 90.78, }, { 103.689, 117.8, }, { 103.414, 118.021, },
  { 103.13, 118.223, }, { 102.833, 118.407, }, { 102.525, 118.572, },
  { 102.208, 118.718, }, { 101.883, 118.844, }, { 101.551, 118.949, },
  { 101.212, 119.035, }, { 100.869, 119.099, }, { 100.523, 119.142, },
  { 100.174, 119.163, }, { 99.826, 119.163, }, { 99.477, 119.142, },
  { 99.131, 119.099, }, { 98.788, 119.035, }, { 98.449, 118.949, },
  { 98.117, 118.844, }, { 97.792, 118.718, }, { 97.475, 118.572, },
  { 97.167, 118.407, }, { 96.87, 118.223, }, { 96.586, 118.022, },
  { 96.274, 117.769, }, { 65.286, 90.78, }, { 64.81, 90.313, },
  { 64.586, 90.052, }, { 64.376, 89.773, }, { 64.183, 89.483, },
  { 64.008, 89.18, }, { 63.853, 88.868, }, { 63.717, 88.547, },
  { 63.601, 88.218, }, { 63.505, 87.882, }, { 63.431, 87.541, },
  { 63.377, 87.196, }, { 63.345, 86.849, }, { 63.334, 86.5, },
  { 63.345, 86.151, }, { 63.377, 85.804, }, { 63.431, 85.459, },
  { 63.505, 85.118, }, { 63.601, 84.782, }, { 63.717, 84.453, },
  { 63.853, 84.132, }, { 64.008, 83.82, }, { 64.183, 83.517, },
  { 64.376, 83.227, }, { 64.586, 82.948, }, { 64.813, 82.683, },
  { 65.056, 82.433, }, { 65.314, 82.197, }, { 65.586, 81.979, },
  { 68.304, 85.181, }, { 68.037, 85.535, }, { 67.847, 85.916, },
  { 67.731, 86.326, }, { 67.691, 86.75, }, { 67.731, 87.174, },
  { 67.847, 87.584, }, { 68.037, 87.965, }, { 68.304, 88.319, },
  { 68.71, 88.667, }, { 69.1, 88.876, }, { 69.506, 89.005, },
  { 69.896, 89.054, }, { 130.104, 89.054, }, { 130.494, 89.005, },
  { 130.9, 88.876, }, { 131.275, 88.675, }, { 131.607, 88.407, },
  { 131.884, 88.084, }, { 132.097, 87.715, }, { 132.239, 87.313, },
  { 132.304, 86.892, }, { 132.291, 86.466, }, { 132.196, 86.034, },
  { 131.973, 85.548, }, { 131.706, 85.195, }, { 131.391, 84.908, },
  { 131.029, 84.683, }, { 130.632, 84.529, }, { 130.246, 84.457, },
  { 130.071, 84.446, }, { 69.896, 84.446, }, { 69.506, 84.495, },
  { 69.1, 84.624, }, { 68.71, 84.833, }, { 68.304, 85.181, },
  { 68.832, 85.677, }, { 68.652, 85.915, }, { 68.521, 86.177, },
  { 68.441, 86.459, }, { 68.414, 86.75, }, { 68.441, 87.041, },
  { 68.521, 87.323, }, { 68.652, 87.585, }, { 68.832, 87.823, },
  { 69.119, 88.069, }, { 69.382, 88.21, }, { 69.661, 88.299, },
  { 69.94, 88.334, }, { 130.06, 88.334, }, { 130.339, 88.299, },
  { 130.618, 88.21, }, { 130.876, 88.072, }, { 131.104, 87.888, },
  { 131.294, 87.666, }, { 131.44, 87.413, }, { 131.538, 87.137, },
  { 131.583, 86.848, }, { 131.574, 86.555, }, { 131.51, 86.264, },
  { 131.352, 85.92, }, { 131.172, 85.682, }, { 130.956, 85.485, },
  { 130.707, 85.331, }, { 130.434, 85.225, }, { 130.157, 85.173, },
  { 130.049, 85.166, }, { 69.94, 85.166, }, { 69.661, 85.201, },
  { 69.382, 85.29, }, { 69.119, 85.431, }, { 68.832, 85.677, },
  { 93.152, 100.76, }, { 92.904, 101.189, }, { 92.683, 101.633, },
  { 92.489, 102.09, }, { 92.324, 102.558, }, { 92.189, 103.035, },
  { 92.083, 103.52, }, { 92.006, 104.01, }, { 91.961, 104.504, },
  { 91.945, 105, }, { 91.961, 105.496, }, { 92.006, 105.99, },
  { 92.083, 106.48, }, { 92.189, 106.965, }, { 92.324, 107.442, },
  { 92.489, 107.91, }, { 92.683, 108.367, }, { 92.904, 108.811, },
  { 93.152, 109.24, }, { 93.426, 109.654, }, { 93.725, 110.05, },
  { 94.048, 110.426, }, { 94.393, 110.783, }, { 94.76, 111.117, },
  { 95.146, 111.428, }, { 95.551, 111.714, }, { 95.973, 111.975, },
  { 96.41, 112.21, }, { 96.86, 112.418, }, { 97.323, 112.597, },
  { 97.796, 112.747, }, { 98.277, 112.868, }, { 98.765, 112.959, },
  { 99.257, 113.02, }, { 99.752, 113.051, }, { 100.248, 113.051, },
  { 100.743, 113.02, }, { 101.235, 112.959, }, { 101.723, 112.868, },
  { 102.204, 112.747, }, { 102.677, 112.597, }, { 103.14, 112.418, },
  { 103.59, 112.21, }, { 104.027, 111.975, }, { 104.449, 111.714, },
  { 104.854, 111.428, }, { 105.24, 111.117, }, { 105.607, 110.783, },
  { 105.952, 110.426, }, { 106.275, 110.05, }, { 106.574, 109.654, },
  { 106.848, 109.24, }, { 107.096, 108.811, }, { 107.317, 108.367, },
  { 107.511, 107.91, }, { 107.676, 107.442, }, { 107.811, 106.965, },
  { 107.917, 106.48, }, { 107.994, 105.99, }, { 108.039, 105.496, },
  { 108.055, 105, }, { 108.039, 104.504, }, { 107.994, 104.01, },
  { 107.917, 103.52, }, { 107.811, 103.035, }, { 107.676, 102.558, },
  { 107.511, 102.09, }, { 107.317, 101.633, }, { 107.096, 101.189, },
  { 106.848, 100.76, }, { 106.574, 100.346, }, { 106.275, 99.95, },
  { 105.952, 99.574, }, { 105.607, 99.217, }, { 105.24, 98.883, },
  { 104.854, 98.572, }, { 104.449, 98.286, }, { 104.027, 98.025, },
  { 103.59, 97.79, }, { 103.14, 97.582, }, { 102.677, 97.403, },
  { 102.204, 97.253, }, { 101.723, 97.132, }, { 101.235, 97.041, },
  { 100.743, 96.98, }, { 100.248, 96.949, }, { 99.752, 96.949, },
  { 99.257, 96.98, }, { 98.765, 97.041, }, { 98.277, 97.132, },
  { 97.796, 97.253, }, { 97.323, 97.403, }, { 96.86, 97.582, },
  { 96.41, 97.79, }, { 95.973, 98.025, }, { 95.551, 98.286, },
  { 95.146, 98.572, }, { 94.76, 98.883, }, { 94.393, 99.217, },
  { 94.048, 99.574, }, { 93.725, 99.95, }, { 93.426, 100.346, },
  { 93.152, 100.76, }, { 93.764, 101.139, }, { 93.538, 101.53, },
  { 93.337, 101.935, }, { 93.161, 102.351, }, { 93.011, 102.777, },
  { 92.887, 103.211, }, { 92.791, 103.652, }, { 92.721, 104.099, },
  { 92.68, 104.548, }, { 92.666, 105, }, { 92.68, 105.451, },
  { 92.721, 105.901, }, { 92.791, 106.348, }, { 92.887, 106.789, },
  { 93.011, 107.223, }, { 93.161, 107.649, }, { 93.337, 108.065, },
  { 93.538, 108.47, }, { 93.764, 108.861, }, { 94.014, 109.238, },
  { 94.286, 109.598, }, { 94.58, 109.941, }, { 94.894, 110.265, },
  { 95.228, 110.57, }, { 95.58, 110.853, }, { 95.949, 111.114, },
  { 96.333, 111.352, }, { 96.731, 111.565, }, { 97.141, 111.754, },
  { 97.562, 111.917, }, { 97.993, 112.054, }, { 98.431, 112.164, },
  { 98.875, 112.247, }, { 99.323, 112.303, }, { 99.774, 112.331, },
  { 100.226, 112.331, }, { 100.677, 112.303, }, { 101.125, 112.247, },
  { 101.569, 112.164, }, { 102.007, 112.054, }, { 102.438, 111.917, },
  { 102.859, 111.754, }, { 103.269, 111.565, }, { 103.667, 111.352, },
  { 104.051, 111.114, }, { 104.42, 110.853, }, { 104.772, 110.57, },
  { 105.106, 110.265, }, { 105.42, 109.941, }, { 105.714, 109.598, },
  { 105.986, 109.238, }, { 106.236, 108.861, }, { 106.462, 108.47, },
  { 106.663, 108.065, }, { 106.839, 107.649, }, { 106.989, 107.223, },
  { 107.113, 106.789, }, { 107.209, 106.348, }, { 107.279, 105.901, },
  { 107.32, 105.451, }, { 107.334, 105, }, { 107.32, 104.548, },
  { 107.279, 104.099, }, { 107.209, 103.652, }, { 107.113, 103.211, },
  { 106.989, 102.777, }, { 106.839, 102.351, }, { 106.663, 101.935, },
  { 106.462, 101.53, }, { 106.236, 101.139, }, { 105.986, 100.762, },
  { 105.714, 100.402, }, { 105.42, 100.059, }, { 105.106, 99.735, },
  { 104.772, 99.43, }, { 104.42, 99.147, }, { 104.051, 98.886, },
  { 103.667, 98.648, }, { 103.269, 98.435, }, { 102.859, 98.246, },
  { 102.438, 98.083, }, { 102.007, 97.946, }, { 101.569, 97.836, },
  { 101.125, 97.752, }, { 100.677, 97.697, }, { 100.226, 97.669, },
  { 99.774, 97.669, }, { 99.323, 97.697, }, { 98.875, 97.752, },
  { 98.431, 97.836, }, { 97.993, 97.946, }, { 97.562, 98.083, },
  { 97.141, 98.246, }, { 96.731, 98.435, }, { 96.333, 98.648, },
  { 95.949, 98.886, }, { 95.58, 99.147, }, { 95.228, 99.43, },
  { 94.894, 99.735, }, { 94.58, 100.059, }, { 94.286, 100.402, },
  { 94.014, 100.762, }, { 93.764, 101.139, }, { 96.733, 113.024, },
  { 101.33, 117.622, }, { 101.44, 117.587, }, { 101.689, 117.49, },
  { 101.931, 117.379, }, { 102.045, 117.318, }, { 98.204, 113.477, },
  { 98.671, 113.564, }, { 99.2, 113.63, }, { 99.386, 113.641, },
  { 102.662, 116.917, }, { 102.834, 116.779, }, { 103.212, 116.449, },
  { 100.417, 113.654, }, { 100.8, 113.63, }, { 101.329, 113.564, },
  { 101.343, 113.562, }, { 103.756, 115.975, }, { 104.301, 115.501, },
  { 102.183, 113.384, }, { 102.372, 113.336, }, { 102.881, 113.174, },
  { 102.961, 113.143, }, { 104.845, 115.027, }, { 105.389, 114.553, },
  { 103.679, 112.843, }, { 103.863, 112.758, }, { 104.333, 112.506, },
  { 104.35, 112.496, }, { 105.933, 114.079, }, { 106.478, 113.605, },
  { 104.969, 112.096, }, { 105.223, 111.916, }, { 105.547, 111.656, },
  { 107.022, 113.131, }, { 107.566, 112.657, }, { 106.081, 111.173, },
  { 106.405, 110.839, }, { 106.572, 110.644, }, { 108.11, 112.183, },
  { 108.655, 111.709, }, { 107.022, 110.077, }, { 107.074, 110.008, },
  { 107.369, 109.563, }, { 107.426, 109.463, }, { 109.199, 111.235, },
  { 109.743, 110.761, }, { 107.784, 108.802, }, { 107.874, 108.622, },
  { 108.082, 108.131, }, { 108.094, 108.094, }, { 110.287, 110.287, },
  { 110.832, 109.813, }, { 108.345, 107.326, }, { 108.405, 107.114, },
  { 108.519, 106.593, }, { 108.534, 106.498, }, { 111.376, 109.339, },
  { 111.92, 108.865, }, { 108.645, 105.591, }, { 108.65, 105.534, },
  { 108.667, 105, }, { 108.654, 104.581, }, { 112.464, 108.391, },
  { 113.008, 107.917, }, { 108.523, 103.432, }, { 108.519, 103.407, },
  { 108.405, 102.886, }, { 108.259, 102.373, }, { 108.138, 102.028, },
  { 113.553, 107.443, }, { 114.097, 106.969, }, { 106.931, 99.804, },
  { 132.895, 87.075, }, { 134.651, 88.831, }, { 134.537, 89.003, },
  { 134.376, 89.217, }, { 134.217, 89.402, }, { 134.211, 89.408, },
  { 132.687, 87.884, }, { 132.656, 87.972, }, { 132.386, 88.439, },
  { 132.311, 88.527, }, { 133.689, 89.905, }, { 133.145, 90.379, },
  { 131.802, 89.036, }, { 131.615, 89.187, }, { 131.172, 89.425, },
  { 132.601, 90.853, }, { 132.057, 91.327, }, { 130.367, 89.638, },
  { 130.142, 89.666, }, { 129.377, 89.666, }, { 131.512, 91.801, },
  { 130.968, 92.275, }, { 128.358, 89.666, }, { 127.34, 89.666, },
  { 130.424, 92.749, }, { 129.88, 93.223, }, { 126.322, 89.666, },
  { 125.304, 89.666, }, { 129.336, 93.697, }, { 128.791, 94.171, },
  { 124.285, 89.666, }, { 123.267, 89.666, }, { 128.247, 94.645, },
  { 127.703, 95.119, }, { 122.249, 89.666, }, { 121.231, 89.666, },
  { 127.159, 95.593, }, { 126.614, 96.067, }, { 120.213, 89.666, },
  { 119.194, 89.666, }, { 126.07, 96.541, }, { 125.526, 97.015, },
  { 118.176, 89.666, }, { 117.158, 89.666, }, { 124.982, 97.489, },
  { 124.437, 97.963, }, { 116.14, 89.666, }, { 115.121, 89.666, },
  { 123.893, 98.437, }, { 123.349, 98.911, }, { 114.103, 89.666, },
  { 113.085, 89.666, }, { 122.805, 99.385, }, { 122.26, 99.859, },
  { 112.067, 89.666, }, { 111.048, 89.666, }, { 121.716, 100.333, },
  { 121.172, 100.807, }, { 110.03, 89.666, }, { 109.012, 89.666, },
  { 120.628, 101.281, }, { 120.084, 101.755, }, { 107.994, 89.666, },
  { 106.976, 89.666, }, { 119.539, 102.229, }, { 118.995, 102.703, },
  { 105.957, 89.666, }, { 104.939, 89.666, }, { 118.451, 103.177, },
  { 117.907, 103.651, }, { 103.921, 89.666, }, { 102.903, 89.666, },
  { 117.362, 104.125, }, { 116.818, 104.599, }, { 101.884, 89.666, },
  { 100.866, 89.666, }, { 116.274, 105.073, }, { 115.73, 105.547, },
  { 99.848, 89.666, }, { 98.83, 89.666, }, { 115.185, 106.021, },
  { 114.641, 106.495, }, { 97.811, 89.666, }, { 96.793, 89.666, },
  { 105.183, 98.055, }, { 104.787, 97.775, }, { 104.333, 97.494, },
  { 103.863, 97.242, }, { 103.378, 97.019, }, { 102.97, 96.86, },
  { 95.775, 89.666, }, { 94.757, 89.666, }, { 101.572, 96.481, },
  { 101.329, 96.436, }, { 100.8, 96.37, }, { 100.42, 96.347, },
  { 93.738, 89.666, }, { 92.72, 89.666, }, { 99.412, 96.357, },
  { 99.2, 96.37, }, { 98.671, 96.436, }, { 98.503, 96.467, },
  { 91.702, 89.666, }, { 90.684, 89.666, }, { 97.671, 96.653, },
  { 97.628, 96.664, }, { 97.119, 96.826, }, { 96.908, 96.908, },
  { 89.666, 89.666, }, { 88.647, 89.666, }, { 96.196, 97.214, },
  { 96.137, 97.242, }, { 95.667, 97.494, }, { 95.538, 97.574, },
  { 87.629, 89.666, }, { 86.611, 89.666, }, { 94.925, 97.979, },
  { 94.777, 98.084, }, { 94.361, 98.418, }, { 94.353, 98.426, },
  { 85.593, 89.666, }, { 84.574, 89.666, }, { 93.829, 98.92, },
  { 93.595, 99.161, }, { 93.344, 99.454, }, { 83.556, 89.666, },
  { 82.538, 89.666, }, { 92.902, 100.029, }, { 92.631, 100.437, },
  { 92.507, 100.653, }, { 81.52, 89.666, }, { 80.501, 89.666, },
  { 92.155, 101.319, }, { 92.126, 101.378, }, { 91.918, 101.869, },
  { 91.858, 102.04, }, { 79.483, 89.666, }, { 78.465, 89.666, },
  { 91.615, 102.815, }, { 91.595, 102.886, }, { 91.481, 103.407, },
  { 91.441, 103.66, }, { 77.447, 89.666, }, { 76.429, 89.666, },
  { 91.346, 104.583, }, { 91.333, 105, }, { 91.35, 105.534, },
  { 91.357, 105.612, }, { 75.41, 89.666, }, { 74.392, 89.666, },
  { 91.526, 106.799, }, { 91.595, 107.114, }, { 91.741, 107.627, },
  { 91.918, 108.131, }, { 91.976, 108.268, }, { 73.374, 89.666, },
  { 72.356, 89.666, }, { 100.493, 117.803, }, { 100.4, 117.815, },
  { 100.133, 117.831, }, { 99.867, 117.831, }, { 99.6, 117.815, },
  { 99.47, 117.799, }, { 71.337, 89.666, }, { 70.319, 89.666, },
  { 97.99, 117.337, }, { 97.834, 117.253, }, { 97.607, 117.112, },
  { 97.391, 116.959, }, { 97.13, 116.748, }, { 91.344, 111.709, },
  { 69.179, 89.543, }, { 67.67, 91.09, }, { 65.253, 88.673, },
  { 65.183, 88.55, }, { 65.064, 88.311, }, { 64.959, 88.065, },
  { 64.871, 87.814, }, { 64.798, 87.557, }, { 64.741, 87.296, },
  { 64.712, 87.113, }, { 75.562, 97.963, }, { 83.453, 104.836, },
  { 64.69, 86.073, }, { 64.7, 85.968, }, { 64.741, 85.704, },
  { 64.798, 85.443, }, { 64.86, 85.225, }, { 67.207, 87.572, },
  { 67.126, 87.287, }, { 67.077, 86.75, }, { 67.104, 86.451, },
  { 65.156, 84.503, }, { 65.183, 84.45, }, { 65.316, 84.219, },
  { 65.463, 83.997, }, { 65.552, 83.88, }, { 67.305, 85.633, },
  { 67.515, 85.211, }, { 67.681, 84.991, }, { 66.034, 83.344, },
  { 66.181, 83.21, }, { 66.389, 83.042, }, { 66.6, 82.892, },
  { 68.185, 84.477, }, { 68.362, 84.326, }, { 68.811, 84.084, },
  { 67.26, 82.533, }, { 67.311, 82.51, }, { 67.56, 82.413, },
  { 67.814, 82.332, }, { 68.024, 82.28, }, { 69.61, 83.865, },
  { 69.858, 83.834, }, { 70.597, 83.834, }, { 68.932, 82.169, },
  { 69.95, 82.169, }, { 71.616, 83.834, }, { 72.634, 83.834, },
  { 70.968, 82.169, }, { 71.987, 82.169, }, { 73.652, 83.834, },
  { 74.67, 83.834, }, { 73.005, 82.169, }, { 74.023, 82.169, },
  { 75.689, 83.834, }, { 76.707, 83.834, }, { 75.041, 82.169, },
  { 76.06, 82.169, }, { 77.725, 83.834, }, { 78.743, 83.834, },
  { 77.078, 82.169, }, { 78.096, 82.169, }, { 79.762, 83.834, },
  { 80.78, 83.834, }, { 79.114, 82.169, }, { 80.133, 82.169, },
  { 81.798, 83.834, }, { 82.816, 83.834, }, { 81.151, 82.169, },
  { 82.169, 82.169, }, { 83.834, 83.834, }, { 84.853, 83.834, },
  { 83.187, 82.169, }, { 84.205, 82.169, }, { 85.871, 83.834, },
  { 86.889, 83.834, }, { 85.224, 82.169, }, { 86.242, 82.169, },
  { 87.907, 83.834, }, { 88.926, 83.834, }, { 87.26, 82.169, },
  { 88.278, 82.169, }, { 89.944, 83.834, }, { 90.962, 83.834, },
  { 89.297, 82.169, }, { 90.315, 82.169, }, { 91.98, 83.834, },
  { 92.999, 83.834, }, { 91.333, 82.169, }, { 92.351, 82.169, },
  { 94.017, 83.834, }, { 95.035, 83.834, }, { 93.37, 82.169, },
  { 94.388, 82.169, }, { 96.053, 83.834, }, { 97.072, 83.834, },
  { 95.406, 82.169, }, { 96.424, 82.169, }, { 98.09, 83.834, },
  { 99.108, 83.834, }, { 97.442, 82.169, }, { 98.461, 82.169, },
  { 100.126, 83.834, }, { 101.144, 83.834, }, { 99.479, 82.169, },
  { 100.497, 82.169, }, { 102.163, 83.834, }, { 103.181, 83.834, },
  { 101.515, 82.169, }, { 102.534, 82.169, }, { 104.199, 83.834, },
  { 105.217, 83.834, }, { 103.552, 82.169, }, { 104.57, 82.169, },
  { 106.236, 83.834, }, { 107.254, 83.834, }, { 105.588, 82.169, },
  { 106.607, 82.169, }, { 108.272, 83.834, }, { 109.29, 83.834, },
  { 107.625, 82.169, }, { 108.643, 82.169, }, { 110.309, 83.834, },
  { 111.327, 83.834, }, { 109.661, 82.169, }, { 110.68, 82.169, },
  { 112.345, 83.834, }, { 113.363, 83.834, }, { 111.698, 82.169, },
  { 112.716, 82.169, }, { 114.381, 83.834, }, { 115.4, 83.834, },
  { 113.734, 82.169, }, { 114.752, 82.169, }, { 116.418, 83.834, },
  { 117.436, 83.834, }, { 115.771, 82.169, }, { 116.789, 82.169, },
  { 118.454, 83.834, }, { 119.473, 83.834, }, { 117.807, 82.169, },
  { 118.825, 82.169, }, { 120.491, 83.834, }, { 121.509, 83.834, },
  { 119.844, 82.169, }, { 120.862, 82.169, }, { 122.527, 83.834, },
  { 123.546, 83.834, }, { 121.88, 82.169, }, { 122.898, 82.169, },
  { 124.564, 83.834, }, { 125.582, 83.834, }, { 123.917, 82.169, },
  { 124.935, 82.169, }, { 126.6, 83.834, }, { 127.619, 83.834, },
  { 125.953, 82.169, }, { 126.971, 82.169, }, { 128.637, 83.834, },
  { 129.655, 83.834, }, { 127.989, 82.169, }, { 129.008, 82.169, },
  { 130.772, 83.933, }, { 130.8, 83.938, }, { 131.303, 84.133, },
  { 131.762, 84.417, }, { 132.16, 84.781, }, { 132.501, 85.232, },
  { 132.78, 85.838, }, { 132.809, 85.97, }, { 135, 88.161, },
  { 135.04, 88.065, }, { 135.129, 87.813, }, { 135.202, 87.557, },
  { 135.24, 87.383, }, { 130.026, 82.169, }, { 131.044, 82.169, },
  { 135.332, 86.457, }, { 135.325, 86.233, }, { 135.3, 85.968, },
  { 135.259, 85.704, }, { 135.202, 85.443, }, { 135.149, 85.255, },
  { 132.244, 82.351, },
};
const PROGMEM int speeds[911] = {
  667, 667, 667, 667,
  667, 667, 667,
  667, 667, 667,
  667, 667, 667,
  667, 667, 667,
  667, 667, 667,
  667, 667, 667,
  667, 667, 667,
  667, 667, 667,
  667, 667, 667,
  667, 667, 667,
  667, 667, 667,
  667, 667, 667,
  667, 667, 667,
  667, 667, 667,
  667, 667, 667,
  667, 667, 667,
  667, 667, 667,
  667, 667, 667,
  667, 667, 667,
  667, 667, 667,
  667, 667, 667,
  667, 667, 667,
  667, 667, 667,
  667, 667, 667,
  667, 667, 667,
  667, 667, 667,
  667, 667, 667,
  667, 667, 667,
  667, 667, 667,
  667, 667, 667,
  667, 667, 667,
  667, 667, 667,
  667, 667, 156,
  1000, 1000, 1000,
  1000, 1000, 1000,
  1000, 1000, 1000,
  1000, 1000, 1000,
  1000, 1000, 1000,
  1000, 1000, 1000,
  1000, 1000, 1000,
  1000, 1000, 1000,
  1000, 1000, 1000,
  1000, 1000, 1000,
  1000, 1000, 1000,
  1000, 1000, 1000,
  1000, 1000, 1000,
  1000, 1000, 1000,
  1000, 1000, 1000,
  1000, 1000, 1000,
  1000, 1000, 1000,
  1000, 1000, 1000,
  1000, 1000, 1000,
  1000, 1000, 1000,
  1000, 1000, 1000,
  1000, 1000, 1000,
  1000, 1000, 1000,
  1000, 1000, 1000,
  1000, 1000, 1000,
  1000, 1000, 1000,
  1000, 1000, 1000,
  1000, 1000, 1000,
  1000, 1000, 1000,
  1000, 1000, 1000,
  1000, 1000, 1000,
  1000, 1000, 1000,
  1000, 1000, 1000,
  1000, 1000, 1000,
  156, 667, 667,
  667, 667, 667,
  667, 667, 667,
  667, 667, 667,
  667, 667, 667,
  667, 667, 667,
  667, 667, 667,
  667, 667, 667,
  667, 667, 667,
  667, 667, 667,
  667, 667, 667,
  667, 667, 667,
  156, 1000, 1000,
  1000, 1000, 1000,
  1000, 1000, 1000,
  1000, 1000, 1000,
  1000, 1000, 1000,
  1000, 1000, 1000,
  1000, 1000, 1000,
  1000, 1000, 1000,
  1000, 1000, 1000,
  1000, 1000, 1000,
  1000, 1000, 1000,
  1000, 1000, 1000,
  156, 667, 667,
  667, 667, 667,
  667, 667, 667,
  667, 667, 667,
  667, 667, 667,
  667, 667, 667,
  667, 667, 667,
  667, 667, 667,
  667, 667, 667,
  667, 667, 667,
  667, 667, 667,
  667, 667, 667,
  667, 667, 667,
  667, 667, 667,
  667, 667, 667,
  667, 667, 667,
  667, 667, 667,
  667, 667, 667,
  667, 667, 667,
  667, 667, 667,
  667, 667, 667,
  667, 667, 667,
  667, 667, 667,
  667, 667, 667,
  667, 667, 667,
  667, 667, 667,
  667, 667, 667,
  667, 667, 667,
  667, 667, 667,
  667, 667, 667,
  667, 667, 667,
  667, 667, 667,
  667, 667, 667,
  667, 667, 667,
  667, 156, 1000,
  1000, 1000, 1000,
  1000, 1000, 1000,
  1000, 1000, 1000,
  1000, 1000, 1000,
  1000, 1000, 1000,
  1000, 1000, 1000,
  1000, 1000, 1000,
  1000, 1000, 1000,
  1000, 1000, 1000,
  1000, 1000, 1000,
  1000, 1000, 1000,
  1000, 1000, 1000,
  1000, 1000, 1000,
  1000, 1000, 1000,
  1000, 1000, 1000,
  1000, 1000, 1000,
  1000, 1000, 1000,
  1000, 1000, 1000,
  1000, 1000, 1000,
  1000, 1000, 1000,
  1000, 1000, 1000,
  1000, 1000, 1000,
  1000, 1000, 1000,
  1000, 1000, 1000,
  1000, 1000, 1000,
  1000, 1000, 1000,
  1000, 1000, 1000,
  1000, 1000, 1000,
  1000, 1000, 1000,
  1000, 1000, 1000,
  1000, 1000, 1000,
  1000, 1000, 1000,
  1000, 1000, 1000,
  1000, 1000, 156,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  156, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 156, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625,
};

void x_move(double x_dis, int DIR, int speed)
{
  //dir
  if (DIR == x_left) PORTF = X_LEFT;
  if (DIR == x_right) PORTF = X_RIGHT;
  // distance
  x_distance = x_dis;
  // speed
  OCR1A = speed;
  // on
  TIMSK1 = 0x02;
}

void y_move(int y_dis, int DIR, int speed)
{
  // dir
  if (DIR == y_up) PORTF = Y_UP;
  if (DIR == y_down) PORTF = Y_DOWN;
  // distance
  y_distance = y_dis;
  // speed
  OCR3A = speed;
  // on
  TIMSK3 = 0x02;
}

void z_move(int z_dis, int DIR, int speed)
{
  // dir
  if (DIR == z_up) PORTL = Z_UP;
  if (DIR == z_down) PORTL = Z_DOWN;
  // distance
  z_distance = z_dis;
  // speed
  OCR4A = speed;
  // on
  TIMSK4 = 0x02;
}

void ushift(double x, double y, int speed) // 대각선을 속도를 맞춰서 그리게하는 함수
{
  double dis_X = 0; // 변수들 초기화
  double dis_Y = 0;
  int x_dir = x_right;
  int y_dir = y_up;

  dis_X = abs(currunt_x - x); // 가야할 거리를 음수가 아니게 만들어서 값을 저장해준다
  dis_Y = abs(currunt_y - y);

  if ((double)currunt_x > (double)x) x_dir = x_left; // 가야할 방향을 맞추게 만들었다
  if ((double)currunt_y > (double)y) y_dir = y_down;

  if (currunt_x - x == 0) // 하나의 축(x,y)이 움직이지 않으면 반대축만 움직이게 한다
  {
    y_move((dis_Y * ONE_MM), y_dir, speed);
    currunt_y = y;
    while (TIMSK3 != 0X00);
    if (y_reset == 1) y_reset = 0;
    return;
  }
  if (currunt_y - y == 0)
  {
    x_move((dis_X * ONE_MM), x_dir, speed);
    currunt_x = x;
    while (TIMSK1 != 0X00);
    if (x_reset == 1) x_reset = 0;
    return;
  }

  double x_speed = 0;
  double y_speed = 0;
  double angle = 0;

  angle = atan(dis_Y / dis_X); // 앵글 == 아크탄젠트(길이(y/x))

  x_speed = speed * (1 / cos(angle)); // 원래 스피드를 구하는 식
  y_speed = speed * (1 / sin(angle));

  x_move((dis_X * ONE_MM), x_dir, x_speed);
  y_move((dis_Y * ONE_MM), y_dir, y_speed);

  currunt_x = x; // 현재 좌표를 저장한다
  currunt_y = y;

  // Serial.print(x); // 디버깅용 코드
  // Serial.print(" , y=");
  // Serial.print(y);
  // Serial.print(" , x_dir=");
  // Serial.print(x_dir);
  // Serial.print(" , y_dir=");
  // Serial.print(y_dir);
  // Serial.println();
  //while (TIMSK1 != 0X00 || TIMSK3 != 0X00); // 끝나는걸 기다려준다
  if (x_reset == 1) x_reset = 0;
  if (y_reset == 1) y_reset = 0;
}

void reset()
{
  x_reset = 1;
  y_reset = 1;
  z_reset = 1;
  z_move(32000, z_down, 600);
  while (z_reset != 0);
  delay(100);
  x_move(32000, x_left, 600);
  y_move(32000, y_up, 600);
  while (x_reset != 0 || y_reset != 0); // 끝날때까지 대기
  currunt_x = 0; // 초기화 했으니 좌표도 초기화
  currunt_y = 0;
}

void setup()
{
  DDRF |= X_STEP | X_DIR;
  DDRD |= X_ENABLE;
  DDRF |= Y_STEP | Y_DIR | Y_ENABLE;
  DDRL |= Z_STEP | Z_DIR;
  DDRK |= Z_ENABLE;
  DDRA |= E_STEP | E_DIR | E_ENABLE;
  DDRC |= E1_STEP | E1_DIR | E1_ENABLE;

  PORTL |= 0x01;
  //---------------------------
  // DDRD |= 0x40;
  // DDRB |= 0x03;
  // PORTB &= ~0x01;
  // DDRD |= 0x10;
  // PORTD &= ~0x10; // bed
  // DDRD |= 0x20;
  // PORTD &= ~0x20; // end
  // DDRB |= 0x10; // fan
  //---------------------------

  // x
  TCCR1A = 0x00;
  TCCR1B = 0x0a;
  TCCR1C = 0x00;
  OCR1A = 400;
  TIMSK1 = 0x00;

  // e
  TCCR2A = 0x02;
  TCCR2B = 0x06;
  TCNT2 = 0x00;
  OCR2A = 240;
  TIMSK2 = 0x00;

  // y
  TCCR3A = 0x00;
  TCCR3B = 0x0a;
  TCCR3C = 0x00;
  OCR3A = 400;
  TIMSK3 = 0x00;

  // z
  TCCR4A = 0x00;
  TCCR4B = 0x0a;
  TCCR4C = 0x00;
  OCR4A = 400;
  TIMSK4 = 0x00;

 // z_distance = 5000;
  //TIMSK4 = 0x02;
  currunt_y = 0;
  currunt_x = 0;
  //reset();
  x_move(100, x_right, 600);
  y_move(100, y_up, 600);
  z_move(300, z_up, 600);
  while (TIMSK1 != 0 || TIMSK3 != 0 || TIMSK4 != 0);
  delay(2000);

  Serial.begin(9600);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(A13, INPUT); // ther 0
  pinMode(A14, INPUT); // ther 1
  pinMode(A5, OUTPUT); // relay pin
  pinMode(10, OUTPUT); 

//  while(1)
//  {
//    for(int i = 0; i < 911; i++)
//    {
//      Serial.print(" x:");
//      Serial.print(pgm_read_float_near(&xy_pos[i][0]));
//      Serial.print(" y:");
//      Serial.print(pgm_read_float_near(&xy_pos[i][0]));
//      Serial.print(" speed:");
//      Serial.println(pgm_read_word_near(&speeds[i]));
//      delay(100);
//    }
//  }
  
  int b, e;
  while(e < 95 && b < 865)
  {
    int b = analogRead(A14);
    int e = analogRead(A13);
    if(b < 865)
    {
      digitalWrite(A5, LOW);
    }
    else if(870 < b)
    {
      digitalWrite(A5, HIGH);
    }
  
    if(e < 95)
    {
      digitalWrite(10, LOW);
    }
    else if(100 < e)
    {
      digitalWrite(10, HIGH);
    }
    Serial.print(" b:");
    Serial.print(b);
    Serial.print(" e:");
    Serial.print(e);
    Serial.println(" ");
    delay(500);
  }

  Serial.println("setup end");
}

volatile int i = 0;
volatile int end_analog_value = 0;
// volatile int bed_analog_value = 0;
volatile unsigned long c_millis = 0;
unsigned long p_millis = 0;
unsigned long p_millis1 = 0;
volatile unsigned long c_micros = 0;
volatile unsigned long p_micros = 0;
volatile int count = 0;
int b, e;
int atoggle = 0;
void loop() // 커스텀 프린터 태스트
{
  c_millis = millis();
  int b = analogRead(A14);
  int e = analogRead(A13);
  
  if(b < 865)
  {
    digitalWrite(A5, LOW);
  }
  else if(870 < b)
  {
    digitalWrite(A5, HIGH);
  }

  if(e < 95)
  {
    digitalWrite(10, LOW);
  }
  else if(100 < e)
  {
    digitalWrite(10, HIGH);
  }
    
  if(c_millis - p_millis > 1000)
  {
    p_millis = c_millis;
    Serial.print(" b:");
    Serial.print(b);
    Serial.print(" e:");
    Serial.print(e);
    Serial.println(" ");
  }

  

  if(TIMSK1 == 0X00 || TIMSK3 == 0X00)
  {
    atoggle = 1;
    
  }

  if(atoggle == 1)
  {
    atoggle = 0;
    //Serial.printf(" %lf, %lf, %lf",xy_pos[i][0], xy_pos[i][1], speeds[i]);
    ushift(pgm_read_float_near(&xy_pos[i][0]), pgm_read_float_near(&xy_pos[i][1]), pgm_read_word_near(&speeds[i]));
    i++;
    if(i == 911) i = 0;
  }
}

volatile char x_step_toggle = 0;
volatile int x_step_count = 0;

volatile char y_step_toggle = 0;
volatile int y_step_count = 0;

volatile char z_step_toggle = 0;
volatile int z_step_count = 0;

volatile char toggle = 0;

ISR(TIMER1_COMPA_vect)
{
  if (x_step_toggle == 0)
  {
    x_step_toggle = 1;
    PORTF |= X_STEP;
  }
  else
  {
    x_step_toggle = 0;
    PORTF &= ~(X_STEP);
    x_step_count++;
    //char x_limit_switch = PINC & X_STOP;

    if (x_step_count >= x_distance)
    {
      x_step_count = 0;
      TIMSK1 = 0x00;
    }
    //    if(x_limit_switch && x_reset == 0)
    //    {
    //      TIMSK1 = 0x00;
    //      x_step_count = 0;
    //    }
  }
}

ISR(TIMER2_COMPA_vect) {
  if (toggle == 0)
  {
    toggle = 1;
    PORTA |= E_STEP;
  }
  else
  {
    toggle = 0;
    PORTA &= ~(E_STEP);
  }
}

ISR(TIMER3_COMPA_vect)
{
  if (y_step_toggle == 0)
  {
    y_step_toggle = 1;
    PORTF |= Y_STEP;
  }
  else
  {
    y_step_toggle = 0;
    PORTF &= ~(Y_STEP);
    y_step_count++;
    //char y_limit_switch = PINC & Y_STOP;

    if (y_step_count >= y_distance)
    {
      y_step_count = 0;
      TIMSK3 = 0x00;
    }
    //    if(y_limit_switch  && y_reset == 0)
    //    {
    //      TIMSK3 = 0x00;
    //      y_step_count = 0;
    //    }
  }
}

ISR(TIMER4_COMPA_vect)
{
  if (z_step_toggle == 0)
  {
    z_step_toggle = 1;
    PORTL |= 0x08;
  }
  else
  {
    z_step_toggle = 0;
    PORTL &= ~(0x08);
    z_step_count++;
    //char z_limit_switch = PINC & Z_STOP;

    if (z_step_count >= z_distance)
    {
      z_step_count = 0;
      TIMSK4 = 0x00;
    }
    //    if(z_limit_switch && z_reset == 0)
    //    {
    //      TIMSK4 = 0x00;
    //      z_step_count = 0;
    //    }
  }
}
//1 step = 0.0125mm
//80 step = 1mm
//1 zsetp = 0.0025mm