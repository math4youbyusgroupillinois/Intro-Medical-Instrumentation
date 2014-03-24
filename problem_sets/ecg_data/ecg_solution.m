load BME354L_CANINE_ECG.mat

fs=32;
figure;
plot(t,ecg,'-k');
xlabel('Time (s)','FontSize',fs);
ylabel('Voltage (mV)','FontSize',fs);
title('BME354L CANINE ECG (MAT)','FontSize',fs)
set(gca,'FontSize',fs)
xlim([0 5]) 

print('-deps','ecg_mat.eps');

