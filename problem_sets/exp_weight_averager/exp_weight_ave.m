function []=exp_weight_ave()
% function []=exp_weight_ave()
%
% Demonstrate the properties of the exponentially-weighted averager (compared
% w/ a boxcar averager).  [BME154L S12 PS6.3]
%
% INPUTS: None
%
% OUPUTS: None
%
% EXAMPLE: exp_weight_ave
%
% Mark Palmeri (mlp6)
% 2012-03-16

% define coefficient weight
% m = 1 is a limiting case since only the S_n term will be non-zero, 
% which makes this impulse response look like a delta function

N = [0:30]; % number of samples in the averager to show

for m=1:10,
    for n=N,
        S(n+1,m) = (1/m)*((m-1)/(m))^n;
    end;
end;

figure;
bar(N,S);
xlim([-1 31]);
set(gca,'XTick',[0:5:30]);
xlabel('N','FontSize',18);
ylabel('Sample Weight','FontSize',18);
title('Exponential Averager Sample Weight','FontSize',18);

legend('m = 1','m = 2','m = 3','m = 4','m = 5','m = 6','m = 7','m = 8','m = 9','m = 10')
legend boxoff

set(gca,'FontSize',14)

print('-depsc2','exp_weight_ave.eps');

% lets do some Fourier analysis on these...

fs = 1000; % Hz

expFFT = zeros(1000,100);
M=[1 5 10];
for m=1:length(M),
    expbox(45:45+max(N),m) = S(:,M(m));
    expFFT = fft(expbox(:,m));
    f = linspace(-fs/2,fs/2,length(expFFT));
    subplot(length(M),2,m*2-1);
    stem(expbox(:,m),'MarkerSize',4);
    xlim([40 40+max(N)+10]);
    set(gca,'XTick',[]);
    title(sprintf('Exponential Weights (m = %i)',M(m)));
    xlabel('Samples');
    if(m == 2),
        ylabel('Arbitrary Value');
    end;
    subplot(length(M),2,m*2);
    plot(f,fftshift(20*log10(abs(expFFT)./max(abs(expFFT)))));
    ylim([-25 1]);
    xlabel('Frequency (Hz)');
    if(m == 2),
        ylabel('Normalized Power Spectrum (dB)');
    end;
    title(sprintf('FFT (m = %i)',M(m)));
end;

print('-depsc2','exp_weight_ave_fft.eps');
