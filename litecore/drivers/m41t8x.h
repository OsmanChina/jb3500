#ifndef __M41T8X_H__
#define __M41T8X_H__



#ifdef __cplusplus
extern "C" {
#endif








//External Functions
sys_res rtc_i2c_Init(p_dev_i2c p);
sys_res rtc_i2c_GetTime(p_dev_i2c p, time_t *pTime);
sys_res rtc_i2c_SetTime(p_dev_i2c p, time_t tTime);


#ifdef __cplusplus
}
#endif


#endif




