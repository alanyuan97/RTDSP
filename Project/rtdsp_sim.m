%% Read audio signal IN
% eleven_8k.wav beep_8k.wav journey_no_noise_8k.wav noisy_journey_8k.wav
name = 'journey_no_noise_8k.wav';
info = audioinfo(name);
[y,Fs] = audioread(name);
%sound(y,Fs) % play the sound
t = 0:seconds(1/Fs):seconds(info.Duration);
t = t(1:end-1);
plot(t,y)
xlabel('Time')
ylabel('Audio Signal')

%% Fast FT & Visualize
close all
Nfft=4000;
%Nfft = length of fft
f=linspace(0,Fs,Nfft);
X=abs(fft(y,Nfft));
%G = the fft of the samples y in 1024 points
figure;
plot(f(1:Nfft/2),X(1:Nfft/2))
grid on
grid minor
title('Frequency Spectrum - Beep')
xlabel('Frequency / Hz')
ylabel('Magnitude')

% region_find = X(1:Nfft/2);
% a = find(region_find>25);
% 
% for j = 1:length(a)-1
%     if a(j+1)-a(j) ~=1 
%         Report index j
%         fprintf('Index cut %d \n',j)
%     end
% end
% figure;
% plot(find(region_find>20))
%% Circulate buffer
Nfft=4000;
% Fs = 8000 , 2.5s 
target_time = 0.125;
sample_nums = Fs*target_time;
frame_sample = sample_nums *4;
index =1;
k = 0:1:frame_sample-1;
hamming_window = sqrt(1-0.85185*cos((2*k+1)*pi/frame_sample))';
iterations = round(length(y)/sample_nums)-1;
A = zeros(960000,1);
freq_inx = find(f>2500 & f <2600);
freq_inx_ele1 = find(f>265 & f <275);
freq_inx_ele2 = find(f>555 & f <565);
freq_inx_ele3 = find(f>845 & f <855);
highline = zeros(960000,1);
ground = zeros(960000,1);
for i= 1:iterations
    tic;
    % assign buffer
    
    if index+frame_sample-1>length(y)
        buff = y(index:end,:);
        need_zero = frame_sample - length(buff);
        buff = cat(1,buff,zeros(need_zero,1));
    else
        buff = y(index:index+frame_sample-1,1);
    end
    
    newbuff = buff .* hamming_window;
    
    % FFT
    X=fft(newbuff,Nfft);
    % Magnitude 
    amp = abs(X);
    res_angle = angle(X);
    
%     if mod(i,4)==1
%        min_amp = min(amp);
%     else
%         if min_amp>min(amp)
%             min_amp = min(amp);
%         end
%     end
%     
%     if mod(i,4)==0
%         % perform amp sub
%         amp = amp - min_amp;
%     end
    
    crop_amp1= amp(freq_inx(1):freq_inx(end));
%     crop_amp2= amp(freq_inx_ele2(1):freq_inx_ele2(end));
%     crop_amp3= amp(freq_inx_ele3(1):freq_inx_ele3(end));
    if length(crop_amp1(crop_amp1>2.5))>2
        fprintf('Found Beep @ iteration %d \n',i)
        fprintf('Found Beep @ Time %f to %f s \n',index/8000,(index+frame_sample)/8000)
        highline(index:index+frame_sample-1,:)= 2.5;
    end

%     if 1
%         sum(region_find - amp(1:Nfft/2))
%         fprintf('Found Ele @ iteration %d \n',i)
%         fprintf('Found Ele @ Time %f to %f s \n',index/8000,(index+frame_sample)/8000)
%     end
%     [c,lags] = xcorr(newbuff,eleven(1:2000),'coeff');
%     if length(c(c>0.4))>10
%         figure
%         plot(c)
%         fprintf('Found Ele @ Time %f to %f s \n',index/8000,(index+frame_sample-1)/8000)
%     end
    cpx = complex(amp.*cos(res_angle),amp.*sin(res_angle));
    % IFFT
    Y=real(ifft(cpx));
    Y = Y .* hamming_window;
    % shift the buffer
    front = zeros(sample_nums*(i-1),1);
    back = zeros(sample_nums*(iterations-i),1);
    output = cat(1,front,Y);
    output = cat(1,output,back);
    A = A + output;
    index  = index + sample_nums;
    toc;
%     pause(1)
end
%%
% Assuming ground truth below:
ground_truth_beep = [3 4 5 7 8 10 23 42 43 44 60 68 72 ];
for i = 1:length(ground_truth_beep)
    ground(Fs*(ground_truth_beep(i)):Fs*(ground_truth_beep(i)+0.1))= 2.5;
end
tt = linspace(0,119.77,960000);
figure
% plot(tt,A,'Color',[0 0.4470 0.7410])
hold on
plot(tt,highline,'r--')
grid on
grid minor
plot(tt,ground,'Color',[0 0.4470 0.7410])
xlabel('Time /s')
ylabel('Audio signal')
legend('Detected Frame','Ground Truth Frame')
title('Detected Frame vs. Ground Truth Frame')
%% Plot
subplot(2,1,1)
plot(t,y)
xlabel('Time / s')
ylabel('Audio Signal')
grid on
grid minor
title('Time Domain - Eleven')
subplot(2,1,2)
plot(f(1:Nfft/2),X(1:Nfft/2))
grid on
grid minor
title('Frequency Spectrum - Eleven')
xlabel('Frequency / Hz')
ylabel('Magnitude')