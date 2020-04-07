% This script calculates the proper constants for the motor control
% Moment of inertia of the rotor [kg.m^2].
J = 0.01;

% Motor viscous friction constant [N.m.s].
b = 0.1;

% Electromotive force constant [V/rad/sec].
K = 2.20;

% Electric resistance [Ohm].
R = 4.8;
% Electric inductance [H].
L = 0.00092;

% Creates transfer function model.
s = tf('s');

% Open-loop transfer function for DC motor in Laplace domain.
P_motor = K/((J*s+b)*(L*s+R)+K^2);

% PID values
Kp = 80
Ki = 50
Kd = 10

C = pid(Kp,Ki,Kd);
sys_cl = feedback(C*P_motor,1);
step(sys_cl, 0:0.01:4)
grid
title('PID Control')
