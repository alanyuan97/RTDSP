%% Read audio signal IN
% eleven_8k.wav beep_8k.wav journey_no_noise_8k.wav

info = audioinfo('journey_no_noise_8k.wav');
[y,Fs] = audioread('journey_no_noise_8k.wav');
%sound(y,Fs) % play the sound
t = 0:seconds(1/Fs):seconds(info.Duration);
t = t(1:end-1);
plot(t,y)
xlabel('Time')
ylabel('Audio Signal')

%% Fast FT & Visualize
Nfft=1024;
%Nfft = length of fft
f=linspace(0,Fs,Nfft);
X=abs(fft(y,Nfft));
%G = the fft of the samples y in 1024 points
figure ; plot(f(1:Nfft/2),X(1:Nfft/2))
grid on
grid minor

%% Circulate buffer

% Fs = 8000 , 2.5s 
target_time = 2.5;
sample_nums = Fs*target_time;
frame_sample = sample_nums *4;
index =0;
while 1
    tic;
    % Check for wrap
    if index> length(y)
        index =0;
    end
    % assign buffer
    buff = y(index:index+frame_sample,:);
    % FFT
    X=abs(fft(buff,Nfft));
    printf()
    % Find min
    index  = index + sample_nums;
    toc;
end