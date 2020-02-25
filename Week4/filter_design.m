%% Lab 4 FIR design
rp = 0.5;           % Passband ripple
rs = 48;          % Stopband ripple
fs = 8000;        % Sampling frequency
f = [355 415 1200 1270];    % Cutoff frequencies
a = [0 1 0];        % Desired amplitudes
dev = [10^(-rs/20) (10^(rp/20)-1)/(10^(rp/20)+1)  10^(-rs/20)]; 
[n,fo,ao,w] = firpmord(f,a,dev,fs);
% fn= f/(0.5*fs);
b = firpm(n,fo,ao,w);
freqz(b,1,1024,fs) % coef b and all ones, with 1024 data points and fs of 8k
title('Band-pass Filter Designed to Specifications')


write_B=fopen('fir_coef.txt','wt');

fprintf(write_B,'#define N %d\n', length(b));
fprintf(write_B,'double B[]={');

for i=1:length(b)-1
    fprintf(write_B,'%e, ',b(i));
end

fprintf(write_B,'%e};\n',b(length(b)));
fclose('all');
%% FIR response figure
x = linspace(-fs/2,fs/2,249);
plot(x,b)
grid on
grid minor
title('Finite Impulse Response Coefficients ')


%% Lab5
rp = 0.3;           % Passband ripple
rs = 20;          % Stopband ripple
fs = 8000;        % Sampling frequency
f = [270 450];    % Cutoff frequencies

[b,a] = ellip(4,rp,rs,2*f/fs); % 4th order,rp,rs,wp -> normalized frequency
freqz(b,a)


write_B=fopen('iir_coef.txt','wt');

fprintf(write_B,'#define Na %d\n', length(a));
fprintf(write_B,'#define Nb %d\n', length(b));

fprintf(write_B,'double A[]={');

for i=1:length(a)-1
    fprintf(write_B,'%e, ',a(i));
end

fprintf(write_B,'%e};\n',a(length(a)));

fprintf(write_B,'double B[]={');

for i=1:length(b)-1
    fprintf(write_B,'%e, ',b(i));
end

fprintf(write_B,'%e};\n',b(length(b)));

fclose('all');