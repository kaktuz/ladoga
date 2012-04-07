
EXTRA_CFLAGS = -DCONFIG_LADOGA_DEBUG

#obj-$(CONFIG_IPW2100) += ipw2100.o
obj-m := ladoga.o

#libipw-objs :=
#	libipw_module.o
#	libipw_tx.o
#	libipw_rx.o
#	libipw_wx.o
#	libipw_geo.o


all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
