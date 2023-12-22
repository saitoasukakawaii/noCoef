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
	 if method==6	% profile from Alastruey
        T = 1.;
        qnew = 4.0*atan(1.0)*0.0126*0.0126*(0.20617+0.37759*sin(2*pi*t/T+0.59605)  ...
         +0.2804*sin(4*pi*t/T-0.35859)+0.15337*sin(6*pi*t/T-1.2509)         ...
         -0.049889*sin(8*pi*t/T+1.3921)+0.038107*sin(10*pi*t/T-1.1068)      ...
         -0.041699*sin(12*pi*t/T+1.3985)-0.020754*sin(14*pi*t/T+0.72921)    ...
         +0.013367*sin(16*pi*t/T-1.5394)-0.021983*sin(18*pi*t/T+0.95617)    ...
         -0.013072*sin(20*pi*t/T-0.022417)+0.0037028*sin(22*pi*t/T-1.4146)  ...
         -0.013973*sin(24*pi*t/T+0.77416)-0.012423*sin(26*pi*t/T-0.46511)   ...
         +0.0040098*sin(28*pi*t/T+0.95145)-0.0059704*sin(30*pi*t/T+0.86369) ...
         -0.0073439*sin(32*pi*t/T-0.64769)+0.0037006*sin(34*pi*t/T+0.74663) ...
         -0.0032069*sin(36*pi*t/T+0.85926)-0.0048171*sin(38*pi*t/T-1.0306)  ...
         +0.0040403*sin(40*pi*t/T+0.28009)-0.0032409*sin(42*pi*t/T+1.202)   ...
         -0.0032517*sin(44*pi*t/T-0.93316)+0.0029112*sin(46*pi*t/T+0.21405) ...
         -0.0022708*sin(48*pi*t/T+1.1869)-0.0021566*sin(50*pi*t/T-1.1574)   ...
         +0.0025511*sin(52*pi*t/T-0.12915)-0.0024448*sin(54*pi*t/T+1.1185)  ...
         -0.0019032*sin(56*pi*t/T-0.99244)+0.0019476*sin(58*pi*t/T-0.059885)...
         -0.0019477*sin(60*pi*t/T+1.1655)-0.0014545*sin(62*pi*t/T-0.85829)  ...
         +0.0013979*sin(64*pi*t/T+0.042912)-0.0014305*sin(66*pi*t/T+1.2439) ...
         -0.0010775*sin(68*pi*t/T-0.79464)+0.0010368*sin(70*pi*t/T-0.0043058)...
         -0.0012162*sin(72*pi*t/T+1.211)-0.00095707*sin(74*pi*t/T-0.66203)  ...
         +0.00077733*sin(76*pi*t/T+0.25642)-0.00092407*sin(78*pi*t/T+1.3954)...
         -0.00079585*sin(80*pi*t/T-0.49973));
        qnew = qnew*1e6;
    end
    if method ==7	% profile from masri
        T = 1.;

        a0 = 0.0001124; a1 = 0.0001786; a10=-8.372e-06; a11=-9.083e-07; a12= 1.907e-06;
        a13=-6.632e-06; a14= 3.862e-06; a15=-4.594e-06; a16= 1.923e-06; a17=-1.079e-06;
        a18=-2.148e-06; a19= 2.134e-06; a2 = 0.0001209; a20=-2.424e-06; a21= 3.560e-06;
        a22=-4.968e-07; a23= 1.335e-06; a24= 1.654e-06; a25=-1.039e-06; a26= 1.961e-06;
        a27=-1.369e-06; a28= 6.939e-07; a29=-5.074e-07; a3 = 7.475e-06; a30=-5.823e-07;
        a31= 6.673e-07; a32=-4.002e-07; a33= 1.174e-06; a34=-2.676e-08; a35= 5.731e-07;
        a36= 2.481e-07; a37= 5.591e-09; a38= 2.225e-07; a39=-2.888e-07; a4 = 9.288e-06;
        a40= 1.392e-07; a41=-2.650e-07; a42= 1.963e-08; a43=-2.423e-07; a44=-4.568e-08;
        a45=-8.697e-08; a46=-1.500e-07; a47= 7.880e-08; a48=-3.559e-07; a49=-5.747e-08;
        a5 =-2.512e-05; a50=-3.524e-07; a51=-1.828e-07; a52=-5.408e-08; a53=-1.346e-07;
        a54=-3.579e-08; a55=-2.260e-08; a56= 1.724e-08; a57= 1.125e-07;
        a6 =-1.781e-05; a7 =-5.275e-06; a8 =-2.023e-05; a9 = 1.864e-06;
        
        b1 = 9.703e-05; b10=-3.295e-06; b11= 9.483e-06; b12=-4.943e-06; b13= 3.898e-06;
        b14= 1.308e-06; b15=-7.887e-07; b16= 4.199e-06; b17=-3.003e-06; b18= 4.301e-06;
        b19=-2.706e-07; b2 =-7.841e-05; b20= 2.075e-06; b21= 2.107e-06; b22=-1.486e-06;
        b23= 2.474e-06; b24=-2.221e-06; b25= 7.553e-07; b26=-6.255e-07; b27=-9.754e-07;
        b28= 7.788e-07; b29=-1.243e-06; b3 =-6.508e-05; b30= 1.232e-06; b31=-1.755e-07; 
        b32= 5.782e-07; b33= 2.466e-07; b34=-5.016e-07; b35= 2.433e-07; b36=-7.421e-07; 
        b37=-2.784e-08; b38=-5.353e-07; b39=-1.913e-07; b4 =-3.838e-05; b40=-1.914e-07;
        b41=-2.802e-07; b42=-4.436e-08; b43=-2.603e-07; b44= 9.438e-08; b45=-2.501e-07;
        b46= 7.856e-08; b47=-2.234e-07; b48=-1.422e-07; b49= 2.254e-08; b5 =-3.891e-05; 
        b50=-1.352e-07; b51= 2.759e-07; b52= 3.514e-08; b53= 1.349e-07; b54= 1.388e-07; 
        b55= 1.241e-07; b56= 1.399e-07; b6 = 3.077e-06; b7 =-1.311e-05; b8 = 2.965e-06;
        b9 = 5.842e-06;

         qnew=a0+a1*sin(2*1*pi*t/T)+b1*cos(2*1*pi*t/T)+a2*sin(2*2*pi*t/T)+b2*cos(2*2*pi*t/T)...
            +a3*sin(2*3*pi*t/T)+b3*cos(2*3*pi*t/T)+a4*sin(2*4*pi*t/T)+b4*cos(2*4*pi*t/T)...
            +a5*sin(2*5*pi*t/T)+b5*cos(2*5*pi*t/T)+a6*sin(2*6*pi*t/T)+b6*cos(2*6*pi*t/T)...
            +a7*sin(2*7*pi*t/T)+b7*cos(2*7*pi*t/T)+a8*sin(2*8*pi*t/T)+b8*cos(2*8*pi*t/T)...
            +a9*sin(2*9*pi*t/T)+b9*cos(2*9*pi*t/T)+a10*sin(2*10*pi*t/T)+b10*cos(2*10*pi*t/T)...
            +a11*sin(2*11*pi*t/T)+b11*cos(2*11*pi*t/T)+a12*sin(2*12*pi*t/T)+b12*cos(2*12*pi*t/T)...
            +a13*sin(2*13*pi*t/T)+b13*cos(2*13*pi*t/T)+a14*sin(2*14*pi*t/T)+b14*cos(2*14*pi*t/T)...
            +a15*sin(2*15*pi*t/T)+b15*cos(2*15*pi*t/T)+a16*sin(2*16*pi*t/T)+b16*cos(2*16*pi*t/T)...
            +a17*sin(2*17*pi*t/T)+b17*cos(2*17*pi*t/T)+a18*sin(2*18*pi*t/T)+b18*cos(2*18*pi*t/T)...
            +a19*sin(2*19*pi*t/T)+b19*cos(2*19*pi*t/T)+a20*sin(2*20*pi*t/T)+b20*cos(2*20*pi*t/T)...
            +a21*sin(2*21*pi*t/T)+b21*cos(2*21*pi*t/T)+a22*sin(2*22*pi*t/T)+b22*cos(2*22*pi*t/T)...
            +a23*sin(2*23*pi*t/T)+b23*cos(2*23*pi*t/T)+a24*sin(2*24*pi*t/T)+b24*cos(2*24*pi*t/T)...
            +a25*sin(2*25*pi*t/T)+b25*cos(2*25*pi*t/T)+a26*sin(2*26*pi*t/T)+b26*cos(2*26*pi*t/T)...
            +a27*sin(2*27*pi*t/T)+b27*cos(2*27*pi*t/T)+a28*sin(2*28*pi*t/T)+b28*cos(2*28*pi*t/T)...
            +a29*sin(2*29*pi*t/T)+b29*cos(2*29*pi*t/T)+a30*sin(2*30*pi*t/T)+b30*cos(2*30*pi*t/T)...
            +a31*sin(2*31*pi*t/T)+b31*cos(2*31*pi*t/T)+a32*sin(2*32*pi*t/T)+b32*cos(2*32*pi*t/T)...
            +a33*sin(2*33*pi*t/T)+b33*cos(2*33*pi*t/T)+a34*sin(2*34*pi*t/T)+b34*cos(2*34*pi*t/T)...
            +a35*sin(2*35*pi*t/T)+b35*cos(2*35*pi*t/T)+a36*sin(2*36*pi*t/T)+b36*cos(2*36*pi*t/T)...
            +a37*sin(2*37*pi*t/T)+b37*cos(2*37*pi*t/T)+a38*sin(2*38*pi*t/T)+b38*cos(2*38*pi*t/T)...
            +a39*sin(2*39*pi*t/T)+b39*cos(2*39*pi*t/T)+a40*sin(2*40*pi*t/T)+b40*cos(2*40*pi*t/T)...
            +a41*sin(2*41*pi*t/T)+b41*cos(2*41*pi*t/T)+a42*sin(2*42*pi*t/T)+b42*cos(2*42*pi*t/T)...
            +a43*sin(2*43*pi*t/T)+b43*cos(2*43*pi*t/T)+a44*sin(2*44*pi*t/T)+b44*cos(2*44*pi*t/T)...
            +a45*sin(2*45*pi*t/T)+b45*cos(2*45*pi*t/T)+a46*sin(2*46*pi*t/T)+b46*cos(2*46*pi*t/T)...
            +a47*sin(2*47*pi*t/T)+b47*cos(2*47*pi*t/T)+a48*sin(2*48*pi*t/T)+b48*cos(2*48*pi*t/T)...
            +a49*sin(2*49*pi*t/T)+b49*cos(2*49*pi*t/T)+a50*sin(2*50*pi*t/T)+b50*cos(2*50*pi*t/T)...
            +a51*sin(2*51*pi*t/T)+b51*cos(2*51*pi*t/T)+a52*sin(2*52*pi*t/T)+b52*cos(2*52*pi*t/T)...
            +a53*sin(2*53*pi*t/T)+b53*cos(2*53*pi*t/T)+a54*sin(2*54*pi*t/T)+b54*cos(2*54*pi*t/T)...
            +a55*sin(2*55*pi*t/T)+b55*cos(2*55*pi*t/T)+a56*sin(2*56*pi*t/T)+b56*cos(2*56*pi*t/T)...
            +a57*sin(2*57*pi*t/T);
         qnew = qnew*1e6;
    end
    % from jcj single peak
	if method ==8
		q0=0.0;
		qmax=500;
		a=2.0/3.0;
		T0=1.0;
		if(t<=T0)
			if(t<=a)
                if(t<0.3)
				    fi=3*pi*t-1.4142;
				    qnew=q0+qmax*(0.251+0.290*(cos(fi)+0.97*cos(2*fi)+0.47*cos(3*fi)+0.14*cos(4*fi)));
                else
                    qnew=-0.0995668201965449;
                end
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
                if(temp<0.3)
				    fi=3*pi*temp-1.4142
				    qnew=q0+qmax*(0.251+0.290*(cos(fi)+0.97*cos(2*fi)+0.47*cos(3*fi)+0.14*cos(4*fi)));
                else
                    qnew=-0.0995668201965449;
                end
			else
				fi=3*pi*a-1.4142
				qnew=q0+qmax*(0.251+0.290*(cos(fi)+0.97*cos(2*fi)+0.47*cos(3*fi)+0.14*cos(4*fi)));

			end
        end
    end
end