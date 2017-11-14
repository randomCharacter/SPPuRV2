#Kernel moduli se prevode upotrebom Linux Kernel Build (Kbuild) okruzenja. Nakon uspesnog 
#prevodjenja u korenom (root) direktorijumu projekta treba da se pojavi .ko datoteka koja
#predstavlja prevedeni kernel modul u binarnom obliku. 

-------------------------------------------------------------------------------------------
#nothing.ko
#Prijavljivanje (registrovanje) modula u Linux kernel - insmod komanda
insmod ./nothing.ko

#Komandom lsmod omoguceno je izlistavanje svih modula koji se trenutno nalaze u kernelu.
lsmod

#Nakon izlistavanja u spisku svih modula, ukoliko je modul uspesno preveden i registrovan,
#moze se naci nothing modul. (Pomoc: lsmod | grep "nothing")

#Brisanje modula iz kernela - rmmod komanda
rmmod nothing

#Ukoliko ponovo izlistate sve module videcete da modul nothing ne postoji u listi modula.

-------------------------------------------------------------------------------------------
#hello1.ko

#Registrovanje modula.
insmod ./hello1.ko

#Proveravamo da li je modul uspesno registrovan.
lsmod

#Kako bi se omogucilo pracenje log poruka koje su definisane unutar kernel modula 
#(printk funkcija) potrebno je uraditi jedno od sledeceg:
# 1) Upotrebiti dmesg komandu
# 2) Pristupiti (sudo) posebnom uredjaju /dev/kmsg unutar OS Linux koji omogucuje
#	 pracenje log poruka.
#
#dmesg: Ova komanda omogucuje izlistavanje svih log poruka koje su se desile unutar Linux 
#		kernela. Nakon zavrsetka dmesg komande korisnicki terminal postaje ponovo dostupan
#		korisniku za poziv novih komandi - dmesg komanda nije blokirajuca. Brisanje svih 
#		log poruka se vrsi pozivom "dmesg -c".
#
#/dev/kmsg: Da bi se pristupilo log porukama pomocu uredjaja /dev/kmsg potrebno je pozvati
#			"sudo cat /dev/kmsg" komandu. Na ovaj nacin se iscitavaju samo nove poruke, tj. 
#			ne izlistavaju se sve poruke koje su bile. Za razliku od upotrebe dmesg komande
#			ovaj nacin iscitavanja je blokirajuci. Da bi se oslobodio korisnicki terminal
#			potrebno je pozvati "Ctrl + C".

#Izlistavanje kernel log-a.
dmesg

#Poslednja linija u terminalu.
Hello world 1

#Brisanje modula.
rmmod hello1

#Izlistavanje kernel log-a.
dmesg

#Poslednja linija u terminalu.
Goodbye world 1

-------------------------------------------------------------------------------------------
#hello2.ko

#Registrovanje modula.
insmod ./hello2.ko

#Proveravamo da li je modul uspesno registrovan.
lsmod

#Izlistavanje kernel log-a.
dmesg

#Poslednja linija u terminalu.
Hello world 2

#Brisanje modula.
rmmod hello2

#Izlistavanje kernel log-a.
dmesg

#Poslednja linija u terminalu.
Goodbye world 2

-------------------------------------------------------------------------------------------
#hello3.ko

#Registrovanje modula.
insmod ./hello3.ko

#Proveravamo da li je modul uspesno registrovan.
lsmod

#Izlistavanje kernel log-a.
dmesg

#Poslednja linija u terminalu.
Hello world 3

#Brisanje modula.
rmmod hello3

#Izlistavanje kernel log-a.
dmesg

#Poslednja linija u terminalu.
Goodbye world 3

-------------------------------------------------------------------------------------------
#hello4.ko

#Registrovanje modula.
insmod ./hello4.ko

#Proveravamo da li je modul uspesno registrovan.
lsmod

#Izlistavanje kernel log-a.
dmesg

#Poslednja linija u terminalu.
Hello world 4

#Izlistavanje dokumentacije modula - modinfo komanda.
modinfo ./hello4.ko

#Izlaz.
filename:       ./hello4.ko
description:    A simple driver
author:         RT-RK
license:        GPL
srcversion:     6EF93F994B70D0795902EC7
depends:       
vermagic:       3.8.0-30-generic SMP mod_unload modversions 68

#Brisanje modula.
rmmod hello4

#Izlistavanje kernel log-a.
dmesg

