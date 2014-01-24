% plot_f.m

figure;
t = [0:0.01e-6:2e-6];
vi = 100*cos(2*pi*1e6*t);
subplot(2,1,1);
plot(t*1e6,vi,'Color','k');
xlabel('Time (\mus)');
ylabel('V_i (V)');
grid on
axis([0 2 -150 150])

subplot(2,1,2);
plot(t*1e6,t*0,'Color','k');
axis([0 2 -150 150])
xlabel('Time (\mus)');
ylabel('V_o (     )');
grid on
set(gca,'YTickLabel','');

print -deps2 plot_f.eps
