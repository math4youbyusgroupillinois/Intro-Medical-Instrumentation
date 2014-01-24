% plot_g.m

figure;
t = [0:0.0001:0.02];
vi = 100*cos(2*pi*100*t);
subplot(2,1,1);
plot(t,vi,'Color','k');
xlabel('Time (s)');
ylabel('V_i (V)');
grid on
axis([0 0.02 -150 150])

subplot(2,1,2);
plot(t,t*0,'Color','k');
axis([0 0.02 -150 150])
xlabel('Time (s)');
ylabel('V_o (     )');
grid on
set(gca,'YTickLabel','');

print -deps2 plot_g.eps
