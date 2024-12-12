#include "foc.h"
#include "conf.h"
#include "motor_runtime_param.h"
#include "arm_math.h"
#include <stdbool.h>
#include <stdio.h>
#define deg2rad(a) (PI * (a) / 180)
#define rad2deg(a) (180 * (a) / PI)
#define max(a, b) ((a) > (b) ? (a) : (b))
#define min(a, b) ((a) < (b) ? (a) : (b))

#define SQRT3 1.73205080756887729353
#define rad60 deg2rad(60)
static arm_pid_instance_f32 pid_position;
static arm_pid_instance_f32 pid_speed;
static arm_pid_instance_f32 pid_torque_d;
static arm_pid_instance_f32 pid_torque_q;
motor_control_context_t motor_control_context;

/**
 * @brief 笛卡尔坐标系下的svpwm
 *
 * @param phi 转子角度
 * @param d d轴强度单位比例
 * @param q q轴强度单位比例
 * @param d_u u轴占空比
 * @param d_v v轴占空比
 * @param d_w w轴占空比
 */
static void svpwm(float phi, float d, float q, float *d_u, float *d_v, float *d_w)
{
    const int v[6][3] = {{1, 0, 0}, {1, 1, 0}, {0, 1, 0}, {0, 1, 1}, {0, 0, 1}, {1, 0, 1}};
    const int K_to_sector[] = {4, 6, 5, 5, 3, 1, 2, 2};
    float sin_phi = arm_sin_f32(phi);
    float cos_phi = arm_cos_f32(phi);
    float alpha = 0;
    float beta = 0;
    arm_inv_park_f32(d, q, &alpha, &beta, sin_phi, cos_phi);

    bool A = beta > 0;
    bool B = fabs(beta) > SQRT3 * fabs(alpha);
    bool C = alpha > 0;

    int K = 4 * A + 2 * B + C;
    int sector = K_to_sector[K];

    float t_m = arm_sin_f32(sector * rad60) * alpha - arm_cos_f32(sector * rad60) * beta;
    float t_n = beta * arm_cos_f32(sector * rad60 - rad60) - alpha * arm_sin_f32(sector * rad60 - rad60);
    float t_0 = 1 - t_m - t_n;

    *d_u = t_m * v[sector - 1][0] + t_n * v[(sector) % 6][0] + t_0 / 2;
    *d_v = t_m * v[sector - 1][1] + t_n * v[(sector) % 6][1] + t_0 / 2;
    *d_w = t_m * v[sector - 1][2] + t_n * v[(sector) % 6][2] + t_0 / 2;
}

__attribute__((weak)) void set_pwm_duty(float d_u, float d_v, float d_w)
{
    while (1)
        ;
}

/**
 * @brief foc计算函数，计算完成后，设置三相pwm
 *
 * @param u_d d轴电压，归一化
 * @param u_q q轴电压，归一化
 * @param rad 电机转子角度
 */
void foc(float u_d, float u_q, float rad)
{
    u_d = min(u_d, 1);
    u_d = max(u_d, -1);
    u_q = min(u_q, 1);
    u_q = max(u_q, -1);
    float d_u = 0;
    float d_v = 0;
    float d_w = 0;
    svpwm(rad, u_d, u_q, &d_u, &d_v, &d_w);
    set_pwm_duty(d_u, d_v, d_w);
}

/**
 * @brief 位置环，任意输出，可以输出速度到速度环、电流到电流环、直接输出q轴，不同的输出方式需要设置对应的pid参数
 *
 * @param rad
 * @return float 下一环的输入
 */
static float position_loop(float rad)
{
    float diff = regular_diff(rad - motor_logic_angle, position_cycle);
    // printf("%f\n", diff);
    // printf("%f,%f,%f\n", diff, motor_logic_angle, rad);
    return arm_pid_f32(&pid_position, diff);
}

/**
 * @brief 速度环，任意输出，可以输出电流到电流环、直接输出q轴，不同的输出方式需要设置对应的pid参数
 *
 * @param speed_rad
 * @return float 下一环的输入
 */
static float speed_loop(float speed_rad)
{
    float diff = speed_rad - motor_speed;
    // printf("%f\n", motor_speed);

    return arm_pid_f32(&pid_speed, diff);
}

/**
 * @brief 力矩环，输出d轴。
 *
 * @param i_d d轴电流，归一化
 * @return float d轴电压，归一化
 */
static float torque_d_loop(float d)
{
    float diff = d - motor_i_d / MAX_CURRENT;

    return arm_pid_f32(&pid_torque_d, diff);
}

/**
 * @brief 力矩环，输出q轴。
 *
 * @param q q轴电流，归一化
 * @return float q轴电压，归一化
 */
static float torque_q_loop(float q)
{
    float diff = q - motor_i_q / MAX_CURRENT;

    return arm_pid_f32(&pid_torque_q, diff);
}

void lib_position_control(float rad)
{
    float u_d = 0;
    float u_q = position_loop(rad);
    // printf("%f\n", u_q);
    // printf("%f\n", rotor_phy_angle);

    foc(u_d, u_q, rotor_logic_angle);
}

void lib_speed_control(float speed)
{
    float u_d = 0;
    float u_q = speed_loop(speed);
    // printf("%f\n", u_q);

    foc(u_d, u_q, rotor_logic_angle);
}

void lib_torque_control(float torque_norm_d, float torque_norm_q)
{
    float d = torque_d_loop(torque_norm_d);
    float q = torque_q_loop(torque_norm_q);
    foc(d, q, rotor_logic_angle);
}

void lib_speed_torque_control(float speed_rad, float max_torque_norm)
{
    float torque_norm = speed_loop(speed_rad);
    torque_norm = min(fabs(torque_norm), max_torque_norm) * (torque_norm > 0 ? 1 : -1);
    lib_torque_control(0, torque_norm);
}

void lib_position_speed_torque_control(float position_rad, float max_speed_rad, float max_torque_norm)
{
    float speed_rad = position_loop(position_rad);
    speed_rad = min(fabs(speed_rad), max_speed_rad) * (speed_rad > 0 ? 1 : -1);
    lib_speed_torque_control(speed_rad, max_torque_norm);
}

void set_motor_pid(
    float position_p, float position_i, float position_d,
    float speed_p, float speed_i, float speed_d,
    float torque_d_p, float torque_d_i, float torque_d_d,
    float torque_q_p, float torque_q_i, float torque_q_d)
{
    pid_position.Kp = position_p;
    pid_position.Ki = position_i;
    pid_position.Kd = position_d;

    pid_speed.Kp = speed_p;
    pid_speed.Ki = speed_i;
    pid_speed.Kd = speed_d;

    pid_torque_d.Kp = torque_d_p;
    pid_torque_d.Ki = torque_d_i;
    pid_torque_d.Kd = torque_d_d;

    pid_torque_q.Kp = torque_q_p;
    pid_torque_q.Ki = torque_q_i;
    pid_torque_q.Kd = torque_q_d;
    arm_pid_init_f32(&pid_position, false);
    arm_pid_init_f32(&pid_speed, false);
    arm_pid_init_f32(&pid_torque_d, false);
    arm_pid_init_f32(&pid_torque_q, false);
}

float regular_diff(float diff, float cycle)
{
    diff = fmodf(diff + cycle, cycle);
    if (diff > (cycle / 2))
        diff -= cycle;
    return diff;
}