ziva=load('ziva.asc');
t = ziva(:,1);
ecg=ziva(:,2);
clear('ziva');

figure(1);
fsz = 32;
tmax = min(find(t>=28));
t = t(1:tmax);
ecg = ecg(1:tmax);
plot(t,ecg);
xlabel('Time (s)','FontSize',fsz);
ylabel('Voltage (mV)','FontSize',fsz);
title('Ziva''s ECG Trace','FontSize',fsz);
set(gca,'FontSize',fsz);
print('-deps2','ziva_ecg.eps');

figure(2);
ecgfft = fft(ecg);
fs = 1/mean(diff(t));
f = linspace(-fs/2,fs/2,length(t));
% lets smooth this out some
a = mean(diff(f))/20;
ecgfft = filter(a,[1 a-1],ecgfft);
ecgfft = fftshift(20*log10(abs(ecgfft)./max(abs(ecgfft)))); 
f0 = min(find(f >= 0));
f = f(f0:end);
ecgfft = ecgfft(f0:end);
plot(f,ecgfft);
xlabel('Frequency (Hz)','FontSize',fsz);
ylabel('Power (dB)','FontSize',fsz);
title('Ziva''s ECG Power Spectrum','FontSize',fsz);
set(gca,'FontSize',fsz);
xlim([0 500]);
print('-deps2','ziva_ecgfft.eps');
