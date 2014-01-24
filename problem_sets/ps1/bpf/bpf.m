% BME153L.02 (Spring 2009)
% Test #2 BPF Transfer Function Interpretation
% Mark Palmeri (mark.palmeri@duke.edu)
% 2009-03-21

R = 9000;
C = 5.55e-9;
L = 2e-3;

w = logspace(3,8,10000);

H = (i*w*C*R)./(1+i*w*C*R+((i*w).^2)*L*C);

subplot(2,1,1);
h=semilogx(w,20*log10(abs(H)));
set(h,'LineWidth',1,'Color','k');
xlabel('\omega (rad/s)','FontSize',14)
ylabel('|H(j\omega)| (dB)','FontSize',14)
set(gca,'YTick',[-40 -20 -12 -6 -3 0])
set(gca,'FontSize',14);
grid on;
a=axis;
axis([a(1) a(2) -25 0]);

subplot(2,1,2);
h=semilogx(w,angle(H)*360/(2*pi));
set(h,'LineWidth',1,'Color','k');
xlabel('\omega (rad/s)','FontSize',14)
ylabel('Phase (deg)','FontSize',14)
set(gca,'YTick',[-90:45:90]);
grid on;
set(gca,'FontSize',14);

print -deps2 bpf.eps
