% sliding_average.m
%
% BME154L (Spring 2012)
% Problem Set #6 (Sliding Window Averager)
%
% Mark Palmeri

data = [ 0 0.2 0 -0.1 0.3 0.8 1.2 0.9 1.1 1.2 0.8 1.1 0 -0.2 -0.1 0 -0.2 ];

% pad the data with a 0 on the front and back to do a 3-sample boxcar
data = [0 data 0];

figure;
plot(data,'-x');
hold on;

% 3-sample boxcar
data_boxcar = zeros(1,length(data));
data_boxcar(2:end-1) = (data(1:end-2) + data(2:end-1) + data(3:end))/3;
plot(data_boxcar,'--s');

% real signal
real = [0 0 0 0 0 0 0 1 1 1 1 1 0 0 0 0 0 0 0];
stem(real,'-')

% compute SNR
nonzero = find(real ~= 0)
snr_data = 20*log10(mean(data(nonzero))/std(data(nonzero)))
snr_data_boxcar = 20*log10(mean(data_boxcar(nonzero))/std(data_boxcar(nonzero)))

legend('Raw Data','Boxcar','Real Data','Location','Northwest');
legend boxoff
text(14.0,0.8,sprintf('Data SNR = %.1f dB',snr_data));
text(14.0,0.7,sprintf('Boxcar SNR = %.1f dB',snr_data_boxcar));
