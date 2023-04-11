function qnew=init(t, dt, method)
	if method == 1
		q0=0.0;
		qmax=430;
		fi=3*pi*t-1.4142;
		qnew=q0+qmax*(0.251+0.290*(cos(fi)+0.97*cos(2*fi)+0.47*cos(3*fi)+0.14*cos(4*fi)));
	end
	if method ==2
		T0=1.0;
		a=0.1;
		qmax=70;
		qnew = qmax*t/(a^2)*exp(-t^2/(2*a^2));
	end
	if method ==3
		T0=1.0;
		a=0.3;
		qmax=485;
		if (t<=T0)
			if(t<a)
				qnew=qmax*sin(pi*t/a);
			else
				qnew = 0.0;
			end
		else
			temp=t;
			while(temp>T0)
				temp = temp-T0;
			end
			if(temp<a)
				qnew=qmax*sin(pi*temp/a);
			else
				qnew = 0.0;
            end
        end
    end
	if method ==4
		q0=0.0;
		qmax=500;
		a=2.0/3.0;
		T0=1.0;
		if(t<=T0)
			if(t<=a)
				fi=3*pi*t-1.4142;
				qnew=q0+qmax*(0.251+0.290*(cos(fi)+0.97*cos(2*fi)+0.47*cos(3*fi)+0.14*cos(4*fi)));
			else
				fi=3*pi*a-1.4142;
				qnew=q0+qmax*(0.251+0.290*(cos(fi)+0.97*cos(2*fi)+0.47*cos(3*fi)+0.14*cos(4*fi)));
			end
		else
			temp=t;
			while(temp>T0)
				temp=temp-T0;
			end
			if(temp<=a)
				fi=3*pi*temp-1.4142
				qnew=q0+qmax*(0.251+0.290*(cos(fi)+0.97*cos(2*fi)+0.47*cos(3*fi)+0.14*cos(4*fi)));
			else
				fi=3*pi*a-1.4142
				qnew=q0+qmax*(0.251+0.290*(cos(fi)+0.97*cos(2*fi)+0.47*cos(3*fi)+0.14*cos(4*fi)));

			end
        end
    end
	if method==5
		T = t;
        dt=dt;
		b = floor(t);
		x = T-b;
		qnew=5*(198.7*exp(-((x-0.1402)/0.0513)*(x-0.1402)/0.0513) + 163.8*exp(-((x-0.09724)/0.03729)*((x-0.09724)/0.03729)) - 1.433e+05*exp(-((x-0.3121)/0.1368)*((x-0.3121)/0.1368)) + 1.434e+05*exp(-((x-0.3119)/0.137)*((x-0.3119)/0.137)))/5000;
		qnew = qnew*1000;
    end
    if method ==6
		q0=0.0;
		qmax=500;
		a=2.0/3.0;
		T0=1.0;
		if(t<=T0)
			if(t<=a)
				fi=3*pi*t-1.4142;
				qnew=q0+qmax*(0.251+0.290*(cos(fi)+0.97*cos(2*fi)+0.47*cos(3*fi)+0.14*cos(4*fi)));
			else
				fi=3*pi*a-1.4142;
				qnew=q0+qmax*(0.251+0.290*(cos(fi)+0.97*cos(2*fi)+0.47*cos(3*fi)+0.14*cos(4*fi)));
            end
            if(t>0.3)
                qnew = -0.0995668201965449;
            end
		else
			temp=t;
			while(temp>T0)
				temp=temp-T0;
			end
			if(temp<=a)
				fi=3*pi*temp-1.4142
				qnew=q0+qmax*(0.251+0.290*(cos(fi)+0.97*cos(2*fi)+0.47*cos(3*fi)+0.14*cos(4*fi)));
			else
				fi=3*pi*a-1.4142
				qnew=q0+qmax*(0.251+0.290*(cos(fi)+0.97*cos(2*fi)+0.47*cos(3*fi)+0.14*cos(4*fi)));

            end
            if(temp>0.3)
                qnew = -0.0995668201965449;
            end
        end
    end
end