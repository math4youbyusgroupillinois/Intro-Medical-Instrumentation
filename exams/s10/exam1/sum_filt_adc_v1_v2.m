figure;
subplot(2,1,1);

f1 = 20;
w1 = 2*pi*f1;
t1 = [0:0.001:0.5];
plot(t1,100*sin(w1.*t1),'LineWidth',3);
xlabel('Time (s)','FontSize',18);
ylabel('V1 (mV)','FontSize',18);
set(gca,'FontSize',18);

subplot(2,1,2);
f2 = 10000;
w2 = 2*pi*f2;
t2 = [0:0.000001:0.0005];
plot(t2*1e3,100*sin(w2.*t2),'LineWidth',3);
xlabel('Time (ms)','FontSize',18);
ylabel('V2 (mV)','FontSize',18);
set(gca,'FontSize',18);

print -deps2 sum_filt_adc_v1_v2.eps
