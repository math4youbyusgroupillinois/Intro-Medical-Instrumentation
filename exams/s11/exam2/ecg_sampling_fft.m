function []=ecg_sampling_fft()
% BME154L, Spring 2011
% Exam #2 Problem
% Mark Palmeri
% 2011-04-02

% load in the data
fid = fopen('../../problem_sets/ps6/BME154L_S11_PS6_ECG.dat','r');
data = fread(fid,'float32');
fclose(fid);
t = data(1:2:end);
ecg = data(2:2:end);
clear data

% downsample t and ecg by factor of 10 to 2000 Hz
t = t(1:10:end);
ecg = ecg(1:10:end);

% compute FFT
fs = 1/mean(diff(t));
ft = fft(ecg);
f = linspace(-fs/2,fs/2,length(ft));

% remove DC offset and linear tilt from data
p = polyfit(t,ecg,1); % fit a line to the data
artifact = p(1)*t + p(2);
clean_ecg = ecg - artifact;

% compute clean FFT
ft_clean = fft(clean_ecg);

figure;

ecgFFT = fftshift(20*log10(abs(ft)./max(abs(ft_clean))));
ecgS = repmat(ecgFFT,5,1);
fS = linspace(-fs*5/2,fs*5/2,length(ecgS));

subplot(2,1,1)
plot(fS,ecgS);
axis([-5000 5000 -60 0]);
xlabel('Frequency (Hz)','FontSize',18);
ylabel('Relative Power (dB)','FontSize',18);
title('Power Spectrum of ECG Signal','FontSize',18);
set(gca,'FontSize',18);
set(gca,'XTick',[-4000,-2000,0,2000,4000]);

subplot(2,1,2);
plot(f,fftshift(20*log10(abs(ft)./max(abs(ft_clean)))));
axis([0 650 -60 0]);
xlabel('Frequency (Hz)','FontSize',18);
ylabel('Relative Power (dB)','FontSize',18);
title('Zoomed in Region from Power Spectrum','FontSize',18);
set(gca,'FontSize',18);

print -deps2 ecg_sampling_fft.eps
