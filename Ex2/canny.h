#ifndef canny_h
#define canny_h
#ifdef __cplusplus
extern "C"
#endif
unsigned char *canny(unsigned char *grey, int width, int height);
#ifdef __cplusplus
extern "C"
#endif
unsigned char *cannyparam(unsigned char *grey, int width, int height, 
						  float lowThreshold, float highthreshold, 
						  float gaussiankernelradius, int gaussiankernelwidth,  
						  int contrastnormalised);
#endif
