%% Funci�n de transferencia PID
clear
clc

%Carga de paquetes. ------------------------------------------------------------
pkg load control

% Modelado de la planta en tiempo cont�nuo. ------------------------------------
% Valores de la funci�n de planta
J = 8.43e-6
b = 0.006
K = 0.278
R = 14
L = 0.008
s = tf('s');

% Valores PID
Kp = 100
Ki = 200
Kd = 10

% Funci�n de transferencia
P_motor = K / ((J*s+b)*(L*s+R)+K^2);
H = zpk(P_motor)

% An�lisis de lazo abierto en tiempo cont�nuo. ---------------------------------
% C�lculo de ancho de banda
[open_loop_num,open_loop_den] = tfdata(H,'v')
[bw_lower, bw_upper] = bandwidth(open_loop_den)

% Gr�fica de respuesta al escal�n unitario.
figure(1)
subplot (2, 1 , 1)
step(H)
grid on
title('Respuesta al escal�n unitario lazo cerrado')
xlabel('Tiempo')
ylabel('Amplitud')

% Diagrama de polos y ceros.
subplot (2, 1 , 2 )
pzmap(H)
xlim ([-12, 0]);
title('Diagrama de polos y ceros')
xlabel('Im')
ylabel('Re')

% Diagrama de Bode.
figure(2)
bode(H)
grid on

% An�lisis de lazo cerrado en tiempo cont�nuo. ---------------------------------
figure(3)

%define valores para numerador y denominador de PID
numc=[Kd, Kp, Ki];
denc=[1 0];
numa=conv(open_loop_num,numc);
dena=conv(open_loop_den,denc);

% Respuesta al escal�n unitario
subplot (2, 1 , 1)
PID_tf=tf(numa,dena)
H_loop=feedback(PID_tf,1);
step(H_loop)
grid on
title('Respuesta al escal�n unitario')
xlabel('Tiempo')
ylabel('Amplitud')

% Diagrama de polos y ceros.
subplot (2, 1 , 2 )
pzmap(H_loop)
xlim ([-12, 0]);
title('Diagrama de polos y ceros')
xlabel('Im')
ylabel('Re')

% Diagrama de Bode.
figure(4)
bode(H_loop)
grid on

% C�lculo de ancho de banda
[closed_loop_num,closed_loop_den] = tfdata(H_loop,'v')
[bw_lower_closed, bw_upper_closed] = bandwidth(closed_loop_den)

% An�lisis de lazo cerrado en tiempo discreto. ---------------------------------
sample_rate_hz = 25

% Discretizaci�n de las variables
sample_rate_sec = 1 / sample_rate_hz;
H_loop_disc = c2d(H_loop,sample_rate_sec, 'zoh')

% C�lculo de ancho de banda
[closed_loop_num_z,closed_loop_den_z] = tfdata(H_loop_disc,'v')
[bw_lower_closed, bw_upper_closed] = bandwidth(closed_loop_den_z)

% Respuesta al escal�n unitario
figure(5)
subplot (2, 1 , 1)
step(H_loop_disc)
grid on
title('Respuesta al escal�n unitario')
xlabel('Tiempo')
ylabel('Amplitud')

% Diagrama de polos y ceros.
subplot (2, 1 , 2 )
pzmap(H_loop_disc)
xlim ([-12, 0]);
title('Diagrama de polos y ceros')
xlabel('Im')
ylabel('Re')

% Diagrama de Bode.
figure(6)
bode(H_loop_disc)
grid on
