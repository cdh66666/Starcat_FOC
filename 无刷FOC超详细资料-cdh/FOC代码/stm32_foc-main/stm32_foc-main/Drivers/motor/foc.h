#pragma once

typedef enum
{
    control_type_null,
    control_type_position_speed_torque,
    control_type_speed_torque,
    control_type_position,
    control_type_speed,
    control_type_torque,
} motor_control_type;

typedef struct
{
    motor_control_type type;
    float position;
    float speed;
    float max_speed;
    float torque_norm_d;
    float torque_norm_q;
    float max_torque_norm;
} motor_control_context_t;

extern motor_control_context_t motor_control_context;

void lib_position_control(float rad);
void lib_speed_control(float speed);
void lib_torque_control(float torque_norm_d, float torque_norm_q);
void lib_speed_torque_control(float speed, float max_torque_norm);
void lib_position_speed_torque_control(float position, float max_speed, float max_torque_norm);
void set_motor_pid(
    float position_p, float position_i, float position_d,
    float speed_p, float speed_i, float speed_d,
    float torque_d_p, float torque_d_i, float torque_d_d,
    float torque_q_p, float torque_q_i, float torque_q_d);
float regular_diff(float diff, float cycle);