#Poslednja linija u terminalu.
Goodbye world 4

-------------------------------------------------------------------------------------------
#hello5.ko

#Registrovanje modula.
insmod ./hello5.ko

#Proveravamo da li je modul uspesno registrovan.
lsmod

#Izlistavanje kernel log-a.
dmesg

#Izlaz.
Hello world 5
myShort is a short integer: 1
myInt is an integer: 100
myLong is a long integer: 1000
myString is a string: initString...
myIntArray[0] = -1
myIntArray[1] = -1
Got 0 arguments for myIntArray.


#Izlistavanje dokumentacije modula - modinfo komanda.
modinfo ./hello5.ko

#Izlaz.
filename:       ./hello5.ko
description:    Demonstrates command line argument passing to a kernel module
author:         RT-RK
license:        GPL
srcversion:     C4A2BE7854E02AA2DAD4224
depends:       
vermagic:       3.8.0-30-generic SMP mod_unload modversions 686
parm:           myShort:A short integer. (short)
parm:           myInt:An integer. (int)
parm:           myLong:A long integer. (long)
parm:           myString:A character string. (charp)
parm:           myIntArray:An array of integers. (array of int)

#Brisanje modula.
rmmod hello5

#Izlistavanje kernel log-a.
dmesg

#Poslednja linija u terminalu.
Goodbye world 5

#Registrovanje modula sa parametrima.
insmod hello5.ko myShort=234 myInt=12345 myLong=9123456 myString="test" myIntArray=7,8

#Izlistavanje kernel log-a.
dmesg

#Izlaz (primetite vrednosti parametara).
Hello world 5
myShort is a short integer: 234
myInt is an integer: 12345
myLong is a long integer: 9123456
myString is a string: test
myIntArray[0] = 7
myIntArray[1] = 8
Got 2 arguments for myIntArray.

#Brisanje modula.
rmmod hello5

-------------------------------------------------------------------------------------------
#chardev.ko

#Registrovanje modula.
insmod ./chardev.ko

#Proveravamo da li je modul uspesno registrovan.
lsmod

#Izlistavanje kernel log-a.
dmesg

#Izlaz:
I was assigned major number 250. To talk to
the driver, create a dev file with
'mknod /dev/chardev c 250 0'.
Try various minor numbers. Try to cat and echo to
the device file.
Remove the device file and module when done.

#Definisanje cvora (node) novog uredjaja u OS Linux preko kog se omogucuje pristup 
#uredjaju. Pravnjenje novog cvora se vrsi upotrebom mknod komande.
mknod /dev/chardev c 250 0

#Citanje uredjaja upotrebom cat comande.
cat /dev/chardev

#Izlaz.
I already told you 0 times Hello world!

#Citanje uredjaja upotrebom cat comande.
cat /dev/chardev

#Izlaz.
I already told you 1 times Hello world!

#Pisanje u uredjaj upotrebom echo komande.
echo "test" >> /dev/chardev

#Izlistavanje kernel log-a.
dmesg

#Izlaz (poslednja linija).
Sorry, this operation isn't supported.

#Brisanje modula.
rmmod chardev

#Brisanje cvora uredjaja.
rm /dev/chardev

-------------------------------------------------------------------------------------------
#memory.ko

#Registrovanje modula.
insmod ./memory.ko

#Proveravamo da li je modul uspesno registrovan.
lsmod

#Izlistavanje kernel log-a.
dmesg

#Izlaz:
Inserting memory module

#Definisanje cvora novog uredjaja.
mknod /dev/memory c 60 0

#Pisanje u uredjaj upotrebom echo komande.
echo "test" >> /dev/memory

#Citanje uredjaja upotrebom cat comande.
cat /dev/memory

#Izlaz (korisnicki terminal).
test

#Brisanje modula.
rmmod memory

#Izlistavanje kernel log-a.
dmesg

#Izlaz:
Removing memory module

#Brisanje cvora uredjaja.
rm /dev/memory

-------------------------------------------------------------------------------------------
#memory_test_app

#Pozivanje testnog programa(potrebno je pozicionirati se u memory_test_app/bin/Release).
./memory_test_app

#Izlaz.
'/dev/memory' device is successfully opened!
Written message: '/dev/memory' test message!!!
ret_val: 30
Read message: '/dev/memory' test message!!!
ret_val: 30

-------------------------------------------------------------------------------------------
