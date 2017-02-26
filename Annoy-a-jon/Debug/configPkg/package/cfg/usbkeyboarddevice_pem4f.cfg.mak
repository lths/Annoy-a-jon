# invoke SourceDir generated makefile for usbkeyboarddevice.pem4f
usbkeyboarddevice.pem4f: .libraries,usbkeyboarddevice.pem4f
.libraries,usbkeyboarddevice.pem4f: package/cfg/usbkeyboarddevice_pem4f.xdl
	$(MAKE) -f C:\Users\luke\workspace_v7\Annoy-a-jon/src/makefile.libs

clean::
	$(MAKE) -f C:\Users\luke\workspace_v7\Annoy-a-jon/src/makefile.libs clean

